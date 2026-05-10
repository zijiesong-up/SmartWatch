/**
  ******************************************************************************
  * @file    sin_cauclaute_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-05-10T18:03:33+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef SIN_CAUCLAUTE_DATA_PARAMS_H
#define SIN_CAUCLAUTE_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_SIN_CAUCLAUTE_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_sin_cauclaute_data_weights_params[1]))
*/

#define AI_SIN_CAUCLAUTE_DATA_CONFIG               (NULL)


#define AI_SIN_CAUCLAUTE_DATA_ACTIVATIONS_SIZES \
  { 60, }
#define AI_SIN_CAUCLAUTE_DATA_ACTIVATIONS_SIZE     (60)
#define AI_SIN_CAUCLAUTE_DATA_ACTIVATIONS_COUNT    (1)
#define AI_SIN_CAUCLAUTE_DATA_ACTIVATION_1_SIZE    (60)



#define AI_SIN_CAUCLAUTE_DATA_WEIGHTS_SIZES \
  { 324, }
#define AI_SIN_CAUCLAUTE_DATA_WEIGHTS_SIZE         (324)
#define AI_SIN_CAUCLAUTE_DATA_WEIGHTS_COUNT        (1)
#define AI_SIN_CAUCLAUTE_DATA_WEIGHT_1_SIZE        (324)



#define AI_SIN_CAUCLAUTE_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_sin_cauclaute_activations_table[1])

extern ai_handle g_sin_cauclaute_activations_table[1 + 2];



#define AI_SIN_CAUCLAUTE_DATA_WEIGHTS_TABLE_GET() \
  (&g_sin_cauclaute_weights_table[1])

extern ai_handle g_sin_cauclaute_weights_table[1 + 2];


#endif    /* SIN_CAUCLAUTE_DATA_PARAMS_H */
