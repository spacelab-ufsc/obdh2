/*
 * isis_antenna_cmds.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
 * 
 * This file is part of FloripaSat-TTC.
 * 
 * FloripaSat-TTC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-TTC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief ISIS antenna driver commands.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.9
 * 
 * \date 20/09/2017
 * 
 * \defgroup isis_antenna_cmds
 * \{
 */

#ifndef ISIS_ANTENNA_CMDS_H_
#define ISIS_ANTENNA_CMDS_H_

/**
 * \brief Performs a reset of the microcontroller.
 */
#define ISIS_ANTENNA_CMD_RESET                          0xAA

/**
 * \brief Arm antenna system.
 * 
 * Arm the antenna system. The system needs to be armed to be
 * able to deploy antennas.
 */
#define ISIS_ANTENNA_CMD_ARM                            0xAD

/**
 * \brief Disarm antenna system.
 * 
 * Disarms the antenna system. This command will deactivate any active antenna
 * deployment systems and terminate an ongoing automated sequential antenna
 * deployment before disarming the system. The antenna system will only be able
 * to deploy antennas once it has been armed again.
 */
#define ISIS_ANTENNA_CMD_DISARM                         0xAC

/**
 * \brief Deploy antenna 1.
 * 
 * Description: Attempt to deploy antenna 1 by activating the deployment
 * system for antenna 1. The deployment mechanism will remain activated until:
 *      - The deployment of antenna 1 is detected using the corresponding
 *        deployment switch.
 *      - The deployment system has been active for the specified amount of time.
 *      - The deployment system has been active for the amount of time specified
 *        in the safety timeout.
 *      .
 * 
 * This command will not be executed when:
 *      - The antenna system has not been armed.
 *      - Another antenna deployment system is currently active.
 *      - The antenna's deployment switch indicates the antenna is deployed.
 *      - An automatic sequential antenna deployment is in progress.
 *      .
 * 
 * Parameters: [001 - 001] n/a, binary. The maximum activation time for the
 * deployment system in seconds. The minimum value is 0 and the maximum value
 * is 255 seconds. Please note that if a value of 0 is specified, only the
 * safety timeout will be taken into account.
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_ANT_1                   0xA1

/**
 * \brief Deploy antenna 2.
 * 
 * Description: Attempt to deploy antenna 2 by activating the deployment
 * system for antenna 2. The deployment mechanism will remain activated until:
 *      - The deployment of antenna 2 is detected using the corresponding
 *        deployment switch.
 *      - The deployment system has been active for the specified amount of time.
 *      - The deployment system has been active for the amount of time specified
 *        in the safety timeout.
 *      .
 * 
 * This command will not be executed when:
 *      - The antenna system has not been armed.
 *      - Another antenna deployment system is currently active.
 *      - The antenna's deployment switch indicates the antenna is deployed.
 *      - An automatic sequential antenna deployment is in progress.
 *      .
 * 
 * Parameters: [001 - 001] n/a, binary. The maximum activation time for the
 * deployment system in seconds. The minimum value is 0 and the maximum value
 * is 255 seconds. Please note that if a value of 0 is specified, only the
 * safety timeout will be taken into account.
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_ANT_2                   0xA2

/**
 * \brief Deploy antenna 3.
 * 
 * Description: Attempt to deploy antenna 3 by activating the deployment
 * system for antenna 3. The deployment mechanism will remain activated until:
 *      - The deployment of antenna 3 is detected using the corresponding
 *        deployment switch.
 *      - The deployment system has been active for the specified amount of time.
 *      - The deployment system has been active for the amount of time specified
 *        in the safety timeout.
 *      .
 * 
 * This command will not be executed when:
 *      - The antenna system has not been armed.
 *      - Another antenna deployment system is currently active.
 *      - The antenna's deployment switch indicates the antenna is deployed.
 *      - An automatic sequential antenna deployment is in progress.
 *      .
 * 
 * Parameters: [001 - 001] n/a, binary. The maximum activation time for the
 * deployment system in seconds. The minimum value is 0 and the maximum value
 * is 255 seconds. Please note that if a value of 0 is specified, only the
 * safety timeout will be taken into account.
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_ANT_3                   0xA3

/**
 * \brief Deploy antenna 4.
 * 
 * Description: Attempt to deploy antenna 4 by activating the deployment
 * system for antenna 4. The deployment mechanism will remain activated until:
 *      - The deployment of antenna 4 is detected using the corresponding
 *        deployment switch.
 *      - The deployment system has been active for the specified amount of time.
 *      - The deployment system has been active for the amount of time specified
 *        in the safety timeout.
 *      .
 * 
 * This command will not be executed when:
 *      - The antenna system has not been armed.
 *      - Another antenna deployment system is currently active.
 *      - The antenna's deployment switch indicates the antenna is deployed.
 *      - An automatic sequential antenna deployment is in progress.
 *      .
 * 
 * Parameters: [001 - 001] n/a, binary. The maximum activation time for the
 * deployment system in seconds. The minimum value is 0 and the maximum value
 * is 255 seconds. Please note that if a value of 0 is specified, only the
 * safety timeout will be taken into account.
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_ANT_4                   0xA4

/**
 * \brief Start automated sequential antenna deployment.
 * 
 * Description: Attempt to sequentially deploy all the antennas present on the
 * system, without intervation. This command will start at the antenna with the
 * lowest number and attempt to deploy it. After this attempt has finished, the
 * system will automatically move on to the next antenna and attempt to deploy
 * that. This process will continue until deployment of all the antennas present
 * on the system has been attempted.
 * This command is similar to sequentially executing an "Deploy antenna" command
 * for each antenna present on the system.
 * The antenna deployment system for each antenna is activated until:
 *      - The deployment of the antenna is detected using the corresponding
 *        deployment switch.
 *      - The deployment system has been active for the specified amount of time.
 *      - The deployment system has been active for the amount of time specified
 *        in the safety timeout.
 *      .
 * 
 * This command will not be executed when:
 *      - The antenna system has not been armed.
 *      - Another antenna deployment system is currently active.
 *      - An automatic sequential antenna deployment is in progress.
 *      .
 * 
 * Parameters: [001 - 001] n/a, binary. The maximum activation time for each
 * deployment system in seconds. The minimum value is 0 and the maximum value is
 * 255 seconds. Please note that if a value of 0 is specified, only the safety
 * timeout will be taken into account.
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_SEQUENTIAL              0xA5

/**
 * \brief Deploy antenna 1 with override.
 * 
 * Description: Attempt to deploy antenna 1 by activating the deployment system
 * for antenna 1. This command ignores the corresponding deployment switch that indicates
 * deployment of antenna 1. The deployment mechanism will remain activated until:
 *      - The deployment system has been active for the specified amount of time.
 *      - The deployment system has been active for the amount of time specified
 *        in the safety timeout.
 *      .
 * 
 * This command will not be executed when:
 *      - The antenna system has not been armed.
 *      - Another antenna deployment system is currently active.
 *      - An automatic sequential antenna deployment is in progress.
 *      .
 * 
 * Parameters: [001 - 001] n/a, binary. The maximum activation time for the
 * deployment system in seconds. The minimum value is 0 and the maximum value
 * is 255 seconds. Please note that if a value of 0 is specified, only the
 * safety timeout will be taken into account.
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_ANT_1_WITH_OVERRIDE     0xBA

/**
 * \brief Deploy antenna 2 with override.
 * 
 * Description: Attempt to deploy antenna 2 by activating the deployment system
 * for antenna 2. This command ignores the corresponding deployment switch that indicates
 * deployment of antenna 2. The deployment mechanism will remain activated until:
 *      - The deployment system has been active for the specified amount of time.
 *      - The deployment system has been active for the amount of time specified
 *        in the safety timeout.
 *      .
 * 
 * This command will not be executed when:
 *      - The antenna system has not been armed.
 *      - Another antenna deployment system is currently active.
 *      - An automatic sequential antenna deployment is in progress.
 *      .
 * 
 * Parameters: [001 - 001] n/a, binary. The maximum activation time for the
 * deployment system in seconds. The minimum value is 0 and the maximum value
 * is 255 seconds. Please note that if a value of 0 is specified, only the
 * safety timeout will be taken into account.
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_ANT_2_WITH_OVERRIDE     0xBB

/**
 * \brief Deploy antenna 3 with override.
 * 
 * Description: Attempt to deploy antenna 3 by activating the deployment system
 * for antenna 3. This command ignores the corresponding deployment switch that indicates
 * deployment of antenna 3. The deployment mechanism will remain activated until:
 *      - The deployment system has been active for the specified amount of time.
 *      - The deployment system has been active for the amount of time specified
 *        in the safety timeout.
 *      .
 * 
 * This command will not be executed when:
 *      - The antenna system has not been armed.
 *      - Another antenna deployment system is currently active.
 *      - An automatic sequential antenna deployment is in progress.
 *      .
 * 
 * Parameters: [001 - 001] n/a, binary. The maximum activation time for the
 * deployment system in seconds. The minimum value is 0 and the maximum value
 * is 255 seconds. Please note that if a value of 0 is specified, only the
 * safety timeout will be taken into account.
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_ANT_3_WITH_OVERRIDE     0xBC

/**
 * \brief Deploy antenna 4 with override.
 * 
 * Description: Attempt to deploy antenna 4 by activating the deployment system
 * for antenna 4. This command ignores the corresponding deployment switch that indicates
 * deployment of antenna 4. The deployment mechanism will remain activated until:
 *      - The deployment system has been active for the specified amount of time.
 *      - The deployment system has been active for the amount of time specified
 *        in the safety timeout.
 *      .
 * This command will not be executed when:
 *      - The antenna system has not been armed.
 *      - Another antenna deployment system is currently active.
 *      - An automatic sequential antenna deployment is in progress.
 *      .
 * 
 * Parameters: [001 - 001] n/a, binary. The maximum activation time for the
 * deployment system in seconds. The minimum value is 0 and the maximum value
 * is 255 seconds. Please note that if a value of 0 is specified, only the
 * safety timeout will be taken into account.
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_ANT_4_WITH_OVERRIDE     0xBD

/**
 * \brief Cancel deployment activation.
 * 
 * Description: Deactivate any deployment systems that are currently active,
 * and terminate ongoing automated sequential antenna deployment. This command
 * only has effect when the system has been armed and a deployment system is
 * currently active or an automated sequential antenna deployment is currently
 * ongoing.
 * 
 * Parameters: None
 * 
 * Response: None
 */
#define ISIS_ANTENNA_CMD_DEPLOY_CANCEL                  0xA9

/**
 * \brief Measure antenna system temperature.
 * 
 * Description: Measures the temperature as reported by the temperature sensor
 * on the antenna system. Please note that each microcontroller connects to its
 * own temperature sensor.
 * 
 * Parameters: None
 * 
 * Response: [000 - 001] 000000xx xxxxxxxx. Raw 10 bits measurement of the
 * temperature. Please refer to Annex A. for the conversion table.
 */
#define ISIS_ANTENNA_CMD_MEASURE_TEMPERATURE            0xC0

/**
 * \brief Report deployment status.
 * 
 * Description: Report the deployment status of the antenna system. This status
 * contains information for each antenna as well as system level information.
 * 
 * Parameters: None
 * 
 * Response: [000 - 001] xxxxxxxx xxxxxxxx.
 * Format of the deployment status information:
 * 
 *           bit 7                                      bit 0
 *          --------------------------------------------------
 * MSB      | A1S | A1T | A1B | 0    | A2S | A2T | A2B | IG  |
 * LSB      | A3S | A3T | A3B | INDB | A4S | A4T | A4B | ARM |
 *          --------------------------------------------------
 * 
 * AxS
 *      - 1: This antenna's deployment switch indicates this antenna is NOT deployed.
 *      - 0: This antenna's deployment switch indicates this antenna is deployed.
 *      .
 * 
 * AxT
 * The latest deployment system activation for this antenna was stopped because:
 *      - 1: a time limit was reached (specified time or safety time limit).
 *      - 0: a reason other than reaching a time limit.
 *      .
 * 
 * AxB
 *      - 1: This antenna's deployment system is currently active.
 *      - 0: This antenna's deployment system is currently NOT active.
 *      .
 * 
 * INDB
 *      - 1: The antenna system independent burn is currently active.
 *      - 0: The antenna system independent burn is currently NOT active.
 *      .
 * 
 * IG
 *      - 1: The antenna system is currently ignoring the antenna deployment switches.
 *      - 0: The antenna system is currently NOT ignoring the antenna deployment switches.
 *      .
 * 
 * ARM
 *      - 1: The antenna system is currently armed.
 *      - 0: The antenna system is currently NOT armed.
 *      .
 */
#define ISIS_ANTENNA_CMD_REPORT_DEPLOY_STATUS           0xC3

/**
 * \brief Report antenna 1 deployment system activation count.
 *
 * Description: Reports the number of times the deployment system for antenna 1
 * has been activated since the last reset of the microcontroller.
 *
 * Parameters: None
 *
 * Response: [000 - 000] xxxxxxxx. The number of deployment system activations.
 * This number has a minimum value of 0 and a maximum value of 255.
 */
#define ISIS_ANTENNA_CMD_REPORT_DEPLOY_COUNTER_ANT_1    0xB0

/**
 * \brief Report antenna 2 deployment system activation count.
 * 
 * Description: Reports the number of times the deployment system for antenna 2
 * has been activated since the last reset of the microcontroller.
 * 
 * Parameters: None
 * 
 * Response: [000 - 000] xxxxxxxx. The number of deployment system activations.
 * This number has a minimum value of 0 and a maximum value of 255.
 */
#define ISIS_ANTENNA_CMD_REPORT_DEPLOY_COUNTER_ANT_2    0xB1

/**
 * \brief Report antenna 3 deployment system activation count.
 * 
 * Description: Reports the number of times the deployment system for antenna 3
 * has been activated since the last reset of the microcontroller.
 * 
 * Parameters: None
 * 
 * Response: [000 - 000] xxxxxxxx. The number of deployment system activations.
 * This number has a minimum value of 0 and a maximum value of 255.
 */
#define ISIS_ANTENNA_CMD_REPORT_DEPLOY_COUNTER_3        0xB2

/**
 * \brief Report antenna 4 deployment system activation count.
 * 
 * Description: Reports the number of times the deployment system for antenna 4
 * has been activated since the last reset of the microcontroller.
 * 
 * Parameters: None
 * 
 * Response: [000 - 000] xxxxxxxx. The number of deployment system activations.
 * This number has a minimum value of 0 and a maximum value of 255.
 */
#define ISIS_ANTENNA_CMD_REPORT_DEPLOY_COUNTER_4        0xB3

/**
 * \brief Report antenna 1 deployment system activation time.
 * 
 * Description: Reports the total amount of time the deployment system for
 * antenna 1 has been active since the last reset of the microcontroller.
 * Please note that this value is cumulative, i.e. it sums the activation
 * times for separate deployment system activations.
 * 
 * Parameters: None
 * 
 * Response: [000 - 001] xxxxxxxx xxxxxxxx
 * The cumulative activation time of the deployment system in 50 ms steps. The
 * minimum value for this time is 0 and the maximum value is 65535. To obtain
 * the activation time in seconds, divide this number by 20.
 */
#define ISIS_ANTENNA_CMD_REPORT_DEPLOY_TIMER_1          0xB4

/**
 * \brief Report antenna 2 deployment system activation time.
 * 
 * Description: Reports the total amount of time the deployment system for
 * antenna 2 has been active since the last reset of the microcontroller.
 * Please note that this value is cumulative, i.e. it sums the activation
 * times for separate deployment system activations.
 * 
 * Parameters: None
 * 
 * Response: [000 - 001] xxxxxxxx xxxxxxxx
 * The cumulative activation time of the deployment system in 50 ms steps. The
 * minimum value for this time is 0 and the maximum value is 65535. To obtain
 * the activation time in seconds, divide this number by 20.
 */
#define ISIS_ANTENNA_CMD_REPORT_DEPLOY_TIMER_2          0xB5

/**
 * \brief Report antenna 3 deployment system activation time.
 * 
 * Description: Reports the total amount of time the deployment system for
 * antenna 3 has been active since the last reset of the microcontroller.
 * Please note that this value is cumulative, i.e. it sums the activation
 * times for separate deployment system activations.
 * 
 * Parameters: None
 * 
 * Response: [000 - 001] xxxxxxxx xxxxxxxx
 * The cumulative activation time of the deployment system in 50 ms steps. The
 * minimum value for this time is 0 and the maximum value is 65535. To obtain
 * the activation time in seconds, divide this number by 20. 
 */
#define ISIS_ANTENNA_CMD_REPORT_DEPLOY_TIMER_3          0xB6

/**
 * \brief Report antenna 4 deployment system activation time.
 * 
 * Description: Reports the total amount of time the deployment system for
 * antenna 4 has been active since the last reset of the microcontroller.
 * Please note that this value is cumulative, i.e. it sums the activation
 * times for separate deployment system activations.
 * 
 * Parameters: None
 * 
 * Response: [000 - 001] xxxxxxxx xxxxxxxx
 * The cumulative activation time of the deployment system in 50 ms steps. The
 * minimum value for this time is 0 and the maximum value is 65535. To obtain
 * the activation time in seconds, divide this number by 20.
 */
#define ISIS_ANTENNA_CMD_REPORT_DEPLOY_TIMER_4          0xB7

#endif // ISIS_ANTENNA_CMDS_H_

//! \} End of isis_antenna_cmds group
