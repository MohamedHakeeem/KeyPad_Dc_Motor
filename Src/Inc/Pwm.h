
#ifndef INC_PWM_H_
#define INC_PWM_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"



void Pwm_Init(void);
void Pwm_Duty(uint16_t freq ,uint16_t duty_cycle  );







/* C++ detection */
#ifdef __cplusplus
}
#endif




#endif /* INC_PWM_H_ */
