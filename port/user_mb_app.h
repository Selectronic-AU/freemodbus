#ifndef USER_APP
#define USER_APP

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"

/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START        1
#define S_DISCRETE_INPUT_NDISCRETES   16
#define S_COIL_START                  1
#define S_COIL_NCOILS                 64
#define S_REG_INPUT_START             1
#define S_REG_INPUT_NREGS             100
#define S_REG_HOLDING_START           1
#define S_REG_HOLDING_NREGS           100
//植儂耀宒ㄩ婓悵厥敵湔笢ㄛ跪跺華硊勤茼腔髡夔隅砱
#define          S_HD_RESERVE                     0     //悵隱
#define          S_HD_CPU_USAGE_MAJOR             1     //絞CPU瞳蚚薹腔淕杅弇
#define          S_HD_CPU_USAGE_MINOR             2     //絞CPU瞳蚚薹腔苤杅弇

//植儂耀宒ㄩ婓怀敵湔笢ㄛ跪跺華硊勤茼腔髡夔隅砱
#define          S_IN_RESERVE                     0     //悵隱

//植儂耀宒ㄩ婓盄笢ㄛ跪跺華硊勤茼腔髡夔隅砱
#define          S_CO_RESERVE                     2     //悵隱

//植儂耀宒ㄩ婓燭汃怀笢ㄛ跪跺華硊勤茼腔髡夔隅砱
#define          S_DI_RESERVE                     1     //悵隱

/* -----------------------Master Defines -------------------------------------*/
#define M_DISCRETE_INPUT_START        1
#define M_DISCRETE_INPUT_NDISCRETES   16
#define M_COIL_START                  1
#define M_COIL_NCOILS                 64
#define M_REG_INPUT_START             1
#define M_REG_INPUT_NREGS             100
#define M_REG_HOLDING_START           1
#define M_REG_HOLDING_NREGS           100
//翋儂耀宒ㄩ婓悵厥敵湔笢ㄛ跪跺華硊勤茼腔髡夔隅砱
#define          M_HD_RESERVE                     0     //悵隱

//翋儂耀宒ㄩ婓怀敵湔笢ㄛ跪跺華硊勤茼腔髡夔隅砱
#define          M_IN_RESERVE                     0     //悵隱

//翋儂耀宒ㄩ婓盄笢ㄛ跪跺華硊勤茼腔髡夔隅砱
#define          M_CO_RESERVE                     2     //悵隱

//翋儂耀宒ㄩ婓燭汃怀笢ㄛ跪跺華硊勤茼腔髡夔隅砱
#define          M_DI_RESERVE                     1     //悵隱

#endif
