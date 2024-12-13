/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    fdcan.h
 * @brief   This file provides code for the configuration
 *          of the FDCAN instances.
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
#include "fdcan.h"

/* USER CODE BEGIN 0 */

FDCAN_FilterTypeDef hfdcan1_RX_Filter; /* FDCAN1滤波�?????????0对象句柄 */
FDCAN_TxHeaderTypeDef fdcan_TxHeader;
FDCAN_RxHeaderTypeDef fdcan_RxHeader;
uint8_t Emm_mode;
uint8_t RxFlag;
/* USER CODE END 0 */

FDCAN_HandleTypeDef hfdcan1;

/* FDCAN1 init function */
void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 10;
  hfdcan1.Init.NominalSyncJumpWidth = 8;
  hfdcan1.Init.NominalTimeSeg1 = 2;
  hfdcan1.Init.NominalTimeSeg2 = 2;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 10;
  hfdcan1.Init.DataTimeSeg1 = 2;
  hfdcan1.Init.DataTimeSeg2 = 2;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 1;
  hfdcan1.Init.RxFifo0ElmtsNbr = 4;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 0;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 0;
  hfdcan1.Init.TxBuffersNbr = 0;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 1;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */
//配置RX滤波
	hfdcan1_RX_Filter.IdType = FDCAN_EXTENDED_ID;       //标准 IDFDCAN_STANDARD_ID
	hfdcan1_RX_Filter.FilterIndex = 0;                           //滤波器索�?????????
	hfdcan1_RX_Filter.FilterType = FDCAN_FILTER_MASK;            //滤波器类�?????????
	hfdcan1_RX_Filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //过滤�?????????0关联到FIFO0
	hfdcan1_RX_Filter.FilterID1 = 0x000;                               //32位ID
	hfdcan1_RX_Filter.FilterID2 = 0x000; //如果FDCAN配置为传统模式的话，这里�?????????32位掩�?????????
	HAL_FDCAN_ConfigFilter(&hfdcan1, &hfdcan1_RX_Filter);               //滤波器初始化
	HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, ENABLE,
			ENABLE);
	HAL_FDCAN_Start(&hfdcan1);                               //�????启FDCAN
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
  /* USER CODE END FDCAN1_Init 2 */

}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspInit 0 */

  /* USER CODE END FDCAN1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_HSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN1 clock enable */
    __HAL_RCC_FDCAN_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PD0     ------> FDCAN1_RX
    PD1     ------> FDCAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspInit 1 */

  /* USER CODE END FDCAN1_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspDeInit 0 */

  /* USER CODE END FDCAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_FDCAN_CLK_DISABLE();

    /**FDCAN1 GPIO Configuration
    PD0     ------> FDCAN1_RX
    PD1     ------> FDCAN1_TX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

    /* FDCAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

  /* USER CODE END FDCAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void FDCAN_Send_Msg(uint8_t *msg, uint32_t len)
{
	uint8_t i = 0, j = 0, k = 0, l = 0, packNum = 0;
	uint8_t TxData[len];
	j = len - 2;
	// 除去ID地址和功能码后的数据长度
	TxData[0] = msg[1];
	// 发�?�数�?????????
	while (i < j)
	{
		// 数据个数
		k = j - i;
		fdcan_TxHeader.Identifier = ((uint32_t) msg[0] << 8)
				| (uint32_t) packNum;  //32位ID   0x00
		fdcan_TxHeader.IdType = FDCAN_EXTENDED_ID;      //ID   FDCAN_STANDARD_ID
		fdcan_TxHeader.TxFrameType = FDCAN_DATA_FRAME;             //数据�?????????
		//    fdcan_TxHeader.DataLength=len;              //数据长度
		fdcan_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		fdcan_TxHeader.BitRateSwitch = FDCAN_BRS_OFF;               //关闭速率切换
		fdcan_TxHeader.FDFormat = FDCAN_CLASSIC_CAN;                //传统的CAN模式
		fdcan_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;  //无发送事�?????????
		fdcan_TxHeader.MessageMarker = 0;
		//小于8字节命令
		if (k < 8)
		{
			for (l = 0; l < k; l++, i++)
			{
				TxData[l + 1] = msg[i + 2];
			}
			fdcan_TxHeader.DataLength = k + 1;
		}
		// 大于8字节命令，分包发送，每包数据�?????????多发�?????????8个字�?????????
		else
		{
			for (l = 0; l < 7; l++, i++)
			{
				TxData[l + 1] = msg[i + 2];
			}
			fdcan_TxHeader.DataLength = 8;
		}

		// 发�?�数�???????
		HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &fdcan_TxHeader, TxData);
		HAL_Delay(5);  //发第二帧数据的关键，若无延时，不发第二帧数据
		// 记录发�?�的第几包的数据
		++packNum;
	}
}

//can口接收数据查�?????????
//buf:数据缓存�?????????;
//返回�?????????:0,无数据被收到;
//		 其他,接收的数据长�?????????;
//u8 FDCAN1_Receive_Msg(u8 *buf) {
//	if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &fdcan_RxHeader, buf)
//			!= HAL_OK)
//		return 0;		//接收数据
//	return fdcan_RxHeader.DataLength >> 16;
//}

// ---- FIFO接收中断回调函数：用于到位回读，判断步进电机是否到位----
// FIXME:该判断缺少对步进电机ID帧的判断,但rxdata数组中似乎并没有储存地址�??
// 解决进度: 在一帧数据中，数据的发�?�方使用fdcan_RxHeader.Identifier进行存储�??
//         其�?�应当除�??256以获得十进制下的步进电机序号�??
// 定义uint8_t类型的数组变量rxdata用于存储接收到的数据
uint8_t rxdata[8];
// 定义静�?�变量RxState，用于记录接收状�?
uint8_t RxState = 0;

//	TODO:两个变量分别记录底盘步进电机和升降步进电机的到位状�??
bool Base_Data = false;
bool Top_Data = false;
// 允许查询状�?�标志位
extern bool Call_Flag;

//	多机同步运动接收标志位，默认是0
uint8_t broadcast_flag = 0;
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
//	提取FIFO中的数据,
	HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &fdcan_RxHeader, rxdata);
	static uint8_t broadcast_status = 0;
	if(fdcan_RxHeader.Identifier / 256 == 1)
	{
		if(broadcast_status == 0)
		{
			if(rxdata[0] == 0xFF)
				broadcast_status = 1;
			else
				broadcast_status = 0;
		}
		if (broadcast_status == 1)
		{
			if(rxdata[1] == 0x02)
				broadcast_status = 2;
			else
				broadcast_status = 0;
		}
		if(broadcast_status == 2)
		{
			if(rxdata[2] == 0x6B){
				broadcast_status = 0;
				broadcast_flag = 1;
			}
			else
				broadcast_status = 0;
		}
	}
	if (Call_Flag == true)
	{
//	底盘步进电机帧头
		if (fdcan_RxHeader.Identifier / 256 == 3)
		{
			if (RxState == 0)
			{
				if (rxdata[0] == 0xFD)
				{
					RxState = 1;
				}
				else if(rxdata[0] == 0x37)
				{
					RxState = 3;
				}
				else
					RxState = 0;
			}
			if (RxState == 1)
			{
				if (rxdata[1] == 0x9F)
				{
					RxState = 2;
				}
			}
			if (RxState == 2)
			{
				if (rxdata[2] == 0x6B)
				{
//				到位数据接收完毕，底盘步进确认到�?
					Base_Data = true;
					RxState = 0;
					memset(rxdata, 0, sizeof(rxdata));
				}
				else
				{
					;
				}
			}
		}
	}
	HAL_FDCAN_ActivateNotification(hfdcan,
	FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}
////			位置误差读取
//			else if (RxState == 3)
//			{
////				符号�?
//				Err_Sybol = rxdata[1];
//				RxState == 4;
//			}
//			else if (RxState == 4)
//			{
////				符号�?
//				Err_Sybol = rxdata[1];
//				RxState == 5;
//			}
//		}
//	升降步进电机帧头
//		else if (fdcan_RxHeader.Identifier / 256 == 5)
//		{
//			if (RxState == 0)
//			{
//				if (rxdata[0] == 0xFD)
//				{
//					RxState = 1;
//				}
//			}
//			if (RxState == 1)
//			{
//				if (rxdata[1] == 0x9F)
//				{
//					RxState = 2;
//				}
//			}
//			if (RxState == 2)
//			{
//				if (rxdata[2] == 0x6B)
//				{
////				到位数据接收完毕，升降步进确认到�?
//					Top_Data = true;
//					RxState = 0;
//				}
//			}
//		}


/* USER CODE END 1 */
