//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "scicomm.h"
#include "math.h"


// Variáveis globais

volatile int num_amostras_adc = NUM_ADC;
volatile float gain = 1.0f;

int g_senoideDAC[NUM_DAC];
int g_senoideADC[NUM_ADC];
volatile bool adc_buffer_cheio = false;
volatile Protocol_Header_t g_prot_header = {CMD_NONE, 0};
uint32_t clk = 20000000; // Valor padrão do microcontrolador

//
// Função Principal
//
void main(void)
{
    Device_init();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    Board_init();  // Inicialização feita via syscfg

    EINT;
    ERTM;

    while (1)
    {
        if (g_prot_header.cmd != CMD_NONE)
        {
            switch (g_prot_header.cmd)
            {
                case CMD_RECEIVE_INT:
                    // Recebe número de amostras por ciclo
                    num_amostras_adc = protocolReceiveInt(SCI0_BASE);
                    clk = SysCtl_getClock(XTAL_FREQ);
                    CPUTimer_setPeriod(CPUTIMER0_BASE, clk/(FREQ_FUNDAMENTAL*num_amostras_adc)-1);

                    break;

                case CMD_SEND_INT:
                    protocolSendInt(SCI0_BASE, num_amostras_adc);
                    break;

                case CMD_RECEIVE_SEN:
                    protocolReceiveSenoidDAC(SCI0_BASE);  // Recebe senoide do PC
                    break;

                case CMD_SEND_SEN:
                    protocolSendSenoid(SCI0_BASE, g_senoideADC);  // Envia ADC para PC
                    break;

                default:
                    break;
            }

            SCI_clearInterruptStatus(SCI0_BASE, SCI_INT_RXFF);
            g_prot_header.cmd = CMD_NONE;
        }
    }
}

//
// Interrupção de recepção SCI
//
__interrupt void INT_SCI0_RX_ISR(void)
{
    uint16_t header[PROTOCOL_HEADER_SIZE];
    uint16_t cmd;

    SCI_readCharArray(SCI0_BASE, header, PROTOCOL_HEADER_SIZE);
    cmd = header[0];
    g_prot_header.data_len = header[1] | (header[2] << 8);
    g_prot_header.cmd = (cmd < CMD_COUNT) ? (SCI_Command_e)cmd : CMD_NONE;

    Interrupt_clearACKGroup(INT_SCI0_RX_INTERRUPT_ACK_GROUP);
}

//
// Interrupção do ADC – Armazena valores no buffer
//
__interrupt void INT_ADC0_1_ISR(void)
{
    static uint16_t cnt_adc = 0;

    g_senoideADC[cnt_adc] = ADC_readResult(ADC0_RESULT_BASE, ADC_SOC_NUMBER0);
    cnt_adc++;

    if (cnt_adc >= NUM_ADC)
    {
        cnt_adc = 0;
        adc_buffer_cheio = true;
    }

    ADC_clearInterruptStatus(ADC0_BASE, ADC_INT_NUMBER1);
    Interrupt_clearACKGroup(INT_ADC0_1_INTERRUPT_ACK_GROUP);
}

//
// Interrupção do Timer1 – Atualiza DAC
//
__interrupt void INT_myCPUTIMER1_ISR(void)
{
    static uint16_t cnt_dac = 0;

    DAC_setShadowValue(DAC0_BASE, (uint16_t)(gain * g_senoideDAC[cnt_dac]));
    cnt_dac = (cnt_dac + 1) % NUM_DAC;
}
