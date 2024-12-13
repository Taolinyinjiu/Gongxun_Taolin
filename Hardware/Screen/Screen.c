/*
 * Screen.c
 *
 *  Created on: Oct 26, 2024
 *      Author: Taolin
 */
#include "Screen.h"
#include "stm32h7xx_hal.h"
#include "usart.h"
#include "stdio.h"
#include "stdlib.h"
#include "Action/Action.h"
#include "Location/Location.h"
#include "string.h"

char Z_str[8];
char X_srt[20];
char Y_srt[20];
char X_pid[20];
char Y_pid[20];
char TX2_X_Point[8];
char TX2_Y_Point[8];
char TX2_Color[2];

uint8_t System_Flag = 0;
uint8_t Joy_Flag = 0;
uint8_t Ready_Flag = 0;
// 接收二维码数据
extern char QR_data[6];	//123+321+'\0'
// 接收点位数据
//extern char RxBuffer[RXBUFFERSIZE];
extern float x;
extern float y;
extern int colour;
extern char Point_Flag;

void Action_Show(void) {
	sprintf(Z_str, "%.2f", zangle);
	sprintf(X_srt, "%.2f", pos_x);
	sprintf(Y_srt, "%.2f", pos_y);
	HAL_UART_Transmit(&huart4, (uint8_t*) "Z_Angle.txt=\"",
			sizeof("Z_Angle.txt=\"") - 1, 0xffff);
	HAL_UART_Transmit(&huart4, (uint8_t*) Z_str, sizeof(X_srt) - 1, 0xffff);
	HAL_UART_Transmit(&huart4, (uint8_t*) "\"\xff\xff\xff",
			sizeof("\"\xff\xff\xff") - 1, 0xffff);

	HAL_UART_Transmit(&huart4, (uint8_t*) "X_Point.txt=\"",
			sizeof("X_Point.txt=\"") - 1, 0xffff);
	HAL_UART_Transmit(&huart4, (uint8_t*) X_srt, sizeof(X_srt) - 1, 0xffff);
	HAL_UART_Transmit(&huart4, (uint8_t*) "\"\xff\xff\xff",
			sizeof("\"\xff\xff\xff") - 1, 0xffff);

	HAL_UART_Transmit(&huart4, (uint8_t*) "Y_Point.txt=\"",
			sizeof("Y_Point.txt=\"") - 1, 0xffff);
	HAL_UART_Transmit(&huart4, (uint8_t*) Y_srt, sizeof(X_srt) - 1, 0xffff);
	HAL_UART_Transmit(&huart4, (uint8_t*) "\"\xff\xff\xff",
			sizeof("\"\xff\xff\xff") - 1, 0xffff);
}
//extern int QR_data[6];
void QR_Show(void) {
//	for (int i = 0; i < 6; i++) {
//		sprintf(&Y_pid[i], "%d", QR_data[i]);
//	}
//	HAL_UART_Transmit(&huart4, (uint8_t*) "QR.txt=\"", sizeof("QR.txt=\"") - 1,
//			0xffff);
//	HAL_UART_Transmit(&huart4, (uint8_t*) Y_pid, sizeof(Y_pid) - 1, 0xffff);
//	HAL_UART_Transmit(&huart4, (uint8_t*) "\"\xff\xff\xff",
//			sizeof("\"\xff\xff\xff") - 1, 0xffff);
//	HAL_Delay(20);

	 HAL_UART_Transmit(&huart4, (uint8_t*) "QR.txt=\"", sizeof("QR.txt=\"") - 1, 0xffff);
	 HAL_UART_Transmit(&huart4, (uint8_t*) QR_data, sizeof(QR_data) - 1, 0xffff);
	 HAL_UART_Transmit(&huart4, (uint8_t*) "\"\xff\xff\xff", sizeof("\"\xff\xff\xff") - 1, 0xffff);

}
//extern float x;
//extern uint8_t RxFlag;

//void Point_Show(void) {
////	点位的x与y均不为0，说明数据转换完成，数据有效
////	if (Point_Flag != 0) {
////		Point_Flag = 0;
////		HAL_UART_Transmit(&huart4, (uint8_t*) "Point.txt=\"",
////				sizeof("Point.txt=\"") - 1, 0xffff);
////		HAL_UART_Transmit(&huart4, (uint8_t*) RxBuffer + 1,
////				sizeof("333.1,333.1"), 0xffffff);
////		HAL_UART_Transmit(&huart10, (uint8_t*) RxBuffer + 1,
////				sizeof(RxBuffer + 1), 0xffffff);
////		HAL_UART_Transmit(&huart4, (uint8_t*) "\"\xff\xff\xff",
////				sizeof("\"\xff\xff\xff") - 1, 0xffff);
//	sprintf(color, "%d", RxFlag);
//	HAL_UART_Transmit(&huart4, (uint8_t*) "Point.txt=\"",
//			sizeof("Point.txt=\"") - 1, 0xffff);
//	HAL_UART_Transmit(&huart4, (uint8_t*) color, sizeof(color)-1,
//			0xffffff);
////	HAL_UART_Transmit(&huart10, (uint8_t*) RxBuffer + 1, sizeof(RxBuffer + 1),
////			0xffffff);
//	HAL_UART_Transmit(&huart4, (uint8_t*) "\"\xff\xff\xff",
//			sizeof("\"\xff\xff\xff") - 1, 0xffff);
////	}
//}


// 抓取过程标志位，值为目标颜色
extern char Match_Flag;
// 抓取计算稳定性标志位，0为识别有故障,1为正常计算
extern char Check_flag;
// 借用一下屏幕上的识别物料颜色，查看flag系统标志位
extern uint16_t flag;

// TODO：屏幕缺失字符"+",暂时判断为编码格式问题，需要通过串口助手确定编码格式
void Point_Show(void) {
//	if(Point_Flag == 0)
//	{
////		TX2点位数据还未接收，结束该函数
//		return ;
//	}
//	Point_Flag = 0;
//	在抓取过程中,进行颜色校验，如果颜色与抓取目标颜色不同，则放弃该帧数据，并传递放弃稳定性计算标志位
	if(Match_Flag != colour)
		Check_flag = 0;
	else
		Check_flag = 1;
	sprintf(TX2_X_Point, "%.2f", x);
	sprintf(TX2_Y_Point, "%.2f", y);
	sprintf(TX2_Color, "%d", flag);
//	开始发送一帧点位数据
	HAL_UART_Transmit(&huart4, (uint8_t*) "Point.txt=\"",sizeof("Point.txt=\"") - 1, 0xffff);
//	if(Choke_Flag == true)
//		HAL_UART_Transmit(&huart4, (uint8_t*) "true", sizeof("true") - 2, 0xffff);
//	else
//		HAL_UART_Transmit(&huart4, (uint8_t*) "false", sizeof("false") - 2, 0xffff);
	HAL_UART_Transmit(&huart4, (uint8_t*) TX2_X_Point, sizeof(TX2_X_Point) - 2, 0xffff);
	HAL_UART_Transmit(&huart4, (uint8_t*) "+", sizeof("+")-2, 0xffff);
	HAL_UART_Transmit(&huart4, (uint8_t*) TX2_Y_Point, sizeof(TX2_Y_Point) - 1, 0xffff);
//	结束发送一帧点位数据
	HAL_UART_Transmit(&huart4, (uint8_t*) "\"\xff\xff\xff",sizeof("\"\xff\xff\xff") - 1, 0xffff);

//	开始发送一帧颜色数据
	HAL_UART_Transmit(&huart4, (uint8_t*) "Color.txt=\"",sizeof("Color.txt=\"") - 1, 0xffff);

	HAL_UART_Transmit(&huart4, (uint8_t*) TX2_Color, sizeof(TX2_Color)-1 , 0xffff);

//	结束发送一帧颜色数据
	HAL_UART_Transmit(&huart4, (uint8_t*) "\"\xff\xff\xff",sizeof("\"\xff\xff\xff") - 1, 0xffff);
}

void Check_Flag(uint8_t rec) {
	static uint8_t ch;
	static uint8_t count = 0;

	ch = rec;

	switch (count) {
	case 0:
		if (ch == 'G')
			count = 1;
		else if (ch == 'R')
			count = 2;
		else if (ch == 'M')
			count = 4;
		else
			count = 0;
		break;
	case 1:
		if (ch == 'o') {
			System_Flag = 1;
			count = 0;
		} else if (ch == 'G')
			;
		else
			count = 0;
		break;
	case 2:
//		此处用于判断是否进入准备模式，调整车身姿态
		if (ch == 'e')
			count++;
		else
			count = 0;
		break;
	case 3:
		if (ch == 'a') {
			Ready_Flag = 1;
			count = 0;
		} else
			count = 0;
		break;
	case 4:
		if (ch == 'a')
			count++;
		else
			count = 0;
		break;
	case 5:
		if (ch == 't') {
			System_Flag = 1;
			count = 0;
		}
		break;
	default:
		count = 0;
		break;
	}

}
