#include <stdint.h>
#ifndef INC_T9_RIO_REGISTERS_H_
#define INC_T9_RIO_REGISTERS_H_

#ifndef T9_RMT

#define LENGTH_OF_BLE_COMPLETE_LOCAL_NAME						  20
#define LENGTH_OF_BLE_SECURITY_KEY								  6

#define LENGTH_OF_COMMUNICATION_NETWORK_SSID					  32
#define LENGTH_OF_COMMUNICATION_NETWORK_PASSWORD				  64 // 16
#define LENGTH_OF_COMMUNICATION_NETWORK_IP				  		  4
#define LENGTH_OF_COMMUNICATION_NETWORK_SUBNET				  	  4
#define LENGTH_OF_COMMUNICATION_NETWORK_ROUTER				  	  4

#define DEFAULT_WIRELESS_COMM_TECHNOLOGY				0 /* off */
#define DEFAULT_BLE_COMPLETE_LOCAL_NAME					"T9-RIO-XXXXXXXX"
#define DEFAULT_BLE_SECURITY_KEY						"123456"
#define DEFAULT_WIFI_MODE								0 /* access point */
#define DEFAULT_NETWORK_SSID							"T9-RIO-XXXXXXXX"
#define DEFAULT_NETWORK_PASSWORD						"12345678"
#define DEFAULT_IP_CONFIGURATION						0 /* manual */

typedef struct _COMMUNICATION_SETTINGS_
{
	uint16_t	eWirelessCommunicationType;
	char		BLECompleteLocalName[LENGTH_OF_BLE_COMPLETE_LOCAL_NAME];
	char		BLESecurityKey[LENGTH_OF_BLE_SECURITY_KEY];
	uint16_t	eWifiMode;
	char		ssid[LENGTH_OF_COMMUNICATION_NETWORK_SSID];
	char		password[LENGTH_OF_COMMUNICATION_NETWORK_PASSWORD];
	uint16_t 	eIpConfiguration;
	uint16_t	ipAddress[LENGTH_OF_COMMUNICATION_NETWORK_IP];
	uint16_t	subnetAddress[LENGTH_OF_COMMUNICATION_NETWORK_SUBNET];
	uint16_t	router[LENGTH_OF_COMMUNICATION_NETWORK_ROUTER];
}COMMUNICATION_SETTINGS, *pCOMMUNICATION_SETTINGS;

#endif

#ifndef T9_RMT
#define DEFAULT_SERIAL_NUMBER							"RIO-12345678"
#else
#define DEFAULT_SERIAL_NUMBER							"RMT-12345678"
#endif

typedef struct _HW_INFO_SETTINGS_
{
	uint16_t mac_address5_4;
	uint16_t mac_address3_2;
	uint16_t mac_address1_0;
	struct
	{
		uint8_t minor_version;
		uint8_t major_version;
	}hardware_version;
	char	serialNumber[16];
}HW_INFO_SETTINGS, *pHW_INFO_SETTINGS;

typedef enum
{
	MANUAL,
	DHCP
}ip_configuration_t;

typedef enum
{
	AP,
	STA
}wifi_modes_t;

typedef enum
{
	off = 0,
	bluetooth,
	wifi
}wireless_comm_selection_t;

enum
{
	CNT1,
	CNT2,
	CNT3,
	CNT4,
	NUM_OF_MIO_COUNTERS,
	CNT5 = NUM_OF_MIO_COUNTERS,
	CNT6,
	CNT7,
	CNT8,
	NUM_OF_COUNTERS
};

enum
{
	ENC1,
	ENC2,
	NUM_OF_MIO_ENCODERS,
	ENC3 = NUM_OF_MIO_ENCODERS,
	ENC4,
	NUM_OF_ENCODERS
};

enum
{
	PWM_CHANNEL_1,
	PWM_CHANNEL_2,
	PWM_CHANNEL_3,
	PWM_CHANNEL_4,
	PWM_CHANNEL_5,
	PWM_CHANNEL_6,
	NUM_OF_PWM_CHANNELS
};
enum
{
	AIO_OK,
	AIO_SHORT_CIRCUIT,
	AIO_OPEN_CIRCUIT,
	AIO_INTERNAL_ERROR
};

enum
{
	NO_ACTION,
	RE_ENUMERATE,
	SAVE_MODBUS_SETTINGS = RE_ENUMERATE,
	RETURN_TO_FACTORY_SETTINGS = SAVE_MODBUS_SETTINGS,
	EXPERIENCED_POWER_ON = RETURN_TO_FACTORY_SETTINGS,
	SAVE_HW_INFO = EXPERIENCED_POWER_ON
};

enum
{
	DISABLE_OUTPUTS,
	ENABLE_OUTPUTS
};

enum
{
	BAUD_1200,
	BAUD_2400,
	BAUD_4800,
	BAUD_9600,
	BAUD_14400,
	BAUD_19200,
	BAUD_38400,
	BAUD_57600,
	BAUD_115200
};

enum
{
	PARITY_NONE,
	PARITY_ODD,
	PARITY_EVEN
};

#define MAX_NUM_OF_MIO_ADC_CHANNELS 2
#define MAX_NUM_OF_ADC_CHANNELS 8
#ifndef T9_RMT
#define MAX_NUM_OF_SLOTS 32
#else
#define MAX_NUM_OF_SLOTS 1
#endif

#define MAX_NUM_OF_ADC_OR_DAC_CHANNELS 8
#define MAX_NUM_OF_SLOTS 32 
#pragma pack (1)
typedef struct {
	struct
	{
		uint8_t minor_version;
		uint8_t major_version;
	}firmware_version;
	struct
	{
		uint8_t minor_version;
		uint8_t major_version;
	}hardware_version;
	char	serialNumber[16];
	uint16_t slot_save_hw_info;
	uint16_t slot_slave_address;
	uint16_t slot_enumerated;
	uint16_t left_slot_address;
	uint16_t right_slot_address;
	uint16_t trigger_right_slot_enumeration;
	uint16_t all_slots_got_enumerated;
	uint16_t reenumerate;	
	uint16_t firmware_update_request;
	uint16_t slot_type;
	uint16_t disable_all_outputs;
}slot_info_registers;
typedef	struct
	{
		uint32_t ch1: 4;
		uint32_t ch2: 4;
		uint32_t ch3: 4;
		uint32_t ch4: 4;
		uint32_t ch5: 4;
		uint32_t ch6: 4;
		uint32_t ch7: 4;
		uint32_t ch8: 4;
	}ch_functions;
typedef union{
	uint32_t value_32bit;
	uint16_t value_16bit[2];
	ch_functions channel_functions;
}u_ch_functions;
typedef struct
{
	slot_info_registers di_slot_info;
	uint16_t enable_counters;
	uint16_t enable_frequency_mode;
	uint32_t cnt_preset_regs[NUM_OF_COUNTERS];
	uint16_t cnt_reset_flags;
	uint16_t cnt_preset_flags;
	uint16_t enable_encoders;
	uint32_t enc_preset_regs[NUM_OF_ENCODERS];
	uint16_t enc_reset_flags;
	uint16_t enc_preset_flags;
	uint16_t digital_inputs;
	uint32_t cnt[NUM_OF_COUNTERS];
	uint32_t enc[NUM_OF_ENCODERS];
	uint16_t enc_direction;

	uint16_t reserved_1[22];

	slot_info_registers do_slot_info;
	uint16_t enable_pwm_channels;
	uint16_t pwm_mode;
	uint16_t digital_outputs;
	uint16_t pwm_frequency[NUM_OF_PWM_CHANNELS];
	uint16_t pwm_duty[NUM_OF_PWM_CHANNELS];
	uint16_t fault_reg;

	uint16_t reserved_2[63];
	
	slot_info_registers aio_slot_info;
	ch_functions channel_functions;
	uint16_t adc_en_50_60_hz;
	float channel_out_values[MAX_NUM_OF_ADC_CHANNELS];
	float channel_in_values[MAX_NUM_OF_ADC_CHANNELS];
	uint16_t open_circuit_flags;
	uint16_t short_circuit_flags;
	uint16_t channel_1_4_faults;
	uint16_t channel_5_8_faults;

	uint16_t reserved_3[40];

	slot_info_registers mio_slot_info;
	uint16_t mio_digital_outputs;
	uint16_t mio_enable_counters;
	uint32_t mio_cnt_preset_regs[NUM_OF_MIO_COUNTERS];
	uint16_t mio_cnt_reset_flags;
	uint16_t mio_cnt_preset_flags;
	uint16_t mio_enable_encoders;
	uint32_t mio_enc_preset_regs[NUM_OF_MIO_ENCODERS];
	uint16_t mio_enc_reset_flags;
	uint16_t mio_enc_preset_flags;
	uint16_t mio_analog_channels_type;
	uint16_t mio_digital_inputs;
	uint32_t mio_cnt[NUM_OF_MIO_COUNTERS];
	uint32_t mio_enc[NUM_OF_MIO_ENCODERS];
	uint16_t mio_enc_direction;
	float mio_channel_in_values[MAX_NUM_OF_MIO_ADC_CHANNELS];

	uint16_t reserved_4[41];

	uint16_t reserved_5[600];
}
Slot_Regs_t;

_Static_assert(sizeof(Slot_Regs_t) == (1000*2), "Slot_Regs struct size is not compatible with modbus register list");
//static_assert(sizeof(Slot_Regs_t) == (500*2), "Slot_Regs struct size is not compatible with modbus register list"); 

#pragma pack (1)
typedef struct
{
	struct
	{
		uint8_t minor_version;
		uint8_t major_version;
	}firmware_version;

	uint16_t modbus_device_address;
	uint16_t modbus_baudrate;
	uint16_t modbus_parity;
	uint16_t modbus_stop_bits;
	uint16_t ip_address4;
	uint16_t ip_address3;
	uint16_t ip_address2;
	uint16_t ip_address1;
	uint16_t subnet_mask4;
	uint16_t subnet_mask3;
	uint16_t subnet_mask2;
	uint16_t subnet_mask1;
	uint16_t default_gateway4;
	uint16_t default_gateway3;
	uint16_t default_gateway2;
	uint16_t default_gateway1;
#ifndef T9_RMT
	COMMUNICATION_SETTINGS wireless_communication_setttings;
#endif
	uint16_t save_modbus_settings;
	uint16_t return_to_factory_settings;
#ifndef T9_RMT
	uint16_t num_of_cards;
	uint32_t slots_comm_status_register;
	uint32_t slots_fault_status_register;
#else
	uint16_t slots_fault_status_register;
#endif
	uint16_t slot_type[MAX_NUM_OF_SLOTS];
	uint16_t slot_firmware_version[MAX_NUM_OF_SLOTS];
	uint16_t disable_all_outputs;
	uint16_t re_enumerate_cards;
	uint16_t power_on;

	HW_INFO_SETTINGS hw_info_settings;
	uint16_t save_hw_info;

#ifndef T9_RMT
	uint16_t reserved[820];
#else
	uint16_t reserved[962];
#endif
}
Device_Regs_t;

_Static_assert(sizeof(Device_Regs_t) == (1000*2), "Device_Regs struct size is not compatible with modbus register list");

#pragma pack (1)
typedef struct
{
	Device_Regs_t Device_Regs;
	Slot_Regs_t	Slot_Regs[MAX_NUM_OF_SLOTS];
}
Holding_Regs_t;

typedef union
{
	uint16_t 	HoldingRegisters[((sizeof(Device_Regs_t)/2)+ (sizeof(Slot_Regs_t)/2)*MAX_NUM_OF_SLOTS)];
	Holding_Regs_t	Holding_Regs;
} ModbusRegs_t;



extern uint16_t previous_re_enumerate_cards;
void setChannelFunctions(ch_functions*,uint8_t,uint8_t);

void Init_Modbus_Registers();
uint32_t GetBaudrate(uint16_t modbus_baudrate);

#endif /* INC_T9_RIO_REGISTERS_H_ */
