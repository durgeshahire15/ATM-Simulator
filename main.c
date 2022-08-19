#include <at89c5131.h>
#include "lcd.h"		// Header file with LCD interfacing functions
#include "serial.c"	// C file with UART interfacing functions
#include <stdlib.h>

//The maximum amount that can be dispensed by the ATM is 2000*20 + 500*30 + 200*10 + 100*10 = 58000.

/*************************************
Defining various variables required for the program
**************************************/

// Defining counter variables for the notes

unsigned int count_2000 = 20;
unsigned int count_500 = 30;
unsigned int count_200 = 10;
unsigned int count_100 = 10;

// Variable for storing the ASCII value of a two digit integer which would be the count of notes of various denominations

char count_int_data_ascii[3]={0,0,'\0'};

unsigned char cash_char[6] = {'0','0','0','0','0','\0'}; // I have taken a 5 digit container since the max. value that can be dispensed is 58k

// Variable to store the input cash value by the user. 
unsigned int cash_int=0;

/************************
Atm function definitions
*************************/
void main_2(void); // Defining the Frame for the Looping Function for our ATM  

void atm_state(void)
{
	// Function used for printing the total number of available notes in the ATM 
	
	lcd_cmd(0x80); // Moving cursor to the 1st line

	lcd_write_string("2000:");
	int_to_string(count_2000,count_int_data_ascii);	// Converting the integer counter value of 2000 notes to string 
	lcd_write_string(count_int_data_ascii);
	lcd_write_string(" ");
	
	lcd_write_string("500:");
	int_to_string(count_500,count_int_data_ascii);	// Converting the integer counter value of 500 notes to string
	lcd_write_string(count_int_data_ascii);
	
	lcd_cmd(0xC0); //Moving cursor to the 2nd line	
	lcd_write_string("200:");
	int_to_string(count_200,count_int_data_ascii);	// Converting the integer counter value of 200 notes to string
	lcd_write_string(count_int_data_ascii);
	lcd_write_string(" ");
	
	lcd_write_string("100:");
	int_to_string(count_100,count_int_data_ascii);	// Converting the integer counter value of 100 notes to string
	lcd_write_string(count_int_data_ascii);
	msdelay(3000); 																	// Time delay of 3 seconds
	
}

/***********Error Messages for the following cases:************** 

1) First Character not 'd' 
2) Second character either a '0' or Enter Key 
3) The input cash not ending in two zeroes
4) Any other intermediate character other than the integers from '0' to '9' 

*****************************************************************/
void error_mssg(void)
{
	lcd_cmd(0xC0);
	lcd_write_string("Invalid Input");
	msdelay(2000);
	lcd_cmd(0x01);
}

/****************************************************
Function displaying the various denomination notes available in the ATM
*****************************************************/
void disp_denominations(void)
{
	lcd_cmd(0x80);
	
	if(count_2000 != 0){
				lcd_write_string("2000,");
			}
	
	if(count_500 != 0){
				lcd_write_string("500,");
		  }
	
	if(count_200 != 0){
				lcd_write_string("200,");
		  }
	
	if(count_100!= 0){
				lcd_write_string("100");
			}
}
/***********************************************
Decider Function Used for dispensing the available notes
************************************************/
void atm_dispense(void)
{	
	//Defining temporary variables for holding the values of the original counter variable 
	
	unsigned int count_2000_temp = count_2000;
	unsigned int count_500_temp = count_500;
	unsigned int count_200_temp = count_200;
  unsigned int count_100_temp = count_100;
	
	if (cash_int%100 != 0){
		//Checking whether the last two digits of the number have 0 or not
 		error_mssg();
		main_2();
	}
	
	else{
		if(cash_int/2000 != 0 && count_2000_temp >0){ 	// Ensuring whether the 2000 notes can be dispensed or not // Ensuring whether the 2000 notes can be dispensed or not
			
			if(cash_int/2000 <= count_2000_temp){ 				// If requirement of 2000 notes is less than available amount, than dispense the required amount
				count_2000_temp = count_2000_temp - cash_int/2000;
				cash_int = cash_int - 2000*(cash_int/2000);
				}
			else{																					// If requirement of 2000 notes is more than available amount, than dispense the maximum amount
				cash_int = cash_int - count_2000_temp*2000;
				count_2000_temp = 0;
			}
		}
		
		if(cash_int/500 != 0 && count_500_temp > 0){  // Ensuring whether the 500 notes can be dispensed or not
			if(cash_int/500 <= count_500_temp){  				// If requirement of 500 notes is less than available than dispense the required amount
		 			count_500_temp = count_500_temp - cash_int/500;
					cash_int = cash_int - 500*(cash_int/500);
			}
			else{                                       //If requirement of 500 notes is more than available amount, than dispense the maximum amount
				cash_int = cash_int - count_500_temp*500;
				count_500_temp = 0;
			}
		}
		
		if(cash_int/200 != 0 && count_200_temp >0){   // Ensuring whether the 200 notes can be dispensed or not
			
			if(cash_int/200 <= count_200_temp){					//If requirement of 200 notes is less than available than dispense the required amount
			count_200_temp = count_200_temp - cash_int/200;
			cash_int = cash_int - 200*(cash_int/200);
			}
			else{																				//If requirement of 200 notes is more than available amount, than dispense the maximum amount
			cash_int = cash_int - count_200_temp*200;
			count_200_temp = 0;
			}
		}
		
		if(cash_int/100 != 0 && count_100_temp>0){		// Ensuring whether the 100 notes can be dispensed or not
			
			if(cash_int/100 <= count_100_temp){					//If requirement of 100 notes is less than available than dispense the required amount
			count_100_temp = count_100_temp - cash_int/100;
			cash_int = cash_int - 100*(cash_int/100);
			}
			else{																				//If requirement of 100 notes is more than available amount, than dispense the maximum amount
			cash_int = cash_int - count_100_temp*100;
			count_100_temp = 0;
			}
		}
		
		if(cash_int == 0){
			//If successfully dispensed the entire amount than re-assign the final values of temporary variables to the original counter variables
			count_2000 = count_2000_temp;
			count_500 = count_500_temp;
			count_200 = count_200_temp;
			count_100 = count_100_temp;
			lcd_cmd(0xC0);
			lcd_write_string("Collect Cash");
			msdelay(2000);
			lcd_cmd(0x01);
		
		}
		
		else{
			//If successfully cash not dispensed then don't reassign the value of temporary variables
			lcd_cmd(0xC0);
			lcd_write_string("Try Again");
			msdelay(2000);
			lcd_cmd(0x01);
		}
	}
}

void main(void)
{
	//Call initialization functions
	lcd_init();
	uart_init();
	main_2();
}
		
void main_2(void){
	  
	while(1){
			
			unsigned char ch=0;
			unsigned int i=0;
			//Receive a character
			ch = receive_char();
			
		if (ch == 0){ 
			disp_denominations();
		}
		
		else{
			
			//Decide which test function to run based on character sent
      //Displays the string on the LCD
			
			switch(ch){
				
				case 'd':
					lcd_cmd(0x01);	
					atm_state();
					break;
						
				default: 
					error_mssg();
					main_2();
					break;
		}
			
			lcd_cmd(0x01);
			disp_denominations();
			lcd_cmd(0xC0);
			msdelay(100);
			
			ch = receive_char();//Inputting Second Character
		
			while(ch==0){ch = receive_char();}
		
			if (ch == '0' || ch ==13){ // If second character is an enter key or a zero
				error_mssg();
				main_2();
			} 
		
			while(1){
			// Second Character is input only when it is non-zero and neither an enter key
				if(ch >= '0' && ch <= '9' && i <=4){
					lcd_write_char(ch);
					cash_char[i] = ch;
					i=i+1;
				}
			//Stop Receiving input character when enter key is pressed
				else if(ch == 13){ // Here 13 is the ascii value of enter key
					cash_char[i] = '\0';
					break;
				}
			//Stop Receiving input character if the user enters any other non-numeric data
				else{
					error_mssg();
					main_2();
			}
				ch = receive_char();//Inputting Second Character
				while(ch==0){ch = receive_char();}
			}
		
			cash_int = atoi(cash_char);
			atm_dispense();
		
		}
	}
}

