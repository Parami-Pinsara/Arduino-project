#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define F_CPU 16000000

void init_lcd(void);
void moveto(unsigned char, unsigned char);
void stringout(char *);
void stringclear(void);
void writecommand(unsigned char);
void writedata(unsigned char);
void writenibble(unsigned char);
void Alarm_status(void);
void Local_time_set(void);
void view_day(void);
void set_day(void);
void Alarm_set(void);
void set_buzzer(void);
void Back_light(void);
ISR(TIMER1_COMPA_vect);

int H1=1;
int H2 =0;
int M1 =0;
int M2 =0;
int S1 = 0;
int S2 = 0;

int Alm_M1 = 1;
int Alm_M2 = 0;
int Alm_H1 = 1;
int Alm_H2 = 0;
int Day = 0;
int status = 0;
int z = 0;
int Bl;


int main(void) {
int a=1;
int Alarm;
char x[100];
char b[100];

    /* Main program goes here */

init_lcd();
  
		DDRC &=0xE0;
		DDRB |= 0x18;
 
   
	  
	  TCCR1B =(1<<WGM12);
	  OCR1A=15624;
	  TIMSK1 =(1<<OCIE1A);
	 
	  sei();
	  TCCR1B |=(1<<CS12)|(1<<CS10);
	  
	  
	  
while (1) {  // Loop forever
	
	
	if( (PINC & 0x01) != 0){ 
	 
	 
	PORTB |= 0x08;

		 z++;
	 
	 _delay_ms(800);
	 
	}
	 if(z == 1){
		
		moveto(0,0);
		stringout("Alarm:");
		 
		 moveto(1,0);
		 stringout("Alarm:");
		 
		 moveto(1,6);
		 sprintf(x,"%d%d:%d%d", Alm_H1,Alm_H2,Alm_M1,Alm_M2);
		 stringout(x);
			
			Alarm_status();
	 
	 }
	 else{
	 
		z = 0;
		}
	if(z == 0){
		
		if ( status == 1){
			
		set_buzzer();
		}
		moveto(0,0);
		sprintf(b,"%d%d:%d%d:%d%d ", H1,H2,M1,M2,S1,S2);
		stringout(b);
		
		
		view_day();	
		Local_time_set();
		set_day();
	}
	
		Back_light();
		
   }
 
   

    return 0;   /* never reached */
}

/*
  init_lcd - Configure the I/O ports and send the initialization commands
*/
void init_lcd()
{
    /* ??? */                   // Set the DDR register bits for ports B and D
	DDRD|=0xF0;
	DDRB|=0x03;
    _delay_ms(15);              // Delay at least 15ms

	
	writecommand(0x03);
    /* ??? */                   // Use writenibble to send 0011
    _delay_ms(5);               // Delay at least 4msec
	writecommand(0x03);
    /* ??? */                   // Use writenibble to send 0011
    _delay_us(120);             // Delay at least 100usec

    /* ??? */                   // Use writenibble to send 0011, no delay needed
	writecommand(0x03);

	writecommand(0x02);
    /* ??? */                   // Use writenibble to send 0010
    _delay_ms(2);               // Delay at least 2ms
    
    writecommand(0x28);         // Function Set: 4-bit interface, 2 lines
_delay_ms(2);
    writecommand(0x0f);         // Display and cursor on
_delay_ms(25); 

writecommand(0x01); 
_delay_ms(25); 
//writecommand(1);
}

/*
  moveto - Move the cursor to the row (0 or 1) and column (0 to 15) specified
*/
void moveto(unsigned char row, unsigned char col)
{
    /* Don't need this routine for Lab 5 */
	if(row==0){
		
		writecommand(0x80+col);
	}
	if(row==1){
		
		writecommand(0xc0+col);
	}
	
}

/*
  stringout - Write the string pointed to by "str" at the current position
*/
void stringout(char *str)
{
    /* Don't need this routine for Lab 5 */
	do{
		
		writedata(*str);
		str++;
		
	}while(*str!= '\0');
	
	
}


/*
  writecommand - Send the 8-bit byte "cmd" to the LCD command register
*/
void writecommand(unsigned char cmd)
{
	unsigned char temp;

PORTB&=~(0x01);
temp=cmd&0xF0;
writenibble(temp);
temp=cmd&0x0F;
temp=temp<<4;
writenibble(temp);
_delay_ms(3);

}

/*
  writedata - Send the 8-bit byte "dat" to the LCD data register
*/
void writedata(unsigned char dat)
{
unsigned char temp;

PORTB|=0x01;
temp=dat&0xF0;
writenibble(temp);
temp=dat&0x0F;
temp=temp<<4;
writenibble(temp);
_delay_ms(3);

}

/*
  writenibble - Send four bits of the byte "lcdbits" to the LCD
*/
void writenibble(unsigned char lcdbits)
{
PORTD = lcdbits;//&0xF0;
//PORTB |= 0x02;
PORTB &= ~(0x02);
PORTB |= 0x02;
PORTB &= ~(0x02);
}
void Alarm_status(){

	if( (PINC & 0x02) != 0){
		
		_delay_ms(2000);
		PORTB |= 0x08;
		status++;
		
		_delay_ms(800);
		
	}
		if(status == 1){			//status of ther alarm is on
			
			Alarm_set();
			moveto(0,6);
			stringout("ON ");
		
			set_buzzer();
			 
				
			}else{
			
			
			status=0;
		}

		if(status == 0){			//status of the alarm is off
			
		PORTB &= ~(0x10);	
		moveto(0,6);
		stringout("Off");
		
		
		
		}
		
			
}
void Local_time_set(){
	
		if( (PINC & 0x08) != 0){  //if button 4 is pressed
			
			_delay_ms(2500);
			
			PORTB |= 0x08;
			
			if(M2<=9){
				
				M2++;
				_delay_ms(220);
				
				if(M2 == 10){
					
					M2 = 0;
					M1++;
				}
			}
			if(M1 == 6){
				M1 = 0;
				H2++;
				
				if(H2 == 10 && (H1 ==0 | H1 == 1) ){
					
					H2 = 0;
					H1++;
					
				}
				
				if(H1 == 2 && H2 == 4 ){
					
					H1 = 0;
					H2 = 0;
					
				}
				
			}
		}
}
void set_day(){
	
	if( (PINC & 0x10) != 0){			//if  button 5 is pressed
	
		_delay_ms(2500);
		Day++;
	}
	if(Day == 7){
	
	Day = 0;
	
}
	
}
void view_day(){
	
	if(Day == 0){
		
		moveto(1,0);
		stringout("Monday     ");
	}
	if(Day == 1){
		
		moveto(1,0);
		stringout("Tuesday    ");
	}
	if(Day == 2){
		
		moveto(1,0);
		stringout("Wednesday  ");
	
	}if(Day == 3){
	
		moveto(1,0);
		stringout("Thursday   ");
	
	}if(Day == 4){

		moveto(1,0);
		stringout("Friday     ");

	}if(Day == 5){

		moveto(1,0);
		stringout("Saturday   ");
	
	}if(Day == 6){
	
		moveto(1,0);
		stringout("Sunday     ");
	}
}
		
void Alarm_set(){
	
	  moveto(1,6);
	  if( (PINC & 0x04) != 0){  //if button 3 is pressed
		  
		  _delay_ms(2500);
		  
		  PORTB |= 0x08;
		  
		  	  
		  if(Alm_M2<=9){
			  
			  Alm_M2++;
			  _delay_ms(220);
			  
			  if(Alm_M2 == 10){
				  
				  Alm_M2 = 0;
				  Alm_M1++;
			  }
		  }
		  if(Alm_M1 == 6){
			  Alm_M1 = 0;
			  Alm_H2++;
			  
			  if(Alm_H2 == 10 && (Alm_H1 ==0 | Alm_H1 == 1) ){
				  
				  Alm_H2 = 0;
				  Alm_H1++;
				  
			  }
			  
			  if(Alm_H1 == 2 && Alm_H2 == 4 ){
				  
				  Alm_H1 = 0;
				  Alm_H2 = 0;
				  
			  }
			  
		  }
	  }
	  
}
void set_buzzer(){

	if((M1 == Alm_M1) && (M2 == Alm_M2) && (H1 == Alm_H1) && (H2 == Alm_H2)){  //check the local time and the alarm time
		
		z=1;
		PORTB |= 0x10;			//buzzer on 
		PORTB |= 0x08;
		
		}else{
		
		PORTB &= ~(0x10);		//buzzer off
		
		
	}
	
}
void Back_light(){

	if( (PINC & 0x20) != 0){		//if button 6 is pressed
		
		 _delay_ms(2500);
		
			PORTB &= ~(0x08);		//Alarm off
	}
	
}
ISR(TIMER1_COMPA_vect) //loop to be executed on counter compare match

{
	

	if (S2 <= 9)
	{
		S2++;
		
		if(S2 == 10){

			S2 = 0;
			S1++;
		}	
		if(S1 == 6){
			S1 = 0;
			M2++;
			
			if(M2 == 10){
				M2 = 0;
				M1++;
			}	
		}
		if(M1 == 6){
			M1 = 0;
			H2++;
			
			if(H2 == 10 && (H1 ==0 | H1 == 1) ){
				
				H2 = 0;
				H1++;
				
			}
			
			if(H1 == 2 && H2 == 4 ){
				
				H1 = 0;
				H2 = 0;
				Day++;
				
	}if(Day == 7){
	
			Day = 0;
	
	}
	
		}
	}
		}
	
	

	


		
