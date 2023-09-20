/*************************************************************************/
/********************** SWC    : I2C                **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 27, 2021       **********************/
/*************************************************************************/
#ifndef MCAL_I2C_INTERFACE_H_
#define MCAL_I2C_INTERFACE_H_


#define I2C_SLAVE_RECEIVE       2
#define I2C_SLAVE_TRANSMIT      3

typedef enum{
    I2C_SLAVE_STOP_INTERRUPT  = ( 1 << 2 ),
    I2C_SLAVE_START_INTERRUPT = ( 1 << 1 ),
    I2C_SLAVE_DATA_INTERRUPT  = ( 1 << 0 ),
    I2C_SLAVE_NO_INTERRUPTS
}I2C_SLAVE_Interrupt_t;

typedef enum{
    I2C_0,
    I2C_1,
    I2C_2,
    I2C_3
}I2C_ChannelID_t;

typedef enum{
    I2C_MASTER_FUNCTION_DISABLE = (0 << 4),
    I2C_MASTER_FUNCTION_ENABLE  = (1 << 4),
    I2C_SLAVE_FUNCTION_DISABLE  = (0 << 5),
    I2C_SLAVE_FUNCTION_ENABLE   = (1 << 5),
}I2C_Function_t;

typedef enum{
    I2C_MASTER_INTERRUPT_DISABLE,
    I2C_MASTER_INTERRUPT_ENABLE ,
    I2C_CLOCK_TIMEOUT_ENABLE    ,
    I2C_CLOCK_TIMEOUT_DISABLE
}I2C_Interrupt_t;

typedef enum{
    I2C_HIGH_SPEED_DISABLE  ,
    I2C_HIGH_SPEED_ENABLE
}I2C_HighSpeed_t;

typedef enum{
    I2C_GLITCH_FILTER_DISABLE               ,
    GLITCH_PULSE_CLOCK_1     = ( 1 << 4 )   ,
    GLITCH_PULSE_CLOCK_2     = ( 2 << 4 )   ,
    GLITCH_PULSE_CLOCK_3     = ( 3 << 4 )   ,
    GLITCH_PULSE_CLOCK_4     = ( 4 << 4 )   ,
    GLITCH_PULSE_CLOCK_8     = ( 5 << 4 )   ,
    GLITCH_PULSE_CLOCK_16    = ( 6 << 4 )   ,
    GLITCH_PULSE_CLOCK_31    = ( 7 << 4 )   ,

}I2C_Glitch_Filter_t;

typedef struct{
    I2C_ChannelID_t     I2C_PeripherlNumber ;
    u8                  I2C_NodeOwnAddress  ;
    I2C_Interrupt_t     I2C_InterruptState  ;
    I2C_HighSpeed_t     I2C_HighSpeedState  ;
    I2C_Glitch_Filter_t I2C_GlitchFiltrt    ;
    I2C_Function_t      I2C_Mode            ;
    u32                 I2C_Clock_Speed     ;
}I2C_Master_Configs_t;


typedef struct{
    I2C_ChannelID_t         I2C_PeripherlNumber ;
    u8                      I2C_NodeOwnAddress  ;
    I2C_Function_t          I2C_Mode            ;
    I2C_SLAVE_Interrupt_t   I2C_SlaveInterrupt  ;
}I2C_Slave_Configs_t;
/* Master Functions */
u8 I2C_u8MasterSetConfiguartions ( const I2C_Master_Configs_t * Copy_structMasterConfigs );
u8 I2C_u8MasterSendStartCondition( I2C_ChannelID_t Copy_u8I2C_ID );
u8 I2C_u8MasterSendStopCondition ( I2C_ChannelID_t Copy_u8I2C_ID );
u8 I2C_u8MasterSendSlaveAddress_Write( I2C_ChannelID_t Copy_u8I2C_ID, u8 Copy_u8SlaveAddress );
u8 I2C_u8MasterSendSlaveAddress_Read ( I2C_ChannelID_t Copy_u8I2C_ID, u8 Copy_u8SlaveAddress );
u8 I2C_u8MasterSingleTransmitData( I2C_ChannelID_t Copy_u8I2C_ID, u8 Copy_u8TransmittedData );
u8 I2C_u8MasterSingleReceiveData (I2C_ChannelID_t Copy_u8I2C_ID, u8 * Copy_ptrDataBuffer);
/*****************************************************************************************************/
u8 I2C_u8SlaveSetConfiguartions ( const I2C_Slave_Configs_t * Copy_structSlaveConfigs );
u8 I2C_u8SlaveListenTheBus( I2C_ChannelID_t Copy_enumI2C_ID );
u8 I2C_u8SlaveRecieveByte (I2C_ChannelID_t Copy_enumI2C_ID, u8 *Copy_ptrData);
u8 I2C_u8SlaveTransmitByte(I2C_ChannelID_t Copy_enumI2C_ID, u8 Copy_ptrData);

#endif /* MCAL_I2C_INTERFACE_H_ */
