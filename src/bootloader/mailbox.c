#include "mailbox.h"
#include "peripherals/base.h"
#include "printf.h"
#include "mem.h"

typedef struct
{
    reg32 read;
    reg32 res[5];
    reg32 status;
    reg32 config;
    reg32 write;
} mailbox_regs;

mailbox_regs *MBX() {
    return (mailbox_regs *)(PBASE + 0xB880);
}

typedef struct {
    u32 size;
    u32 code;
    u8 tag[0];
} propretyBuffer;

static u32 propretyData[8192] __attribute__((aligned(16)));


#define MAIL_EMPTY 0x40000000
#define MAIL_FULL  0x80000000

#define MAIL_POWER    0x0 // Mailbox Channel 0: Power Management Interface
#define MAIL_FB       0x1 // Mailbox Channel 1: Frame Buffer
#define MAIL_VUART    0x2 // Mailbox Channel 2: Virtual UART
#define MAIL_VCHIQ    0x3 // Mailbox Channel 3: VCHIQ Interface
#define MAIL_LEDS     0x4 // Mailbox Channel 4: LEDs Interface
#define MAIL_BUTTONS  0x5 // Mailbox Channel 5: Buttons Interface
#define MAIL_TOUCH    0x6 // Mailbox Channel 6: Touchscreen Interface
#define MAIL_COUNT    0x7 // Mailbox Channel 7: Counter
#define MAIL_TAGS     0x8 // Mailbox Channel 8: Tags (ARM to VC)

static u0 mailbox_write(u8 channel, u32 data){
    while(MBX()->status & MAIL_FULL) ;

    MBX()->write = ((data & 0xFFFFFFF0) | (channel & 0xF));
}

static u32 mailbox_read(u8 channel){
    while(true){
        while(MBX()->status & MAIL_EMPTY);

        u32 data = MBX()->read;

        u8 readChannel = (u8)(data & 0xF);

        if(readChannel == channel){
            return data & 0xFFFFFFF0;
        }
    }
}

bool mailbox_process(mailbox_Tag *tag, u32 tagSize){
    u32 bufferSize = tagSize + 12;

    memcpy(&propretyData[2], tag, tagSize);

    propretyBuffer *buff = (propretyBuffer *)propretyData;
    buff->size = bufferSize;
    buff->code = RPI_FIRMWARE_STATUS_REQUEST;
    propretyData[(tagSize + 12) / 4 - 1] = RPI_FIRMWARE_PROPERTY_END;
    
    mailbox_write(MAIL_TAGS, (u32)(void*) propretyData);

    mailbox_read(MAIL_TAGS);

    memcpy(tag, propretyData + 2, tagSize);

    return true;
}

bool mailbox_genericCommand(u32 tagId, u32 id, u32 *value){
    mailbox_Generic mbx;
    mbx.tag.id = tagId;
    mbx.tag.valueLength = 0;
    mbx.tag.bufferSize = sizeof(mailbox_Generic) - sizeof(mailbox_Tag);
    mbx.id = id;
    mbx.value = *value;

    if(!mailbox_process((mailbox_Tag *)&mbx, sizeof(mbx))) {
        printf("FAILED TO PROCESS: %X\n", tagId);
        return false;
    }

    *value = mbx.value;
    return true;
}

u32 mailbox_clockRate(ClockType ct) {
    mailbox_Clock c;
    c.tag.id = RPI_FIRMWARE_GET_CLOCK_RATE;
    c.tag.valueLength = 0;
    c.tag.bufferSize = sizeof(c) - sizeof(c.tag);
    c.id = ct;

    mailbox_process((mailbox_Tag *)&c, sizeof(c));
    return c.rate;
}

bool mailbox_powerCheck(u32 type) {
    mailbox_Power p;
    p.tag.id = RPI_FIRMWARE_GET_DOMAIN_STATE;
    p.tag.valueLength = 0;
    p.tag.bufferSize = sizeof(p) - sizeof(p.tag);
    p.id = type;
    p.state = ~0;

    mailbox_process((mailbox_Tag *)&p, sizeof(p));

    return p.state && (p.state != ~0);
}
