/*
 * io_cards_handler.h
 *
 *  Created on: Jun 10, 2022
 *      Author: cagdas
 */

#ifndef INC_IO_CARDS_HANDLER_H_
#define INC_IO_CARDS_HANDLER_H_

#include "T9_RIO_Registers.h"

#define MB_ADDRESS_BROADCAST (0) /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN (1)		 /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX (247)	 /*! Biggest possible slave address. */


#define QUERY_RETRY_COUNT (1)




#define MAX_NUM_OF_ANALOG_CHANNELS 8
extern const char *SlotTypesStr[];
enum
{
	RIO_INPUT_PT100,
	RIO_INPUT_CURRENT,
	RIO_INPUT_VOLTAGE
};

enum
{
	RIO_OUTPUT_CURRENT,
	RIO_OUTPUT_VOLTAGE
};

enum
{
	HIGH_IMPEDANCE,
	VOLTAGE_OUTPUT,
	CURRENT_OUTPUT,
	VOLTAGE_INPUT,
	CURRENT_INPUT,
	RESISTANCE_INPUT,
	PT100_INPUT,
	PT1000_INPUT
};

enum
{
	CARD_NOT_ENUMERATED,
	CARD_ENUMERATED
};

typedef enum
{
	ENUMERATE_IO_CARDS,
	GET_IO_CARD_TYPES,
	READ_WRITE_IO_CARDS
} IO_Cards_Comm_State;

#define NUM_OF_PWM_CHANNELS (6)

#define AUTHENTICATION_PASSWORD_SIZE 9

#define DI_RESERVED_SIZE 14
#define DO_RESERVED_SIZE 14
#define AIO_RESERVED_SIZE 21
#define MIO_RESERVED_SIZE 22

enum
{
	SLOT_FIRMWARE_VERSION_REG,
	SLOT_HARDWARE_VERSION_REG,
	SLOT_SERIAL_NO_REG,
	SAVE_HW_INFO_REG =  SLOT_SERIAL_NO_REG + 8,
	SLOT_SLAVE_ADDRESS,
	SLOT_ENUMERATED_REG,
	LEFT_SLOT_ADDRESS,
	RIGHT_SLOT_ADDRESS,
	TRIGGER_RIGHT_SLOT_ENUMERATION,
	ALL_SLOTS_GOT_ENUMERATED_REG,
	RE_ENUMERATE_REG,
	FIRMWARE_UPDATE_REG,
	SLOT_TYPE_REG,
	DISABLE_ALL_OUTPUTS_REG,
	ENABLE_COUNTERS_REG,
	CNT_PRESET_BASE_REG,
	CNT_RESET_FLAGS_REG = (CNT_PRESET_BASE_REG + 2 * NUM_OF_COUNTERS),
	CNT_PRESET_FLAGS_REG,
	ENABLE_ENCODERS_REG,
	ENC_PRESET_BASE_REG,
	ENC_RESET_FLAGS_REG = (ENC_PRESET_BASE_REG + 2 * NUM_OF_ENCODERS),
	ENC_PRESET_FLAGS_REG,
	DIGITAL_INPUTS_REG,
	CNT_BASE_REG,
	ENC_BASE_REG = (CNT_BASE_REG + 2 * NUM_OF_COUNTERS),
	ENC_DIRECTION_FLAGS_REG = (ENC_BASE_REG + 2 * NUM_OF_ENCODERS),
	DI_RESERVED_REG,
	DI_AUTHENTICATION_PASSWORD = DI_RESERVED_REG + DI_RESERVED_SIZE,

	ENABLE_PWM_CHANNELS_REG = DISABLE_ALL_OUTPUTS_REG + 1,
	PWM_MODE_REG,
	DIGITAL_OUTPUTS_REG,
	PWM_FREQUENCY_REG_BASE,
	PWM_DUTY_REG_BASE = PWM_FREQUENCY_REG_BASE + NUM_OF_PWM_CHANNELS,
	FAULT_REG = PWM_DUTY_REG_BASE + NUM_OF_PWM_CHANNELS,
	DO_RESERVED_REG,
	DO_AUTHENTICATION_PASSWORD = DO_RESERVED_REG + DO_RESERVED_SIZE,


	 
	CHANNEL_1_4_FUNCTION_REG = DISABLE_ALL_OUTPUTS_REG + 1,
	CHANNEL_5_8_FUNCTION_REG,
	ADC_EN_50_60_HZ_REG,
	CHANNEL1_OUT_REG,
	CHANNEL2_OUT_REG = CHANNEL1_OUT_REG + 2,
	CHANNEL3_OUT_REG = CHANNEL2_OUT_REG + 2,
	CHANNEL4_OUT_REG = CHANNEL3_OUT_REG + 2,
	CHANNEL5_OUT_REG = CHANNEL4_OUT_REG + 2,
	CHANNEL6_OUT_REG = CHANNEL5_OUT_REG + 2,
	CHANNEL7_OUT_REG = CHANNEL6_OUT_REG + 2,
	CHANNEL8_OUT_REG = CHANNEL7_OUT_REG + 2,
	CHANNEL1_IN_REG = CHANNEL8_OUT_REG + 2,
	CHANNEL2_IN_REG = CHANNEL1_IN_REG + 2,
	CHANNEL3_IN_REG = CHANNEL2_IN_REG + 2,
	CHANNEL4_IN_REG = CHANNEL3_IN_REG + 2,
	CHANNEL5_IN_REG = CHANNEL4_IN_REG + 2,
	CHANNEL6_IN_REG = CHANNEL5_IN_REG + 2,
	CHANNEL7_IN_REG = CHANNEL6_IN_REG + 2,
	CHANNEL8_IN_REG = CHANNEL7_IN_REG + 2,
	OPEN_CIRCUIT_FLAGS_REG = CHANNEL8_IN_REG + 2,
	SHORT_CIRCUIT_FLAGS_REG,
	CH1_4_INTERNAL_ERROR_FLAG_REG,
	CH5_8_INTERNAL_ERROR_FLAG_REG,
	AIO_RESERVED_REG,
	AIO_AUTHENTICATION_PASSWORD = AIO_RESERVED_REG + AIO_RESERVED_SIZE,
	AIO_CALIBRATION_TYPE = AIO_AUTHENTICATION_PASSWORD + AUTHENTICATION_PASSWORD_SIZE,
	AIO_CALIBRATION_COMMANND_TYPE_REG,
	AIO_TRIGGER_CALIBRATION_COMMANND_REG,
	AIO_CALIBRATION_VALUE_BASE_REG,


	MIO_DIGITAL_OUTPUTS_REG = DISABLE_ALL_OUTPUTS_REG + 1,
	MIO_ENABLE_COUNTERS_REG,
	MIO_CNT_PRESET_BASE_REG,
	MIO_CNT_RESET_FLAGS_REG = (MIO_CNT_PRESET_BASE_REG + 2 * NUM_OF_MIO_COUNTERS),
	MIO_CNT_PRESET_FLAGS_REG,
	MIO_ENABLE_ENCODERS_REG,
	MIO_ENC_PRESET_BASE_REG,
	MIO_ENC_RESET_FLAGS_REG = (MIO_ENC_PRESET_BASE_REG + 2 * NUM_OF_MIO_ENCODERS),
	MIO_ENC_PRESET_FLAGS_REG,
	MIO_ANALOG_CHANNELS_TYPE_REG,
	MIO_DIGITAL_INPUTS_REG,
	MIO_CNT_BASE_REG,
	MIO_ENC_BASE_REG = (MIO_CNT_BASE_REG + 2 * NUM_OF_MIO_COUNTERS),
	MIO_ENC_DIRECTION_FLAGS_REG = (MIO_ENC_BASE_REG + 2 * NUM_OF_MIO_ENCODERS),
	MIO_CHANNEL1_IN_REG,
	MIO_CHANNEL2_IN_REG = MIO_CHANNEL1_IN_REG + 2,
	MIO_RESERVED_REG  = MIO_CHANNEL2_IN_REG + 2,
	MIO_AUTHENTICATION_PASSWORD = MIO_RESERVED_REG + MIO_RESERVED_SIZE,
	/*MIO Calibration Params*/
	MIO_CALIBRATION_TYPE_REG = MIO_AUTHENTICATION_PASSWORD + AUTHENTICATION_PASSWORD_SIZE,
	MIO_CALIBRATION_COMMANND_TYPE_REG,
	MIO_TRIGGER_CALIBRATION_COMMANND_REG,
	MIO_CALIBRATION_VALUE_BASE_REG,


};

typedef enum Slot_Types
{
	SLOT_UNKNOWN,
	SLOT_TYPE_DI8N,
	SLOT_TYPE_DI16N,
	SLOT_TYPE_ENC4,
	SLOT_TYPE_CNT8,
	SLOT_TYPE_DO8P,
	SLOT_TYPE_DO16P,
	SLOT_TYPE_AI4,
	SLOT_TYPE_AI8,
	SLOT_TYPE_AO4,
	SLOT_TYPE_AO8,
	SLOT_TYPE_AIO4,
	SLOT_TYPE_AIO8,
	SLOT_TYPE_DI8P,
	SLOT_TYPE_MIO442,
	SLOT_TYPE_DI16P,
	SLOT_TYPE_DO8N,
	SLOT_TYPE_DO8R,
	SLOT_TYPE_DO16N,
	SLOT_TYPE_DO16R,
	SLOT_TYPE_AI4T,
	SLOT_TYPE_AI8T,
	SLOT_TYPE_AIO4T,
	SLOT_TYPE_AIO8T,
	TOTAL_SLOT_TYPES
} SlotTypes;

typedef struct
{
	bool enumeration_completed;
	uint8_t slot_not_found_counter;
	uint8_t current_slot;
	uint8_t final_slot;
	IO_Cards_Comm_State IO_CardsCommState;
	uint8_t slot_types[MAX_NUM_OF_SLOTS];
	uint8_t slot_comm_fail_counter[MAX_NUM_OF_SLOTS];
} IO_Cards_Struct;

#endif /* INC_IO_CARDS_HANDLER_H_ */
