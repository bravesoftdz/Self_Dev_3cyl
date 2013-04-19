//=============================================================================
// include files
//=============================================================================
#include "HLS.h"
#include "hal_eng.h"


//=============================================================================
// global variable
//=============================================================================
uint8_t Startup_Counter;
bool     First_Syn_Flag;

//=============================================================================
// static variable
//=============================================================================
unsigned char Tooth_Interrupt_Flag;

//=============================================================================
//  IO_Eng_Cyl_Update
//=============================================================================
  void IO_Eng_Cyl_Update(void)
{

   LLD_cyl_num  = HAL_Eng_Get_Cyl_Number();
   crank_sig.engine_rpm = HAL_Eng_Get_Engine_Speed()/2;//rpm scale 0.125 to 0.25(chery) 
   crank_sig.segment_time = HAL_Eng_Get_Engine_Reference_Time()/4;// time base is 4M, convert to 1us
   crank_sig.crank_status.B_crank_loss_of_sync =  HAL_Eng_Get_Loss_Of_Sync();
   if(!First_Syn_Flag)
   {
         HLS_firstsyn();
   }		 
}

//=============================================================================
// IO_Eng_Engine_Stall_Reset
//=============================================================================
 void  IO_Eng_Engine_Stall_Reset(void) 
{

  First_Syn_Flag= 0;
  Startup_Counter = 0;
 Tooth_Interrupt_Flag = 0;

  crank_sig.crank_status.B_crank_stall = ( bitfield16_t )1; 
  crank_sig.crank_status.B_crank_sync = ( bitfield16_t )0 ;
  crank_sig.crank_status.B_crank_pre_sync = ( bitfield16_t )0 ;
  crank_sig.engine_rpm =0;
  crank_sig.segment_time = 0xFFFFFFFF;

}


//=============================================================================
// IO_Eng_Engine_Start_Crank
//=============================================================================
 void  IO_Eng_Engine_Start_Crank(void) 
{

  crank_sig.crank_status.B_crank_no_sync = ( bitfield16_t )0;
  crank_sig.crank_status.B_crank_stall = ( bitfield16_t )0;
  crank_sig.crank_status.B_crank_pre_sync  =( bitfield16_t )1;

}


//=============================================================================
// IO_Eng_Engine_First_Gap
//=============================================================================
 void  IO_Eng_Engine_First_Gap(void) 
{
  crank_sig.crank_status.B_crank_pre_sync  =( bitfield16_t )0;
  crank_sig.crank_status.B_crank_sync = ( bitfield16_t )1 ;

}

//=============================================================================
// IO_Eng_Engine_Init
//=============================================================================
 void  IO_Eng_Engine_Init(void) 
{
 crank_sig.crank_status.B_crank_no_sync = ( bitfield16_t )1; 

}


//=============================================================================
// IO_Eng_ToothInt
//=============================================================================
void  IO_Eng_ToothInt(void)
{
 //Chery requirement: Update the 32bit system time
  // Set the tooth interrupt flag to avoid the background update the system time 
 Tooth_Interrupt_Flag = 1;
 crank_sig.engine_position_tc = HAL_Eng_Get_Engine_Tooth()%60;
  Sys_time = HAL_Eng_Get_Sys_Timer()/80;
//Chery requirement: Call the HLS tooth task at every falling edge of crank tooth 
   HLS_tooth_interrupt();   
}

//=============================================================================
// IO_Eng_Update_System_Time_Background
//=============================================================================
 void IO_Eng_Update_System_Time_Background(void)
{
   if ( Tooth_Interrupt_Flag == false )
  {
    Sys_time = HAL_Eng_Get_Sys_Timer()/80;
  }

   if(crank_sig.crank_status.B_crank_pre_sync )
   {
     crank_sig.engine_rpm = 400; //100rpm
     crank_sig.segment_time = 300000;//100rpm,300ms
   }
   
}
