
#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_


/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"

typedef enum
{
	refresh,
	col1,
	col2,
	col3,
	col4

}col_num;


typedef enum
{
	idle,
	row1,
	row2,
	row3,
	row4

}row_num;




void R_inp_init(void);
void C_out_init(void);

void Write_C(col_num col_i);
uint8_t Read_R(row_num row_i);
void Key_Process(void);








/* C++ detection */
#ifdef __cplusplus
}
#endif



#endif /* INC_KEYPAD_H_ */
