/*
 * Run.h
 *
 *  Created on: Nov 1, 2024
 *      Author: Taolin
 */

#ifndef RUN_RUN_H_
#define RUN_RUN_H_

#include "main.h"
#include "Emm_V5/Emm_V5.h"
#include "fdcan.h"
#include "Location/Location.h"
//#include "Arm/Arm.h"


#define yanshi 50

extern int time5_jiancha;


#define RunSpeed 100
#define RunAcc 120


//位置模式
bool Move_Line(u16 speed, u8 acc, u32 length);
bool Move_Left(u16 speed, u8 acc, u32 length);
//void Move_Left(u16 speed, u8 acc, u32 length);
bool Move_Back(u16 speed, u8 acc, u32 length);
bool Move_Right(u16 speed, u8 acc, u32 length);
bool Move_zhengxuzhuan(u16 speed, u8 acc, u32 length);
bool Move_fanxuzhuan(u16 speed, u8 acc, u32 length);
void Move_Xiajiang(u16 speed, u8 acc, u32 length);
void Drop_Location_jiang(u16 speed, u8 acc, u32 length);
void Drop_Location_Sheng(u16 speed, u8 acc, u32 length);
//速度模式
void Move_Val_Line(u16 speed, u8 acc);
void Move_Val_Back(u16 speed, u8 acc);
void Move_Val_Left(u16 speed, u8 acc);
void Move_Val_Right(u16 speed, u8 acc);
void Move_Stop(void);


void motor_clear_all(void);
void Move_GO(void);

void Move_TO_Saomaqu(float Leftclk, float Lineclk);
void Move_TO_yuanliaoqu(float Lineclk);
void Move_TO_jianzhi1(float Backclk, float fanzhuanclk);
void Move_TO_zancunqu(float Lineclk, float fanzhuanclk);
void Move_TO_jianzhi2(float Backclk, float Zhengzhuanclk);
void Move_TO_cujiagongqu(float Backclk);
void Move_TO_jianzhi3(float Backclk, float Zhengzhuanclk);
void Move_TO_fanyuanliaoqu(float Lineclk);



void Move_Action_Left_Ctrl(void);
void Move_Action_Forward_Ctrl(void);
void Move_Action_Right_Ctrl(void);
void put_location(float x_goal,float y_goal);



//void Move_Action_Nopid_Left_Ctrl(float x_goal,float y_goal);
bool Move_Action_Nopid_Left_Ctrl(float x_goal, float y_goal);
//void Move_Action_Nopid_Forward_Ctrl(float x_goal, float y_goal);
bool Move_Action_Nopid_Forward_Ctrl(float x_goal, float y_goal);
void Move_Action_Nopid_Right_Ctrl(float x_goal, float y_goal);

//TX调整
//void Move_Tx_Pid_Ctrl(float TX_X_Goal,float TX_Y_Goal);
bool Move_Tx_Pid_Ctrl(float TX_X_Goal, float TX_Y_Goal);
void Move_Tx_NO_Pid_Ctrl(float x2,float y2);
void Move_Tx_Val_Ctrl(float* tx_target,float* ty_target,float x1,float y1);

void Move_Tx_Ctrl(void);
#endif /* RUN_RUN_H_ */
