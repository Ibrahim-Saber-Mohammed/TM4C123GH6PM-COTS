/*************************************************************************/
/********************** SWC    : I2C                **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 27, 2021       **********************/
/*************************************************************************/
#ifndef MCAL_I2C_PRIVATE_H_
#define MCAL_I2C_PRIVATE_H_

#define MAX_NUMBER_SLAVES       127

#define HS          4
#define ACK         3
#define STOP        2
#define START       1
#define RUN         0

#define CLKTO       7
#define BUSBUSY     6
#define IDLE        5
#define ARBLST      4
#define DATACK      3
#define ADRACK      2
#define ERROR       1
#define BUSY        0

#define IM          0
#define CLKIM       1

#define GFE         6

#define OAR2SEL     3
#define FBR         2
#define TREQ        1
#define RREQ        0

#define DA          0

#endif /* MCAL_I2C_PRIVATE_H_ */
