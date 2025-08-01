/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "ssd1306.h"
#include "fonts.h"
#include "stdio.h"
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
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

// sin lookup table from -pi/2 to pi/2 - normalized so that values are between 0 and 1
float sinlookup[256] = {3.7649080427748505e-05, 0.00015059065189787502, 0.0003388077058252281, 0.0006022718974137975, 0.0009409435499254104, 0.0013547716606548965,
0.0018436939086109994, 0.002407636663901591, 0.0030465149988219697, 0.0037602327006450165, 0.004548682286109995,
0.005411745017609493, 0.006349290921070783, 0.007361178805529389, 0.008447256284391857, 0.009607359798384785,
0.010841314640186173, 0.012148934980735715, 0.013530023897219967, 0.014984373402728013, 0.016511764477573965,
0.01811196710228008, 0.019784740292217107, 0.021529832133895532, 0.023346979822903124, 0.025235909703481663,
0.02719633730973936, 0.029227967408489597, 0.031330494043712576, 0.03350360058263058, 0.035746959763392205,
0.03806023374435663, 0.040443074154971115, 0.042895122148234655, 0.04541600845473881, 0.04800535343827833,
0.05066276715302309, 0.05338784940224234, 0.05618018979857303, 0.05903936782582253, 0.0619649529022967,
0.06495650444564433, 0.06801357193920665, 0.07113569499986394, 0.0743224034473674, 0.07757321737514644,
0.08088764722258102, 0.08426519384872738, 0.08770534860748735, 0.09120759342420814, 0.09477140087370262,
0.09839623425967758, 0.10208154769555827, 0.10582678618669689, 0.10963138571395276, 0.11349477331863156,
0.11741636718877052, 0.12139557674675772, 0.12543180273827037, 0.12952443732252045, 0.1336728641637936,
0.13787645852426655, 0.1421345873580907, 0.14644660940672627, 0.15081187529551354, 0.15522972763146659,
0.1596995011022735, 0.16422052257649083, 0.1687921112049141, 0.17341357852311162, 0.1780842285551043,
0.18280335791817726, 0.18757025592880683, 0.1923842047096866, 0.19724447929783723, 0.20215034775378332,
0.20710107127178057, 0.21209590429107733, 0.21713409460819344, 0.2222148834901989, 0.22733750578897677,
0.23250119005645137, 0.23770515866076558, 0.24294862790338917, 0.2482308081371412, 0.253550903885108,
0.2589081139604386, 0.26430163158700115, 0.26973064452088, 0.27519433517269676, 0.28069188073073614,
0.28622245328485896, 0.29178521995118134, 0.2973793429975051, 0.303003979969476, 0.3086582838174551,
0.3143414030240812, 0.3200524817325059, 0.3257906598752828, 0.33155507330389, 0.33734485391886854,
0.34315912980055424, 0.3489970253403859, 0.35485766137276886, 0.3607401553074735, 0.3666436212625508,
0.3725671701977427, 0.37850991004836804, 0.3844709458596644, 0.39044937992156514, 0.39644431190389073,
0.4024548389919359, 0.4084800560224295, 0.41451905561984936, 0.4205709283330693, 0.42663476277231915,
0.4327096457464369, 0.4387946624003919, 0.44488889635305845, 0.45099142983521967, 0.4571013438277801,
0.46321771820016633, 0.4693396318488957, 0.47546616283629095, 0.4815963885293205, 0.4877293857385439,
0.49386423085714004, 0.5, 0.50613576914286, 0.5122706142614561, 0.5184036114706795,
0.524533837163709, 0.5306603681511043, 0.5367822817998337, 0.5428986561722199, 0.5490085701647803,
0.5551111036469416, 0.5612053375996081, 0.5672903542535631, 0.5733652372276808, 0.5794290716669307,
0.5854809443801506, 0.5915199439775705, 0.5975451610080641, 0.6035556880961093, 0.6095506200784349,
0.6155290541403355, 0.621490089951632, 0.6274328298022573, 0.6333563787374492, 0.6392598446925265,
0.6451423386272311, 0.6510029746596141, 0.6568408701994457, 0.6626551460811314, 0.66844492669611,
0.6742093401247172, 0.6799475182674941, 0.6856585969759188, 0.6913417161825449, 0.6969960200305241,
0.7026206570024949, 0.7082147800488185, 0.7137775467151409, 0.7193081192692637, 0.7248056648273034,
0.7302693554791201, 0.7356983684129988, 0.7410918860395614, 0.746449096114892, 0.7517691918628588,
0.7570513720966108, 0.7622948413392344, 0.7674988099435485, 0.7726624942110232, 0.7777851165098011,
0.7828659053918067, 0.7879040957089227, 0.7928989287282194, 0.7978496522462166, 0.8027555207021628,
0.8076157952903134, 0.8124297440711932, 0.8171966420818226, 0.8219157714448957, 0.8265864214768883,
0.831207888795086, 0.8357794774235092, 0.8403004988977265, 0.8447702723685335, 0.8491881247044865,
0.8535533905932737, 0.8578654126419093, 0.8621235414757334, 0.8663271358362064, 0.8704755626774795,
0.8745681972617296, 0.8786044232532424, 0.8825836328112295, 0.8865052266813684, 0.8903686142860472,
0.8941732138133032, 0.8979184523044417, 0.9016037657403224, 0.9052285991262974, 0.9087924065757917,
0.9122946513925125, 0.9157348061512727, 0.919112352777419, 0.9224267826248536, 0.9256775965526326,
0.9288643050001361, 0.9319864280607933, 0.9350434955543556, 0.9380350470977032, 0.9409606321741775,
0.9438198102014269, 0.9466121505977576, 0.9493372328469769, 0.9519946465617217, 0.9545839915452612,
0.9571048778517653, 0.9595569258450289, 0.9619397662556434, 0.9642530402366079, 0.9664963994173694,
0.9686695059562874, 0.9707720325915103, 0.9728036626902606, 0.9747640902965183, 0.9766530201770969,
0.9784701678661045, 0.9802152597077829, 0.98188803289772, 0.983488235522426, 0.985015626597272,
0.98646997610278, 0.9878510650192642, 0.9891586853598138, 0.9903926402016152, 0.9915527437156082,
0.9926388211944706, 0.9936507090789293, 0.9945882549823906, 0.99545131771389, 0.996239767299355,
0.9969534850011781, 0.9975923633360984, 0.998156306091389, 0.9986452283393451, 0.9990590564500745,
0.9993977281025862, 0.9996611922941747, 0.9998494093481021, 0.9999623509195723, 1.0,
};
// all possible dac output values are calculated once
float Vout_possible_values[256] = {};

// This consists of PRE-CALCULATED 256 values for each quadrant of sine wave (clock is automatically divided into 1024 steps per function period)
// Values represent voltages, as indices of Vout_possible_values
uint8_t sin_voltage_steps[1024] = {};

/*
 * 0 = sine
 * 1 = square
 * 2 = triangle
 * 3 = saw
 */
volatile uint8_t func_mode = 3;
volatile uint8_t MODE_GENERATION = 0; // Function generation is done only when this is 1
volatile float f = 500; // Tool global target frequency
volatile float f_temp = 0;
volatile uint8_t button_pressed = 0;

volatile uint8_t request_display_refresh = 0;
char oled_buffer_line1[10] = {0};
char oled_buffer_line2[10] = {0};
// Copy buffers for prev/current display content comparison so display can be updated without clearing
char new_oled_buffer_line1[10] = {0};
char new_oled_buffer_line2[10] = {0};

#define CPU_FREQ 84e6  // TIM1 frequency (Clock configuration APB2)
#define FMAX 25e3      // Tool maximum frequency
#define OLEDFONTWIDTH 11  // see fonts.h
// If button is pressed for X consecutive ticks, it is a long press
#define LONGPRESSFACTOR 0b00111111

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void dac_out(uint8_t);
void sinefunc(void);
void squarefunc(void);
void trianglefunc(void);
void sawfunc(void);
void calc_dac_values(void);
void print_to_oled(char*, char*);
void refresh_menu_display(void);
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
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  // Determine all possible dac combinations:voltage values; do once on startup
  calc_dac_values();

  SSD1306_Init ();
  request_display_refresh = 1;
  htim1.Instance->PSC = 8400;
  htim1.Instance->CCR1 = 1023;  // DAC clock has 1024 samples per frequency period
  HAL_TIM_Base_Start_IT(&htim1);
  htim3.Instance->PSC = 8400;
  // This determines short press duration (and long press but it is multiplied by a factor)
  htim3.Instance->ARR = 800;
  HAL_TIM_Base_Start_IT(&htim3);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (MODE_GENERATION){
		  HAL_TIM_Base_Stop_IT(&htim1);
		  switch(func_mode){
		  case 0:
			  request_display_refresh = 1;
			  refresh_menu_display();
			  sinefunc();
			  break;
		  case 1:
			  request_display_refresh = 1;
			  refresh_menu_display();
			  squarefunc();
			  break;
		  case 2:
			  request_display_refresh = 1;
			  refresh_menu_display();
			  trianglefunc();
			  break;
		  case 3:
			  request_display_refresh = 1;
			  refresh_menu_display();
			  sawfunc();
			  break;
		  }
	  } else {
		  // In selection menu
		  refresh_menu_display();
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
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

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 84;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1023;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 8400;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_4|LD2_Pin|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA4 LD2_Pin PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|LD2_Pin|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB10 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// Initialize string to "\0\0\0..."
void strclr(char *str, uint8_t len){
	for (int i = 0; i < len; i++){
		str[i] = 0;
	}
}

void refresh_menu_display(){
	if (!request_display_refresh) return;
	if (func_mode == 0){
		sprintf(&new_oled_buffer_line1, "SIN      ");
	} else if (func_mode == 1){
		sprintf(&new_oled_buffer_line1, "SQUARE   ");
	} else if (func_mode == 2){
		sprintf(&new_oled_buffer_line1, "TRIANGLE  ");
	} else if (func_mode == 3){
		sprintf(&new_oled_buffer_line1, "SAW      ");
	}
	if (!MODE_GENERATION){
		// Display "W" to indicate that we are in menu
		new_oled_buffer_line1[8] = 'W';
	}
	new_oled_buffer_line1[9] = '\0';
	// Fill line2 with frequency info
	sprintf(&new_oled_buffer_line2, "%d Hz%c", (uint16_t) f, '\0');
	uint8_t break_string = 0;
	// update only parts that have changed to reduce flickering
	for (int i = 0; i < 9; i++){
		if(!new_oled_buffer_line1[i]) break_string=1;
		if (break_string){
			SSD1306_GotoXY(10+i*OLEDFONTWIDTH,10);
			SSD1306_Putc(' ', &Font_11x18, 1);
		} else {
			if (new_oled_buffer_line1[i] < 32 || new_oled_buffer_line1[i] >= 126) continue;
			if (oled_buffer_line1[i] != new_oled_buffer_line1[i]){
				SSD1306_GotoXY(10+i*OLEDFONTWIDTH,10);
				SSD1306_Putc(new_oled_buffer_line1[i], &Font_11x18, 1);
			}
		}
	}
	break_string = 0;
	for (int i = 0; i < 9; i++){
		if(!new_oled_buffer_line2[i]) break_string=1;
		if (break_string){
			SSD1306_GotoXY(10+i*OLEDFONTWIDTH,30);
			SSD1306_Putc(' ', &Font_11x18, 1);
		}
		else {
			if (new_oled_buffer_line2[i] < 32 || new_oled_buffer_line2[i] >= 126) continue;
			if (oled_buffer_line2[i] != new_oled_buffer_line2[i]){
				SSD1306_GotoXY(10+i*OLEDFONTWIDTH,30);
				SSD1306_Putc(new_oled_buffer_line2[i], &Font_11x18, 1);
			}
		}
	}
	strclr(oled_buffer_line1,9);
	strclr(oled_buffer_line2,9);
	sprintf(&oled_buffer_line1, &new_oled_buffer_line1);
	sprintf(&oled_buffer_line2, &new_oled_buffer_line2);
	SSD1306_UpdateScreen();
	request_display_refresh = 0;
}

// Runs only at startup
// Calculates this equation with every 8bit combination (Va-Vh)
// Vout = (1Va + 2Vb + 4Vc + 8Vd + 16Ve + 32Vf + 64Vg + 128Vh) / 256
void calc_dac_values(){
	    int sum = 0;
	    for (int i = 0; i < 256; i++){
			sum = 0;
			for (int j = 0 ; j<=7; j++){
				sum += ((i>>j)&1) * pow(2,j);
			}
		Vout_possible_values[i] = (sum/256.0);
	    }
}

// Run once when sine mode is started for optimization
// When a full sine period is divided into 1024 time steps, calculate each timestep voltage index (out of Vout_possible_values)
void calc_sin_steps(){
	int index = 0;
	for (int i = 0; i<1024; i++){
		index = 0;
		float fraction_of_period = (i*1.0)/1023.0;
		// first ¼ ´ 	sinlookup[128:-1]
		// second ¼ ` 	sinlookup[-1:128]
		// third ¼ ` 	sinlookup [128:0]
		// fourth ¼ ´ 	sinlookup[0:128]
		if (fraction_of_period < 1.0/4.0){
			index = 127 + (255*2 * fraction_of_period);
		} else if(fraction_of_period <= 2.0/4.0){
			index = 255 - (255*2 * (fraction_of_period-0.25));
		} else if(fraction_of_period <= 3.0/4.0){
			index = 128 - (255*2 * (fraction_of_period-0.5));
		} else {
			index = (255*2 * (fraction_of_period-0.75));
		}
		for (int j = 0; j<=255; j++){
			if (j==255){
				sin_voltage_steps[i] = 255;
				break;
			}
			else if (Vout_possible_values[j] < sinlookup[index] && Vout_possible_values[j+1] > sinlookup[index]){
				sin_voltage_steps[i] = j;
				break;
			}
		}
	}
}

// Sends parallel 8bit "digital out" to respective gpio pins
// input argument is precalculated 8bit value that matches with target voltage value
void dac_out(uint8_t val){
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, val & (1<<0));
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, val & (1<<1));
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, val & (1<<2));
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, val & (1<<3));
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, val & (1<<4));
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, val & (1<<5));
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, val & (1<<6));
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, val & (1<<7));
}

// Divide timer so that it only ticks 1024 times every 1/f
// (using 8 bit dac and more ticks doesn't bring any benefits, only makes code more complex ... maybe slower too?)
void prescale_timer(int16_t sinfreq){
	htim1.Instance->PSC = (CPU_FREQ*1.0) / (1024.0*sinfreq);
}

// Main loop when SINE is selected as tool mode
void sinefunc(){
	calc_sin_steps();
	prescale_timer(f);
	HAL_TIM_Base_Start(&htim1);
	while(MODE_GENERATION){
		dac_out(sin_voltage_steps[htim1.Instance->CNT]);
	}
	dac_out(0);  // reset gpio pin states
}

void squarefunc(){
	prescale_timer(f);
	uint16_t timstep = 0;
	HAL_TIM_Base_Start(&htim1);
	while(MODE_GENERATION){
		timstep = TIM1->CNT;
		if(timstep < 1024/2){
			dac_out(255);
		} else {
			dac_out(0);
		}
	}
	dac_out(0);
}

void trianglefunc(){
	prescale_timer(f);
	uint16_t timstep = 0;
	HAL_TIM_Base_Start(&htim1);
	while(MODE_GENERATION){
		timstep = TIM1->CNT;
		if(timstep < 1024/2){
			dac_out(timstep/2);
		} else {
			dac_out(255-(timstep/2));
		}
	}
	dac_out(0);
}

void sawfunc(){
	prescale_timer(f);
	uint16_t timstep = 0;
	HAL_TIM_Base_Start(&htim1);
	while(MODE_GENERATION){
		timstep = TIM1->CNT;
		dac_out(timstep/4-1);
	}
	dac_out(0);
}

// Rounds value to nearest 100
float round_frequency(float freq){
	return (float) ((uint16_t) freq - (uint16_t) freq % 100);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// TIM3 is used for detecting button presses only
	if (htim->Instance==TIM3){
		uint8_t longpress = 0;
		if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)){
			button_pressed = (button_pressed << 1) | 1;
		} else {
			button_pressed = (button_pressed << 1) & ~1;
		}
		// Falling edge detection, button is released, check if it was pressed for a short time or held
		if ((button_pressed & 0b00000011) == 0b00000010){
			if (button_pressed >= LONGPRESSFACTOR){
					longpress = 1;
				}
			HAL_TIM_Base_Stop_IT(&htim1);
			// Button is clicked  in tool mode, ignore long presses
			if (MODE_GENERATION && !longpress){
				MODE_GENERATION = 0;  // return to menu mode
			// Button is clicked  in menu mode
			} else {
				// long press cycles functions, does not start tool mode
				if (longpress){
					func_mode++;
					if (func_mode > 3) {
						func_mode = 0;
					}
				// Button clicking starts tool mode in displayed function and frequency
				} else {
					MODE_GENERATION = 1;
				}
			}
			// clear button timing array after a command is detected
			button_pressed = 0;
			request_display_refresh = 1;
			refresh_menu_display();
			TIM1->CNT = 0;
			HAL_TIM_Base_Start_IT(&htim1);
		}
	}
	// TIM1 interrupts are used for potentiometer reading IN MENU MODE
	// TIM1 is also used in function timing IN TOOL MODE but without interrupts
	if (htim-> Instance==TIM1){
		float adcval = 0.0;
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 10);
		adcval = HAL_ADC_GetValue(&hadc1)/4095.0 * FMAX;
		f_temp = f;
		if (adcval < 50){
			f = 10;
		} else {
			f = round_frequency(adcval);
		}
		HAL_ADC_Stop(&hadc1);
		if (f < f_temp - 80 || f > f_temp + 80){  // hysteresis because adc setup is noisy...
			request_display_refresh = 1;
		} else {
			f = f_temp;  // Revert value if conditions aren't met
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
