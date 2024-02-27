
#include  <stdint.h>
#define GSDM_DO8_CHANNEL_NUMBER 8


//Seen from Codesys implementation, each parameter defined must have at least one byte. Otherwise application doesn't build.
//In siemens implementation each paramters has padding of 4 tho.
#pragma pack(push,1)
/*CONFIG DATA -- START*/

typedef struct _gsdm_t9_slot_do_config_data_
{
	uint8_t	activation[GSDM_DO8_CHANNEL_NUMBER];
	uint8_t	pwm_mode[GSDM_DO8_CHANNEL_NUMBER]; //Info: Since channel 7 and 8 will shown as read only as digital output, pwm mode value is te 8 instead of 6.

}gsdm_t9_slot_do_8_config_data;

/*CONFIG DATA -- END*/

#pragma pack(pop)


//IO data aligment is set to 1
#pragma pack(push,1)

/*IO DATA -- START*/

/*DO8*/
typedef	struct	_gsdm_t9_do_channel_pwm_configuration_
{
	uint16_t	duty_frequency;

}gsdm_t9_do_8_out_channel_pwm_configuration;

typedef struct _gsdm_t9_slot_do_8_out_io_data_
{
	uint8_t	do_status;
	gsdm_t9_do_8_out_channel_pwm_configuration pwm_config[MAX_NUMBER_OF_DO_PWM_CHANNEL];

}gsdm_t9_slot_do_8_out_io_data;

/*DI8*/
typedef struct _gsdm_t9_slot_di_8_in_io_data_
{
	uint8_t	di_status;
	
}gsdm_t9_slot_di_8_in_io_data;

/*IO DATA -- END*/

#pragma pack(pop)
