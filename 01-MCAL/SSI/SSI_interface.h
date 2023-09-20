/*************************************************************************/
/********************** SWC    : SSI_interface      **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 22, 2021       **********************/
/*************************************************************************/

#ifndef MCAL_SSI_SSI_INTERFACE_H_
#define MCAL_SSI_SSI_INTERFACE_H_


typedef enum{
    SSI_0   ,
    SSI_1   ,
    SSI_2   ,
    SSI_3
}SSI_ChannelID_t;

typedef enum{
    SSI_MASTER = ( 0b0 << 2),
    SSI_SLAVE  = ( 0b1 << 2 )
}SSI_MASTER_SLAVE_t;

typedef enum{
    SSI_SYSTEM_CLK  = 0x0,
    SSI_PIOSC_CLK   = 0x5
}SSI_ClockSource_t;
typedef enum{
    SSI_FOUR_BITS   = 0x3   ,
    SSI_FIVE_BITS   = 0x4   ,
    SSI_SIX_BITS    = 0x5   ,
    SSI_SEVEN_BITS  = 0x6   ,
    SSI_EIGTH_BITS  = 0x7   ,
    SSI_NINE_BITS   = 0x8   ,
    SSI_TEN_BITS    = 0x9   ,
    SSI_ELEVEN_BITS = 0xA   ,
    SSI_TWELVE_BITS = 0xB    ,
    SSI_THIRTEEN_BITS = 0xC  ,
    SSI_FOURTEEN_BITS = 0xD  ,
    SSI_FIFTEEN_BITS  = 0xE  ,
    SSI_SIXTEEN_BITS  = 0xF
}SSI_DataSize_t;

typedef enum{
    SPI_FREESCALE                = ( 0b00 << 4 ) ,
    TI_Synchronous_Serial_Frame  = ( 0b01 << 4 ) ,
    MICROWIRE_Frame              = ( 0b10 << 4 )
}SSI_Frame_Select_t;

typedef enum{
    SPI_LEADING_RISING,
    SPI_LEADING_FALLING
}SSI_ClockPolarity_t;

typedef enum{
    SPI_LEADING_SAMPLING,
    SPI_LEADING_SETUP
}SSI_ClockPhase_t;

typedef struct {
    SSI_ChannelID_t     SSI_ID          ;
    SSI_MASTER_SLAVE_t  SSI_M_S_Select  ;
    SSI_DataSize_t      SSI_Data        ;
    SSI_Frame_Select_t  SSI_Frame       ;
    SSI_ClockPhase_t    SSI_CLKPH       ;
    SSI_ClockPolarity_t SSI_CLKPOL      ;
    SSI_ClockSource_t   SSI_CLK         ;
    u8 SSI_SeriealClockRate             ;
    u8 SSI_ClockPrescaleDivisor         ;
}SSI_MasterConfigs_t;

typedef struct {
    SSI_ChannelID_t     SSI_ID          ;
    SSI_MASTER_SLAVE_t  SSI_M_S_Select  ;
    SSI_DataSize_t      SSI_Data        ;
    SSI_Frame_Select_t  SSI_Frame       ;
    SSI_ClockPhase_t    SSI_CLKPH       ;
    SSI_ClockPolarity_t SSI_CLKPOL      ;
}SSI_SlaveConfigs_t;

void SSI_voidInit(void);
u8 SSI_u8SetMasterConfigurations(const SSI_MasterConfigs_t * Copy_structSSIConfigurations);

u8 SSI_u8SetSlaveConfigurations(const SSI_SlaveConfigs_t * Copy_structSSIConfigurations);
void SSI_voidTransmit(SSI_ChannelID_t Copy_enumChannelID, u16 Copy_u16DataTransmit);
u8 SSI_voidReceive(SSI_ChannelID_t Copy_enumChannelID, u16 * Copy_ptrDataReceivedBuffer);

#endif /* MCAL_SSI_SSI_INTERFACE_H_ */
