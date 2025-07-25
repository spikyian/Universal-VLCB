
/*
  This work is licensed under the:
      Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit:
      http://creativecommons.org/licenses/by-nc-sa/4.0/
   or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
   License summary:
    You are free to:
      Share, copy and redistribute the material in any medium or format
      Adapt, remix, transform, and build upon the material
    The licensor cannot revoke these freedoms as long as you follow the license terms.
    Attribution : You must give appropriate credit, provide a link to the license,
                   and indicate if changes were made. You may do so in any reasonable manner,
                   but not in any way that suggests the licensor endorses you or your use.
    NonCommercial : You may not use the material for commercial purposes. **(see note below)
    ShareAlike : If you remix, transform, or build upon the material, you must distribute
                  your contributions under the same license as the original.
    No additional restrictions : You may not apply legal terms or technological measures that
                                  legally restrict others from doing anything the license permits.
   ** For commercial use, please contact the original copyright holder(s) to agree licensing terms

*/ 
/* 
 * File:   nodeVarables.h
 * Author: 	Ian Hogg
 * Comments:	Definitions for NVs
 * Revision history: 
 * 
 * ---
 *
 * Revision by Greg Palmer
 * 15 April 2021
 *
 *	Added flag FLAG_INPUT_DISABLE_SOD_RESPONSE value 0x20
 *  This flag overlays FLAG_STARTUP which has no meaning for input types 
 *  Enables a feature that exists in CANACE8C
 *	Applicable to Input types - input, analogue, and magnet to allow users to supress SOD messages
 *    for the channel.
 *
 * --
 */

#ifndef XC_NODE_VARIABLES_H
#define	XC_NODE_VARIABLES_H

/**
 * This is where the module specific NVs are specified.
 * NVs can be accessed either by byte offset (for read/set of NVs in FLiM.c)
 * or by use of a structure with named elements. These two must be kept synchronised.
 * 
 * The following are required by FLiM.c: NV_NUM, AT_NV
 */

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
#include "module.h"
#include "nv.h"

#define FLASH_VERSION   0x02        // Version 2 introduced when Actions were changed to have 5 per channel
    
// Global NVs
#define NV_VERSION                      0
#define NV_SOD_DELAY                    1
#define NV_HB_DELAY                     2  
#define NV_SERVO_SPEED                  3   // Used for Multi and Bounce types where there isn't an NV to define speed.
#define NV_PULLUPS                      4
#define NV_RESPONSE_DELAY               5
#define NV_LOW_CHANNELS_PULLUPS         6
#define NV_MID_CHANNELS_PULLUPS         7
#define NV_TOP_CHANNELS_PULLUPS         8
#define NV_CDU_CHARGE_TIME              9
#define NV_MODULE_FLAGS                 10
#define NV_SPARE8                       11
#define NV_SPARE9                       12
#define NV_SPARE10                      13
#define NV_SPARE11                      14
#define NV_SPARE12                      15
#define NV_IO_START                     16
#define NVS_PER_IO                      7
    
// NVs per IO
#define NV_IO_TYPE_OFFSET               0
#define NV_IO_FLAGS_OFFSET              1
#define NV_IO_TYPE(i)                   (NV_IO_START + NVS_PER_IO*(i) + NV_IO_TYPE_OFFSET)		// TYPE and FLAGS always at the start
#define NV_IO_FLAGS(i)                  (NV_IO_START + NVS_PER_IO*(i) + NV_IO_FLAGS_OFFSET)

// Other NVs depend upon type
#define NV_IO_INPUT_ON_DELAY_OFFSET     2
#define NV_IO_INPUT_OFF_DELAY_OFFSET    3
#define NV_IO_INPUT_ON_DELAY(i)         (NV_IO_START + NVS_PER_IO*(i) + NV_IO_INPUT_ON_DELAY_OFFSET)	// units of 5ms
#define NV_IO_INPUT_OFF_DELAY(i)        (NV_IO_START + NVS_PER_IO*(i) + NV_IO_INPUT_OFF_DELAY_OFFSET)	// units of 5ms

#define NV_IO_OUTPUT_PULSE_DURATION_OFFSET 2
#define NV_IO_OUTPUT_FLASH_PERIOD_OFFSET 3
#define NV_IO_OUTPUT_PULSE_DURATION(i)  (NV_IO_START + NVS_PER_IO*(i) + NV_IO_OUTPUT_PULSE_DURATION_OFFSET)	// units of 0.1 seconds
#define NV_IO_OUTPUT_FLASH_PERIOD(i)    (NV_IO_START + NVS_PER_IO*(i) + NV_IO_OUTPUT_FLASH_PERIOD_OFFSET)	// units of 0.1 seconds

#define NV_IO_SERVO_START_POS_OFFSET    2
#define NV_IO_SERVO_END_POS_OFFSET      3
#define NV_IO_SERVO_SE_SPEED_OFFSET     4
#define NV_IO_SERVO_ES_SPEED_OFFSET     5
#define NV_IO_SERVO_START_POS(i)        (NV_IO_START + NVS_PER_IO*(i) + NV_IO_SERVO_START_POS_OFFSET)
#define NV_IO_SERVO_END_POS(i)          (NV_IO_START + NVS_PER_IO*(i) + NV_IO_SERVO_END_POS_OFFSET)
#define NV_IO_SERVO_SE_SPEED(i)         (NV_IO_START + NVS_PER_IO*(i) + NV_IO_SERVO_SE_SPEED_OFFSET)	// position moved every 100ms
#define NV_IO_SERVO_ES_SPEED(i)         (NV_IO_START + NVS_PER_IO*(i) + NV_IO_SERVO_ES_SPEED_OFFSET)	// position moved every 100ms

#define NV_IO_BOUNCE_UPPER_POS_OFFSET   2
#define NV_IO_BOUNCE_LOWER_POS_OFFSET   3
#define NV_IO_BOUNCE_COEFF_OFFSET       4
#define NV_IO_BOUNCE_PULL_SPEED_OFFSET  5
#define NV_IO_BOUNCE_PULL_PAUSE_OFFSET  6
#define NV_IO_BOUNCE_UPPER_POS(i)       (NV_IO_START + NVS_PER_IO*(i) + NV_IO_BOUNCE_UPPER_POS_OFFSET)
#define NV_IO_BOUNCE_LOWER_POS(i)       (NV_IO_START + NVS_PER_IO*(i) + NV_IO_BOUNCE_LOWER_POS_OFFSET)
#define NV_IO_BOUNCE_COEFF(i)           (NV_IO_START + NVS_PER_IO*(i) + NV_IO_BOUNCE_COEFF_OFFSET)	// Starting speed in other direction
#define NV_IO_BOUNCE_PULL_SPEED(i)         (NV_IO_START + NVS_PER_IO*(i) + NV_IO_BOUNCE_PULL_SPEED_OFFSET)
#define NV_IO_BOUNCE_PULL_PAUSE(i)         (NV_IO_START + NVS_PER_IO*(i) + NV_IO_BOUNCE_PULL_PAUSE_OFFSET)

#define NV_IO_MULTI_NUM_POS_OFFSET      2
#define NV_IO_MULTI_POS1_OFFSET         3
#define NV_IO_MULTI_POS2_OFFSET         4
#define NV_IO_MULTI_POS3_OFFSET         5
#define NV_IO_MULTI_POS4_OFFSET         6
#define NV_IO_MULTI_NUM_POS(i)          (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MULTI_NUM_POS_OFFSET)
#define NV_IO_MULTI_POS1(i)             (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MULTI_POS1_OFFSET)
#define NV_IO_MULTI_POS2(i)             (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MULTI_POS2_OFFSET)
#define NV_IO_MULTI_POS3(i)             (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MULTI_POS3_OFFSET)
#define NV_IO_MULTI_POS4(i)             (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MULTI_POS4_OFFSET)
 
#define NV_IO_ANALOGUE_THRESHOLD_OFFSET        3
#define NV_IO_ANALOGUE_HYSTERESIS_OFFSET       4
#define NV_IO_ANALOGUE_THRES(i)         (NV_IO_START + NVS_PER_IO*(i) + NV_IO_ANALOGUE_THRESHOLD_OFFSET)
#define NV_IO_ANALOGUE_HYST(i)          (NV_IO_START + NVS_PER_IO*(i) + NV_IO_ANALOGUE_HYSTERESIS_OFFSET)
 
#define NV_IO_MAGNET_DOSETUP_OFFSET          2
#define NV_IO_MAGNET_THRESHOLD_OFFSET        3
#define NV_IO_MAGNET_HYSTERESIS_OFFSET       4
#define NV_IO_MAGNET_OFFSET_H_OFFSET         5
#define NV_IO_MAGNET_OFFSET_L_OFFSET         6
#define NV_IO_MAGNET_SETUP(i)         (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MAGNET_DOSETUP_OFFSET)
#define NV_IO_MAGNET_THRES(i)         (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MAGNET_THRESHOLD_OFFSET)
#define NV_IO_MAGNET_HYST(i)          (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MAGNET_HYSTERESIS_OFFSET)
#define NV_IO_MAGNET_OFFSETH(i)       (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MAGNET_OFFSET_H_OFFSET)
#define NV_IO_MAGNET_OFFSETL(i)       (NV_IO_START + NVS_PER_IO*(i) + NV_IO_MAGNET_OFFSET_L_OFFSET)
    
#define NV_IO_CDU_PLSE_TIME_OFFSET           2
#define NV_IO_CDU_PULSE_TIME(i)       (NV_IO_START + NVS_PER_IO*(i) + NV_IO_CDU_PLSE_TIME_OFFSET)
    
#define NV_IO_LEDSW_THRESHOLD_OFFSET         2
#define NV_IO_LEDSW_FLASH_PERIOD_OFFSET      3
#define NV_IO_LEDSW_THRESHOLD(i)      (NV_IO_START + NVS_PER_IO*(i) + NV_IO_LEDSW_THRESHOLD_OFFSET)
#define NV_IO_LEDSW_FLASH_PERIOD(i)   (NV_IO_START + NVS_PER_IO*(i) + NV_IO_LEDSW_FLASH_PERIOD_OFFSET)
    
#define IS_NV_TYPE(i)                   (((i-NV_IO_START) % NVS_PER_IO) == 0)
#define IO_NV(i)                        ((uint8_t)((i-NV_IO_START)/NVS_PER_IO))
#define NV_NV(i)                        ((uint8_t)((i-NV_IO_START) % NVS_PER_IO))
  
// the types
#define TYPE_INPUT                  0
#define TYPE_OUTPUT                 1
#define TYPE_SERVO                  2
#define TYPE_BOUNCE                 3
#define TYPE_MULTI                  4
#define TYPE_ANALOGUE_IN            5
#define TYPE_MAGNET                 6
#define TYPE_CDU                    7
#define TYPE_RAILCOM                8
#define TYPE_LEDSW                  9

// the module flags
#define MODULE_FLAG_DEFAULT_EVENTS      0x01
    
// the channel flags
#define	FLAG_TRIGGER_INVERTED           0x01	// Whether the sense of this input or output inverted
#define FLAG_DISABLE_OFF                0x08    // Whether off events are generated
#define FLAG_RESULT_EVENT_INVERTED      0x40    // whether the generated event is inverted
    
#define FLAG_INPUT_TOGGLE               0x10    // whether an INPUT operates as a pushbutton toggle
#define	FLAG_INPUT_DISABLE_SOD_RESPONSE 0x20    // Whether this input, magnet or analogue channel responds to SOD

#define	FLAG_OUTPUT_STARTUP             0x04	// Whether this output be changed on module start to match values in EE_OP_STATE
#define FLAG_OUTPUT_ACTION_INVERTED     0x20    // whether the resulting action is inverted
#define FLAG_OUTPUT_EXPEDITED           0x80    // whether consumed actions are expedited

#define	FLAG_SERVO_CUTOFF               0x02	// Whether the servo cut off after 1 second once it has reached its target position
#define FLAG_SERVO_START_PULLUP         0x10    // Start servos high or low. Reuse Toggle flag
#define FLAG_SERVO_EXTENDED_TRAVEL      0x80    // 0-90 or 0-180 degree rotation


typedef struct {
    uint8_t type;
    uint8_t flags;
    union {
        struct {
            uint8_t input_on_delay;
            uint8_t input_off_delay;
        } nv_input;
        struct {
            uint8_t output_pulse_duration;
            uint8_t output_flash_period;
        } nv_output;
        struct {
            uint8_t servo_start_pos;
            uint8_t servo_end_pos;
            uint8_t servo_se_speed;
            uint8_t servo_es_speed;
        } nv_servo;
        struct {
            uint8_t bounce_upper_pos;
            uint8_t bounce_lower_pos;
            uint8_t bounce_coeff;
            uint8_t bounce_pull_speed;
            uint8_t bounce_pull_pause;
        } nv_bounce;
        struct {
            uint8_t multi_num_pos;
            uint8_t multi_pos1;
            uint8_t multi_pos2;
            uint8_t multi_pos3;
            uint8_t multi_pos4;
        } nv_multi;
        struct {
            uint8_t analogue_unused;
            uint8_t analogue_threshold;
            uint8_t analogue_hysteresis;
        } nv_analogue_in;
        struct {
            uint8_t magnet_setup;
            uint8_t magnet_threshold;
            uint8_t magnet_hysteresis;
            uint8_t magnet_offset_h;
            uint8_t magnet_offset_l;
        } nv_magnet;
    } nv_io;
} NvIo;

/*
 * This structure is required by FLiM.h
 */
typedef struct {
        uint8_t nv_version;                // 0 version of NV structure
        uint8_t sendSodDelay;              // 1 Time after start in 100mS (plus 2 seconds) to send an automatic SoD. Set to zero for no auto SoD
        uint8_t hbDelay;                   // 2 Interval in 100mS for automatic heartbeat. Set to zero for no heartbeat.
        uint8_t servo_speed;               // 3 default servo speed
        uint8_t pullups;                   // 4 weak pullup resistors
        uint8_t responseDelay;             // 5 timing of multiple responses
        uint8_t xio_pullupsL;               // 6 low pullups
        uint8_t xio_pullupsM;               // 7 mid pullups
        uint8_t xio_pullupsH;               // 8 top pullups
        uint8_t cdu_chargePumpFreq;        // 9 CANCDU charge pump frequency
        uint8_t spare[6];
        NvIo io[NUM_IO];                 // config for each IO
} ModuleNvDefs;

extern void defaultNVs(uint8_t i, uint8_t type);     

// Default type is INPUT for all on CANMIO, 
// on the CANBIP it is OUTPUT for the main board and INPUT for the expansion (daughterboard) connections)
#ifndef TYPE_DEFAULT_MAIN
#ifdef CANBIP
#define TYPE_DEFAULT_MAIN   TYPE_OUTPUT
#else
#ifdef CANCDU
#define TYPE_DEFAULT_MAIN   TYPE_CDU
#else
#define TYPE_DEFAULT_MAIN   TYPE_INPUT
#endif

#define TYPE_DEFAULT_EXP    TYPE_INPUT
#endif
#endif

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_NODE_VARAIABLES_H */

