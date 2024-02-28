/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2021 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#include "app_data.h"
#include "app_utils.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "sampleapp_common.h"
#include "osal.h"
#include "pnal.h"
#include <pnet_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./src/config.h"

extern PT9PLC pShmT9Plc;

#define APP_DATA_DEFAULT_OUTPUT_DATA 0

/* Parameter data for digital submodules
 * The stored value is shared between all digital submodules in this example.
 *
 * Todo: Data is always in pnio data format. Add conversion to uint32_t.
 */
static uint32_t app_param_1 = 0; /* Network endianness */
static uint32_t app_param_2[sizeof (gsdm_t9_slot_do_8_config_data)] = {0};
static uint32_t app_param_DO16_config[sizeof (gsdm_t9_slot_do_16_config_data)] =
   {0};
static uint32_t app_param_AO8_config[sizeof (gsdm_t9_slot_ao_8_config_data)] = {
   0};
static uint32_t app_param_AO4_config[sizeof (gsdm_t9_slot_ao_4_config_data)] = {
   0};

/* Digital submodule process data
 * The stored value is shared between all digital submodules in this example. */

typedef union
{
   uint8_t bytes[1440]; // PF_MAX_UDP_PAYLOAD_SIZE
   uint16_t two_bytes[1440 / 2];
   uint32_t four_bytes[1440 / 4];
} u_data;

static u_data inputdata = {0};
static u_data outputdata = {0};

CC_PACKED_BEGIN

CC_PACKED_END

uint8_t * app_data_get_input_data (
   uint16_t slot_nbr,
   uint16_t subslot_nbr,
   uint32_t submodule_id,
   bool button_pressed,
   uint16_t * size,
   uint8_t * iops)
{

   const uint8_t busSlot_nbr = slot_nbr - 1;
   if (size == NULL || iops == NULL)
   {
      APP_LOG_DEV_INFO ("here2 : slot number %u\n", slot_nbr);
      return NULL;
   }

   if (
      (submodule_id == APP_GSDML_SUBMOD_ID_DI8N) ||
      (submodule_id == APP_GSDML_SUBMOD_ID_DI8P))
   {
      gsdm_t9_slot_di_8_in_io_data dataCard;
      dataCard.di_status = pShmT9Plc->IOIn.t9di[busSlot_nbr];
      inputdata.bytes[0] = dataCard.di_status;

      *size = APP_GSDML_DI8_IO_DATA_INPUT_SIZE;
      *iops = PNET_IOXS_GOOD;
      return inputdata.bytes;
   }

   if (
      (submodule_id == APP_GSDML_SUBMOD_ID_DI16N) ||
      (submodule_id == APP_GSDML_SUBMOD_ID_DI16P))
   {
      gsdm_t9_slot_di_16_in_io_data dataCard;
      dataCard.di_status = pShmT9Plc->IOIn.t9di[busSlot_nbr];
      convertEndiannessUint16 (&dataCard.di_status, 2);

      inputdata.two_bytes[0] = dataCard.di_status;

      *size = APP_GSDML_DI16_IO_DATA_INPUT_SIZE;
      *iops = PNET_IOXS_GOOD;
      return inputdata.bytes;
   }

   if (
      (submodule_id == APP_GSDML_SUBMOD_ID_AO8) ||
      (submodule_id == APP_GSDML_SUBMOD_ID_AO4))
   {
      gsdm_t9_slot_ao_8_in_io_data dataCard;
      memset (&dataCard, 0, sizeof (gsdm_t9_slot_ao_8_in_io_data));
      memcpy (inputdata.bytes, &dataCard, sizeof (gsdm_t9_slot_ao_8_in_io_data));

      *size = APP_GSDML_AO8_IO_DATA_INPUT_SIZE;
      *iops = PNET_IOXS_GOOD;
      return inputdata.bytes;
   }

   /* Automated RT Tester scenario 2 - unsupported (sub)module */
   return NULL;
}

int app_data_set_output_data (
   uint16_t slot_nbr,
   uint16_t subslot_nbr,
   uint32_t submodule_id,
   uint8_t * data,
   uint16_t size)
{
   if (data == NULL)
   {
      return -1;
   }
   const uint8_t busSlot_nbr = slot_nbr - 1;
   if (
      submodule_id == APP_GSDML_SUBMOD_ID_DO8P ||
      submodule_id == APP_GSDML_SUBMOD_ID_DO8N ||
      submodule_id == APP_GSDML_SUBMOD_ID_DO8R)
   {
      if (size == APP_GSDML_DO8_IO_DATA_OUTPUT_SIZE)
      {

         gsdm_t9_slot_do_8_out_io_data dataCard;
         memcpy (outputdata.bytes, data, size);
         // logBuffer (&outputdata, sizeof (gsdm_t9_slot_do_8_out_io_data));
         memcpy (&dataCard, outputdata.bytes, size);
         convertEndiannessUint16 (
            (uint16_t *)&dataCard.pwm_config[0],
            MAX_NUMBER_OF_DO_PWM_CHANNEL);

         pShmT9Plc->IOOut.t9do[busSlot_nbr] = dataCard.do_status;

         for (int channelNumber = 0;
              channelNumber < MAX_NUMBER_OF_DO_PWM_CHANNEL;
              channelNumber++)
         {
            if ((pShmT9Plc->IOOut.t9doconf[busSlot_nbr].mode >> channelNumber &
                 1))
            {
               pShmT9Plc->IOOut.t9doconf[busSlot_nbr]
                  .pwm[channelNumber]
                  .frequency =
                  dataCard.pwm_config[channelNumber].duty_frequency;
            }
            else
            {
               if (dataCard.pwm_config[channelNumber].duty_frequency > 100)
               {
                  return -1;
                  pShmT9Plc->IOOut.t9doconf[busSlot_nbr]
                     .pwm[channelNumber]
                     .duty = 100;
               }
               else
               {
                  pShmT9Plc->IOOut.t9doconf[busSlot_nbr]
                     .pwm[channelNumber]
                     .duty = dataCard.pwm_config[channelNumber].duty_frequency;
               }
            }
         }

         return 0;
      }
   }

   else if (
      submodule_id == APP_GSDML_SUBMOD_ID_DO16P ||
      submodule_id == APP_GSDML_SUBMOD_ID_DO16N ||
      submodule_id == APP_GSDML_SUBMOD_ID_DO16R)
   {
      if (size == APP_GSDML_DO16_IO_DATA_OUTPUT_SIZE)
      {

         gsdm_t9_slot_do_16_out_io_data dataCard;
         memcpy (outputdata.bytes, data, size);
         memcpy (&dataCard, outputdata.bytes, size);

         convertEndiannessUint16 (
            (uint16_t *)&dataCard.pwm_config[0],
            MAX_NUMBER_OF_DO_PWM_CHANNEL);
         convertEndiannessUint16 (&dataCard.do_status, 2);
         pShmT9Plc->IOOut.t9do[busSlot_nbr] = dataCard.do_status;

         for (int channelNumber = 0;
              channelNumber < MAX_NUMBER_OF_DO_PWM_CHANNEL;
              channelNumber++)
         {
            if ((pShmT9Plc->IOOut.t9doconf[busSlot_nbr].mode >> channelNumber &
                 1))
            {
               pShmT9Plc->IOOut.t9doconf[busSlot_nbr]
                  .pwm[channelNumber]
                  .frequency =
                  dataCard.pwm_config[channelNumber].duty_frequency;
            }
            else
            {
               if (dataCard.pwm_config[channelNumber].duty_frequency > 100)
               {
                  return -1;
                  pShmT9Plc->IOOut.t9doconf[busSlot_nbr]
                     .pwm[channelNumber]
                     .duty = 100;
               }
               else
               {
                  pShmT9Plc->IOOut.t9doconf[busSlot_nbr]
                     .pwm[channelNumber]
                     .duty = dataCard.pwm_config[channelNumber].duty_frequency;
               }
            }
         }

         return 0;
      }
   }
   else if (submodule_id == APP_GSDML_SUBMOD_ID_AO8)
   {
      if (size == APP_GSDML_AO8_IO_DATA_OUTPUT_SIZE)
      {

         gsdm_t9_slot_ao_8_out_io_data dataCard;
         memcpy (outputdata.bytes, data, size);
         memcpy (&dataCard, outputdata.bytes, size);

         convertEndiannessUint32 (
            (uint32_t *)dataCard.ao_values,
            GSDM_AO8_CHANNEL_NUMBER);

         //   for(int i =0 ; i < 8 ; i++){
         //       APP_LOG_DEV_INFO("Channel: %d , Mode: %hhu,  Value %.2f\n",
         //       i+1,
         //       pShmT9Plc->IOOut.t9aioconf[busSlot_nbr].channel_function[i],
         //       dataCard.ao_values[i]);
         //    }

         memcpy (
            pShmT9Plc->IOOut.t9ao[busSlot_nbr],
            dataCard.ao_values,
            sizeof (dataCard.ao_values));
         return 0;
      }
   }
   else if (submodule_id == APP_GSDML_SUBMOD_ID_AO4)
   {
      if (size == APP_GSDML_AO4_IO_DATA_OUTPUT_SIZE)
      {

         gsdm_t9_slot_ao_4_out_io_data dataCard;
         memcpy (outputdata.bytes, data, size);
         memcpy (&dataCard, outputdata.bytes, size);

         convertEndiannessUint32 (
            (uint32_t *)dataCard.ao_values,
            GSDM_AO4_CHANNEL_NUMBER);

         // for (int i = 0; i < 4; i++)
         // {
         //    APP_LOG_DEV_INFO (
         //       "Channel: %d , Mode: %hhu,  Value %.2f\n",
         //       i + 1,
         //       pShmT9Plc->IOOut.t9aioconf[busSlot_nbr].channel_function[i],
         //       dataCard.ao_values[i]);
         // }

         memcpy (
            pShmT9Plc->IOOut.t9ao[busSlot_nbr],
            dataCard.ao_values,
            sizeof (dataCard.ao_values));
         return 0;
      }
   }

   return -1;
}

int app_data_set_default_outputs (void)
{
   outputdata.bytes[0] = APP_DATA_DEFAULT_OUTPUT_DATA;
   return 0;
}

int app_data_write_parameter (
   uint16_t slot_nbr,
   uint16_t subslot_nbr,
   uint32_t submodule_id,
   uint32_t index,
   const uint8_t * data,
   uint16_t length)
{
   uint8_t busSlot_nbr = slot_nbr - 1;

   APP_LOG_DEV_INFO (
      "PLC write request  gsd parameter. "
      "Submodule id: %u Index: %u Slot Nmbr: %hu, Subslot Nmbr: %hu\n",

      (unsigned)submodule_id,
      (unsigned)index,
      slot_nbr,
      subslot_nbr);
   // logBuffer (data, length);
   const app_gsdml_param_t * par_cfg;

   par_cfg = app_gsdml_get_parameter_cfg (submodule_id, index);
   if (par_cfg == NULL)
   {
      APP_LOG_WARNING (
         "PLC write request unsupported submodule/parameter. "
         "Submodule id: %u Index: %u\n",
         (unsigned)submodule_id,
         (unsigned)index);
      return -1;
   }

   if (length != par_cfg->length)
   {
      APP_LOG_WARNING (
         "PLC write request unsupported length. "
         "Index: %u Length: %u Expected length: %u\n",
         (unsigned)index,
         (unsigned)length,
         par_cfg->length);
      return -1;
   }

   if (index == APP_GSDML_PARAMETER_1_IDX)
   {
      memcpy (&app_param_1, data, length);
   }
   else if (index == APP_GSDML_PARAMETER_2_IDX)
   {

      memcpy (&app_param_2, data, length);
      gsdm_t9_slot_do_8_config_data dummy;
      memcpy (&dummy, data, length);

      uint8_t data_shm_mode = 0;
      uint8_t data_shm_activation = 0;

      for (int i = 0; i < length; i++)
      {
         data_shm_activation += (dummy.activation[i] & 0x01) << i;
         data_shm_mode += (dummy.pwm_mode[i] & 0x01) << i;
      }
      APP_LOG_DEV_INFO ("mode: %hhu\n", data_shm_mode);
      APP_LOG_DEV_INFO ("activation: %hhu\n", data_shm_activation);
      pShmT9Plc->IOOut.t9doconf[busSlot_nbr].activation = data_shm_activation;
      pShmT9Plc->IOOut.t9doconf[busSlot_nbr].mode = data_shm_mode;
   }

   else if (index == APP_GSDML_PARAMETER_DO16_IDX)
   {
      memcpy (&app_param_DO16_config, data, length);
      gsdm_t9_slot_do_16_config_data cfg_data;
      memcpy (&cfg_data, data, length);

      uint16_t data_shm_mode = 0;
      uint16_t data_shm_activation = 0;

      for (int i = 0; i < length; i++)
      {
         data_shm_activation += (cfg_data.activation[i] & 0x01) << i;
         data_shm_mode += (cfg_data.pwm_mode[i] & 0x01) << i;
      }

      pShmT9Plc->IOOut.t9doconf[busSlot_nbr].activation = data_shm_activation;
      pShmT9Plc->IOOut.t9doconf[busSlot_nbr].mode = data_shm_mode;
   }
   else if (index == APP_GSDML_PARAMETER_AO8_IDX)
   {
      memcpy (&app_param_AO8_config, data, length);
      gsdm_t9_slot_ao_8_config_data cfg_data;
      memcpy (&cfg_data, data, length);
      uint8_t freqeuncy_suppresion = 0;
      for (int i = 0; i < GSDM_AO8_CHANNEL_NUMBER; i++)
      {

         pShmT9Plc->IOOut.t9aioconf[busSlot_nbr].channel_function[i] =
            cfg_data.operation_mode[i];
         freqeuncy_suppresion += (cfg_data.frequency_suppresion[i] & 0x01) << i;
      }
      pShmT9Plc->IOOut.t9aioconf[busSlot_nbr].channel_rejection_50_60_Hz =
         freqeuncy_suppresion;
   }
   else if (index == APP_GSDML_PARAMETER_AO4_IDX)
   {
      memcpy (&app_param_AO4_config, data, length);
      gsdm_t9_slot_ao_4_config_data cfg_data;
      memcpy (&cfg_data, data, length);
      uint8_t freqeuncy_suppresion = 0;
      for (int i = 0; i < GSDM_AO4_CHANNEL_NUMBER; i++)
      {
         pShmT9Plc->IOOut.t9aioconf[busSlot_nbr].channel_function[i] =
            cfg_data.operation_mode[i];
         freqeuncy_suppresion += (cfg_data.frequency_suppresion[i] & 0x01) << i;
      }
      pShmT9Plc->IOOut.t9aioconf[busSlot_nbr].channel_rejection_50_60_Hz =
         freqeuncy_suppresion;
   }

   APP_LOG_DEBUG ("  Writing parameter \"%s\"\n", par_cfg->name);

   app_log_print_bytes (APP_LOG_LEVEL_DEBUG, data, length);

   return 0;
}

int app_data_read_parameter (
   uint16_t slot_nbr,
   uint16_t subslot_nbr,
   uint32_t submodule_id,
   uint32_t index,
   uint8_t ** data,
   uint16_t * length)
{
   const app_gsdml_param_t * par_cfg;

   // TODO: Uygulama büyük ihtimalle burada patlar. uint32_t'lik değerden normal
   // structa çek.

   APP_LOG_DEV_INFO (
      "PLC read request slot number: %hu,  submodule/parameter. "
      "Submodule id: %u Index: %u\n",
      slot_nbr,
      (unsigned)submodule_id,
      (unsigned)index);
   par_cfg = app_gsdml_get_parameter_cfg (submodule_id, index);
   if (par_cfg == NULL)
   {
      APP_LOG_WARNING (
         "PLC read request unsupported submodule/parameter. "
         "Submodule id: %u Index: %u\n",
         (unsigned)submodule_id,
         (unsigned)index);
      return -1;
   }

   if (*length < par_cfg->length)
   {
      APP_LOG_WARNING (
         "PLC read request unsupported length. "
         "Index: %u Max length: %u Data length for our parameter: %u\n",
         (unsigned)index,
         (unsigned)*length,
         par_cfg->length);
      return -1;
   }

   APP_LOG_DEBUG ("Reading \"%s\"\n", par_cfg->name);
   APP_LOG_DEV_INFO ("Reading \"%s\"\n", par_cfg->name);

   if (index == APP_GSDML_PARAMETER_1_IDX)
   {
      *data = (uint8_t *)&app_param_1;
      *length = sizeof (app_param_1);
   }
   else if (index == APP_GSDML_PARAMETER_2_IDX)
   {
      *data = (uint8_t *)&app_param_2;
      *length = sizeof (app_param_2);
   }
   else if (index == APP_GSDML_PARAMETER_DO16_IDX)
   {
      *data = (uint8_t *)&app_param_DO16_config;
      *length = sizeof (app_param_DO16_config);
   }
   else if (index == APP_GSDML_PARAMETER_AO4_IDX)
   {
      APP_LOG_DEV_INFO("jhaskjadsh\n");
      *data = (uint8_t *)&app_param_AO4_config;
      *length = 8;
   }

   app_log_print_bytes (APP_LOG_LEVEL_DEBUG, *data, *length);

   return 0;
}
