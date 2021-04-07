/*--------------------------------------------EE 337: Microprocessors Laboratory (Spring 2021)---------------------------------------------*/
/*--------------------------------------------Project Name : Obstacle Avoidance Game on Pt-51----------------------------------------------*/
/*-----------------------------------------------Made By : Mayur Ware, Roll No. : 19D070070------------------------------------------------*/
#include <at89c5131.h>

bit tx_complete = 0,rx_complete = 0;       //Bit flags for interrupts
unsigned char ch, temp_in;                 //variables

void uart_init(void)
{
	TMOD=0x20;			                        //Configure Timer 1 in Mode 2
	TH1=0xF3;			                          //Load TH1 to obtain require Baudrate (here 4800Hz) (Refer Serial.pdf for calculations)
	SCON=0x50;			                        //Configure UART peripheral for 8-bit data transfer 
	TR1=1;					                        //Start Timer 1
	ES=1;					                          //Enable Serial Interrupt
	EA=1;					                          //Enable Global Interrupt
}

/********transmit_char(<unsigned char ch>): Transmits a character using UART*******/	
void transmit_char(unsigned char ch1)
{
	SBUF=ch1;				                        //Load data in SBUF
	while(!tx_complete); 	                  //Wait for tx_complete flag (interrupt to complete)
	tx_complete = 0;		                    //Clear tx_complete flag 
}

/********transmit_string(<String pointer>): Transmit a string using UART*******/	
void transmit_string(unsigned char *s)
{
	while(*s != 0)
	{
			transmit_char(*s++);
	}
}	

/********Serial_ISR(): Interrupt service routine for UART interrupt.
	Determines whether it is a transmit or receive interrupt and raise corresponding flag.
	Transmit or receive functions (defined above) monitor for these flags to check if data transfer is done.*******/	
void serial_ISR(void) interrupt 4
{
		if(TI==1)			                       //check whether TI is set
		{   
			TI = 0;			                       //Clear TI flag
			tx_complete = 1;	                 //Set tx_complete flag indicating interrupt completion
		}
		else if(RI==1)			                 //check whether RI is set
		{ temp_in = SBUF;					           //Read data from SBUF
			//Directly read character received from UART
			if(temp_in == 'q' || temp_in == 'Q')
				ch = 'q';
			else if(temp_in == 'a' || temp_in == 'A')
				ch = 'a';
			RI = 0;			                       //Clear RI flag
			rx_complete = 1;	                 //Set rx_complete flag indicating interrupt completion
		}
}