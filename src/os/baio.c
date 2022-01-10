#include <os/baio.h>
#include <printf.h>
#include <video.h>
#include <uart.h>
#include <string.h>
#include <timer.h>
#include <mem.h>
#include <mailbox.h>

char *inputBuffer;
char**textbuffer;
u16 charW = 100;
u16 charH = 60;

bool mdebug = false;
bool mvideo = false;


u0 video_drawLine(char *str){
    video_clean();
    for (u16 i = 1; i < charH; i++) {
        strcpy(textbuffer[i-1], textbuffer[i]);
    }
    strcpy(textbuffer[charH-1], str);

    for (u16 i = 0; i < charH; i++) {
        video_drawString(textbuffer[i], 0, i*fontGetHeight());
    }
}

u0 video_drawInline(char *str){
    strcpy(textbuffer[charH-1], str);

    video_drawString(textbuffer[charH-1], 0, (charH-1)*fontGetHeight());
}

u0 video_draw(char c){
    strput(textbuffer[charH-1], c);

    video_drawString(textbuffer[charH-1], 0, (charH-1)*fontGetHeight());
}

u0 baio_scanCmd(){
    char c=0;
    do{
        c = uart_getc();
        if(mdebug) printf("~ uart recv %c\n", c);
        if((c >= ' ' && c <= '~') || c > 0x7F){ // writable
            strput(inputBuffer, c);
            if(mvideo){
                video_draw(c);
                video_dma();
            } else {
                printf("%c", c);
            }
        } else if (c == 0x7F){ // del
            inputBuffer[strlen(inputBuffer)-1] = 0;
            if(mvideo){
                char s[255];
                strcpy(s, "Write cmd > ");
                strcat(s, inputBuffer);
                video_drawInline(s);
                
                for (u16 i = 0; i < charH; i++){
                    video_drawString(textbuffer[i], 0, i*fontGetHeight());
                }
                video_dma;
            } else {
                printf("\b \b");
            }
        } else if (c == 0xD){ // enter cmd
            return;
        } else {
            if(mvideo){
                char s[charW];
                char buff[255];

                video_drawLine("");

                strcpy(s, "Bad input : 0x");
                strcat(s, itoa(c, buff, 16));
                video_drawLine(s);

                strcpy(s, "Write cmd > ");
                strcat(s, inputBuffer);
                video_drawInline(s);

                video_dma();
            } else {
                printf("\n~ bad input : 0x%X\n", c);
                printf("Write cmd > %s", inputBuffer);
            }
        }
    } while(c != '\n');
}

u0 baio_cmd(){
    if(strcmp(inputBuffer, "?") == 0 || strcmp(inputBuffer, "help") == 0){
        if(mvideo){
            video_clean();
            video_drawLine("Basic AmethystOS Input Output Commands:");
            video_drawLine(" -  help - ?        | Display this");
            video_drawLine(" -  temp            | Get system temperature");
            video_drawLine(" -  version - ver   | Display system version");

            video_dma();
        } else {
            printf("Basic AmethystOS Input Output Commands:\n");
            printf(" -  help - ?        | Display this\n");
            printf(" -  temp            | Get system temperature\n");
            printf(" -  version - ver   | Display system version\n");
        }

    } else if(strcmp(inputBuffer, "temp") == 0){
        u32 maxtemp = 0;
        mailbox_genericCommand(RPI_FIRMWARE_GET_MAX_TEMPERATURE, 0, &maxtemp);
        u32 curtemp = 0;
        mailbox_genericCommand(RPI_FIRMWARE_GET_TEMPERATURE, 0, &curtemp);
        if(mvideo){
            char s[charW];
            char buff[255];
            strcpy(s, "System temperature:         ");
            strcat(s, itoa(curtemp, buff, 10));
            strcat(s, " C");
            video_drawLine(s);

            strcpy(s, "System maximal temperature: ");
            strcat(s, itoa(maxtemp, buff, 10));
            strcat(s, " C");
            video_drawLine(s);

            video_dma();
        } else {
            printf("System temperature:         %d C\n", curtemp);
            printf("System maximal temperature: %d C\n", maxtemp);
        }

    } else if(strcmp(inputBuffer, "version") == 0 || strcmp(inputBuffer, "ver") == 0){
        if(mvideo){
            char s[charW];
            char buff[255];
            strcpy(s, "AmethystOS version ");
            strcat(s, itoa(VER_MAJOR, buff, 10));
            strcat(s, ".");
            strcat(s, itoa(VER_MINOR, buff, 10));
            strcat(s, ".");
            strcat(s, itoa(VER_PATCH, buff, 10));
            video_drawLine(s);
            video_drawLine("** AmethystOS is made by Alikadev");
            video_dma();
        } else {
            printf("AmethystOS version %d.%d.%d\n** AmethystOS is made by Alikadev\n", VER_MAJOR, VER_MINOR, VER_PATCH);
        }

    } else {
        if(mvideo){
            char s[charW];
            strcpy(s, "Unrecognized commands : ");
            strcat(s, inputBuffer);
            video_drawLine(s);
            video_drawLine("Please write 'help' for get system commands...");
            video_dma();
        } else {
            printf("Unrecognized commands : %s\nPlease write 'help' for get system commands...\n", inputBuffer);
        }
    }
}

u0 baio_exec() {
    bool end=false;
    do{
        if(mvideo){
            video_drawLine("");
            video_drawInline("Write cmd > ");
            video_dma();
        } else {
            printf("\nWrite cmd > ");
        }
        baio_scanCmd();
        if(mvideo){
            video_clean();
            video_drawLine("");
            video_dma();
        } else {
            printf("\n");
        }
        baio_cmd();
        inputBuffer[0]=0;
    }while(!end);
}

u0 baio_play(char mode){
    /// check mode
    if(mode & BAIO_DEBUG){
        mdebug = true;
        printf("~ debug mode : on");
    }

    if(mode & BAIO_VIDEO){
        printf("~ video mode is not already implemented...\n");
        mvideo = true;
    }


    // ~-~- init
    if(mdebug) printf("~ Initializing...\n");

    void *temp = getFreePages(5);

    // get video size
    if(mvideo) {
        video_clean();
        video_dma();
        charW = video_getResolutionX() / (fontGetWidth()+2);
        charH = video_getResolutionH() / fontGetHeight();
        if(mdebug) printf("~ video is %dx%d char\n", charW, charH);

        textbuffer = malloc(sizeof(*textbuffer) * charH);
        for(u32 i=0; i<charH; i++){
            textbuffer[i] = malloc(sizeof(char)*charW);
        }
        if(mdebug) printf("~ malloc textbuffer at 0x%X\n", textbuffer);

        for (u32 i = 0; i < charH; i++) {
            textbuffer[i][0] = '\0';
        }
        if(mdebug) printf("~ set all textbuffer cells at \\0\n");

    }
    
    inputBuffer = malloc(255);
    inputBuffer[0]=0;
    
    if(mdebug) printf("~ Initialized!\n\n");
    

    baio_exec();


    free(inputBuffer);
    for(u32 i=0; i<charH; i++){
        free(textbuffer[i]);
    }
    free(textbuffer);
}