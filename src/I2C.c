// /*
// * I2C.C
// *
// *  Created on: 05-Feb-2019
// *      Author: AMOGH
// */
//
//#include "I2C.h"
//#include "log.h"
//#include <stdbool.h>
//
//
//#define slave_address 0x80 ;
//struct Possible_events event_set;     // Creating instance for the structure
//
//bool write, read;
//
//
///*******************************************************************************
//* Function Name: i2cinit
//********************************************************************************
//*
//* Summary:
//*  Initializes the I2C
//*
//* Parameters:
//*  Void
//*
//* Return:
//*  None.
//*
//* Reentrant:
//*  No.
//*
//* Reference: Self
//*
//*******************************************************************************/
//
//
//
//void i2cinit()
//{
//
//	I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;  //create an instance by the name i2cInit
//
//	i2cInit.sclPin = 10;                      //  Port C pin 10 selected as scl pin
//	i2cInit.sdaPin = 11;                      //  Port C pin 11 selected as sda pin
//	i2cInit.portLocationScl = 14;             //  Port location selected as 14 for scl
//	i2cInit.portLocationSda = 16;             //  Port location selected as 16 for scl
//
//
//
//	I2CSPM_Init(&i2cInit);                    // Turns on the clock for high frequency peripheral, Enable clock
//	                                          // Sends clock pulses to set slave in defined state
//	                                          // Calls another lower level I2C init function
//
//I2C_IntEnable(I2C0,I2C_IEN_RXDATAV | I2C_IEN_RXFULL | I2C_IEN_TXC );
//NVIC_EnableIRQ(I2C0_IRQn);
//
//}
//
//
///*******************************************************************************
//* Function Name: voidtransfer
//********************************************************************************
//*
//* Summary:
//*  This function enables the user to access the temperature sensor by passing the address, read the data from the temperature sensor
//*
//* Parameters:
//*  Void
//*
//* Return:
//*  None.
//*
//* Reentrant:
//*  No.
//*
//* Reference:
//*  Self
//*******************************************************************************/
//
////uint8_t command = 0xE3;                    // Initialize the variables
//uint8_t command = 0xE5;
//uint8_t read_tempData;
//uint16_t temperature_data;
//uint8_t temperature_degreecelcius;
//
//I2C_TransferReturn_TypeDef result;
//
//I2C_TransferSeq_TypeDef  sequence_write;        // Create instance for write sequence
//
//
//
//void temperature_calculation()
//{
//	temperature_data = 0;
//temperature_data |= (read_tempData << 8);         // Shift the MSB by 8 bits and store the MSB in higher byte
//temperature_data |= read_tempData;                // Take the LSB of the sensor data and store it in the buffer
////temperature_degreecelcius = (175.72* temperature_data/65536) - 46.85;  // Calculate the room temperature in degree celsius based on the formula from the user guide
//temperature_degreecelcius = (125* temperature_data/65536) - 6;
//LOG_INFO("%d", temperature_degreecelcius);        // Print the temperature
//}
//
//
//void i2cReadTransferInit(void)
//{
//
//	sequence_read.flags = I2C_FLAG_READ;              // Read the temperature from the temperature sensor
//	sequence_read.addr = slave_address;
//	sequence_read.buf[0].data = &read_tempData;       // Store the data from the temperature sensor in a buffer
//	sequence_read.buf[0].len = 2;                     // 2 bytes of data stored in the buffer
//    read = 1;
//	I2C_TransferInit(I2C0, &sequence_read);
//
//
//
//}
//
//void i2cWriteTransferInit(void)
//{
//
//	//I2C_TransferReturn_TypeDef status2;
//    sequence_write.flags = I2C_FLAG_WRITE;             // Read the temperature from the temperature sensor
//	sequence_write.addr = slave_address;
//	sequence_write.buf[0].data = &command;             // Store the data from the temperature sensor in a buffer
//	sequence_write.buf[0].len = 1;                     // 2 bytes of data stored in the buffer
//	write = 1;
//
//	I2C_TransferInit(I2C0, &sequence_write);
//}
//
//
//void I2C0_IRQHandler(void)               // Interrupt handler for I2C
//{
//	I2C_TransferReturn_TypeDef status = I2C_Transfer(I2C0);
//    if(status == i2cTransferDone)         // check status
//	{
//
//		//LOG_INFO ("I2C Transferr Complete");
//
//		if (write == 1){
//			event_set.write_transfer_done = 1;            // Set the event when write transfer is done
//			event_set.event_null = 0;
//			write = 0;
//		}
//
//		else if (read == 1){
//			event_set.read_transfer_done = 1;             // Set the event when read transfer is done
//			event_set.event_null = 0;
//			read = 0;
//		}
//	}
//
//	else if(status != i2cTransferInProgress)
//
//	{
//		event_set.event_null = 0;
//		LOG_INFO("I2C Transfer failed");
//	}
//
//}
















/*
 * I2C.C
 *
 *  Created on: 05-Feb-2019
 *      Author: AMOGH
 */

#include "I2C.h"
#include "log.h"
#include <stdbool.h>


//#define slave_address 0xB5
//#define slave_address 0xB6

uint8_t slave_address = 0xB5;
extern struct Possible_events event_set;     // Creating instance for the structure

bool write, read,sensor_status_check, application_write,sensor_mode, CO2_value_calculated, hardware_id_checked;


/*******************************************************************************
 * Function Name: i2cinit
 ********************************************************************************
 *
 * Summary:
 *  Initializes the I2C
 *
 * Parameters:
 *  Void
 *
 * Return:
 *  None.
 *
 * Reentrant:
 *  No.
 *
 * Reference: Self
 *
 *******************************************************************************/



void i2cinit()
{

	I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;  //create an instance by the name i2cInit

	i2cInit.sclPin = 10;                      //  Port C pin 10 selected as scl pin
	i2cInit.sdaPin = 11;                      //  Port C pin 11 selected as sda pin
	i2cInit.portLocationScl = 14;             //  Port location selected as 14 for scl
	i2cInit.portLocationSda = 16;             //  Port location selected as 16 for scl



	I2CSPM_Init(&i2cInit);                    // Turns on the clock for high frequency peripheral, Enable clock
	// Sends clock pulses to set slave in defined state
	// Calls another lower level I2C init function

	I2C_IntEnable(I2C0,I2C_IEN_RXDATAV | I2C_IEN_RXFULL | I2C_IEN_TXC );
	NVIC_EnableIRQ(I2C0_IRQn);

}


/*******************************************************************************
 * Function Name: voidtransfer
 ********************************************************************************
 *
 * Summary:
 *  This function enables the user to access the temperature sensor by passing the address, read the data from the temperature sensor
 *
 * Parameters:
 *  Void
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

//uint8_t command = 0xE3;                    // Initialize the variables
uint8_t status_register = 0x00;
uint8_t application_register = 0xF4;
uint8_t mode_register = 0x01;
uint8_t mode_register_val = 0x10;

uint8_t hardware_id_register = 0x20;
//uint8_t mode_register_val = 0x18;

uint8_t data_register = 0x02;




uint8_t read_tempData;
uint8_t receive_data_store[2];
uint8_t receive_hardware_id_store;


//uint8_t read_tempData;
uint8_t gas_sensor_status_reg;

uint16_t temperature_data;
uint8_t temperature_degreecelcius;

I2C_TransferReturn_TypeDef result;

I2C_TransferSeq_TypeDef sequence_write;        // Create instance for write sequence



//void temperature_calculation()
//{
//	temperature_data = 0;
//temperature_data |= (read_tempData << 8);         // Shift the MSB by 8 bits and store the MSB in higher byte
//temperature_data |= read_tempData;                // Take the LSB of the sensor data and store it in the buffer
////temperature_degreecelcius = (175.72* temperature_data/65536) - 46.85;  // Calculate the room temperature in degree celsius based on the formula from the user guide
//temperature_degreecelcius = (125* temperature_data/65536) - 6;
//LOG_INFO("%d", temperature_degreecelcius);        // Print the temperature
//}


//void i2cReadTransferInit(void)
//{
//
//	sequence_read.flags = I2C_FLAG_READ;              // Read the temperature from the temperature sensor
//	sequence_read.addr = slave_address;
//	sequence_read.buf[0].data = &read_tempData;       // Store the data from the temperature sensor in a buffer
//	sequence_read.buf[0].len = 2;                     // 2 bytes of data stored in the buffer
//    read = 1;
//	I2C_TransferInit(I2C0, &sequence_read);
//
//
//
//}


void hardware_id_check()
{
	sequence_write_hardware_id.flags = I2C_FLAG_WRITE_READ;
	sequence_write_hardware_id.addr = slave_address;
	sequence_write_hardware_id.buf[0].data = &hardware_id_register;             // Store the data from the temperature sensor in a buffer

	sequence_write_hardware_id.buf[1].data = &receive_hardware_id_store;
	//&receive_hardware_id_store = sequence_write_hardware_id.buf[1].data;
	                    // 2 bytes of data stored in the buffer
	sequence_write_hardware_id.buf[0].len = 1;
	sequence_write_hardware_id.buf[1].len = 1;

	LOG_INFO("ID : %d", (sequence_write_hardware_id.buf[1].data));
	LOG_INFO("ID : %d", *(sequence_write_hardware_id.buf[1].data));
	hardware_id_checked = 1;
	I2C_TransferInit(I2C0, &sequence_write_hardware_id);

	LOG_INFO("ID : %d", (sequence_write_hardware_id.buf[1].data));
		LOG_INFO("ID : %d", *(sequence_write_hardware_id.buf[1].data));


	//	LOG_INFO("ID : %d", (sequence_write_hardware_id.buf[1].data));

//	LOG_INFO("ID : %d", &receive_hardware_id_store);
//		I2C_TransferInit(I2C0, &sequence_write_hardware_id);
//		LOG_INFO("ID>>: %d", receive_hardware_id_store);
//		LOG_INFO("ID>> : %d", &receive_hardware_id_store);


		//LOG_INFO("ID : %d", sequence_write_hardware_id.buf[1].data);


//		uint8_t *temp = &receive_hardware_id_store;

		 if ( *(sequence_write_hardware_id.buf[1].data) == 129 )
		  {
			 LOG_INFO("DEVICE ID matches: %d", *(sequence_write_hardware_id.buf[1].data));
		//	hardware_id_checked = 1;
			LOG_INFO("hardware_id_checked>>> : %d", hardware_id_checked);
			//LOG_INFO("ID : %d", temp);
		  }
		 else
		 {
			 LOG_INFO("ERROR in ID");
		 }




}

void write_application()
{
	sequence_write_application.flags = I2C_FLAG_WRITE;
	sequence_write_application.addr = slave_address;
	sequence_write_application.buf[0].data = &application_register;             // Store the data from the temperature sensor in a buffer
	sequence_write_application.buf[0].len = 1;                     // 2 bytes of data stored in the buffer
	application_write = 1;

	I2C_TransferInit(I2C0, &sequence_write_application);

}

void sensor_mode_set(void)
{
	sequence_write_sensor_mode.flags = I2C_FLAG_WRITE_WRITE;
	sequence_write_sensor_mode.addr = slave_address;
	sequence_write_sensor_mode.buf[0].data = &mode_register;             // Store the data from the temperature sensor in a buffer
	sequence_write_sensor_mode.buf[1].data = &mode_register_val;
	sequence_write_sensor_mode.buf[0].len = 1;                     // 2 bytes of data stored in the buffer
	sequence_write_sensor_mode.buf[1].len = 1;
	sensor_mode = 1;

	I2C_TransferInit(I2C0, &sequence_write_sensor_mode);
}


void CO2_value_calculation(void)
{
	sequence_write_calculation.flags = I2C_FLAG_WRITE_READ;
	sequence_write_calculation.addr = slave_address;
	sequence_write_calculation.buf[0].data = &data_register;             // Store the data from the temperature sensor in a buffer
	sequence_write_calculation.buf[1].data = &receive_data_store;
	sequence_write_calculation.buf[0].len = 1;                     // 2 bytes of data stored in the buffer
	sequence_write_calculation.buf[1].len = 2;
	CO2_value_calculated = 1;

	I2C_TransferInit(I2C0, &sequence_write_calculation);



	LOG_INFO("CO2_value_calculation: %d", *(sequence_write_calculation.buf[1].data));
	temperature_data = 0;
	//LOG_INFO("%d", receive_data_store);
	temperature_data |= ((receive_data_store[1]) << 8);         // Shift the MSB by 8 bits and store the MSB in higher byte
	temperature_data |= (receive_data_store[0]);
	LOG_INFO("%d", temperature_data);


}

//void External_sensor_status_check(void)
//{
//	sequence_write_initialization.flags = I2C_FLAG_WRITE_READ;
//	sequence_write_initialization.addr = slave_address;
//	sequence_write_initialization.buf[0].data = &status_register ;
//	sequence_write_initialization.buf[1].data = &gas_sensor_status_reg;
//
//
//	//&gas_sensor_status_reg = sequence_write_initialization.buf[1].data;
//	sequence_write_initialization.buf[0].len = 1;
//	sequence_write_initialization.buf[1].len = 1;
//
//
//
//
//
//	sensor_status_check = 1;
//
//	I2C_TransferInit(I2C0, &sequence_write_initialization);
//
////uint8_t tempval = gas_sensor_status_reg ;
////
////
////	LOG_INFO("%d", tempval);
//
//
//
//
//}
//



//void External_sensor_init(void)
//{
//	sequence_write_initialization.flags = I2C_FLAG_WRITE_READ;
//	sequence_write_initialization.addr = slave_address;
//	sequence_write_initialization.buf[0].data = &status_register;
//	sequence_write_initialization.buf[1].data = &gas_sensor_status_reg;
//	sensor_status_check = 1;
//
//	I2C_TransferInit(I2C0, &sequence_write_initialization);
//}

//void i2cWriteTransferInit(void)
//{
//
//	//I2C_TransferReturn_TypeDef status2;
//    sequence_write.flags = I2C_FLAG_WRITE;             // Read the temperature from the temperature sensor
//	sequence_write.addr = slave_address;
//	sequence_write.buf[0].data = &command;             // Store the data from the temperature sensor in a buffer
//	sequence_write.buf[0].len = 1;                     // 2 bytes of data stored in the buffer
//	write = 1;
//
//	I2C_TransferInit(I2C0, &sequence_write);
//}

void I2C0_IRQHandler(void)               // Interrupt handler for I2C
{
	I2C_TransferReturn_TypeDef status = I2C_Transfer(I2C0);
	if(status == i2cTransferDone)         // check status
	{

		LOG_INFO ("I2C Transfer Complete");

		if (write == 1){
			event_set.write_transfer_done = 1;            // Set the event when write transfer is done
			event_set.event_null = 0;
			write = 0;
		}

		 if (read == 1){
			event_set.read_transfer_done = 1;             // Set the event when read transfer is done
			event_set.event_null = 0;
			read = 0;
		}

		 if (sensor_status_check == 1)
		{
			event_set.sensor_status = 1;             // Set the event when read transfer is done
			event_set.event_null = 0;
			sensor_status_check = 0;
		}


		 if (application_write == 1)
		{
			event_set.application_upload = 1;             // Set the event when read transfer is done
			event_set.event_null = 0;
			application_write = 0;
		}

		 if (sensor_mode == 1)
		{
			event_set.sensor_mode_set = 1;             // Set the event when read transfer is done
			event_set.event_null = 0;
			sensor_mode = 0;
		}

		 if (CO2_value_calculated == 1)
		{
			event_set.value_calculated = 1;             // Set the event when read transfer is done
			event_set.event_null = 0;
			CO2_value_calculated = 0;
		}

		 if (hardware_id_checked == 1)
		{

			LOG_INFO("In Hardware ID check");
			event_set.hardware_id_pass = 1;             // Set the event when read transfer is done
			event_set.event_null = 0;
			hardware_id_checked = 0;
		}
	}

	else if(status != i2cTransferInProgress)

	{
		event_set.event_null = 0;
		LOG_INFO("I2C error ");
	}

}
