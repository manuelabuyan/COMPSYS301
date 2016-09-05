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
void writeCompare(int P1, int P2);
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
    USBUART_Start(0,USBUART_5V_OPERATION);
    UART_Start();
    isrRF_RX_StartEx(isrRF_RX);
    ADC_Start();
    ADC_StartConvert();
    

// -----USB SETUP -----------------    
 /*
#ifdef USE_USB    
    USBUART_Start(0,USBUART_5V_OPERATION);
#endif        
        
    RF_BT_SELECT_Write(0);

    usbPutString(displaystring);
    
    */
//------RUN LOOP-------------------
    for(;;)
    {
		//Get switch information
		uint8 sw1 = DB0_Read();
        uint8 sw2 = DB1_Read();
        uint8 sw3 = DB2_Read();
        
        PWM_1_Start();
        PWM_2_Start();
       
       
		//Choose mode depending on switch position
		
		//Open loop-straight line test
		if (sw1 == 0 && sw2 == 0 && sw3 == 1) 
		{
			Test1();
		}
		//Straight line-with curve test
		else if (sw1 == 0 && sw2 == 1 && sw3 == 0) 
		{
			Test2();
		}
		//Curve tracking test
		else if (sw1 == 0 && sw2 == 1 && sw3 == 1)
		{
			Test3();
		}
		//Turn 90degrees test
		else if (sw1 == 1 && sw2 == 0 && sw3 == 0)
		{
			Test4();
		}
		//ADC Battery read test
		else if (sw1 == 1 && sw2 == 0 && sw3 == 1)
		{
			Test5();
		}
		//RF recieve test
		else if (sw1 == 1 && sw2 == 1 && sw3 == 0)
		{
            writeCompare(127,127);
			handle_usb();
			if (flag_KB_string == 1)
			{
				usbPutString(line);
				flag_KB_string = 0;
			}
		}
        else
        {
            writeCompare(127,127);
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
//Open loop-straight line test
void Test1()
{
    int16 ADC_S1 = ADC_GetResult16(0) / 0.85;
	int16 ADC_S2 = ADC_GetResult16(1) / 0.85;
	
	int8 tLSensor = (ADC_S1 / 1000) % 10;
	int8 tRSensor = (ADC_S2 / 1000) % 10;
    
    if (tLSensor >= 2) {
		writeCompare(89,89);
		CyDelay(50);
	} else {
        writeCompare(127,127);
    }
}
//* ========================================
//Straight line-with curve test
void Test2()
{
	//Get sensor results
	int16 ADC_S1 = ADC_GetResult16(0) / 0.85;
	int16 ADC_S2 = ADC_GetResult16(1) / 0.85;
    int16 ADC_S5 = ADC_GetResult16(4) / 0.85;
	
	int8 tLSensor = (ADC_S1 / 1000) % 10;
	int8 tRSensor = (ADC_S2 / 1000) % 10;
    int8 mSensor = (ADC_S5 / 1000) % 10;

	if (tLSensor >= 1 && tRSensor <1) {
		writeCompare(89,135);
		CyDelay(80);
        writeCompare(75,75);
        CyDelay(80);
	} 
    else if (tRSensor >= 1 && tLSensor <1) {
		writeCompare(135,89);
		CyDelay(80);
        writeCompare(75,75);
        CyDelay(80);
	} 
    else if (tLSensor < 1 && tRSensor <1){
		writeCompare(75,75);
		CyDelay(100);
	}
	
	else{
        writeCompare(127,127);
		CyDelay(100);
	}

}
//* ========================================
//Curve tracking test
void Test3()
{
	//Get sensor results
	int16 ADC_S1 = ADC_GetResult16(0) / 0.85;
	int16 ADC_S2 = ADC_GetResult16(1) / 0.85;
    int16 ADC_S5 = ADC_GetResult16(4) / 0.85;
	
	int8 tLSensor = (ADC_S1 / 1000) % 10;
	int8 tRSensor = (ADC_S2 / 1000) % 10;
    int8 mSensor = (ADC_S5 / 1000) % 10;

	if (tLSensor >= 2 && tRSensor <=1) {
		writeCompare(80,127);
		CyDelay(50);

	} 
    else if (tRSensor >= 2 && tLSensor <=1) {
		writeCompare(127,80);
		CyDelay(50);
	} 
    else if (tLSensor <=1 && tRSensor <=1){
		writeCompare(80,80);
		CyDelay(50);
	}
	
	else{
        writeCompare(127,127);
		CyDelay(50);
	}
}
//* ========================================
//Turn 90degrees test
int isOnLight(int8 numSensor)
{
    int32 ADC;
    int16 i;
    for(i = 0; i < 50; i++)
    {
        while(!ADC_IsEndConversion(0)){}
        ADC = ADC_CountsTo_mVolts(ADC_GetResult16(numSensor));
        if(ADC > 1000)
        {
            return 1;
        }
    }
    return 0;
}

void Test4()
{
    int8 tLSensor = isOnLight(0);
	int8 tRSensor = isOnLight(1);
	int8 mLSensor = isOnLight(2);
	int8 mRSensor = isOnLight(3);
	int8 mSensor = isOnLight(4);
    int8 mBSensor = isOnLight(5);
    
    if (((!tLSensor && !tRSensor)))
    {
		writeCompare(85,85); //Go straight
		CyDelay(100);
    }
    else if (((tLSensor && tRSensor && mLSensor && mRSensor)))
    {
		writeCompare(89,89); //Go straight
		CyDelay(100);
    } 
    else if (tLSensor && !tRSensor && mLSensor && mRSensor) 
    {
		writeCompare(89,135); //Turn right a bit
		CyDelay(10);
	} 
    else if (!tLSensor && tRSensor && mLSensor && mRSensor)
    {
		writeCompare(135,89); //Turn left a bit
		CyDelay(10);
	}  
    else if (!mLSensor) {
        while (tLSensor) 
        {
            tLSensor = isOnLight(0);
            writeCompare(170,65); //Turn left
        }
	} else if (!mRSensor) {
        while (tRSensor ) 
        {
            tRSensor = isOnLight(1);
            writeCompare(65,170); //Turn left
        }
    }
    else {
		writeCompare(127,127);
		CyDelay(1000);
    }
  
   
    /*
	//Get sensor results
	int16 ADC_S1 = ADC_GetResult16(0);
	int16 ADC_S2 = ADC_GetResult16(1);
	int16 ADC_S3 = ADC_GetResult16(2);
	int16 ADC_S4 = ADC_GetResult16(3);
	int16 ADC_S5 = ADC_GetResult16(4);
    int16 ADC_S6 = ADC_GetResult16(5);
	
	int8 tLSensor = (ADC_S1 / 1000) % 10;
	int8 tRSensor = (ADC_S2 / 1000) % 10;
	int8 mLSensor = (ADC_S3 / 1000) % 10;
	int8 mRSensor = (ADC_S4 / 1000) % 10;
	int8 mSensor = (ADC_S5 / 1000) % 10;
    int8 mBSensor = (ADC_S6 / 1000) % 10;
    
	if (tLSensor <= 1 && tRSensor <= 1 && mSensor <= 1 && mLSensor >= 2 && mRSensor >= 2) {
		writeCompare(89,89); //Go straight
		CyDelay(200);
    } else if (tLSensor >= 2 && tRSensor <= 1 && mSensor <= 1 && mLSensor >= 2 && mRSensor >= 2) {
		writeCompare(89,135); //Turn right a bit
		CyDelay(80);
	} else if (tLSensor <= 1 && tRSensor >= 2 && mSensor <= 1 && mLSensor >= 2 && mRSensor >= 2) {
		writeCompare(135,89); //Turn left a bit
		CyDelay(80);
	} else if (tLSensor >= 2 && tRSensor >= 2 && mSensor <= 1 && mLSensor >= 2 && mRSensor >= 2) {
		writeCompare(89,89); //Go straight-faster ticks
		CyDelay(200);
	} else if (tLSensor >= 2 && tRSensor >= 2 && mLSensor >= 2 && mRSensor <= 1) {
        while (tLSensor >= 2) {
            ADC_S1 = ADC_GetResult16(0);
            tLSensor = (ADC_S1 / 1000) % 10;
            writeCompare(165,90); //Turn right
        }
	} else if (tLSensor >= 2 && tRSensor >= 2 && mLSensor <= 1 && mRSensor >= 2) { 
        while (tRSensor >= 2) {
            ADC_S2 = ADC_GetResult16(1);
            tRSensor = (ADC_S2 / 1000) % 10;
            writeCompare(90,165); //Turn right
        }
    } else {
		writeCompare(127,127);
		CyDelay(1000);
    }

            
    */
    
	
}


//* ========================================
//ADC Battery read test
void Test5()
{
    writeCompare(127,127);
    
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
//Write compare values of PWMs
void writeCompare(int P1, int P2)
{
    PWM_1_WriteCompare(P1);
    PWM_2_WriteCompare(P2);
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
