#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "vm.h"
#include "vm_devices.h"

extern vm_t vm;
TIM_HandleTypeDef TimHandle;
static void SystemClock_Config(void);
static void Error_Handler(void);

/*
errcode_t init_uart(vm_uart_t * uart)
{
    uint16_t i;
    for (i = 0; i < VM_UART_QUEUE_SIZE; i++)
    {
        uart->in.queue[i] = '\0';
        uart->out.queue[i] = '\0';
    }
    uart->in.queue_start = uart->in.queue_end = uart->in.queue;
    uart->out.queue_start = uart->out.queue_end = uart->out.queue;
    return OK;
}
*/

errcode_t init_hw()
{
    int i;
    HAL_Init();
    SystemClock_Config();
    
    /* Enable the GPIO Clock */
    __GPIOA_CLK_ENABLE();
    for(i = 4; i < 12; i++)
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        
        /* Configure the GPIO_LED pin */
        GPIO_InitStruct.Pin = 1 << i;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
        
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        HAL_GPIO_WritePin(GPIOA, 1 << i, GPIO_PIN_RESET); 
    }
    return OK;
}

errcode_t gpio_get(uint32_t pin, uint8_t * result)
{
    if(pin >= PIN_COUNT)
    {
        Error_Handler();
    }
    *result = (HAL_GPIO_ReadPin(GPIOA, 1 << pin) ? 1 : 0);
    return OK;
}

errcode_t gpio_set(uint32_t pin, uint8_t value)
{
    if(pin >= PIN_COUNT)
    {
        Error_Handler();
    }
    HAL_GPIO_WritePin(GPIOA, 1 << pin, (value ? GPIO_PIN_SET : GPIO_PIN_RESET));
    return OK;
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSI)
 *            SYSCLK(Hz)                     = 84000000
 *            HCLK(Hz)                       = 84000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 2
 *            APB2 Prescaler                 = 1
 *            HSI Frequency(Hz)              = 16000000
 *            PLL_M                          = 16
 *            PLL_N                          = 336
 *            PLL_P                          = 4
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale2 mode
 *            Flash Latency(WS)              = 2
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /* Enable HSI Oscillator and activate PLL with HSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  This function configures the TIM6 as a time base source.
 *         The time source is configured  to have 1ms time base with a dedicated
 *         Tick interrupt priority.
 * @note   This function is called automatically at the beginning of program after
 *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
 * @param  TickPriority: Tick interrupt priority.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
    RCC_ClkInitTypeDef sClokConfig;
    uint32_t uwTimclock, uwAPB1Prescaler = 0;
    uint32_t uwPrescalerValue = 0;
    uint32_t pFLatency;

    /* Configure the TIM6 IRQ priority */
    HAL_NVIC_SetPriority(TIM5_IRQn, TickPriority ,0);

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&sClokConfig, &pFLatency);

    /* Get APB1 prescaler */
    uwAPB1Prescaler = sClokConfig.APB1CLKDivider;

    /* Compute TIM6 clock */
    if (uwAPB1Prescaler == 0)
    {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
        uwTimclock = 2*HAL_RCC_GetPCLK1Freq();
    }

    /* Compute the prescaler value to have TIM5 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000) - 1);

    /* Initialize TIM5 */
    TimHandle.Instance = TIM5;

    /* Initialize TIMx peripheral as follow:
       + Period = [(TIM5CLK/1000) - 1]. to have a (1/1000) s time base.
       + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
       + ClockDivision = 0
       + Counter direction = Up
       */
    TimHandle.Init.Period = (1000000 / 1000) - 1; /* 1 ms */
    /*TimHandle.Init.Period = (1000000 / 40000) - 1;*/ /* 75 us */
    TimHandle.Init.Prescaler = uwPrescalerValue;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* Start the TIM time Base generation in interrupt mode */
    if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    vm.time++;
    vm.proc_table[vm.PP].current_observed_time++;
    reschedule(&vm);
}

static void Error_Handler(void)
{
    while(1)
    {
    }
}
