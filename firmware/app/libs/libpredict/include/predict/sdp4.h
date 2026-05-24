#ifndef SDP4_H_
#define SDP4_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <predict/predict.h>

/**
 * Initialize SDP4 model parameters.
 *
 * \param orbital_elements Orbital elements
 * \param m Struct to initialize
 **/
void sdp4_init( const predict_orbital_elements_t * orbital_elements,
                struct predict_sdp4 * m );

/**
 * Predict ECI position and velocity of deep-space orbit (period > 225 minutes)
 *according to SDP4 model and the given orbital parameters.
 *
 * \param m SDP4 model parameters
 * \param tsince Time since epoch of TLE in minutes
 * \param output Modeled output parameters
 * \copyright GPLv2+
 **/
void sdp4_predict( const struct predict_sdp4 * m,
                   double tsince,
                   struct model_output * output );

/**
 * Deep space perturbations. Original Deep() function.
 *
 * \param m SDP4 model parameters
 * \param ientry Behavior flag. 1: Deep space secular effects. 2: lunar-solar
 *periodics \param deep_arg Fixed deep perturbation parameters \param deep_dyn
 *Output of deep space perturbations \copyright GPLv2+
 **/
void sdp4_deep( const struct predict_sdp4 * m,
                int ientry,
                const deep_arg_fixed_t * deep_arg,
                deep_arg_dynamic_t * deep_dyn );

#ifdef __cplusplus
}
#endif

#endif
