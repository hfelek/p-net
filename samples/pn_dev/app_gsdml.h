/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2018 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#ifndef APP_GSDML_H
#define APP_GSDML_H

/**
 * @file
 * @brief Device properties defined by the GSDML device definition
 *
 * Functions for getting module, submodule and parameter
 * configurations using their ids.
 *
 * Important:
 * Any change in this file may require an update of the GSDML file.
 * Note that when the GSDML file is updated it has to be reloaded
 * in your Profinet engineering tool. PLC applications may be affected.
 *
 * Design requires unique submodule IDs and unique parameter indexes.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <pnet_api.h>
#include "src/device_parameters.h"
#include "src/device_description.h"
#define APP_GSDML_API 0

#define APP_GSDML_DEFAULT_STATION_NAME "eliar-elektronik"

/* GSDML tag: VendorID */
#define APP_GSDML_VENDOR_ID 0x0493

/* GSDML tag: DeviceID */
#define APP_GSDML_DEVICE_ID 0x0002

/* Used in DCP communication */
#define APP_GSDML_OEM_VENDOR_ID 0xcafe
#define APP_GSDML_OEM_DEVICE_ID 0xee02

/* Used in I&M0 */
#define APP_GSDML_IM_HARDWARE_REVISION 3
#define APP_GSDML_IM_VERSION_MAJOR     1
#define APP_GSDML_IM_VERSION_MINOR     2

/* Allowed: 'V', 'R', 'P', 'U', 'T' */
#define APP_GSDML_SW_REV_PREFIX       'V'
#define APP_GSDML_PROFILE_ID          0x1234
#define APP_GSDML_PROFILE_SPEC_TYPE   0x5678
#define APP_GSDML_IM_REVISION_COUNTER 0 /* Typically 0 */

/* Note: You need to read out the actual hardware serial number instead */
#define APP_GSDML_EXAMPLE_SERIAL_NUMBER "007"

/* Initial values. Can be overwritten by PLC */
#define APP_GSDML_TAG_FUNCTION "my function"
#define APP_GSDML_TAG_LOCATION "my location"
#define APP_GSDML_IM_DATE      "2022-03-01 10:03"
#define APP_GSDML_DESCRIPTOR   "my descriptor"
#define APP_GSDML_SIGNATURE    ""

/* GSDML tag: Writeable_IM_Records */
#define APP_GSDML_IM_SUPPORTED                                                 \
   (PNET_SUPPORTED_IM1 | PNET_SUPPORTED_IM2 | PNET_SUPPORTED_IM3)

/* GSDML tag: OrderNumber */
#define APP_GSDML_ORDER_ID "T9PAC PNet"

/* GSDML tag: ModuleInfo / Name */
#define APP_GSDML_PRODUCT_NAME "T9PAC Profinet Device"

/* GSDML tag: MinDeviceInterval */
#define APP_GSDML_MIN_DEVICE_INTERVAL 32 /* 1 ms */

#define APP_GSDML_DIAG_CUSTOM_USI 0x1234

/* See "Specification for GSDML" 8.26 LogBookEntryItem for allowed values */
#define APP_GSDML_LOGBOOK_ERROR_CODE   0x20 /* Manufacturer specific */
#define APP_GSDML_LOGBOOK_ERROR_DECODE 0x82 /* Manufacturer specific */
#define APP_GSDML_LOGBOOK_ERROR_CODE_1 PNET_ERROR_CODE_1_FSPM
#define APP_GSDML_LOGBOOK_ERROR_CODE_2 0x00       /* Manufacturer specific */
#define APP_GSDML_LOGBOOK_ENTRY_DETAIL 0xFEE1DEAD /* Manufacturer specific */

#define APP_GSDML_PARAMETER_1_IDX        123
#define APP_GSDML_PARAMETER_2_IDX        128
#define APP_GSDML_PARAMETER_DO16_IDX     129
#define APP_GSDML_PARAMETER_AO8_IDX      140
#define APP_GSDML_PARAMETER_AO4_IDX      141
#define APP_GSDML_PARAMETER_AI8_IDX      142
#define APP_GSDML_PARAMETER_AI4_IDX      143
#define APP_GSDML_PARAMETER_AIO8_IDX     144
#define APP_GSDML_PARAMETER_AIO4_IDX     145
#define APP_GSDML_PARAMETER_CNT8_IDX     146
#define APP_GSDML_PARAMETER_ENC4_IDX     147


/* Use same size for all parameters in example */
#define APP_GSDML_PARAMETER_LENGTH 4
#define APP_GSDML_PARAMETER_DO8_LENGTH sizeof(gsdm_t9_slot_do_8_config_data)
#define APP_GSDML_PARAMETER_DO16_LENGTH sizeof(gsdm_t9_slot_do_16_config_data)
#define APP_GSDML_PARAMETER_AO8_LENGTH sizeof(gsdm_t9_slot_ao_8_config_data)
#define APP_GSDML_PARAMETER_AO4_LENGTH sizeof(gsdm_t9_slot_ao_4_config_data)
#define APP_GSDML_PARAMETER_AI8_LENGTH sizeof(gsdm_t9_slot_ai_8_config_data)
#define APP_GSDML_PARAMETER_AI4_LENGTH sizeof(gsdm_t9_slot_ai_4_config_data)
#define APP_GSDML_PARAMETER_AIO8_LENGTH sizeof(gsdm_t9_slot_aio_8_config_data)
#define APP_GSDML_PARAMETER_AIO4_LENGTH sizeof(gsdm_t9_slot_aio_4_config_data)
#define APP_GSDML_PARAMETER_CNT8_LENGTH sizeof(gsdm_t9_slot_cnt_8_config_data)
#define APP_GSDML_PARAMETER_ENC4_LENGTH sizeof(gsdm_t9_slot_enc_4_config_data)


#define APP_GSDML_DEFAULT_MAUTYPE 0x10 /* Copper 100 Mbit/s Full duplex */

typedef struct app_gsdml_module
{
   uint32_t id;

   /** Module name */
   const char * name;

   /** Submodule IDs. Variable length, ends with 0. */
   uint32_t submodules[];
} app_gsdml_module_t;

typedef struct app_gsdml_submodule
{
   uint32_t id;

   /** Submodule name */
   const char * name;

   uint32_t api;
   pnet_submodule_dir_t data_dir;
   uint16_t insize;
   uint16_t outsize;

   /** Parameter indexes. See app_gsdml_parameters.
    * Variable length, ends with 0. */
   uint16_t parameters[];
} app_gsdml_submodule_t;

typedef struct
{
   uint32_t index;
   const char * name;
   uint16_t length;
} app_gsdml_param_t;

#define APP_GSDML_MOD_ID_DI8N                 0x00000030
#define APP_GSDML_MOD_ID_DI8P                 0x0000001E

#define APP_GSDML_MOD_ID_DI16P                0x0000010F
#define APP_GSDML_MOD_ID_DI16N                0x00000102



#define APP_GSDML_MOD_ID_DO8P                0x00000031
#define APP_GSDML_MOD_ID_DO8N                0x000000110
#define APP_GSDML_MOD_ID_DO8R                0x000000111

#define APP_GSDML_MOD_ID_DO16P               0x000000106
#define APP_GSDML_MOD_ID_DO16N               0x000000112
#define APP_GSDML_MOD_ID_DO16R               0x000000113

#define APP_GSDML_MOD_ID_AO8                 0x00000010A
#define APP_GSDML_MOD_ID_AO4                 0x000000109

#define APP_GSDML_MOD_ID_AI8                 0x000000108
#define APP_GSDML_MOD_ID_AI4                 0x000000107

#define APP_GSDML_MOD_ID_AIO8                0x00000010C
#define APP_GSDML_MOD_ID_AIO4                0x00000010B

#define APP_GSDML_MOD_ID_CNT8                0x000000104
#define APP_GSDML_MOD_ID_ENC4                0x000000103


#define APP_GSDML_SUBMOD_ID_DI8N             0x00000132  //DI8N
#define APP_GSDML_SUBMOD_ID_DI8P             0x00000135  //DI8P

#define APP_GSDML_SUBMOD_ID_DI16N            0x00000136  //DI16N
#define APP_GSDML_SUBMOD_ID_DI16P            0x00000137  //DI16P

#define APP_GSDML_SUBMOD_ID_DO8P             0x00000131  //DO8P
#define APP_GSDML_SUBMOD_ID_DO8R             0x00000134  //DO8R
#define APP_GSDML_SUBMOD_ID_DO8N             0x00000133  //DO8N

#define APP_GSDML_SUBMOD_ID_DO16P            0x00000138  //DO16P
#define APP_GSDML_SUBMOD_ID_DO16R            0x00000139  //DO16R
#define APP_GSDML_SUBMOD_ID_DO16N            0x00000140  //DO16N

#define APP_GSDML_SUBMOD_ID_AO8              0x00000141  //AO8
#define APP_GSDML_SUBMOD_ID_AO4              0x00000142  //AO4

#define APP_GSDML_SUBMOD_ID_AI8              0x00000143  //AI8
#define APP_GSDML_SUBMOD_ID_AI4              0x00000144  //AI4

#define APP_GSDML_SUBMOD_ID_AIO8             0x00000145  //AIO8
#define APP_GSDML_SUBMOD_ID_AIO4             0x00000146  //AIO8

#define APP_GSDML_SUBMOD_ID_CNT8             0x00000147  //CNT8

#define APP_GSDML_SUBMOD_ID_ENC4             0x00000148  //ENC4



#define APP_GSDML_DO8_IO_DATA_OUTPUT_SIZE     sizeof(gsdm_t9_slot_do_8_out_io_data) /* bytes */
#define APP_GSDML_DI8_IO_DATA_INPUT_SIZE      sizeof(gsdm_t9_slot_di_8_in_io_data) /* bytes */

#define APP_GSDML_DO16_IO_DATA_OUTPUT_SIZE    sizeof(gsdm_t9_slot_do_16_out_io_data) /* bytes */
#define APP_GSDML_DI16_IO_DATA_INPUT_SIZE     sizeof(gsdm_t9_slot_di_16_in_io_data) /* bytes */

// #define APP_GSDML_AO8_IO_DATA_INPUT_SIZE      sizeof(gsdm_t9_slot_ao_8_in_io_data) /* bytes */
#define APP_GSDML_AO8_IO_DATA_OUTPUT_SIZE     sizeof(gsdm_t9_slot_ao_8_out_io_data) /* bytes */

// #define APP_GSDML_AO4_IO_DATA_INPUT_SIZE      sizeof(gsdm_t9_slot_ao_4_in_io_data) /* bytes */
#define APP_GSDML_AO4_IO_DATA_OUTPUT_SIZE     sizeof(gsdm_t9_slot_ao_4_out_io_data) /* bytes */

#define APP_GSDML_AI8_IO_DATA_INPUT_SIZE      sizeof(gsdm_t9_slot_ai_8_in_io_data) /* bytes */
#define APP_GSDML_AI4_IO_DATA_INPUT_SIZE      sizeof(gsdm_t9_slot_ai_4_in_io_data) /* bytes */

#define APP_GSDML_AIO8_IO_DATA_INPUT_SIZE      sizeof(gsdm_t9_slot_aio_8_in_io_data) /* bytes */
#define APP_GSDML_AIO8_IO_DATA_OUTPUT_SIZE     sizeof(gsdm_t9_slot_aio_8_out_io_data) /* bytes */

#define APP_GSDML_AIO4_IO_DATA_INPUT_SIZE      sizeof(gsdm_t9_slot_aio_4_in_io_data) /* bytes */
#define APP_GSDML_AIO4_IO_DATA_OUTPUT_SIZE     sizeof(gsdm_t9_slot_aio_4_out_io_data) /* bytes */

#define APP_GSDML_CNT8_IO_DATA_INPUT_SIZE      sizeof(gsdm_t9_slot_cnt_8_in_io_data) /* bytes */
#define APP_GSDML_CNT8_IO_DATA_OUTPUT_SIZE     sizeof(gsdm_t9_slot_cnt_8_out_io_data) /* bytes */


#define APP_GSDML_ENC4_IO_DATA_INPUT_SIZE      sizeof(gsdm_t9_slot_enc_4_in_io_data) /* bytes */
#define APP_GSDML_ENC4_IO_DATA_OUTPUT_SIZE     sizeof(gsdm_t9_slot_enc_4_out_io_data) /* bytes */


#define APP_GSDML_INPUT_DATA_ECHO_SIZE      8 /* bytes */
#define APP_GSDML_OUTPUT_DATA_ECHO_SIZE     APP_GSDML_INPUT_DATA_ECHO_SIZE
#define APP_GSDML_ALARM_PAYLOAD_SIZE        1 /* bytes */

/**
 * Get module configuration from module ID
 * @param module_id  In: Module ID
 * @return Module configuration, NULL if not found
 */
const app_gsdml_module_t * app_gsdml_get_module_cfg (uint32_t module_id);

/**
 * Get submodule module configuration from submodule ID
 * @param submodule_id  In: Submodule ID
 * @return Submodule configuration, NULL if not found
 */
const app_gsdml_submodule_t * app_gsdml_get_submodule_cfg (
   uint32_t submodule_id);

/**
 * Get parameter configuration from parameter index
 * @param submodule_id  In: Submodule ID
 * @param index         In: Parameters index
 * @return Parameter configuration, NULL if not found
 */
const app_gsdml_param_t * app_gsdml_get_parameter_cfg (
   uint32_t submodule_id,
   uint32_t index);

#ifdef __cplusplus
}
#endif

#endif /* APP_GSDML_H */
