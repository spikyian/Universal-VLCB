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
 * File:   cdu.c
 * Author: Ian
 * 
 * Handle simple CDU outputs. 
 * 
 * The current output Action is one of:
 * <ul>
 * <li>ACTION_IO_2 for ON</li>
 * <li>ACTION_IO_3 for OFF</li>
 * </ul>
 * The variables from digitalOutput pulseDelays and flashDelays ane reused (misused)
 * here to save creating more variables and allowing some of the code from 
 * digitalOutput to be reused. PulseDelays are used to time the pulse of the CDU
 * output, the same as that done by digitalOutput but here we also need to turn off
 * the charging of the CDU. Once the pulse has completed we turn the CDU charging
 * back on and start the recharge time misusing the flashDelays variable for this.
 * When flashDelays is completed we mark the Action as done and restore the pulseDelays
 * state ready for the next use.
 *
 * Created on 11 June 2024, 17:57
 *
 *
 */

#include <xc.h>

#include "nvm.h"
#include "nv.h"
#include "ticktime.h"

#include "universalNv.h"
#include "universalEvents.h"
#include "digitalOut.h"
#include "cdu.h"

/*
 * TODO
 * No simultaneous CDUs
 * Action processing CDU outputs not working
 * Hash table not being loaded after adding default events
 */

/*
 * Since only a max of 1 CDU pulsed output should be active at any time it would be
 * possible to use a pair of variables for the pulse delay and recharge delay but
 * here we reuse those from digitalOutput to save the declaration of more variables.
 */

extern uint8_t pulseDelays[NUM_IO]; // used for pulse time
extern int8_t flashDelays[NUM_IO]; // used for the recharge time
extern void doneTwoAction(void);
/*
 * For pulsed outputs the array pulseDelays is used for the countdown timer. This
 * is also used for state i.e. a value of 1 is used for COMPLETED and a value of 0 
 * no current pulse (needs starting).
 */

/**
 * Since we share the arrays used by digitalOutput the initialisation is done
 * by initOutputs() and we don't need any additional work here.
 */
void initCdus(void){
    // start the charge pump clock
    // Uses PWM4 and uses PPS to output to RA5

    // PWM4 left aligned mode Slice 1 using LF INT running at 50% duty cycle 100Hz
    PWM4CLKbits.CLK = 4;    // LF INT clock source
    PWM4CPRE = 162;         // clock prescalar gets to 100Hz
    PWM4PR = 1;             // period register
    PWM4S1P1 = 1;           // Half period for square wave
    PWM4S1P2 = 1;           // Half period for square wave
    PWM4CONbits.LD = 1;     // reload registers
    
    // set RA3 as output for the enable 
    TRISAbits.TRISA3 = 0;
    LATAbits.LATA3 = 0;
    // set RA5 to input so that PWM4 can drive it as an output via PPS
    /* According to datasheet:
     With few exceptions, the port TRIS control associated with that pin
     retains control over the pin output driver. Peripherals that control 
     the pin output driver as part of the peripheral operation will override 
     the TRIS control as needed.
     * Very unhelpful.
     */
    TRISAbits.TRISA5 = 0;
    // output PWM4 to pin RA5 channel 15
    RA5PPS = 0x1E;  // 1E is PWM4S1P1_OUT
    // enable the charge pump clock
    PWM4CONbits.EN = 1;     // enable
    LATAbits.LATA3 = 1;     // turn the charger on
}

/**
 * Gets called to start an Action and should, for example, turn an output on.
 * We can reuse the functionality 
 * 
 * @param io
 * @param state
 */
void startCduOutput(uint8_t io, uint8_t state){
    // State ACTION_IO_1 is Change. This is not used here and state will have been changed to on or off
    // State ACTION_IO_2 is ON
    // State ACTION_IO_3 is OFF
    // State ACTION_IO_4 is Flash
    // State ACTION_IO_5 is Change inverted. This is not used here and state will have been changed to on or off
    Boolean actionState;
    
    if ((state == ACTION_IO_2) || (state == ACTION_IO_3)) {
        // state is either ACTION_IO_2(on) or ACTION_IO_3(off))
        actionState = (state == ACTION_IO_2);
  
        // Check if the input event is inverted
        if (getNV(NV_IO_FLAGS(io)) & FLAG_TRIGGER_INVERTED) {
            actionState = !actionState;
        }
        if (actionState) {
            LATAbits.LATA3 = 0; //  disable the charger
            // this will set the pulseDelays[] to the required time
            //and also set flashDelays[] to 0
            startDigitalOutput(io, state);
        } else {
            // An OFF Action should complete immediately
            flashDelays[io] = COMPLETED;
        }
    }
}

/**
 * Update the state of a CDU output based upon an Action request.
 * 
 * @param io the channel
 * @param action set to on or off
 */
void setCduOutput(uint8_t io, uint8_t action){
    if ((action == ACTION_IO_2) || (action == ACTION_IO_3)) {
        setDigitalOutput(io, action);
    }
}

/**
 * Called regularly every 10ms to handle pulse and recharge delay.
 * 
 */
void processCduPulses(void) {
    uint8_t io;
    for (io=0; io<NUM_IO; io++) {
        if (getNV(NV_IO_TYPE(io)) == TYPE_CDU) {
            if (pulseDelays[io] > COMPLETED) {
                pulseDelays[io]--;
            
                // Handle end of CDU pulse
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
                        sendProducedEvent(HAPPENING_IO_INPUT(io), getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED); 
                    }
                    LATAbits.LATA3 = 1;     // re-enable the charger
                    // Now start the recharge timer
                    flashDelays[io] = (int8_t)(getNV(NV_CDU_CHARGE_TIME)+1)/10 +1;
                }
            }
            // handle the recharge timer.
            // Once this reaches COMPLETED the Action will be marked as done
            // by processActions()
            if (flashDelays[io] > COMPLETED) {
                flashDelays[io]--;
            }
        }
    }
}

void finaliseCduOutput(uint8_t io) {
    pulseDelays[io] = flashDelays[io] = NEEDS_STARTING;
}