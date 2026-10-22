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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint8_t adrs[1] = {0x40};

#define rxsize  6
uint8_t rxdata[rxsize];

int flag_recieved = 0;
int start_timer_stc_buzzer = 0;
volatile int timer_stc_buzzer = 0;

int timer_select = 0;

volatile int start_timer1_100ms = 0;
volatile int timer1_100ms = 0;

int shooting = 0;
int shooting_timer = 0;
int shoot_duration = 0;

int ready_stop_one_time_check = 0; 
int ready_stop_one_time = 0;

int step = 0; // 0 : start / 1 : run

int reset_for_first_time = 1;

int connect_to_cartridge = 0;
int connect_to_cartridge_buzzer = 0;
int connect_to_cartridge_handpiece_change = 0;
int connect_to_cartridge_first_time = 1;

volatile int cartridge_0x44_kind_check = 77;
int cartridge_0x44_kind_check_check = 77;

volatile uint16_t cartridge_shot = 0;
uint16_t cartridge_shot_check = 0;

volatile uint16_t current_shot = 0;
uint16_t current_shot_check = 0;

volatile uint16_t per_patient = 0;
uint16_t per_patient_check = 0;

volatile int first_start = 0;
int first_start_check = 0;

volatile int ready = 0;
volatile int ready_check = 0;

volatile int energy_level = 0;
int energy_level_check = 0;

volatile int length_level = 0;
int length_level_check = 0;

volatile int distance_page = 0;
int distance_page_check = 0;

volatile int distance_level = 0;
int distance_level_check = 0;

int ready_led_start = 0;
volatile int timer_ready_led_start = 0;


int handpiece_select = 0; // 0:handpiece1 & 1:handpiece2
int handpiece_select_check = 0;


//*****************THE SIZE OF RECEIVING BUFFER MUST BE ATLEAST 2 TIMES MORE THAN THE BIGGEST DATA SIZE WHICH IS TRANSFERED **************************
uint8_t RxBuf[20];
uint8_t MainBuf[30];
uint8_t temp[2];
int indx = 0;

//uint8_t RxBuf2[2000]; // STC SHOOT CHECK

uint8_t start_treatment[20]    = {0xAA, 0x73, 0x03, 0x7B, 0x00, 0x89, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0x7B, 0x00, 0x89, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t ready_STC[20]					 = {0xAA, 0x73, 0x03, 0xA2, 0x02, 0x68, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0xA2, 0x02, 0x68, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t ready_canccel[20]			 = {0xAA, 0x73, 0x03, 0xAC, 0x02, 0xA9, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0xAC, 0x02, 0xA9, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t energy_up[20]					 = {0xAA, 0x73, 0x03, 0x6D, 0x01, 0x6B, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0x6D, 0x01, 0x6B, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t energy_down[20]				 = {0xAA, 0x73, 0x03, 0x79, 0x01, 0xAF, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0x79, 0x01, 0xAF, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t length_up[20]					 = {0xAA, 0x73, 0x03, 0xC7, 0x01, 0x60, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0xC7, 0x01, 0x60, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t length_down[20]				 = {0xAA, 0x73, 0x03, 0xC5, 0x01, 0xAB, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0xC5, 0x01, 0xAB, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t distance_p[20]			   = {0xAA, 0x73, 0x03, 0xCA, 0x00, 0xDB, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0xCA, 0x00, 0xDB, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t distance_p_cancel[20]	 = {0xAA, 0x73, 0x03, 0xC9, 0x00, 0xE0, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0xC9, 0x00, 0xE0, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t distance_up[20]				 = {0xAA, 0x73, 0x03, 0xCB, 0x01, 0x9E, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0xCB, 0x01, 0x9E, 0xCC, 0x33, 0xC3, 0x3C};
uint8_t distance_down[20]			 = {0xAA, 0x73, 0x03, 0x77, 0x01, 0xA2, 0xCC, 0x33, 0xC3, 0x3C, 0xAA, 0x72, 0x03, 0x77, 0x01, 0xA2, 0xCC, 0x33, 0xC3, 0x3C};
	
	
uint8_t let_start[10]							= {0x5A, 0xA5, 0x07, 0x82, 0x00, 0x84, 0x5A, 0x01, 0x00, 0x01}; // when micro is stablished starts the LCD procces from the page 01 - [9] : pade ID - [7] : change page
uint8_t var_write[8]  						= {0x5A, 0xA5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x00}; 	


///////////////////////////////////////////////////////     DWIN     //////////////////////////////////////////////////////////////////

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		if (huart->Instance == USART1) // interruput normal 1 byte mode
		{	
				memcpy(RxBuf+indx, temp, 1);
				if (++indx >= 9)
				{
						indx = 0;
						flag_recieved = 1;
				}
	
				HAL_UART_Receive_IT(&huart1, temp, 1);
				
  	}
		
}

///////////////////////////////////////////////////////     STC     //////////////////////////////////////////////////////////////////

//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//	if (huart->Instance == USART3) //NORMAL MODE DMA
//	{					
//			if ((RxBuf2[0] == 0xAA) && (RxBuf2[1] == 0x71) && (RxBuf2[2] == 0x09) && (RxBuf2[3] == 0x02) && (RxBuf2[4] == 0xE4))
//			{
//					//memset(RxBuf2, 0, 2000);
//					current_shot ++;
//					per_patient ++;
//					cartridge_shot --;
//				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);	
//			}
//					HAL_UARTEx_ReceiveToIdle_DMA(&huart3, RxBuf2, 2000);
//					__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
//		
// 	}
//	
//}

///////////////////////////////////////////////////////     Small Delay     //////////////////////////////////////////////////////////////////

void Delay_us(uint32_t us) 
{
    uint32_t count = us * 5;  // Approximation for 72 MHz clock, tweak if needed
    while(count--) {
        __asm("nop");  // No operation (1 cycle)
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////     RGB LED     //////////////////////////////////////////////////////////////////

// RED : PA11 (TIM1_CH4)// GREEN : PB1 (TIM3_CH4)// BLUE : PB9 (TIM4_CH4)
/*
   ERROR NO CARTRDIGE : RED FLASHING : CHANGING FREQEUNCY FROM 1KHZ TO 1 HZ, if we do this, MCU strangely deosn't act and change f, I was forced to
	 use delay(700) when changing the f back to 1KHZ to give time to mcu to change f, otherwise it will turn off RED pwm completely, also start stop 
   seems to be necessar, without start & stop again it deosn't work !!!!
*/

void RGB_LED(int cr_kind) 
{
    if (cr_kind == 0) // RED ERROR NO CARTRIDGE FLASHING
		{
				TIM1->PSC = 1125-1;
				TIM1->ARR  = 64000-1;
				TIM1->CCR4 = 32000;			
				
				TIM3->CCR4 = 0;
				
				TIM4->CCR4 = 0;
		}
		
		else
		{
				HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);					
				TIM1->PSC = 720-1;
				TIM1->ARR  = 100-1;
			
				switch (cr_kind)
				{
					case 1:	// ORANGE
									TIM1->CCR4 = 99;				
									HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);								
									HAL_Delay(700);
					
									TIM3->CCR4 = 8;
									TIM4->CCR4 = 0;
									break;
					case 2:	// GREEN
									TIM1->CCR4 = 0;				
									HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);								
									HAL_Delay(700);
					
									TIM3->CCR4 = 99;
									TIM4->CCR4 = 0;
									break;
									
					case 3: // BLUE
									TIM1->CCR4 = 0;				
									HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);								
									HAL_Delay(700);
					
									TIM3->CCR4 = 0;
									TIM4->CCR4 = 99;
									break;
					
					case 4: // PURPLE + PINK
									TIM1->CCR4 = 99;				
									HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);								
									HAL_Delay(700);
					
									TIM3->CCR4 = 0;
									TIM4->CCR4 = 99;
									break;
					
					case 5:	// RED + PINK
									TIM1->CCR4 = 99;				
									HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);								
									HAL_Delay(700);
					
									TIM3->CCR4 = 6;
									TIM4->CCR4 = 6;
									break;
					
				}
		}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
	MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
	
	HAL_Delay(1500);


  MX_USART1_UART_Init();
  MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */


	

	HAL_UART_Receive_IT(&huart1, temp, 1);	
	
	//HAL_Delay (1000);
	
	//HAL_UARTEx_ReceiveToIdle_DMA(&huart3, RxBuf2, 2000);
	//__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
	
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	
	HAL_TIM_Base_Start_IT(&htim2);
	
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	
	
	first_start_check = 0;	
	energy_level_check = energy_level;	
	length_level_check = length_level;	
	distance_page_check = distance_page;
	distance_level_check = distance_level;
	cartridge_shot_check = cartridge_shot;
	current_shot_check = 0;
	per_patient_check = per_patient;	
	cartridge_0x44_kind_check = cartridge_0x44_kind_check_check;
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

// ********************************************* Start Handpiece_selection **********************************************************************
		
		/*
				6 * MUX (3 * CD4052B + 3 * CD4053B) : 2*4052 for I2C & 1*4052 for LED_READY + LED_SHOT
																						: 3*4053 for others.
		  + 2 * Relay Two Contact (Motor 4 wire)
			
			SELECT PINS:
										4052(1)(STM32_I2C) 					 : B = GND / A = PA7  (PA7 Doesn't care which Handpiece is selected, just checks the Availability)
										4052(2)(STC_I2C)   					 : B = PA6 / A = PA7  (PA6 = 0 > CART1 OR PA6 = 1 > CART2 SO PA6 the ONE & ONLY Select pin !!!!!!)
		
										4052(3)(Handpiece's LEDs')   : B = GND / A = PA6	(Common X & Y = STC's pins + STM32's PB13 & PB12)(IN/OUT = CART1[X0,Y0] & CART2[X1,Y1]) 
*** There is No need for a Buffer/BJT to Activate the LED's because STC is Directly Driving the LED's, so the Current Required is not Higher than 10mA
		
										4053(1,2,3)(Others : Ready & Shot Switches + IR motion motor detector + ...)   : A = B = C = PA6
		
										Relay Two Contact : bobbin = PA6     (Normally Connect [NC] = CART1)  (Normally Open [NO] = CART2)
		*/
		
/*		
		if (handpiece_select_check != handpiece_select)
		{
				handpiece_select_check = handpiece_select;
				
				connect_to_cartridge_handpiece_change = 1;
			
				if (handpiece_select == 0)
				{
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);						
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
				}
				
				else
				{
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
				}
		}
		
*/
				
		
// ********************************************* End Handpiece_selection **********************************************************************


// ********************************************* Start Counting Shot by Handpiece's LED *********************************************************
		
		switch (cartridge_0x44_kind_check)
		{
			case 1:
				shoot_duration = (20) * ((length_level + 3) / 10.0);
				if (shoot_duration >= 20)
						shoot_duration = 20;
				if (shoot_duration <= 13)
						shoot_duration = 13;
				break;
			
			case 2:
				shoot_duration = (22 + (energy_level / 13.0 * 7)) * ((length_level + 3) / 21.0); // x.0 is Very Very important since it considers it as float.
				if (shoot_duration >= 29)
						shoot_duration = 29;
				if (shoot_duration <= 13)
						shoot_duration = 13;
				break;
				
			case 3:
				shoot_duration = (26 + (energy_level / 7.0 * 12)) * ((length_level + 3) / 21.0);
				if (shoot_duration >= 38)
						shoot_duration = 38;
				if (shoot_duration <= 13)
						shoot_duration = 13;
				break;
				
			case 4:
				shoot_duration = (24 + (energy_level / 9.0 * 10)) * ((length_level + 3) / 21.0);
				if (shoot_duration >= 38)
						shoot_duration = 38;
				if (shoot_duration <= 13)
						shoot_duration = 13;
				break;
				
			case 5:
				shoot_duration = (24 + (energy_level / 9.0 * 10)) * ((length_level + 3) / 21.0);
				if (shoot_duration >= 38)
						shoot_duration = 38;
				if (shoot_duration <= 13)
						shoot_duration = 13;
				break;
				
			default:
				break;
			
		}
		
//////////////////
		
		if (shooting == 0)
		{
				if (!(HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_12))) // Active LOW
				{
						shooting = 1;
				}
		}
		
		if ( (shooting == 1) && (shooting_timer >= shoot_duration) )
    {
					if (HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_12))
					{
							shooting = 0;
							shooting_timer = 0;
							current_shot ++;
							per_patient ++;
							cartridge_shot --;
					}
    }
		
		
		
// ********************************************* End Counting Shot by Handpiece's LED *********************************************************
		
		
// ********************************************* Start Ready & Stop by Handpiece's LED ********************************************************
		
		
			if (!(HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_13))) // Active LOW
				{
												
						ready_stop_one_time = 1;
				}
				
			else
				{											
						ready_stop_one_time = 0;
				}
				
				if (ready_stop_one_time_check != ready_stop_one_time)
				{
						
					ready_led_start = 1;
					
					if (shooting == 1)
						timer_select = shoot_duration;
					else
						timer_select = 3;
					
					if (timer_ready_led_start >= timer_select)
					{
						
						if(ready_stop_one_time == 1)
						{
							var_write[4] = 0x12;
							var_write[6] = 0x00;
							var_write[7] = 1;  //ready 
							HAL_Delay(50);
							HAL_UART_Transmit (&huart1, var_write, 8, 20);
						}
					
						if(ready_stop_one_time == 0)
						{
							var_write[4] = 0x12;
							var_write[6] = 0x00;
							var_write[7] = 0;  // stop
							HAL_Delay(50);
							HAL_UART_Transmit (&huart1, var_write, 8, 20);								
						}
					
					ready_stop_one_time_check = ready_stop_one_time;
					ready_led_start = 0;
					timer_ready_led_start = 0;
						
				}
			}
// ********************************************* END Ready & Stop by Handpiece's LED *********************************************************
		

// ********************************************* Start Dwin *************************************************************************************			
		
		
	if (flag_recieved == 1)					// 1) using proccess in while(1) instead of func 2) using HAL_UART_RxCpltCallback instead of HAL_UARTEx_RxEventCallback
	{														   	// 3) using simple interrupt 1 byte recieving method instead of DMA 4) in main() copy recieve_IT(RxBuf,1byte)
			memcpy (MainBuf, RxBuf, 9); // 5) defining Mainbuf because we want full 9 byte fixed to proccess 6) cancel 4F 4B response of DWIN in good
			flag_recieved = 0;				  // 7) when 9 bytes recieved flag is 1 and processing starts. 8) we know that DWIN only sends 9 bytes of data
																 // ****) but why ? because DWIN does NOT send these 9 bytes in 1 chunk or part. it sends these 8 byte into 2 parts
			if (MainBuf[3] == 0x83)		 //       or several chunk of bytes. - and with that method with DMA the first part or chunk missed because
			{													 // 			 DWIN inserted a big delay between these 2 chunk and DMA thought the new commands are arriving and puted away
																 //			 the first part. also we have indx that with each byte recieved inrcreamented and inserted to Rxbuf
				switch(MainBuf[4])		 	 // 			 VERY NICE. Thank God !
				{	

// 10 : first_start // 11 ready 1 ,0 // 12 standby wait ready icon 0,1,2 // 13 go to distance_page 1 back 0 // 14 disptance_page_icon 0,1,2=normal
// 15 : current_shot // 16 cartridge_shot // 17 total 9999 // 18 patient // 19 distance_level // 20 length // 21 energy
					
											case 0x10:
												first_start = 1;
												break;
											case 0x12:
												ready = MainBuf[8];
												ready_check = 1;
												break;
											case 0x21:
												energy_level = MainBuf[8];
												break;
											case 0x20:
												length_level = MainBuf[8];
												break;
											case 0x13:
												distance_page = MainBuf[8]; //1 goto 0 back
												break;
											case 0x19:
												distance_level = MainBuf[8];
												break;
											case 0x18:
												per_patient = MainBuf[8];
												break;										
											case 0x30:
												//handpiece_select = MainBuf[8];
												break;
										
											default:
												break;
											
											
						}
		
			}
		}
// ********************************************* End Dwin *************************************************************************************			

// ********************************************* Start Checking STC BUZZER ********************************************************************			
if (connect_to_cartridge == 0)	//connect_to_cartridge_buzzer == 0)
{
		if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_5))
    {
					start_timer_stc_buzzer = 1;
    }
    else
    {
					start_timer_stc_buzzer = 0;
					timer_stc_buzzer = 0;
    }
		
		if (timer_stc_buzzer >= 5)
		{
					connect_to_cartridge = 1; ///connect_to_cartridge = 1
					current_shot = 0;					
		}
}
		
// ********************************************* Start Cartridge_EEPROM_CHECKING **************************************************************				
// do not use 400khz speed. just use 100khz because 400khz is not working for every cartridges

/*	CD4052B MUX/DEMUX Analog Switch

		MUX1 : STM32 (using Ch0 > cart1 & Ch1 > cart2 & Ch2,3 = Empty)

													sel B	(GND)	|	sel A (PA7)
																			|
		CART1 > STM32										0	|	0			(00 = CH0 : CART1)
		if (CART1 check was ok):				0	| 1			(01 = CH1 : CART2)
		 CART1 > STC &	CART2 > STM32			|

		if Handpiece2 selected:(by default and at Start, Handpiece1 is selected)

													sel B	(GND)	|	sel A (PA7)
																			|
		CART2 > STM32										0	|	1		 (01 = CH1 : CART2)
		if (CART2 check was ok):				0	| 0		 (00 = CH0 : CART1)
		 CART2 > STC &	CART1 > STM32			|

		STM32 Doesn't Disconnect from Cartrdiges, it just Alternating between 2 cartridges, if One Checking was ok, then it will Check other one.
---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------
		MUX2 : STC (using Ch1 > cart1 & Ch2 > cart2 & Ch0,3 = Empty)

													sel B	(PA6)	|	sel A (PA7)
																			|
	NO CART (Empty ChanneL0) > STC	  0	|	0			(00 = CH0 : Empty)
	if (CART1 check by STM32 was ok): 	| 
	CART1 > STC &	CART2 > STM32				0	| 1			(01 = CH1 : CART1)

		if Handpiece2 selected:(by default and at Start, Handpiece1 is selected)

													sel B	(PA6)	|	sel A (PA7)
																			|
	NO CART (Empty ChanneL3) > STC		1	|	1			(11 = CH3 : Empty)
	if (CART2 check by STM32 was ok):		| 
	CART2 > STC &	CART1 > STM32				1	| 0			(10 = CH2 : CART2)
	
	STC Has 3 States/Phases : (1) No Connect -2 Channels- (2) Cart1 (3) Cart 2
	
*/				


/*
		if ( (connect_to_cartridge_buzzer == 1) || (connect_to_cartridge_first_time == 1) || (connect_to_cartridge_handpiece_change == 1))
		{
		
			for(int x = 0; x <= 1; x++)
			{
			HAL_StatusTypeDef result; //For checking HAL_OK i2c
			
      memset(rxdata, 0, rxsize);  // Clear the receiving buffer
			
			cartridge_0x44_kind_check = 0;

			if (handpiece_select == 0)
			{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			}
			else
			{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);					
			}

			Delay_us(50);  // Add a small delay before starting communication for MUX select pin to activate
    
      HAL_I2C_Init(&hi2c1); // this way that I Init i2c again is more reliable and i2c failure's rate is reduced	
       
      result = HAL_I2C_Master_Transmit(&hi2c1, 0xA0, adrs, 1, 1);   // Attempt to transmit to EEPROM
        
      if (result == HAL_OK) 
			{           
            result = HAL_I2C_Master_Receive(&hi2c1, 0xA0, rxdata, rxsize, 1); // Transmission was successful, attempt to receive data
            
            if (result == HAL_OK)
						{              
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // Reception was successful, process EEPROM data
												
													switch (rxdata[4])
													{
															case 0x02:
															{															
																cartridge_0x44_kind_check = 1;
																break;
															}
															
															case 0x03:
															{															
																cartridge_0x44_kind_check = 2;
																break;
															}
															
															case 0x06:
															{															
																cartridge_0x44_kind_check = 3;
																break;
															}
															
															case 0x07:
															{															
																cartridge_0x44_kind_check = 4;
																break;
															}
															
															case 0x08:
															{															
																cartridge_0x44_kind_check = 5;
																break;
															}
															
															default:
															{
																cartridge_0x44_kind_check = 0;
																break;
															}
															
													}
													
													if (cartridge_0x44_kind_check != 0)
													{
															cartridge_shot = (rxdata[2] << 8) | rxdata[3];
															// We Don't Want to check EEPROM anymore since we got our Desired DATA :
															connect_to_cartridge_buzzer = 0; 
															connect_to_cartridge_first_time = 0;
															connect_to_cartridge_handpiece_change = 0;
														
															HAL_I2C_DeInit(&hi2c1);
															Delay_us(50); 
															//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
															HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);  
															//break;
													}
																				
            }
						
// What to do When i2c Failed :		
						
						else
						{
                // Reception failed
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            }
						
        }
				
				else
				{
            // Transmission failed
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        }
																	
        HAL_I2C_DeInit(&hi2c1);

	}		
}
		
if ((connect_to_cartridge_buzzer == 0) && (connect_to_cartridge_first_time == 0) && (connect_to_cartridge_handpiece_change == 0))
	
		{
			
				//har yek sanie ya do sanie yekbar ba i2c faghat 2 bar check kon = 42 ro ba 3 ta read befrest, hich kar dige ham nakon
			
			
		}	

*/





if ( (connect_to_cartridge == 1) || (connect_to_cartridge_first_time == 1) )
		{
		
			for(int x = 0; x <= 1; x++)
			{
			HAL_StatusTypeDef result; //For checking HAL_OK i2c
			
      memset(rxdata, 0, rxsize);  // Clear the receiving buffer
			
			cartridge_0x44_kind_check = 0;

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);  // MUX select pin to Activate STM32 channel

			Delay_us(50);  // Add a small delay before starting communication for MUX select pin to activate
    
      HAL_I2C_Init(&hi2c1); // this way that I Init i2c again is more reliable and i2c failure's rate is reduced	
       
      result = HAL_I2C_Master_Transmit(&hi2c1, 0xA0, adrs, 1, 1);   // Attempt to transmit to EEPROM
        
      if (result == HAL_OK) 
			{           
            result = HAL_I2C_Master_Receive(&hi2c1, 0xA0, rxdata, rxsize, 1); // Transmission was successful, attempt to receive data
            
            if (result == HAL_OK)
						{              
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // Reception was successful, process EEPROM data
								HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
												
													switch (rxdata[4])
													{
															case 0x02:
															{															
																cartridge_0x44_kind_check = 1;
																break;
															}
															
															case 0x03:
															{															
																cartridge_0x44_kind_check = 2;
																break;
															}
															
															case 0x06:
															{															
																cartridge_0x44_kind_check = 3;
																break;
															}
															
															case 0x07:
															{															
																cartridge_0x44_kind_check = 4;
																break;
															}
															
															case 0x08:
															{															
																cartridge_0x44_kind_check = 5;
																break;
															}
															
															default:
															{
																cartridge_0x44_kind_check = 0;
																break;
															}
															
													}
													
													if (cartridge_0x44_kind_check != 0)
													{
															cartridge_shot = (rxdata[2] << 8) | rxdata[3];
															connect_to_cartridge = 0; // We Don't Want to check EEPROM anymore since we got our Desired DATA
															connect_to_cartridge_first_time = 0;
															HAL_I2C_DeInit(&hi2c1);
															Delay_us(50); 
															HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);	
															
															//break;
													}
																				
            }
						
// What to do When i2c Failed :		
						
						else
						{
                // Reception failed
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
            }
						
        }
				
				else
				{
            // Transmission failed
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
        }
																	
        HAL_I2C_DeInit(&hi2c1);

	}		
}





		
// ****************************************** End Cartridge_EEPROM_CHECKING ********************************************************************	
	
//***************************************** Start changing to related start_cartridge page *****************************************************

	if (step == 0)
	{
		if (cartridge_0x44_kind_check_check != cartridge_0x44_kind_check)
		{
			cartridge_0x44_kind_check_check = cartridge_0x44_kind_check;
						
		  switch (cartridge_0x44_kind_check)
		  {
				case 0:
					let_start[9] = 0x01;
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, let_start, 10, 20);
				
					break;
				
				case 1: 
					let_start[9] = 0x02;
					HAL_Delay(50);					// the lowest possible Delay to send commands is 30 ms. not 20 !
					HAL_UART_Transmit (&huart1, let_start, 10, 20);
				
					length_level = 9;
					length_level_check = 0;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 9; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
				
					break;
					
				case 2:
					let_start[9] = 0x03;
					HAL_Delay(50);
					HAL_UART_Transmit_DMA (&huart1, let_start, 10);
				
					length_level = 20;
					length_level_check = 0;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 20; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);

					break;
					
				case 3:
					let_start[9] = 0x04; 
					HAL_Delay(50);
					HAL_UART_Transmit_DMA (&huart1, let_start, 10);	

					length_level = 20;
					length_level_check = 0;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 20; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);

					break;
					
				case 4:
					let_start[9] = 0x05;
					HAL_Delay(50);
					HAL_UART_Transmit_DMA (&huart1, let_start, 10);	

					length_level = 20;
					length_level_check = 0;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 20; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);

					break;
					
				case 5:
					let_start[9] = 0x06;
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, let_start, 10, 20);	
					
					length_level = 20;
					length_level_check = 0;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 20; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);

					break;
					
					default:
					break;
					
		}
			
		RGB_LED(cartridge_0x44_kind_check);
			
	}
	
}
	

//************************************** END changing to related start_cartridge page ***********************************************************

//************************************** Start send start_treatment click to STC ***************************************************************

		if (first_start_check != first_start)
		{
				first_start_check = first_start;
				step = 1;
				HAL_Delay(50);
				HAL_UART_Transmit_DMA(&huart3, start_treatment, 20);
				HAL_Delay(50);
				HAL_UART_Transmit_DMA(&huart3, start_treatment, 20);
				 
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

		}

//************************************** END send start_treatment click to STC ******************************************************************

//************************************* Start changing to related runnning_cartridge page *******************************************************

	if (step == 1)
	{
		
		if (cartridge_0x44_kind_check_check != cartridge_0x44_kind_check)
		{
			
			cartridge_0x44_kind_check_check = cartridge_0x44_kind_check;
			
		  switch (cartridge_0x44_kind_check)
		  {
				
				case 0:
					let_start[9] = 0x01;
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, let_start, 10, 20);
				
					break;
				
				case 1: 
					let_start[9] = 0x07;
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, let_start, 10, 20);		
				
					ready = 0;

					var_write[4] = 0x12;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // icon_ready
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
				
					distance_page = 2;  // wow. thankGod

				
					current_shot = 0;
					current_shot_check = 0;
					var_write[4] = 0x15;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // current_shot
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);		
					
//					distance_level = 0;
//					distance_level_check = 0;
//					var_write[4] = 0x19;
//					var_write[6] = 0x00;
//					var_write[7] = 0x00; // distance_level
//					HAL_Delay(50);
//					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					length_level = 9;
					length_level_check = 9;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 9; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					energy_level = 0;
					energy_level_check = 0;
					var_write[4] = 0x21;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // energy_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
										
					cartridge_shot_check = 0;
				
					break;
					
				case 2:

					let_start[9] = 0x08;
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, let_start, 10, 20);		
				
					ready = 0;

					var_write[4] = 0x12;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // icon_ready
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
				
					distance_page = 2;  // wow. thankGod

				
					current_shot = 0;
					current_shot_check = 0;
					var_write[4] = 0x15;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // current_shot
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);		
					
//					distance_level = 0;
//					distance_level_check = 0;
//					var_write[4] = 0x19;
//					var_write[6] = 0x00;
//					var_write[7] = 0x00; // distance_level
//					HAL_Delay(50);
//					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					length_level = 20;
					length_level_check = 20;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 20; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					energy_level = 0;
					energy_level_check = 0;
					var_write[4] = 0x21;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // energy_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
										
					cartridge_shot_check = 0;
					
					break;
				
				case 3:
					
					let_start[9] = 0x09;
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, let_start, 10, 20);		
				
					ready = 0;

					var_write[4] = 0x12;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // icon_ready
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
				
					distance_page = 2;  // wow. thankGod

				
					current_shot = 0;
					current_shot_check = 0;
					var_write[4] = 0x15;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // current_shot
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);		
					
//					distance_level = 0;
//					distance_level_check = 0;
//					var_write[4] = 0x19;
//					var_write[6] = 0x00;
//					var_write[7] = 0x00; // distance_level
//					HAL_Delay(50);
//					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					length_level = 20;
					length_level_check = 20;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 20; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					energy_level = 0;
					energy_level_check = 0;
					var_write[4] = 0x21;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // energy_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
										
					cartridge_shot_check = 0;
								
					break;
				
				case 4:
					
					let_start[9] = 0x0A;
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, let_start, 10, 20);		
				
					ready = 0;

					var_write[4] = 0x12;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // icon_ready
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
				
					distance_page = 2;  // wow. thankGod

				
					current_shot = 0;
					current_shot_check = 0;
					var_write[4] = 0x15;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // current_shot
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);		
					
//					distance_level = 0;
//					distance_level_check = 0;
//					var_write[4] = 0x19;
//					var_write[6] = 0x00;
//					var_write[7] = 0x00; // distance_level
//					HAL_Delay(50);
//					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					length_level = 20;
					length_level_check = 20;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 20; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					energy_level = 0;
					energy_level_check = 0;
					var_write[4] = 0x21;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // energy_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
										
					cartridge_shot_check = 0;
						
					break;
				
				case 5:
					
					let_start[9] = 0x0B;
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, let_start, 10, 20);		
				
					ready = 0;

					var_write[4] = 0x12;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // icon_ready
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
				
					distance_page = 2;  // wow. thankGod

				
					current_shot = 0;
					current_shot_check = 0;
					var_write[4] = 0x15;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // current_shot
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);		
					
//					distance_level = 0;
//					distance_level_check = 0;
//					var_write[4] = 0x19;
//					var_write[6] = 0x00;
//					var_write[7] = 0x00; // distance_level
//					HAL_Delay(50);
//					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					length_level = 20;
					length_level_check = 20;
					var_write[4] = 0x20;
					var_write[6] = 0x00;
					var_write[7] = 20; // length_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
					
					energy_level = 0;
					energy_level_check = 0;
					var_write[4] = 0x21;
					var_write[6] = 0x00;
					var_write[7] = 0x00; // energy_level
					HAL_Delay(50);
					HAL_UART_Transmit (&huart1, var_write, 8, 20);
										
					cartridge_shot_check = 0;
					
					break;
				
				default:
					break;
					
		}
			
		RGB_LED(cartridge_0x44_kind_check);
		
	}
	
}

//************************************** END changing to related running_cartridge page ***********************************************************		
		
//******* Start sending commands (ready + stop + cartridge_shot + current_shot + patient_shot energy + length + distance_page + distance) to STC ********
		
		if (ready_check)
		{				
				if (ready == 1)
				{
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, ready_STC, 20, 20);
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
							
				}
				else
				{
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, ready_canccel, 20, 20);
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
				}
				
				ready_check = 0;
		}		
		
///////////////////////////////////////////////////////////////////////////		
		
		if (energy_level_check != energy_level)
		{
				if (energy_level_check < energy_level)
				{
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, energy_up, 20, 20);
				}
				
				else
				{
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, energy_down, 20, 20);
				}
				energy_level_check = energy_level;
		}

///////////////////////////////////////////////////////////////////////////
		
		if (length_level_check != length_level)
		{
				if (length_level_check < length_level)
				{
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, length_up, 20, 20);
				}
				
				else
				{	
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, length_down, 20, 20);
				}
				length_level_check = length_level;
		}

///////////////////////////////////////////////////////////////////////////		
		
		if (distance_page_check != distance_page)
		{
				if (distance_page == 1)
				{
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, distance_p, 20, 20);
				}
				
				if (distance_page == 0)
				{
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, distance_p_cancel, 20, 20);
				}
				distance_page_check = distance_page;
		}

///////////////////////////////////////////////////////////////////////////
		
		if (distance_page)
		{
				if (distance_level_check != distance_level)
				{
					if (distance_level_check < distance_level)
					{
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, distance_up, 20, 20);
					}	
				
					else
					{
							HAL_Delay(50);
							HAL_UART_Transmit(&huart3, distance_down, 20, 20);
					}
					distance_level_check = distance_level;
				}
		}
		
///////////////////////////////////////////////////////////////////////////
		
		if (per_patient_check != per_patient)
		{
				per_patient_check = per_patient;
				var_write[4] = 0x18;
				var_write[6] = (per_patient >> 8);;  // Most significant byte (MSB)
				var_write[7] = (per_patient & 0xFF);  // Least significant byte (LSB)
				HAL_Delay(50);
				HAL_UART_Transmit(&huart1, var_write, 8, 20);	
		}

///////////////////////////////////////////////////////////////////////////
		
		if (current_shot_check != current_shot)
		{
				current_shot_check = current_shot;
				var_write[4] = 0x15;
				var_write[6] = (current_shot >> 8);  // Most significant byte (MSB)
				var_write[7] = (current_shot & 0xFF);  // Least significant byte (LSB)
				HAL_Delay(50);
			  HAL_UART_Transmit(&huart1, var_write, 8, 20);	
			
					
		}

///////////////////////////////////////////////////////////////////////////
		
		if (cartridge_shot_check != cartridge_shot)
		{
				cartridge_shot_check = cartridge_shot;
				var_write[4] = 0x16;
				var_write[6] = (cartridge_shot >> 8);  // Most significant byte (MSB)
				var_write[7] = (cartridge_shot & 0xFF);  // Least significant byte (LSB)
				HAL_Delay(50);
				HAL_UART_Transmit(&huart1, var_write, 8, 20);
					
		}
		
//******* END sending commands (ready + stop + cartridge_shot + current_shot + patient_shot energy + length + distance_page + distance) to STC ********
	
	
	
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 160;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 720-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 100-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7200-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 720-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 100-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 720-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 100-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA3 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim2.Instance) // 0.1 sec
    { 
				if (start_timer_stc_buzzer == 1)
				{
						timer_stc_buzzer ++;
				}
				
				if (shooting == 1)
				{
						shooting_timer ++;
				}
				
				if (ready_led_start == 1)
				{
						timer_ready_led_start ++;
				}
				
		}
		
}

/* USER CODE END 4 */

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
