
/*************************************************************************/
/********************** SWC    : UART_private       **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 15, 2021       **********************/
/*************************************************************************/

#ifndef MCAL_UART_UART_PRIVATE_H_
#define MCAL_UART_UART_PRIVATE_H_

/*****************************************/
/*********** IM Register    *************/

#define UART_9BITIM         12
#define UART_OEIM           10
#define UART_BEIM           9
#define UART_PEIM           8
#define UART_FEIM           7
#define UART_RTIM           6
#define UART_TXIM           5
#define UART_RXIM           4
#define UART_CTSIM          1

/*****************************************/
/*********** MIS Register    *************/

#define UART_9BITMIS         12
#define UART_OEMIS           10
#define UART_BEMIS           9
#define UART_PEMIS           8
#define UART_FEMIS           7
#define UART_RTMIS           6
#define UART_TXMIS           5
#define UART_RXMIS           4
#define UART_CTSMIS          1

/*****************************************/
/*********** ICR Register    *************/

#define UART_9BITIC         12
#define UART_OEIC           10
#define UART_BEIC           9
#define UART_PEIC           8
#define UART_FEIC           7
#define UART_RTIC           6
#define UART_TXIC           5
#define UART_RXIC           4
#define UART_CTSIC          1

/*****************************************/
/*********** CTL Register    *************/

#define UART_CTSEN          15
#define UART_RTSEN          14
#define UART_RTS            11
#define UART_RXE            9
#define UART_TXE            8
#define UART_LPE            7
#define UART_HSE            5
#define UART_EOT            4
#define UART_SMART          3
#define UART_SIRLP          2
#define UART_SIREN          1
#define UART_EN             0
/*****************************************/

/*****************************************/
/*********** Flag Register    *************/

#define UART_TXFE           7
#define UART_RXFF           6
#define UART_TXFF           5
#define UART_RXFE           4
#define UART_BUSY           3
#define UART_CTS            0
/*****************************************/


#endif /* MCAL_UART_UART_PRIVATE_H_ */
