
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
 * File:   universalEvents.h
 * Author: Ian
 *
 * Created on 17 April 2017, 13:14
 * Revision History 
 * 
 * ---
 * Revised by Greg Palmer
 * 13 April 2021
 * Reflects change in the arguments of sendProducedEvent to include 2 additional arguments.
 * See universalEvents.
 * 11 May 2021
 * Re-introduced ACTION_STOP_PROCESSING
 */

#ifndef universalEvents_H
#define	universalEvents_H

#ifdef	__cplusplus
extern "C" {
#endif
    /*
     * This is where all the module specific actions are defined.
     * The following definitions are required by the FLiM code:
     * NUM_HAPPENINGS, NUM_ACTIONS, HASH_LENGTH, EVT_NUM, 
     * EVperEVT, NUM_CONSUMED_EVENTS
     * 
     * For MIO an action is a uint8_t (uint8_t). The upperupper bit is used
     * to indicate a sequential action. It would be nice to define:
     * 
     * typedef union {
     *      uint8_t action_byte;
     *      struct {
     *          uint8_t action :7;
     *          uint8_t sequential :1;
     *      }
     * } ACtION_T;
     * but C spec doesn't define what size this would be. Therefore I just use
     * byte (uint8_t).
     * 
     * The actions are defined below - but remember consumed actions may also have MSB
     * set indicating sequential.
     */
 
#include "event_teach.h"
    


    
    
/* CONSUMED actions */
#define ACTION_SIMULTANEOUS                 0x80    // default is SEQUENTIAL
#define ACTION_MASK                         0x7F
    
    // Global consumed actions first
#define NO_ACTION                  0
#define ACTION_SOD                 1
#define ACTION_WAIT05              2
#define ACTION_WAIT1               3
#define ACTION_WAIT2               4
#define ACTION_WAIT5               5

#define ACTION_STOP_PROCESSING     7


        // Now Consumed actions per io

#define BASE_ACTION_IO             8
#define ACTION_IO_1                0
#define ACTION_IO_2                1
#define ACTION_IO_3                2
#define ACTION_IO_4                3
#define ACTION_IO_5                4
#define ACTIONS_PER_IO             5   
#define NUM_ACTIONS                (BASE_ACTION_IO + ACTIONS_PER_IO * NUM_IO)   
    
/* PRODUCED actions */    
#define HAPPENING_BASE                1
    // Global produced actions next
#define HAPPENING_SOD                 1
    // produced actions per io
#define BASE_HAPPENING_IO             8
#define HAPPENING_IO_1                0
#define HAPPENING_IO_2                1
#define HAPPENING_IO_3                2
#define HAPPENING_IO_4                3
#define HAPPENINGS_PER_IO             4
#define NUM_HAPPENINGS                (BASE_HAPPENING_IO + HAPPENINGS_PER_IO * NUM_IO)
    

#define HAPPENING_IO_BASE(i)          (BASE_HAPPENING_IO + HAPPENINGS_PER_IO*(i))
#define ACTION_IO_BASE(i)             (BASE_ACTION_IO + ACTIONS_PER_IO*(i))
    
/* PRODUCED actions*/
#define HAPPENING_IO_INPUT(i)         (HAPPENING_IO_BASE(i)+HAPPENING_IO_1)
#define HAPPENING_IO_INPUT_TWO_ON(i)  (HAPPENING_IO_BASE(i)+HAPPENING_IO_2)
    
#define HAPPENING_IO_OUTPUT(i)        (HAPPENING_IO_BASE(i)+HAPPENING_IO_1)

#define HAPPENING_IO_SERVO_START(i)   (HAPPENING_IO_BASE(i)+HAPPENING_IO_1)
#define HAPPENING_IO_SERVO_MID(i)     (HAPPENING_IO_BASE(i)+HAPPENING_IO_2)
#define HAPPENING_IO_SERVO_END(i)     (HAPPENING_IO_BASE(i)+HAPPENING_IO_3)

#define HAPPENING_IO_BOUNCE(i)        (HAPPENING_IO_BASE(i)+HAPPENING_IO_1)
    
#define HAPPENING_IO_MULTI_AT1(i)     (HAPPENING_IO_BASE(i)+HAPPENING_IO_1)
#define HAPPENING_IO_MULTI_AT2(i)     (HAPPENING_IO_BASE(i)+HAPPENING_IO_2)
#define HAPPENING_IO_MULTI_AT3(i)     (HAPPENING_IO_BASE(i)+HAPPENING_IO_3)
#define HAPPENING_IO_MULTI_AT4(i)     (HAPPENING_IO_BASE(i)+HAPPENING_IO_4)
    
#define HAPPENING_IO_ANALOGUE(i)      (HAPPENING_IO_BASE(i)+HAPPENING_IO_1)

#define HAPPENING_IO_MAGNETL(i)       (HAPPENING_IO_BASE(i)+HAPPENING_IO_1)
#define HAPPENING_IO_MAGNETH(i)       (HAPPENING_IO_BASE(i)+HAPPENING_IO_2)
 
#define HAPPENING(a)                  (((a)-BASE_HAPPENING_IO)%HAPPENINGS_PER_IO)
#define HAPPENING_IO(a)               (((a)-BASE_HAPPENING_IO)/HAPPENINGS_PER_IO)
    
/* CONSUMED actions*/
#define ACTION_IO_OUTPUT_EV(i)     (ACTION_IO_BASE(i)+ACTION_IO_1)
#define ACTION_IO_OUTPUT_ON(i)     (ACTION_IO_BASE(i)+ACTION_IO_2)
#define ACTION_IO_OUTPUT_OFF(i)    (ACTION_IO_BASE(i)+ACTION_IO_3)
#define ACTION_IO_OUTPUT_FLASH(i)  (ACTION_IO_BASE(i)+ACTION_IO_4)
#define ACTION_IO_OUTPUT_NOT_EV(i) (ACTION_IO_BASE(i)+ACTION_IO_5)

#define ACTION_IO_SERVO_EV(i)      (ACTION_IO_BASE(i)+ACTION_IO_1)
#define ACTION_IO_SERVO_ON(i)      (ACTION_IO_BASE(i)+ACTION_IO_2)
#define ACTION_IO_SERVO_OFF(i)     (ACTION_IO_BASE(i)+ACTION_IO_3)


#define ACTION_IO_BOUNCE_EV(i)     (ACTION_IO_BASE(i)+ACTION_IO_1)
#define ACTION_IO_BOUNCE_ON(i)     (ACTION_IO_BASE(i)+ACTION_IO_2)
#define ACTION_IO_BOUNCE_OFF(i)    (ACTION_IO_BASE(i)+ACTION_IO_3)


#define ACTION_IO_MULTI_TO1(i)     (ACTION_IO_BASE(i)+ACTION_IO_1)
#define ACTION_IO_MULTI_TO2(i)     (ACTION_IO_BASE(i)+ACTION_IO_2)
#define ACTION_IO_MULTI_TO3(i)     (ACTION_IO_BASE(i)+ACTION_IO_3)
#define ACTION_IO_MULTI_TO4(i)     (ACTION_IO_BASE(i)+ACTION_IO_4)
    
#define ACTION(a)                  (((a)-BASE_ACTION_IO)%ACTIONS_PER_IO)
#define ACTION_IO(a)               (((a)-BASE_ACTION_IO)/ACTIONS_PER_IO)

extern void universalEventsInit(void);
extern void factoryResetGlobalEvents(void);
extern void defaultEvents(uint8_t i, uint8_t type);
extern void clearEvents(uint8_t i);

#define EVperEVT                20          // Max number of EVs per event


// We'll also be using configurable produced events
#define PRODUCED_EVENTS
#define ConsumedActionType  uint8_t;

extern void processEvent(uint8_t eventIndex, uint8_t* message);
extern void processActions(void);

extern Boolean sendInvertedProducedEvent(Happening happening, EventState state, Boolean invert,
                                        Boolean can_send_on, Boolean can_send_off);
extern Boolean alwaysSendInvertedProducedEvent(Happening action, EventState state, Boolean invert);

#ifdef	__cplusplus
}
#endif

#endif	/* universalEvents_H */

