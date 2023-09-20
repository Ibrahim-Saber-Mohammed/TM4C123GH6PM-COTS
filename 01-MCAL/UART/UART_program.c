/*************************************************************************/
/********************** SWC    : UART_program       **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 15, 2021       **********************/
/*************************************************************************/

/* Include the libraries */
#include "../../LIB/BIT_MATH/BIT_MATH.h"
#include "../../LIB/STD_TYPES/STD_TYPES.h"
#include "../../LIB/TIVAC123GH6PM/TIVAC123GH6PM.h"

/* Include Peripheral files */
#include "UART_interface.h"
#include "UART_config.h"
#include "UART_private.h"

/* Array of pointers to structure */
static UART_REGISTER_t*UARTx[8] = { UART0, UART1, UART2, UART3, UART4, UART5, UART6, UART7 };

/* the call back functions that called inside the ISR (Implemented in the main Application) */
static void (*UART_TX_CALLBACK[8])(void) = {NULL};
static void (*UART_RX_CALLBACK[8])(u8 Copy_u8ReeivedChar) = {NULL};

u8 UART_u8SetUartConfigurations(const UART_Configs_t * Copy_structUartConfigs)
{
    u8 LOC_ErrorState = STD_OK;
    u8 LOC_IntegerBaudRate = 0;
    u8 LOC_FractionBaudRate = 0;
    if( Copy_structUartConfigs != NULL )
    {
        UARTx[Copy_structUartConfigs->UART_ChannelID]->CTL  = 0;
        UARTx[Copy_structUartConfigs->UART_ChannelID]->LCRH = 0;
        UARTx[Copy_structUartConfigs->UART_ChannelID]->CC   = 0;
        UARTx[Copy_structUartConfigs->UART_ChannelID]->IM   = 0;

        // Disable the UART
        CLR_BIT(UARTx[Copy_structUartConfigs->UART_ChannelID]->CTL , UART_EN);
        /****************************************************************************************************/
        /* Calculate the BaudRate integer and the Fraction */
        LOC_IntegerBaudRate = ( SYSCLK )/( 16 * Copy_structUartConfigs->UART_BaudRate );
        LOC_FractionBaudRate = ( ( SYSCLK )%( 16 * Copy_structUartConfigs->UART_BaudRate )*64 ) + 0.5;
        UARTx[Copy_structUartConfigs->UART_ChannelID]->IBRD = LOC_IntegerBaudRate;
        UARTx[Copy_structUartConfigs->UART_ChannelID]->FBRD = LOC_FractionBaudRate;
        /****************************************************************************************************/
        /* Select the Clock Source to Baud rate */
        UARTx[Copy_structUartConfigs->UART_ChannelID]->CC |= Copy_structUartConfigs->UART_Clk;
        /****************************************************************************************************/
        /* Configure the UART OPtions (Parity , Data Length , Stop Bit ...) */
        UARTx[Copy_structUartConfigs->UART_ChannelID]->LCRH |= Copy_structUartConfigs->UART_Data | Copy_structUartConfigs->UART_Stop | Copy_structUartConfigs->UART_Parity;
        /****************************************************************************************************/
        /* Enable/Disable the UART TX / RX */
        UARTx[Copy_structUartConfigs->UART_ChannelID]->CTL |= Copy_structUartConfigs->UART_TX | Copy_structUartConfigs->UART_RX ;
        UARTx[Copy_structUartConfigs->UART_ChannelID]->IM  |= Copy_structUartConfigs->UART_Interrupt;
        /* Enable the UART */
        SET_BIT(UARTx[Copy_structUartConfigs->UART_ChannelID]->CTL, UART_EN);


    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;
}

u8 UART_u8SetTXCallBack(UART_Channel_t Copy_enumUARTChannel, void(*PtrFunc)(void) )
{
    /* Error State that the Function has an error ("User should check the return of this Function") */
    u8 LOC_ErrorState = STD_OK ;
    if( PtrFunc != NULL )
    {
        /* Save the Callback Function to the equivalent array index */
        UART_TX_CALLBACK[Copy_enumUARTChannel] = PtrFunc ;
    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;
}
u8 UART_u8SetRXCallBack(UART_Channel_t Copy_enumUARTChannel, void(*PtrFunc)(u8) )
{
    /* Error State that the Function has an error ("User should check the return of this Function") */
    u8 LOC_ErrorState = STD_OK ;
    if( PtrFunc != NULL )
    {
        /* Save the Callback Function to the equivalent array index */
        UART_RX_CALLBACK[Copy_enumUARTChannel] = PtrFunc ;
    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;
}

u8 UART_u8EnableTXInterrupt (UART_Channel_t Copy_enumUartChannelId)
{
    u8 LOC_ErrorState = STD_OK ;
    if( Copy_enumUartChannelId <= UART_7 )
    {
        /* Set the Transmit  Interrupt bit  */
        UARTx[Copy_enumUartChannelId]->IM |= (1 << UART_TXIM);
    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;
}
u8 UART_u8DisableTXInterrupt(UART_Channel_t Copy_enumUartChannelId)
{
    u8 LOC_ErrorState = STD_OK ;
    if(Copy_enumUartChannelId <= UART_7 )
    {
        /* Clear the Transmit  Interrupt bit  */
        UARTx[Copy_enumUartChannelId]->IM &= ~(1 << UART_TXIM);
    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;
}

u8 UART_u8EnableRXInterrupt (UART_Channel_t Copy_enumUartChannelId)
{
    u8 LOC_ErrorState = STD_OK ;
    if( Copy_enumUartChannelId <= UART_7  )
    {
        /* Set the Receive Interrupt bit  */
        UARTx[Copy_enumUartChannelId]->IM |= (1 << UART_RXIM);
    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;
}
u8 UART_u8DisableRXInterrupt(UART_Channel_t Copy_enumUartChannelId)
{
    u8 LOC_ErrorState = STD_OK ;
    if( Copy_enumUartChannelId <= UART_7  )
    {
        /* Clear the Receive  Interrupt bit  */
        UARTx[Copy_enumUartChannelId]->IM &= ~(1 << UART_RXIM);
    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;
}

u8 UART_u8TransmitChar_POL(UART_Channel_t Copy_enumUartChannelId, u8 Copy_u8Character)
{
    u8 LOC_ErrorState = STD_OK ;
    u32 LOC_TimeOut = 0;
    if( Copy_enumUartChannelId <= UART_7  )
    {
        // wait until the Tx Buffer is not full
        while( GET_BIT(UARTx[Copy_enumUartChannelId]->FR, UART_TXFF ) == 1)
        {
            LOC_TimeOut++;
            if ( LOC_TimeOut == 100000)
            {
                LOC_ErrorState = STD_TIME_OUT;
                break;
            }
        }
        if( GET_BIT(UARTx[Copy_enumUartChannelId]->FR, UART_TXFF ) == 0 )
        {
            UARTx[Copy_enumUartChannelId]->DR = Copy_u8Character; // transmit the data by writing it to Data register
            LOC_ErrorState = STD_OK;
        }

    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }

    return LOC_ErrorState;

}
u8 UART_u8ReceiveChar_POL (UART_Channel_t Copy_enumUartChannelId, u8 * Copy_u8CharacterBuffer)
{
    u8 LOC_ErrorState = STD_OK ;
    if( Copy_enumUartChannelId <= UART_7  )
    {
        if ( Copy_u8CharacterBuffer != NULL )
        {
            while( GET_BIT(UARTx[Copy_enumUartChannelId]->FR, UART_RXFF ) == 0 ); // wait until the Rx Buffer is not empty
            *Copy_u8CharacterBuffer = UARTx[Copy_enumUartChannelId]->DR ;    // Write the Received Data to the pointer
        }
        else
        {
            LOC_ErrorState = STD_NOK ;
        }

    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;
}

u8 UART_u8TransmitString_POL(UART_Channel_t Copy_enumUartChannelId, u8 * Copy_u8DataBuffer, u8 Copy_BufferSize)
{
    u8 LOC_ErrorState = STD_OK ;
    u8 LOC_LoopIterator =0;
    if(Copy_enumUartChannelId <= UART_7  )
    {
        /* Send the data character at time and repeat the sending with the data size    */
        for(LOC_LoopIterator = 0; LOC_LoopIterator < Copy_BufferSize; LOC_LoopIterator++ )
        {
            LOC_ErrorState = UART_u8TransmitChar_POL(Copy_enumUartChannelId, Copy_u8DataBuffer[LOC_LoopIterator]);
            if ( LOC_ErrorState == STD_NOK )
            {
                /* It means that an error has been happened so return Not Ok */
                return STD_NOK;
            }
        }
    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;
}


u8 UART_u8ReceiveString_POL(UART_Channel_t Copy_enumUartChannelId , u8 * Copy_u8DataBuffer, u8 Copy_BufferSize)
{
    u8 LOC_ErrorState = STD_OK ;
    u8 LOC_LoopIterator = 0;
    if(Copy_enumUartChannelId <= UART_7 )
    {
        if( Copy_u8DataBuffer != NULL )
        {
            /* Receive the data character at time and repeat the Receiving with the data size    */
            for (LOC_LoopIterator = 0; LOC_LoopIterator < Copy_BufferSize; LOC_LoopIterator++ )
            {
                LOC_ErrorState = UART_u8ReceiveChar_POL(Copy_enumUartChannelId, (Copy_u8DataBuffer + LOC_LoopIterator  ) );
                if( LOC_ErrorState ==STD_NOK )
                {
                    /* It means that an error has been happened so return Not Ok */
                    return STD_NOK;
                }
            }
        }
        else
        {
            LOC_ErrorState = STD_NOK;
        }

    }
    else
    {
        LOC_ErrorState = STD_NOK;
    }
    return LOC_ErrorState;

}


void UART0_Handler(void)
{
    u32 LOC_Readback = 0;
    if( GET_BIT(UARTx[UART_0]->MIS, UART_TXMIS) )
    {
        if(UART_TX_CALLBACK[UART_0] != NULL )
        {
            UART_TX_CALLBACK[UART_0]();                 // Serving the Function to be called during the interrupt
            SET_BIT( UARTx[UART_0]->ICR , UART_TXIC ); // Clear the Tx Flag by Writing '1'
            LOC_Readback = UARTx[UART_0]->ICR ;        // a read to Force Clearing of Interrupt Flag
        }
    }
    else if( GET_BIT(UARTx[UART_0]->MIS, UART_RXMIS) )
    {
        if(UART_RX_CALLBACK[UART_0] != NULL)
        {
            UART_RX_CALLBACK[UART_0](UARTx[UART_0]->DR);                     // Serving the Function to be called during the interrupt
            SET_BIT( UARTx[UART_0]->ICR , UART_RXIC );     // Clear the Rx Flag by Writing '1'
            LOC_Readback = UARTx[UART_0]->ICR ;           // a read to Force Clearing of Interrupt Flag
        }
    }

}
void UART1_Handler(void)
{
    u32 LOC_Readback = 0;
    if( GET_BIT(UARTx[UART_1]->MIS, UART_TXMIS) )
    {
        if(UART_TX_CALLBACK[UART_1] != NULL )
        {
            UART_TX_CALLBACK[UART_1]();
            SET_BIT( UARTx[UART_1]->ICR , UART_TXIC ); // Clear the Tx Flag by Writing '1'
            LOC_Readback = UARTx[UART_1]->ICR ;        // a read to Force Clearing of Interrupt Flag
        }
    }
    else if( GET_BIT(UARTx[UART_1]->MIS, UART_RXMIS) )
    {
        if(UART_RX_CALLBACK[UART_1] != NULL)
        {
            UART_RX_CALLBACK[UART_1](UARTx[UART_1]->DR);
            SET_BIT( UARTx[UART_1]->ICR , UART_RXIC );     // Clear the Rx Flag by Writing '1'
            LOC_Readback = UARTx[UART_1]->ICR ;           // a read to Force Clearing of Interrupt Flag
        }
    }
}
void UART2_Handler(void)
{
    u32 LOC_Readback = 0;
    if( GET_BIT(UARTx[UART_2]->MIS, UART_TXMIS) )
    {
        if(UART_TX_CALLBACK[UART_2] != NULL )
        {
            UART_TX_CALLBACK[UART_2]();
            SET_BIT( UARTx[UART_2]->ICR , UART_TXIC ); // Clear the Tx Flag by Writing '1'
            LOC_Readback = UARTx[UART_2]->ICR ;        // a read to Force Clearing of Interrupt Flag
        }
    }
    else if( GET_BIT(UARTx[UART_2]->MIS, UART_RXMIS) )
    {
        if(UART_RX_CALLBACK[UART_2] != NULL)
        {
            UART_RX_CALLBACK[UART_2](UARTx[UART_2]->DR);
            SET_BIT( UARTx[UART_2]->ICR , UART_RXIC );     // Clear the Rx Flag by Writing '1'
            LOC_Readback = UARTx[UART_2]->ICR ;           // a read to Force Clearing of Interrupt Flag
        }
    }

}
void UART3_Handler(void)
{
    u32 LOC_Readback = 0;
    if( GET_BIT(UARTx[UART_3]->MIS, UART_TXMIS) )
    {
        if(UART_TX_CALLBACK[UART_3] != NULL )
        {
            UART_TX_CALLBACK[UART_3]();
            SET_BIT( UARTx[UART_3]->ICR , UART_TXIC ); // Clear the Tx Flag by Writing '1'
            LOC_Readback = UARTx[UART_3]->ICR ;        // a read to Force Clearing of Interrupt Flag
        }
    }
    else if( GET_BIT(UARTx[UART_3]->MIS, UART_RXMIS) )
    {
        if(UART_RX_CALLBACK[UART_3] != NULL)
        {
            UART_RX_CALLBACK[UART_3](UARTx[UART_3]->DR);
            SET_BIT( UARTx[UART_3]->ICR , UART_RXIC );     // Clear the Rx Flag by Writing '1'
            LOC_Readback = UARTx[UART_3]->ICR ;           // a read to Force Clearing of Interrupt Flag
        }
    }
}
void UART4_Handler(void)
{
    u32 LOC_Readback = 0;
    if( GET_BIT(UARTx[UART_4]->MIS, UART_TXMIS) )
    {
        if(UART_TX_CALLBACK[UART_4] != NULL )
        {
            UART_TX_CALLBACK[UART_4]();
            SET_BIT( UARTx[UART_4]->ICR , UART_TXIC ); // Clear the Tx Flag by Writing '1'
            LOC_Readback = UARTx[UART_4]->ICR ;        // a read to Force Clearing of Interrupt Flag
        }
    }
    else if( GET_BIT(UARTx[UART_4]->MIS, UART_RXMIS) )
    {
        if(UART_RX_CALLBACK[UART_4] != NULL)
        {
            UART_RX_CALLBACK[UART_4](UARTx[UART_4]->DR);
            SET_BIT( UARTx[UART_4]->ICR , UART_RXIC );     // Clear the Rx Flag by Writing '1'
            LOC_Readback = UARTx[UART_4]->ICR ;           // a read to Force Clearing of Interrupt Flag
        }
    }
}
void UART5_Handler(void)
{
    u32 LOC_Readback = 0;
    if( GET_BIT(UARTx[UART_5]->MIS, UART_TXMIS) )
    {
        if(UART_TX_CALLBACK[UART_5] != NULL )
        {
            UART_TX_CALLBACK[UART_5]();
            SET_BIT( UARTx[UART_5]->ICR , UART_TXIC ); // Clear the Tx Flag by Writing '1'
            LOC_Readback = UARTx[UART_5]->ICR ;        // a read to Force Clearing of Interrupt Flag
        }
    }
    else if( GET_BIT(UARTx[UART_5]->MIS, UART_RXMIS) )
    {
        if(UART_RX_CALLBACK[UART_5] != NULL)
        {
            UART_RX_CALLBACK[UART_5](UARTx[UART_5]->DR);
            SET_BIT( UARTx[UART_5]->ICR , UART_RXIC );     // Clear the Rx Flag by Writing '1'
            LOC_Readback = UARTx[UART_5]->ICR ;           // a read to Force Clearing of Interrupt Flag
        }
    }
}
void UART6_Handler(void)
{
    u32 LOC_Readback = 0;
    if( GET_BIT(UARTx[UART_6]->MIS, UART_TXMIS) )
    {
        if(UART_TX_CALLBACK[UART_6] != NULL )
        {
            UART_TX_CALLBACK[UART_6]();
            SET_BIT( UARTx[UART_6]->ICR , UART_TXIC ); // Clear the Tx Flag by Writing '1'
            LOC_Readback = UARTx[UART_6]->ICR ;        // a read to Force Clearing of Interrupt Flag
        }
    }
    else if( GET_BIT(UARTx[UART_6]->MIS, UART_RXMIS) )
    {
        if(UART_RX_CALLBACK[UART_6] != NULL)
        {
            UART_RX_CALLBACK[UART_6](UARTx[UART_6]->DR);
            SET_BIT( UARTx[UART_6]->ICR , UART_RXIC );     // Clear the Rx Flag by Writing '1'
            LOC_Readback = UARTx[UART_6]->ICR ;           // a read to Force Clearing of Interrupt Flag
        }
    }
}
void UART7_Handler(void)
{
    u32 LOC_Readback = 0;
    if( GET_BIT(UARTx[UART_7]->MIS, UART_TXMIS) )
    {
        if(UART_TX_CALLBACK[UART_7] != NULL )
        {
            UART_TX_CALLBACK[UART_7]();
            SET_BIT( UARTx[UART_7]->ICR , UART_TXIC ); // Clear the Tx Flag by Writing '1'
            LOC_Readback = UARTx[UART_7]->ICR ;        // a read to Force Clearing of Interrupt Flag
        }
    }
    else if( GET_BIT(UARTx[UART_7]->MIS, UART_RXMIS) )
    {
        if(UART_RX_CALLBACK[UART_7] != NULL)
        {
            UART_RX_CALLBACK[UART_7](UARTx[UART_7]->DR);
            SET_BIT( UARTx[UART_7]->ICR , UART_RXIC );     // Clear the Rx Flag by Writing '1'
            LOC_Readback = UARTx[UART_7]->ICR ;           // a read to Force Clearing of Interrupt Flag
        }
    }
}

