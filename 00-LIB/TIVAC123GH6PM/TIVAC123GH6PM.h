/*************************************************************************/
/********************** SWC    : TIVAC123GH6PM.h    **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 22, 2021       **********************/
/*************************************************************************/

#ifndef LIB_TIVAC123GH6PM_TIVAC123GH6PM_H_
#define LIB_TIVAC123GH6PM_TIVAC123GH6PM_H_

/**********************************************************************/
        /*************      UART REGISTERS  *************/
/**********************************************************************/
#define UART0_BASE_ADDRESS          0x4000C000
#define UART1_BASE_ADDRESS          0x4000D000
#define UART2_BASE_ADDRESS          0x4000E000
#define UART3_BASE_ADDRESS          0x4000F000
#define UART4_BASE_ADDRESS          0x40010000
#define UART5_BASE_ADDRESS          0x40011000
#define UART6_BASE_ADDRESS          0x40012000
#define UART7_BASE_ADDRESS          0x40013000

typedef struct {

    volatile u32    DR              ;
    volatile u32    RSR_ECR         ;
    volatile u32    FR              ;
    volatile u32    ILPR            ;
    volatile u32    IBRD            ;
    volatile u32    FBRD            ;
    volatile u32    LCRH            ;
    volatile u32    CTL             ;
    volatile u32    IFLS            ;
    volatile u32    IM              ;
    volatile u32    RIS             ;
    volatile u32    MIS             ;
    volatile u32    ICR             ;
    volatile u32    DMACTL          ;
    volatile u32    Reserved1[22]   ;
    volatile u32    BIT_9_ADDR      ;
    volatile u32    BIT_9_AMASK     ;
    volatile u32    Reserved2[965]  ;
    volatile u32    PP              ;
    volatile u32    Reserved3       ;
    volatile u32    CC              ;
    volatile u32    Reserved4       ;
    volatile u32    PeriphID_4_7[4] ;
    volatile u32    PeriphID_0_3[4] ;
    volatile u32    PCellID_0_3[4]  ;

}UART_REGISTER_t;

#define UART0                  ( ( UART_REGISTER_t *  ) UART0_BASE_ADDRESS)
#define UART1                  ( ( UART_REGISTER_t *  ) UART1_BASE_ADDRESS)
#define UART2                  ( ( UART_REGISTER_t *  ) UART2_BASE_ADDRESS)
#define UART3                  ( ( UART_REGISTER_t *  ) UART3_BASE_ADDRESS)
#define UART4                  ( ( UART_REGISTER_t *  ) UART4_BASE_ADDRESS)
#define UART5                  ( ( UART_REGISTER_t *  ) UART5_BASE_ADDRESS)
#define UART6                  ( ( UART_REGISTER_t *  ) UART6_BASE_ADDRESS)
#define UART7                  ( ( UART_REGISTER_t *  ) UART7_BASE_ADDRESS)

/**********************************************************************/

/**********************************************************************/
        /*************      SSI REGISTERS  *************/
/**********************************************************************/
#define SSI0_BASE_ADDRESS           0x40008000
#define SSI1_BASE_ADDRESS           0x40009000
#define SSI2_BASE_ADDRESS           0x4000A000
#define SSI3_BASE_ADDRESS           0x4000B000

typedef struct {
    volatile u32 CR0    ;
    volatile u32 CR1    ;
    volatile u32 DR     ;
    volatile u32 SR     ;
    volatile u32 CPSR   ;
    volatile u32 IM     ;
    volatile u32 RIS    ;
    volatile u32 MIS    ;
    volatile u32 ICR    ;
    volatile u32 DMACTL ;
    volatile u32 Reserved1[1000]  ;
    volatile u32 CC               ;
    volatile u32 Reserved2        ;
    volatile u32 PeriphID_4_7[4]  ;
    volatile u32 PeriphID_0_3[4]  ;
    volatile u32 PCellID_0_3[4]   ;
}SSI_REGISTER_t;

#define SSI0               ( (SSI_REGISTER_t *) SSI0_BASE_ADDRESS )
#define SSI1               ( (SSI_REGISTER_t *) SSI1_BASE_ADDRESS )
#define SSI2               ( (SSI_REGISTER_t *) SSI2_BASE_ADDRESS )
#define SSI3               ( (SSI_REGISTER_t *) SSI3_BASE_ADDRESS )

/**********************************************************************/

/**********************************************************************/
        /*************      I2C REGISTERS  *************/
/**********************************************************************/

#define I2C0_BASE_ADDRESS       0x40020000
#define I2C1_BASE_ADDRESS       0x40021000
#define I2C2_BASE_ADDRESS       0x40022000
#define I2C3_BASE_ADDRESS       0x40023000


typedef struct{

/********************** Master Registers **********************/
    volatile u32 MCA            ;
    volatile u32 MCS            ;
    volatile u32 MDR            ;
    volatile u32 MTPR           ;
    volatile u32 MIMR           ;
    volatile u32 MRIS           ;
    volatile u32 MMIS           ;
    volatile u32 MICR           ;
    volatile u32 MCR            ;
    volatile u32 MCLKOCNT       ;
    volatile u32 Reserved1      ;
    volatile u32 MBMON          ;
    volatile u32 Reserved2[2]   ;
    volatile u32 MCR2           ;
    volatile u32 Reserved3[497] ;
/********************** Slave Registers **********************/
    volatile u32 SOAR           ;
    volatile u32 SCSR           ;
    volatile u32 SDR            ;
    volatile u32 SIMR           ;
    volatile u32 SRIS           ;
    volatile u32 SMIS           ;
    volatile u32 SICR           ;
    volatile u32 SOAR2          ;
    volatile u32 SACKCTL        ;
    volatile u32 Reserved4[487] ;
    volatile u32 PP             ;
    volatile u32 PC             ;

}I2C_REGISTERS_t;

#define   I2C0                 ( (I2C_REGISTERS_t * ) I2C0_BASE_ADDRESS)
#define   I2C1                 ( (I2C_REGISTERS_t * ) I2C1_BASE_ADDRESS)
#define   I2C2                 ( (I2C_REGISTERS_t * ) I2C2_BASE_ADDRESS)
#define   I2C3                 ( (I2C_REGISTERS_t * ) I2C3_BASE_ADDRESS)

#endif /* LIB_TIVAC123GH6PM_TIVAC123GH6PM_H_ */
