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
void pacMan();
void opticalStraight();
void readBattery();
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
    PWM_1_Start();
    PWM_2_Start();
    QuadDec_M1_Start();
    QuadDec_M2_Start();

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
		uint8 sw1 = !DB0_Read();
        uint8 sw2 = DB2_Read();
        uint8 sw3 = DB1_Read();
        
		//Choose mode depending on switch position
		if (!sw1 && !sw2 && !sw3)
		{
			pacMan();
		}
        else if (sw1 && sw2 && sw3)
		{
			opticalStraight();
		}
		//ADC Battery read test
		else if (sw1 && !sw2 && sw3)
		{
			readBattery();
		}
		//RF recieve test
		else if (sw1 && sw2 && !sw3)
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
//Calculates the sensors' status
void computeSensor(uint8 start, uint8 end)
{
    int32 ADC[6];
    uint8 j;
    uint8 i;
    for(i = start; i <= end; i++)
    {
        sensors[i] = 0;
    }
    for(i = 0; i < 55; i++)
    {
        while(!ADC_IsEndConversion(0)){}
        for(j = start; j <= end; j++)
        {
            ADC[j] = ADC_CountsTo_mVolts(ADC_GetResult16(j));
            if(ADC[j] > 1000)
            {
                sensors[j] = 1;
            }
        }
    }
}

void pacMan()
{
    computeSensor(0,5);
    if (((!sensors[FL] && !sensors[FR])))
    {
		writeCompare(78,78); //Go straight
    }
    else if (((!sensors[MM] || !sensors[BM]) && (sensors[FL] && sensors[FR] && sensors[ML] && sensors[MR])))
    {
		writeCompare(78,78); //Go straight
    } 
    else if (sensors[FL] && !sensors[FR] && sensors[ML] && sensors[MR]) 
    {
		writeCompare(70,78); //Turn right a bit
		//CyDelay(5);
	} 
    else if (!sensors[FL] && sensors[FR] && sensors[ML] && sensors[MR])
    {
		writeCompare(78,70); //Turn left a bit
		//CyDelay(5);
	}  
    else if (!sensors[ML]) {
        while (sensors[FR]) 
        {
            computeSensor(1,1);
            writeCompare(170,83); //Turn left
        }
	} 
    else if (!sensors[MR]) {

        while (sensors[FL]) 
        {
            computeSensor(0,0);
            writeCompare(83,170); //Turn right
        }
    }
    else {
		writeCompare(127,127);
		CyDelay(100);
    }
	
}
void opticalStraight()
{
    int16 distanceM1, distanceM2, decCounter;
    
    writeCompare(127,127);
    CyDelay(2000);
    QuadDec_M1_SetCounter(0);
    decCounter  = 0;
    while (decCounter >  - (DISTANCE * 11.4) * 1.02){
        computeSensor(0,5);
        
        writeCompare(30,30);
        /*
        //sensor shit
        if (((!sensors[FL] && !sensors[FR])))
        {
    		writeCompare(78,78); //Go straight
    		//CyDelay(100);
            
        }
        else if (((!sensors[MM] || !sensors[BM]) && (sensors[FL] && sensors[FR] && sensors[ML] && sensors[MR])))
        {
    		writeCompare(89,89); //Go straight
    		//CyDelay(100);
        } 
        else if (sensors[FL] && !sensors[FR] && sensors[ML] && sensors[MR]) 
        {
    		writeCompare(70,78); //Turn right a bit
    	} 
        else if (!sensors[FL] && sensors[FR] && sensors[ML] && sensors[MR])
        {
    		writeCompare(78,70); //Turn left a bit
    	}  
        else {
    		writeCompare(127,127);
    		CyDelay(100);
        }
        */
        decCounter  = QuadDec_M1_GetCounter();
    }
    
    
   
}

//* ========================================
//ADC Battery read test
void readBattery()
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
    usbPutChar('\r');
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