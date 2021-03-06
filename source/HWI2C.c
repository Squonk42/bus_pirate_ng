//TODO: add timeout to all I2C stuff that can hang!
#include <stdint.h>
#include "buspirateNG.h"
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "HWI2C.h"
#include "cdcacm.h"
#include "UI.h"
#include "sniffer.h"

static uint8_t	speed;

static uint8_t checkshort(void);
static void I2Csearch(void);


static uint16_t LA_period[2]={
    LA_I2C_PERIOD_100KHZ,
    LA_I2C_PERIOD_400KHZ
};

void HWI2C_start(void)
{
	uint8_t timeout;

	if(checkshort())
	{
		cdcprintf("no pullup or short");
		modeConfig.error=1;
		return;
	}

	cdcprintf("I2C START");
	i2c_send_start(BP_I2C);

	timeout=100; // 1 us enough?

	// wait for start (SB), switched to master (MSL) and a taken bus (BUSY)
	while ((!((I2C_SR1(BP_I2C)&I2C_SR1_SB)&&(I2C_SR2(BP_I2C)&(I2C_SR2_MSL|I2C_SR2_BUSY))))&&timeout) { timeout--; delayus(1); }

	if(timeout==0)
	{
		cdcprintf(" TIMEOUT");
		modeConfig.error=1;
	}
}

void HWI2C_stop(void)
{
	uint8_t timeout;

	if(!(I2C_SR2(BP_I2C)&I2C_SR2_TRA))
	{
		cdcprintf("!!WARNING: two extra bytes read!!");

	//	i2c_nack_current(BP_I2C);
		i2c_disable_ack(BP_I2C);

		timeout=100; // 100 us enough?

		// two bytes are buffered :( 
		while ((!(I2C_SR1(BP_I2C) & I2C_SR1_RxNE))&&timeout) { timeout--; delayus(1); }		// wait until data available
		(void)i2c_get_data(BP_I2C);

		if(timeout==0) cdcprintf(" TIMEOUT");

		timeout=100;

		while ((!(I2C_SR1(BP_I2C) & I2C_SR1_RxNE))&&timeout) { timeout--; delayus(1); }		// wait until data available
		(void)i2c_get_data(BP_I2C);

		if(timeout==0) cdcprintf(" TIMEOUT");

		cdcprintf("\r\n");
	}

	cdcprintf("I2C STOP");
	i2c_send_stop(BP_I2C);

}

uint32_t HWI2C_send(uint32_t d)
{
	uint8_t ack, timeout;
	uint32_t temp;

//	HWI2C_printI2Cflags();

	if(I2C_SR2(BP_I2C)&I2C_SR2_MSL)					// we can only send if master! (please issue a start condition frist)
	{
		if((I2C_SR1(BP_I2C)&I2C_SR1_SB)||(I2C_SR2(BP_I2C)&I2C_SR2_TRA))	// writing is only enable after start or during transmisson
		{
			temp=(I2C_SR1(BP_I2C)&I2C_SR1_SB); 		// gets destroyed by writing?

			i2c_send_data(BP_I2C, d);

//			HWI2C_printI2Cflags();

			timeout=100;	// 100us enough?

			if (temp) while(((!(I2C_SR1(BP_I2C)&I2C_SR1_ADDR))&&(!(I2C_SR1(BP_I2C)&I2C_SR1_AF)))&&timeout) { timeout--; delayus(1); } // or BTF??
			else while(((!(I2C_SR1(BP_I2C)&I2C_SR1_BTF))&&(!(I2C_SR1(BP_I2C)&I2C_SR1_AF)))&&timeout) { timeout--; delayus(1); }

//			HWI2C_printI2Cflags();

//			if(I2C_SR2(BP_I2C)&I2C_SR2_MSL)			// are we mastah??
			ack=!(I2C_SR1(BP_I2C)&I2C_SR1_AF);		// no ack error is ack
//			else
//				ack=0;		
	
			temp=I2C_SR1(BP_I2C);				// need to read both status registers??
			temp=I2C_SR2(BP_I2C);
			I2C_SR1(BP_I2C)=0;				// clear all errors/flags
			I2C_SR2(BP_I2C)=0;				// clear all errors/flags

			cdcprintf(" %s", (ack?"ACK":"NACK"));
		}
		else
		{
			cdcprintf("Not allowed to send (wrong address)");
			modeConfig.error=1;
			ack=0;
		}
	}
	else
	{
		cdcprintf("Not allowed to send (START)");
		modeConfig.error=1;
		ack=0;
	}

	return (!ack);
}

uint32_t HWI2C_read(void)
{
	uint32_t returnval;
	uint8_t timeout;

	if(!(I2C_SR2(BP_I2C)&I2C_SR2_TRA))
	{
		i2c_enable_ack(BP_I2C);				// TODO: clever way to nack last byte as per spec

		timeout=100; // 100us enough?

		while ((!(I2C_SR1(BP_I2C) & I2C_SR1_RxNE))&&timeout) { timeout--; delayus(1); };		// wait until data available

		if(timeout==0) cdcprintf(" TIMEOUT");

		returnval=i2c_get_data(BP_I2C);
	}
	else
	{
		cdcprintf("Not allowed to read (wrong address)");
		modeConfig.error=1;
		returnval=0;
	}

	return returnval;
}

void HWI2C_macro(uint32_t macro)
{
	switch(macro)
	{
		case 0:		cdcprintf(" 1. I2C Address search\r\n");
				cdcprintf(" 2. I2C sniffer\r\n");
				break;
		case 1:		I2Csearch();
				break;
		case 2:		cdcprintf("press any key to exit\r\n");
				HWI2C_cleanup();
				sniffI2C();
				HWI2C_setup_exc();
				break;

				break;
		default:	cdcprintf("Macro not defined");
				modeConfig.error=1;
	}
}

void HWI2C_setup(void)
{
	// speed
	if(cmdtail!=cmdhead) cmdtail=(cmdtail+1)&(CMDBUFFSIZE-1);
	consumewhitechars();
	speed=getint();
	if((speed>0)&&(speed<=2)) speed-=1;
	else modeConfig.error=1;

	// did the user did it right?
	if(modeConfig.error)			// go interactive 
	{
		speed=(askint(HWI2CSPEEDMENU, 1, 2, 1));
	}
}

void HWI2C_setup_exc(void)
{
	rcc_periph_clock_enable(BP_I2C_CLK);

	gpio_set_mode(BP_I2C_SDA_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, BP_I2C_SDA_PIN);
	gpio_set_mode(BP_I2C_CLK_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, BP_I2C_CLK_PIN);

	// i2c needs to be disabled before it can be programmed
	i2c_peripheral_disable(BP_I2C);
	i2c_reset(BP_I2C);

	// setup timing 
	// speed=0 100KHz
	// speed=1 400KHz
	i2c_set_speed(BP_I2C, speed,I2C_CR2_FREQ_36MHZ);	// 36MHz is max

	//?
	//i2c_set_own_7bit_slave_address(I2C2, sp,0x32); // do we want this?

	// go!
	i2c_peripheral_enable(BP_I2C);

	// update modeConfig pins
	modeConfig.mosiport=BP_I2C_SDA_PORT;
	modeConfig.clkport=BP_I2C_SDA_PORT;
	modeConfig.mosipin=BP_I2C_SDA_PIN;
	modeConfig.clkpin=BP_I2C_SDA_PIN;

	modeConfig.logicanalyzerperiod=LA_period[speed];
}

void HWI2C_cleanup(void)
{
	rcc_periph_clock_disable(RCC_I2C2);
	i2c_peripheral_disable(I2C2);
	gpio_set_mode(BP_I2C_SDA_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, BP_I2C_SDA_PIN);
	gpio_set_mode(BP_I2C_CLK_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, BP_I2C_CLK_PIN);

	// update modeConfig pins
	modeConfig.misoport=0;
	modeConfig.mosiport=0;
	modeConfig.csport=0;
	modeConfig.clkport=0;
	modeConfig.misopin=0;
	modeConfig.mosipin=0;
	modeConfig.cspin=0;
	modeConfig.clkpin=0;

}

void HWI2C_pins(void)
{
	cdcprintf("-\t-\tSCL\tSDA");
}

void HWI2C_settings(void)
{
	cdcprintf("HWI2C (speed)=(%d)", speed);
}

void HWI2C_printI2Cflags(void)
{
	uint32_t temp;

	temp=I2C_SR1(BP_I2C);

	if(temp&I2C_SR1_SMBALERT) cdcprintf(" SMBALERT");
	if(temp&I2C_SR1_TIMEOUT) cdcprintf(" TIMEOUT");
	if(temp&I2C_SR1_PECERR) cdcprintf(" PECERR");
	if(temp&I2C_SR1_OVR) cdcprintf(" OVR");
	if(temp&I2C_SR1_AF) cdcprintf(" AF");
	if(temp&I2C_SR1_ARLO) cdcprintf(" ARLO");
	if(temp&I2C_SR1_BERR) cdcprintf(" BERR");
	if(temp&I2C_SR1_TxE) cdcprintf(" TxE");
	if(temp&I2C_SR1_RxNE) cdcprintf(" RxNE");
	if(temp&I2C_SR1_STOPF) cdcprintf(" STOPF");
	if(temp&I2C_SR1_ADD10) cdcprintf(" ADD10");
	if(temp&I2C_SR1_BTF) cdcprintf(" BTF");
	if(temp&I2C_SR1_ADDR) cdcprintf(" ADDR");
	if(temp&I2C_SR1_SB) cdcprintf(" SB");

	temp=I2C_SR2(BP_I2C);

	if(temp&I2C_SR2_DUALF) cdcprintf(" DUALF");
	if(temp&I2C_SR2_SMBHOST) cdcprintf(" SMBHOST");
	if(temp&I2C_SR2_SMBDEFAULT) cdcprintf(" SMBDEFAULT");
	if(temp&I2C_SR2_GENCALL) cdcprintf(" GENCALL");
	if(temp&I2C_SR2_TRA) cdcprintf(" TRA");
	if(temp&I2C_SR2_BUSY) cdcprintf(" BUSY");
	if(temp&I2C_SR2_MSL) cdcprintf(" MSL");
}

void HWI2C_help(void)
{
	cdcprintf("Muli-Master-multi-slave 2 wire protocol using a CLOCK and an bidirectional DATA\r\n");
	cdcprintf("line in opendrain configuration. Standard clock frequencies are 100KHz, 400KHz\r\n");
	cdcprintf("and 1MHz.\r\n");
	cdcprintf("\r\n");
	cdcprintf("More info: https://en.wikipedia.org/wiki/I2C\r\n");
	cdcprintf("\r\n");
	cdcprintf("Electrical:\r\n");
	cdcprintf("\r\n");
	cdcprintf("BPCMD\t   { |            ADDRES(7bits+R/!W bit)             |\r\n");
	cdcprintf("CMD\tSTART| A6  | A5  | A4  | A3  | A2  | A1  | A0  | R/!W| ACK* \r\n");
	cdcprintf("\t-----|-----|-----|-----|-----|-----|-----|-----|-----|-----\r\n");
	cdcprintf("SDA\t\"\"___|_###_|_###_|_###_|_###_|_###_|_###_|_###_|_###_|_###_ ..\r\n");
	cdcprintf("SCL\t\"\"\"\"\"|__\"__|__\"__|__\"__|__\"__|__\"__|__\"__|__\"__|__\"__|__\"__ ..\r\n");
	cdcprintf("\r\n");
	cdcprintf("BPCMD\t   |                      DATA (8bit)              |     |  ]  |\r\n");
	cdcprintf("CMD\t.. | D7  | D6  | D5  | D4  | D3  | D2  | D1  | D0  | ACK*| STOP|  \r\n");
	cdcprintf("\t  -|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|\r\n");
	cdcprintf("SDA\t.. |_###_|_###_|_###_|_###_|_###_|_###_|_###_|_###_|_###_|___\"\"|\r\n");
	cdcprintf("SCL\t.. |__\"__|__\"__|__\"__|__\"__|__\"__|__\"__|__\"__|__\"__|__\"__|\"\"\"\"\"|\r\n");
	cdcprintf("\r\n");
	cdcprintf("* Receiver needs to pull SDA down when address/byte is received correctly\r\n");
	cdcprintf("\r\n");
	cdcprintf("Connection:\r\n");
	cdcprintf("\t\t  +--[4k7]---+--- +3V3 or +5V0\r\n");
	cdcprintf("\t\t  | +-[4k7]--|\r\n");
	cdcprintf("\t\t  | |\r\n");
	cdcprintf("\tSDA \t--+-|------------- SDA\r\n");
	cdcprintf("{BP}\tSCL\t----+------------- SCL  {DUT}\r\n");
	cdcprintf("\tGND\t------------------ GND\r\n");			
}


static uint8_t checkshort(void)
{
	uint8_t temp;

	temp=(gpio_get(BP_I2C_SDA_SENSE_PORT, BP_I2C_SDA_SENSE_PIN)==0?1:0);
	temp|=(gpio_get(BP_I2C_SCL_SENSE_PORT, BP_I2C_SCL_SENSE_PIN)==0?2:0);

	return (temp==3);			// there is only a short when both are 0 otherwise repeated start wont work
}

static void I2Csearch(void)
{
	int i=0;
	uint8_t timeout;
	uint16_t ack;

	if(checkshort())
	{
		cdcprintf("no pullup or short");
		modeConfig.error=1;
		return;
	}

	cdcprintf("Found:\r\n");

	for(i=0; i<256; i++)
	{
		i2c_send_start(BP_I2C);

		timeout=100; // 1 us enough?

		// wait for start (SB), switched to master (MSL) and a taken bus (BUSY)
		while ((!((I2C_SR1(BP_I2C)&I2C_SR1_SB)&&(I2C_SR2(BP_I2C)&(I2C_SR2_MSL|I2C_SR2_BUSY))))&&timeout) { timeout--; delayus(1); }

		if(timeout==0)
		{
				
		}

		i2c_send_data(BP_I2C, i);

		timeout=100;	// 100us enough?

		while(((!(I2C_SR1(BP_I2C)&I2C_SR1_ADDR))&&(!(I2C_SR1(BP_I2C)&I2C_SR1_AF)))&&timeout) { timeout--; delayus(1); } // or BTF??

		ack=!(I2C_SR1(BP_I2C)&I2C_SR1_AF);

		if(ack) cdcprintf("0x%02X(%c) ", i, ((i&0x1)?'R':'W'));

		ack=I2C_SR1(BP_I2C);				// need to read both status registers??
		ack=I2C_SR2(BP_I2C);
		I2C_SR1(BP_I2C)=0;				// clear all errors/flags
		I2C_SR2(BP_I2C)=0;				// clear all errors/flags

		i2c_send_stop(BP_I2C);
	}
}


