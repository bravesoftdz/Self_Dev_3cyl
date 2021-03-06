#ifndef DD_CRANK_INTERFACE_H
#define DD_CRANK_INTERFACE_H

#include "dd_crank.h"


//=============================================================================
// CRANK_EngineStall_PerioCheck
//=============================================================================

void CRANK_EngineStall_PerioCheck(void);

//=============================================================================
// CRANK_SCHEDULER_Assign_Event
//=============================================================================
bool CRANK_SCHEDULER_Assign_Event( 
   Crank_Event_Id_T  in_event_id, 
   uCrank_Angle_T    in_first_event_angle, 
   uCrank_Angle_T    in_event_angle_offset, 
   uint32_t          in_number_of_events );

//=============================================================================
// CRANK_SCHEDULER_Remove_Event
//=============================================================================
void CRANK_SCHEDULER_Remove_Event( 
   Crank_Event_Id_T  in_event_id, 
   uCrank_Angle_T    in_first_event_angle, 
   uCrank_Angle_T    in_event_angle_offset, 
   uint32_t          in_number_of_events );

//=============================================================================
// CRANK_SCHEDULER_Assign_Handler_To_Event_ID
//=============================================================================
void CRANK_SCHEDULER_Assign_Handler_To_Event_ID(
   Crank_Event_Id_T  in_event_id,
   IO_Callback_T     in_callback );
//=============================================================================
//
//  FUNCTION: CRANK_High_Priority_Cylinder_Event
//=============================================================================
void CRANK_High_Priority_Cylinder_Event( void );

//=============================================================================
// CRANK_Get_Cylinder_ID
//=============================================================================
Crank_Cylinder_T CRANK_Get_Cylinder_ID( void );

//=============================================================================
// CRANK_Get_Engine_Speed
//=============================================================================
uint32_t CRANK_Get_Engine_Speed( void );

//=============================================================================
// CRANK_Get_Engine_Reference_Time
//=============================================================================
uint32_t CRANK_Get_Engine_Reference_Time( void );
//=============================================================================
// CRANK_Get_Loss_Of_Sync
//=============================================================================
bool CRANK_Get_Loss_Of_Sync( void );

//=============================================================================
// CRANK_Get_Engine_Tooth
//=============================================================================
uCrank_Count_T CRANK_Get_Engine_Tooth( void );
//=============================================================================
// CRANK_Get_First_Sync_Set
//=============================================================================
bool CRANK_Get_First_Sync_Set( void );

//=============================================================================
// CRANK_Get_Diag_Tooth_Cnt, for crank status diagnose
//=============================================================================
uint8_t CRANK_Get_Diag_Tooth_Cnt(void);

//=============================================================================
// CRANK_Set_Diag_Tooth_Cnt, for crank status diagnose
//=============================================================================
void CRANK_Set_Diag_Tooth_Cnt(uint8_t cnt);

//=============================================================================
// Re set the VR1 filter at run engine state 
//=============================================================================
 void  InitHWIO_CrankToRun(void);
//=============================================================================
//  Re set the VR1 filter at crank engine state 
//=============================================================================
 void  InitHWIO_RunToCrank(void);
//=============================================================================
//  Re set the VR1 filter at crank engine state 
//=============================================================================
void InitHWIO_ZeroEngSpd(void);
//=============================================================================
//  Re set the VR1 filter at crank engine state 
//=============================================================================
void InitHWIO_KeyonToCrank(void);
//=============================================================================
// CRANK_Get_Tooth_Period
//=============================================================================
uint32_t  CRANK_Get_Tooth_Period( void );

//=============================================================================
// CRANK_Set_Current_Event_Tooth
//=============================================================================
void CRANK_Set_Current_Event_Tooth(uCrank_Count_T tooth_number);

//=============================================================================
// CRANK EVENT : Synchronization Error Recovery. .
//
// Try to find a correct gap after a missed sync.
//=============================================================================
void CRANK_Recover_From_Synch_Error( void );

bool CRANK_Check_Real_Gap_In_Backup_Mode( void );

void CRANK_Backup_Set_Syn_Status(void);

uint32_t IO_PULSE_Convert_Counts_To_Time(
   uint32_t    counts_per_time,
   uint32_t    in_count,
   uint8_t     in_time_precision,
   uint8_t     in_time_resolution );

uint8_t CRANK_Get_Diag58x_Error_Cnt(void);
void    CRANK_Set_Diag58x_Error_Cnt(uint8_t cnt);
#endif // DD_CRANK_INTERFACE_H
