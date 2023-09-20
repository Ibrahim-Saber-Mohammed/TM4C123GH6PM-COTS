/*************************************************************************/
/********************** SWC    : I2C                **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 27, 2021       **********************/
/*************************************************************************/

#include "../LIB/STD_TYPES/STD_TYPES.h"
#include "../LIB/BIT_MATH/BIT_MATH.h"
#include "../LIB/TIVAC123GH6PM/TIVAC123GH6PM.h"

#include "I2C_interface.h"
#include "I2C_config.h"
#include "I2C_private.h"

static I2C_REGISTERS_t * I2Cx[4] = { I2C0, I2C1, I2C2 , I2C3 };


u8 I2C_u8MasterSetConfiguartions ( const I2C_Master_Configs_t * Copy_structMasterConfigs )
{
    u8 Local_u8ErrorState = STD_OK;
    if( Copy_structMasterConfigs != NULL )
    {
        if(Copy_structMasterConfigs->I2C_PeripherlNumber <= I2C_3)
        {
            /* Selecting Master/Slave */
            I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MCR |= Copy_structMasterConfigs->I2C_Mode;
            /* I2C Master INTERRUPT */
            if(Copy_structMasterConfigs->I2C_InterruptState == I2C_MASTER_INTERRUPT_ENABLE)
            {
                SET_BIT( I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MIMR, IM );
            }
            else if ( Copy_structMasterConfigs->I2C_InterruptState == I2C_CLOCK_TIMEOUT_ENABLE )
            {
                SET_BIT( I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MIMR, CLKIM );
            }
            else
            {
                CLR_BIT( I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MIMR, IM );
                CLR_BIT( I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MIMR, CLKIM );
            }
            I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->SOAR = Copy_structMasterConfigs->I2C_NodeOwnAddress;

            if( Copy_structMasterConfigs->I2C_GlitchFiltrt == I2C_GLITCH_FILTER_DISABLE )
            {
                CLR_BIT( I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MCR, GFE);
            }
            else
            {
                SET_BIT( I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MCR, GFE);
                I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MCR2 |= Copy_structMasterConfigs->I2C_GlitchFiltrt ;
            }

            if( Copy_structMasterConfigs->I2C_HighSpeedState == I2C_HIGH_SPEED_ENABLE )
            {
                SET_BIT( I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MCS , HS );
                /* The System Clock is 80MHz and SCL clock speed of 3.33 Mbps */
                I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MTPR = 3;
            }
            else
            {
                /* Low Speed */
                CLR_BIT( I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MCS , HS );
                /* Calculating the TPR Value  */
                I2Cx[Copy_structMasterConfigs->I2C_PeripherlNumber]->MTPR = (SYS_CLOCK)/(20*Copy_structMasterConfigs->I2C_Clock_Speed)-1 ;
            }
        }
        else
        {
            Local_u8ErrorState = STD_NOK;
        }
    }
    else
    {
        Local_u8ErrorState = STD_NOK;
    }
    return (Local_u8ErrorState);
}

u8 I2C_u8SlaveSetConfiguartions ( const I2C_Slave_Configs_t * Copy_structSlaveConfigs )
{
    u8 Local_u8ErrorState = STD_OK;
    if( Copy_structSlaveConfigs != NULL )
    {
        if(Copy_structSlaveConfigs->I2C_PeripherlNumber <= I2C_3)
        {
            I2Cx[Copy_structSlaveConfigs->I2C_PeripherlNumber]->SOAR = Copy_structSlaveConfigs->I2C_NodeOwnAddress;
            I2Cx[Copy_structSlaveConfigs->I2C_PeripherlNumber]->SCSR |= (1 << 0);
            I2Cx[Copy_structSlaveConfigs->I2C_PeripherlNumber]->MCR |= Copy_structSlaveConfigs->I2C_Mode;
            if(Copy_structSlaveConfigs->I2C_SlaveInterrupt == I2C_SLAVE_NO_INTERRUPTS)
            {
                I2Cx[Copy_structSlaveConfigs->I2C_PeripherlNumber]->SIMR &=~(0b111 << 0);
            }
            else
            {
                I2Cx[Copy_structSlaveConfigs->I2C_PeripherlNumber]->SIMR |= Copy_structSlaveConfigs->I2C_SlaveInterrupt;
            }
        }
        else
        {
            Local_u8ErrorState = STD_NOK;
        }
    }
    else
    {
        Local_u8ErrorState = STD_NOK;
    }
    return (Local_u8ErrorState);
}

u8 I2C_u8MasterSendStartCondition( I2C_ChannelID_t Copy_enumI2C_ID )
{
    /* The user Should read the Output of this Function if it is OK then continue sending the Slave Address */
    /* If the return of the function is not OK then  that is mean another master have the bus and i should wait until the bus is not busy  */

    u8 Local_u8ErrorState = STD_OK;
    if( Copy_enumI2C_ID <= I2C_3 ) /* if the user enters I2C number that is nor valid */
    {
        if( GET_BIT(I2Cx[Copy_enumI2C_ID]->MCS , BUSBUSY ) == 0 ) /* in multiple MSATERS then Should Check if the Bus is Busy or not  */
        {
            SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , START );  /* if there is not any Master taking the bus then send Start Condition  */
        }
        else
        {
            Local_u8ErrorState = STD_NOK;  /* if there is another Master have the bus then Send Error State  */
        }

    }
    else
    {
        Local_u8ErrorState = STD_NOK;
    }
    return (Local_u8ErrorState); /* return the Error State Flag  */
}

u8 I2C_u8MasterSendStopCondition( I2C_ChannelID_t Copy_enumI2C_ID )
{
    /* This Function sends the STOP condition so the Master is no longer have the Bus and to Start new transaction should send START Condition*/

    u8 Local_u8ErrorState = STD_OK;
    if( Copy_enumI2C_ID <= I2C_3 )  /* if the user enters I2C number that is nor valid */
    {
        SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , STOP ); /* Generate the stop Condition */
        while( GET_BIT(I2Cx[Copy_enumI2C_ID]->MCS , BUSY)!= 0 ); /* wait until the I2C is not Busy */
    }
    else
    {
        Local_u8ErrorState = STD_NOK;
    }
    return (Local_u8ErrorState); /* return the Error State Flag  */
}

u8 I2C_u8MasterSendSlaveAddress_Write( I2C_ChannelID_t Copy_enumI2C_ID, u8 Copy_u8SlaveAddress )
{
    u8 Local_u8ErrorState = STD_OK;
    if( Copy_enumI2C_ID <= I2C_3 ) /* if the user enters I2C number that is nor valid */
    {
        if( Copy_u8SlaveAddress > MAX_NUMBER_SLAVES )
        {
            /* To Send Slave Address With Write  */
            /*  <-----Address----->   W
             *  b7 b6 b5 b4 b3 b2 b1  0*/
            I2Cx[Copy_enumI2C_ID]->MCA = (Copy_u8SlaveAddress << 1);
        }
        else
        {
            Local_u8ErrorState = STD_NOK; /* raise the error Flag */
        }
    }
    else
    {
        Local_u8ErrorState = STD_NOK; /* raise the error Flag */
    }
    return (Local_u8ErrorState); /* return the Error State Flag  */
}
u8 I2C_u8MasterSendSlaveAddress_Read ( I2C_ChannelID_t Copy_enumI2C_ID, u8 Copy_u8SlaveAddress )
{
    u8 Local_u8ErrorState = STD_OK;
    if( Copy_enumI2C_ID <= I2C_3 )
    {
        if( Copy_u8SlaveAddress > MAX_NUMBER_SLAVES )
        {
            /* To Send Slave Address With Read  */
            /*  <-----Address----->  | R |
             *  b7 b6 b5 b4 b3 b2 b1 | 1 |*/
            I2Cx[Copy_enumI2C_ID]->MCA = ( (Copy_u8SlaveAddress << 1) | 0x01 );
        }
        else
        {
            Local_u8ErrorState = STD_NOK;  /* raise the error Flag */
        }
    }
    else
    {
        Local_u8ErrorState = STD_NOK; /* raise the error Flag */
    }
    return (Local_u8ErrorState); /* return the Error State Flag  */
}

u8 I2C_u8MasterSingleTransmitData( I2C_ChannelID_t Copy_enumI2C_ID, u8 Copy_u8TransmittedData )
{
    u8 Local_u8ErrorState = STD_OK;
    if(Copy_enumI2C_ID <= I2C_3)
    {
        /* Write the Data To the Master Data Register */
        I2Cx[Copy_enumI2C_ID]->MDR = Copy_u8TransmittedData;
        /* The Start Condition and the address of the SLave Should be sent
         * Start Sending  */
        SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS, RUN );
        while( GET_BIT(I2Cx[Copy_enumI2C_ID]->MCS , BUSY)!= 0 );  // wait the I2C controller is not Busy " Finished his job "
        if( GET_BIT(I2Cx[Copy_enumI2C_ID]->MCS , ERROR) != 0 )      /* Check the Error */
        {
            Local_u8ErrorState = STD_NOK; /* raise the error Flag */
        }
        else
        {
            // Do nothing
        }
    }
    else
    {
        Local_u8ErrorState = STD_NOK; /* raise the error Flag */
    }
    return (Local_u8ErrorState); /* return the Error State Flag  */
}

u8 I2C_u8MasterSingleReceiveData(I2C_ChannelID_t Copy_enumI2C_ID, u8 * Copy_ptrDataBuffer)
{
    u8 Local_u8ErrorState = STD_OK;
    if( Copy_enumI2C_ID <=I2C_3 )
    {
        if( Copy_ptrDataBuffer != NULL )
        {
            /* The Start Condition and the address of the SLave Should be sent
             * Start Sending  */
            SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS, RUN);
            while( GET_BIT(I2Cx[Copy_enumI2C_ID]->MCS , BUSY)!= 0  ); // wait the I2C controller is not Busy " Finished his job "
            if( GET_BIT(I2Cx[Copy_enumI2C_ID]->MCS , ERROR) != 0 ) /* Check the Error */
            {
                Local_u8ErrorState = STD_NOK; /* raise the error Flag */
            }
            else
            {
                *Copy_ptrDataBuffer = I2Cx[Copy_enumI2C_ID]->MDR; // If no Error then Read Data from the Master data register
            }
        }
        else
        {
            Local_u8ErrorState = STD_NOK;   /* raise the error Flag */
        }

    }
    else
    {
        Local_u8ErrorState = STD_NOK;   /* raise the error Flag */
    }
    return (Local_u8ErrorState);  /* return the Error State Flag  */
}

u8 I2C_u8MasterSendRepeatedStart( I2C_ChannelID_t Copy_enumI2C_ID )
{
    u8 Local_u8ErrorState = STD_OK;
    if( Copy_enumI2C_ID <= I2C_3 )
    {
        SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , START );  // Re send the Start Condition
        while( GET_BIT(I2Cx[Copy_enumI2C_ID]->MCS , BUSY)!= 0 );  // wait the I2C is not Busy

    }
    else
    {
        Local_u8ErrorState = STD_NOK;  // raise the error flag
    }
    return (Local_u8ErrorState);    /* return the error flag */
}
/****************************************************************************************************************************/
/* Function Flow Chart is in page 1000 in the data sheet of TMC123GH6PM */
u8 I2C_u8MasterTransmitMultipleDataBytes( I2C_ChannelID_t Copy_enumI2C_ID, u8 * Copy_ptrTransmitDataBuffer , u8 Copy_u8DataSize)
{
    // local variables to return the Error State in this Function if equal STD_OK then the Function has no Error
    // if Not then the user need to handle the Error
    u8 Local_u8ErrorState = STD_OK;
    u8 Local_u8Check = STD_OK;        // variable to receive the return of the  I2C_u8MasterSingleReceiveData
    u8 Local_u8LoopIterator = 0;     // Loop iterator

    // If the user of the Function Send I2C_ID not Supported then it is an Error
    // If the user of the Function Send a NULL Pointer then it is an Error
    if( (Copy_enumI2C_ID > I2C_3) || Copy_ptrTransmitDataBuffer == NULL )
    {
        Local_u8ErrorState = STD_NOK;
    }
    else
    {
        for( Local_u8LoopIterator = 0; Local_u8LoopIterator<Copy_u8DataSize; Local_u8LoopIterator++ )
        {
            /* Master Transmit the Byte to be Transmitted according to the Buffer */
            Local_u8Check = I2C_u8MasterSingleTransmitData(Copy_enumI2C_ID , Copy_ptrTransmitDataBuffer[Local_u8LoopIterator]);
            if( Local_u8Check == STD_OK)
            {
                continue; /* If the Byte has Been Transmitted without any errors then Send the next byte */
            }
            else
            {
                /* the Transmitted byte was not Sent correctly then raise the error state flag and Stop Sending */
                if ( GET_BIT( I2Cx[Copy_enumI2C_ID]->MCS, ARBLST ) )
                {
                    Local_u8ErrorState = STD_NOK;  /* Raise the flag and Stop Sending */
                    break;
                }
                else
                {
                    CLR_BIT( I2Cx[Copy_enumI2C_ID]->MCS , START );
                    CLR_BIT( I2Cx[Copy_enumI2C_ID]->MCS , RUN );
                    SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , STOP ); /* Stop Sending and Generate Stop Condition */
                    Local_u8ErrorState = STD_NOK;  /* Raise the flag */
                    break;
                }
            }
        }
        /* After transmitting all data in the buffer we should  */
        CLR_BIT( I2Cx[Copy_enumI2C_ID]->MCS , START );
        /* The user Will Send the Stop Condition after this Function */
        //        SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , STOP );  /* Sending Stop Condition and the Master Will not be able to send repeated start */
        SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , RUN );
        while( GET_BIT(I2Cx[Copy_enumI2C_ID]->MCS , BUSY)!= 0 ); /* Waiting the I2C is not busy   */
        if( GET_BIT( I2Cx[Copy_enumI2C_ID]->MCS, ERROR ) != 0)  /* Checking if there is an error or not */
        {
            Local_u8ErrorState = STD_NOK;
        }
        else
        {

        }

    }
    return (Local_u8ErrorState); /* return the Error Flag State */
}

/* Function Flow Chart is in page 1001 Data sheet of TMC123GH6PM  */
u8 I2C_MasterReceiveMultipleBytes( I2C_ChannelID_t Copy_enumI2C_ID, u8 * Copy_ptrDataBuffer , u8 Copy_u8DataSize )
{
    // local variables to return the Error State in this Function if equal STD_OK then the Function has no Error
    // if Not then the user need to handle the Error
    u8 Local_u8ErrorState = STD_OK;
    u8 Local_u8LoopIterator = 0; // Loop iterator
    u8 Local_u8Check = STD_OK;  // variable to receive the return of the  I2C_u8MasterSingleReceiveData

    // If the user of the Function Send I2C_ID not Supported then it is an Error
    // If the user of the Function Send a NULL Pointer then its an Error
    if( Copy_ptrDataBuffer == NULL || Copy_enumI2C_ID > I2C_3 )
    {
        Local_u8ErrorState = STD_NOK;
    }
    else
    {
        for( Local_u8LoopIterator=0; Local_u8LoopIterator < Copy_u8DataSize ; Local_u8LoopIterator++ )
        {
            // To Receive multiple Bytes then Master should Send An ACK to the Slave
            // The Enable of RUN bit is done inside the Function I2C_u8MasterSingleReceiveData
            SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , ACK  );
            CLR_BIT( I2Cx[Copy_enumI2C_ID]->MCS , STOP );  // As the Master will Receive multiple Bytes
            Local_u8Check = I2C_u8MasterSingleReceiveData(Copy_enumI2C_ID, (Copy_ptrDataBuffer+Local_u8LoopIterator) );

            if( Local_u8Check == STD_OK) /* if the Receive Function has no Error then Continue Receiving */
            {
                continue;
            }
            else     /* if the Receive Function has an Error then Stop Receiving and break the Loop After Setting the Error State Flag */
            {
                if ( GET_BIT( I2Cx[Copy_enumI2C_ID]->MCS, ARBLST ) )
                {
                    Local_u8ErrorState = STD_NOK;  /* Raise the Error State Flag  */
                    break;
                }
                else
                {
                    CLR_BIT( I2Cx[Copy_enumI2C_ID]->MCS , START ); /* Clear the Start Bit  */
                    CLR_BIT( I2Cx[Copy_enumI2C_ID]->MCS , RUN );   /* Clear the RUN bit  */
                    SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , STOP );  /* Send STOP Condition */
                    Local_u8ErrorState = STD_NOK; /* Raise the Error State Flag  */
                    break;
                }
            }
            // if the Byte to be received is (index -1 ) then we Should Clear the ACK bit
            if( Local_u8LoopIterator == (Copy_u8DataSize-1 ) )
            {
                CLR_BIT( I2Cx[Copy_enumI2C_ID]->MCS , ACK  );
                CLR_BIT( I2Cx[Copy_enumI2C_ID]->MCS , START);
                //                SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , STOP ); /* The user in main Should Send STOP Condition After this Function */
                /* Sending the STOP Condition in the Function then the Master Will Lose The Bus and Can not send Repeated Start */
                SET_BIT( I2Cx[Copy_enumI2C_ID]->MCS , RUN  );
            }
        }
    }
    return (Local_u8ErrorState);  /* Return the Error State */
}
/*************************************************************************************************************/

u8 I2C_u8SlaveListenTheBus( I2C_ChannelID_t Copy_enumI2C_ID )
{
    u8 Local_u8ErrorState = STD_OK;
    u8 Local_u8CheckResult = 0;
    u32 Local_u32Timeout = 0;  /* variable to control the while loop so the Slave will not stuck in it forever */
    if( Copy_enumI2C_ID > I2C_3 ) /* Check the Channel ID is valid or not */
    {
        Local_u8ErrorState = STD_NOK; /* Raise the Error Flag the Channel is not valid */
    }
    else
    {
        /*The Slave listens to the Bus for receiving or sending data if the Slave does not receive any data for I2C_SLAVE_POOLING_TIMEOUT then Will raise an Error and break the loop */
        while( Local_u32Timeout <= I2C_SLAVE_POOLING_TIMEOUT)
        {
            if( GET_BIT( I2Cx[Copy_enumI2C_ID]->SCSR, RREQ ) ) /* Checking the Slave received his address and Requested to receive data */
            {
                Local_u8CheckResult = I2C_SLAVE_RECEIVE;
                return Local_u8CheckResult; /* Return with the RECEIVE request */
            }
            else if( GET_BIT( I2Cx[Copy_enumI2C_ID]->SCSR, TREQ ) ) /* Checking the Slave received his address and Requested to Transmit data */
            {
                Local_u8CheckResult = I2C_SLAVE_TRANSMIT;
                return Local_u8CheckResult; /* Return with the TRANSMIT request */
            }
            else
            {
                Local_u32Timeout++; /* Increment the Time out variable */

            }
        }
        Local_u8ErrorState = STD_TIME_OUT; /* Raise the time out flag */
    }
    return (Local_u8ErrorState);  /* return the error flag */
}

u8 I2C_u8SlaveRecieveByte(I2C_ChannelID_t Copy_enumI2C_ID, u8 *Copy_ptrData)
{
    u8 Local_u8ErrorState = STD_OK;
    if( (Copy_enumI2C_ID > I2C_3) || (Copy_ptrData == NULL) )
    {
        Local_u8ErrorState = STD_NOK;
    }
    else
    {
        *Copy_ptrData = I2Cx[Copy_enumI2C_ID]->SDR;  /* Reading the data from the Master */
    }
    return (Local_u8ErrorState);
}
u8 I2C_u8SlaveTransmitByte(I2C_ChannelID_t Copy_enumI2C_ID, u8 Copy_ptrData)
{
    u8 Local_u8ErrorState = STD_OK;
    if( (Copy_enumI2C_ID > I2C_3) || (Copy_ptrData == NULL) )
    {
        Local_u8ErrorState = STD_NOK;
    }
    else
    {
        I2Cx[Copy_enumI2C_ID]->SDR = Copy_ptrData; /* Transmitting data to the master */
    }
    return (Local_u8ErrorState);
}
