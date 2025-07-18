
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
 * File:   outputs.c
 * Author: Ian
 * 
 * Handle the various types of outputs. 
 * Call specific functions for the different output types.
 * This should really be an Interface or a super class but this is C
 *
 * Created on 17 April 2017, 13:14
 */

#include "module.h"

#include "config.h"
#include "ticktime.h"
#include "nvm.h"
#include "universalEEPROM.h"

#include "universalNv.h"
#include "universalEvents.h"
#include "servo.h"
#include "digitalOut.h"
#include "outputs.h"
#ifdef CANCDU
#include "cdu.h"
#endif

// Forward declarations


// Externs
extern void setOuputPin(uint8_t io, Boolean state);

extern uint8_t pulseDelays[NUM_IO];
extern int8_t flashDelays[NUM_IO];

/**
 * Start the processing of an Action.
 *  
 * @param i the IO
 * @param state on/off or position
 * @param type type of output
 */
void startOutput(uint8_t io, uint8_t act, uint8_t type) {
    switch(type) {
        case TYPE_INPUT:
            // this should never happen
            return;
        case TYPE_OUTPUT:
#ifdef LEDSW
        case TYPE_LEDSW:
#endif
            startDigitalOutput(io, act);
            return;
#ifdef BOUNCE
        case TYPE_BOUNCE:
            startBounceOutput(io, act);
            return;
#endif
#ifdef SERVO
        case TYPE_SERVO:
            startServoOutput(io, act);
            return;
#endif
#ifdef MULTI
        case TYPE_MULTI:
            startMultiOutput(io, act);
            return;
#endif
#ifdef CANCDU
        case TYPE_CDU:
            startCduOutput(io, act);
            return;
#endif
    }
}

/**
 * Set an output to the requested state.
 *  
 * @param i the IO
 * @param state on/off or position
 * @param type type of output
 */
void setOutputState(uint8_t io, uint8_t act, uint8_t type) {
    switch(type) {
        case TYPE_INPUT:
        case TYPE_ANALOGUE_IN:
        case TYPE_MAGNET:
            // this should never happen
            return;
        case TYPE_OUTPUT:
#ifdef CANCDU
        case TYPE_CDU:
#endif
#ifdef LEDSW
        case TYPE_LEDSW:
#endif
            return;
#ifdef BOUNCE
        case TYPE_BOUNCE:
            setBounceState(io, act);
            return;
#endif
#ifdef SERVO
        case TYPE_SERVO:
            setServoState(io, act);
            return;
#endif
#ifdef MULTI
        case TYPE_MULTI:
            setMultiState(io, act);
            return;
#endif

    }
}

/**
 * Set an output to the requested position Called during initialisation
 *  
 * @param i the IO
 * @param pos on/off or position
 * @param type type of output
 */
void setOutputPosition(uint8_t io, uint8_t pos, uint8_t type) {
    switch(type) {
        case TYPE_INPUT:
        case TYPE_ANALOGUE_IN:
        case TYPE_MAGNET:
            // this should never happen
            return;
        case TYPE_OUTPUT:
            setDigitalOutput(io, pos);
            return;
#ifdef BOUNCE
        case TYPE_BOUNCE:
#endif
#ifdef MULTI
        case TYPE_MULTI:
#endif
#ifdef SERVO
        case TYPE_SERVO:
            setServoPosition(io, pos);
            setOutputPin(io, getNV(NV_IO_FLAGS(io) & FLAG_OUTPUT_ACTION_INVERTED)?TRUE:FALSE);
            return;
#endif
#ifdef CANCDU
        case TYPE_CDU:
            setCduOutput(io, pos);
            return;
#endif
    }
}

/**
 * Indicates if the action needs to be started.
 * @ return true if needs starting
 */
Boolean needsStarting(uint8_t io, uint8_t act, uint8_t type) {
    switch(type) {
        case TYPE_INPUT:
            // this should never happen
            return FALSE;
#ifdef LEDSW
        case TYPE_LEDSW:
            return TRUE;
#endif
        case TYPE_OUTPUT:
#ifdef CANCDU
        case TYPE_CDU:      // we reuse a lot of the output code
#endif
            // Normal outputs will complete immediately.
            // Flashing outputs will complete immediately
            // pulsed output will complete after pulse has finished
            // but we need to start them all
            return (pulseDelays[io] == NEEDS_STARTING);
#ifdef SERVO
        case TYPE_SERVO:
#ifdef BOUNCE
            
        case TYPE_BOUNCE:
#endif
#ifdef MULTI
        case TYPE_MULTI:
#endif
            if (targetPos[io] == currentPos[io]) {
                //servoState[io] = OFF;
                return FALSE;
            }
            return (servoState[io] != SS_MOVING);
#endif
    }
    return TRUE;
}

/**
 * Indicates if the action has been completed.
 * @return true if completed
 */
Boolean completed(uint8_t io, Action action, uint8_t type) {
    switch(type) {
        case TYPE_INPUT:
            // this should never happen
            // fall through
#ifdef LEDSW
        case TYPE_LEDSW:
#endif
            return TRUE;
        case TYPE_OUTPUT:
            // wait for a pulse to complete. 
            return pulseDelays[io] == COMPLETED;

            
#ifdef SERVO
        case TYPE_SERVO:
#ifdef BOUNCE
        case TYPE_BOUNCE:
#endif
#ifdef MULTI
        case TYPE_MULTI:
#endif
            return (targetPos[io] == currentPos[io]) && ((servoState[io] == SS_STOPPED) || (servoState[io] == SS_OFF));
#endif
#ifdef CANCDU
        case TYPE_CDU:
            // wait for the recharge to complete. 
            return flashDelays[io] == COMPLETED;
#endif
    }
    return TRUE;
}

/**
 * Tidy up after completing an Action and the Action is marked as Done.
 *  
 * @param i the IO
 * @param state on/off or position
 * @param type type of output
 */
void finaliseOutput(uint8_t io, uint8_t type) {
    switch(type) {
        case TYPE_OUTPUT:
            return;
#ifdef CANCDU
        case TYPE_CDU:
            finaliseCduOutput(io);
            return;
#endif
        default:
            return;
    }
}