
/*
 Routines for CBUS FLiM operations - part of CBUS libraries for PIC 18F
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
**************************************************************************************************************
	The FLiM routines have no code or definitions that are specific to any
	module, so they can be used to provide FLiM facilities for any module 
	using these libraries.
	
*/ 
/* 
 * File:   digitalOut.c
 * Author: Ian
 * 
 * Handle simple digital outputs. Including pulsed outputs and
 * generate Produced events.
 * 
 * The current output Action is saved in EEPROM in EE_OP_STATE. This is one of:
 * <ul>
 * <li>ACTION_IO_2 for ON</li>
 * <li>ACTION_IO_3 for OFF and pulse</li>
 * <li>ACTION_IO_4 for flash</li>
 * </ul>
 *
 * Created on 1 June 2017, 13:14
 * 
 * Revision by Greg Palmer
 * 13 April 2021
 *
 *	Modified calls to sendInvertedProducedEvent to have additional arguments.
 *	see universalEvents.c for a description of the change.
 *
*/

#include <xc.h>

#include "nvm.h"
#include "nv.h"
#include "ticktime.h"
#include "event_producer.h"

#include "universalEEPROM.h"
#include "universalNv.h"
#include "universalEvents.h"
#include "config.h"
#include "config.h"
#include "digitalOut.h"

/* For pulsed outputs the array pulseDelays is used for the countdown timer. This
 * is also used for state i.e. a value of 1 is used for COMPLETED and a value of 0 
 * no current pulse (needs starting). 
 * TODO Perhaps this state information should go into
 * a generic channel state variable and combined with servoState, this would save 
 * a few bytes of RAM. 
 */
uint8_t pulseDelays[NUM_IO];
int8_t flashDelays[NUM_IO];

// Forward declarations
void setOutputPin(uint8_t io, Boolean state);

// Externs

/**
 * Initialise the IO digital outputs.
 * Set up the pulse data structures.
 */
void initOutputs(void) {
    // probably initialised to 0 by the compiler but make sure here
    uint8_t io;
    for (io=0; io<NUM_IO; io++) {
       pulseDelays[io] = NEEDS_STARTING;
       flashDelays[io] = 0;
    }
}
/**
 * Start a digital output. Handles inverted outputs and pulsed outputs. Sends the
 * produced events.
 * Handles:
 * * Output event
 * * Output OFF event disable
 * * Output event invert
 * * Flash
 * * Pulse
 * 
 * @param io
 * @param action
 */
void startDigitalOutput(uint8_t io, uint8_t state) {
    Boolean pinState;
    Boolean actionState;
    uint8_t nvPulseDuration;
    // State ACTION_IO_1 is Change. This is not used here and state will have been changed to on or off
    // State ACTION_IO_2 is ON
    // State ACTION_IO_3 is OFF
    // State ACTION_IO_4 is Flash
    // State ACTION_IO_5 is Change inverted. This is not used here and state will have been changed to on or off

    if (state == ACTION_IO_4) {
        flashDelays[io] = (int8_t)getNV(NV_IO_OUTPUT_FLASH_PERIOD(io));
        pulseDelays[io] = COMPLETED;
        setOutputPin(io, ! getNV(NV_IO_FLAGS(io) & FLAG_OUTPUT_ACTION_INVERTED));
        writeNVM(EEPROM_NVM_TYPE, (eeprom_address_t)EE_OP_STATE+io, state);	// save the current state of output
        sendInvertedProducedEvent(HAPPENING_IO_INPUT(io), EVENT_ON, 
                getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED, TRUE, TRUE);
        return;
    }
    flashDelays[io] = 0;	// turn flash off
    // state is either ACTION_IO_2(on) or ACTION_IO_3(off))
    actionState = (state == ACTION_IO_2);
  
    // Check if the input event is inverted
    if (getNV(NV_IO_FLAGS(io)) & FLAG_TRIGGER_INVERTED) {
        actionState = !actionState;
    }
#ifdef LEDSW
    if ((uint8_t)getNV(NV_IO_TYPE(io)) == TYPE_LEDSW) {
        nvPulseDuration = 0;
    } else 
#endif
        nvPulseDuration = (uint8_t)getNV(NV_IO_OUTPUT_PULSE_DURATION(io));
    // ignore OFF on pulse outputs
    if (( ! actionState) && nvPulseDuration) {
        pulseDelays[io] = COMPLETED;
        return;
    }
    
    // Save state in EEPROM
    // Was this a ON and we have a pulse duration defined and this is the start of the pulse?
    if ((actionState) && nvPulseDuration && (pulseDelays[io] == NEEDS_STARTING)) {
        pulseDelays[io] = nvPulseDuration;
        // save the current state of output as OFF so 
        // we don't power up with pulse active 
        writeNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io, ACTION_IO_3);	// save the current state of output
    } else {
        pulseDelays[io] = COMPLETED;
        writeNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io, actionState?ACTION_IO_2:ACTION_IO_3);	// save the current state of output
    }

    pinState = actionState;
    if (getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_ACTION_INVERTED) {
        pinState = !pinState;
    }
    setOutputPin(io, pinState);
    
    if (getNV(NV_IO_TYPE(io)) == TYPE_OUTPUT) {
        // Send events
        // check if OFF events are enabled
        if (getNV(NV_IO_FLAGS(io)) & FLAG_DISABLE_OFF) {
            if (actionState) {
                // only ON
                // check if produced event is inverted
                sendInvertedProducedEvent(HAPPENING_IO_INPUT(io), actionState?EVENT_ON:EVENT_OFF, 
                        getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED, TRUE, TRUE);
            }
        } else {
            // check if produced event is inverted
            sendInvertedProducedEvent(HAPPENING_IO_INPUT(io), actionState?EVENT_ON:EVENT_OFF, 
                    getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED, TRUE, TRUE);
        }
    }
}

/**
 * Called regularly to handle pulse and flash.
 */
void processOutputs(void) {
    uint8_t io;
    for (io=0; io<NUM_IO; io++) {
        if ((getNV(NV_IO_TYPE(io)) == TYPE_OUTPUT) 
#ifdef LEDSW
                || (getNV(NV_IO_TYPE(io)) == TYPE_LEDSW)
#endif
                ) {
            // Handle the FLASH toggle
            if (flashDelays[io] == 1) {
                setOutputPin(io, getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_ACTION_INVERTED);
                flashDelays[io] = (int8_t)getNV(NV_IO_OUTPUT_FLASH_PERIOD(io));
                flashDelays[io] = - flashDelays[io];
            }
            if (flashDelays[io] == -1) {
                setOutputPin(io, ! (getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_ACTION_INVERTED));
                flashDelays[io] = (int8_t)getNV(NV_IO_OUTPUT_FLASH_PERIOD(io));
            }
            if (flashDelays[io] > 1) {
                flashDelays[io]--;
            } else if (flashDelays[io] < -1) {
                flashDelays[io]++;
            }
        }
        if (getNV(NV_IO_TYPE(io)) == TYPE_OUTPUT) {
            // Handle PULSEd outputs
            if (pulseDelays[io] != NEEDS_STARTING) {
                pulseDelays[io]--;
            
                // PULSE finished
                if (pulseDelays[io] == COMPLETED) {
                    // time to go off
                    if (getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_ACTION_INVERTED) {
                        setOutputPin(io, TRUE);
                    } else {
                        setOutputPin(io, FALSE);
                    }
                    // check if OFF events are enabled
                    if ( ! (getNV(NV_IO_FLAGS(io)) & FLAG_DISABLE_OFF)) {
                        // check if produced event is inverted
                        sendProducedEvent(HAPPENING_IO_INPUT(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF); 
                    }
                }
            }
        }
    }
}

/**
 * Update the state of a digital output based upon an Action request.
 * 
 * @param io
 * @param action
 */
void setDigitalOutput(uint8_t io, uint8_t state) {
    Boolean pinState;
    switch (state) {
        default: // After TYPE change could be invalid
            state = ACTION_IO_3;   // default OFF
            // fall through
        case ACTION_IO_2:
        case ACTION_IO_3:
            pinState = (state == ACTION_IO_2);
//            if (getNV(NV_IO_FLAGS(io)) & FLAG_TRIGGER_INVERTED) {
//                pinState = pinState?0:1;
//            }
            if (getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_ACTION_INVERTED) {
                setOutputPin(io, ! pinState);
            } else {
                setOutputPin(io, pinState);
            }
            break;
        case ACTION_IO_4:
            flashDelays[io] = (int8_t)getNV(NV_IO_OUTPUT_FLASH_PERIOD(io));
            setOutputPin(io, TRUE);
            break;
    }
}

/**
 * Set a particular output pin to the given state.
 * @param io
 * @param state
 */
void setOutputPin(uint8_t io, Boolean state) {
    switch (configs[io].port) {
        case 'A':
            if (state) {
                // set it
               LATA |= (1<<configs[io].no);
            } else {
                // clear it
                LATA &= ~(1<<configs[io].no);
            }
            break;
        case 'B':
            if (state) {
                // set it
                LATB |= (1<<configs[io].no);
            } else {
                // clear it
                LATB &= ~(1<<configs[io].no);
            }
            break;
        case 'C':
            if (state) {
                // set it
                LATC |= (1<<configs[io].no);
            } else {
                // clear it
                LATC &= ~(1<<configs[io].no);
            }
            break;
#ifdef CANXIO            
        case 'D':
            if (state) {
                // set it
                LATD |= (1<<configs[io].no);
            } else {
                // clear it
                LATD &= ~(1<<configs[io].no);
            }
            break;  
        case 'E':
            if (state) {
                // set it
                LATE |= (1<<configs[io].no);
            } else {
                // clear it
                LATE &= ~(1<<configs[io].no);
            }
            break;   
#endif            
    }
}

