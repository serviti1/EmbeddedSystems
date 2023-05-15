#include "audio.h"
#include "twid.h"
#include "twi.h"
#include "ssc.h"
#include "chip.h"
#include "board.h"
#include "xdmac.h"
#include "board.h"

Twid Audio_twid;

Twid *pAudio_twid = &Audio_twid;

const Pin twhis0Pins[] = PINS_TWIHS0;
const Pin ssc_tk_tf_pins[] = PINS_TK_TF;
const Pin ssc_td_rf_pins[] = PINS_TD_RF;
const Pin ssc_rk_pins[] = PINS_RK_FLAG;
const Pin ssc_pck2_pins[] = PINS_PCK2;

static LinkedListDescriporView1 linklist_write[TOTAL_BUFFERS];
static LinkedListDescriporView1 linklist_read[TOTAL_BUFFERS];

void xdmac_configure_transfer(Xdmac *xdmac,
							  uint32_t channel_num, sXdmadCfg *cfg)
{
	XDMAC_GetChannelIsr(xdmac, channel_num);
	XDMAC_SetSourceAddr(xdmac, channel_num, cfg->mbr_sa);
	XDMAC_SetDestinationAddr(xdmac, channel_num, cfg->mbr_da);
	XDMAC_SetMicroblockControl(xdmac, channel_num, cfg->mbr_ubc);
	XDMAC_SetBlockControl(xdmac, channel_num, cfg->mbr_bc);
	XDMAC_SetDataStride_MemPattern(xdmac, channel_num, cfg->mbr_ds);
	XDMAC_SetSourceMicroBlockStride(xdmac, channel_num, cfg->mbr_sus);
	XDMAC_SetDestinationMicroBlockStride(xdmac, channel_num, cfg->mbr_dus);
	XDMAC_SetChannelConfig(xdmac, channel_num, cfg->mbr_cfg);
}

/**
 *  \brief Configure DMA
 */
static void configure_xdma(uint16_t *Buffer)
{
	uint16_t *src;
	uint8_t i;

	sXdmadCfg xdmac_channel_cfg = {0};

	/* Initialize and enable DMA controller */
	PMC_EnablePeripheral(ID_XDMAC);

	xdmac_channel_cfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN | XDMAC_CC_MBSIZE_SINGLE
								//| XDMAC_CC_DSYNC_MEM2PER
								| XDMAC_CC_DSYNC_PER2MEM | XDMAC_CC_CSIZE_CHK_1 | XDMAC_CC_DWIDTH_HALFWORD | XDMAC_CC_SIF_AHB_IF0 | XDMAC_CC_DIF_AHB_IF1
								//| XDMAC_CC_SAM_INCREMENTED_AM
								//| XDMAC_CC_DAM_FIXED_AM
								| XDMAC_CC_SAM_FIXED_AM | XDMAC_CC_DAM_INCREMENTED_AM | XDMAC_CC_PERID(33);
	xdmac_configure_transfer(XDMAC, XDMA_CH_SSC_RX, &xdmac_channel_cfg);

	src = Buffer;
	for (i = 0; i < TOTAL_BUFFERS; i++)
	{
		linklist_read[i].mbr_ubc = XDMAC_UBC_NVIEW_NDV1 | XDMAC_UBC_NDE_FETCH_EN | XDMAC_UBC_NSEN_UPDATED | XDMAC_CUBC_UBLEN(MICROBLOCK_LEN);
		linklist_read[i].mbr_sa = (uint32_t) & (SSC->SSC_RHR);
		linklist_read[i].mbr_da = (uint32_t)(src);
		if (i == (TOTAL_BUFFERS - 1))
		{
			linklist_read[i].mbr_nda = (uint32_t)&linklist_read[0];
		}
		else
		{
			linklist_read[i].mbr_nda = (uint32_t)&linklist_read[i + 1];
		}
		src += MICROBLOCK_LEN;
	}

	XDMAC_SetDescriptorControl(XDMAC, XDMA_CH_SSC_RX,
							   XDMAC_CNDC_NDVIEW_NDV1 | XDMAC_CNDC_NDE_DSCR_FETCH_EN | XDMAC_CNDC_NDSUP_SRC_PARAMS_UPDATED | XDMAC_CNDC_NDDUP_DST_PARAMS_UPDATED);
	XDMAC_SetDescriptorAddr(XDMAC, XDMA_CH_SSC_RX, (uint32_t)(&linklist_read[0]), 0);

	XDMAC_EnableGIt(XDMAC, XDMA_CH_SSC_RX);
	XDMAC_EnableChannelIt(XDMAC, XDMA_CH_SSC_RX, XDMAC_CIE_LIE);

	xdmac_channel_cfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN | XDMAC_CC_MBSIZE_SINGLE | XDMAC_CC_DSYNC_MEM2PER | XDMAC_CC_CSIZE_CHK_1 | XDMAC_CC_DWIDTH_HALFWORD | XDMAC_CC_SIF_AHB_IF0 | XDMAC_CC_DIF_AHB_IF1 | XDMAC_CC_SAM_INCREMENTED_AM | XDMAC_CC_DAM_FIXED_AM | XDMAC_CC_PERID(32);
	xdmac_configure_transfer(XDMAC, XDMA_CH_SSC_TX, &xdmac_channel_cfg);

	src = Buffer;
	for (i = 0; i < TOTAL_BUFFERS; i++)
	{
		linklist_write[i].mbr_ubc = XDMAC_UBC_NVIEW_NDV1 | XDMAC_UBC_NDE_FETCH_EN | XDMAC_UBC_NSEN_UPDATED | XDMAC_CUBC_UBLEN(MICROBLOCK_LEN);
		linklist_write[i].mbr_sa = (uint32_t)(src);
		linklist_write[i].mbr_da = (uint32_t) & (SSC->SSC_THR);
		if (i == (TOTAL_BUFFERS - 1))
		{
			linklist_write[i].mbr_nda = (uint32_t)&linklist_write[0];
		}
		else
		{
			linklist_write[i].mbr_nda = (uint32_t)&linklist_write[i + 1];
		}
		src += MICROBLOCK_LEN;
	}

	XDMAC_SetDescriptorControl(XDMAC, XDMA_CH_SSC_TX, XDMAC_CNDC_NDVIEW_NDV1 | XDMAC_CNDC_NDE_DSCR_FETCH_EN | XDMAC_CNDC_NDSUP_SRC_PARAMS_UPDATED | XDMAC_CNDC_NDDUP_DST_PARAMS_UPDATED);
	XDMAC_SetDescriptorAddr(XDMAC, XDMA_CH_SSC_TX, (uint32_t)(&linklist_write[0]), 0);

	XDMAC_EnableGIt(XDMAC, XDMA_CH_SSC_TX);
	XDMAC_EnableChannelIt(XDMAC, XDMA_CH_SSC_TX, XDMAC_CIE_LIE);

	/*Enable XDMA interrupt */
	NVIC_ClearPendingIRQ(XDMAC_IRQn);
	NVIC_SetPriority(XDMAC_IRQn, 1);
	NVIC_EnableIRQ(XDMAC_IRQn);
}

uint32_t init_audio(uint16_t *Buffer)
{
	/* Enable required peripherals */
	PMC_EnablePeripheral(WM8904_TWIHS_ID);
	PMC_EnablePeripheral(ID_SSC);

	PIO_Configure(twhis0Pins, PIO_LISTSIZE(twhis0Pins));
	PIO_Configure(ssc_tk_tf_pins, PIO_LISTSIZE(ssc_tk_tf_pins));
	PIO_Configure(ssc_td_rf_pins, PIO_LISTSIZE(ssc_td_rf_pins));
	PIO_Configure(ssc_rk_pins, PIO_LISTSIZE(ssc_rk_pins));
	PIO_Configure(ssc_pck2_pins, PIO_LISTSIZE(ssc_pck2_pins));

	/* Configure Master TWIHS0 */
	TWI_ConfigureMaster(WM8904_TWIHS, WM8904_TWIHS_CLK, BOARD_MCK);

	/* Initialize TWID to be used */
	TWID_Initialize(pAudio_twid, WM8904_TWIHS);

	WM8904_Init(pAudio_twid, WM8904_SLAVE_ADDRESS, PMC_MCKR_CSS_SLOW_CLK);

	SSC_Configure(SSC, 0, BOARD_MCK);

	SSC_ConfigureTransmitter(SSC,
							 (SSC_TCMR_CKS_TK | SSC_TCMR_CKO_NONE | SSC_TCMR_CKG_CONTINUOUS | SSC_TCMR_START_TF_EDGE | SSC_TCMR_PERIOD(0) | SSC_TCMR_STTDLY(1)),
							 (SSC_TFMR_DATLEN(15) | SSC_TFMR_MSBF | SSC_TFMR_FSLEN(15) | SSC_TFMR_FSOS_NONE | SSC_TFMR_FSEDGE_POSITIVE));

	SSC_ConfigureReceiver(SSC,
						  (SSC_RCMR_CKS_TK | SSC_RCMR_CKO_NONE | SSC_RCMR_CKI | SSC_RCMR_CKG_CONTINUOUS | SSC_RCMR_START_RF_EDGE | SSC_RCMR_STTDLY(1) | SSC_RCMR_PERIOD(0)),
						  (SSC_RFMR_DATLEN(15) | SSC_RFMR_MSBF | SSC_RFMR_FSLEN(15) | SSC_RFMR_FSOS_NONE | SSC_RFMR_FSEDGE_POSITIVE));

	SSC_DisableReceiver(SSC);

	SSC_DisableTransmitter(SSC);
	SSC_DisableInterrupts(SSC, 0xFFFFFFF);

	configure_xdma(Buffer);

	PMC_ConfigurePCK2(PMC_MCKR_CSS_SLOW_CLK, 0);

	SSC_EnableReceiver(SSC);

	XDMAC_EnableChannel(XDMAC, XDMA_CH_SSC_RX);
	Wait(300);

	SSC_EnableTransmitter(SSC);
	XDMAC_EnableChannel(XDMAC, XDMA_CH_SSC_TX);
}