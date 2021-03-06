#ifndef VEHICLE_CAN_H
#define VEHICLE_CAN_H

//=============================================================================
// include files
//=============================================================================
#include "reuse.h"

//=============================================================================
// extern variable
//=============================================================================
extern const bool K_Can_Meter_MIL_Disable;
extern const bool K_Can_Meter_SVS_Disable ;
extern const bool K_Can_Meter_TACH_Disable;
extern const bool K_Can_Meter_Fuel_Consum_Disable;

/* can msg enable or disable calibration switch */
extern const bool KbHwio_CheryCanID0FA_Enable;
extern const bool KbHwio_CheryCanID101_Enable;
extern const bool KbHwio_CheryCanID270_Enable;
extern const bool KbHwio_CheryCanID278_Enable;
extern const bool KbHwio_CheryCanID2C1_Enable;
extern const bool KbHwio_CheryCanID370_Enable;
extern const bool KbHwio_CheryCanID378_Enable;
extern const bool KbHwio_CheryCanID501_Enable;
extern const bool KbHwio_CheryCanID623_Enable;
extern const bool KbHwio_CheryCanID7C1_Enable;

/* ecu receive cam msg from other vehicle ecus */
extern const bool KbHwio_CheryCanID1AE_Enable;
extern const bool KbHwio_CheryCanID2E9_Enable;
extern const bool KbHwio_CheryCanID310_Enable;
extern const bool KbHwio_CheryCanID3C0_Enable;
extern const bool KbHwio_CheryCanID430_Enable;
extern const bool KbHwio_CheryCanID300_Enable;
extern const bool KbHwio_CheryCanID51A_Enable;
#endif

