#include "mg_hal_config.h"
#include "mg_hal.h"
#include "mg_hal_discrete.h"
#include "mg_hal_timer.h"
#include "dd_vsep_pwm_interface.h"
#include "dd_mios_interface.h"
#include "io_config_tpu.h"
#include "dd_mcd5402_interface.h"
#include "dd_swt_interface.h"

/*=============================================================================
 * mg_HAL_PWM_Set_Discrete_Out_Group_Frequency_And_Duty
 * @func  set discrete out group frequency and duty
 * @parm  frequency, duty
 * @rdesc none 
 *===========================================================================*/
void mg_HAL_PWM_Set_Discrete_Out_Group_Frequency_And_Duty(uint8_t index)
{
    if (MG_HIODEVICE_DO_NULL != MG_HAL_DISCRETE_OUT_GROUP[index].io)
    {
        //period in us
        //duty resoultion is 1/1000
        MIOS_PWM_Set_Period_And_DutyCycle_US(MG_HAL_DISCRETE_OUT_GROUP[index].io,
                                             (uint32_t)(1000 * 1000)/MG_HAL_DISCRETE_OUT_GROUP[index].frequency,
                                             (uint32_t)MG_HAL_DISCRETE_OUT_GROUP[index].duty*1000/100);
    }
}

/*=============================================================================
 * mg_HAL_PWM_Set_Discrete_Out_Group_Frequency_And_Duty_Immediate
 * @func  set discrete out group frequency and duty immediately
 * @parm  frequency, duty
 * @rdesc none 
 *===========================================================================*/
void mg_HAL_PWM_Set_Discrete_Out_Group_Frequency_And_Duty_Immediate(uint8_t index)
{
    if (MG_HIODEVICE_DO_NULL != MG_HAL_DISCRETE_OUT_GROUP[index].io)
    {
        /* convert hz and duty(0-100) to vsep pwm module accuracy */
        /* VSEP channel: period resoultion is 1/64ms; duty resoultion is 1/32768 */
        VSEP_PWM_PWMSetPeriodImmediate(MG_HAL_DISCRETE_OUT_GROUP[index].io, (1000 * 64)/MG_HAL_DISCRETE_OUT_GROUP[index].frequency);
        VSEP_PWM_PWMSetDutyImmediate(MG_HAL_DISCRETE_OUT_GROUP[index].io, (MG_HAL_DISCRETE_OUT_GROUP[index].duty * 32768)/100);
    }
}

/*=============================================================================
 * mg_HAL_PWM_Set_ETCCTLPWM_Frequency_And_Duty
 * @func  set ETCCTLPWM frequency and duty
 * @parm  frequency, duty
 * @rdesc none 
 *===========================================================================*/
void mg_HAL_PWM_Set_ETCCTLPWM_Frequency_And_Duty(uint32_t frequency, uint32_t duty)
{
    MIOS_PWM_Set_Period_And_DutyCycle_US(MIOS_ETCCTLPWM_CH, (uint32_t)(1000 * 1000)/frequency, duty*1000/100);
    // IO_PULSE_PWM_Set_Frequency_And_Duty_Cycle(&MTSA_PWM_ETCCTLPWM, frequency, 0, duty, 100);
}

/*=============================================================================
 * mg_HAL_PWM_Set_Fuel_Pump_High_Frequency_And_Duty
 * @func  set fuel pump frequency and duty
 * @parm  frequency, duty
 * @rdesc none 
 *===========================================================================*/
// void mg_HAL_PWM_Set_Fuel_Pump_High_Frequency_And_Duty(uint32_t frequency, uint32_t duty)
// {
//     IO_PULSE_PWM_Set_Frequency_And_Duty_Cycle(&MTSA_PWM_FP_IN, frequency, 0, duty, 100);
// }

/*=============================================================================
 * mg_HAL_PWM_Set_SWT_Timer
 * @func  set SWT value
 * @parm  time
 * @rdesc none 
 *===========================================================================*/
void mg_HAL_PWM_Set_SWT_Timer(uint32_t time)
{
    SWT_Set_Timeout_Value(time * 8);
}

/*=============================================================================
 * mg_HAL_PWM_EMC_Cycling_Initialize
 * @func  initialize EMC cycling PWM
 * @parm  time
 * @rdesc none 
 *===========================================================================*/
// void mg_HAL_PWM_EMC_Cycling_Initialize(void)
// {
//     IO_PULSE_PWM_Initialize_Channel(&MG_MTSA_PWM_FUEL_CONSUM);
// }

/*=============================================================================
 * mg_HAL_PWM_Validation_Cycling_Initialize
 * @func  initialize validation cycling PWM
 * @parm  time
 * @rdesc none 
 *===========================================================================*/
// void mg_HAL_PWM_Validation_Cycling_Initialize(void)
// {
//     IO_PULSE_PWM_Initialize_Channel(&MG_MTSA_PWM_FUEL_CONSUM);
// }

/*=============================================================================
 * mg_HAL_PWM_Get_Frequency_In_Group_Period
 * @func  get frequency in froup period
 * @parm  index
 * @rdesc period 
 *===========================================================================*/
uint32_t mg_HAL_PWM_Get_Frequency_In_Group_Period(uint8_t index)
{
    PWM_Data_T PWM_Data;
    if (MG_HIODEVICE_FI_NULL != MG_HAL_FREQUENCY_IN_GROUP[index].io)
    {
         PWM_Data.Period = MCD5402_Get_Period(PWMIE_TPU_INDEX, MG_TPU_CONFIG_IC_PWMIE[MG_HAL_FREQUENCY_IN_GROUP[index].io]);
    } else {
         PWM_Data.Period = 0;
    }
    return (PWM_Data.Period >> 2);
}

/*=============================================================================
 * mg_HAL_PWM_Get_ETCCTLPWM_Frequency
 * @func  get frequency ETCCTLPWM
 * @parm  none
 * @rdesc period 
 *===========================================================================*/
// uint32_t mg_HAL_PWM_Get_ETCCTLPWM_Frequency(void)
// {
// #ifdef __MG_PHDL_FAULT_DIAGNOSTIC_TEST
//     return IO_PULSE_IC_Get_Frequency((HIODEVICE)&MTSA_IC_ETCCTLPWMFB, 0);
// #else
//     return 0;
// #endif
// }

/*=============================================================================
 * mg_HAL_PWM_Discrete_Out_Cycling
 * @func  cycling discrete out according to the initialzation parameters
 * @parm  none
 * @rdesc none 
 *===========================================================================*/
void mg_HAL_PWM_Discrete_Out_Cycling(void)
{
    uint8_t idx;
    uint32_t time_diff;

    for (idx = 0; idx < MG_Number_of_Discrete_Out_Device; idx++) 
    {
        if (MG_HIODEVICE_DO_NULL != MG_HAL_DISCRETE_OUT_GROUP[idx].io) 
        {
            if (DISCRETE == MG_HAL_DISCRETE_OUT_GROUP[idx].type)
            {
                if ((0 != MG_HAL_DISCRETE_OUT_GROUP[idx].on_time_us) && (0!=MG_HAL_DISCRETE_OUT_GROUP[idx].off_time_us)) 
                {
                    MG_HAL_DISCRETE_OUT_GROUP[idx].timer.current_time = mg_HAL_Timer_Get_STM_In_CNT();

                    time_diff = mg_HAL_Timer_Get_STM_Diff_In_CNT(MG_HAL_DISCRETE_OUT_GROUP[idx].timer.current_time,MG_HAL_DISCRETE_OUT_GROUP[idx].timer.start_time);

                    switch(MG_HAL_DISCRETE_OUT_GROUP[idx].current_state)
                    {
                        case true:
                            if (time_diff >= MG_HAL_DISCRETE_OUT_GROUP[idx].on_time_us)
                            {
                                mg_HAL_Discrete_Set_Discrete_Out_Group_Value(idx, false);
                                MG_HAL_DISCRETE_OUT_GROUP[idx].timer.start_time = MG_HAL_DISCRETE_OUT_GROUP[idx].timer.current_time;
                                MG_HAL_DISCRETE_OUT_GROUP[idx].current_state = false;
                            }
                            break;
                        case false:
                            if (time_diff >= MG_HAL_DISCRETE_OUT_GROUP[idx].off_time_us)
                            {
                                mg_HAL_Discrete_Set_Discrete_Out_Group_Value(idx, true);
                                MG_HAL_DISCRETE_OUT_GROUP[idx].timer.start_time = MG_HAL_DISCRETE_OUT_GROUP[idx].timer.current_time;
                                MG_HAL_DISCRETE_OUT_GROUP[idx].current_state = true;
                            }
                            break;
                        default:
                            break;
                    }
                }
                else if (0 == MG_HAL_DISCRETE_OUT_GROUP[idx].on_time_us)
                {
                    mg_HAL_Discrete_Set_Discrete_Out_Group_Value(idx, false);
                }
                else if (0 == MG_HAL_DISCRETE_OUT_GROUP[idx].off_time_us)
                {
                    mg_HAL_Discrete_Set_Discrete_Out_Group_Value(idx, true);
                }
                else
                {
                    /* do nothing */
                }
            }
            else
            {
                /* do nothing */
            }
        }
        mg_HAL_Service_WatchDog();
    }
}
