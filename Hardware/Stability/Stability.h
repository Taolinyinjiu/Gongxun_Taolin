/*
 * Stability.h
 *
 *  Created on: Dec 9, 2024
 *      Author: Taolin
 */

#ifndef STABILITY_STABILITY_H_
#define STABILITY_STABILITY_H_

#include "stm32h7xx_hal.h"
#include "Arm/Arm.h"
#include "Run/Run.h"
#include "SCSLib/SCServo.h"

extern uint8_t Zero_data;
extern uint8_t RxState;
extern char tx2_empty_recv_cnt;						//引入外部文件的变量                    12/11 ttxQWQ534
/*
HACK: 
使用方差判断稳定性,引入tx2_empty_recv_cnt用于判断tx2是否没有检测到目标，如果是则不用抓取，但是实际来说检测不到目标的时间很短，在没有目标时坐标值被保留
没有新的坐标更新，就可以不用抓取，可以用定时器试一试，如果用tx2发过来的标志的话，其实5帧左右就可以判断为没有目标
*/
//uint8_t Check_Stability(float check_x,float check_y,uint8_t Check_flag, uint8_t tx2_empty_recv_cnt_local);
 uint8_t Check_Stability(float check_x,float check_y,uint8_t Check_flag);
// 飞特舵机回零
uint8_t FT_Servo_Zero(uint8_t Number);
// 飞特舵机调整为向外
uint8_t FT_Servo_Orth(void);
// 发车前检查状态
uint8_t Check_Status(void);
// 物料抓完，回滚状态
uint8_t Roll_Status(void);
//准备抓取物料，爪子张开位于最低位置
uint8_t put_Status(void);
// 飞特舵机等待回零到位
uint8_t FT_Servo_Wait_Zero(uint8_t ID);

// 菲特舵机转动，从抓取物料到车上
uint8_t FT_Servo_Put(uint8_t Servo_ID, uint8_t Pub_Number);


#endif /* STABILITY_STABILITY_H_ */
