/*
 * scicomm.c
 *
 * Comunicação SCI com protocolo simples para int16 e buffers de onda
 */

#include "board.h"
#include "device.h"
#include "scicomm.h"

// Buffers globais definidos no main
extern volatile int16_t g_senoideDAC[NUM_DAC];
extern volatile int16_t g_senoideADC[NUM_ADC];

//
// Recebe um int16_t do PC via SCI (2 bytes little-endian)
//
int protocolReceiveInt(unsigned int sci_base)
{
    uint16_t buffer[INT_SIZE];
    SCI_readCharArray(sci_base, buffer, INT_SIZE);
    return (int16_t)(buffer[0] | (buffer[1] << 8U));
}

//
// Envia um int16_t para o PC via SCI (2 bytes little-endian)
//
void protocolSendInt(unsigned int sci_base, int data)
{
    uint16_t txBuf[INT_SIZE];
    txBuf[0] = (uint16_t)(data & 0x00FF);
    txBuf[1] = (uint16_t)((data >> 8U) & 0x00FF);

    SCI_writeCharArray(sci_base, txBuf, INT_SIZE);
}

//
// Recebe 200 amostras da senoide do PC (DAC) via SCI
//
void protocolReceiveSenoidDAC(uint32_t sci_base)
{
    uint16_t buffer[NUM_DAC * 2]; // Cada int16_t = 2 bytes
    SCI_readCharArray(sci_base, buffer, NUM_DAC * 2);

    for (int i = 0; i < NUM_DAC; i++) {
        g_senoideDAC[i] = (int16_t)(buffer[2 * i] | (buffer[2 * i + 1] << 8));
    }
}

//
// Envia 100 amostras (ADC) para o PC via SCI
//
void protocolSendSenoid(uint32_t sci_base, volatile int16_t *dados)
{
    uint16_t tx_buffer[NUM_ADC * 2];

    for (int i = 0; i < NUM_ADC; i++) {
        tx_buffer[2 * i]     = (uint16_t)(dados[i] & 0x00FF);        // LSB
        tx_buffer[2 * i + 1] = (uint16_t)((dados[i] >> 8U) & 0x00FF); // MSB
    }

    SCI_writeCharArray(sci_base, tx_buffer, NUM_ADC * 2);
}





