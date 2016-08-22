/* ========================================
 * Fully working code: 
 * PWM      : 
 * Encoder  : 
 * ADC      :
 * USB      : port displays speed and position.
 * CMD: "PW xx"
 * Copyright Univ of Auckland, 2016
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Univ of Auckland.
 *
 * ========================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <project.h>
//* ========================================
#include "defines.h"
#include "vars.h"
//* ========================================
void usbPutString(char *s);;
void usbPutChar(char c);
void handle_usb();
//* ========================================

void driveStraight()
{
    int master,slave;
    int error= 0;
    int kp = 10;
 
    QuadDec_M1_SetCounter(0);
    QuadDec_M2_SetCounter(0);
    
    master = ((1/PWM_1_ReadPeriod())*60)/QuadDec_M1_GetCounter();
    slave = ((1/PWM_2_ReadPeriod())*60)/QuadDec_M2_GetCounter();
    
    error = 1/((master - slave)/kp);
    PWM_2_WriteCompare(51-error);
    
    QuadDec_M1_SetCounter(0);
    QuadDec_M2_SetCounter(0);
    
    CyDelay(10000);
}
void writeCompare(int P1, int P2)
{
    PWM_1_WriteCompare(P1);
    PWM_2_WriteCompare(P2);
}

int main()
{   
    PWM_2_Start();
    PWM_1_Start();
    
    /*QuadDec_M1_Start();
    QuadDec_M2_Start();
    
    writeCompare(204,51);
    CyDelay(400);
    
    writeCompare(0,127);
    CyDelay (1000);
    
    writeCompare(204,51);
    CyDelay(445);
    
    writeCompare(0,127);
    CyDelay(1000);*/
    
// --------------------------------    
// ----- INITIALIZATIONS ----------
    CYGlobalIntEnable;
}

// ------USB SETUP ----------------    
/*#ifdef USE_USB    
    USBUART_Start(0,USBUART_5V_OPERATION);
#endif        
        
    RF_BT_SELECT_Write(0);
+
    usbPutString(displaystring);
    for(;;)
    {
        /* Place your application code here. */
        /*handle_usb();
        if (flag_KB_string == 1)
        {
            usbPutString(line);
            flag_KB_string = 0;
        }        
    }   
} */
//========================================
/*void usbPutString(char *s)
{
// !! Assumes that *s is a string with allocated space >=64 chars     
//  Since USB implementation retricts data packets to 64 chars, this function truncates the
//  length to 62 char (63rd char is a '!')

#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    s[63]='\0';
    s[62]='!';
    USBUART_PutData((uint8*)s,strlen(s));
#endif
}
/* ========================================
void usbPutChar(char c)
{
#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    USBUART_PutChar(c);
#endif    
}
//* ========================================
void handle_usb()
{
    // handles input at terminal, echos it back to the terminal
    // turn echo OFF, key emulation: only CR
    // entered string is made available in 'line' and 'flag_KB_string' is set
    
    static uint8 usbStarted = FALSE;
    static uint16 usbBufCount = 0;
    uint8 c; 
    

    if (!usbStarted)
    {
        if (USBUART_GetConfiguration())
        {
            USBUART_CDC_Init();
            usbStarted = TRUE;
        }
    }
    else
    {
        if (USBUART_DataIsReady() != 0)
        {  
            c = USBUART_GetChar();

            if ((c == 13) || (c == 10))
            {
//                if (usbBufCount > 0)
                {
                    entry[usbBufCount]= '\0';
                    strcpy(line,entry);
                    usbBufCount = 0;
                    flag_KB_string = 1;
                }
            }
            else 
            {
                if (((c == CHAR_BACKSP) || (c == CHAR_DEL) ) && (usbBufCount > 0) )
                    usbBufCount--;
                else
                {
                    if (usbBufCount > (BUF_SIZE-2) ) // one less else strtok triggers a crash
                    {
                       USBUART_PutChar('!');        
                    }
                    else
                        entry[usbBufCount++] = c;  
                }  
            }
        }
    }    
}*/


/* [] END OF FILE */
