#ifndef DD_SIU_INTERFACE_H
#define DD_SIU_INTERFACE_H

//=============================================================================
//                                Includes
//=============================================================================

#include "dd_siu.h"

//=============================================================================
//                                extern init  function
//=============================================================================
extern void SIU_Initialize_Device(void );

//=============================================================================
//                                extern init  function
//=============================================================================
extern void SIU_GPIO_Initialize_Device( void );

//=============================================================================
// SIU_GPIO_DISCRETE_Set_State
//=============================================================================
extern void SIU_GPIO_DISCRETE_Set_State(
   SIU_GPIO_Channel_T channel,
   bool                 in_state );

//=============================================================================
// SIU_GPIO_DISCRETE_Get_State_Local
//=============================================================================
extern bool SIU_GPIO_DISCRETE_Get_State(SIU_GPIO_Channel_T channel );

//=============================================================================
// SIU_GPIO_DISCRETE_Toggle_State
//=============================================================================
extern void SIU_GPIO_DISCRETE_Toggle_State(SIU_GPIO_Channel_T channel );
//=============================================================================
// SIU_GPIO_DISCRETE_Get_State_Local
//=============================================================================
 void SIU_GPIO_Output_Confige(SIU_GPIO_Channel_T channel, bool enable );
 
//=============================================================================
// SIU_GPIO_OpenDrain_Confige
//=============================================================================
 void SIU_GPIO_OpenDrain_Confige(SIU_GPIO_Channel_T channel, bool enable );
 
//=============================================================================
// SIU_GPIO_InputBuffer_Config
//=============================================================================
 void SIU_GPIO_InputBuffer_Config(SIU_GPIO_Channel_T channel, bool enable );

//=============================================================================
// SIU_Pad_Config
//=============================================================================
void SIU_Pad_Config(SIU_GPIO_Channel_T channel, SIU_GPIO_Pin_Assignment_T type );

//=============================================================================
// SIU_Knock_Connect_Pin_To_ETPU
//=============================================================================
 void SIU_Knock_Connect_Pin_To_ETPU(bool state, SIU_GPIO_Channel_T siu_channel);

#endif // DD_SIU_INTERFACE_H

