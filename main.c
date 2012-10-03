
#include "msp430g2553.h"

#include "TI_USCI_I2C_slave.h"

void receive_cb(unsigned char receive);
void transmit_cb(unsigned char volatile *receive);
void start_cb();

unsigned char RXData = 0 ;

unsigned char pins[4];

void main(void)
{
  P1DIR |= BIT0;  // set LED 1 to output
  //P1OUT &= ~BIT0;  // set LED 1 off

  P2DIR |= BIT0 + BIT1 + BIT2 + BIT3;
  P2OUT = 0x00;

  pins[0] = pins[1] = pins[2] = pins[3] = 0;

  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  TI_USCI_I2C_slaveinit(start_cb, transmit_cb, receive_cb, 0x05); // init the slave

  _EINT();
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ;

  LPM0; // Enter LPM0.
}

void start_cb(){
  P1OUT ^= BIT0;  // toggle the LED
}

void receive_cb(unsigned char receive){
  if (RXData == 0) {
	  RXData = receive;
  } else {
	  if (RXData >=1 && RXData <= 4) {
		  if (receive == 0) {
			  switch (RXData) {
			  case 1:
				  P2OUT &= ~BIT0;
				  break;
			  case 2:
				  P2OUT &= ~BIT1;
				  break;
			  case 3:
				  P2OUT &= ~BIT2;
				  break;
			  case 4:
			  	  P2OUT &= ~BIT3;
			  	  break;
			  default:
				  break;
			  }
			  pins[RXData-1] = 0;
		  } else { // receive != 0
			  switch (RXData) {
			  case 1:
			      P2OUT |= BIT0;
			  	  break;
			  case 2:
			 	  P2OUT |= BIT1;
			 	  break;
			  case 3:
			 	  P2OUT |= BIT2;
			 	  break;
			  case 4:
			  	  P2OUT |= BIT3;
			  	  break;
			  default:
			 	  break;
			  }
			  pins[RXData-1] = 1;
		  }
	  }
	  RXData = 0;
  }
}

void transmit_cb(unsigned char volatile *byte) {
  if (RXData >= 1 && RXData <= 4) {
	  *byte = pins[RXData-1];
  } else {
	  *byte = 0xFF;
  }
  RXData = 0;
}
