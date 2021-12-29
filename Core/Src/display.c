/*
 * display.c
 *
 *  Created on: Nov 23, 2020
 *      Author: Stancoj
 */

#include "display.h"

uint64_t disp_time = 0, disp_time_saved = 0;
unsigned char chr;
char dataDisplay[4];
int pom;
int state;

void updateDisplay(void);
void setDigit(uint8_t pos);

/*Reset (turn-off) all the segments of display*/
void resetSegments(void)
{
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3);
}

/*Set (turn-on) all the segments of display*/
void setSegments(void)
{
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3);
}

/* Reset (turn-off) all digits*/
void resetDigits(void)
{
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7);
}

/* Reset (turn-on) all digits*/
void setDigits(void)
{
	DIGIT_1_ON;
	DIGIT_2_ON;
	DIGIT_3_ON;
	DIGIT_4_ON;
	DIGIT_TIME_ON;
}

const unsigned char seven_seg_digits_decode_abcdefg[75]= {
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x37, 0x06, 0x3C, 0x57, 0x0E, 0x55, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x0F, 0x3E, 0x1C, 0x5C, 0x13, 0x3B, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x08,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E, 0x37, 0x06, 0x3C, 0x57,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x0E, 0x55, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B, 0x0F, 0x3E, 0x1C, 0x5C,
/*  x     y     z     */
    0x13, 0x3B, 0x6D
};

void decode_7seg(char chr)
{
	if(seven_seg_digits_decode_abcdefg[chr - '0'] & 1<<6)
		LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);

	if(seven_seg_digits_decode_abcdefg[chr - '0'] & 1<<5)
		LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);

	if(seven_seg_digits_decode_abcdefg[chr - '0'] & 1<<4)
		LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);

	if(seven_seg_digits_decode_abcdefg[chr - '0'] & 1<<3)
		LL_GPIO_ResetOutputPin(SEGMENTD_PORT, SEGMENTD_PIN);

	if(seven_seg_digits_decode_abcdefg[chr - '0'] & 1<<2)
		LL_GPIO_ResetOutputPin(SEGMENTE_PORT, SEGMENTE_PIN);

	if(seven_seg_digits_decode_abcdefg[chr - '0'] & 1<<1)
		LL_GPIO_ResetOutputPin(SEGMENTF_PORT, SEGMENTF_PIN);

	if(seven_seg_digits_decode_abcdefg[chr - '0'] & 1<<0)
		LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
}

void setDP(void)
{
	LL_GPIO_ResetOutputPin(SEGMENTDP_PORT, SEGMENTDP_PIN);
}


void displaySign(char *c, int indexMain, int switch_stateMain)
{
	for(int j=0; j<4; j++)
	{
		dataDisplay[j] = *c;
		c++;
	}

	pom = indexMain;
	state = switch_stateMain;


}

/*
 * Turns required digit ON
 */
void setDigit(uint8_t pos)
{
	switch(pos)
	{
		case 0:
			DIGIT_1_ON;
			break;
		case 1:
			DIGIT_2_ON;
			break;
		case 2:
			DIGIT_3_ON;
			break;
		case 3:
			DIGIT_4_ON;
			break;
	}
}

/**
 * Display data in dDisplayData.
 * Sets every digit to display its value and decimal point.
 */
void updateDisplay(void)
{

	for(uint8_t i = 0; i < 4; i++)
	{
		setDigit(i);
		decode_7seg(dataDisplay[i]);

		if(state == 0)
		{
			if(pom == 3 && i == 3)
				setDP();

			if(pom == 4 && i == 2)
				setDP();
		}


		if(state == 2)
		{
			if(pom == 3 && i == 3)
				setDP();

			if(pom == 4 && i == 2)
				setDP();

			if(pom == 5 && i == 1)
				setDP();
		}

		if(state == 3)
		{
			if(pom == 3 && i == 3)
				setDP();

			if(pom == 4 && i == 2)
				setDP();
		}

		disp_time_saved = disp_time;
		while((disp_time_saved + 2) > disp_time){};

		resetDigits();
		resetSegments();
	}
}

//Update displayed data and keep display ON
void TIM3_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIM3))
	{
		updateDisplay();
	}

	LL_TIM_ClearFlag_UPDATE(TIM3);
}

