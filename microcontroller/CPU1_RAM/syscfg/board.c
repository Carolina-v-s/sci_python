/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "board.h"

//*****************************************************************************
//
// Board Configurations
// Initializes the rest of the modules. 
// Call this function in your application if you wish to do all module 
// initialization.
// If you wish to not use some of the initializations, instead of the 
// Board_init use the individual Module_inits
//
//*****************************************************************************
void Board_init()
{
	EALLOW;

	PinMux_init();
	ADC_init();
	CPUTIMER_init();
	DAC_init();
	SCI_init();
	INTERRUPT_init();

	EDIS;
}

//*****************************************************************************
//
// PINMUX Configurations
//
//*****************************************************************************
void PinMux_init()
{
	//
	// PinMux for modules assigned to CPU1
	//
	
	//
	// SCIA -> SCI0 Pinmux
	//
	GPIO_setPinConfig(SCI0_SCIRX_PIN_CONFIG);
	GPIO_setPadConfig(SCI0_SCIRX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(SCI0_SCIRX_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SCI0_SCITX_PIN_CONFIG);
	GPIO_setPadConfig(SCI0_SCITX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(SCI0_SCITX_GPIO, GPIO_QUAL_ASYNC);


}

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
void ADC_init(){
	ADC0_init();
}

void ADC0_init(){
	//
	// Configures the analog-to-digital converter module prescaler.
	//
	ADC_setPrescaler(ADC0_BASE, ADC_CLK_DIV_4_0);
	//
	// Configures the analog-to-digital converter resolution and signal mode.
	//
	ADC_setMode(ADC0_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
	//
	// Sets the timing of the end-of-conversion pulse
	//
	ADC_setInterruptPulseMode(ADC0_BASE, ADC_PULSE_END_OF_ACQ_WIN);
	//
	// Powers up the analog-to-digital converter core.
	//
	ADC_enableConverter(ADC0_BASE);
	//
	// Delay for 1ms to allow ADC time to power up
	//
	DEVICE_DELAY_US(500);
	//
	// SOC Configuration: Setup ADC EPWM channel and trigger settings
	//
	// Disables SOC burst mode.
	//
	ADC_disableBurstMode(ADC0_BASE);
	//
	// Sets the priority mode of the SOCs.
	//
	ADC_setSOCPriority(ADC0_BASE, ADC_PRI_ALL_ROUND_ROBIN);
	//
	// Start of Conversion 0 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 0
	//	  	Trigger			: ADC_TRIGGER_CPU1_TINT0
	//	  	Channel			: ADC_CH_ADCIN0
	//	 	Sample Window	: 15 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(ADC0_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_CPU1_TINT0, ADC_CH_ADCIN0, 15U);
	ADC_setInterruptSOCTrigger(ADC0_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
	//
	// ADC Interrupt 1 Configuration
	// 		Source	: ADC_SOC_NUMBER0
	// 		Interrupt Source: enabled
	// 		Continuous Mode	: disabled
	//
	//
	ADC_setInterruptSource(ADC0_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
	ADC_clearInterruptStatus(ADC0_BASE, ADC_INT_NUMBER1);
	ADC_disableContinuousMode(ADC0_BASE, ADC_INT_NUMBER1);
	ADC_enableInterrupt(ADC0_BASE, ADC_INT_NUMBER1);
}


//*****************************************************************************
//
// CPUTIMER Configurations
//
//*****************************************************************************
void CPUTIMER_init(){
	myCPUTIMER0_init();
	myCPUTIMER1_init();
}

void myCPUTIMER0_init(){
	CPUTimer_setEmulationMode(myCPUTIMER0_BASE, CPUTIMER_EMULATIONMODE_RUNFREE);
	CPUTimer_setPreScaler(myCPUTIMER0_BASE, 0U);
	CPUTimer_setPeriod(myCPUTIMER0_BASE, 39999U);
	CPUTimer_enableInterrupt(myCPUTIMER0_BASE);
	CPUTimer_stopTimer(myCPUTIMER0_BASE);

	CPUTimer_reloadTimerCounter(myCPUTIMER0_BASE);
	CPUTimer_startTimer(myCPUTIMER0_BASE);
}
void myCPUTIMER1_init(){
	CPUTimer_setEmulationMode(myCPUTIMER1_BASE, CPUTIMER_EMULATIONMODE_RUNFREE);
	CPUTimer_setPreScaler(myCPUTIMER1_BASE, 0U);
	CPUTimer_setPeriod(myCPUTIMER1_BASE, 19999U);
	CPUTimer_enableInterrupt(myCPUTIMER1_BASE);
	CPUTimer_stopTimer(myCPUTIMER1_BASE);

	CPUTimer_reloadTimerCounter(myCPUTIMER1_BASE);
	CPUTimer_startTimer(myCPUTIMER1_BASE);
}

//*****************************************************************************
//
// DAC Configurations
//
//*****************************************************************************
void DAC_init(){
	DAC0_init();
}

void DAC0_init(){
	//
	// Set DAC reference voltage.
	//
	DAC_setReferenceVoltage(DAC0_BASE, DAC_REF_ADC_VREFHI);
	//
	// Set DAC load mode.
	//
	DAC_setLoadMode(DAC0_BASE, DAC_LOAD_SYSCLK);
	//
	// Enable the DAC output
	//
	DAC_enableOutput(DAC0_BASE);
	//
	// Set the DAC shadow output
	//
	DAC_setShadowValue(DAC0_BASE, 0U);

	//
	// Delay for buffered DAC to power up.
	//
	DEVICE_DELAY_US(500);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Settings for INT_ADC0_1
	// ISR need to be defined for the registered interrupts
	Interrupt_register(INT_ADC0_1, &INT_ADC0_1_ISR);
	Interrupt_enable(INT_ADC0_1);
	
	// Interrupt Settings for INT_myCPUTIMER1
	// ISR need to be defined for the registered interrupts
	Interrupt_register(INT_myCPUTIMER1, &INT_myCPUTIMER1_ISR);
	Interrupt_enable(INT_myCPUTIMER1);
	
	// Interrupt Settings for INT_SCI0_RX
	// ISR need to be defined for the registered interrupts
	Interrupt_register(INT_SCI0_RX, &INT_SCI0_RX_ISR);
	Interrupt_enable(INT_SCI0_RX);
}
//*****************************************************************************
//
// SCI Configurations
//
//*****************************************************************************
void SCI_init(){
	SCI0_init();
}

void SCI0_init(){
	SCI_clearInterruptStatus(SCI0_BASE, SCI_INT_RXFF | SCI_INT_TXFF | SCI_INT_FE | SCI_INT_OE | SCI_INT_PE | SCI_INT_RXERR | SCI_INT_RXRDY_BRKDT | SCI_INT_TXRDY);
	SCI_clearOverflowStatus(SCI0_BASE);
	SCI_resetTxFIFO(SCI0_BASE);
	SCI_resetRxFIFO(SCI0_BASE);
	SCI_resetChannels(SCI0_BASE);
	SCI_setConfig(SCI0_BASE, DEVICE_LSPCLK_FREQ, SCI0_BAUDRATE, (SCI_CONFIG_WLEN_8|SCI_CONFIG_STOP_ONE|SCI_CONFIG_PAR_NONE));
	SCI_disableLoopback(SCI0_BASE);
	SCI_performSoftwareReset(SCI0_BASE);
	SCI_enableInterrupt(SCI0_BASE, SCI_INT_RXFF);
	SCI_setFIFOInterruptLevel(SCI0_BASE, SCI_FIFO_TX0, SCI_FIFO_RX1);
	SCI_enableFIFO(SCI0_BASE);
	SCI_enableModule(SCI0_BASE);
}

