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

#include "sampleapp_common.h"
#include "app_utils.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "osal.h"
#include "pnal.h"
#include <pnet_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************* Supported modules ***************************/

static const app_gsdml_module_t dap_1 = {
   .id = PNET_MOD_DAP_IDENT,
   .name = "DAP 1234234",
   .submodules = {
      PNET_SUBMOD_DAP_IDENT,
      PNET_SUBMOD_DAP_INTERFACE_1_PORT_1_IDENT,
      PNET_SUBMOD_DAP_INTERFACE_1_PORT_2_IDENT,
      PNET_SUBMOD_DAP_INTERFACE_1_PORT_3_IDENT,
      PNET_SUBMOD_DAP_INTERFACE_1_PORT_4_IDENT,
      0}};

static const app_gsdml_module_t module_DI8N = {
   .id = APP_GSDML_MOD_ID_DI8N,
   .name = "DI 8xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DI8N, 0},
};

static const app_gsdml_module_t module_DI8P = {
   .id = APP_GSDML_MOD_ID_DI8P,
   .name = "DI 8xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DI8P, 0},
};

static const app_gsdml_module_t module_DI16N = {
   .id = APP_GSDML_MOD_ID_DI16N,
   .name = "DI 16xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DI16N, 0},
};

static const app_gsdml_module_t module_DI16P = {
   .id = APP_GSDML_MOD_ID_DI16P,
   .name = "DI 16xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DI16P, 0},
};

static const app_gsdml_module_t module_DO8P = {
   .id = APP_GSDML_MOD_ID_DO8P,
   .name = "DO 8xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DO8P, 0}};

static const app_gsdml_module_t module_DO8N = {
   .id = APP_GSDML_MOD_ID_DO8N,
   .name = "DO 8xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DO8N, 0}};

static const app_gsdml_module_t module_DO8R = {
   .id = APP_GSDML_MOD_ID_DO8R,
   .name = "DO 8xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DO8R, 0}};

static const app_gsdml_module_t module_DO16P = {
   .id = APP_GSDML_MOD_ID_DO16P,
   .name = "DO 16xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DO16P, 0}};

static const app_gsdml_module_t module_DO16N = {
   .id = APP_GSDML_MOD_ID_DO16N,
   .name = "DO 16xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DO16N, 0}};

static const app_gsdml_module_t module_DO16R = {
   .id = APP_GSDML_MOD_ID_DO16R,
   .name = "DO 16xLogicLevel",
   .submodules = {APP_GSDML_SUBMOD_ID_DO16R, 0}};

static const app_gsdml_module_t module_AO8 = {
   .id = APP_GSDML_MOD_ID_AO8,
   .name = "Analog Outputx8",
   .submodules = {APP_GSDML_SUBMOD_ID_AO8, 0}};

static const app_gsdml_module_t module_AO4 = {
   .id = APP_GSDML_MOD_ID_AO4,
   .name = "Analog Outputx4",
   .submodules = {APP_GSDML_SUBMOD_ID_AO4, 0}};

static const app_gsdml_module_t module_AI8 = {
   .id = APP_GSDML_MOD_ID_AI8,
   .name = "Analog Inputx8",
   .submodules = {APP_GSDML_SUBMOD_ID_AI8, 0}};

static const app_gsdml_module_t module_AI4 = {
   .id = APP_GSDML_MOD_ID_AI4,
   .name = "Analog Inputx4",
   .submodules = {APP_GSDML_SUBMOD_ID_AI4, 0}};

static const app_gsdml_module_t module_AIO8 = {
   .id = APP_GSDML_MOD_ID_AIO8,
   .name = "Analog Input/Outputx8",
   .submodules = {APP_GSDML_SUBMOD_ID_AIO8, 0}};

static const app_gsdml_module_t module_AIO4 = {
   .id = APP_GSDML_MOD_ID_AIO4,
   .name = "Analog Input/Outputx4",
   .submodules = {APP_GSDML_SUBMOD_ID_AIO4, 0}};

static const app_gsdml_module_t module_CNT8 = {
   .id = APP_GSDML_MOD_ID_CNT8,
   .name = "Counter 8",
   .submodules = {APP_GSDML_SUBMOD_ID_CNT8, 0}};

static const app_gsdml_module_t module_ENC4 = {
   .id = APP_GSDML_MOD_ID_ENC4,
   .name = "Encoder 4",
   .submodules = {APP_GSDML_SUBMOD_ID_ENC4, 0}};

/******************* Supported submodules ************************/

static const app_gsdml_submodule_t dap_indentity_1 = {
   .name = "DAP Identity 1",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_interface_1 = {
   .name = "DAP Interface 1",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_port_1 = {
   .name = "DAP Port 1",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_PORT_1_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_port_2 = {
   .name = "DAP Port 2",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_PORT_2_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_port_3 = {
   .name = "DAP Port 3",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_PORT_3_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_port_4 = {
   .name = "DAP Port 4",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_PORT_4_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t submodule_DI8N = {
   .id = APP_GSDML_SUBMOD_ID_DI8N,
   .name = "Module DI8N",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_INPUT,
   .insize = APP_GSDML_DI8_IO_DATA_INPUT_SIZE,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t submodule_DI8P = {
   .id = APP_GSDML_SUBMOD_ID_DI8P,
   .name = "Module DI8P",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_INPUT,
   .insize = APP_GSDML_DI8_IO_DATA_INPUT_SIZE,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t submodule_DI16N = {
   .id = APP_GSDML_SUBMOD_ID_DI16N,
   .name = "Module DI16N",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_INPUT,
   .insize = APP_GSDML_DI16_IO_DATA_INPUT_SIZE,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t submodule_DI16P = {
   .id = APP_GSDML_SUBMOD_ID_DI16P,
   .name = "Module DI16P",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_INPUT,
   .insize = APP_GSDML_DI16_IO_DATA_INPUT_SIZE,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t submodule_DO8P = {
   .id = APP_GSDML_SUBMOD_ID_DO8P,
   .name = "Module DO8P",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_OUTPUT,
   .insize = 0,
   .outsize = APP_GSDML_DO8_IO_DATA_OUTPUT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_2_IDX}};

static const app_gsdml_submodule_t submodule_DO8N = {
   .id = APP_GSDML_SUBMOD_ID_DO8N,
   .name = "Module DO8N",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_OUTPUT,
   .insize = 0,
   .outsize = APP_GSDML_DO8_IO_DATA_OUTPUT_SIZE,
   .parameters = {128}};

static const app_gsdml_submodule_t submodule_DO8R = {
   .id = APP_GSDML_SUBMOD_ID_DO8R,
   .name = "Module DO8R",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_OUTPUT,
   .insize = 0,
   .outsize = APP_GSDML_DO8_IO_DATA_OUTPUT_SIZE,
   .parameters = {128}};

static const app_gsdml_submodule_t submodule_DO16R = {
   .id = APP_GSDML_SUBMOD_ID_DO16R,
   .name = "Module DO16R",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_OUTPUT,
   .insize = 0,
   .outsize = APP_GSDML_DO16_IO_DATA_OUTPUT_SIZE,
   .parameters = {128}};

static const app_gsdml_submodule_t submodule_DO16N = {
   .id = APP_GSDML_SUBMOD_ID_DO16N,
   .name = "Module DO16N",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_OUTPUT,
   .insize = 0,
   .outsize = APP_GSDML_DO16_IO_DATA_OUTPUT_SIZE,
   .parameters = {128}};

static const app_gsdml_submodule_t submodule_DO16P = {
   .id = APP_GSDML_SUBMOD_ID_DO16P,
   .name = "Module DO16P",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_OUTPUT,
   .insize = 0,
   .outsize = APP_GSDML_DO16_IO_DATA_OUTPUT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_DO16_IDX}};

static const app_gsdml_submodule_t submodule_AO8 = {
   .id = APP_GSDML_SUBMOD_ID_AO8,
   .name = "Module AO8",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_IO,
   .insize = 0,
   .outsize = APP_GSDML_AO8_IO_DATA_OUTPUT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_AO8_IDX}};

static const app_gsdml_submodule_t submodule_AO4 = {
   .id = APP_GSDML_SUBMOD_ID_AO4,
   .name = "Module AO4",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_IO,
   .insize = 0,
   .outsize = APP_GSDML_AO4_IO_DATA_OUTPUT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_AO4_IDX}};

static const app_gsdml_submodule_t submodule_AI8 = {
   .id = APP_GSDML_SUBMOD_ID_AI8,
   .name = "Module AI8",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_INPUT,
   .insize = APP_GSDML_AI8_IO_DATA_INPUT_SIZE,
   .outsize = 0,
   .parameters = {APP_GSDML_PARAMETER_AI8_IDX}};

static const app_gsdml_submodule_t submodule_AI4 = {
   .id = APP_GSDML_SUBMOD_ID_AI4,
   .name = "Module AI4",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_INPUT,
   .insize = APP_GSDML_AI4_IO_DATA_INPUT_SIZE,
   .outsize = 0,
   .parameters = {APP_GSDML_PARAMETER_AI4_IDX}};

static const app_gsdml_submodule_t submodule_AIO8 = {
   .id = APP_GSDML_SUBMOD_ID_AIO8,
   .name = "Module AIO8",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_IO,
   .insize = APP_GSDML_AIO8_IO_DATA_INPUT_SIZE,
   .outsize = APP_GSDML_AIO8_IO_DATA_OUTPUT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_AIO8_IDX}};

static const app_gsdml_submodule_t submodule_AIO4 = {
   .id = APP_GSDML_SUBMOD_ID_AIO4,
   .name = "Module AIO4",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_IO,
   .insize = APP_GSDML_AIO4_IO_DATA_INPUT_SIZE,
   .outsize = APP_GSDML_AIO4_IO_DATA_OUTPUT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_AIO4_IDX}};

static const app_gsdml_submodule_t submodule_CNT8 = {
   .id = APP_GSDML_SUBMOD_ID_CNT8,
   .name = "Module CNT8",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_IO,
   .insize = APP_GSDML_CNT8_IO_DATA_INPUT_SIZE,
   .outsize = APP_GSDML_CNT8_IO_DATA_OUTPUT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_CNT8_IDX}};

static const app_gsdml_submodule_t submodule_ENC4 = {
   .id = APP_GSDML_SUBMOD_ID_ENC4,
   .name = "Module ENC4",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_IO,
   .insize = APP_GSDML_ENC4_IO_DATA_INPUT_SIZE,
   .outsize = APP_GSDML_ENC4_IO_DATA_OUTPUT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_ENC4_IDX}};



/** List of supported modules */
static const app_gsdml_module_t * app_gsdml_modules[] = {
   &dap_1,
   &module_DI8N,
   &module_DI8P,
   &module_DI16N,
   &module_DI16P,

   &module_DO8P,
   &module_DO8R,
   &module_DO8N,
   &module_DO16P,
   &module_DO16R,
   &module_DO16N,
   &module_AO8,
   &module_AO4,
   &module_AI8,
   &module_AI4,
   &module_AIO8,
   &module_AIO4,
   &module_CNT8,
   &module_ENC4,

};

/** List of supported submodules */
static const app_gsdml_submodule_t * app_gsdml_submodules[] = {
   &dap_indentity_1, &dap_interface_1, &dap_port_1,      &dap_port_2,
   &dap_port_3,      &dap_port_4,

   &submodule_DI8N,  &submodule_DI8P,  &submodule_DI16N, &submodule_DI16P,

   &submodule_DO8P,  &submodule_DO8R,  &submodule_DO8N,  &submodule_DO16P,
   &submodule_DO16R, &submodule_DO16N, &submodule_CNT8,

   &submodule_AO8,   &submodule_AO4,   &submodule_ENC4,

   &submodule_AI8,   &submodule_AI4,

   &submodule_AIO8,  &submodule_AIO4

};

/* List of supported parameters.
 * Note that parameters are submodule attributes.
 * This list contain all parameters while each
 * submodule list its supported parameters using
 * their indexes.
 */
static app_gsdml_param_t app_gsdml_parameters[] = {
   {
      .index = APP_GSDML_PARAMETER_1_IDX,
      .name = "Demo 1",
      .length = APP_GSDML_PARAMETER_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_2_IDX,
      .name = "Demo 2",
      .length = APP_GSDML_PARAMETER_DO8_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_DO16_IDX,
      .name = "DO16 Configuration",
      .length = APP_GSDML_PARAMETER_DO16_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_AO8_IDX,
      .name = "AO8 Configuration",
      .length = APP_GSDML_PARAMETER_AO8_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_AO4_IDX,
      .name = "AO4 Configuration",
      .length = APP_GSDML_PARAMETER_AO4_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_AI8_IDX,
      .name = "AI8 Configuration",
      .length = APP_GSDML_PARAMETER_AI8_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_AI4_IDX,
      .name = "AI4 Configuration",
      .length = APP_GSDML_PARAMETER_AI4_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_AIO8_IDX,
      .name = "AIO8 Configuration",
      .length = APP_GSDML_PARAMETER_AIO8_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_AIO4_IDX,
      .name = "AIO4 Configuration",
      .length = APP_GSDML_PARAMETER_AIO4_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_CNT8_IDX,
      .name = "AIO4 Configuration",
      .length = APP_GSDML_PARAMETER_CNT8_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_ENC4_IDX,
      .name = "ENC4 Configuration",
      .length = APP_GSDML_PARAMETER_ENC4_LENGTH,
   },
};

const app_gsdml_module_t * app_gsdml_get_module_cfg (uint32_t id)
{
   uint32_t i;
   for (i = 0; i < NELEMENTS (app_gsdml_modules); i++)
   {
      if (app_gsdml_modules[i]->id == id)
      {
         return app_gsdml_modules[i];
      }
   }
   return NULL;
}

const app_gsdml_submodule_t * app_gsdml_get_submodule_cfg (uint32_t id)
{
   uint32_t i;
   for (i = 0; i < NELEMENTS (app_gsdml_submodules); i++)
   {
      if (app_gsdml_submodules[i]->id == id)
      {
         return app_gsdml_submodules[i];
      }
   }
   return NULL;
}

const app_gsdml_param_t * app_gsdml_get_parameter_cfg (
   uint32_t submodule_id,
   uint32_t index)
{
   uint16_t i;
   uint16_t j;

   const app_gsdml_submodule_t * submodule_cfg =
      app_gsdml_get_submodule_cfg (submodule_id);

   if (submodule_cfg == NULL)
   {
      APP_LOG_DEV_INFO ("Unsupported submodule id\n");
      /* Unsupported submodule id */
      return NULL;
   }

   /* Search for parameter index in submodule configuration */
   for (i = 0; submodule_cfg->parameters[i] != 0; i++)
   {
      if (submodule_cfg->parameters[i] == index)
      {
         /* Find parameter configuration */

         for (j = 0; j < NELEMENTS (app_gsdml_parameters); j++)
         {

            if (app_gsdml_parameters[j].index == index)
            {
               // APP_LOG_DEV_INFO("Number: %hu, Index: %u,
               // app_gsdml_parameters[j].index: %u
               // \n",j,index,app_gsdml_parameters[j].index);

               return &app_gsdml_parameters[j];
            }
         }
      }
   }

   return NULL;
}
