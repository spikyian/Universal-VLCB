
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
 * File:   universalEvents.c
 * Author: Ian Hogg
 * 
 * Here we deal with the module specific event handling. This covers:
 * <UL>
 * <LI>Setting of default events.</LI>
 * <LI>Processing of inbound, consumed events</LI>
 * <LI>Processing Start of Day (SOD) action</LI>
 *</UL>
 * 
 * Created on 10 April 2017, 10:26
 *
 * Modified by Greg Palmer 7 April 2021
 *
 * Corrected doSOD() to include handling of DISABLE_OFF.
 * Added Processing Start of Day (SOD) to the list in the comment block above.
 * Added new function Boolean produceStatusEvent(HAPPENING_T action, Boolean state, Boolean invert, Boolean can_send_on, Boolean can_send_off) to
 * simplify handling treatment of channel flags in doSOD. 
 * Note: (event_inverted & disable_off) causes suppression of ON events and sends OFF events only. 
 * For inputs this also affects events for happening TWO_ON.
 * Added FLAG_INPUT_DISABLE_SOD_RESPONSE support in doSOD to inhibit SOD response for input type channels (INPUT, ANALOGUE, MAGNET)
 * 11-May 2021
 * Reintroduced ACTION_STOP_PROCESSING
 */

#include <stddef.h>

#include "vlcb.h"
#include "nvm.h"
#include "module.h"
#include "event_producer.h"
#include "event_consumerDualActionQueue.h"
#include "event_teach_large.h"
#include "mns.h"
#include "timedResponse.h"

#include "event_consumerDualActionQueue.h"
#include "universalNv.h"
#include "universalEvents.h"
#include "universalEEPROM.h"
#include "outputs.h"
#ifdef ANALOGUE
#include "analogue.h"
#endif


// forward declarations
void clearEvents(uint8_t i);
void doWait(uint16_t duration);
Boolean sendInvertedProducedEvent(Happening happening, EventState state, Boolean invert, Boolean can_send_on, Boolean can_send_off);
Boolean alwaysSendInvertedProducedEvent(Happening action, EventState state, Boolean invert);
void doSOD(void);
Action getTwoAction(void);
void doneTwoAction(void);
Action peekTwoActionQueue(uint8_t index);
void deleteTwoActionQueue(uint8_t index);
TimedResponseResult sodTRCallback(uint8_t type, uint8_t serviceIndex, uint8_t step);

extern uint8_t outputState[NUM_IO];
extern uint8_t currentPos[NUM_IO];

static TickValue startWait;

void universalEventsInit(void) {
    startWait.val = 0;
}

/**
 * Set Global Events back to factory defaults.
 */
void factoryResetGlobalEvents(void) {
    // we don't create a default SOD event
}

/**
 * Reset events for the IO back to default. Called when the Type of the IO
 * is changed.
 * @param io the channel number
 * @param type the TYPE of the channel
 */
void defaultEvents(uint8_t io, uint8_t type) {
    uint16_t en = io+1;
    uint8_t b;

#ifdef CANCDU
    // The CDU type has Actions for both the current channel and its pair 
    // partner. If this isn't conditional then when the partner's default events
    // are added then it removed the Action from the first of the pair.
    if (type != TYPE_CDU) {
#endif
        clearEvents(io);
#ifdef CANCDU
    }
#endif
    // Check if default events should be created.
    if (MODULE_FLAG_DEFAULT_EVENTS & getNV(NV_MODULE_FLAGS)) return;
    // add the module's default events for this io
    switch(type) {
        
#ifdef BOUNCE
        case TYPE_BOUNCE:
             addEvent(nn.word, 100+en, 0, HAPPENING_IO_OUTPUT(io), TRUE);
            // fall through
#endif 
        case TYPE_OUTPUT:    
            // Consume ACON/ASON and ACOF/ASOF events with en as port number
            addEvent(nn.word, en, 1, ACTION_IO_OUTPUT_EV(io), TRUE);
            // don't produce the event by default any more
            // addEvent(nodeID, 100+en, 0, HAPPENING_IO_OUTPUT(io), TRUE);
            break;
        case TYPE_INPUT:
            // Produce ACON/ASON and ACOF/ASOF events with en as port number
            addEvent(nn.word, en, 0, HAPPENING_IO_INPUT(io), TRUE);
            break;
#ifdef SERVO
        case TYPE_SERVO:
            // Produce ACON/ASON and ACOF/ASOF events with en as port number
            addEvent(nn.word, 100+en, 0, HAPPENING_IO_SERVO_START(io), TRUE);
            addEvent(nn.word, 300+en, 0, HAPPENING_IO_SERVO_MID(io), TRUE);
            addEvent(nn.word, 200+en, 0, HAPPENING_IO_SERVO_END(io), TRUE);
            // Consume ACON/ASON and ACOF/ASOF events with en as port number
            addEvent(nn.word, en, 1, ACTION_IO_SERVO_EV(io), TRUE);
            break;
#endif 
#ifdef MULTI
        case TYPE_MULTI:
            // no defaults for multi
            break;
#endif 
#ifdef ANALOGUE
        case TYPE_ANALOGUE_IN:
            // Produce ACON/ASON and ACOF/ASOF events with en as port number
            addEvent(nn.word, en, 0, HAPPENING_IO_ANALOGUE(io), TRUE);
            break;
        case TYPE_MAGNET:
            // Produce ACON/ASON and ACOF/ASOF events with en as port number
            addEvent(nn.word, en, 0, HAPPENING_IO_MAGNETH(io), TRUE);
            addEvent(nn.word, 100+en, 0, HAPPENING_IO_MAGNETL(io), TRUE);
            break;
#endif
#ifdef CANCDU
        case TYPE_CDU:    
            // Consume ACON/ASON and ACOF/ASOF events with en as port number
            addEvent(nn.word, en, 1, ACTION_IO_CDU_EV(io), TRUE);
            // The cast below shouldn't be necessary but there seems to be a bug 
            // in the code generation of XC8. The result of an XOR is not of the  
            // correct type but the compiler thinks it is so the function  
            // parameter stack gets corrupted.
            b=ACTION_IO_CDU_NOT_EV(io ^ 1);
            addEvent(nn.word, en, 2, b, TRUE);
            break;
#endif
    }
}

/**
 * Universal specific version of add an event/EV.
 * This ensures that if a Happening is being written then this happening does not
 * exist for any other event.
 * 
 * @param nodeNumber
 * @param eventNumber
 * @param evNum the EV index (starts at 0 for the produced action)
 * @param evVal the EV value
 * @param forceOwnNN the value of the flag
 * @return error number or 0 for success
 */
uint8_t APP_addEvent(uint16_t nodeNumber, uint16_t eventNumber, uint8_t evNum, uint8_t evVal, Boolean forceOwnNN) {
#ifdef EVENT_HASH_TABLE       // This generates compile errors if hash table not defined, because producer events are not supported if hash table turned off 
    if ((evNum == 0) && (evVal != NO_ACTION))
    {
        // this is a Happening
        if ((evVal >= HAPPENING_BASE) && (evVal <= MAX_HAPPENING)) {
            uint8_t tableIndex = happening2Event[evVal-HAPPENING_BASE];
            if (tableIndex != NO_INDEX) {
                // Happening already exists
                // remove it
                writeEv(tableIndex, 0, NO_ACTION);
                checkRemoveTableEntry(tableIndex);

                rebuildHashtable();         
            }
        }
    }
#endif  
    return addEvent(nodeNumber, eventNumber, evNum, evVal, forceOwnNN);
}


/**
 * Clear the events for the IO. Called prior to setting the default events.
 * 
 * @param i the IO number
 */
void clearEvents(uint8_t io) {
    deleteActionRange((Action)(uint8_t)ACTION_IO_BASE(io),                       ACTIONS_PER_IO);
    deleteActionRange((Action)(uint8_t)(ACTION_IO_BASE(io) | ACTION_SIMULTANEOUS), ACTIONS_PER_IO);
    deleteHappeningRange(HAPPENING_IO_BASE(io),                 HAPPENINGS_PER_IO);
}


/**
 * This needs to be called on a regular basis to see if any
 * actions have finished and the next needs to be started.
 */
void processActions(void) {
    uint8_t io;
    uint8_t type;
    Action action;
    uint8_t ioAction;
    uint8_t simultaneous;
    uint8_t peekItem;
    
    while (1) {
        action = getTwoAction();
        
        if (action == NO_ACTION) {
            doneTwoAction();
            return;
        }
        // Check for SOD
        if (action == ACTION_SOD) {
            // Do the SOD
            doSOD();
            doneTwoAction();
            return;
        }
        if (action == ACTION_WAIT05) {
            doWait(5);
            return;
        }
        if (action == ACTION_WAIT1) {
            doWait(10);
            return;
        }
        if (action == ACTION_WAIT2) {
            doWait(20);
            return;
        }
        if (action == ACTION_WAIT5) {
            doWait(50);
            return;
        }
        simultaneous = action & ACTION_SIMULTANEOUS;
        ioAction = action&ACTION_MASK;
        if ((ioAction >= BASE_ACTION_IO) && (ioAction < NUM_ACTIONS)) {
            // process IO based consumed actions

            io = ACTION_IO(ioAction);
            ioAction = ACTION(ioAction);
            type = (uint8_t)getNV(NV_IO_TYPE(io));

            setOutputState(io, ioAction, type);
            if (needsStarting(io, ioAction, type)) {
                startOutput(io, ioAction, type);
            }
            // now check to see if any others need starting  
            peekItem = 1;
            while (simultaneous) {
                Action nextAction;
                uint8_t nextIo;
                uint8_t nextType;

                nextAction = peekTwoActionQueue(peekItem);

                if (nextAction == NO_ACTION) break;
                simultaneous = nextAction & ACTION_SIMULTANEOUS;
                nextAction &= ACTION_MASK;
                nextIo = ACTION_IO(nextAction);
                if (nextIo == io) {
                    // if we have 2 simultaneous actions on the same IO at the same 
                    // time then cancel the first and just do the second
                    // doneAction(); decided to not implement this change after all.
                    break;
                }
                nextAction = ACTION(nextAction);
                nextType = (uint8_t)getNV(NV_IO_TYPE(io));
                setOutputState(nextIo, nextAction, nextType);
                if (needsStarting(nextIo, nextAction, nextType)) {
                    startOutput(nextIo, nextAction, nextType);
                }
                if (completed(nextIo, nextAction, nextType)) {
                    deleteTwoActionQueue(peekItem);
                    finaliseOutput(io, type);
                }
                peekItem++;
            }
            // check if this current action has been completed
            if (completed(io, action, type)) {
                doneTwoAction();
                finaliseOutput(io, type);
            } else {
                // If the current action hasn't yet completed return so other stuff
                // can get done before we get called back on next poll.
                return;
            }
        } else {
            // shouldn't get here as this is an unknown action
            // In case we do get here make sure we do the action to prevent endless loop
            doneTwoAction();
        }
    }
}

/**
 * Stop processing actions for a while.
 * 
 * @param duration in 0.1second units
 */
void doWait(uint16_t duration) {
    // start the timer
    if (startWait.val == 0) {
        startWait.val = tickGet();
        return;
    } else {
        // check if timer expired
        if ((tickTimeSince(startWait) > ((long)duration * (long)HUNDRED_MILI_SECOND))) {
            doneTwoAction();
            startWait.val = 0;
            return;
        } 
    }
}

/**
 * Helper function used when sending SOD response. Inverts the polarity of event if required and also
 * checks if we are allowed to send on or off events.
 * @param happening the happening number to send
 * @param state the event polarity
 * @param invert whether the event should be inverted
 * @param can_send_on if we are allowed to send ON events 
 * @param can_send_off is we are allowed to send OFF events
 * @return true if the event was able to be sent
 */
Boolean sendInvertedProducedEvent(Happening happening, EventState state, Boolean invert, Boolean can_send_on, Boolean can_send_off) {
	EventState state_to_send = invert?!state:state;
	if ((state_to_send && can_send_on) || (!state_to_send && can_send_off)) {
		return sendProducedEvent(happening, state_to_send);
	} else {
		return TRUE;
	}
}

/**
 * Helper function used when sending SOD response. Inverts the polarity of event if required.
 * @param happening the happening number to send
 * @param state the event polarity
 * @param invert whether the event should be inverted
 * @return true if the event was able to be sent
 */
Boolean alwaysSendInvertedProducedEvent(Happening action, EventState state, Boolean invert) {
    return sendProducedEvent(action, invert?!state:state);
}

/**
 * Do the consumed SOD. 
 * This sets things up so that timedResponse will call back into APP_doSOD() whenever another response is
 * required.
 */
void doSOD(void) {
    startTimedResponse(TIMED_RESPONSE_SOD, findServiceIndex(SERVICE_ID_PRODUCER), sodTRCallback);
}

/**
 * Send one response CBUS message and increment the step counter ready for the next call.
 * 
 * Here I use step 0 to 63 for 16 channels of 4 happenings so that channel is step/16.
 * I could have used step as happening but that seemed to use slightly more maths but very little in it. May change at a later time.
 * 
 * This is the callback used by the service discovery responses.
 * @param type always set to TIMED_RESPONSE_NVRD
 * @param serviceIndex indicates the service requesting the responses
 * @param step loops through each service to be discovered
 * @return whether all of the responses have been sent yet.
 */
TimedResponseResult sodTRCallback(uint8_t type, uint8_t serviceIndex, uint8_t step) {
    uint8_t io;
    uint8_t happeningIndex;
    Boolean disable_off;
	Boolean send_on_ok;
	Boolean send_off_ok;
	Boolean event_inverted;
    Boolean disable_SOD_response;
    uint8_t flags;
    EventState value;

    // The step is used to index through the events and the channels
    io = step/HAPPENINGS_PER_IO;
    if (io >= NUM_IO) {     // finished?
        return TIMED_RESPONSE_RESULT_FINISHED;
    }
    happeningIndex = step%HAPPENINGS_PER_IO;
    flags = (uint8_t)getNV(NV_IO_FLAGS(io));;
    disable_SOD_response = flags & FLAG_INPUT_DISABLE_SOD_RESPONSE;
                     
    event_inverted = flags & FLAG_RESULT_EVENT_INVERTED;
    disable_off = flags & FLAG_DISABLE_OFF;
    send_on_ok  = !( disable_off && event_inverted );
    send_off_ok = !( disable_off && !event_inverted);

    value = APP_GetEventState(HAPPENING_IO_BASE(io)+happeningIndex);
             
    switch((uint8_t)getNV(NV_IO_TYPE(io))) {
        case TYPE_INPUT:
#ifdef ANALOGUE
        case TYPE_ANALOGUE_IN:
        case TYPE_MAGNET:
#endif
            if (disable_SOD_response) {
                return TIMED_RESPONSE_RESULT_NEXT;
            }
            break;
#ifdef SERVO
        case TYPE_SERVO:
#ifdef BOUNCE
        case TYPE_BOUNCE:
#endif
#ifdef MULTI
        case TYPE_MULTI:
#endif
            send_on_ok = TRUE;
            send_off_ok = TRUE;
            break;
#endif
    }
    if (value != EVENT_UNKNOWN) {
        sendInvertedProducedEvent(HAPPENING_IO_BASE(io)+happeningIndex, value, event_inverted, send_on_ok, send_off_ok);
    }
    return TIMED_RESPONSE_RESULT_NEXT;
}
#ifdef NEVER
/**
 * Do the consumed SOD action. This sends events to indicate current state of the system.
 */
void doSOD(void) {
    uint8_t midway;
    uint8_t state;
    uint8_t io;
    
    // Although I agreed with Pete that SOD is only applicable to EV#2 I actually allow it at any EV#

    for (io=0; io < NUM_IO; io++) {
        uint8_t event_inverted = getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED;
        switch(getNV(NV_IO_TYPE(io))) {
            case TYPE_INPUT:
                /* The TRIGGER_INVERTED has already been taken into account when saved in outputState. No need to check again */
                alwaysSendInvertedProducedEvent(HAPPENING_IO_INPUT(io), outputState[io], event_inverted);
                break;
            case TYPE_OUTPUT:
                state = (uint8_t)readNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io);
                alwaysSendInvertedProducedEvent(HAPPENING_IO_OUTPUT(io), state!=ACTION_IO_3, event_inverted);
                break;
#ifdef SERVO
            case TYPE_SERVO:
                alwaysSendInvertedProducedEvent(HAPPENING_IO_SERVO_START(io), currentPos[io] == getNV(NV_IO_SERVO_START_POS(io)), event_inverted);
                alwaysSendInvertedProducedEvent(HAPPENING_IO_SERVO_END(io), currentPos[io] == getNV(NV_IO_SERVO_END_POS(io)), event_inverted);
                // send the last mid
                midway = (uint8_t)(getNV(NV_IO_SERVO_END_POS(io))/2 + getNV(NV_IO_SERVO_START_POS(io))/2);
                alwaysSendInvertedProducedEvent(HAPPENING_IO_SERVO_MID(io), currentPos[io] >= midway, event_inverted);
                break;
#ifdef BOUNCE
            case TYPE_BOUNCE:
                state = (uint8_t)readNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io);
                alwaysSendInvertedProducedEvent(HAPPENING_IO_BOUNCE(io), state, event_inverted);
                break;
#endif
#ifdef MULTI
            case TYPE_MULTI:
                alwaysSendInvertedProducedEvent(HAPPENING_IO_MULTI_AT1(io), currentPos[io] == getNV(NV_IO_MULTI_POS1(io)), event_inverted);
                alwaysSendInvertedProducedEvent(HAPPENING_IO_MULTI_AT2(io), currentPos[io] == getNV(NV_IO_MULTI_POS2(io)), event_inverted);
                alwaysSendInvertedProducedEvent(HAPPENING_IO_MULTI_AT3(io), currentPos[io] == getNV(NV_IO_MULTI_POS3(io)), event_inverted);
                if (getNV(NV_IO_MULTI_NUM_POS(io)) > 3) {
                    alwaysSendInvertedProducedEvent(HAPPENING_IO_MULTI_AT4(io), currentPos[io] == getNV(NV_IO_MULTI_POS4(io)), event_inverted);
                }
                break;
#endif
#endif
#ifdef ANALOGUE
            case TYPE_ANALOGUE_IN:
            case TYPE_MAGNET:
                alwaysSendInvertedProducedEvent(HAPPENING_IO_MAGNETL(io), analogueState[io].eventState == ANALOGUE_EVENT_LOWER, event_inverted);
                alwaysSendInvertedProducedEvent(HAPPENING_IO_MAGNETH(io), analogueState[io].eventState == ANALOGUE_EVENT_UPPER, event_inverted);
                break;
#endif
        }
    }
}


#endif
