//=============================================================================
// Include file
//=============================================================================

#include "reuse.h"
#include "dd_siu_hwinfo.h"
#include "dd_flash_hwinfo.h"
#include "dd_swt_interface.h"
#include "dd_mmu_interface.h"
#include "dd_flash_interface.h"
#include "dd_xbar_interface.h"
#include "dd_flash_memory_interface.h"
#include "dd_intc_interface.h"
#include "dd_dma_interface.h"
#include "dd_mios_interface.h"
#include "dd_dspi_interface.h"
#include "dd_pit_interface.h"
#include "io_config_swt.h"
#include "io_config_dma.h"
#include "io_config_pit.h"
#include "hal_emulated_eeprom.h"
#include "dd_pit_interface.h"
#include "dd_l9958.h"
#include "dd_vsep_est_select.h"
#include "dd_stm.h"


/* private variable define */
static bool BatteryRemoved;

/* global variable define */
CPU_Info_T CPU_Info;
FLASH_MODULE_T Flash_Info;
HWIO_Reset_Status_T Reset_Status;

/* global variable reference */
//The address of these variables are the size numbers because these variables are calculated by the linker
extern char HWIO_DATA_ROM_START[];
extern char HWIO_DATA_RAM_START[];
extern char HWIO_BSS_START[];
extern uint16_t HWIO_DATA_SIZE;
extern uint16_t HWIO_BSS_SIZE;

/* private function define */
void CPU_DIAB_Set_Data_Area_Pointers(void);
 
void CPU_DIAB_Copy_Table( void )
{
   /* move initialized data from ROM to RAM if defined at	*/
   /* different locations					*/
   if( &HWIO_DATA_ROM_START[0] != &HWIO_DATA_RAM_START[0] ) 
   {
       Copy_Longs((void*)&HWIO_DATA_ROM_START[0],&HWIO_DATA_RAM_START[0],(((uint16_t)(&HWIO_DATA_SIZE)+3)/4));
   }
   /* clear uninitialized data				*/
   if (&HWIO_BSS_SIZE) 
   {
   // bss zero has been done in dd_start.s 
   // Clear_Longs(&HWIO_BSS_START[0],(((uint16_t)(&HWIO_BSS_SIZE)+3)/4));
   }
}

//=============================================================================
// CPU_DIAB_Set_Data_Area_Pointers
//=============================================================================

asm void CPU_DIAB_Set_Data_Area_Pointers(void)
{
   ;--------------------------------------------
   ; Clear r0 to use in following instructions.
   ;--------------------------------------------
    addis    r0,0,0

    addis    r13,r0,__SDA_BASE_@ha  # Initialize r13 to sdata base
    addi     r13,r13,__SDA_BASE_@l  # (provided by linker).
}


//=============================================================================
// Initialize all hardware related registers.  It is assumed that
// the processor has just come out of reset.
//=============================================================================
void InitializeHardwareRegisters(void)
{
	bool flash_init_sucess;

	FMPLL_Initialize_Device();

	// Set the watchdog timeout for 400ms during initialization
	SWT_Set_Timeout_Value(SWT_TIMEOUT_VALUE_INIT) ;
	SWT_Service_WatchDog();

	CPU_DIAB_Set_Data_Area_Pointers();
	CPU_DIAB_Copy_Table();


	CPU_Info =  SIU_Get_CPU_Information();
	Flash_Info = Get_FLASH_Driver_Mode();

	if(CPU_LCI == CPU_Info)
	{
		MPC5644_FLASH_Initialize_Normal();
		XBAR_MPC5644A_Initialize_Device();
		//flash_init_sucess = C90FL_Initialize();
	}
	else
	{
		MPC5634_FLASH_Initialize_Normal();
		XBAR_MPC5634M_Initialize_Device();
		// flash_init_sucess =	  C90LC_Initialize();
	}
	flash_init_sucess = flash_memory_interface->FLASH_Memory_Initial();

	Reset_Status = SIU_RESET_Get_Status();

	SIU_Initialize_Device();
	SIU_GPIO_Initialize_Device();

	INTC_Initialize_Device();

	STM_Initialize_Device();

	ECSM_Initialize_Device();

	DMA_Initialize_Device();

	DMA_Initialize_Channel(
	DMA_CHANNEL_QADC_FISR4_CFFF_4,
	DMA_QADC_CFFF_4_Source_Address,
	DMA_QADC_CFFF_4_Second_32Bit,
	DMA_QADC_CFFF_4_Third_32Bit,
	DMA_QADC_CFFF_4_Slast,
	DMA_QADC_CFFF_4_Dest_Address,
	DMA_QADC_CFFF_4_Sixth_32Bit,
	DMA_QADC_CFFF_4_Dlast,
	DMA_QADC_CFFF_4_Eighth_32Bit);


	DMA_Initialize_Channel(
	DMA_CHANNEL_QADC_FISR4_RFDF_4,
	DMA_QADC_RFDF_4_Source_Address,
	DMA_QADC_RFDF_4_Second_32Bit,
	DMA_QADC_RFDF_4_Third_32Bit,
	DMA_QADC_RFDF_4_Slast,
	DMA_QADC_RFDF_4_Dest_Address,
	DMA_QADC_RFDF_4_Sixth_32Bit,
	DMA_QADC_RFDF_4_Dlast,
	DMA_QADC_RFDF_4_Eighth_32Bit);

	QADC_Initialize_Device();
	PIT_Initialize_Device();

	//set up PIT time 5us
	PIT_TIMER_Set_Value( PIT_CHANNEL_1, PIT_LOAD_VALUE_20US);
	PIT_TIMER_Set_Value( PIT_CHANNEL_0, PIT_LOAD_VALUE_5US);

	//enable QADC DMA time base scan
	DMA_Enable_Request(DMA_CHANNEL_QADC_FISR4_RFDF_4);
	DMA_Enable_Request(DMA_CHANNEL_QADC_FISR4_CFFF_4);

	MIOS_Initialize_Device();

	DSPI_B_Initialize_Device();

	DMA_Initialize_Channel(
	DMA_CHANNEL_DSPI_B_SR_TFFF,
	DMA_DSPIB_SR_TFFF_Source_Address,
	DMA_DSPIB_SR_TFFF_Second_32Bit,
	DMA_DSPIB_SR_TFFF_Third_32Bit,
	DMA_DSPIB_SR_TFFF_Slast,
	DMA_DSPIB_SR_TFFF_Dest_Address,
	DMA_DSPIB_SR_TFFF_Sixth_32Bit,
	DMA_DSPIB_SR_TFFF_Dlast,
	DMA_DSPIB_SR_TFFF_Eighth_32Bit);

	DMA_Initialize_Channel(
	DMA_CHANNEL_DSPI_B_SR_RFDF,
	DMA_DSPIB_SR_RFDF_Source_Address,
	DMA_DSPIB_SR_RFDF_Second_32Bit,
	DMA_DSPIB_SR_RFDF_Third_32Bit,
	DMA_DSPIB_SR_RFDF_Slast,
	DMA_DSPIB_SR_RFDF_Dest_Address,
	DMA_DSPIB_SR_RFDF_Sixth_32Bit,
	DMA_DSPIB_SR_RFDF_Dlast,
	DMA_DSPIB_SR_RFDF_Eighth_32Bit);

	DSPI_B_Enable_Transfer(true);

	DMA_Initialize_Channel(
		DMA_CHANNEL_MIOS_EMIOSFLAG_4,
		DMA_EMIOSFLAG_4_Source_Address,
		DMA_EMIOSFLAG_4_Second_32Bit,
		DMA_EMIOSFLAG_4_Third_32Bit,
		DMA_EMIOSFLAG_4_Slast,
		DMA_EMIOSFLAG_4_Dest_Address,
		DMA_EMIOSFLAG_4_Sixth_32Bit,
		DMA_EMIOSFLAG_4_Dlast,
		DMA_EMIOSFLAG_4_Eighth_32Bit);

	STM_Set_Timer_Enable(true);

	QADC_ANALOG_Calibrate_Converter(ADC_CONVERTER_0);
	QADC_ANALOG_Calibrate_Converter(ADC_CONVERTER_1); 

	/* init can device and ccp can id and channel */
	FlexCAN_A_Initialize_Device();
	CCP_Initialize();
	HAL_CAN_Initialize();

	L9958_Device_Initialize();

	//Prepare Vsep clock
	IO_Pulse_VSEP_CLK_Enable();
	HAL_GPIO_SET_GEN_Enable(false);  // Enable FSE pin
	HAL_GPIO_SET_FSE_Enable(false);  // Enable the IOEN line to enable the IO pins
	VSEP_Initialize_Device();
}

//=============================================================================
// Refreshes all the hardware related registers.
//=============================================================================
void RefreshHardwareRegisters(void)
{
}


//=============================================================================
// This procedure is the very last to be called after processor
// reset.  It is assumed that the scheduler will be the next to run.
//=============================================================================
void InitializeHardwareLast(void)
{
	EEPROM_Operation_Status_T op_Return;

	HAL_GPIO_DI_Active_Status_Init();
	if(!HAL_GPIO_GET_Reset_DIO_Status()) {
		BatteryRemoved =  true;
	} else {
		BatteryRemoved =  false; 
	}

	HAL_GPIO_Reset_DIO_Output_Confige(true);

	// only LCI will do the instrumentation operation
	if(CPU_LCI == CPU_Info) {
		// resotre cal backup to ram
		INST_Restore_Working_Page(Reset_Status);
	}
	HAL_GPIO_SET_Reset_DIO_Enable(true);

	EEPROM_Restore_Vehicle_NVRAM_Block(Reset_Status);  
	op_Return = EEPROM_Restore_MFG_NVM_Block();  // restore Pfalsh MFG if it is valid
	INST_Initialize_Calibration_Pages();

	/* init tpu device, this part must placed behind nvram erase */
	TPU_Initialize_Device();
	InitializeComplexIO();
	FI_Initialize();
}


//=============================================================================
// Refreshes all the hardware related registers.
//=============================================================================
bool Get_BatteryRemove_Status(void)
{
	return BatteryRemoved;
}

