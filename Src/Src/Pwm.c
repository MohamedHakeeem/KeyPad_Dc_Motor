#include "Pwm.h"




void Pwm_Init(void)
{
	/******************************Gpio[PA8]_Inti*******************************/

	// clock configuration
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN ;
	// select direction of GPIO as  Alternate function mode[10]
	GPIOA->MODER &= ~(GPIO_MODER_MODE8)  ;
	GPIOA->MODER |= (GPIO_MODER_MODE8_1 )  ;
	// select two pin as  push pull mean the output is "1" or "0"
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT8);
	// Select no pull
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR8);

	//Alternate AF1 for tim1:
	GPIOA->AFR[1] &=  ~GPIO_AFRH_AFSEL8;
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL8_0 ;

	/*****************************Pwm_Config*************************************/
	/*  configuration [APB2] of clock */
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN ;
	/*Auto pre_load register must be enable */
	TIM1->CR1 |= TIM_CR1_ARPE;
	/*up_countert and edge_lined*/
	TIM1->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS );
	/*(slave mode disabled & internal clock*/
	TIM1->SMCR &= ~ TIM_SMCR_SMS;


	/*Set timer Pre_scaler, bus clock = 84 MHz, Tim_Clock = 84*2=168 MHz if the pre_scaler is_not set to (1)
	 * the bus timer clock is twice of the APB1 bus clock
	 * fCK_PSC / (PSC[15:0] + 1)
	 * CK_CNT = 168000000 / ( 1679+ 1) -> 100000 Hz -> time base = 10 us
	 **/
	 TIM1->PSC =1679;
	 /*overflow occur at max   -------> ARR=(10^(6)/freq(Hz)*10) */
 	 TIM1->ARR = 0xFFFF;

 	  /* Set 50% duty cycle */
 	  TIM1->CCR1 = 0xFFFF / 2;

 	/*Set Capture/Compare 1 as output */
 	 TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;

     /* Output Compare 1 pre_load enable */
 	 TIM1->CCMR1 |= TIM_CCMR1_OC1PE;

 	 /* Select Output Compare 1 PWM mode 1
 	  * TIMx_CNT < TIMx_CCR1 -> Output Active
 	  * TIMx_CNT >= TIMx_CCR1 -> Output Inactive
 	  **/
 	  TIM1->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);


 	  /* Select Capture/Compare 1 output polarity active low  in some system the the concept is diffferent  */
 	  TIM1->CCER |= TIM_CCER_CC1P;

 	  /* Initialize all the registers   */
 	  TIM1->EGR |= TIM_EGR_UG;

 	 /* Enable Capture/Compare 1 output */
 	  TIM1->CCER |= TIM_CCER_CC1E;


 	 /* Enable timer main output */
 	   TIM1->BDTR |= TIM_BDTR_MOE;


	 /* Enable TIM1 counter */
     	TIM1->CR1 |= TIM_CR1_CEN;


}





void Pwm_Duty(uint16_t freq ,uint16_t duty_cycle  )
{

	if(freq !=0)
	{/*overflow occur at    -------> ARR=(10^(6)/freq(Hz)*10) */
		uint16_t auto_reload=(uint16_t)((100000)/(freq));
	    TIM1->ARR =auto_reload  ;

	    /* Update CCR1 with duty cycle
	        *  duty_cycle = (TIM1->CCR1 / TIM1->ARR) * 100
	        * TIM1->CCR1 = ( duty_cycle * TIM1->ARR) / 100 */
	       /* Calculate capture compare value */

	       uint16_t capture_compare =
	           (uint16_t)(( duty_cycle * auto_reload) / 100);

		 TIM1->CCR1 =  capture_compare;

	}

	else
	{
		//nothing
	}




}
