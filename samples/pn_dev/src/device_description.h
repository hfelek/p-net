
#include  <stdint.h>

typedef	struct	_gsdm_t9_do_channel_pwm_configuration_
{
	uint16_t	duty_frequency;

}gsdm_t9_do_8_out_channel_pwm_configuration;

typedef struct _gsdm_t9_slot_do_out_io_data_
{
	uint8_t	do_status;
	gsdm_t9_do_8_out_channel_pwm_configuration pwm_config[MAX_NUMBER_OF_DO_PWM_CHANNEL];

}gsdm_t9_slot_do_8_out_io_data;

typedef struct _gsdm_t9_slot_do_config_data_
{
	uint8_t	activation;
	uint8_t	pwm_mode;

}gsdm_t9_slot_do_8_config_data;