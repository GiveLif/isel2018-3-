#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void blink_raya(void){
    GPIO_OUTPUT_SET(2, 0);  //on
    vTaskDelay(3*250/portTICK_RATE_MS);
    GPIO_OUTPUT_SET(2, 1);  //off
    vTaskDelay(250/portTICK_RATE_MS);
}

void blink_punto(void){
    GPIO_OUTPUT_SET(2, 0);  //on
    vTaskDelay(250/portTICK_RATE_MS);
    GPIO_OUTPUT_SET(2, 1);  //off
    vTaskDelay(250/portTICK_RATE_MS);
}

void blink_espacio(void){
    GPIO_OUTPUT_SET(2, 1);  //off
    vTaskDelay(250/portTICK_RATE_MS);
}

const char* morse (int c){
  static const char* morse_ch[] = { //letras mas 3 espacios
    ".-   ", //"A"
    "-...   ", //"B"
    "-.-.   ", //"C"
    "-..   ", // "D"
    ".   ", // "E"
    "..-.   ", // "F"
    "--.   ", //"G"
    "....   ", //"H"
    "..   ", //"I"
    ".---   ", //"J"
    "-.-   ", //"K"
    ".-..   ", //"L"
    "--   ", //"M"
    "-.   ", //"N"
    "---   ", //"O"
    ".--.   ", //"P"
    "--.-   ", //"Q"
    ".-.   ", //"R"
    "...   ", //"S"
    "-   ", //"T"
    "..-   ", //"U"
    "...-   ", //"V"
    ".--   ", //"W"
    "-..-   ", //"X"
    "-.--   ", //"Y"
    "--..   "//"Z"
  };
  if (c==' ')
    return "  "; //+2 espacios tras una palabra para hacer un espacio
  if((c>='a')&&(c<='z'))
    return morse_ch[c-'a'];
  if((c<'a')||(c>'z'))
    return "";
}

int str2morse(char* buf, const char* str, int n){
  while(*str && n){
    const char* c = morse(*str);
    int len = strlen(c);
    strncpy(buf,c,n);
    n-=len;
    buf+=len;
    ++str;
  }
}

void msg_morse (void* ignore){
  char msg[256];
  str2morse(msg, "hola mundo", 256);
  int p = 0;
  while(true){
    if (msg[p]=='-')
      blink_raya();
     if (msg[p]=='.')
       blink_punto();
     if (msg[p]==' ')
       blink_espacio();
     p++;
  }
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    //xTaskCreate(&task_blink, "startup", 2048, NULL, 1, NULL);
    xTaskCreate(&msg_morse, "startup", 2048, NULL, 1, NULL);
}
