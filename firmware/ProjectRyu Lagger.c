Copyright 2016 Stefan Andrei Chelariu

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


/**************************
*Project Ryu Guitar Effect*
*Model: Lagger            *
*Date: 11.01.2014         *
*Author: S.A.Chelariu     *
**************************/

/**************************
*Revision: 1              *
*Date: 24.03.2014         *
*By: S.A. Chelariu        *
*Note: Removed Sustain    *
*added interrupt routine  *
*for bypass switch        *
**************************/

/**************************
*Revision: 2              *
*Date: 22.02.2015         *
*By: S.A. Chelariu        *
*Note: Removed interrupt*
*True bypass on footsw*
**************************/


#include<p18f1320.h>
#include<delays.h>

//***Defines***//
#define SWELL LATBbits.LATB3
#define FOOTSW_READ PORTBbits.RB2
#define FOOTSW LATBbits.LATB4
#define BUFFER_SIZE 200

//***Prototypes***//
void Board_Init(void);
int GET_SIGNAL(void);
int GET_PARAMETER(void);


void main(void)
{
	int INPUT, RELEASE, THRESHOLD, ENABLE,i;
	Board_Init();
	SWELL = 1;
	ENABLE = FOOTSW_READ;
	Delay10KTCYx(1);
	while(1)
	{
	
		THRESHOLD = GET_PARAMETER();
		THRESHOLD = THRESHOLD / 50;
		INPUT = GET_SIGNAL();
		
		if(INPUT >THRESHOLD)
			{
				SWELL = 0;
				Delay1KTCYx (1);
				
			}
		else
			{
				SWELL = 1;
				Delay1KTCYx (1);
			}

	}
}

void Board_Init(void)
{
 	TRISAbits.TRISA0 = 1;
	TRISAbits.TRISA1 = 1;
	TRISAbits.TRISA2 = 1;
	TRISAbits.TRISA4 = 0;
	TRISBbits.TRISB3 = 0;
	TRISBbits.TRISB1 = 1;
	TRISBbits.TRISB4 = 1;

	//ADC Setup
	ADCON0 = 0b00000000;	
	ADCON1 = 0b01111000;
	ADCON2 = 0b10011010;
	
}

int GET_SIGNAL(void)
{
	int VREF, SIGNAL, i;
	double SUM;
	SUM=0;
	for(i=0;i<BUFFER_SIZE;i++)
	{
	ADCON0 = 0b00000011;
	while(ADCON0bits.GO_DONE==1);
	VREF = ADRES;
	
	ADCON0 = 0b00000111;
	while(ADCON0bits.GO_DONE==1);
	SIGNAL = ADRES;
	//Rectify
	if(SIGNAL >= VREF)
	SIGNAL=SIGNAL - VREF;
	else if(SIGNAL < VREF)
	SIGNAL=VREF-SIGNAL;	
	SUM=SUM+SIGNAL;	
	}
	SIGNAL = SUM/BUFFER_SIZE;
	SIGNAL = SIGNAL *4.8;
	SUM=0;
	return SIGNAL;
}

int GET_PARAMETER(void)
{
	ADCON0 = 0b00001011;
	while(ADCON0bits.GO_DONE==1);
	return ADRES;
}