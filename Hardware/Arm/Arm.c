#include "arm.h"
#include "Stability/Stability.h"
#include "Run/Run.h"
extern int frist_wuliao, second_wuliao, three_wuliao, wuliao_falg;
extern int colour;
int Grab_count;
#define time 1000

//爪子落地 步进最低，爪子 780
#define Arm_close 20
#define Arm_low 600
#define Arm_high 1000
#define Emm_Speed  50     //步进电机
#define Emm_Acc  50
#define Max_High 12000

// extern
extern float tx_target;
extern float ty_target;
extern bool TX2_ENABLE;
//控制单个舵机运动
//参数：sevoID:舵机ID，Position:转动角度（这里以270°为例）,Time:转动时间
void Move_Arm(uint8_t servoID, int16_t Position, uint16_t Time) {
	uint8_t TxBuf[11];
	if (servoID > 31 || !(Time > 0) || Position > 32767) {
		return;
	}
	TxBuf[0] = FRAME_HEADER;
	TxBuf[1] = FRAME_HEADER;
	TxBuf[2] = 8;
	TxBuf[3] = CMD_SERVO_MOVE;
	TxBuf[4] = 1;
	TxBuf[5] = (uint8_t) Time;
	TxBuf[6] = (uint8_t) (Time >> 8);
	TxBuf[7] = servoID;
	TxBuf[8] = (uint8_t) Position;
	TxBuf[9] = (uint8_t) (Position >> 8);

	HAL_UART_Transmit(&jixiebi_usart, TxBuf, 10, 0xffff);
	HAL_Delay(5);

}

void QR(void) {

}
//步进上升1800  下降1800
extern float x, y;
void catch_Frist(uint16_t Pos1, uint16_t Pos2) //5400
{
	WritePosEx(0, Pos1, 500, 50); //物料底盘移动

	Move_Arm(1, Arm_close, time); //爪子闭合
	HAL_Delay(time);
	Drop_Location_Sheng(Emm_Speed, Emm_Acc, 500);
//	HAL_Delay(3000);
	WritePosEx(1, Pos2, 500, 50); //爪子底盘向左
	HAL_Delay(3000);
//	if(x-317.5>=50&&y-230.5>=50)
//	{
//		WritePosEx(1, 4045, 500, 50); //爪子底盘向左
//		HAL_Delay(1500);
//		Grab_count--;
//	}
	Move_Arm(6, 550, time); //爪子下降
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 4500);
	Move_Arm(1, 190, time); //爪子张开
	HAL_Delay(time);
	Move_Arm(6, 1000, time); //爪子上升
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 500);

	WritePosEx(1, 4050, 500, 50); //爪子底盘向左
	HAL_Delay(1500);
	Move_Arm(1, 500, time); //爪子张开
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 8600);
}
//上升1800，下降1800
void catch_Second(uint16_t Pos1, uint16_t Pos2) //从地上抓取到车上
{

	WritePosEx(0, Pos1, 500, 50); //物料底盘移动
	Move_Arm(1, 500, time); //爪子张开
	HAL_Delay(time);
	Move_Arm(6, 360, time); //爪子下降
	HAL_Delay(time);

	Move_Arm(1, Arm_close, time); //爪子闭合
	HAL_Delay(time);
	Move_Arm(6, 1000, time); //爪子上升
	HAL_Delay(time);
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 1000);
	WritePosEx(1, Pos2, 500, 50); //爪子底盘向左
	HAL_Delay(5000);
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 5000);
	Move_Arm(6, 550, time); //爪子下降
	HAL_Delay(time);
	Move_Arm(1, 190, time); //爪子张开
	HAL_Delay(time);
	Move_Arm(6, 1000, time); //爪子上升
	HAL_Delay(time);
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 5000);
	WritePosEx(1, 4045, 500, 50); //爪子底盘向左
	HAL_Delay(1500);
	Move_Arm(1, 500, time); //爪子张开
	HAL_Delay(time);
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 11000);

}

extern int QR_Flag;
void Start(void) //下降14000
{
	HAL_UART_Transmit(&huart10, (uint8_t*) "我来扫码辣\n", sizeof("我来扫码辣\n") - 1,0xffff);
	bool temp = Move_Line(RunSpeed, RunAcc, 8250);
	HAL_Delay(50);
	while(temp != true)
	{
		temp = Move_Line(RunSpeed, RunAcc, 8250);
	}
	HAL_UART_Transmit(&huart10, (uint8_t*) "确认接收\n", sizeof("确认接收\n") - 1,0xffff);
	//	先将爪子升起来，避免打到TX2或者物料盘
//	Drop_Location_jiang(320, 160, 4000);
	//	将舵机向外转动
//	FT_Servo_Orth();
//	Move_Arm(1, 500, 300); //爪子张开
//	Move_Arm(6, 500, 300);
//	Drop_Location_jiang(320, 160, 11000);
	// 向前移动到扫码区域
//	while (QR_Flag == false)
//	{
//		;  //等待扫码完成
//	}
	// 扫码完成后将爪子提起来
//	Move_Arm(6, 900, 300);
//	Drop_Location_jiang(320, 160, 8600);
	
}
void put(uint16_t Pos1, uint16_t Pos2) {
	WritePosEx(0, Pos1, 1000, 100); //物料底盘移动
	Move_Arm(6, 1000, time); //爪子上升
	Move_Arm(1, 150, time); //爪子张开
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 3500);
	HAL_Delay(1000);
	WritePosEx(1, Pos2, 800, 50); //爪子底盘向左
	HAL_Delay(2000);                                       //延时是为了抓取底盘降下来，防止变转变降
	Move_Arm(6, 410, time); //爪子下降
	HAL_Delay(time);
	Move_Arm(1, Arm_close, time); //爪子闭合
	HAL_Delay(time);
	Move_Arm(6, 1000, time); //爪子上升
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 1000);
	WritePosEx(1, 4100, 800, 50); //抓取底盘向摆放位置
	HAL_Delay(2000);                                        //延时是为了让抓取底盘转过来
	Move_Arm(6, 360, time); //爪子下降
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 11000);
	Move_Arm(1, 500, time); //爪子张开

}

void put_second(uint16_t Pos1, uint16_t Pos2) {
	WritePosEx(0, Pos1, 1000, 100); //物料底盘移动
	Move_Arm(6, 1000, time); //爪子上升
	Move_Arm(1, 150, time); //爪子张开
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 3500);
	HAL_Delay(1000);
	WritePosEx(1, Pos2, 800, 50); //爪子底盘向左
	HAL_Delay(2000);                                       //延时是为了抓取底盘降下来，防止变转变降
	Move_Arm(6, 410, time); //爪子下降
	HAL_Delay(time);
	Move_Arm(1, Arm_close, time); //爪子闭合
	HAL_Delay(time);
	Move_Arm(6, 1000, time); //爪子上升
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 1000);
	WritePosEx(1, 4100, 800, 50); //抓取底盘向摆放位置
	HAL_Delay(2000);                                        //延时是为了让抓取底盘转过来
	Move_Arm(6, 1000, time); //爪子下降
	Drop_Location_jiang(Emm_Speed, Emm_Acc, 11000);
	Move_Arm(1, 500, time); //爪子张开
}
//爪子在高5000处，必须搭配一个下降

extern int QR_data[6];
void Realize_Stop() {
//	FIXME:有BUG
	float err_x, err_y;

	while (1) {
		err_x = x;
		err_y = y;
		if (x - err_x <= 5 && y - err_y <= 5) {
			break;
		}
	}
}
// 识别到的二维码顺序
extern int QR_data[6];
// 抓取过程标志位
extern char Match_Flag;
// 抓取计算稳定性标志位，0为识别有故障,1为正常计算
extern char Check_flag;
// 第一次抓取物料
void Frist_Grab_Wuliao(void) {
//	FIXME:在每一次抓取前都应该判断物料停止与基于TX2坐标闭环
// 		  注意,该代码块有漏洞,
// 		  1. 该函数并未实现任意二维码下的最优摆放顺序，
// 		  2. 判断暂时是依据二维码：123+123 进行的

	// 设置物料抓取次数变量Grab_count
	static uint8_t Grab_count = 0;
	while (Grab_count < 3) {
	//	抓取物料过程，置标志位,标志位为颜色
	Match_Flag = QR_data[Grab_count];
	//	if (QR_data[0] == colour && wuliao_falg == 0) {
	//	先判断物料是否停止
	if(TX2_ENABLE == true){
//	while(Check_Stability(x,y,Check_flag) == 2)
//	{
//		HAL_Delay(30);
//	}
	//	Realize_Stop();
	//	基于TX2坐标进行闭环
		bool TX_Flag = Move_Tx_Pid_Ctrl(tx_target, ty_target);
		HAL_Delay(50);
		while(TX_Flag == false)
		{
			HAL_Delay(50);
			TX_Flag = Move_Tx_Pid_Ctrl(tx_target, ty_target);

		}
	}
	else
		HAL_Delay(100);
	//	抓取第一个物料
	//	物料盘先转动到目标位置
	// 函数位置似乎可以放一下,这个是非阻塞的函数,或许可以放在某些阻塞函数的前面
	FT_Servo_Put(0,Grab_count + 1);

	Move_Arm(1, 50, 300);
	// 爪子上升
	Drop_Location_jiang(200, 120, 1000);

	// 转动1号舵机, 将物料放置到车上
	// 因为 Grab_count是从0开始的, 而FT_Servo_Put传入参数的范围是1-3,所以这里需要 +1
	FT_Servo_Put(1,Grab_count + 1);
	//TODO:调试，失能五号步进电机
	Drop_Location_jiang(200, 120, 6000);
//			Emm_V5_En_Control(5,false,false);
//			while(1)
//			{
//				;
//			}
	// 松开爪子, 放置物料
	Move_Arm(1,500, 300);
	// 上升
	Drop_Location_jiang(200, 120, 1000);
	// 向右转，与车身正交
	if(Grab_count +1 < 3)
	{
		FT_Servo_Orth();
		Drop_Location_jiang(200, 120, 8600);
	}
	else
		FT_Servo_Zero(1);
	Grab_count += 1;
	}
	Grab_count = 0;
}

// void put_wuliao_to_circular_frist(void) {

// 	for (int i = 0; i < 3; i++) {
// 		if (QR_data[i] + 3 == 4) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1200);

// 		}
// 		if (QR_data[i] + 3 == 5) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1050);
// 		}
// 		if (QR_data[i] + 3 == 6) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 885);
// 		}
// 		if (i == 0) {
// 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// 			put(2150, 2680);
// 		} else if (i == 1) {
// 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// 			put(3205, 2680);
// 		} else if (i == 2) {
// 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// 			put(2960, 2370);
// 		}

// 	}
// }
// void put_wuliao_to_circular_frist(void) {                  //重新编写了放到色环的动作，需要试试然后更改些许值       12/11 cha
// 	//TODO:放在圆环的动作编辑
// 	static uint8_t put_count = 1;
// 	//一共放三次，
// 	while (put_count < 4) {

// 		if (QR_data[put_count - 1] + 3 == 4) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1200);
// 		}
// 		if (QR_data[put_count - 1] + 3 == 5) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1050);
// 		}
// 		if (QR_data[put_count - 1] + 3 == 6) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 885);
// 		}

// 		if (QR_data[put_count - 1] == colour) {
// 			//基于TX2坐标进行闭环
// 			Move_Tx_Pid_Ctrl(tx_target, ty_target);
// 			//放第一个物料
// 			//物料盘先转动到目标位置
// 			FT_Servo_Put(0, put_count);
// 			//白色爪子确保在最高处，否则可能在抓取时被物料卡住导致不能下降
// 			Move_Arm(6, 900, 300);
// 			//上升到1000处，防止打到物料
// 			Drop_Location_jiang(200, 120, 1000);
// 			//爪子舵机转动到放置物料到车上的角度
// 			FT_Servo_Put(1, put_count);
// 			//先闭合部分爪子，防止下降途中打到物料
// 			Move_Arm(1, 170, 300);
// 			//FIXME：爪子下降，根据具体情况修改参数
// 			Drop_Location_jiang(200, 120, 6000);
// 			//闭合爪子 ，抓取物料
// 			Move_Arm(1, 50, 300);
// 			//上升到1000处
// 			Drop_Location_jiang(200, 120, 1000);
// 			//向右转，与车身正交
// 			FT_Servo_Orth();
// 			//FIXME：加了部分延时，防止爪子边转边降
// 			HAL_Delay(500);

// 			Drop_Location_jiang(200, 120, 11000);
// 			//爪子下降，放置物料
// 			Move_Arm(6, 360, 300);
// 			//爪子张开，放置物料
// 			Move_Arm(1, 300, 300);
// 			put_count += 1;
// 		}
// 	}
// 	put_count = 1;
// }


// // void put_wuliao_to_Car_frist(void) {
// // 	for (int i = 0; i < 3; i++) {
// // 		if (QR_data[i] == 1) {
// // 			Move_Action_Nopid_Right_Ctrl(1850, 1200);
// // 		}
// // 		if (QR_data[i] == 2) {
// // 			Move_Action_Nopid_Right_Ctrl(1850, 1050);
// // 		}
// // 		if (QR_data[i] == 3) {
// // 			Move_Action_Nopid_Right_Ctrl(1850, 885);
// // 		}
// // 		if (i == 0) {
// // 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX
// // 			catch_Second(860, 2375);

// // 		} else if (i == 1) {
// // 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// // 			catch_Second(1910, 2370);
// // 		} else if (i == 2) {
// // 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// // 			catch_Second(2960, 2370);
// // 		}

// // 	}
// // }
// void put_wuliao_to_Car_frist(void) {                               //重新编写了抓取到车的动作，需要试试然后更改些许值       12/11 cha
// //TODO:完成了第一次抓到车上的动作编辑
// 	static uint8_t Grab_count = 1;
// 	//一共放三次，
// 	while (Grab_count < 4) {

// 		if (QR_data[Grab_count - 1] == 4) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1200);
// 		}
// 		if (QR_data[Grab_count - 1] == 5) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1050);
// 		}
// 		if (QR_data[Grab_count - 1] == 6) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 885);
// 		}

// 		if (QR_data[Grab_count - 1] == colour) {
// 			//			基于TX2坐标进行闭环
// 			Move_Tx_Pid_Ctrl(tx_target, ty_target);

// 			//放第一个物料
// 			//物料盘先转动到目标位置
// 			FT_Servo_Put(0, Grab_count);
// 			//爪子下降，准备抓取物料
// 			Move_Arm(6, 360, 300);
// 			//爪子闭合，抓取物料
// 			Move_Arm(1, 50, 300);
// 			//爪子上升到1000处
// 			Drop_Location_jiang(200, 120, 1000);
// 			//爪子上升，防止物料打到物料或物料盘
// 			Move_Arm(6, 900, 300);
// 			//爪子舵机转动到放置物料到车上的角度
// 			FT_Servo_Put(1, Grab_count);
// 			//爪子下降
// 			Drop_Location_jiang(200, 120, 6000);
// 			//松开爪子，这里张开爪子给的小了一些，防止打到别的物料
// 			Move_Arm(1, 190, 300);
// 			//			上升
// 			Drop_Location_jiang(200, 120, 1000);
// 			//			向右转，与车身正交
// 			if (Grab_count < 3) {
// 			//松开爪子，爪子张大，防止影响识别
// 			Move_Arm(1, 300, 300);
// 				FT_Servo_Orth();
// 				Drop_Location_jiang(200, 120, 8600);
			
// 			} else
// 				FT_Servo_Zero(1);
// 			Grab_count += 1;
// 		}
// 	}
// 	Grab_count = 1;

// }

// void put_wuliao_to_circular_second(void) {        //重新编写了抓取到车的动作，需要试试然后更改些许值       12/11 cha
// 	//TODO:放在圆环的动作编辑
// 		static uint8_t put_count = 1;
// 		//一共放三次，
// 		while (put_count < 4) {

// 			if (QR_data[put_count - 1] + 3 == 4) {
// 				Move_Action_Nopid_Forward_Ctrl(954, 1850);
// 			}
// 			if (QR_data[put_count - 1] + 3 == 5) {
// 				Move_Action_Nopid_Forward_Ctrl(1050, 1850);
// 			}
// 			if (QR_data[put_count - 1] + 3 == 6) {
// 				Move_Action_Nopid_Forward_Ctrl(1208, 1850);
// 			}

// 			if (QR_data[put_count - 1] == colour) {
// 				//			基于TX2坐标进行闭环
// 				Move_Tx_Pid_Ctrl(tx_target, ty_target);
// 				//放第一个物料
// 				//物料盘先转动到目标位置
// 				FT_Servo_Put(0, put_count);
// 				//白色爪子确保在最高处，否则可能在抓取时被物料卡住导致不能下降
// 				Move_Arm(6, 900, 300);
// 				//上升到1000处，防止打到物料
// 				Drop_Location_jiang(200, 120, 1000);
// 				//爪子舵机转动到放置物料到车上的角度
// 				FT_Servo_Put(1, put_count);
// 				//先闭合部分爪子，防止下降途中打到物料
// 				Move_Arm(1, 170, 300);
// 				//FIXME：爪子下降，根据具体情况修改参数
// 				Drop_Location_jiang(200, 120, 6000);
// 				//闭合爪子 ，抓取物料
// 				Move_Arm(1, 50, 300);
// 				//上升到1000处
// 				Drop_Location_jiang(200, 120, 1000);
// 				//向右转，与车身正交
// 				FT_Servo_Orth();
// 				//FIXME：加了部分延时，防止爪子边转边降
// 				HAL_Delay(500);

// 				Drop_Location_jiang(200, 120, 11000);
// 				//爪子下降，放置物料
// 				Move_Arm(6, 360, 300);
// 				//爪子张开，放置物料
// 				Move_Arm(1, 300, 300);
// 				put_count += 1;
// 			}
// 		}
// 		put_count = 1;
// }
// // void put_wuliao_to_circular_second(void) {
// // 	for (int i = 0; i < 3; i++) {
// // 		if (QR_data[i] + 3 == 4) {
// // 			Move_Action_Nopid_Forward_Ctrl(954, 1850);
// // 		}
// // 		if (QR_data[i] + 3 == 5) {
// // 			Move_Action_Nopid_Forward_Ctrl(1050, 1850);
// // 		}
// // 		if (QR_data[i] + 3 == 6) {
// // 			Move_Action_Nopid_Forward_Ctrl(1208, 1850);
// // 		}
// // 		if (i == 0) {
// // 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX
// // 			put(2150, 2680);

// // 		} else if (i == 1) {

// // 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX
// // 			put(3205, 2680);
// // 		} else if (i == 2) {
// // 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX
// // 			put(2960, 2370);
// // 		}

// // 	}
// // 	Move_Action_Nopid_Forward_Ctrl(1000, 1850);
// // }

// /*************************第二次抓取***********************************************/

// void Second_Run_Frist_Grab_Wuliao(void) {
	
// }

// // void Second_Run_Frist_Grab_Wuliao(void) {
// // 	static uint8_t Grab_count = 0;
// // 	wuliao_falg = 0;
// // 	while (Grab_count < 3) {
// // 		if (QR_data[3] == colour && wuliao_falg == 0) {
// // 			catch_Frist(860, 2375);
// // 			wuliao_falg = 1;
// // 			Grab_count += 1;
// // 		}
// // 		if (QR_data[4] == colour && wuliao_falg == 1) {
// // 			catch_Frist(1910, 2370);
// // 			wuliao_falg = 2;
// // 			Grab_count += 1;
// // 		}
// // 		if (QR_data[5] == colour && wuliao_falg == 2) {
// // 			catch_Frist(2960, 2370);
// // 			wuliao_falg = 0;
// // 			Grab_count += 1;
// // 		}

// // 	}
// // }
// void Second_Run_put_wuliao_to_Car_frist(void) {
// // 	for (int i = 3; i < 6; i++) {
// // 		if (QR_data[i] == 1) {
// // 			Move_Action_Nopid_Right_Ctrl(1850, 1200);
// // 		}
// // 		if (QR_data[i] == 2) {
// // 			Move_Action_Nopid_Right_Ctrl(1850, 1050);
// // 		}
// // 		if (QR_data[i] == 3) {
// // 			Move_Action_Nopid_Right_Ctrl(1850, 885);
// // 		}
// // 		if (i == 3) {
// // 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX
// // 			catch_Second(860, 2375);

// // 		} else if (i == 4) {
// // 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// // 			catch_Second(1910, 2370);
// // 		} else if (i == 5) {
// // 			Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// // 			catch_Second(2960, 2370);
// // 		}

// // 	}
// // }
// //TODO:完成了第二次抓到车上的动作编辑
// 	static uint8_t Grab_count = 1;
// 	//一共放三次，
// 	while (Grab_count < 4) {

// 		if (QR_data[Grab_count + 2] == 4) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1200);
// 		}
// 		if (QR_data[Grab_count + 2] == 5) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1050);
// 		}
// 		if (QR_data[Grab_count + 2] == 6) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 885);
// 		}

// 		if (QR_data[Grab_count + 2] == colour) {
// 			//			基于TX2坐标进行闭环
// 			Move_Tx_Pid_Ctrl(tx_target, ty_target);

// 			//放第一个物料
// 			//物料盘先转动到目标位置
// 			FT_Servo_Put(0, Grab_count);
// 			//爪子下降，准备抓取物料
// 			Move_Arm(6, 360, 300);
// 			//爪子闭合，抓取物料
// 			Move_Arm(1, 50, 300);
// 			//爪子上升到1000处
// 			Drop_Location_jiang(200, 120, 1000);
// 			//爪子上升，防止物料打到物料或物料盘
// 			Move_Arm(6, 900, 300);
// 			//爪子舵机转动到放置物料到车上的角度
// 			FT_Servo_Put(1, Grab_count);
// 			//爪子下降
// 			Drop_Location_jiang(200, 120, 6000);
// 			//松开爪子，这里张开爪子给的小了一些，防止打到别的物料
// 			Move_Arm(1, 190, 300);
// 			//			上升
// 			Drop_Location_jiang(200, 120, 1000);
// 			//			向右转，与车身正交
// 			if (Grab_count < 3) {
// 			//松开爪子，爪子张大，防止影响识别
// 			Move_Arm(1, 300, 300);
// 				FT_Servo_Orth();
// 				Drop_Location_jiang(200, 120, 8600);
			
// 			} else
// 				FT_Servo_Zero(1);
// 			Grab_count += 1;
// 		}
// 	}
// 	Grab_count = 1;

// }

// void Second_Run_put_wuliao_to_circular_frist(void) {

// 	// for (int i = 3; i < 6; i++) {
// 	// 	if (QR_data[i] == 1) {
// 	// 		Move_Action_Nopid_Right_Ctrl(910, 1910);

// 	// 	}
// 	// 	if (QR_data[i] == 2) {
// 	// 		Move_Action_Nopid_Right_Ctrl(1060, 1910);
// 	// 	}
// 	// 	if (QR_data[i] == 3) {
// 	// 		Move_Action_Nopid_Right_Ctrl(1185, 1910);
// 	// 	}
// 	// 	if (i == 3) {
// 	// 		Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// 	// 		put(2150, 2680);
// 	// 	} else if (i == 4) {
// 	// 		Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// 	// 		put(3205, 2680);
// 	// 	} else if (i == 5) {
// 	// 		Move_Tx_Pid_Ctrl(tx_target, ty_target); //TX调整
// 	// 		put(2960, 2370);
// 	// 	}

// 	// }
// 	//TODO:放在圆环的动作编辑
// 	static uint8_t put_count = 1;
// 	//一共放三次，
// 	while (put_count < 4) {

// 		if (QR_data[put_count + 2] + 3 == 4) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1200);
// 		}
// 		if (QR_data[put_count + 2] + 3 == 5) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 1050);
// 		}
// 		if (QR_data[put_count + 2] + 3 == 6) {
// 			Move_Action_Nopid_Right_Ctrl(1850, 885);
// 		}

// 		if (QR_data[put_count + 2] == colour) {
// 			//基于TX2坐标进行闭环
// 			Move_Tx_Pid_Ctrl(tx_target, ty_target);
// 			//放第一个物料
// 			//物料盘先转动到目标位置
// 			FT_Servo_Put(0, put_count);
// 			//白色爪子确保在最高处，否则可能在抓取时被物料卡住导致不能下降
// 			Move_Arm(6, 900, 300);
// 			//上升到1000处，防止打到物料
// 			Drop_Location_jiang(200, 120, 1000);
// 			//爪子舵机转动到放置物料到车上的角度
// 			FT_Servo_Put(1, put_count);
// 			//先闭合部分爪子，防止下降途中打到物料
// 			Move_Arm(1, 170, 300);
// 			//FIXME：爪子下降，根据具体情况修改参数
// 			Drop_Location_jiang(200, 120, 6000);
// 			//闭合爪子 ，抓取物料
// 			Move_Arm(1, 50, 300);
// 			//上升到1000处
// 			Drop_Location_jiang(200, 120, 1000);
// 			//向右转，与车身正交
// 			FT_Servo_Orth();
// 			//FIXME：加了部分延时，防止爪子边转边降
// 			HAL_Delay(500);

// 			Drop_Location_jiang(200, 120, 11000);
// 			//爪子下降，放置物料
// 			Move_Arm(6, 360, 300);
// 			//爪子张开，放置物料
// 			Move_Arm(1, 300, 300);
// 			put_count += 1;
// 		}
// 	}
// 	put_count = 1;
// }




// void Second_Run_put_wuliao_to_circular_second(void) {         
// 	//TODO:放在圆环的动作编辑
// 		static uint8_t put_count = 1;
// 		//一共放三次，
// 		while (put_count < 4) {

// 			if (QR_data[put_count + 2 ] + 3 == 4) {
// 				Move_Action_Nopid_Forward_Ctrl(954, 1850);
// 			}
// 			if (QR_data[put_count + 2 ] + 3 == 5) {
// 				Move_Action_Nopid_Forward_Ctrl(1050, 1850);
// 			}
// 			if (QR_data[put_count + 2] + 3 == 6) {
// 				Move_Action_Nopid_Forward_Ctrl(1208, 1850);
// 			}
// 			if (QR_data[put_count + 2] == colour) {
// 				//			基于TX2坐标进行闭环
// 				Move_Tx_Pid_Ctrl(tx_target, ty_target);
// 				//放第一个物料
// 				//物料盘先转动到目标位置
// 				FT_Servo_Put(0, put_count);
// 				//白色爪子确保在最高处，否则可能在抓取时被物料卡住导致不能下降
// 				Move_Arm(6, 900, 300);
// 				//上升到1000处，防止打到物料
// 				Drop_Location_jiang(200, 120, 1000);
// 				//爪子舵机转动到放置物料到车上的角度
// 				FT_Servo_Put(1, put_count);
// 				//先闭合部分爪子，防止下降途中打到物料
// 				Move_Arm(1, 170, 300);
// 				//FIXME：爪子下降，根据具体情况修改参数
// 				Drop_Location_jiang(200, 120, 6000);
// 				//闭合爪子 ，抓取物料
// 				Move_Arm(1, 50, 300);
// 				//上升到1000处
// 				Drop_Location_jiang(200, 120, 1000);
// 				//向右转，与车身正交
// 				FT_Servo_Orth();
// 				//FIXME：加了部分延时，防止爪子边转边降
// 				HAL_Delay(500);
// 				Drop_Location_jiang(200, 120, 11000);
// 				//爪子下降，放置物料
// 				Move_Arm(6, 360, 300);
// 				//爪子张开，放置物料
// 				Move_Arm(1, 300, 300);
// 				put_count += 1;
// 			}
// 		}
// 		put_count = 1;
// }







//暂存区利用ACTION进行位置的改变，圆环图像反映为    红 1  ，绿  2  ，蓝 3                            //重新编写了抓取到车的动作       12/11 cha_a_chong 学姐太强啦
void Move_To_Position_Staging_Area(int position) {  
    //ACTION调整位置。此处不宜太前，会导致去往下一个位置时碰到刚放置的物料
	const int forwardPosition = 1850;

    switch (position) {
        case 1:
            Move_Action_Nopid_Right_Ctrl(forwardPosition, 1200);   
            break;
        case 2:
            Move_Action_Nopid_Right_Ctrl(forwardPosition, 1050);
            break;
        case 3:
           Move_Action_Nopid_Right_Ctrl(forwardPosition, 885);
            break;
        default:
            // TODO:处理无效位置或记录错误
            break;
    }
}

//粗加工区利用ACTION进行位置的改变，圆环图像反映为  红 1 ，绿  2  ，蓝 3
void Move_To_Position_Rough_Processing_Area(int position) {  
	//ACTION调整位置。此处不宜太前，会导致去往下一个位置时碰到刚放置的物料
	const int forwardPosition = 1850;
    switch (position) {
        case 1:
            Move_Action_Nopid_Forward_Ctrl(954, forwardPosition);
            break;
        case 2:
            Move_Action_Nopid_Forward_Ctrl(1050, forwardPosition);
            break;
        case 3:
            Move_Action_Nopid_Forward_Ctrl(1208, forwardPosition);
            break;
        default:
            // TODO:处理无效位置或记录错误
            break;
	}
}



//将物料从车上抓下放在圆环上,ismaterial 填0或者1 0代表放在圆环上，1代表放在物料上
void Place_Material_On_Circular(uint8_t putCount,int IsMaterial) {
   
    
	// 物料盘先转动到目标位置
    FT_Servo_Put(0, putCount);

	// 白色爪子确保在最高处，否则可能在抓取时被物料卡住导致不能下降
    Move_Arm(6, 900, 300);

    // FIXME：先闭合部分爪子，防止下降途中打到物料
    Move_Arm(1, 160, 300);

    // 上升到1000处，防止打到物料
    Drop_Location_jiang(200, 120, 1000);

    // 爪子舵机转动到放置物料到车上的角度
    FT_Servo_Put(1, putCount);

    // FIXME：爪子下降，根据具体情况修改参数
    Drop_Location_jiang(200, 120, 7000);

    // 闭合爪子，抓取物料
    Move_Arm(1, 50, 300);

    // 上升到1000处
    Drop_Location_jiang(200, 120, 1000);

    // 向右转，与车身正交
    FT_Servo_Orth();

    // FIXME：加了部分延时，防止爪子边转边降
    HAL_Delay(500);
	switch(IsMaterial)
	{
		case 0:
			//步进电机降到11000处
			Drop_Location_jiang(200, 120, 12000);

			// FIXME：爪子下降，放置物料
			Move_Arm(6, 390, 300);

			// 爪子张开，放置物料
			Move_Arm(1, 500, 300);
			break;
		case 1:
			//FIXME：步进电机降到8600处
			Drop_Location_jiang(200, 120, 8600);
			// 爪子张开，放置物料
			Move_Arm(1, 500, 300);
			break;
	}
   
}       //此刻爪子处于低位并张开，便于再次识别
        //TODO：注意，第一次识别也要记得将物料至于低位并张开
		//TODO：那个粗加工区玩之后要不要爪子回到车身里面


//将物料从圆环上抓起放到车上
void Place_Material_On_Car(uint8_t grabCount) {                              
	//物料盘先转动到目标位置
	FT_Servo_Put(0, grabCount);

	//爪子下降，准备抓取物料
	Move_Arm(6, 360, 300);

	//爪子闭合，抓取物料
	Move_Arm(1, 50, 300);

    //爪子上升到1000处
	Drop_Location_jiang(200, 120, 1000);

	//爪子上升，防止物料打到物料或物料盘
	Move_Arm(6, 900, 300);
			
	//爪子舵机转动到放置物料到车上的角度
	FT_Servo_Put(1, grabCount);

	//爪子下降
	Drop_Location_jiang(200, 120, 6000);

	//松开爪子，这里张开爪子给的小了一些，防止打到别的物料
	Move_Arm(1, 190, 300);

	//上升
	Drop_Location_jiang(200, 120, 1000);
			
	//向右转，与车身正交
	if (grabCount <= 3) {
	//松开爪子，爪子张大，防止影响识别
	Move_Arm(1, 500, 300);
	FT_Servo_Orth();
	Drop_Location_jiang(200, 120,12000);
			
	} else
	     // 车身状态回滚为爪子向内的状态
			Roll_Status();
			//若为第三次抓取后，则将爪子收进来
			//TODO:三次抓取此时爪子处于车身内，调用时看一下
}

// //堆码放置物料，将这个融合到放圆环代码，            用switch语句？  
// void Stacking_and_placing_materials(void)
// {
// 	// 物料盘先转动到目标位置
//     FT_Servo_Put(0, putCount);

// 	// 白色爪子确保在最高处，否则可能在抓取时被物料卡住导致不能下降
//     Move_Arm(6, 900, 300);

//     // FIXME：先闭合部分爪子，防止下降途中打到物料
//     Move_Arm(1, 170, 300);

//     // 上升到1000处，防止打到物料
//     Drop_Location_jiang(200, 120, 1000);

//     // 爪子舵机转动到放置物料到车上的角度
//     FT_Servo_Put(1, putCount);

//     // FIXME：爪子下降，根据具体情况修改参数
//     Drop_Location_jiang(200, 120, 6000);

//     // 闭合爪子，抓取物料
//     Move_Arm(1, 50, 300);

//     // 上升到1000处
//     Drop_Location_jiang(200, 120, 1000);

//     // 向右转，与车身正交
//     FT_Servo_Orth();

//     // FIXME：加了部分延时，防止爪子边转边降
//     HAL_Delay(500);
//     //FIXME：步进电机降到8600处
//     Drop_Location_jiang(200, 120, 8600);
//     // 爪子张开，放置物料
//     Move_Arm(1, 300, 300);
// }       //此刻爪子处于低位并张开，便于再次识别
//         //TODO：注意，第一次识别也要记得将物料至于低位并张开
// 		//TODO：那个粗加工区玩之后要不要爪子回到车身里面







//TO Circular
//第一次跑在暂存区将物料放在圆环上
//若要第二次跑，主要是要改变一下数组的次序
//TODO：在此函数执行前，必须确保爪子在底部并张开，进行物料的识别
//第一次跑QR_Add为0，第二次传形参QR_Add为3
void put_Material_to_circular_Staging_Area_frist(int QR_Add) {
    static uint8_t put_count = 1;
    const int maxPutCount = 3;    

    // 一共放三次
    while (put_count <= maxPutCount) {
        int adjustedQRData = QR_data[put_count - 1 + QR_Add] ;
        if (adjustedQRData >= 1 && adjustedQRData <= 3) {
            Move_To_Position_Staging_Area(adjustedQRData);
        }
		//进行颜色识别，然后抓取
        if (QR_data[put_count - 1+ QR_Add] +3 == colour) {
			 // 基于TX2坐标进行闭环，先把车体调整至最佳位置
            Move_Tx_Pid_Ctrl(tx_target, ty_target);
			//从车上放在色环上
            Place_Material_On_Circular(put_count,0);
            put_count++;
        }
		//若因为TX识别错误，则重新回到原位置
		else{
			Move_To_Position_Staging_Area(adjustedQRData);
		}

    }
    put_count = 1; // 重置计数器
}

//TO Car
//第一次跑在暂存区区将物料放在车上
//若要第二次跑，主要是要改变一下数组的次序
//TODO：在此函数执行前，必须确保爪子在底部并张开，进行物料的识别
void Grab_Material_to_Car_Staging_Area_frist(int QR_Add) {
    static uint8_t grab_count = 1;
    const int maxPutCount = 3;    
    // 一共抓三次

    while (grab_count <= maxPutCount) {
        int adjustedQRData = QR_data[grab_count - 1+ QR_Add];
		//在这里根据二维码进行判断然后抓取
        if (adjustedQRData >= 1 && adjustedQRData <= 3) {
            Move_To_Position_Staging_Area(adjustedQRData);
        }
		//进行颜色识别，然后抓取
        if (QR_data[grab_count - 1+ QR_Add] == colour) {

		 // 基于TX2坐标进行闭环，先把车体调整至最佳位置
        Move_Tx_Pid_Ctrl(tx_target, ty_target);
		//放到色环上
            Place_Material_On_Car(grab_count);
            grab_count++;
        }
		//若因为TX识别错误，则重新回到原位置
		else{
			Move_To_Position_Staging_Area(adjustedQRData);
		}

    }
    grab_count = 1; // 重置计数器
}


//TO Circular
//第一次跑在粗加工区区将物料放在圆环上
//若要第二次跑，主要是要改变一下数组的次序
//TODO：在此函数执行前，必须确保爪子在底部并张开，进行物料的识别
void put_Material_to_Circular_Rough_Processing_Area_frist(int QR_Add,int iswuliao) {
    static uint8_t put_count = 1;
    const int maxPutCount = 3;    

    // 一共放三次

    while (put_count <= maxPutCount) {
        int adjustedQRData = QR_data[put_count - 1+ QR_Add];
		//在这里根据二维码进行判断然后抓取
        if (adjustedQRData >= 1 && adjustedQRData <= 3) {
            Move_To_Position_Rough_Processing_Area(adjustedQRData);
        }
		//进行颜色识别，然后抓取
        if (QR_data[put_count - 1+ QR_Add]+3 == colour) {

		 // 基于TX2坐标进行闭环，先把车体调整至最佳位置
        Move_Tx_Pid_Ctrl(tx_target, ty_target);
		//放到色环上
        Place_Material_On_Circular(put_count,iswuliao);
        put_count++;
        }
		//若因为TX识别错误，则重新回到原位置
		else{
			Move_To_Position_Rough_Processing_Area(adjustedQRData);
		}

    }
    put_count = 1; // 重置计数器
}
