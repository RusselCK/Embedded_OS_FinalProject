/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "printf.h"

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"

#include "minunit.h"
#include <treadmill/gc.h>
#include <treadmill/darray.h>
#include <treadmill/vector.h>
#include <treadmill/gc_test_setup.h>
#include <treadmill/state.h>
#include <treadmill/object.h>
#include <treadmill/gc_hashmap.h>
#include <treadmill/gc_vector.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BOTTOM  heap->bottom
#define TOP     heap->top
#define SCAN    heap->scan
#define FREE    heap->free
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void  printf_test(void *pvParameters);
void  darray_test(void *pvParameters);
void  gc_test(void *pvParameters);
void  vector_test(void *pvParameters);
void  hashmap_test(void *pvParameters);
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
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
//  xTaskCreate( printf_test, " printf_test", 500, NULL, 1, NULL);

//  xTaskCreate( darray_test, " darray_test", 500, NULL, 1, NULL);
//  xTaskCreate( gc_test, " gc_test", 500, NULL, 1, NULL);
  xTaskCreate( vector_test, " vector_test", 500, NULL, 1, NULL);
//  xTaskCreate( hashmap_test, " hashmap_test", 500, NULL, 1, NULL);
  vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void printf_test(void *pvParameters){

	while(1) {
//		char monitor[30] = "hello world\n\r";
//		HAL_UART_Transmit(&huart4, (uint8_t *)monitor, strlen(monitor), 0xFFFF);
		printf("hello world!\n\r");
	}
}


void darray_test(void *pvParameters) {

	while(1) {
		Tm_DArray *array = NULL;
		int *val1 = NULL;
		int *val2 = NULL;

		// create test
		array = Tm_DArray_create(sizeof(int), 100);
		// new test
		val1 = Tm_DArray_new(array);
		val2 = Tm_DArray_new(array);
		// set test
		Tm_DArray_set(array, 0, val1);
		Tm_DArray_set(array, 1, val2);
		// get test
		void* get1 = Tm_DArray_get(array, 0);
		void* get2 = Tm_DArray_get(array, 1);
		// remove test
		int *val_check = Tm_DArray_remove(array, 0);
		Tm_DArray_free(val_check);
		val_check = Tm_DArray_remove(array, 1);
		Tm_DArray_free(val_check);


		// destroy test
		Tm_DArray_destroy(array);
	}
}

void test_size (TmHeap* heap, double* heap_size, double* white_size, double* ecru_size, double* grey_size, double* black_size) {

	*heap_size = TmHeap_size(heap);
	*white_size = TmHeap_white_size(heap);
	*ecru_size = TmHeap_ecru_size(heap);
	*grey_size = TmHeap_grey_size(heap);
	*black_size = TmHeap_black_size(heap);
}

void  gc_test(void *pvParameters) {

	double heap_size, white_size, ecru_size, grey_size, black_size;

	while (1) {
		// allocate
		/*
		State *state = State_new();
		TmHeap *heap = new_heap(state, 10, 10);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		Object *obj  = Object_new(heap);
		TmCell *cell = obj->gc.cell;
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		TmHeap_destroy(heap);
		State_destroy(state);

		// allocate and flip
		state = State_new();
		heap = new_heap(state, 3, 3);

		Object_new(heap);
		Object_new(heap);
		Object_new(heap);
		Object_new(heap); // this triggers a flip
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		TmHeap_destroy(heap);
		State_destroy(state);
		*/
		// allocate and flip twice
		State *state = State_new();
		TmHeap *heap = new_heap(state, 3, 3);

		/*
		 * parent1 ->
		 *   child11
		 *   child12
		 */
		Object *parent1 = Number_new(heap, 12);
		Object_make_root(parent1, state);
		Object *child11 = Number_new(heap, 34);
		Object_relate(parent1, child11);

		Object *child12 = String_new(heap, "hello, world!");
		Object_relate(parent1, child12);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		Number_new(heap, 32); // this triggers a flip
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		Number_new(heap, 33);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		String_new(heap, "hi");  // this triggers a scan
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		String_new(heap, "bye"); // this triggers another flip
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		TmHeap_destroy(heap);
		State_destroy(state);
	}
}

int count = 0;
void count_iter(Object* obj) {

	count = count + OBJ2NUM(obj);
}
//void relate_iter
void  vector_test(void *pvParameters) {

	double heap_size, white_size, ecru_size, grey_size, black_size;

	while (1) {
		State *state = State_new();
		TmHeap *heap = new_heap(state, 3, 3);

		Object* vector = Vector_new(heap, 10);
		Object_make_root(vector, state);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		gc_vector_push(heap, vector, 1);
		gc_vector_push(heap, vector, 2);
		gc_vector_push(heap, vector, 3);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		count = 0;
		Vector_each(vector, count_iter);
		printf("count = %d\r\n", count);

		Number_new(heap, 7);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		Number_new(heap, 7); //scan
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		Number_new(heap, 7); // add rootset
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		gc_vector_clear(vector);

		count = 0;
		Vector_each(vector, count_iter);
		printf("count = %d\r\n", count);

		Number_new(heap, 7);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		Number_new(heap, 7);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		Number_new(heap, 7);  // flip + scan + add rootset
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		TmHeap_destroy(heap);
		State_destroy(state);
	}
}


void  hashmap_test(void *pvParameters) {


	double heap_size, white_size, ecru_size, grey_size, black_size;

	while (1) {
		State *state = State_new();
		TmHeap *heap = new_heap(state, 3, 4);

		Object* map = Map_new(heap);
		Object_make_root(map, state);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		// set and get
		int rc = gc_Hashmap_set(heap, map, "test data 1", "THE OBJECT 1");
		char* result = gc_Hashmap_get(map, "test data 1");
		printf("hashmap get: %d\n\r", 333);
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		rc = gc_Hashmap_set(heap, map, "test data 2", "THE OBJECT 2");
		result = gc_Hashmap_get(map, "test data 2");
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		gc_Hashmap_delete(map, "test data 1");
		result = gc_Hashmap_get(map, "test data 1");
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		rc = gc_Hashmap_set(heap, map, "test data 3", "THE OBJECT 3");
		result = gc_Hashmap_get(map, "test data 3");
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);
		// delete
		gc_Hashmap_delete(map, "test data 1");
		result = gc_Hashmap_get(map, "test data 1");
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		gc_Hashmap_delete(map, "test data 2");
		result = gc_Hashmap_get(map, "test data 2");
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		gc_Hashmap_delete(map, "test data 3");
		result = gc_Hashmap_get(map, "test data 3");
		test_size(heap, &heap_size, &white_size, &ecru_size, &grey_size, &black_size);

		TmHeap_destroy(heap);
		State_destroy(state);
	}
}
/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
