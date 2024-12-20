/*
 * Action.c
 *
 *  Created on: Oct 26, 2024
 *      Author: Taolin
 */

#include "main.h"
#include "Action.h"
/**
 * @brief 解析结果变量，如需跨文件调用，需要外部声明
 */
uint8_t Action_rxdata = 0;
float pos_x = 0;
float pos_y = 0;
float zangle = 0;
float xangle = 0;
float yangle = 0;
float w_z = 0;

void Action_It_Init(void)
{
	HAL_UART_Receive_IT(&huart9, &Action_rxdata, 1);
}

/**
 * @brief 数据解析函数  如更换MCU平台或更换软件库，只需将串口接收到的值传入该函数即可解析
 * @param  rec 串口接收到的字节数据
 */
void Data_Analyse(uint8_t rec)
{
	static uint8_t ch;
	static union
	{
		uint8_t date[24];
		float ActVal[6];
	} posture;
	static uint8_t count = 0;
	static uint8_t i = 0;

	ch = rec;
//	HAL_UART_Transmit(&huart10, &ch, 1, 0xff);
//	HAL_UART_Transmit(&huart10, (uint8_t*) ch, 1, 0xff);
	switch (count)
	{
	case 0:
		if (ch == 0x0d)
			count++;
		else
			count = 0;
		break;
	case 1:
		if (ch == 0x0a)
		{
			i = 0;
			count++;
		}
		else if (ch == 0x0d)
			;
		else
			count = 0;
		break;
	case 2:
		posture.date[i] = ch;
		i++;
		if (i >= 24)
		{
			i = 0;
			count++;
		}
		break;
	case 3:
		if (ch == 0x0a)
			count++;
		else
			count = 0;
		break;
	case 4:
		if (ch == 0x0d)
		{
			zangle = posture.ActVal[0];
			xangle = posture.ActVal[1];
			yangle = posture.ActVal[2];
			pos_x = posture.ActVal[3];
			pos_y = posture.ActVal[4];
			w_z = posture.ActVal[5];
		}
		count = 0;
		break;
	default:
		count = 0;
		break;
	}
}

