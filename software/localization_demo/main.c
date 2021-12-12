/* ----------------------------------------------------------------------
* Copyright (C) 2019-2020 ARM Limited. All rights reserved.
*
* $Date:         09. December 2019
* $Revision:     V1.0.0
*
* Project:       CMSIS DSP Library
* Title:         arm_bayes_example_f32.c
*
* Description:   Example code demonstrating how to use Bayes functions.
*
* Target Processor: Cortex-M/Cortex-A
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------- */

/**
 * @ingroup groupExamples
 */

/**
 * @defgroup BayesExample Bayes Example
 *
 * \par Description:
 * \par
 * Demonstrates the use of Bayesian classifier functions. It is complementing the tutorial
 * about classical ML with CMSIS-DSP and python scikit-learn:
 * https://developer.arm.com/solutions/machine-learning-on-arm/developer-material/how-to-guides/implement-classical-ml-with-arm-cmsis-dsp-libraries
 *
 */


/** \example arm_bayes_example_f32.c
  */

#include <math.h>
#include <stdio.h>
#include "arm_math.h"

/* 
Those parameters can be generated with the python library scikit-learn.
*/
arm_gaussian_naive_bayes_instance_f32 S;

#define NB_OF_CLASSES 3
#define VECTOR_DIMENSION 6

const float32_t theta[NB_OF_CLASSES*VECTOR_DIMENSION] = {
-52.906172839506176, -72.64444444444445, -80.73827160493828,-51.79925650557621,
 -74.02973977695167, -84.38289962825279, -54.23622047244095, -71.48818897637796, 
 -80.89763779527559, -74.36363636363636, -63.83116883116883, -78.83116883116882,-70.55833333333334,
  -54.925, -74.94166666666666, -69.1640625, -60.4765625, -79.859375
}; /**< Mean values for the Gaussians */

const float32_t sigma[NB_OF_CLASSES*VECTOR_DIMENSION] = {
 24.39613483391136, 7.666172937858941, 31.106806987546573, 9.885352705420123, 2.3634279116776375, 17.1507857425534,
  2.164672427697435, 20.391592881538326, 0.5328291640109084, 31.60802843365385, 1.880587043874784, 23.38708055036829,
   21.229930653908333, 11.586041765019429, 3.538263987241654, 4.809021094446527, 8.311950781946527, 2.683349707727777
}; /**< Variances for the Gaussians */

const float32_t classPriors[6] = {
 0.33665835411471323f, 0.22360764754779716f, 0.1055694098088113f, 0.12801330008312553f, 0.09975062344139651f, 
 0.10640066500415628f
}; /**< Class prior probabilities */

int32_t main(void)
{
  /* Array of input data */
  float32_t in[3];

  /* Result of the classifier */
  float32_t result[NB_OF_CLASSES];
  float32_t temp[NB_OF_CLASSES];
  float32_t maxProba;
  uint32_t index;
  
  S.vectorDimension = VECTOR_DIMENSION; 
  S.numberOfClasses = NB_OF_CLASSES; 
  S.theta = theta;          
  S.sigma = sigma;         
  S.classPriors = classPriors;    
  S.epsilon=4.328939296523643e-09f; 

  in[0] = -0.0f;
  in[1] = -1.0f;
  in[2] = 1.0f;

  index = arm_gaussian_naive_bayes_predict_f32(&S, in, result);

  maxProba = result[index];

#if defined(SEMIHOSTING)
  printf("Class = %d\n", index);
#endif

  in[0] =-0.8f;
  in[1] = -1.0f;
  in[2] = 1.0f; 

  index = arm_gaussian_naive_bayes_predict_f32(&S, in, result);

  maxProba = result[index];

#if defined(SEMIHOSTING)
  printf("Class = %d\n", index);
#endif
  
  in[0] = -1.0f;
  in[1] = -1.0f;
  in[2] = 1.0f;

  index = arm_gaussian_naive_bayes_predict_f32(&S, in, result);

  maxProba = result[index];

#if defined(SEMIHOSTING)
  printf("Class = %d\n", index);
#endif

#if !defined(SEMIHOSTING)
  while (1); /* main function does not return */
#endif
}


