

// development test platform
#define BP_PLATFORM		"vNG1"
#define FIRMWARE_VERSION "v8"

// SPI hardware (HW-SPI)
#define BP_SPI			SPI2
#define BP_SPI_CLK		RCC_SPI2
#define BP_SPI_MISO_PORT	GPIOB
#define BP_SPI_MOSI_PORT	GPIOB
#define BP_SPI_CS_PORT		GPIOB
#define BP_SPI_CLK_PORT		GPIOB
#define BP_SPI_MISO_PIN		GPIO14
#define BP_SPI_MOSI_PIN		GPIO15
#define BP_SPI_CS_PIN		GPIO12
#define BP_SPI_CLK_PIN		GPIO13

// AUX/PWM/freq counter
#define BP_AUX_PORT		GPIOB
#define BP_AUX_PIN		GPIO0

#define BP_PWM_CLOCK		RCC_TIM3
#define BP_PWM_PORT		GPIOB
#define BP_PWM_PIN		GPIO0
#define BP_PWM_TIMER		TIM3
#define BP_PWM_CHAN		TIM_OC3
#define BP_PWM_CHANCHAN		3

#define BP_FREQ_PORT		GPIOD
#define BP_FREQ_PIN		GPIO2
#define BP_FREQ_TIMER		TIM3
#define BP_FREQ_CLK		RCC_TIM3
#define BP_FREQ_NVIC		NVIC_TIM3_IRQ

// ADC's
#define BP_ADC			ADC1
#define BP_ADC_CLK		RCC_ADC1
#define BP_ADC_CHAN		9
#define BP_ADC_PORT		GPIOB
#define BP_ADC_PIN		GPIO1
#define BP_3V3_CHAN		11
#define BP_3V3_PORT		GPIOC
#define BP_3V3_PIN		GPIO1
#define BP_5V0_CHAN		12
#define BP_5V0_PORT		GPIOC
#define BP_5V0_PIN		GPIO2
#define BP_VPU_CHAN		14		// vpu fed into resistors
#define BP_VPU_PORT		GPIOC
#define BP_VPU_PIN		GPIO4
#define BP_VSUP_CHAN		10
#define BP_VSUP_PORT		GPIOC
#define BP_VSUP_PIN		GPIO0
#define BP_VEXT_CHAN		13		// voltage on Vpu pin
#define BP_VEXT_PORT		GPIOC
#define BP_VEXT_PIN		GPIO3


// debug USART
#define BP_DEBUG_TX_PORT	GPIOA
#define BP_DEBUG_TX_PIN		GPIO9
#define BP_DEBUG_USART		USART1
#define BP_DEBUG_USART_CLK	RCC_USART1

// regular USART (HW-USART)
#define BP_USART_TX_PORT	GPIOB
#define BP_USART_RX_PORT	GPIOB
#define BP_USART_TX_PIN		GPIO10
#define BP_USART_RX_PIN		GPIO11
#define BP_USART_CLK		RCC_USART3
#define BP_USART		USART3

// hw-i2c
#define BP_I2C			I2C1
#define BP_I2C_CLK		RCC_I2C1
#define BP_I2C_SDA_PORT		GPIOB
#define BP_I2C_CLK_PORT		GPIOB
#define BP_I2C_SDA_PIN		GPIO7
#define BP_I2C_CLK_PIN		GPIO6

#define BP_I2C_SDA_SENSE_PORT	GPIOB
#define BP_I2C_SDA_SENSE_PIN	GPIO10
#define BP_I2C_SCL_SENSE_PORT	GPIOB
#define BP_I2C_SCL_SENSE_PIN	GPIO13




// logic analyzer
#define BP_LA_SRAM_SIZE 0x20000 //size in bytes
#define BP_LA_BUFFSIZE		2048

#define BP_LA_SRAM_CLK_PORT	GPIOA
#define BP_LA_SRAM_CLK_PIN	GPIO8
#define BP_LA_SRAM_CS_PORT	GPIOC
#define BP_LA_SRAM_CS_PIN	GPIO13

#define BP_LA_LATCH_PORT	GPIOC
#define BP_LA_LATCH_PIN		GPIO7

#define BP_LA_CHAN1_PORT	GPIOA
#define BP_LA_CHAN1_PIN		GPIO0
#define BP_LA_CHAN2_PORT	GPIOA
#define BP_LA_CHAN2_PIN		GPIO1
#define BP_LA_CHAN3_PORT	GPIOA
#define BP_LA_CHAN3_PIN		GPIO2
#define BP_LA_CHAN4_PORT	GPIOA
#define BP_LA_CHAN4_PIN		GPIO3
#define BP_LA_CHAN5_PORT	GPIOA
#define BP_LA_CHAN5_PIN		GPIO4
#define BP_LA_CHAN6_PORT	GPIOA
#define BP_LA_CHAN6_PIN		GPIO5
#define BP_LA_CHAN7_PORT	GPIOA
#define BP_LA_CHAN7_PIN		GPIO6
#define BP_LA_CHAN8_PORT	GPIOA
#define BP_LA_CHAN8_PIN		GPIO7

#define BP_LA_TIM_CLOCK		RCC_TIM1
#define BP_LA_TIMER			TIM1
#define BP_LA_TIM_CHAN		TIM_OC1
#define BP_LA_TIM_NVIC		NVIC_TIM1_CC_IRQ

#define BP_LA_COUNTER_CLOCK	RCC_TIM2
#define BP_LA_COUNTER		TIM2
#define BP_LA_COUNTER_NVIC	NVIC_TIM2_IRQ

// SW 3wire (SW3W)
#define BP_SW3W_MISO_PORT	GPIOB
#define BP_SW3W_MOSI_PORT	GPIOB
#define BP_SW3W_CS_PORT		GPIOB
#define BP_SW3W_CLK_PORT	GPIOB
#define BP_SW3W_MISO_PIN	GPIO14
#define BP_SW3W_MOSI_PIN	GPIO15
#define BP_SW3W_CS_PIN		GPIO12
#define BP_SW3W_CLK_PIN		GPIO13

// SW 2WIRE (SW2W)
#define BP_SW2W_SDA_PORT	GPIOB
#define BP_SW2W_CLK_PORT	GPIOB
#define BP_SW2W_SDA_PIN		GPIO7
#define BP_SW2W_CLK_PIN		GPIO6

// DIO
#define BP_DIO_DIO0_PORT	GPIOB
#define BP_DIO_DIO0_PIN		GPIO12
#define BP_DIO_DIO1_PORT	GPIOB
#define BP_DIO_DIO1_PIN		GPIO14
#define BP_DIO_DIO2_PORT	GPIOB
#define BP_DIO_DIO2_PIN		GPIO13
#define BP_DIO_DIO3_PORT	GPIOB
#define BP_DIO_DIO3_PIN		GPIO15

// 1wire
#define BP_1WIRE_PORT		GPIOB
#define BP_1WIRE_PIN		GPIO15
#define BP_1WIRE_SENSE_PORT	GPIOB
#define BP_1WIRE_SENSE_PIN	GPIO7


// voltage regulators and pullupvoltages
#define BP_PSUEN_PORT		GPIOB
#define BP_PSUEN_PIN		GPIO9
#define BP_VPUEN_PORT		GPIOC
#define BP_VPUEN_PIN		GPIO6
#define BP_VPU33EN_PORT		GPIOC		// TODO more descriptive name
#define BP_VPU33EN_PIN		GPIO14		// TODO more descriptive name
#define BP_VPU50EN_PORT		GPIOC		// TODO more descriptive name
#define BP_VPU50EN_PIN		GPIO15		// TODO more descriptive name

// LED
#define BP_USB_LED_PORT		GPIOC
#define BP_USB_LED_PIN		GPIO9
#define BP_MODE_LED_PORT	GPIOC
#define BP_MODE_LED_PIN		GPIO8

// delay
#define BP_DELAYTIMER		TIM4
#define BP_DELAYTIMER_CLOCK	RCC_TIM4

// USB PULLUP
//#define BP_CONTROLS_PU	
//#define BP_USB_PULLUP_PORT	GPIOC
//#define BP_USB_PULLUP_PIN	GPIO10





