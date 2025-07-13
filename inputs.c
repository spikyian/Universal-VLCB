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
**************************************************************************************************************
*/ 
/* 
 * File:   inputs.c
 * Author: Ian
 * 
 * Handle in input Type variant of the IO. Scan for changes in state and 
 * generate Produced events.
 *
 * Created on 17 April 2017, 13:14
 */

#include <stddef.h>

#include "nvm.h"
#include "event_producer.h"
#include "nv.h"

#include "universalNv.h"
#include "module.h"
#include "config.h"
#include "universalEvents.h"
#include "universalEEPROM.h"

/**
 * The current state of the input pins. This may not be the actual read state uas we
 * could still be doing the debounce. Instead this is the currently reported input state.
 */
static uint8_t inputState[NUM_IO];
/**
 * The effective state of the inputs after handling toggle. 
 */
uint8_t outputState[NUM_IO];
/*
 * Counts the number of cycles since the input changed state.
 */
static uint8_t delayCount[NUM_IO];

// forward declarations
Boolean readInput(uint8_t io);
// externs

static uint8_t io;

/**
 * Initialise the input scan.
 * Initialise using the current input state so that we don't generate state 
 * change events on power up.
 */
void initInputScan(void) {
    for (io=0; io<NUM_IO; io++) {
        uint8_t input = (uint8_t)readInput(io);
        inputState[io] = input;
        if (!(getNV(NV_IO_FLAGS(io)) & FLAG_TRIGGER_INVERTED)) {
            input = !input;
        }
        outputState[io] = input;
        if (getNV(NV_IO_FLAGS(io)) & FLAG_INPUT_TOGGLE) {
            outputState[io] = (uint8_t)readNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io);
        }
        delayCount[io] = 0;
    }
}

/**
 * Called regularly to check for changes on the inputs.
 * Generate Produced events upon input change.
 * Handles:
 * * Input pin invert
 * * Toggle
 * * Output OFF event disable
 * * Output event invert
 *   
 */
void inputScan(void) {
    uint8_t type;
    uint8_t flags;
    
    for (io=0; io< NUM_IO; io++) {
        type = (uint8_t)getNV(NV_IO_TYPE(io));
        if (type == TYPE_INPUT) {
            uint8_t input = (uint8_t)readInput(io);
            if (input != inputState[io]) {
                Boolean change = FALSE;
                // check if we have reached the debounce count
                if (inputState[io] && (delayCount[io] == getNV(NV_IO_INPUT_ON_DELAY(io)))) {
                    change = TRUE;
                }
                if (!inputState[io] && (delayCount[io] == getNV(NV_IO_INPUT_OFF_DELAY(io)))) {
                    change = TRUE;
                }
                if (change) {
                    flags = (uint8_t)getNV(NV_IO_FLAGS(io));
                    // input been steady long enough to be treated as a real change
                    delayCount[io] = 0;
                    inputState[io] = input;
                    // check if input pin is inverted
                    if (!(flags & FLAG_TRIGGER_INVERTED)) {
                        input = !input;
                    }
                    // Check if toggle
                    if (flags & FLAG_INPUT_TOGGLE) {
                        if (input) {
                            outputState[io] = ! outputState[io];
                        } else {
                            continue;
                        }
                        writeNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io, outputState[io]);
                     } else {
                        outputState[io] = input;
                    }
                    
                    // check if OFF events are enabled
                    if (flags & FLAG_DISABLE_OFF) {
                        if (outputState[io]) {
                            // only ON
                            // check if produced event is inverted
                            if (flags & FLAG_RESULT_EVENT_INVERTED) {
                                sendProducedEvent(HAPPENING_IO_INPUT(io), EVENT_OFF);
                            } else {
                                sendProducedEvent(HAPPENING_IO_INPUT(io), EVENT_ON);
                            }
                        } else {
                            if (flags & FLAG_RESULT_EVENT_INVERTED) {
                                sendProducedEvent(HAPPENING_IO_INPUT_TWO_ON(io), EVENT_OFF);
                            } else {
                                sendProducedEvent(HAPPENING_IO_INPUT_TWO_ON(io), EVENT_ON);
                            }
                        }
                    } else {
                        // check if produced event is inverted
                        if (flags & FLAG_RESULT_EVENT_INVERTED) {
                            sendProducedEvent(HAPPENING_IO_INPUT(io), outputState[io]?EVENT_OFF:EVENT_ON);
                        } else {
                            sendProducedEvent(HAPPENING_IO_INPUT(io), outputState[io]?EVENT_ON:EVENT_OFF);
                        }
                    }
                } else {
                    delayCount[io]++;
                }
            } else {
                delayCount[io] = 0;
            }
        }
    }
}

/**
 * Read the input state from the IO pins.
 * @param io the IO number
 * @return Non zero is the input is high or FALSE if the input is low
 */
Boolean readInput(uint8_t io) {
    if (getNV(NV_IO_TYPE(io)) == TYPE_INPUT) {
            switch(configs[io].port) {
            case 'A':
                return PORTA & (1<<configs[io].no);
            case 'B':
                return PORTB & (1<<configs[io].no);
            case 'C':
                return PORTC & (1<<configs[io].no);
#ifdef CANXIO
            case 'D':
                return PORTD & (1<<configs[io].no);
            case 'E':
                return PORTE & (1<<configs[io].no);                
#endif                
            }
    }
    return FALSE;
}
