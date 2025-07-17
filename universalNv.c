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
 * File:   nodeVariables.c
 * Author: Ian Hogg
 *
 * Created on 05 March 2016, 18:14
 */

/**
 * Node variables contain global parameters that need to be persisted to Flash.
 */

#include "module.h"
#include "universalNv.h"
#include "universalEEPROM.h"
#ifdef SERVO
#include "universalEvents.h"
#endif
#include "nvm.h"

#include "servo.h"
#include "config.h"
#include "analogue.h"
#include "nv.h"

extern void setType(uint8_t i, uint8_t type);

/*
 * A table of default NV values.
 */
static const uint8_t channelDefaultNVs[11][NVS_PER_IO] = {
    {0,6,4,4,0,0,0},          // INPUT
    {1,6,0,0,0,0,0},          // OUTPUT
    {2,6,128,128,235,235,0},  // SERVO
    {3,6,128,127,64,3,60},    // BOUNCE
    {4,6,3,128,128,128,0},    // MULTI
    {5,6,128,16,0,0,0},       // ANALOGUE
    {6,6,0,123,32,7,255},     // MAGNET
    {7,6,5,0,0,0,0},          // CDU
    {8,0,0,0,0,0,0},          // RAILCOM
    {9,6,46,0,0,0,0},         // LEDSW
};

/**
 * The Application specific NV defaults are defined here.
 */
uint8_t APP_nvDefault(uint8_t index) {
    if (index < NV_IO_START) {
        // Global NVs
        switch(index) {
            case NV_SOD_DELAY:
                return 0;
            case NV_HB_DELAY:
                return 0;
            case NV_SERVO_SPEED:
                return 234;
            case NV_PULLUPS:
                return 0x33;
            case NV_RESPONSE_DELAY:
                return 2;
            case NV_LOW_CHANNELS_PULLUPS:
            case NV_MID_CHANNELS_PULLUPS:
            case NV_TOP_CHANNELS_PULLUPS:
                return 0xFF;
            case NV_CDU_CHARGE_TIME:
                return 20;
            default:
                return 0;
        }
    }
    // Now the channel defaults
    return channelDefaultNVs[(IO_NV(index) < NUM_IO_MAIN) ? TYPE_DEFAULT_MAIN : TYPE_DEFAULT_EXP][(index-NV_IO_START) % NVS_PER_IO];
} 

/**
 * We perform the necessary action when an NV changes value.
 */
void APP_nvValueChanged(uint8_t index, uint8_t value, uint8_t oldValue) {
    uint8_t nv;
    uint8_t io;
    
    if (IS_NV_TYPE(index)) {
        // If the IO type is changed then we need to do a bit or work
        io = index-NV_IO_START;
        io /= NVS_PER_IO;
        if (oldValue != value) {
            setType(io, value);
        }
    }
    if (index == NV_RESPONSE_DELAY) {
        setTimedResponseDelay(value);
    }
    // Now check if the pullups must be modified
#if defined(_18F66K80_FAMILY_)
    if (index == NV_PULLUPS) {
        WPUB = value; 
    }
#endif
#if defined(_18FXXQ83_FAMILY_)
    if (index == NV_LOW_CHANNELS_PULLUPS) {
        for (io=0; io<8; io++) {
            switch (configs[io].port) {
            case 'A':
                if (value && (1 << io % 8))
                    WPUA |= 1<<(configs[io].no);
                else
                    WPUA &= ~(1<<(configs[io].no));
                break;
            case 'B':
                if (value && (1 << io % 8))
                    WPUB |= 1<<(configs[io].no);
                else
                    WPUB &= ~(1<<(configs[io].no));
                break;
            case 'C':
                if (value && (1 << io % 8))
                    WPUC |= 1<<(configs[io].no);
                else
                    WPUC &= ~(1<<(configs[io].no));
                break;
            }
        }
    }
    if (index == NV_MID_CHANNELS_PULLUPS) {
        for (io=8; io<16; io++) {
            switch (configs[io].port) {
            case 'A':
                if (value && (1 << io % 8))
                    WPUA |= 1<<(configs[io].no);
                else
                    WPUA &= ~(1<<(configs[io].no));
                break;
            case 'B':
                if (value && (1 << io % 8))
                    WPUB |= 1<<(configs[io].no);
                else
                    WPUB &= ~(1<<(configs[io].no));
                break;
            case 'C':
                if (value && (1 << io % 8))
                    WPUC |= 1<<(configs[io].no);
                else
                    WPUC &= ~(1<<(configs[io].no));
                break;
            }
        }
    }
#ifdef CANXIO
    if (index == NV_TOP_CHANNELS_PULLUPS) {
        for (io=16; io<24; io++) {
            switch (configs[io].port) {
            case 'A':
                if (value && (1 << io % 8))
                    WPUA |= 1<<(configs[io].no);
                else
                    WPUA &= ~(1<<(configs[io].no));
                break;
            case 'B':
                if (value && (1 << io % 8))
                    WPUB |= 1<<(configs[io].no);
                else
                    WPUB &= ~(1<<(configs[io].no));
                break;
            case 'C':
                if (value && (1 << io % 8))
                    WPUC |= 1<<(configs[io].no);
                else
                    WPUC &= ~(1<<(configs[io].no));
                break;
            case 'D':
                if (value && (1 << io % 8))
                    WPUD |= 1<<(configs[io].no);
                else
                    WPUD &= ~(1<<(configs[io].no));
                break;
            case 'E':
                if (value && (1 << io % 8))
                    WPUE |= 1<<(configs[io].no);
                else
                    WPUE &= ~(1<<(configs[io].no));
                break;

            }
        }
    }  
#endif
#endif
    // Now some channel type specific functionality
    if (index >= NV_IO_START) {
        io = IO_NV(index);
        nv = NV_NV(index);
        switch(getNV(NV_IO_TYPE(io))) {
#ifdef ANALOGUE
            case TYPE_MAGNET:
                // if MAGNET setup is written then do the adc and save the result as the offset
                if (index == NV_IO_MAGNET_SETUP(io)) {
                    //setup variables so that the analogue poll will do the correct thing
                    setupIo = io;
                    setupState = (value & 0x80)?SETUP_REPORT_AND_SAVE:SETUP_REPORT;
                }
                break;
#endif
#ifdef SERVO
            case TYPE_SERVO:
                // if a servo position is changed then move servo to that position
                if (index == NV_IO_SERVO_START_POS(io)) {
                    setServoState(io, ACTION_IO_3);
                    startServoOutput(io, ACTION_IO_3);
                } else if (index == NV_IO_SERVO_END_POS(io)) {
                    setServoState(io, ACTION_IO_2);
                    startServoOutput(io, ACTION_IO_2);
                } else if (index == NV_IO_FLAGS(io)) {
                    if (((value & FLAG_SERVO_EXTENDED_TRAVEL) == FLAG_SERVO_EXTENDED_TRAVEL) &&
                            (oldValue & FLAG_SERVO_EXTENDED_TRAVEL) == 0) {
                        // The extended travel has just been set
                        // adjust current position to midway point so that the servo doesn't move much
                        setServoPosition(io, (uint8_t)(getNV(NV_IO_SERVO_END_POS(io))/2 + getNV(NV_IO_SERVO_START_POS(io))/2));
                    }
                }
                break;
#endif
#ifdef BOUNCE
            case TYPE_BOUNCE:
                if (index == NV_IO_BOUNCE_LOWER_POS(io)) {
                    setBounceState(io, ACTION_IO_3);
                    startBounceOutput(io, ACTION_IO_3);
                } else if (index == NV_IO_BOUNCE_UPPER_POS(io)) {
                    setBounceState(io, ACTION_IO_2);
                    startBounceOutput(io, ACTION_IO_2);
                } else if (index == NV_IO_FLAGS(io)) {
                    if (((value & FLAG_SERVO_EXTENDED_TRAVEL) == FLAG_SERVO_EXTENDED_TRAVEL) &&
                            (oldValue & FLAG_SERVO_EXTENDED_TRAVEL) == 0) {
                        // The extended travel has just been set
                        // adjust current position to midway point so that the servo doesn't move much
                        currentPos[io] = (uint8_t)(getNV(NV_IO_SERVO_END_POS(io))/2 + getNV(NV_IO_SERVO_START_POS(io))/2);
                    }
                }
                break;
#endif
#ifdef MULTI
            case TYPE_MULTI:
                if (index == NV_IO_MULTI_POS1(io)) {
                    setMultiState(io, ACTION_IO_1);
                    startMultiOutput(io, ACTION_IO_1);
                } else if (index == NV_IO_MULTI_POS2(io)) {
                    setMultiState(io, ACTION_IO_2);
                    startMultiOutput(io, ACTION_IO_2);
                } else if (index == NV_IO_MULTI_POS3(io)) {
                    setMultiState(io, ACTION_IO_3);
                    startMultiOutput(io, ACTION_IO_3);
                } else if (index == NV_IO_MULTI_POS4(io)) {
                    setMultiState(io, ACTION_IO_4);
                    startMultiOutput(io, ACTION_IO_4);
                } else if (index == NV_IO_FLAGS(io)) {
                    if (((value & FLAG_SERVO_EXTENDED_TRAVEL) == FLAG_SERVO_EXTENDED_TRAVEL) &&
                            ((oldValue & FLAG_SERVO_EXTENDED_TRAVEL) == 0)) {
                        // The extended travel has just been set
                        // adjust current position to midway point so that the servo doesn't move much
                        currentPos[io] = (uint8_t)(getNV(NV_IO_SERVO_END_POS(io))/2 + getNV(NV_IO_SERVO_START_POS(io))/2);
                    }
                }
                break;
#endif
        }
    }
}


/**
 * We validate NV values here.
 * We currently only validate the TYPE.
 */
NvValidation APP_nvValidate(uint8_t index, uint8_t value)  {
    uint8_t io;
    if ((index >= NV_IO_START) && IS_NV_TYPE(index)) {
        io = IO_NV(index);
#ifdef CANCDU
        if (io < NUM_IO_MAIN) {
            return INVALID;
        }
#endif
        switch (value) {
#ifdef ANALOGUE
            case TYPE_ANALOGUE_IN:
            case TYPE_MAGNET:
#if defined(_18F66K80_FAMILY_)
                if (configs[io].an == 0xFF) return INVALID;
#endif
                break;
#endif
#ifdef MULTI
            case TYPE_MULTI:
                break;
#endif
#ifdef BOUNCE
            case TYPE_BOUNCE:
                break;
#endif
#ifdef SERVO
            case TYPE_SERVO:
                break;
#endif
            case TYPE_OUTPUT:
            case TYPE_INPUT:
#ifdef LEDSW
            case TYPE_LEDSW:
#endif
                break;
            default:
                return INVALID;
        }
    }
    return VALID;
}

/**
 * Reset NVs for the specified channel back to default. 
 * @param io the channel
 * @param type the channel's new type
 */
void defaultNVs(uint8_t io, uint8_t type) {
    uint8_t i;
    
    for (i=1; i<NVS_PER_IO; i++) {
        saveNV(NV_IO_TYPE(io)+i , channelDefaultNVs[type][i]);
    }
    loadNvCache();
}