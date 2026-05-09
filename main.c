#include "stm32f10x.h"                  // Device header
#include "Delay.h"                  // Device header
#include "Key.h"                  // Device header
#include "LED.h"                  // Device header
#include "Uart1.h"                  // Device header
#include "Exti.h" 
#include "Timer.h"                  // Device header
#include "24l01.h"                  // Device header
#include "OLED.h"  
#include "string.h"  
int main(void)
{   
	
	
	u8 Nrf24L01_buf[32];
	u8 Nrf24L01_buf2[32];
	char cmd[6];
	char display_str[17]; // 用于在OLED上显示的字符串缓冲区
	//inital tx memory data//
	u8 i;
	Nrf24L01_buf[0]=31; //lenth of 
	
	for(i=1;i<32;i++)
	{
	  Nrf24L01_buf[i]=i+100;
	}
	
	Nrf24L01_buf[1]='g';
	Nrf24L01_buf[2]='d';
	Nrf24L01_buf[3]='m';
	Nrf24L01_buf[4]='u';
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Uart1_Init();
	LED_Init();
	Key_Init();
	Exti_Init();
	OLED_Init();
//	while(1);
//	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms 
//	// test tick interupt //
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
//	SysTick_Config(9000*10); // 500ms
	
	//******** test wireless mode ***************//

	NRF24L01_Init();  //initial NRF24L01 including SPI1
	
	if(NRF24L01_Check())
	{
		printf("\r\n Failed to find NRF24l01....\r\n");//插入换行
		OLED_ShowString(1,1,"Failed to find NRF24l01");
	 }
	else
	{
		printf("\r\n Find NRF24l01....\r\n");//插入换行
	    OLED_ShowString(1,1,"Find NRF24l01");
	}
	
    printf("\r\n read addr width= %d\r\n",NRF24L01_Read_Reg(SETUP_AW));//插入换行sta=;  //读取状态寄存器的值
//	NRF24L01_TX_Mode();
//	NRF24L01_TxPacket(Nrf24L01_buf);
	
	//**** test receive mode ********//
	
	NRF24L01_RX_Mode();
	while(1)
	{
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)!=0);// PA15 NRF24L01_IRQ
		if (NRF24L01_RxPacket(Nrf24L01_buf2)==0) //receive data package
		{
		  for(i=1;i<=Nrf24L01_buf2[0];i++)
			{
			   printf("%c",Nrf24L01_buf2[i]);//插入换行
			}
			printf("\r\n");


			// --- 新增：在OLED上显示收到的内容 ---
			// 清除第3行 (OLED通常每行16个字符)
			OLED_ShowString(3, 1, "                "); 
			
			// 提取接收到的字符串并补上结束符 '\0'
			u8 len = Nrf24L01_buf2[0];
			if(len > 16) len = 16; // 防止超出OLED一行显示范围
			for(i=0; i<len; i++)
			{
			    display_str[i] = Nrf24L01_buf2[i+1];
			}
			display_str[len] = '\0';
			
			// 在OLED第3行第1列显示
			OLED_ShowString(3, 1, display_str);
			// ----------------------------------


			for(i=1;i<=6;i++) //get command
			{
			   cmd[i-1]=Nrf24L01_buf2[i];
			}
		
			if (strcmp(cmd,"led_on")==0)
			{
			  LED1_ON();
			 }
		    else if(strcmp(cmd,"led_of")==0)
			{
			  LED1_OFF();
			}
		}
		
	 }
//	//**** test receive mode ********//

//	NRF24L01_TX_Mode();
	
	while(1)
	{
//		OLED_ShowNum(1,1,Get_Couter(),4);
//	    if(Key_GetNum()==1)
//		{
//		  LED1_Turn();
//		  
//		}
		
		
	
	NRF24L01_TxPacket(Nrf24L01_buf);
	Delay_s(5); 	

	}

}
