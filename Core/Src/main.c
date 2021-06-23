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
#include "semphr.h"
#include "stm32f4xx_hal.h"

#include "minunit.h"

#include <treadmill/darray.h>

#include <treadmill/gc.h>
#include <treadmill/gc_test_setup.h>
#include <treadmill/state.h>
#include <treadmill/object.h>

#include <treadmill/vector.h>
#include <treadmill/gc_vector.h>

#include <treadmill/gc_hashmap.h>

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
UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */
State *state;
TmHeap *heap;
SemaphoreHandle_t xSemaphore;

TaskHandle_t vector_task_handle = NULL;
TaskHandle_t map_task_handle = NULL;
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
void  gc_flip_test(void *pvParameters);
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
  xSemaphore = xSemaphoreCreateMutex();

//  xTaskCreate( printf_test, " printf_test", 500, NULL, 1, NULL);

//  xTaskCreate( darray_test, " darray_test", 500, NULL, 1, NULL);
//  xTaskCreate( gc_test, " gc_test", 500, NULL, 1, NULL);
//  xTaskCreate( gc_flip_test, "gc_flip_test", 500, NULL, 1, NULL);

  state = State_new();
  heap = new_heap(state, 3, 4);
  debug("[TmHeap] New a TmHeap with size 4 and growth rate 4");
  TmHeap_print(heap);
//  xTaskCreate( vector_test, " vector_test", 500, NULL, 1, &vector_task_handle);
//  xTaskCreate( vector_test, " vector_test_2", 500, NULL, 1,NULL);
  xTaskCreate( hashmap_test, " hashmap_test", 500, NULL, 1, &map_task_handle);

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

		// create a 100 elements int DArray
		Tm_DArray *array = Tm_DArray_create(sizeof(int), 100);
		mu_assert(array != NULL, "DArray create failed.");
		mu_assert(array->contents != NULL, "contents are wrong in DArray");
		mu_assert(array->end == 0, "end isn't at the right spot");
		mu_assert(array->element_size == sizeof(int), "element size is wrong.");
		mu_assert(array->max == 100, "wrong max length on initial size");
		debug("[DArray] Create a 100 elements int DArray.");
		// new a int DArray element
		int *val1 = Tm_DArray_new(array);
		mu_assert(val1 != NULL, "failed to make a new element.");
		debug("[DArray] New the first int DArray element.");
		int *val2 = Tm_DArray_new(array);
		mu_assert(val2 != NULL, "failed to make a new element.");
		debug("[DArray] New the second int DArray element.");
		// set
		Tm_DArray_set(array, 0, val1);
		debug("[DArray] Set first element in array[0].");
		Tm_DArray_set(array, 1, val2);
		debug("[DArray] Set second value in array[1].");
		// get
		void* get1 = Tm_DArray_get(array, 0);
		mu_assert(Tm_DArray_get(array, 0) == val1, "Wrong first value.");
		debug("[DArray] Get correct first value.");
		void* get2 = Tm_DArray_get(array, 1);
		mu_assert(Tm_DArray_get(array, 1) == val2, "Wrong second value.");
		debug("[DArray] Get correct second value.");
		// remove
		int *val_check = Tm_DArray_remove(array, 0);
		mu_assert(val_check != NULL, "Should not get NULL.");
		mu_assert(*val_check == *val1, "Should get the first value.");
		mu_assert(Tm_DArray_get(array, 0) == NULL, "Should be gone.");
		Tm_DArray_free(val_check);
		debug("[DArray] Remove the first element.");
		val_check = Tm_DArray_remove(array, 1);
		mu_assert(val_check != NULL, "Should not get NULL.");
		mu_assert(*val_check == *val2, "Should get the first value.");
		mu_assert(Tm_DArray_get(array, 1) == NULL, "Should be gone.");
		Tm_DArray_free(val_check);
		debug("[DArray] Remove the second element.");
		// expand
		int old_max = array->max;
		Tm_DArray_expand(array);
		mu_assert((unsigned int)array->max == old_max + array->expand_rate, "Wrong size after expand.");
		debug("[DArray] Expand the DArray. ");
		//contract
		Tm_DArray_contract(array);
		mu_assert((unsigned int)array->max == array->expand_rate + 1, "Should stay at the expand_rate at least.");

		Tm_DArray_contract(array);
		mu_assert((unsigned int)array->max == array->expand_rate + 1, "Should stay at the expand_rate at least.");
		// push & pop
		for(int i = 0; i < 100; i++) {
		  int *val = Tm_DArray_new(array);
		  *val = i * 333;
		  Tm_DArray_push(array, val);
		}
		mu_assert(array->max == 101, "Wrong max size.");
		for(int i = 99; i >= 0; i--) {
		  int *val = Tm_DArray_pop(array);
		  mu_assert(val != NULL, "Shouldn't get a NULL.");
		  mu_assert(*val == i * 333, "Wrong value.");
		  Tm_DArray_free(val);
		}
   	    debug("[DArray] Push and Pop test success.");

		// destroy
		Tm_DArray_destroy(array);
	}
}

void test_size (TmHeap* heap, int* heap_size, int* white_size, int* ecru_size, int* grey_size, int* black_size) {

	portDISABLE_INTERRUPTS();
		*heap_size = TmHeap_size(heap);
		*white_size = TmHeap_white_size(heap);
		*ecru_size = TmHeap_ecru_size(heap);
		*grey_size = TmHeap_grey_size(heap);
		*black_size = TmHeap_black_size(heap);
		fprintf(stderr, "heap = %d, white = %d, ecru = %d, grey = %d, black = %d\n\r", *heap_size, *white_size, *ecru_size, *grey_size , *black_size);
	portENABLE_INTERRUPTS();
}

void gc_flip_test(void *pvParameters) {

	while (1) {
		state = State_new();
		heap = new_heap(state, 3, 3);
		debug("[TmHeap] New a TmHeap with size 4 and growth rate 3");
		TmHeap_print(heap);

		Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate the first Object in TmHeap");
		TmHeap_print(heap);

		Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate the second Object in TmHeap");
		TmHeap_print(heap);

		Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate the third Object in TmHeap");
		TmHeap_print(heap);

		Object_new(heap, ObjectType); // this triggers a flip
		debug("[TmHeap] Allocate the 4th Object in TmHeap");
		debug("[TmHeap] TmHeap size grow up from 4 to 7 by a flip");
		TmHeap_print(heap);

		TmHeap_destroy(heap);
		State_destroy(state);
		debug("[TmHeap] Destroy the TmHeap\n");
	}
}


void  gc_test(void *pvParameters) {

//	int heap_size, white_size, ecru_size, grey_size, black_size;

	while (1) {

		// heap new
		state = State_new();
		heap = new_heap(state, 10, 10);
		assert_heap_size(11);
		assert_white_size(11);
		assert_ecru_size(0);
		assert_grey_size(0);
		assert_black_size(0);
		debug("[TmHeap] New a TmHeap with size 11 ");
		TmHeap_print(heap);

		TmHeap_destroy(heap);
		State_destroy(state);
		debug("[TmHeap] Destroy the TmHeap");
		// heap grow
		state = State_new();
		heap = new_heap(state, 3, 10);
		assert_heap_size(4);
		debug("[TmHeap] New a TmHeap with size 4");
		TmHeap_print(heap);

		TmHeap_grow(heap, 2);
		assert_heap_size(6);
		assert_white_size(6);
		assert_ecru_size(0);
		assert_grey_size(0);
		assert_black_size(0);
		debug("[TmHeap] TmHeap size grow up from 4 to 6");
		TmHeap_print(heap);

		TmHeap_destroy(heap);
		State_destroy(state);
		debug("[TmHeap] Destroy the TmHeap\n");
		// heap allocate
		state = State_new();
		heap = new_heap(state, 10, 10);
		debug("[TmHeap] New a TmHeap with size 11 ");
		TmHeap_print(heap);

		Object *obj  = Object_new(heap, ObjectType);
		TmCell *cell = obj->gc.cell;
		mu_assert(cell == FREE->prev, "Cell should be right before the free pointer");
		mu_assert(heap->allocs == 1, "Allocation didn't update the allocs count.");
		assert_heap_size(11);
     	assert_white_size(10);
		assert_ecru_size(0);
		assert_grey_size(0);
		assert_black_size(1);
		debug("[TmHeap] Allocate an Object in TmHeap");
//		TmHeap_print_all(heap);
		TmHeap_print(heap);

		TmHeap_destroy(heap);
		State_destroy(state);
		debug("[TmHeap] Destroy the TmHeap\n");
		// allocate and flip
		state = State_new();
		heap = new_heap(state, 3, 3);
		debug("[TmHeap] New a TmHeap with size 4 and growth rate 3");
		TmHeap_print(heap);

		Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate the first Object in TmHeap");
		TmHeap_print(heap);

		Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate the second Object in TmHeap");
		TmHeap_print(heap);

		Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate the third Object in TmHeap");
		TmHeap_print(heap);

		Object_new(heap, ObjectType); // this triggers a flip
		assert_heap_size(7);
	    assert_white_size(3); // the newly grown region
		assert_ecru_size(3);  // the three first allocated objects
		assert_grey_size(0);
		assert_black_size(1);
		debug("[TmHeap] Allocate the 4th Object in TmHeap");
		debug("[TmHeap] TmHeap size grow up from 4 to 7 by a flip");
//		TmHeap_print_all(heap);
		TmHeap_print(heap);

		TmHeap_destroy(heap);
		State_destroy(state);
		debug("[TmHeap] Destroy the TmHeap\n");
		// allocate and flip twice
		state = State_new();
		heap = new_heap(state, 3, 3);
		debug("[TmHeap] New a TmHeap with size 4 and growth rate 3");
		TmHeap_print(heap);
		/*
		 * parent1 ->
		 *   child11
		 *   child12
		 */
		Object *parent1 = Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate an Object 'Parent1' in TmHeap");
		Object_make_root(parent1, state);
		debug("[TmHeap]  Make 'Parent1' be a root");
		TmHeap_print(heap);

		Object *child11 = Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate an Object 'Child11' in TmHeap");
		Object_relate(parent1, child11);
		debug("[TmHeap] Let 'Child11' be a child of 'Parent1'");
		TmHeap_print(heap);

		Object *child12 = Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate an Object 'Child12' in TmHeap");
		Object_relate(parent1, child12);
		debug("[TmHeap] Let 'Child12' be a child of 'Parent1'");
		TmHeap_print(heap);

		Object_new(heap, ObjectType); // this triggers a flip
		debug("[TmHeap] Allocate an Object in TmHeap");
//		TmHeap_print_all(heap);
		TmHeap_print(heap);

		Object_new(heap, ObjectType);
		debug("[TmHeap] Allocate an Object in TmHeap");
		TmHeap_print(heap);

		Object_new(heap, ObjectType);  // this triggers a scan
		debug("[TmHeap] Allocate an Object in TmHeap");
//		TmHeap_print_all(heap);
		TmHeap_print(heap);

		Object_new(heap, ObjectType); // this triggers another flip
		assert_heap_size(10);
		assert_white_size(3); // the newly grown region
		assert_ecru_size(5);
		assert_grey_size(1);
		assert_black_size(1);
		debug("[TmHeap] Allocate an Object in TmHeap");
//		TmHeap_print_all(heap);
		TmHeap_print(heap);

		TmHeap_destroy(heap);
		State_destroy(state);
		debug("[TmHeap] Destroy the TmHeap\n");
	}
}

int count = 0;
void count_iter(Object* obj) {

	count = count + OBJ2NUM(obj);
}
//void relate_iter
void  vector_test(void *pvParameters) {

//	state = State_new();
//	heap = new_heap(state, 3, 4);
//	debug("[TmHeap] New a TmHeap with size 4 and growth rate 4");
//    TmHeap_print(heap);

	while (1) {
		printf("\r\n");
		debug("[vector] vector task");
		TmHeap_print(heap);

		Object* vector = Vector_new(heap, 10);
		if (!vector) continue;
		mu_assert(vector->type == VectorType, "failed creating vector");
		mu_assert(OBJ2ARY(vector)->max == 10, "failed assigning array to vector");
		debug("[vector] create a vector with size 10");
		Object_make_root(vector, state);
		debug("[vector]  Make the vector be a root");
		TmHeap_print(heap);

		if (gc_vector_push(heap, vector, 1) == 0) {
			debug("[vector] push 1 in the vector");
			TmHeap_print(heap);
		}
		if (gc_vector_push(heap, vector, 2) == 0) {
			debug("[vector] push 2 in the vector");
			TmHeap_print(heap);
		}
		if (gc_vector_push(heap, vector, 3) == 0){ // flip
			debug("[vector] push 3 in the vector");
			TmHeap_print(heap);
		}

		count = 0;
		Vector_each(vector, count_iter);
		printf("sum of vector = %d\r\n", count);

		Number_new(heap, 7);
		debug("[TmHeap] Allocate a Number Object in TmHeap");
		TmHeap_print(heap);

		Number_new(heap, 7); //scan
		debug("[TmHeap] Allocate a Number Object in TmHeap");
		TmHeap_print(heap);

		Number_new(heap, 7);
		debug("[TmHeap] Allocate a Number Object in TmHeap");
		TmHeap_print(heap);

		gc_vector_clear(vector);
		debug("[vector] clear the vector");
		TmHeap_print(heap);

		count = 0;
		Vector_each(vector, count_iter);
		printf("sum of vector = %d\r\n", count);

		Number_new(heap, 7); // flip
		debug("[TmHeap] Allocate a Number Object in TmHeap");
		TmHeap_print(heap);

		Number_new(heap, 7);
		debug("[TmHeap] Allocate a Number Object in TmHeap");
		TmHeap_print(heap);

		Number_new(heap, 7);
		debug("[TmHeap] Allocate a Number Object in TmHeap");
		TmHeap_print(heap);

		Number_new(heap, 7);
		debug("[TmHeap] Allocate a Number Object in TmHeap");
		TmHeap_print(heap);

		Object_delete_root(vector, state);
		debug("[vector] delete vector from rootset");

	}
}


void  hashmap_test(void *pvParameters) {


//	state = State_new();
//	heap = new_heap(state, 3, 4);
//	debug("[TmHeap] New a TmHeap with size 4 and growth rate 4");
//    TmHeap_print(heap);

	while (1) {

		printf("\r\n");
		debug("[hashmap] map task");
		TmHeap_print(heap);

		// map new
		Object* map = Map_new(heap);
		if (!map) continue;
		mu_assert(map->type == MapType, "failed creating map");
		mu_assert(OBJ2HASH(map), "failed assigning hash to map");
		debug("[hashmap] create a hashmap Object");
		Object_make_root(map, state);
		debug("[hashmap]  Make the hashmap be a root");
		TmHeap_print(heap);
		// set and get
		char* result;
		if( gc_Hashmap_set(heap, map, "test data 1", "THE OBJECT 1") == 0) {
			debug("[hashmap] map set 1");
			result = gc_Hashmap_get(map, "test data 1");
//			printf("The data of key 'test data 1' is '%s'\r\n", result);
			TmHeap_print(heap);
		}
		if( gc_Hashmap_set(heap, map, "test data 2", "THE OBJECT 2") == 0) { // flip
			debug("[hashmap] map set 2");//stop
			result = gc_Hashmap_get(map, "test data 2");
//			printf("The data of key 'test data 2' is '%s'\r\n", result);
			TmHeap_print(heap);
		}
		if( gc_Hashmap_set(heap, map, "test data 3", "THE OBJECT 3") == 0) {
			debug("[hashmap] map set 3");
			result = gc_Hashmap_get(map, "test data 3");
//			printf("The data of key 'test data 3' is '%s'\r\n", result);
			TmHeap_print(heap);
		}
		if( gc_Hashmap_set(heap, map, "test data 4", "THE OBJECT 4") == 0) {
			debug("[hashmap] map set 4");
			result = gc_Hashmap_get(map, "test data 4");
//			printf("The data of key 'test data 4' is '%s'\r\n", result);
			TmHeap_print(heap);
		}
		// delete
		gc_Hashmap_delete(map, "test data 1");
		result = gc_Hashmap_get(map, "test data 1");
		debug("[hashmap] map delete 1");
		TmHeap_print(heap);

		gc_Hashmap_delete(map, "test data 2");
		result = gc_Hashmap_get(map, "test data 2");
		debug("[hashmap] map delete 2");
		TmHeap_print(heap);

		gc_Hashmap_delete(map, "test data 3");
		result = gc_Hashmap_get(map, "test data 3");
		debug("[hashmap] map delete 3");
		TmHeap_print(heap);

		gc_Hashmap_delete(map, "test data 4");
		result = gc_Hashmap_get(map, "test data 4");
		debug("[hashmap] map delete 4");
		TmHeap_print(heap);

		String_new(heap, "String A");
		debug("[TmHeap] Allocate a String Object in TmHeap");
		TmHeap_print(heap);

		String_new(heap, "String B");
		debug("[TmHeap] Allocate a String Object in TmHeap");
		TmHeap_print(heap);

		Object_delete_root(map, state);
		debug("[hashmap] delete map from rootset");

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
