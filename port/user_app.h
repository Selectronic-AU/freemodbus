#ifndef USER_MODBUS_SLAVER_H
#define USER_MODBUS_SLAVER_H

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define DISCRETE_INPUT_START        1
#define DISCRETE_INPUT_NDISCRETES   16
#define COIL_START                  1
#define COIL_NCOILS                 64
#define REG_INPUT_START             1
#define REG_INPUT_NREGS             100
#define REG_HOLDING_START           1
#define REG_HOLDING_NREGS           100

//===========================婓悵厥敵湔笢ㄛ跪跺華硊勤茼腔髡夔隅砱========================
#define          HD_RESERVE                     0       //悵隱
#define          HD_CPU_USAGE_MAJOR             1       //絞CPU瞳蚚薹腔淕杅弇
#define          HD_CPU_USAGE_MINOR             2       //絞CPU瞳蚚薹腔苤杅弇

//===========================婓怀敵湔笢ㄛ跪跺華硊勤茼腔髡夔隅砱========================
#define          IN_RESERVE                     0       //悵隱

//===========================婓盄笢ㄛ跪跺華硊勤茼腔髡夔隅砱============================
#define          CO_RESERVE                     2       //悵隱

//===========================婓燭汃怀笢ㄛ跪跺華硊勤茼腔髡夔隅砱=========================
#define          DI_RESERVE                     1       //悵隱

/*--------------------------Extern Functions------------------------------------*/
extern UCHAR    xMBUtilGetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits );
extern void     xMBUtilSetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits, UCHAR ucValue );

#endif
