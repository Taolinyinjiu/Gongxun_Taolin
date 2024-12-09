/*
 * Stability.c
 *
 *  Created on: Dec 9, 2024
 *      Author: Taolin
 */

#include "Stability.h"
#include "Stability_sub.h"

// 计算平均值
double calculate_mean(double data[], int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;
}

 // 计算方差
double calculate_variance(double data[], int n) {
     double mean = calculate_mean(data, n);
     double sum_of_squares = 0.0;
     for (int i = 0; i < n; i++) {
         sum_of_squares += (data[i] - mean) * (data[i] - mean);
     }
     return sum_of_squares / n;
}

//  使用方差判断稳定性
uint8_t Check_Stability(float check_x,float check_y)
{
	static float data_x[10];
	static float data_y[10];
	static uint8_t frequency = 0;
	if(frequency <= 9)
	{
		data_x[frequency] = check_x;
		data_y[frequency] = check_y;
//		0说明数据量过少
		return 0;
	}
//	显式类型转换，调用函数获得方差
	double variance_x = calculate_variance((double*)data_x, 10);
	double variance_y = calculate_variance((double*)data_y, 10);
	if((variance_x > 5) || (variance_y >5))
	{
//		1说明还未稳定
		return 1;
	}
	else
		return 2;
}

// 检查当前车身状态
/*
	首先，在步进电机上电以后，需要进行调零，找到一个零位用来确定后面一切操作的参考点，因此5号步进电机设置为上电回零
	但是在比赛过程中，要求车身及其垂直投影部分在未发车前全部处于发车区域，这就要求爪子在发车前是在车体内侧的，同时爪子不能处于太高处，否则违规
	因此在Check_Status()函数用于在回零操作已完成后进行调整，以满足发车条件，
*/
uint8_t Check_Status(void)
{
//  首先控制步进电机,回到零点
	Drop_Location_jiang(200, 120, 0);
	Move_Arm(1, 50, 300);
//  6号舵机900
	Move_Arm(6, 900, 300);
//  旋转物料结构的支撑舵机
	FT_Servo_Zero(0);
//  旋转爪子结构的支撑舵机
	FT_Servo_Zero(1);
//  步进电机下降,将位置调整为最最低点
	Drop_Location_jiang(200, 120, 9000);
	return 1;
}

// 将目标舵机回零
uint8_t FT_Servo_Zero(uint8_t Number)
{
	if(Number == 0)
		WritePosEx(0, 0, 0, 0);
	else if (Number == 1)
		WritePosEx(1, 1020, 0, 0);
	return 1;
}

// 将爪子处飞特舵机调整为向外，正交于车身
uint8_t FT_Servo_Orth(void)
{
	WritePosEx(1, 3087, 0, 0);
	return 1;
}

// 物料摆放
uint8_t FT_Servo_Put(uint8_t Servo_ID, uint8_t Pub_Number)
{
	if(Servo_ID == 0)
	{
		switch (Pub_Number)
		{
			case 1:WritePosEx(0, 1180, 0, 0);break;
			case 2:WritePosEx(0, 2245, 0, 0);break;
			case 3:WritePosEx(0, 3292, 0, 0);break;
		}
	}
	else if (Servo_ID == 1)
	{
		WritePosEx(1, 1716, 0, 0);
	}
	while(ReadMove(Servo_ID))
	{
		;
	}
	return 1;
}

uint8_t FT_Servo_Wait_Zero(uint8_t ID)
{
//	查询回零状态
	Emm_mode = 2;
	Emm_V5_Get_Origin_Status(ID);
	while((RxState == 0)&&(Emm_mode =0))
	{
		;
	}
//	在查询完成后，Zero_data的值应该会被刷新，如果Zero_data不是0，说明在回零过程中
	while(Zero_data)
	{
		Emm_mode = 2;
		Emm_V5_Get_Origin_Status(ID);
		HAL_Delay(20);
	}
	HAL_Delay(100);
	return 1;
}

// uint8_t Check_TX_Point()
// {

// }