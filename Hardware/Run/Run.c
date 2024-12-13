/*
 * Run.c
 *
 *  Created on: Nov 1, 2024
 *      Author: Taolin
 */

#include "Run.h"
#include "Bool.h"
#include "main.h"
#include "stdio.h"
/* ------ Work Log -------- */
/*
 TODO: 实现步进电机底盘运动的非阻塞函数




 */

// 阻塞操作标志位 false表示 非阻塞，true表示阻塞
bool Choke_Flag = false;
// 函数申请阻塞操作标志位 false表示 非阻塞，true表示阻塞
bool Apply_Chock = false;
// 接收回调进行检测
bool Call_Flag = false;
// 多机同步运动标志位
extern uint8_t broadcast_flag;

int time5_jiancha = 0;

uint8_t GetRxFlag(void)
{
	if (RxFlag == 1)
	{
		RxFlag = 0;
		return 1;
	}
	return 0;
}

/**
 * @brief 实现四个电机同步向前运动 Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true)
 * @param  第一列 电机地址
 * @param  第二列 方向 ，0为CW，其余值为CCW
 * @param  第三列 速度 ，范围0 - 5000RPM
 * @param  第四列 加速度 ，范围0 - 255，注意：0是直接启动
 * @param  第五列 脉冲数 ，范围0- (2^32 - 1)个
 * @param  第六列 相位/绝对标志，false为相对运动，true为绝对值运动
 * @param  第七列 snF ：多机同步标志 ，false为不启用，true为启用
 * @retval
 */
////0正传，1反转
////1.8度电机,16细分，3200个脉冲，电机转动一圈
////曲线加减速时间计算公式：t2 - t1 = (256 - acc) * 50(us)，Vt2 = Vt1 + 1(RPM)；
////（注：acc 为加速度档位，Vt1为 t1 时刻的转速，，Vt2为 t2 时刻的转速）
/////////////////////////////////////////////////////////////////////////////////////////////
//后续根据实物再进行调整
//void Move_Line(u16 speed, u8 acc, u32 length)  //初始设定130.5个脉冲是一厘米
//{
//	int time;
//	time = length / speed * 20;  //毫秒
//	Emm_V5_Pos_Control(1, 0, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(2, 0, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(3, 0, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(4, 0, speed, acc, length, 0, true);
//
//	Emm_V5_Synchronous_motion();  // 触发多机同步开始运动
//	Emm_mode = 1;
//	time5_jiancha = (time + 800) / 10;
//	while ((GetRxFlag() == 0)&& (time5_jiancha != 0) )
//		;
////	RxFlag = 0;·
//	HAL_Delay(30);
//}
bool Move_Line(u16 speed, u8 acc, u32 length)
{
	// 查询当前是否有阻塞操作,有阻塞操作，结束函数并返回false
	if (Choke_Flag == true)
		return false;
	else
		Choke_Flag = true;
	// 无阻塞操作，申请阻塞
	Apply_Chock = true;
	// 底盘步进电机移动
	Emm_V5_Pos_Control(1, 0, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(2, 0, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(3, 0, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(4, 0, speed, acc, length, 0, true);
	// 触发多机同步开始运动
	Emm_V5_Synchronous_motion();
	HAL_Delay(1000);
	while(broadcast_flag == 0){
		// 在broadcast_flag = 1时跳出去
		// 底盘步进电机移动
		Emm_V5_Pos_Control(1, 0, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(2, 0, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(3, 0, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(4, 0, speed, acc, length, 0, true);
		// 触发多机同步开始运动
		Emm_V5_Synchronous_motion();
	}
	broadcast_flag = 0;
	// 使能阻塞定时器中断
	HAL_TIM_Base_Start_IT(&htim12);
	Call_Flag = true;
	return true;
}
bool Move_Left(u16 speed, u8 acc, u32 length)
{
	// 查询当前是否有阻塞操作,有阻塞操作，结束函数并返回false
	if (Choke_Flag == true)
		return false;
	else
		Choke_Flag = true;
	// 无阻塞操作，申请阻塞
	Apply_Chock = true;
	// 底盘步进电机移动
	Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(2, 0, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(3, 1, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(4, 0, speed, acc, length, 0, true);
	// 触发多机同步开始运动
	Emm_V5_Synchronous_motion();
	HAL_Delay(1000);
	while(broadcast_flag == 0){
		// 在broadcast_flag = 1时跳出去
		// 底盘步进电机移动
		Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(2, 0, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(3, 1, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(4, 0, speed, acc, length, 0, true);
		// 触发多机同步开始运动
		Emm_V5_Synchronous_motion();
	}
	broadcast_flag = 0;
	// 使能阻塞定时器中断
	HAL_TIM_Base_Start_IT(&htim12);
	Call_Flag = true;
	return true;
}

bool Move_Back(u16 speed, u8 acc, u32 length)
{
	// 查询当前是否有阻塞操作,有阻塞操作，结束函数并返回false
	if (Choke_Flag == true)
		return false;
	else
		Choke_Flag = true;
	// 无阻塞操作，申请阻塞
	Apply_Chock = true;
	// 底盘步进电机移动
	Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(2, 1, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(3, 1, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(4, 1, speed, acc, length, 0, true);
	// 触发多机同步开始运动
	Emm_V5_Synchronous_motion();
	HAL_Delay(1000);
	while(broadcast_flag == 0){
		// 在broadcast_flag = 1时跳出去
		// 底盘步进电机移动
		Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(2, 1, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(3, 1, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(4, 1, speed, acc, length, 0, true);
		// 触发多机同步开始运动
		Emm_V5_Synchronous_motion();
	}
	broadcast_flag = 0;
	// 使能阻塞定时器中断
	HAL_TIM_Base_Start_IT(&htim12);
	Call_Flag = true;
	return true;
}

bool Move_fanxuzhuan(u16 speed, u8 acc, u32 length)
{
	// 查询当前是否有阻塞操作,有阻塞操作，结束函数并返回false
	if (Choke_Flag == true)
		return false;
	else
		Choke_Flag = true;
	// 无阻塞操作，申请阻塞
	Apply_Chock = true;
	// 底盘步进电机移动
	Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(2, 0, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(3, 0, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(4, 1, speed, acc, length, 0, true);
	// 触发多机同步开始运动
	Emm_V5_Synchronous_motion();
	HAL_Delay(1000);
	while(broadcast_flag == 0){
		// 在broadcast_flag = 1时跳出去
		// 底盘步进电机移动
		Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(2, 0, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(3, 0, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(4, 1, speed, acc, length, 0, true);
		// 触发多机同步开始运动
		Emm_V5_Synchronous_motion();
	}
	broadcast_flag = 0;
	// 使能阻塞定时器中断
	HAL_TIM_Base_Start_IT(&htim12);
	Call_Flag = true;
	return true;
}

// void Move_Left(u16 speed, u8 acc, u32 length) {
// 	int time;
// 	time = length / speed * 20;  //毫秒
// 	// Emm_V5_Pos_Control是非阻塞函数
// 	// Move_Left是阻塞函数
// 	Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true);
// 	Emm_V5_Pos_Control(2, 0, speed, acc, length, 0, true);
// 	Emm_V5_Pos_Control(3, 1, speed, acc, length, 0, true);
// 	Emm_V5_Pos_Control(4, 0, speed, acc, length, 0, true);

// 	Emm_V5_Synchronous_motion(); // 触发多机同步开始运动

// 	Emm_mode = 1;
// 	time5_jiancha = (time + 800) / 10;
// 	while ((GetRxFlag() == 0) && (time5_jiancha != 0))
// 		;
// //	RxFlag = 0;
// 	HAL_Delay(30);
// }

// void Move_Back(u16 speed, u8 acc, u32 length) {
// 	int time;
// 	time = length / speed * 20;  //毫秒
// 	Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true);
// 	Emm_V5_Pos_Control(2, 1, speed, acc, length, 0, true);
// 	Emm_V5_Pos_Control(3, 1, speed, acc, length, 0, true);
// 	Emm_V5_Pos_Control(4, 1, speed, acc, length, 0, true);

// 	Emm_V5_Synchronous_motion(); // 触发多机同步开始运动

// 	Emm_mode = 1;
// 	time5_jiancha = (time + 800) / 10;
// 	while ((GetRxFlag() == 0) && (time5_jiancha != 0))
// 		;
// //	RxFlag = 0;
// 	HAL_Delay(30);
// }

bool Move_Right(u16 speed, u8 acc, u32 length)
{
	// 查询当前是否有阻塞操作,有阻塞操作，结束函数并返回false
	if (Choke_Flag == true)
		return false;
	else
		Choke_Flag = true;
	// 无阻塞操作，申请阻塞
	Apply_Chock = true;
	// 底盘步进电机移动
	Emm_V5_Pos_Control(1, 0, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(2, 1, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(3, 0, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(4, 1, speed, acc, length, 0, true);
	// 触发多机同步开始运动
	Emm_V5_Synchronous_motion();
	HAL_Delay(1000);
	while(broadcast_flag == 0){
		// 在broadcast_flag = 1时跳出去
//		HAL_Delay(200);
		// 底盘步进电机移动
		Emm_V5_Pos_Control(1, 0, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(2, 1, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(3, 0, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(4, 1, speed, acc, length, 0, true);
		// 触发多机同步开始运动
		Emm_V5_Synchronous_motion();
	}
	broadcast_flag = 0;
	// 使能阻塞定时器中断
	HAL_TIM_Base_Start_IT(&htim12);
	Call_Flag = true;
	return true;
}
//void Move_Right(u16 speed, u8 acc, u32 length) {
//	int time;
//	time = length / speed * 20;  //毫秒
//	Emm_V5_Pos_Control(1, 0, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(2, 1, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(3, 0, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(4, 1, speed, acc, length, 0, true);
//
//	Emm_V5_Synchronous_motion();
//
//
//	Emm_mode = 1;
//	time5_jiancha = (time + 800) / 10;
//	while ((GetRxFlag() == 0) && (time5_jiancha != 0))
//		;
////	RxFlag = 0;
//	HAL_Delay(30);
//}

bool Move_zhengxuzhuan(u16 speed, u8 acc, u32 length)
{
	// 查询当前是否有阻塞操作,有阻塞操作，结束函数并返回false
	if (Choke_Flag == true)
		return false;
	else
		Choke_Flag = true;
	// 无阻塞操作，申请阻塞
	Apply_Chock = true;
	// 底盘步进电机移动
	Emm_V5_Pos_Control(1, 0, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(2, 1, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(3, 1, speed, acc, length, 0, true);
	Emm_V5_Pos_Control(4, 0, speed, acc, length, 0, true);
	// 触发多机同步开始运动
	Emm_V5_Synchronous_motion();
	HAL_Delay(1000);
	while(broadcast_flag == 0){
		// 在broadcast_flag = 1时跳出去
//		HAL_Delay(200);
		// 底盘步进电机移动
		Emm_V5_Pos_Control(1, 0, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(2, 1, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(3, 1, speed, acc, length, 0, true);
		Emm_V5_Pos_Control(4, 0, speed, acc, length, 0, true);
		// 触发多机同步开始运动
		Emm_V5_Synchronous_motion();
	}
	broadcast_flag = 0;
	// 使能阻塞定时器中断
	HAL_TIM_Base_Start_IT(&htim12);
	Call_Flag = true;
	return true;
}
//void Move_zhengxuzhuan(u16 speed, u8 acc, u32 length) {
//	int time;
//	time = length / speed * 20;  //毫秒
//	Emm_V5_Pos_Control(1, 0, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(2, 1, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(3, 1, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(4, 0, speed, acc, length, 0, true);
//
//	Emm_V5_Synchronous_motion(); // 触发多机同步开始运动
//
//	Emm_mode = 1;
//	time5_jiancha = (time + 800) / 10;
//	while ((GetRxFlag() == 0) && (time5_jiancha != 0))
//		;
////	RxFlag = 0;
//	HAL_Delay(30);
//}

//void Move_fanxuzhuan(u16 speed, u8 acc, u32 length) //4340是90度
//{
//	int time;
//	time = length / speed * 20;  //毫秒
//
//	Emm_V5_Pos_Control(1, 1, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(2, 0, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(3, 0, speed, acc, length, 0, true);
//	Emm_V5_Pos_Control(4, 1, speed, acc, length, 0, true);
//
//	Emm_V5_Synchronous_motion(); // 触发多机同步开始运动
//
//
//	Emm_mode = 1;
//	time5_jiancha = (time + 800) / 10;
//	while ((GetRxFlag() == 0) && (time5_jiancha != 0))
//		;
////	RxFlag = 0;
//	HAL_Delay(30);
//}
uint16_t High_Length = 0;
//void Move_Xiajiang(u16 speed, u8 acc, u32 length) {
//	int time;
//	time = length / speed * 20;  //毫秒
//	High_Length+=length;
//	Emm_V5_Pos_Control(5, 1, speed, acc, length, 1, 0);
//
//
//	Emm_mode = 1;
//	time5_jiancha = (time + 800) / 10;
//	while ((GetRxFlag() == 0) && (time5_jiancha != 0))
//		;
////	RxFlag = 0;
//	if(High_Length>=14000)
//		Emm_V5_Stop_Now(5,0);
//	HAL_Delay(30);
//}
void Drop_Location_jiang(u16 speed, u8 acc, u32 length)
{
	int time;
	time = length / speed * 20;  //毫秒
	Emm_V5_Pos_Control(5, 1, speed, acc, length, 1, 0);
//	检查到位回读
	Emm_mode = 1;
	time5_jiancha = (time + 800) / 10;
	while ((GetRxFlag() == 0) && (time5_jiancha != 0))
		;
	HAL_Delay(30);
}
void Drop_Location_Sheng(u16 speed, u8 acc, u32 length)
{
	int time;
	time = length / speed * 20;  //毫秒
	Emm_V5_Pos_Control(5, 1, speed, acc, length, 1, 0);
	Emm_mode = 1;
	time5_jiancha = (time + 800) / 10;
	while ((GetRxFlag() == 0) && (time5_jiancha != 0))
		;

	HAL_Delay(30);
}
/////////////////////////////////////////////////////////////////////////////////////

/***********将当前位置清零**********************/
void motor_clear_all(void)
{
	Emm_V5_Reset_CurPos_To_Zero(1);
	Emm_V5_Reset_CurPos_To_Zero(2);
	Emm_V5_Reset_CurPos_To_Zero(3);
	Emm_V5_Reset_CurPos_To_Zero(4);
}
/*********************************************/

/******************______速度模式控制______***************************/
void Move_Val_Line(u16 speed, u8 acc)
{
	Emm_V5_Vel_Control(1, 0, speed, acc, true);
	Emm_V5_Vel_Control(2, 0, speed, acc, true);
	Emm_V5_Vel_Control(3, 0, speed, acc, true);
	Emm_V5_Vel_Control(4, 0, speed, acc, true);
	Emm_V5_Synchronous_motion();  // 触发多机同步开始运动

}
void Move_Val_Back(u16 speed, u8 acc)
{
	Emm_V5_Vel_Control(1, 1, speed, acc, true);
	Emm_V5_Vel_Control(2, 1, speed, acc, true);
	Emm_V5_Vel_Control(3, 1, speed, acc, true);
	Emm_V5_Vel_Control(4, 1, speed, acc, true);
	Emm_V5_Synchronous_motion();  // 触发多机同步开始运动

}
void Move_Val_Left(u16 speed, u8 acc)
{
	Emm_V5_Vel_Control(1, 1, speed, acc, true);
	Emm_V5_Vel_Control(2, 0, speed, acc, true);
	Emm_V5_Vel_Control(3, 1, speed, acc, true);
	Emm_V5_Vel_Control(4, 0, speed, acc, true);
	Emm_V5_Synchronous_motion();  // 触发多机同步开始运动

}
void Move_Val_Right(u16 speed, u8 acc)
{
	Emm_V5_Vel_Control(1, 0, speed, acc, true);
	Emm_V5_Vel_Control(2, 1, speed, acc, true);
	Emm_V5_Vel_Control(3, 0, speed, acc, true);
	Emm_V5_Vel_Control(4, 1, speed, acc, true);
	Emm_V5_Synchronous_motion();  // 触发多机同步开始运动

}

void Move_Stop(void)
{
	Emm_V5_Stop_Now(1, true);
	Emm_V5_Stop_Now(2, true);
	Emm_V5_Stop_Now(3, true);
	Emm_V5_Stop_Now(4, true);
	Emm_V5_Synchronous_motion();  // 触发多机同步开始运动
}
/*********************_______END_________************************/
void Move_GO(void)
{
	int time;
	time = 5000 / RunSpeed * 20;  //毫秒
	Emm_V5_Pos_Control(1, 1, 0, 0, 0, 0, true);
	Emm_V5_Pos_Control(2, 0, RunSpeed, RunAcc, 5000, 0, true);
	Emm_V5_Pos_Control(3, 1, 0, 0, 0, 0, true);
	Emm_V5_Pos_Control(4, 0, RunSpeed, RunAcc, 5000, 0, true);

	Emm_V5_Synchronous_motion();

	RxFlag = 0;
	Emm_mode = 1;
	time5_jiancha = (time - 500) / 10;
	while ((GetRxFlag() == 0) && (time5_jiancha != 0))
		;
	HAL_Delay(30);

}
extern void Start(void);
void Move_TO_Saomaqu(float Leftclk, float Lineclk)
{
	Move_Left(RunSpeed, RunAcc, Leftclk);                             //爪子目前在最低处
	HAL_Delay(20);
//	HAL_Delay(yanshi);

	Move_Line(RunSpeed, RunAcc, Lineclk); ////130.5
//	HAL_Delay(yanshi);
}
void Move_TO_yuanliaoqu(float Lineclk) //物料区
{
	Move_Line(RunSpeed, RunAcc, Lineclk);
	HAL_Delay(yanshi);
}

void Move_TO_jianzhi1(float Backclk, float fanzhuanclk) //粗加工区
{
	HAL_UART_Transmit(&huart10, (uint8_t*) "我要后退喽\n", sizeof("我要后退喽\n") - 1,0xffff);
	bool temp = Move_Back(RunSpeed, RunAcc, Backclk);
	HAL_Delay(50);
	while (temp != true)
	{
		temp = Move_Back(RunSpeed, RunAcc, Backclk);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "确认接收\n", sizeof("确认接收\n") - 1,0xffff);
	HAL_UART_Transmit(&huart10, (uint8_t*) "我要转45度辣\n", sizeof("我要转45度辣\n") - 1,0xffff);
	temp = Move_fanxuzhuan(RunSpeed, RunAcc, fanzhuanclk);
	HAL_Delay(50);
	while (temp != true)
	{
		temp = Move_fanxuzhuan(RunSpeed, RunAcc, fanzhuanclk);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "确认接收\n", sizeof("确认接收\n") - 1,0xffff);
}

void Move_TO_zancunqu(float Lineclk, float fanzhuanclk) //暂存区
{
	HAL_UART_Transmit(&huart10, (uint8_t*) "向十字出发喽\n", sizeof("向十字出发喽\n") - 1,0xffff);
	bool temp = Move_Line(RunSpeed, RunAcc, Lineclk);
	HAL_Delay(50);
	while (temp != true)
	{
		temp = Move_Line(RunSpeed, RunAcc, Lineclk);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "收到\n", sizeof("收到\n") - 1,0xffff);
//	旋转
	HAL_UART_Transmit(&huart10, (uint8_t*) "喜不喜欢我的反旋转\n", sizeof("喜不喜欢我的反旋转\n") - 1,0xffff);
	temp = Move_fanxuzhuan(RunSpeed, RunAcc, fanzhuanclk);
	HAL_Delay(50);

	while (temp != true)
	{
		temp = Move_fanxuzhuan(RunSpeed, RunAcc, fanzhuanclk);
//		HAL_UART_Transmit(&huart10, (uint8_t*) "反选转的temp是false\n", sizeof("反选转的temp是false\n") - 1,0xffff);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "转个45度给你看看\n", sizeof("转个45度给你看看\n") - 1,0xffff);
}


void Move_TO_jianzhi2(float Backclk, float Zhengzhuanclk) //从暂存区回到原料区
{
	HAL_UART_Transmit(&huart10, (uint8_t*) "向粗加工区出发\n", sizeof("向粗加工区出发\n") - 1,0xffff);
	bool temp = Move_Back(RunSpeed, RunAcc, Backclk);
	HAL_Delay(50);
	while (temp != true)
	{
		temp = Move_Back(RunSpeed, RunAcc, Backclk);
//		HAL_UART_Transmit(&huart10, (uint8_t*) "后退的temp是false\n", sizeof("后退的temp是false\n") - 1,0xffff);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "到位\n", sizeof("到位\n") - 1,0xffff);
	HAL_UART_Transmit(&huart10, (uint8_t*) "来个正转给大家开开眼\n", sizeof("来个正转给大家开开眼\n") - 1,0xffff);
	temp = Move_zhengxuzhuan(RunSpeed, RunAcc, Zhengzhuanclk);
	HAL_Delay(50);
	while (temp != true)
	{
		temp = Move_zhengxuzhuan(RunSpeed, RunAcc, Zhengzhuanclk);
//		HAL_UART_Transmit(&huart10, (uint8_t*) "正旋转的temp是false\n", sizeof("正旋转的temp是false\n") - 1,0xffff);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "转晕了家人们\n", sizeof("转晕了家人们\n") - 1,0xffff);
}
void Move_TO_cujiagongqu(float Backclk) //粗加工区
{
	HAL_UART_Transmit(&huart10, (uint8_t*) "粗加工区好难走啊\n", sizeof("粗加工区好难走啊\n") - 1,0xffff);
	bool temp = Move_Back(RunSpeed, RunAcc, Backclk);
	HAL_Delay(50);
	while (temp != true)
	{
		temp = Move_Back(RunSpeed, RunAcc, Backclk);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "你说的对\n", sizeof("你说的对\n") - 1,0xffff);
	// Move_Back(RunSpeed, RunAcc, Backclk);
	// HAL_Delay(yanshi);
}
void Move_TO_jianzhi3(float Backclk, float Zhengzhuanclk) //从暂存区回到原料区
{

	HAL_UART_Transmit(&huart10, (uint8_t*) "到拐角了家人们\n", sizeof("到拐角了家人们\n") - 1,0xffff);
	bool temp = Move_Back(RunSpeed, RunAcc, Backclk);
	HAL_Delay(50);
	while (temp != true)
	{
		temp = Move_Back(RunSpeed, RunAcc, Backclk);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "滴滴\n", sizeof("滴滴\n") - 1,0xffff);
	// Move_Back(RunSpeed, RunAcc, Backclk);
	// HAL_Delay(yanshi);
	HAL_UART_Transmit(&huart10, (uint8_t*) "欸我再转\n", sizeof("欸我再转\n") - 1,0xffff);
	temp = Move_zhengxuzhuan(RunSpeed, RunAcc, Zhengzhuanclk);
	HAL_Delay(50);
	while (temp != true)
	{
		temp = Move_zhengxuzhuan(RunSpeed, RunAcc, Zhengzhuanclk);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "差不多要到了吧\n", sizeof("差不多要到了吧\n") - 1,0xffff);
//	 Move_zhengxuzhuan(RunSpeed, RunAcc, Zhengzhuanclk);
//	 HAL_Delay(yanshi);
}
void Move_TO_fanyuanliaoqu(float Backclk) //物料区
{
	HAL_UART_Transmit(&huart10, (uint8_t*) "回到了梦开始的地方\n", sizeof("回到了梦开始的地方\n") - 1,0xffff);
	bool temp = Move_Back(RunSpeed, RunAcc, Backclk);
	HAL_Delay(50);
	while (temp != true)
	{
		temp = Move_Back(RunSpeed, RunAcc, Backclk);
	}
	// Move_Back(RunSpeed, RunAcc, Lineclk);
	// HAL_Delay(20000);
}

// void Move_Action_Left_Ctrl(void) {

// 	if (X_out > 0) {
// 		Move_Left(100, 100, X_out);
// 		HAL_Delay(yanshi);
// 	} else {
// 		Move_Right(100, 100, -X_out);
// 		HAL_Delay(yanshi);
// 	}
// 	if (Y_out > 0) {
// 		Move_Line(100, 100, Y_out);
// 		HAL_Delay(yanshi);
// 	} else {
// 		Move_Back(100, 100, -Y_out);
// 		HAL_Delay(yanshi);
// 	}
// }

// void Move_Action_Forward_Ctrl(void) {

// 	if (X_out > 0) {
// 		bool temp = Move_Line(100, 100, X_out);
// 		while(temp != true)
// 		{
// 			temp = Move_Line(100, 100, X_out);
// 		}
// 	} else {
// 		temp = Move_Back(100, 100, -X_out);
// 		while(temp != true)
// 		{
// 			temp = Move_Back(100, 100, -X_out);
// 		}
// 		// Move_Back(100, 100, -X_out);
// 		// HAL_Delay(yanshi);
// 	}
// 	if (Y_out > 0) {
// 		temp = Move_Right(100, 100, Y_out);
// 		while(temp != true)
// 		{
// 			temp = Move_Right(100, 100, Y_out);
// 		}
// 		// Move_Right(100, 100, Y_out);
// 		// HAL_Delay(yanshi);
// 	} else {
// 		temp = Move_Left(100, 100, -Y_out);
// 		while(temp != true)
// 		{
// 			temp = Move_Left(100, 100, -Y_out);
// 		}
// 		// Move_Left(100, 100, -Y_out);
// 		// HAL_Delay(yanshi);
// 	}

// }
// void Move_Action_Right_Ctrl(void) {
// 	if (X_out > 0) {

// 		Move_Right(100, 100, X_out);
// 		HAL_Delay(yanshi);
// 	} else {
// 		Move_Left(100, 100, -X_out);
// 		HAL_Delay(yanshi);
// 	}
// 	if (Y_out > 0) {
// 		Move_Back(100, 100, Y_out);
// 		HAL_Delay(yanshi);
// 	} else {
// 		Move_Line(100, 100, -Y_out);
// 		HAL_Delay(yanshi);
// 	}
// }
#define		ABS(x)		((x) > 0 ? (x) : -(x))
#define Action_Speed 100
#define Action_Acc 100

// void put_location(float x_goal, float y_goal) {
// 	if ((X_NOW - x_goal) < 0) {

// 		Move_Right(100, 100, ABS(X_NOW - x_goal) * 13.8);
// 		HAL_Delay(yanshi);
// 	} else {
// 		Move_Left(100, 100, ABS(X_NOW - x_goal) * 13.8);
// 		HAL_Delay(yanshi);
// 	}
// 	if ((Y_NOW - y_goal) < 0) {
// 		Move_Back(100, 100, ABS(Y_NOW - y_goal) * 13.8);
// 		HAL_Delay(yanshi);
// 	} else {
// 		Move_Line(100, 100, ABS(Y_NOW - y_goal) * 13.8);
// 		HAL_Delay(yanshi);
// 	}
// }

//这个函数是爪子朝向左边时，也就是在原料区进行ACTION调整的
void Move_Action_Nopid_Left_Ctrl(float x_goal, float y_goal)
{
	while (1)
	{
//		取X,Y差值
		uint16_t X_Diff = (uint16_t)ABS(X_NOW - x_goal);
		uint16_t Y_Diff = (uint16_t)ABS(Y_NOW - y_goal);
//		差值满足要求, 结束调节
		if(( X_Diff <  10 ) && ( Y_Diff < 10 ))
			return;

		// 当前X坐标小于目标X
		if ((X_NOW - x_goal) < 0)
		{
			bool temp = Move_Left(Action_Speed, Action_Acc, X_Diff * 5);
			while (temp != true)
			{
				temp = Move_Left(Action_Speed, Action_Acc,X_Diff * 5);
			}
		}
		// 当前X坐标大于目标X
		else if ((X_NOW - x_goal) > 0)
		{
			bool temp = Move_Right(Action_Speed, Action_Acc, X_Diff * 5);
			while (temp != true)
			{
				temp = Move_Right(Action_Speed, Action_Acc,X_Diff * 5);
			}
		}
		// 当前Y坐标小于目标Y
		if (Y_NOW - y_goal < 0)
		{
			bool temp = Move_Line(Action_Speed, Action_Acc,Y_Diff * 5);
			while (temp != true)
			{
				temp = Move_Line(Action_Speed, Action_Acc,Y_Diff * 5);
			}
		}
		// 当前Y坐标大于目标Y
		else if (Y_NOW - y_goal > 0)
		{
			bool temp = Move_Back(Action_Speed, Action_Acc,Y_Diff * 5);
			while (temp != true)
			{
				temp = Move_Back(Action_Speed, Action_Acc,Y_Diff* 5);
			}
		}

	}
}
//这个函数是爪子朝向前方时，第一个十字和粗加工区进行ACTION调整
bool Move_Action_Nopid_Forward_Ctrl(float x_goal, float y_goal)
{
	//ACTION调整
	//		取X,Y差值
	uint16_t X_Diff = (uint16_t)ABS(X_NOW - x_goal);
	uint16_t Y_Diff = (uint16_t)ABS(Y_NOW - y_goal);
	char X_send[8];
	char Y_send[8];
	sprintf(X_send, "%d", X_Diff);
	sprintf(Y_send, "%d", Y_Diff);
	if( X_Diff <  10 ){
		HAL_UART_Transmit(&huart10, (uint8_t*) "X_Diff = ", sizeof("X_Diff = ") - 2,0xffff);
		HAL_UART_Transmit(&huart10, (uint8_t*) X_send, sizeof(X_send) - 1,0xffff);
		HAL_UART_Transmit(&huart10, (uint8_t*) "\n", sizeof("\n= ") - 1,0xffff);
	}
	if( Y_Diff <  10 ){
		HAL_UART_Transmit(&huart10, (uint8_t*) "Y_Diff = ", sizeof("X_Diff = ") - 2,0xffff);
		HAL_UART_Transmit(&huart10, (uint8_t*) Y_send, sizeof(Y_send) - 1,0xffff);
		HAL_UART_Transmit(&huart10, (uint8_t*) "\n", sizeof("\n= ") - 1,0xffff);
	}
	if(( X_Diff <  10 ) && ( Y_Diff < 10 ))
		return true;
//	X差值不满足要求
	if( X_Diff >=  10 )
	{
//		X与当前目标值差值小于0
		if ((X_NOW - x_goal) < 0)
		{
			bool temp = Move_Line(Action_Speed, Action_Acc, X_Diff* 3);
			HAL_Delay(5);
			while (temp != true)
			{
				temp = Move_Line(Action_Speed, Action_Acc, X_Diff * 3);
			}
			return false;
		}
			// 当前X坐标大于目标X
		else if ((X_NOW - x_goal) > 0)
		{
			bool temp = Move_Back(Action_Speed, Action_Acc, X_Diff * 3);
			HAL_Delay(5);
			while (temp != true)
			{
				temp = Move_Back(Action_Speed, Action_Acc, X_Diff * 3);
			}
			return false;
		}
	}
	if ( Y_Diff >= 10 )
	{
		// 当前Y坐标小于目标Y
		if ((Y_NOW - y_goal) < 0)
		{
			bool temp = Move_Right(Action_Speed, Action_Acc,Y_Diff * 3);
			HAL_Delay(5);
			while (temp != true)
			{
				temp = Move_Right(Action_Speed, Action_Acc, Y_Diff * 3);
			}
			return false;
		}
		// 当前Y坐标大于目标Y
		else if (Y_NOW - y_goal > 0)
		{
			bool temp = Move_Left(Action_Speed, Action_Acc, Y_Diff * 3);
			HAL_Delay(5);
			while (temp != true)
			{
				temp = Move_Left(Action_Speed, Action_Acc, Y_Diff * 3);
			}
			return false;
		}
	}
	return false;
}


void Move_Action_Nopid_Right_Ctrl(float x_goal, float y_goal)
{
	//ACTION调整
	//		取X,Y差值
	uint16_t X_Diff = (uint16_t)ABS(X_NOW - x_goal);
	uint16_t Y_Diff = (uint16_t)ABS(Y_NOW - y_goal);
	while (1)
	{
		if (( X_Diff < 5 ) && ( Y_Diff < 5))
		{
			break; /* 到达目标 */
		}
		// 当前X坐标小于目标X
		if ((X_NOW - x_goal) < 0)
		{
			bool temp = Move_Right(Action_Speed, Action_Acc, X_Diff * 13.8);
			while (temp != true)
			{
				temp = Move_Right(Action_Speed, Action_Acc, X_Diff * 13.8);
			}
		}
		// 当前X坐标大于目标X
		else
		{
			bool temp = Move_Left(Action_Speed, Action_Acc, X_Diff * 13.8);
			while (temp != true)
			{
				temp = Move_Left(Action_Speed, Action_Acc, X_Diff * 13.8);
			}
		}
		// 当前Y坐标小于目标Y
		if ((Y_NOW - y_goal) < 0)
		{
			bool temp = Move_Back(Action_Speed, Action_Acc,Y_Diff * 13.8);
			while (temp != true)
			{
				temp = Move_Back(Action_Speed, Action_Acc, Y_Diff * 13.8);
			}
		}
		// 当前Y坐标大于目标Y
		else
		{
			bool temp = Move_Line(Action_Speed, Action_Acc, Y_Diff * 13.8);
			while (temp != true)
			{
				temp = Move_Line(Action_Speed, Action_Acc, Y_Diff * 13.8);
			}
		}
	}
}

/* Tx姿态调整 */
//
uint16_t time_tx = 0;
void Move_Tx_Pid_Ctrl(float TX_X_Goal, float TX_Y_Goal)
{

	while (1)
	{
		if (TX_Y_out >= 0)
		{
			bool temp = Move_Right(100, 120, TX_Y_out * 5);
			while (temp != true)
			{
				temp = Move_Right(100, 120, TX_Y_out * 5);
			}
			// Move_Right(80, 80, TX_Y_out * 5);
			// HAL_Delay(5);
		}
		else
		{
			bool temp = Move_Left(100, 120, -TX_Y_out * 5);
			while (temp != true)
			{
				temp = Move_Left(100, 120, -TX_Y_out * 5);
			}
			// Move_Left(80, 80, -TX_Y_out * 5);
			// HAL_Delay(5);
		}

		if (TX_X_out >= 0)
		{
			bool temp = Move_Back(100, 120, TX_X_out * 5);
			while (temp != true)
			{
				temp = Move_Back(100, 120, TX_X_out * 5);
			}
			// Move_Back(80, 80, TX_X_out * 5);
			// HAL_Delay(5);
		}
		else
		{
			bool temp = Move_Line(100, 120, -TX_X_out * 5);
			while (temp != true)
			{
				temp = Move_Line(100, 120, -TX_X_out * 5);
			}
			// Move_Line(80, 80, -TX_X_out * 5);
			// HAL_Delay(5);
		}

		if (ABS(Tx_NOW - 311.5) < 5 && ABS(Ty_NOW -125.5) < 5)
		{
			break;
		}
		//这里是防止调节时间过长所加的
		// if(time_tx>=250)
		// {time_tx=0;
		// 	break;
		// }
	}

}

/* -----查询爪子处步进电机是否到位 -----*/
//// 爪子处步进电机阻塞标志位
//bool Yaw_Choke_Flag;
//
//// 申请五号步进电机进行阻塞
//bool Apply_Yaw_Choke(void)
//{
//	if(Yaw_Choke_Flag == true)
//		return  true;
//	else
//		Yaw_Choke_Flag = true;
//	while(Status == false)
//	{
//		// 申请失败,我再来看看怎么个事儿
//		Status = Apply_Yaw_Choke();
//	}
//	return false;
//}
//bool Get_Paw_Status(void)
//{
//	bool Status = Apply_Yaw_Choke();
//
//
//}
// void Move_Tx_NO_Pid_Ctrl(float x2 ,float y2) {
// 	while (1) {
// 	if ((Tx_NOW - x2)< 0) {
// 			Move_Right(80, 80, ABS(Tx_NOW - x2) * 10);
// 			HAL_Delay(yanshi);
// 		} else {
// 			Move_Left(80, 80, ABS(Tx_NOW - x2) * 10);
// 			HAL_Delay(yanshi);
// 		}
// 		if ((Ty_NOW - y2) < 0) {
// 			Move_Back(80, 80, ABS(Ty_NOW - y2) * 10);
// 			HAL_Delay(yanshi);
// 		} else {
// 			Move_Line(80, 80, ABS(Ty_NOW - y2) * 10 );
// 			HAL_Delay(yanshi);
// 		}
// 		if (ABS(Tx_NOW - x2) < 2 && ABS(Ty_NOW - y2) < 2) {
// 			break;
// 		}
// 	}
// }
// void Move_Tx_Val_Ctrl(float* tx_target,float* ty_target,float x1,float y1) {
// 	 if (tx_target != NULL && ty_target != NULL)
// 	 {
// 	*tx_target = x1;
// 	*ty_target = y1;
// 	 }
// 	while (1) {
// 	if (TX_Y_out >= 0) {
// 			Move_Val_Right(50, 50);
// 			HAL_Delay(yanshi);
// 		} else {
// 			Move_Val_Left(50, 50);
// 			HAL_Delay(yanshi);
// 		}
// 		if (TX_X_out >= 0) {
// 			Move_Val_Back(50, 50);
// 			HAL_Delay(yanshi);
// 		} else {
// 			Move_Val_Line(50, 50);
// 			HAL_Delay(yanshi);
// 		}
// 		if (ABS(Tx_NOW - x1) < 5 && ABS(Ty_NOW - y1) < 5) {
// 			Move_Stop();
// 			break;
// 		}
// 	}
// }
// void Move_Tx_Ctrl(void){
// 	if (TX_Y_out >= 0) {
// 				Move_Right(80, 80, TX_Y_out * 2);
// 				HAL_Delay(yanshi);
// 			} else {
// 				Move_Left(80, 80, -TX_Y_out * 2);
// 				HAL_Delay(yanshi);
// 			}
// 			if (TX_X_out >= 0) {
// 				Move_Back(80, 80, TX_X_out * 2);
// 				HAL_Delay(yanshi);
// 			} else {
// 				Move_Line(80, 80, -TX_X_out * 2);
// 				HAL_Delay(yanshi);
// 			}
// }
