#ifndef SGP4_H_
#define SGP4_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <predict/predict.h>

/**
 * Initialize SGP4 model parameters.
 *
 * \param orbital_elements Orbital elements
 * \param m Struct to initialize
 * \copyright GPLv2+
 **/
void sgp4_init( const predict_orbital_elements_t * orbital_elements,
                struct predict_sgp4 * m );

/**
 * Predict ECI position and velocity of near-earth orbit (period < 225 minutes)
 *according to SGP4 model and the given orbital parameters.
 *
 * \param m SGP4 model parameters
 * \param tsince Time since epoch of TLE in minutes
 * \param output Output of model
 * \copyright GPLv2+
 **/
void sgp4_predict( const struct predict_sgp4 * m,
                   double tsince,
                   struct model_output * output );

#ifdef __cplusplus
}
#endif

#endif
