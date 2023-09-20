/*************************************************************************/
/********************** SWC    : SSI_program        **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 22, 2021       **********************/
/*************************************************************************/

#include "../../LIB/STD_TYPES/STD_TYPES.h"
#include "../../LIB/BIT_MATH/BIT_MATH.h"
#include "../../LIB/TIVAC123GH6PM/TIVAC123GH6PM.h"

#include "SSI_interface.h"
#include "SSI_config.h"
#include "SSI_private.h"

static SSI_REGISTER_t * SSIx[4] = {SSI0, SSI1, SSI2, SSI3};

u8 SSI_u8SetMasterConfigurations(const SSI_MasterConfigs_t * Copy_structSSIConfigurations)
{
    u8 LOC_u8ErrorState = STD_OK;
    if (Copy_structSSIConfigurations != NULL)
    {
        /* Disable the SSI   */
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CR1 &=~(1 << SSE);
        /* Choosing Master/Slave */
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CR1 |= Copy_structSSIConfigurations->SSI_M_S_Select;
        /* Selecting Clock Source */
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CC |= Copy_structSSIConfigurations->SSI_CLK;
        /* Setting the Pre-scaler for master */
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CPSR = Copy_structSSIConfigurations->SSI_ClockPrescaleDivisor | (Copy_structSSIConfigurations->SSI_SeriealClockRate << 8);
        /*Setting the SSI  Data Length and Frame Type*/
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CR0 |= Copy_structSSIConfigurations->SSI_Frame | Copy_structSSIConfigurations->SSI_Data;
        /* Configuring the SPO and SPH */
        //SSIx[Copy_structSSIConfigurations->SSI_ID]->CR0 |=
        /* Enable the SSI   */
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CR1 |=(1 << SSE);
    }
    else
    {
        LOC_u8ErrorState = STD_NOK;
    }
    return LOC_u8ErrorState;
}

u8 SSI_u8SetSlaveConfigurations(const SSI_SlaveConfigs_t * Copy_structSSIConfigurations)
{
    u8 LOC_u8ErrorState = STD_OK;
    if (Copy_structSSIConfigurations != NULL)
    {
        /* Disable the SSI   */
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CR1 &=~(1 << SSE);
        /* Choosing Master/Slave */
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CR1 |= Copy_structSSIConfigurations->SSI_M_S_Select;
        /*Setting the SSI  Data Length and Frame Type*/
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CR0 |= Copy_structSSIConfigurations->SSI_Frame | Copy_structSSIConfigurations->SSI_Data;
        /* Configuring the SPO and SPH */
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CR0 |= ( Copy_structSSIConfigurations->SSI_CLKPH << SPH )| ( Copy_structSSIConfigurations->SSI_CLKPOL << SPOL);
        /* Enable the SSI   */
        SSIx[Copy_structSSIConfigurations->SSI_ID]->CR1 |=(1 << SSE);
    }
    else
    {
        LOC_u8ErrorState = STD_NOK;
    }
    return LOC_u8ErrorState;

}

void SSI_voidTransmit(SSI_ChannelID_t Copy_enumChannelID, u16 Copy_u16DataTransmit)
{
    while( GET_BIT(SSIx[Copy_enumChannelID]->SR , TNF) == 0); // wait until FIFO is not Full
    SSIx[Copy_enumChannelID]->DR = Copy_u16DataTransmit;
    while( GET_BIT(SSIx[Copy_enumChannelID]->SR , BSY) == 1); // wait until SSI finished last operation
}

u8 SSI_voidReceive(SSI_ChannelID_t Copy_enumChannelID, u16 * Copy_ptrDataReceivedBuffer)
{
    u8 Local_u8ErrorState = STD_OK;
    if( Copy_ptrDataReceivedBuffer != NULL )
    {
        while( GET_BIT(SSIx[Copy_enumChannelID]->SR , BSY) == 1); // wait until SSI finished last operation
        *Copy_ptrDataReceivedBuffer = SSIx[Copy_enumChannelID]->DR ;
    }
    else
    {
        Local_u8ErrorState = STD_NOK;
    }
    return Local_u8ErrorState;
}
