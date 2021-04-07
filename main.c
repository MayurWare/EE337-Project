/*--------------------------------------------EE 337: Microprocessors Laboratory (Spring 2021)---------------------------------------------*/
/*--------------------------------------------Project Name : Obstacle Avoidance Game on Pt-51----------------------------------------------*/
/*-----------------------------------------------Made By : Mayur Ware, Roll No. : 19D070070------------------------------------------------*/

#include <at89c5131.h>
#include "lcd.h"		                 //Header file with LCD interfacing functions
#include "serial.c"	                 //C file with UART interfacing functions

unsigned char b0,b1,b2;              //Chars for implementing the linear feedback shift register (LFSR) to generate pseudorandom-ness.
unsigned char check[2];              //Check variables
unsigned char init[3] = {0,1,1};     //To initialize lfsr bits and also to preserve the previous state after game restarts
unsigned char str1[17] = "                ";   //str1 to display on Line 1 of LCD
unsigned char str2[17] = "                ";   //str2 to display on Line 2 of LCD
unsigned char store[2] ="  ";        //Store variables 
unsigned int score = 0;              //To update score aafter each second
unsigned int best = 0;               //To keep track of the best score

//Initializing functions
void msdelay(unsigned int);					//Takes integer value as an input and generates corresponding delay in milli seconds
void lfsr_func(void);               //Implements the linear feedback shift register (LFSR) to generate pseudorandom-ness.
void collision(void);               //Checks if Dinosaur and obstacle are collided 
void reorder(void);                 //Sends sequence of new characters
void print_data(void);              //Prints the updates strings
void main(void);                    //Main 
void get_char(void);                //Gets character from UART

/*------------------------ms delay function-----------------------------*/
void msdelay(unsigned int time)
{
	int i,j;
	for(i=0;i<time;i++)
	{ get_char();
		collision();
		for(j=0;j<382;j++);
	}
}
/*--------------------------lfsr_func function---------------------------*/
void lfsr_func(void)
{ unsigned char buff;               //To temporarily store the value of b2
	buff = b2;
	b2 = b2^b0;                       //b2 = b2 xor b0
	init[2] = b2;                     //Preserve value of b2
	b0 = b1;
	init[0] = b0;                     //Preserve value of b0
	b1 = buff;
	init[1] = b1;                     //Preserve value of b1
}
/*--------------------------collision function---------------------------*/
void collision(void)
{ if(score>best)                    //Check if score>best and then assign it to best
			{
				best = score;            
		  }
	if(check[0] == str1[2] | check[1] == str2[2])   //If check variables and characters at column 3 of strings
	{ P1 = 0xA0;                                    //Change LED status
		lcd_cmd(0x01);                                //Clear LCD
		lcd_cmd(0x80);
		lcd_write_string("Score :");                  //Send "Score :" 
		msdelay(4);
		
		lcd_cmd(0x8C);
		int_to_string(score);                         //Send the score to LCD
		msdelay(4);
	
		lcd_cmd(0xC0);
		lcd_write_string("High Score :");             //Send "High Score :" 
		msdelay(4);
		
		lcd_cmd(0xCD);
		int_to_string(best);                          //Send the best score to LCD
		msdelay(3000);                                //3 Second delay
		main();                                       //Jump to main();
	}
}	
/*--------------------------shuffle function---------------------------*/
void shuffle(unsigned char b0, unsigned char b1, unsigned char b2)
{
	if(b0==1 & b1==0 & b2==1)                       //Case 1
	{
		str1[15]=' ';
    str2[15]=' ';
    print_data();		
		str1[15]='*';		
    str2[15]=' ';
		print_data();			
		str1[15]='*';	
    str2[15]=' ';	
		print_data();	
	}
	else if(b0==0 & b1==1 & b2==0)                  //Case 2
	{
		str1[15]=' ';
    str2[15]=' ';
		print_data();	
		str1[15]=' ';
		str2[15]='*';
		print_data();	
		str1[15]=' ';
		str2[15]='*';
    print_data();			
	}
	else if(b0==0 & b1==0 & b2==1)                  //Case 3
	{
		str1[15]=' ';
    str2[15]=' ';
		print_data();	
		str1[15]='*';
		str2[15]=' ';
		print_data();	
		str1[15]=' ';
		str2[15]=' ';
	}
	else if(b0==1 & b1==0 & b2==0)                   //Case 4
	{
		str1[15]=' ';
    str2[15]=' ';
		print_data();	
		str1[15]=' ';
		str2[15]=' ';
		print_data();	
		str1[15]=' ';
		str2[15]='*';
		print_data();	
	}
	else if(b0==1 & b1==1 & b2==0)                   //Case 5
	{
		str1[15]=' ';
    str2[15]=' ';
		print_data();	
		str1[15]=' ';
		str2[15]='*';
		print_data();	
		str1[15]=' ';
		str2[15]=' ';
    print_data();			
	}
	else if(b0==1 & b1==1 & b2==1)                   //Case 6
	{
		str1[15]=' ';
    str2[15]=' ';
		print_data();	
		str1[15]=' ';
		str2[15]=' ';
		print_data();	
		str1[15]='*';
		str2[15]=' ';
		print_data();	
	}
	else if(b0==0 & b1==1 & b2==1)                   //Case 7
	{
		str1[15]=' ';
    str2[15]=' ';
		print_data();	
		str1[15]=' ';
		str2[15]=' ';
		print_data();	
		str1[15]=' ';
		str2[15]=' ';
		print_data();	
	}
}
/*--------------------------reorder function---------------------------*/
void reorder(void)
{ unsigned int i = 0;                             
	store[0] = str1[3];                             //Load store[0] into str1[3]
	store[1] = str2[3];                             //Load store[1] into str2[3]
	for ( i=0;i < 15; i++)                          //Loop into strings
	{if (str1[i+1] == 'D'){                         //If 'D' appears, send previous character
	      str1[i] = str1[i+2];}
	 else{str1[i] = str1[i+1];}                     //Shift each character of str1 left by 1
 
	 if (str2[i+1] == 'D'){                         //If 'D' appears, send previous character
	     str2[i] = str2[i+2];} 
	 else{str2[i] = str2[i+1];}                     //Shift each character of str2 left by 1
	}
	collision();                                    //Check if collision occurred
}
/*--------------------------print_data function---------------------------*/
void print_data(void)
{ P1 = 0xF0;                                      //LEDs ON
	//Send both strings to LCD
	lcd_cmd(0x80);
	lcd_write_string(str1);
  lcd_cmd(0xC0);
  lcd_write_string(str2);
	msdelay(1000);                                 //Delay of 1s
	score++;                                       //Increment Score
	P1 = 0x00;                                     //LEDs OFF
  //Send both strings to LCD
	lcd_cmd(0x80);
	lcd_write_string(str1);
  lcd_cmd(0xC0);
  lcd_write_string(str2);
	collision();                                   //Check if collision occurred
  reorder();                                     //Jump to reorder();
}
/*--------------------------get_char function---------------------------*/
void get_char(void)
{
	   //Receive a character ch from UART
			switch(ch)
			{
				case 'q':                                //Case for key 'q' pressed to move 'D' up					
				         str1[2] = 'D';                  //Send 'D' on Line 1 Column 3
                 str2[2]= store[1];				       //Store current value of str2[2] into store[1]
				         //Update CHeck variables
				         check[0]='*';
				         check[1]='&';
								 break;
				
				case 'a':                                //Case for key 'a' pressed to move 'D' down			
				         str1[2] = store[0];             //Store current value of str1[2] into store[0]
                 str2[2]= 'D';					         //Send 'D' on Line 2 Column 3
				         //Update CHeck variables
				         check[0]='&';
				         check[1]='*';
								 break;		
				default:		 break;		
			}
}
/*--------------------------main function---------------------------*/
void main(void)
{ unsigned int i = 0;
	//Loop through both strings and make them blank
	for (i=0; i < 15; i++) 
	{str1[i] = ' ';
  str2[i] = ' ';}
	//Initialize Char variables
	check[0] = '#';
	check[1] = '#';
	//Stop printing instruction
	str1[16] = '\0';
  str2[16] = '\0';
	//Initialize Store variables
	store[0] = ' ';
	store[1] = ' ';
	//Start 'D' at Line 1 Column 3
	ch = 'q';
	//Set Score to 0
	score = 0;
	//Load the preserved states into b0, b1 and b2
	b0 = init[0];
	b1 = init[1];
	b2 = init[2];
	//Initialize port P1 for output from P1.7-P1.4
	P1 = 0xF0;
	//Call initialization functions
	lcd_init();      //Call LCD function
	uart_init();     //Initialize UART
	//Send Game instructions for player
	transmit_string("Welcome to Obstacle Avoidance Game!\r\n");
	transmit_string("Press 'Q' and 'A' keys to move dinosaur UP and DOWN to avoid obstacles\r\n");
	transmit_string("The game will end once Dinosaur and obsacle will collide\r\n");
  transmit_string("All the Best!\r\n");	
  //Start
	while(1)
	{  
		lfsr_func();
		shuffle(b0, b1, b2);
	}
}