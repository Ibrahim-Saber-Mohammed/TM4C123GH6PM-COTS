/*************************************************************************/
/********************** SWC    : UART_interface     **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 15, 2021       **********************/
/*************************************************************************/

#ifndef MCAL_UART_UART_INTERFACE_H_
#define MCAL_UART_UART_INTERFACE_H_



typedef enum{
    UART_0  ,
    UART_1  ,
    UART_2  ,
    UART_3  ,
    UART_4  ,
    UART_5  ,
    UART_6  ,
    UART_7
}UART_Channel_t;

typedef enum{
   ONE_STOP = (0b0 << 3 ),
   TWO_STOP = (0b1 << 3 )
}UART_Stop_Bit_t;

typedef enum{
    NO_PARITY   = (0b0 << 1) ,
    EVEN_PARITY = (0b1 << 2) | (0b1 << 1),
    ODD_PARITY  = (0b0 << 2) | (0b1 << 1)
}UART_Parity_t;

typedef enum{
    FIVE_BITS  = (0b00 << 5) ,
    SIX_BITS   = (0b01 << 5) ,
    SEVEN_BITS = (0b10 << 5) ,
    EIGHT_BITS = (0b11 << 5) ,
}UART_Dtat_Length_t;

typedef enum
{
    UART_TXEnable   = (1 << 8),
    UART_TXDisable  = (0 << 8),
}UART_TXMode_t;

typedef enum
{
    UART_RXEnable   = (1 << 9),
    UART_RXDisable  = (0 << 9),
}UART_RXMode_t;


typedef enum{

    TX_ENABLE_INTERRUPT   = (0b1 << 5) ,
    TX_DISABEL_INTERRuPT  = (0b0 << 5)  ,
    RX_ENABLE_INTERRUPT   = (0b1 << 4)  ,
    RX_DISABEL_INTERRuPT  = (0b0 << 4)  ,
}UART_Interrupt_t;

typedef enum{
    SYSTEM_CLOCK = (0b0000),
    PIOSC        = (0x5)
}UART_ClockSrc_t;
typedef struct {
    UART_Channel_t        UART_ChannelID    ;
    u32                   UART_BaudRate     ;
    UART_ClockSrc_t       UART_Clk          ;
    UART_TXMode_t         UART_TX           ;
    UART_RXMode_t         UART_RX           ;
    UART_Dtat_Length_t    UART_Data         ;
    UART_Parity_t         UART_Parity       ;
    UART_Stop_Bit_t       UART_Stop         ;
    UART_Interrupt_t      UART_Interrupt    ;

}UART_Configs_t;


u8 UART_u8SetUartConfigurations(const UART_Configs_t * Copy_structUartConfigs);

u8 UART_u8SetTXCallBack(UART_Channel_t Copy_enumUARTChannel, void(*PtrFunc)(void) );
u8 UART_u8SetRXCallBack(UART_Channel_t Copy_enumUARTChannel, void(*PtrFunc)(u8) );

u8 UART_u8EnableTXInterrupt (UART_Channel_t Copy_enumUartChannelId);
u8 UART_u8DisableTXInterrupt(UART_Channel_t Copy_enumUartChannelId);

u8 UART_u8EnableRXInterrupt (UART_Channel_t Copy_enumUartChannelId);
u8 UART_u8DisableRXInterrupt(UART_Channel_t Copy_enumUartChannelId);

u8 UART_u8TransmitChar_POL(UART_Channel_t Copy_enumUartChannelId, u8 Copy_u8Character);
u8 UART_u8ReceiveChar_POL (UART_Channel_t Copy_enumUartChannelId, u8 * Copy_u8CharacterBuffer);

u8 UART_u8TransmitString_POL(UART_Channel_t Copy_enumUartChannelId, u8 * Copy_u8DataBuffer, u8 Copy_BufferSize);
u8 UART_u8ReceiveString_POL(UART_Channel_t Copy_enumUartChannelId , u8 * Copy_u8DataBuffer, u8 Copy_BufferSize);



#endif /* MCAL_UART_UART_INTERFACE_H_ */
