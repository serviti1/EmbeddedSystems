#ifndef AUDIO_H
#define AUDIO_H

#include "board.h"
#include "chip.h"

#define WM8904_TWIHS TWIHS0
#define WM8904_TWIHS_ID ID_TWIHS0

#define PINS_TWIHS0 \
        {PIO_PA4A_TWCK0 | PIO_PA3A_TWD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

#define PINS_TK_TF  \
        {PIO_PB1D_TK | PIO_PB0D_TF, PIOB, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT}

#define PINS_TD_RF  \
        {PIO_PD26B_TD | PIO_PD24B_RF, PIOD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT}

#define PINS_RK_FLAG \
        {PIO_PA22A_RK, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

#define PINS_PCK2 \
        {PIO_PA18B_PCK2, PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}

#define XDMA_CH_SSC_RX    0
#define XDMA_CH_SSC_TX    1

#define XDMAC_UBC_NVIEW_NDV1 (0x1u << 27)
#define XDMAC_UBC_NDE_FETCH_EN   (0x1u << 24)
#define XDMAC_UBC_NSEN_UPDATED   (0x1u << 25)

/** TWI Bus Clock 400kHz */
#define WM8904_TWIHS_CLK 400000
#define TOTAL_BUFFERS (2) 
#define MICROBLOCK_LEN 0x1000 
#define BITS_BY_SLOT 16

uint32_t init_audio_codec(uint16_t* Buffer);

extern void SSC_Configure(Ssc *ssc, uint32_t bitRate, uint32_t masterClock);
extern void SSC_ConfigureTransmitter(Ssc *ssc, uint32_t tcmr, uint32_t tfmr);
extern void SSC_ConfigureReceiver(Ssc *ssc, uint32_t rcmr, uint32_t rfmr);
extern void SSC_EnableTransmitter(Ssc *ssc);
extern void SSC_DisableTransmitter(Ssc *ssc);
extern void SSC_EnableReceiver(Ssc *ssc);


#endif