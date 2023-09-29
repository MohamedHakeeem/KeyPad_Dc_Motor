#include "KeyPad.h"
#include "Systick.h"
#include "Gpio.h"
#include "Pwm.h"


col_num col=refresh;
row_num row=idle;

/*GPIOE [E0,E1,E2,E3] ARE CONFIGURED AS INPUT*/
void R_inp_init(void)
{
	/* Enable post GPIOE clock in RCC */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

	/* Select input mode for PC1 */
	GPIOE->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1
			| GPIO_MODER_MODER2 | GPIO_MODER_MODER3);

	/* Select pull down(10)  */
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1
			| GPIO_PUPDR_PUPD2 |GPIO_PUPDR_PUPD3);

	GPIOE->PUPDR |=  (GPIO_PUPDR_PUPD0_1 |GPIO_PUPDR_PUPD1_1
			|GPIO_PUPDR_PUPD2_1 |GPIO_PUPDR_PUPD3_1 );


}
/*GPIOE [E4,E5,E6,E7] ARE CONFIGURED AS OUTPUT*/
void C_out_init(void)
{

	/* Enable post GPIOE clock in RCC */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	/* select direction of GPIO as output */
	GPIOE->MODER &= ~(GPIO_MODER_MODE4 | GPIO_MODER_MODE5
			| GPIO_MODER_MODE6  |GPIO_MODER_MODE7 )  ;
	GPIOE->MODER |= (GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0
			| GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0)  ;
	/* select two pin as  push pull mean the output is "1" or "0" */
	GPIOE->OTYPER &= ~(GPIO_OTYPER_OT4 |GPIO_OTYPER_OT5
			| GPIO_OTYPER_OT6 |GPIO_OTYPER_OT7);
	/* the output speed is high speed (10) */
	GPIOE->OSPEEDR |= (GPIO_OSPEEDR_OSPEED4_1 | GPIO_OSPEEDR_OSPEED5_1
			 | GPIO_OSPEEDR_OSPEED6_1 | GPIO_OSPEEDR_OSPEED7_1);

	/* Enable SYSCFG clock */
	RCC->APB2ENR |= RCC_APB2LPENR_SYSCFGLPEN;

	/* Compensation Cell for high speed output */
	SYSCFG->CMPCR |= SYSCFG_CMPCR_CMP_PD  ;

	// while(SYSCFG_CMPCR_READY != (SYSCFG->CMPCR & SYSCFG_CMPCR_READY) )
	//{
	//	//nothing
	//}


	 /* Select no pull */
	 GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPDR4 |GPIO_PUPDR_PUPDR5
			  | GPIO_PUPDR_PUPDR6 |GPIO_PUPDR_PUPDR7);
}


/*1000 for column 1
 *0100 for column 2
 *0010 for column 3
 *0001 for column 4
 *0000 for refreshing the R_inputs
 * */

void Write_C(col_num col_i)
{
	switch( col_i)
	{

	   case(refresh):
			GPIOE->BSRR = (GPIO_BSRR_BR4 |GPIO_BSRR_BR5
			    	| GPIO_BSRR_BR6 |GPIO_BSRR_BR7 );
				break;
		case(col1):
		    GPIOE->BSRR = (GPIO_BSRR_BS4 |GPIO_BSRR_BR5
		    		 | GPIO_BSRR_BR6 |GPIO_BSRR_BR7 );
				break;
		case(col2):
		    GPIOE->BSRR = (GPIO_BSRR_BR4 |GPIO_BSRR_BS5
		    		 | GPIO_BSRR_BR6 |GPIO_BSRR_BR7 );
				break;

		case(col3):
			 GPIOE->BSRR = (GPIO_BSRR_BR4 |GPIO_BSRR_BR5
		    		 | GPIO_BSRR_BS6 |GPIO_BSRR_BR7 );
					break;
		case(col4):
			 GPIOE->BSRR = (GPIO_BSRR_BR4 |GPIO_BSRR_BR5
		    		 | GPIO_BSRR_BR6 |GPIO_BSRR_BS7 );
					break;
		default:
			break;

	}



}

uint8_t Read_R(row_num row_i)
{
	switch( row_i)
			{
			   case(idle):
						break;
				case(row1):
							if( GPIO_IDR_ID0 == (GPIOE->IDR & GPIO_IDR_ID0 ))
								{return(1);	}
						break;
				case(row2):
							if( GPIO_IDR_ID1 == (GPIOE->IDR & GPIO_IDR_ID1 ))
								{return(1);	}
						break;

				case(row3):
							if( GPIO_IDR_ID2 == (GPIOE->IDR & GPIO_IDR_ID2 ))
								{return(1);	}
							break;
				case(row4):
							if( GPIO_IDR_ID3 == (GPIOE->IDR & GPIO_IDR_ID3 ))
								{return(1);}
							break;
				default:
					break;

			}
	  // Add a default return statement here
	    return 0;
}



/*1000 for column 1
 *0100 for column 2
 *0010 for column 3
 *0001 for column 4
 *0000 for refreshing the R_inputs
 * */

void Key_Process(void)
{
	/******col1******/
	Write_C(refresh);
	SysTick_Delay(10);
	Write_C(col1);
	SysTick_Delay(10);
	if (Read_R(row1)==1){
		Gpio_off(Green_Led);
		Gpio_off(Red_Led);   }      //num:7;
	if (Read_R(row2)==1)
		Pwm_Duty(100, 100);      // num:4
	if (Read_R(row3)==1)
		Pwm_Duty(100, 25);                 // num:1
	if (Read_R(row4)==1){}
		            // num:"on";

	/******col2******/
	SysTick_Delay(10);
	Write_C(col2);
	SysTick_Delay(10);
	if (Read_R(row1)==1){}
                        //num:8;
	if (Read_R(row2)==1){
		Gpio_on(Green_Led);
		Gpio_off(Red_Led);}  // num:5
	if (Read_R(row3)==1)
		Pwm_Duty(100, 50);                         // num:2
	if (Read_R(row4)==1){}
		              //0;
	/******col3******/
	SysTick_Delay(10);
	Write_C(col3);
	SysTick_Delay(10);
	if (Read_R(row1)==1)
	{}               //9;
	if (Read_R(row2)==1){
		Gpio_on(Red_Led);
		Gpio_off(Green_Led);}              // num:6
	if (Read_R(row3)==1)
		Pwm_Duty(100, 75);                       // num:3
	if (Read_R(row4)==1){}
		                      //=;
	/******col4******/
	SysTick_Delay(10);
	Write_C(col4);
	SysTick_Delay(10);
	if (Read_R(row1)==1){}
		                //"/";
	if (Read_R(row2)==1){}
	                     //"x";
	if (Read_R(row3)==1){}
		                       //"+;
	if (Read_R(row4)==1){}
		                        ; //"-"
	SysTick_Delay(10);
}
