/*
 * Arm.h
 *
 *  Created on: Nov 1, 2024
 *      Author: Taolin
 */

#ifndef ARM_ARM_H_
#define ARM_ARM_H_

#define FRAME_HEADER            0x55
#define CMD_SERVO_MOVE          0x03

#include "main.h"
#include "usart.h"
#include "SCSLib/SCServo.h"
#include "SCSLib/SCS_Uart.h"
#include "Emm_V5/Emm_V5.h"
#include "Run/Run.h"
#define jixiebi_usart huart5





void Move_Arm(uint8_t servoID, int16_t Position, uint16_t Time);
void catch_Frist(uint16_t Pos1, uint16_t Pos2);
void catch_Second(uint16_t Pos1, uint16_t Pos2);
void Start(void);
void put(uint16_t Pos1, uint16_t Pos2);


/*************************第一次抓取***********************************************/
void Frist_Grab_Wuliao(void);
void put_wuliao_to_circular_frist(void);
void put_wuliao_to_Car_frist(void);
void put_wuliao_to_circular_second(void);
/*************************第二次抓取***********************************************/
void Second_Run_Frist_Grab_Wuliao(void);
void Second_Run_put_wuliao_to_Car_frist(void);
void Second_Run_put_wuliao_to_circular_frist(void);
void Second_Run_put_wuliao_to_circular_second(void);



//TO Circular
//第一次跑在暂存区将物料放在圆环上
//若要第二次跑，主要是要改变一下数组的次序
//TODO：在此函数执行前，必须确保爪子在底部并张开，进行物料的识别
//QR_Add   值为0 第一次抓取，值为3 跑第二次抓取
void put_Material_to_circular_Staging_Area_frist(int QR_Add) ;

//TO Car
//第一次跑在暂存区区将物料放在车上
//若要第二次跑，主要是要改变一下数组的次序
//TODO：在此函数执行前，必须确保爪子在底部并张开，进行物料的识别
//QR_Add   值为0 第一次抓取，值为3 跑第二次抓取
void Grab_Material_to_Car_Staging_Area_frist(int QR_Add);

//TO Circular
//第一次跑在粗加工区区将物料放在圆环上
//若要第二次跑，主要是要改变一下数组的次序
//TODO：在此函数执行前，必须确保爪子在底部并张开，进行物料的识别
//QR_Add   值为0 第一次抓取，值为3 跑第二次抓取，iswuliao 值为0 放在色环上 ，值为1 放在物料上
void put_Material_to_Circular_Rough_Processing_Area_frist(int QR_Add,int iswuliao);

#endif /* ARM_ARM_H_ */
