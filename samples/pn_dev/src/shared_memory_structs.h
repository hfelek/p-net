#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "io_cards_handler.h"
#include "T9_RIO_Registers.h"


#define MAX_NUMBER_OF_SLOT				32
#define MAX_NUMBER_OF_DIO_CHANNEL		16
#define MAX_NUMBER_OF_AIO_CHANNEL		8
#define MAX_NUMBER_OF_AI_CHANNEL		8
#define MAX_NUMBER_OF_AO_CHANNEL		8
#define MAX_NUMBER_OF_CNT_CHANNEL		8
#define MAX_NUMBER_OF_ENC_CHANNEL		4
#define MAX_NUMBER_OF_DO_PWM_CHANNEL	6
#pragma pack(push, 4)

typedef struct _t9_encoder_
{
	unsigned short int value[MAX_NUMBER_OF_ENC_CHANNEL];
	unsigned char 	   sign;
}t9_encoder;

typedef struct _t9_aio_diagnostics_
{
	unsigned char aio_fault[MAX_NUMBER_OF_AIO_CHANNEL];
	unsigned short int do_fault;
} t9_aio_diagnostics;

typedef struct _t9_aio_configuration_
{
	unsigned char 	   channel_function[MAX_NUMBER_OF_AIO_CHANNEL];
	unsigned char 	   channel_rejection_50_60_Hz;
}t9_aio_configuration;

typedef struct _t9_counter_configuration_
{
	unsigned char		activation;
	unsigned int		preset[MAX_NUMBER_OF_CNT_CHANNEL];
	unsigned char 	   	reset_trigger;
	unsigned char		preset_trigger;
	unsigned char       enable_frequency_mode;

}t9_counter_configuration;

typedef struct _t9_encoder_configuration_
{
	unsigned char		activation;
	unsigned int		preset[MAX_NUMBER_OF_ENC_CHANNEL];
	unsigned char 	   	reset_trigger;
	unsigned char		preset_trigger;
} t9_encoder_configuration;

typedef	struct	_t9_do_pwm_
{
	unsigned short int	frequency;
	unsigned short int	duty;
}t9_do_pwm;

typedef struct _t9_do_configuration_
{
	unsigned char	activation;
	unsigned char	mode;
	t9_do_pwm		pwm[MAX_NUMBER_OF_DO_PWM_CHANNEL];
}t9_do_configuration;

typedef struct
{
	unsigned short int 	t9di[MAX_NUMBER_OF_SLOT];
	float		  		t9ai[MAX_NUMBER_OF_SLOT][MAX_NUMBER_OF_AI_CHANNEL];
	unsigned int 		t9cnt[MAX_NUMBER_OF_SLOT][MAX_NUMBER_OF_CNT_CHANNEL];
	t9_encoder			t9enc[MAX_NUMBER_OF_SLOT];
	t9_aio_diagnostics	t9diag[MAX_NUMBER_OF_SLOT];

}t9ioin;

typedef struct
{
	unsigned short int 			t9do[MAX_NUMBER_OF_SLOT];	
	float 						t9ao[MAX_NUMBER_OF_SLOT][MAX_NUMBER_OF_AI_CHANNEL];
	t9_aio_configuration		t9aioconf[MAX_NUMBER_OF_SLOT];
	t9_counter_configuration	t9cntconf[MAX_NUMBER_OF_SLOT];
	t9_encoder_configuration	t9encconf[MAX_NUMBER_OF_SLOT];
	t9_do_configuration			t9doconf[MAX_NUMBER_OF_SLOT];
	unsigned char 	    triggerreenumeration;
}t9ioout;

typedef struct _t9_device_config_
{
    unsigned char       ModulesTypes[MAX_NUMBER_OF_SLOT];
	unsigned int 	    t9comcnt[MAX_NUMBER_OF_SLOT];
	unsigned char       slotfailstatus[MAX_NUMBER_OF_SLOT];
	unsigned char       unaddressedcardsavailable;
	unsigned char 	   	enumerationstatus;
}t9_device_config;

typedef struct _T9PLC
{
    unsigned char        ModulesTypes[MAX_NUMBER_OF_SLOT];
    unsigned char        ModulesCommand;
    t9ioin      IOIn;
    t9ioout     IOOut;
	t9_device_config    DeviceConfig;
}T9PLC, *PT9PLC;
#pragma pack(pop)
