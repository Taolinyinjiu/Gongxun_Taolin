/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fdcan.h"
#include "memorymap.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Arm/Arm.h"
#include "Run/Run.h"
#include "Emm_V5/Emm_V5.h"
#include "Action/Action.h"
#include "Screen/Screen.h"
#include "Joy_Interface/Joy_Interface.h"
#include "Location/Location.h"
#include "stdio.h"
#include "string.h"
#include "SCSLib/SCServo.h"
#include "SCSLib/SCS_Uart.h"
#include "HWT101CT/HWT101CT.h"
#include "Bool.h"
#include "Stability/Stability.h"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RunSpeed 100
#define RunAcc 120
#define yanshi 50

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

//	串口接受数据变量
uint8_t data;
uint8_t Screen_data;
uint8_t Joy_data = 0;
uint8_t HwtData;
// TX2 接收数据变量
uint8_t aRxBuffer = 0;
char RxBuffer[RXBUFFERSIZE] ={ 0 };
uint8_t Uart10_Rx_Cnt = 0;
float x = .0;
float y = .0;
char Point_Flag = 0;
int QR_data[6] = { 0, 0, 0, 0, 0, 0 };
char tx2_empty_recv_cnt = 0;  							//tx2未检测到目标物体时返回的标志的计数器，在接收到正常坐标时应该清零											ttxQWQ534
// PID调整目标值
float x_goal, y_goal, a_goal;
// TODO:根据调试过程确定X与Y轴的目标值
float tx_target = 339;
float ty_target = 227;
uint16_t flag = 0;
uint16_t TX_flag = 0;
int colour;
int wuliao_falg;

int QR_Flag = 0;

// 正式比赛前车身姿态调整标志位
extern uint8_t Ready_Flag;
// 抓取过程标志位
bool Match_Flag = false;
// 抓取过程稳定性标志位,false为有故障,true为可以抓取
bool Check_flag = false;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MPU Configuration--------------------------------------------------------*/
	MPU_Config();

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */
	// USART10 用于Action串口转发,暂时使用无线串口将数据转发到主机
	// TODO:根据通信质量的稳定性决定是否要更换为有线USB转TTL
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_FDCAN1_Init();
	MX_UART4_Init();
	MX_UART5_Init();
	MX_UART9_Init();
	MX_USART6_UART_Init();
	MX_USART10_UART_Init();
	MX_TIM2_Init();
	MX_USART3_UART_Init();
	MX_UART7_Init();
	MX_UART8_Init();
	MX_TIM3_Init();
	MX_TIM5_Init();
	MX_USART2_UART_Init();
	MX_TIM12_Init();
	/* USER CODE BEGIN 2 */

//	清除定时器初始化过程中的更新中断标志
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
//	使能定时器中断
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_Base_Start_IT(&htim3);
//	使能串口接收中断
	HAL_UART_Receive_IT(&huart4, &Screen_data, 1);
//	HWT101预留中断接口
	HAL_UART_Receive_IT(&huart2, &HwtData, 1);
//	TX2通信接收中断
	HAL_UART_Receive_IT(&huart10, (uint8_t*) &aRxBuffer, 1);
	HAL_UART_Receive_IT(&huart9, &data, 1);
//	HAL_UART_Receive_IT(&huart10, &Joy_data, 1);

	PID_Init();
//  延时2s用于等待步进电机上电,考虑到实际过程中步进电机先于32上电,32需要手动发车,因此不需要再延时等待
//	HAL_Delay(2000);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
// 		检测准备发车标志位,如果准备发车,则调用Check_Status()函数将车身调整为准备比赛姿态
		if (Ready_Flag == 1)
		{
			Ready_Flag = 0;
			Check_Status();
		}
// 		检测发车标志位
		if (System_Flag == 1)
		{
			System_Flag = 0;
//			向TX2发送字符串 "e1f"开始执行程序
			HAL_UART_Transmit(&huart10, (uint8_t*) "e1f", sizeof("elf") - 1,0x1000);
			/************************************************/
			while (1)
			{
				switch (flag)
				{
				case 0:  //发车, 先左移, 然后直行, 进入扫码区域
				// Move_TO_Saomaqu(2400, 8250);
				// BUG:遇到左移后,步进电机没有移动到目标点位,没有返回到位标志，但是卡住不动的情况, 记录在Bug.md中的 E项
					bool temp = Move_Left(RunSpeed, RunAcc, 2400);
					while (temp != true)
					{
						temp = Move_Left(RunSpeed, RunAcc, 2400);
					}
				// 左移完成后,向前移动,同时将机械臂调整为扫码姿态
					Start();
					flag = 1;
					break;
				case 1:  //离开扫码区,进入暂存区抓取物料
				// 向前移动,非阻塞
					temp = Move_Line(RunSpeed, RunAcc, 10500);
					while (temp != true)
					{
						temp = Move_Line(RunSpeed, RunAcc, 10500);
					}
				// Choke_Flag = true说明当前底盘步进电机被阻塞,
					while (Choke_Flag == true)
					{
						;
					}
				//	等待TX2返回物料坐标点信息
					while (Point_Flag != 1)
					{
						;  
					}
				// TODO:调试,根据TX2返回坐标点信息进行车身调整,待物料稳定后抓取物料
					Frist_Grab_Wuliao();
					flag = 2;
					break;

				case 2:  // 离开原料区,进入十字区
					Move_TO_jianzhi1(4500, 4335);
				// 车身状态回滚为爪子向内的状态
					Roll_Status();
					HAL_Delay(50);
				// 根据Action返回的坐标点进行校准
					Move_Action_Nopid_Forward_Ctrl(160, 1070);
					flag = 3;
					break;
				case 3:    // 离开十字区域,进入暂存区
					Move_TO_zancunqu(22000, 4335);
					while (1)
					{
						;
					}
					// HACK: 我认为这里的代码有需要求改的地方,但缺少更好的底层
					// 将物料从车上放到目标区域
					put_wuliao_to_circular_frist();
					// 将物料从目标区域抓取回车上
					put_wuliao_to_Car_frist();
					// TODO: 延时需要修改
					HAL_Delay(yanshi);
					flag = 4;
					break;
				case 4:
					Move_TO_jianzhi2(9000, 4335);     
					Move_Action_Nopid_Forward_Ctrl(1870, 1860);
					flag = 5;
					break;
				case 5:		//移动到粗加工区       
					Move_TO_cujiagongqu(10000);
					// 将物料放置到第一层
					put_wuliao_to_circular_second();
					flag = 6;
					break;
				case 6:
					Move_TO_jianzhi3(9000, 4335);
					Move_Action_Nopid_Left_Ctrl(170, 1860);
					Drop_Location_jiang(50, 50, 11000);
					flag = 7;
					break;
				case 7:      //ԭ��???
					Move_TO_fanyuanliaoqu(4000);
					Move_Action_Nopid_Left_Ctrl(160, 1400);      //ACTION����
					Second_Run_Frist_Grab_Wuliao();
					flag = 8;
					break;
				case 8:
					Move_TO_jianzhi1(4500, 4335);
					Drop_Location_jiang(50, 50, 11000);
					Move_Action_Nopid_Forward_Ctrl(160, 1070);
					flag = 9;
					break;
				case 9:    //�ݴ�???
					Move_TO_zancunqu(22000, 4335);
					Second_Run_put_wuliao_to_circular_frist();

					Second_Run_put_wuliao_to_Car_frist();
					HAL_Delay(yanshi);
					flag = 10;
					break;
				case 10:
					Move_TO_jianzhi2(9000, 4335);     //ACTION����
					Move_Action_Nopid_Forward_Ctrl(1870, 1860);
					flag = 11;
					break;
				case 11:       //�ּӹ���
					Move_TO_cujiagongqu(10000);

					flag = 12;
					break;
				case 12:
					Move_TO_jianzhi3(9000, 4335);
					Move_Action_Nopid_Left_Ctrl(170, 1860);
					flag = 13;
					break;
				case 13:      //ԭ��???
					Move_TO_fanyuanliaoqu(22000);
					flag = 14;
					break;
				default:
					Move_Stop();
					break;
				}
			}
			if (Ready_Flag == 1)
			{
				Ready_Flag = 0;
				//			��������
				Check_Status();
			}
			/*********************************����???*******************************************/
//
//			while (1) {
//
//				switch (flag) {
//				case 0:  //ɨ��???
//					Move_TO_Saomaqu(2100, 7500);
//					flag = 1;
//					break;
//				case 1:  //ԭ��???
//					Move_TO_yuanliaoqu(10500);
//					Move_Action_Nopid_Left_Ctrl(160, 1400);  //ACTION����
//					flag = 2;
//					break;
//
//				case 2:
//					Move_TO_jianzhi1(4500, 4335);
//					Move_Action_Nopid_Forward_Ctrl(160, 1070);
//					flag = 3;
//					break;
//				case 3:
//					Move_TO_zancunqu(22000, 4335);
//
//					Move_Action_Nopid_Right_Ctrl(1900, 1050);
//					HAL_Delay(2000);
//					Move_Action_Nopid_Right_Ctrl(1900, 885 );
//					HAL_Delay(2000);
//					Move_Action_Nopid_Right_Ctrl(1900, 1185);
//					HAL_Delay(2000);
//					flag = 4;
//					break;
//				case 4:
//					Move_TO_jianzhi2(9000, 4335);     //ACTION����
//					Move_Action_Nopid_Forward_Ctrl(1870, 1860);
//					flag = 5;
//					break;
//				case 5:
//					Move_TO_cujiagongqu(10000);
//
//					Move_Action_Nopid_Forward_Ctrl(1065, 1910);
//					HAL_Delay(2000);
//					Move_Action_Nopid_Forward_Ctrl(1185, 1910);
//					HAL_Delay(2000);
//					Move_Action_Nopid_Forward_Ctrl(910, 1910);
//					HAL_Delay(2000);
//					flag = 6;
//					break;
//				case 6:
//					Move_TO_jianzhi3(9000, 4335);
//					Move_Action_Nopid_Left_Ctrl(170, 1860);
//					flag = 7;
//					break;
//				case 7:
//					//ԭ��???
//					Move_TO_fanyuanliaoqu(5000);
//					Move_Action_Nopid_Left_Ctrl(160, 1400);      //ACTION����
//					flag = 8;
//					break;
//				default:
//					flag = 0;
//					break;
//				}
//			}
/////////////////////////////////////////////////////////////////////////////////
			/**************************ץȡ����*********************************************/
//					catch(860, 2375);
//					catch(1910, 2370);
//					catch(2960, 2370);
//					put(2150, 2680);
//					HAL_Delay(2000);
//					put(3205, 2680);
//					HAL_Delay(2000);
//					put(2960, 2370);
//					HAL_Delay(2000);
//  			    put(1910,2370);
//					HAL_Delay(2000);
//				    put(860,2370);
//					HAL_Delay(2000);
//					put(2960, 2370);
//					HAL_Delay(2000);
		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{ 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{ 0 };

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
	{
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 2;
	RCC_OscInitStruct.PLL.PLLN = 44;
	RCC_OscInitStruct.PLL.PLLP = 1;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1
			| RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart4, (uint8_t*) &ch, 1, 0xFFFF);
	return ch;
}

/*  ------------ 串口中断回调函数 -----------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// UART9 中断回调函数
	if (huart->Instance == UART9)                          
	{
		// 将缓冲区中的Action数据输入到Data_Analyse()函数进行处理
		Data_Analyse(data);                               
		// 更新UART9接收中断回调, 中断源UART9, 缓冲区data, 缓冲大小 1 
		HAL_UART_Receive_IT(&huart9, &data, 1);            

	}
	// UART4 中断回调函数
	else if (huart->Instance == UART4)                   
	{
		// 将缓冲区中的屏幕数据输入到Data_Analyse()函数进行处理
		Check_Flag(Screen_data);                       	   // ����������??
		HAL_UART_Receive_IT(&huart4, &Screen_data, 1);     // �����жϻص�
	}

	else if (huart->Instance == USART10)
	{
		//static uint8_t RxState = 0;

		RxBuffer[Uart10_Rx_Cnt++] = aRxBuffer;

		if (Uart10_Rx_Cnt >= sizeof(RxBuffer))
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			Uart10_Rx_Cnt = 0;
			return;
		}

		if (aRxBuffer == 'd')
		{ // �����β??'d'��˵�������Ϻ�Բ����λ��??
			char *start = strchr(RxBuffer, 'a');
			char *end = strchr(RxBuffer, 'd');

			if (start != NULL && end != NULL && end > start)
			{
				*end = '\0';
				if (sscanf(start + 1, "%f,%f,%d", &x, &y, &colour) == 3)
				{
					// ���ݽ����ɹ�
					Point_Flag = 1;
					tx2_empty_recv_cnt = 0;									//接收到正常坐标返回时，a将tx2_empty_recv_cnt清零								12/11 ttx
				}

				// ��ջ���??
				memset(RxBuffer, 0, sizeof(RxBuffer));
				Uart10_Rx_Cnt = 0;
			}
		}

		if (aRxBuffer == 'b')
		{ // �����β??'b'��˵���Ƕ�ά����??
			char *start = strchr(RxBuffer, 'c');
			char *end = strchr(RxBuffer, 'b');

			if (start != NULL && end != NULL && end > start)
			{
				*end = '\0';
//				for (int i = 0; i < 6; i++) {
//					if (sscanf(start +1+ i, "%d", &QR_data[i])) {
				if (sscanf(start + 1, "%d,%d,%d,%d,%d,%d", &QR_data[0],
						&QR_data[1], &QR_data[2], &QR_data[3], &QR_data[4],
						&QR_data[5]))
				{
					// ���ݽ����ɹ�
//						if(i==5) QR_Flag =  1;
					QR_Flag = 1;
				}
//				}
				// ��ջ���??
				memset(RxBuffer, 0, sizeof(RxBuffer));
				Uart10_Rx_Cnt = 0;
			}
		}
		
		if (aRxBuffer == 'h')				//接收到包尾是h，说明接收到了tx2发送的未检测到目标物体的标志，则此中断每进一次，tx2_empty_recv_cnt要自增         			12/11 ttxQWQ534
		{
			char *start = strchr(RxBuffer, 'g');
			char *end 	= strchr(RxBuffer, 'h');
			
			if (start!= NULL && end != NULL && end > start)
			{
				*end = '\0';
				if (sscanf(start + 1, "%c", tx2_empty_recv_cnt))
				{
					tx2_empty_recv_cnt++;
				}
			}
			
			memset(RxBuffer, 0, sizeof(RxBuffer));
			Uart
		}
	}
		//	更新串口接收中断,中断触发源 UART10,接收数组 aRxBuffer,缓存大小 1
		HAL_UART_Receive_IT(&huart10, (uint8_t*) &aRxBuffer, 1);
}
// HWT101预留数据处理代码片段
//	else if (huart->Instance == USART2)                            // �ж��ж�
//	{
//
//		static uint8_t rx_buffer[11];
//		static uint8_t rx_index = 0;
//		static uint8_t state = 0; // 0: Ѱ�Ұ�ͷ???1: ��������
//
//		if (state == 0) { // Ѱ�Ұ�ͷ
//
//			if (HwtData == 0x55) {
//				rx_buffer[0] = HwtData;
//				rx_index = 1;
//				state = 1;
//			}
//		} else if (state == 1) { // ��������
//			rx_buffer[rx_index++] = HwtData;
//			if (rx_index == 2 && rx_buffer[1] != 0x53 && rx_buffer[1] != 0x52) { // ����ڶ��ֽڲ���0x53???0x52�������Ѱ����һ???0x55
//				if (rx_buffer[1] == 0x55) {
//					rx_index = 1;
//				} else {
//					state = 0;
//					rx_index = 0;
//				}
//			} else if (rx_index == 11) { // ���յ������������ݰ�
//				ParseAndPrintData(rx_buffer, 11);
//				rx_index = 0;
//				state = 0; // ����Ѱ�Ұ�ͷ
//			}
//		}
//		HAL_UART_Receive_IT(&huart2, &HwtData, 1);
//
//	}
//
// 手柄接口预留数据处理代码片段
//	else if (huart->Instance == USART10)                            // �ж��ж�
//	{
//		Recive_Joydata(Joy_data);                             	// ����Joy_data����
//		HAL_UART_Receive_IT(&huart10, &Joy_data, 1);       // �����жϻص�
//	}

extern uint16_t time_tx;
/*  ------------ 定时器中断回调函数 -----------*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// 如果中断源为TIM2
	if (htim == &htim2)
	{

//		X_out = X_Ctrl(&XPID, x_goal);
//		Y_out = Y_Ctrl(&YPID, y_goal);
//		A_out = A_Ctrl(&APID, a_goal);
		// Action数据展示
		Action_Show();
		// 二维码数据展示
		QR_Show();
		// 色环,物料坐标数据展示
		Point_Show();

	}
	// 如果中断源为TIM3
	else if (htim == &htim3)
	{
		// 根据TX2回传坐标进行PID调节
		TX_X_out = Tx_X_Ctrl(&TXPID, tx_target);
		TX_Y_out = Tx_Y_Ctrl(&TYPID, ty_target);
		time_tx++;
	}
	// 如果中断源为TIM5
	else if (htim == &htim5)
	{
		// TODO: 需要注释理解
		if (time5_jiancha != 0)
			time5_jiancha--;
	}
//	中断源为TIM12
	else if (htim == &htim12)
	{
		// 如果允许阻塞查询标志位为true
		if (Apply_Chock == true)
		{
			// 如果底层步进电机到位返回标志位为true
			if (Base_Data == true)
			{
				// 取消阻塞状态,阻塞标志位记为false
				Choke_Flag = false;
				// 申请阻塞查询标志位记为false
				Apply_Chock = false;
				// 关闭定时器中断
				HAL_TIM_Base_Stop_IT(&htim12);
				// 重置底层步进电机到位返回标志位为false
				Base_Data = false;
			}
			else
			{
				return;
			}
		}
	}
}
/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct =
	{ 0 };

	/* Disables the MPU */
	HAL_MPU_Disable();

	/** Initializes and configures the Region and the memory to be protected
	 */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress = 0x0;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.SubRegionDisable = 0x87;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Enables the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
