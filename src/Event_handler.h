/*
 * Event_handler.h
 *
 *  Created on: 13-Feb-2019
 *      Author: AMOGH
 */

#ifndef SRC_EVENT_HANDLER_H_
#define SRC_EVENT_HANDLER_H_

#include <stdbool.h>


// Structure of possible events which can cause transition in states
struct Possible_events{
	bool timer_UF;                    // This event occurs when an underflow is reached
	bool timer_comp1;                 // This event occurs when the required delay for device power on is met
	bool write_transfer_done;         // This event occurs when I2C successfully writes the slave address and the command to the sensor
	bool read_transfer_done;          // This event is set when the raw value from the temperature sensor is obtained via I2C
	bool event_error;                 // This event occurs when none of the above events are met
	bool sensor_status;
	bool application_upload;
	bool sensor_mode_set;
	bool timer_comp1_I2C_ready;
	bool value_calculated;
	bool hardware_id_pass;
	bool event_null;


	};

#endif /* SRC_EVENT_HANDLER_H_ */
