/*
 * scicomm.h
 *
 *  Created on: 13 de jun de 2025
 *      Author: Guilherme Márcio Soares
 */

#ifndef SRC_SCICOMM_H_
#define SRC_SCICOMM_H_

#include <stdint.h>

#define INT_SIZE 2U
#define NUM_DAC 200U
#define NUM_ADC 100U
#define PROTOCOL_HEADER_SIZE 3U
#define FREQ_FUNDAMENTAL 50
#define XTAL_FREQ  10000000
typedef enum
{
    CMD_NONE = 0,
    CMD_RECEIVE_INT,
    CMD_SEND_INT,
    CMD_RECEIVE_SEN,
    CMD_SEND_SEN,
    CMD_SET_FREQ,

    CMD_COUNT

} SCI_Command_e;

typedef struct
{
    SCI_Command_e cmd;
    uint16_t data_len;
} Protocol_Header_t;

// Funções de protocolo SCI
int  protocolReceiveInt(unsigned int sci_base);
void protocolSendInt(unsigned int sci_base, int data);
void protocolReceiveSenoidDAC(uint32_t sci_base); // recebe 200 amostras via SCI
void protocolSendSenoid(uint32_t sci_base, volatile int16_t *dados); // envia 100 amostras via SCI

#endif /* SRC_SCICOMM_H_ */







