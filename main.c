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
#include <math.h>
//* ========================================
#include "defines.h"
#include "vars.h"
//* ========================================
void Test1();
void Test2();
void Test3();
void Test4();
void Test5();
void Test5();
void usbPutString(char *s);
void usbPutChar(char c);
void handle_usb();
//* ========================================

CY_ISR(isrRF_RX)
{
    uint8 c;
    c = UART_GetChar();
    USBUART_PutChar(c);
}

int main()
{
    

// --------------------------------    
// ----- INITIALIZATIONS ----------
    CYGlobalIntEnable;
    PWM_1_Start();
    USBUART_Start(0,USBUART_5V_OPERATION);
    UART_Start();
    isrRF_RX_StartEx(isrRF_RX);
    ADC_Start();
    ADC_StartConvert();
    

// ------USB SETUP ----------------    
#ifdef USE_USB    
    USBUART_Start(0,USBUART_5V_OPERATION);
#endif        
        
    RF_BT_SELECT_Write(0);

    usbPutString(displaystring);
    for(;;)
    {
		//Get switch information
		
		
		
		//Choose mode depending on switch position
		
		//Open loop-straight line test
		if (sw1 = 0, sw2 = 0, sw3 = 1) //TEMP PSEUDO CODE
		{
			Test1();
		}
		
		//Straight line-with curve test
		if (sw1 = 0, sw2 = 1, sw3 = 0) //TEMP PSEUDO CODE
		{
			Test2();
		}
		
		//Curve tracking test
		if (sw1 = 0, sw2 = 1, sw3 = 1) //TEMP PSEUDO CODE
		{
			Test3();
		}
		
		//Turn 90degrees test
		if (sw1 = 1, sw2 = 0, sw3 = 0) //TEMP PSEUDO CODE
		{
			Test4();
		}
		
		//ADC Battery read test
		if (sw1 = 1, sw2 = 0, sw3 = 1) //TEMP PSEUDO CODE
		{
			Test5();
		}
		
		//RF recieve test
		if (sw1 = 1, sw2 = 1, sw3 = 0) //TEMP PSEUDO CODE
		{
			//Test6();
			handle_usb();
			if (flag_KB_string == 1)
			{
				usbPutString(line);
				flag_KB_string = 0;
			}
		}
		
        
		/*
		Putty for recieving data
		
        handle_usb();
        if (flag_KB_string == 1)
        {
            usbPutString(line);
            flag_KB_string = 0;
        }    
		*/
    }   
}



//* ========================================
void Test1()
{
	Go straight
}
//* ========================================
//Straight line-with curve test
void Test2()
{
	//Get sensor results
	int16 ADC_S1 = ADC_GetResult16(0);
	int16 ADC_S2 = ADC_GetResult16(1);
	
	int8 tLSensor = (ADC_S1 / 1000) % 10;
	int8 tRSensor = (ADC_S2 / 1000) % 10;
	
	if (tLSensor >= 32) {
		turn right a LITTLE bit;
		CyDelay(5);
	} else if (tRSensor >= 32) {
		turn left a  LITTLE bit;
		CyDelay(5);
	} else {
		go straight;
		CyDelay(30);
	}
	
	if (tLSensor >= 32 && tRSensor >= 32) {
		stop;
		CyDelay(10000);
	}
}
//* ========================================
//Curve tracking test
void Test3()
{
	//Get sensor results
	int16 ADC_S1 = ADC_GetResult16(0);
	int16 ADC_S2 = ADC_GetResult16(1);
	int16 ADC_S5 = ADC_GetResult16(4);
	
	int8 tLSensor = (ADC_S1 / 1000) % 10;
	int8 tRSensor = (ADC_S2 / 1000) % 10;
	int8 mSensor = (ADC_S5 / 1000) % 10;
	
	if (tLSensor >= 32) {
		turn right a LITTLE bit;
		CyDelay(5);
	} else if (tRSensor >= 32) {
		turn left a  LITTLE bit;
		CyDelay(5);
	} else {
		go straight;
		CyDelay(30);
	}
	
	if (tLSensor >= 32 && tRSensor >= 32 && mSensor >= 32) {
		stop;
		CyDelay(10000);
	}
}
//* ========================================
//Turn 90degrees test
void Test4()
{
	//Get sensor results
	int16 ADC_S1 = ADC_GetResult16(0);
	int16 ADC_S2 = ADC_GetResult16(1);
	int16 ADC_S3 = ADC_GetResult16(2);
	int16 ADC_S4 = ADC_GetResult16(3);
	int16 ADC_S5 = ADC_GetResult16(4);
	
	int8 tLSensor = (ADC_S1 / 1000) % 10;
	int8 tRSensor = (ADC_S2 / 1000) % 10;
	int8 mLSensor = (ADC_S3 / 1000) % 10;
	int8 mRSensor = (ADC_S4 / 1000) % 10;
	int8 mSensor = (ADC_S5 / 1000) % 10;
	
	if (tLSensor <= 31 && tRSensor <= 31 && mSensor <= 31 && mLSensor >= 32 && mRSensor >= 32) {
		go straight;
		CyDelay(20);
	} else if (tLSensor >= 32 && tRSensor >= 32 && mSensor <= 31 && mLSensor <= 31 && mRSensor >= 32) {
		turn left 90 degrees;
		CyDelay(20);
	} else if (tLSensor >= 32 && tRSensor >= 32 && mSensor <= 31 && mLSensor >= 32 && mRSensor <= 31) {
		turn right 90 degrees;
		CyDelay(20);
	} else {
		stop;
		CyDelay(10000);
	}
	
}
//* ========================================
void Test5()
{
	int16 ADC_5VIN = ADC_GetResult16(6) / 0.85;
	
	int8 digit1 = ADC_5VIN / 10000;
	int8 digit2 = (ADC_5VIN / 1000) % 10;
	int8 digit3 = ((ADC_5VIN / 100) % 100) % 10 ;
	int8 digit4 = (ADC_5VIN % 100) / 10;
	int8 digit5 = ADC_5VIN % 10;
	usbPutChar(digit1 + 48);
	usbPutChar(digit2 + 48);
	usbPutChar('.');
	usbPutChar(digit3 + 48);
	usbPutChar(digit4 + 48);
	usbPutChar(digit5 + 48);
	usbPutChar('V');
	usbPutChar('\n');
}
//* ========================================
void Test6()
{
	
}



//* ========================================
void usbPutString(char *s)
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
//* ========================================
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
}


/* [] END OF FILE */
