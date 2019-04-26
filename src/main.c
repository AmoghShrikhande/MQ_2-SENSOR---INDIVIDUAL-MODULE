/***************************************************************************************
 * License[]
 ***************************************************************************************/

/**
 * @file      Load Power Management
 * @author    Amogh Shrikhande
 * @brief     File contains Load Power Management
 * @date      February 6, 2019
 **/

/***************************************************************************************
 *                          HEADERS & MACROS                                            *
 ***************************************************************************************/
/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include "em_gpio.h"
#include "em_device.h"
#include "em_chip.h"
#include "gpio.h"
#include "sleep.h"
#include "Low_energy_sleep.h"
#include "Low_energy_timer.h"
#include "I2C.h"
#include "log.h"
#include "em_letimer.h"
#include "em_core.h"
#include "Event_handler.h"


#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
		.config_flags = 0,
		.sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
		.bluetooth.max_connections = MAX_CONNECTIONS,
		.bluetooth.heap = bluetooth_stack_heap,
		.bluetooth.heap_size = sizeof(bluetooth_stack_heap),
		.bluetooth.sleep_clock_accuracy = 100, // ppm
		.gattdb = &bg_gattdb_data,
		.ota.flags = 0,
		.ota.device_name_len = 3,
		.ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
		.pa.config_enable = 1, // Enable high power PA
		.pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};


void i2cWriteTransferInit(void);
void i2cReadTransferInit(void);

struct Possible_events event_set = {0};

uint32_t div1;
uint32_t mode_count_load ;

enum temperature_I2C_states{
	Power_off,
	Wait_for_Power_Up,
	Device_correct,
	Application_uploaded,
	I2C_ready,
    Measurement_mode_configured,
	Status_check,
	Calculated_CO2_value,
	Wait_for_I2C_Write_Complete,
	Wait_for_I2C_Read_Complete,

	//Error_State
};

enum temperature_I2C_states current_state = Power_off;
enum temperature_I2C_states next_state;




/*******************************************************************************
 * Function Name: timerWaitUs
 ********************************************************************************
 *
 * Summary:
 *  This function enables the user to create a required delay
 *
 * Parameters:
 *  uint32_t us_wait
 *
 * Return:
 *  None.
 *
 * Reentrant:
 *  No.
 *
 * Reference:
 *  Self
 *******************************************************************************/

void timerWaitUs(uint32_t us_wait)

{
	if (( SLEEP_MODE == 0) || (SLEEP_MODE == 1) || (SLEEP_MODE == 2))
	{
		div1 = us_wait / 61.035;
		mode_count_load = 49152;
	}
	if (SLEEP_MODE == 3)
	{
		div1 = us_wait / 1000 ;
		mode_count_load = 3002;
	}
	//uint32_t div1 = us_wait*div_freq ;               // divide user input by 61.035 to get the number of ticks
	uint32_t base = LETIMER_CounterGet(LETIMER0);   // Get the current count stored in the count register in base variable
	uint32_t base1;
	uint32_t base2;
	if (base < div1)                                 // If the required count is greater than
	{
		base1 = mode_count_load + ( base - div1);

		//LETIMER_CounterGet(LETIMER0);
		LETIMER_CompareSet(LETIMER0, 1, base1);
	}

	else
	{
		base2 = base - div1;
		//current_count1 = LETIMER_CounterGet(LETIMER0);
		LETIMER_CompareSet(LETIMER0, 1, base2);
	}

}


uint32_t timer_log_stamp;

/*******************************************************************************
 * Function Name: main
 ********************************************************************************
 *
 * Summary:
 *  This function enables the user to create a required delay
 *
 * Parameters:
 *  void
 *
 * Return:
 *  int
 *
 * Reentrant:
 *  No.
 *
 * Reference:
 *  Self
 *******************************************************************************/


int main(void)
{
	// Initialize device
	initMcu();
	// Initialize board
	initBoard();
	// Initialize application
	initApp();

	gpioInit();

	logInit();

	// Initialize stack
	gecko_init(&config);

	leTimer_config();

	LETIMER_Enable(LETIMER0, true);             // Enable Letimer0


	sleep_config();                             // Configure the sleep routines

	i2cinit();                                  // Initialize the I2C and make it ready for transfer


	//SLEEP_SleepBlockBegin(SLEEP_MODE+1); // Comment out in EM3

	/* Infinite loop */
	while(1){                           // State machine starts here

		switch (current_state){             // Switch for current state
		case Power_off:                     // Case 1, the board is in power off state
			if(event_set.timer_UF == 1)     // Waiting for the Underflow flag event, that is delay of 3 sec
			{
				event_set.timer_UF = 0;     // Clear the event flag
				event_set.event_null = 1;

				// External_sensor_init();

				GPIO_PinOutSet (gpioPortC,10);  // Enable the I2C port pins scl and sda
				GPIO_PinOutSet (gpioPortC,11);
				timerWaitUs(19000);         // time in 80 ms

				LOG_INFO("IN Power_off CASE");
				next_state = Wait_for_Power_Up; // Next state
			}
			break;

			//		  case Wait_for_Power_Up:              // Case 2, the board is waiting for power up
			//			  if(event_set.timer_comp1 == 1 )
			//			  {
			//				  event_set.timer_comp1 = 0;    // Clear the comp1 flag
			//				  event_set.event_null = 1;
			//				  SLEEP_SleepBlockBegin(sleepEM2); // Put the board in EM0 or EM1 mode to enable I2C trnsfer
			// 				  i2cWriteTransferInit();       // Make setup to ready transfer
			// 				  next_state = Wait_for_I2C_Write_Complete; // define next state
			//			  }
			//			  break;


			//		case Wait_for_Power_Up:              // Case 2, the board is waiting for power up
			//			if(event_set.timer_comp1 == 1 )
			//			{
			//				event_set.timer_comp1 = 0;    // Clear the comp1 flag
			//				event_set.event_null = 1;
			//				// SLEEP_SleepBlockBegin(sleepEM2); // Put the board in EM0 or EM1 mode to enable I2C trnsfer
			//				// External_sensor_status_check();       // Make setup to ready transfer
			//				write_application();
			//				LOG_INFO("IN Wait_for_Power_Up CASE");
			//				next_state = Application_uploaded; // define next state
			//			}
			//			break;


		case Wait_for_Power_Up:              // Case 2, the board is waiting for power up
			if(event_set.timer_comp1 == 1 )
			{
				event_set.timer_comp1 = 0;    // Clear the comp1 flag
				event_set.event_null = 1;
				// SLEEP_SleepBlockBegin(sleepEM2); // Put the board in EM0 or EM1 mode to enable I2C trnsfer
				// External_sensor_status_check();       // Make setup to ready transfer
				hardware_id_check();
				LOG_INFO("IN Wait_for_Power_Up CASE");
				next_state = Device_correct; // define next state
			}
			break;

		case Device_correct:        // Case 2, the board is waiting for power up
			LOG_INFO("In Device_correct before if");
			LOG_INFO(" (event_set.hardware_id_pass  = %d",event_set.hardware_id_pass );

			if(event_set.hardware_id_pass == 1 )
			{
				event_set.hardware_id_pass = 0;    // Clear the comp1 flag
				event_set.event_null = 1;
				// SLEEP_SleepBlockBegin(sleepEM2); // Put the board in EM0 or EM1 mode to enable I2C trnsfer
				// External_sensor_status_check();       // Make setup to ready transfer
				write_application();
				LOG_INFO("IN Device_correct CASE");
				next_state = Application_uploaded; // define next state
			}
			break;

		case Application_uploaded:        // Case 2, the board is waiting for power up
			if(event_set.application_upload == 1 )
			{
				event_set.application_upload = 0;    // Clear the comp1 flag
				event_set.event_null = 1;
				// SLEEP_SleepBlockBegin(sleepEM2); // Put the board in EM0 or EM1 mode to enable I2C trnsfer
				// External_sensor_status_check();       // Make setup to ready transfer
				timerWaitUs(1300);
				LOG_INFO("IN Application_uploaded CASE");
				next_state = I2C_ready; // define next state
			}
			break;


		case I2C_ready:              // Case 2, the board is waiting for power up
			if(event_set.timer_comp1 == 1 )
			{
				event_set.timer_comp1 = 0;    // Clear the comp1 flag
				event_set.event_null = 1;
				// SLEEP_SleepBlockBegin(sleepEM2); // Put the board in EM0 or EM1 mode to enable I2C trnsfer
				// External_sensor_status_check();       // Make setup to ready transfer
				sensor_mode_set();
				LOG_INFO("IN I2C_ready CASE");
				next_state = Measurement_mode_configured; // define next state
			}
			break;
		case Measurement_mode_configured:              // Case 2, the board is waiting for power up
			if(event_set.sensor_mode_set == 1 )
			{
				event_set.sensor_mode_set = 0;    // Clear the comp1 flag
				event_set.event_null = 1;
				// SLEEP_SleepBlockBegin(sleepEM2); // Put the board in EM0 or EM1 mode to enable I2C trnsfer
				// External_sensor_status_check();       // Make setup to ready transfer
				CO2_value_calculation();
				LOG_INFO("IN Measurement_mode_configured CASE");
				next_state = Power_off; // define next state
			}
			break;






			//		   case Status_check:              // Case 2, the board is waiting for power up
			//					  if(event_set.sensor_status == 1 )
			//					  {
			//						  event_set.sensor_status = 0;    // Clear the comp1 flag
			//						  event_set.event_null = 1;
			//						 SLEEP_SleepBlockBegin(sleepEM2); // Put the board in EM0 or EM1 mode to enable I2C trnsfer
			//						 External_sensor_status_check();       // Make setup to ready transfer
			//
			//		 				  next_state = Wait_for_I2C_Write_Complete; // define next state
			//					  }
			//					  break;
			//



			//	      case Wait_for_I2C_Write_Complete:     // Case 3, I2C write completed
			//	    	  if(event_set.write_transfer_done == 1) // Check if the event has occured
			//	    	  {
			//	    		  event_set.write_transfer_done = 0; // Clear the event flag
			//	    		  event_set.event_null = 1;
			//	    		  i2cReadTransferInit();             // Start to read data from temperature sensor
			//	    		  next_state = Wait_for_I2C_Read_Complete; // define next state
			//              }
			//	    	  break;
			//
			//		  case Wait_for_I2C_Read_Complete:       // Case 4, I2C read complete state
			//			  if(event_set.read_transfer_done == 1)   // Check for the event
			//			  {
			//				  event_set.read_transfer_done = 0;  // Clear the event
			//				  event_set.event_null = 1;
			//
			//				 SLEEP_SleepBlockEnd(sleepEM2);     // Sleep in higher energy modes to save power
			//				  temperature_calculation();         // Calculate the temperature
			//				  GPIO_PinOutClear (gpioPortD,15);   // Disable temperature sensor
			//				  GPIO_PinOutClear (gpioPortC,10);   // Disable I2C pin
			//			      GPIO_PinOutClear (gpioPortC,11);
			//				 // temperature_calculation();         // Calculate the temperature
			//				  next_state = Power_off;
			//              }
		}

		// Changing states
		if(current_state != next_state)           // Check if state transition has taken place
		{
			// timer_log_stamp = loggerGetTimestamp();
			LOG_INFO("%d",timer_log_stamp);
			LOG_INFO("State transferred from %d to %d", current_state, next_state);
			current_state = next_state;           // Set the current state to next state
		}


		//		if (event_set.event_null == 1 && SLEEP_MODE > sleepEM0)  // Sleep if currently no event is executed
		//		{
		//			SLEEP_Sleep();
		//		}


	}
}
