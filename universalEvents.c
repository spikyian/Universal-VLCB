
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
#include "event_consumer.h"
#include "mns.h"

#include "actionQueue.h"
#include "universalNv.h"
#include "universalEvents.h"
#include "universalEEPROM.h"
#include "outputs.h"


// forward declarations
void clearEvents(uint8_t i);
void doWait(uint16_t duration);
Boolean sendInvertedProducedEvent(Happening happening, EventState state, Boolean invert, Boolean can_send_on, Boolean can_send_off);
Boolean alwaysSendInvertedProducedEvent(Happening action, EventState state, Boolean invert);
void doSOD(void);

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
    clearEvents(io); 

    // add the module's default events for this io
    switch(type) {
        
#ifdef BOUNCE
        case TYPE_BOUNCE:
             addEvent(nn, 100+en, 0, HAPPENING_IO_OUTPUT(io), TRUE);
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
    if ((evNum == 0) && (evVal != NO_ACTION))
    {
        // this is a Happening
#ifdef HASH_TABLE       // This generates compile errors if hash table not defined, because producer events are not supported if hash table turned off 
        uint8_t tableIndex = happening2Event[evVal-HAPPENING_BASE];
        if (tableIndex != NO_INDEX) {
            // Happening already exists
            // remove it
            writeEv(tableIndex, 0, NO_ACTION);
            checkRemoveTableEntry(tableIndex);

            rebuildHashtable();         
        }
#endif  
    }
    return addEvent(nodeNumber, eventNumber, evNum, evVal, forceOwnNN);
}


/**
 * Clear the events for the IO. Called prior to setting the default events.
 * 
 * @param i the IO number
 */
void clearEvents(uint8_t io) {
    deleteActionRange(ACTION_IO_BASE(io),                       ACTIONS_PER_IO);
    deleteActionRange(ACTION_IO_BASE(io) | ACTION_SIMULTANEOUS, ACTIONS_PER_IO);
    deleteHappeningRange(HAPPENING_IO_BASE(io),                 HAPPENINGS_PER_IO);
}


/**
 * This needs to be called on a regular basis to see if any
 * actions have finished and the next needs to be started.
 */
void processActions(void) {
    uint8_t io;
    uint8_t type;
    ActionAndState * action = getTwoAction();
    uint8_t ioAction;
    uint8_t simultaneous;
    uint8_t peekItem;
    
    
    if (action->a.value == NO_ACTION) {
        doneTwoAction();
        return;
    }
    // Check for SOD
    if (action->a.value == ACTION_SOD) {
        // Do the SOD
        doSOD();
        doneTwoAction();
        return;
    }
    if (action->a.value == ACTION_WAIT05) {
        doWait(5);
        return;
    }
    if (action->a.value == ACTION_WAIT1) {
        doWait(10);
        return;
    }
    if (action->a.value == ACTION_WAIT2) {
        doWait(20);
        return;
    }
    if (action->a.value == ACTION_WAIT5) {
        doWait(50);
        return;
    }
    simultaneous = action->a.value & ACTION_SIMULTANEOUS;
    ioAction = action->a.value&ACTION_MASK;
    if ((ioAction >= BASE_ACTION_IO) && (ioAction < NUM_ACTIONS)) {
        // process IO based consumed actions
        
        io = ACTION_IO(ioAction);
        ioAction = ACTION(ioAction);
        type = (uint8_t)getNV(NV_IO_TYPE(io));

        // check if a simultaneous action needs to be started
        setOutputState(io, ioAction, type);
        if (needsStarting(io, ioAction, type)) {
            startOutput(io, ioAction, type);
        }
        // now check to see if any others need starting  
        peekItem = 1;
        while (simultaneous) {
            ActionAndState * nextAction;
            uint8_t nextIo;
            uint8_t nextType;
            
            nextAction = peekTwoActionQueue(peekItem);
            
            if (nextAction->a.value == NO_ACTION) break;
            simultaneous = nextAction->a.value & ACTION_SIMULTANEOUS;
            nextAction->a.value &= ACTION_MASK;
            nextIo = ACTION_IO(nextAction->a.value);
            if (nextIo == io) {
                // if we have 2 simultaneous actions on the same IO at the same 
                // time then cancel the first and just do the second
                // doneAction(); decided to not implement this change after all.
                break;
            }
            nextType = (uint8_t)getNV(NV_IO_TYPE(io));
            setOutputState(nextIo, nextAction->a.value, nextType);
            if (needsStarting(nextIo, nextAction->a.value, nextType)) {
                startOutput(nextIo, nextAction->a.value, nextType);
            }
            if (completed(nextIo, nextAction, nextType)) {
                deleteTwoActionQueue(peekItem);
            }
            peekItem++;
        }
        // check if this current action has been completed
        if (completed(io, action, type)) {
            doneTwoAction();
        }
    } else {
        // shouldn't get here as this is an unknown action
        // In case we do get here make sure we do the action to prevent endless loop
        doneTwoAction();
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

#ifdef TIMED_RESPONSE
/**
 * Do the consumed SOD. 
 * This sets things up so that timedResponse will call back into APP_doSOD() whenever another response is
 * required.
 */
void doSOD(void) {
    timedResponse = TIMED_RESPONSE_SOD;
    timedResponseStep = 0;
}

/**
 * Send one response CBUS message and increment the step counter ready for the next call.
 * Although I agreed with Pete that SOD is only applicable to EV#2 I actually allow it at any EV#
 * 
 * Here I use step 0 to 63 for 16 channels of 4 happenings so that channel is step/16.
 * I could have used step as happening but that seemed to use slightly more maths but very little in it. May change at a later time.
 * 
 * @param step the timed response step
 * @return the status of sending SoD responses
 */
uint8_t APP_doSOD(uint8_t step) {
    uint8_t io;
    uint8_t happeningIndex;
    Boolean disable_off;
	Boolean send_on_ok;
	Boolean send_off_ok;
	Boolean event_inverted;
    Boolean disable_SOD_response;
    uint8_t flags;
    uint8_t value;


    // The step is used to index through the events and the channels
    io = step/HAPPENINGS_PER_IO;
    if (io >= NUM_IO) {     // finished?
        return TIMED_RESPONSE_APP_FINISHED;
    }
    happeningIndex = step%HAPPENINGS_PER_IO;
    flags = (uint8_t)getNV(NV_IO_FLAGS(io));;
    disable_SOD_response = flags & FLAG_INPUT_DISABLE_SOD_RESPONSE;
                     
    event_inverted = flags & FLAG_RESULT_EVENT_INVERTED;
    disable_off = flags & FLAG_DISABLE_OFF;
    send_on_ok  = !( disable_off && event_inverted );
    send_off_ok = !( disable_off && !event_inverted);
    value = 255;    // indicates no response at this step
                    
    switch((uint8_t)getNV(NV_IO_TYPE(io));) {
        case TYPE_INPUT:
            if (disable_SOD_response) {
                return TIMED_RESPONSE_APP_NEXT;
            }
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    // The TRIGGER_INVERTED has already been taken into account when saved in outputState. No need to check again
                    value = outputState[io];
                    break;
                case HAPPENING_IO_2:
                    // TWO_ON is only sent if DISABLE_OFF is set GP//
                    if (disable_off) {
                        value = (outputState[io]==0);
                    }
                    break;
            }
            break;
        case TYPE_OUTPUT:
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    value = (ee_read(EE_OP_STATE+io)!=ACTION_IO_3);
                    break;
            }
            break;
#ifdef SERVO
        case TYPE_SERVO:
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    value = (currentPos[io] == (uint8_t)getNV(NV_IO_SERVO_START_POS(io));
                    break;
                case HAPPENING_IO_3:
                    value = (currentPos[io] == (uint8_t)getNV(NV_IO_SERVO_END_POS(io));
                    break;
                    // send the last mid
                case HAPPENING_IO_2:
                    value = (currentPos[io] >= ((uint8_t)getNV(NV_IO_SERVO_END_POS(io)))/2 + 
                         ((uint8_t)getNV(NV_IO_SERVO_START_POS(io)))/2);
                    break;
            }
            send_on_ok = TRUE;
            send_off_ok = TRUE;
            break;
#ifdef BOUNCE
        case TYPE_BOUNCE:
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    value = ee_read(EE_OP_STATE+io);
                    break;
            }
            send_on_ok = TRUE;
            send_off_ok = TRUE;
            break;
#endif
#ifdef MULTI
        case TYPE_MULTI:
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    value = (currentPos[io] == getNV(NV_IO_MULTI_POS1(io));
                    break;
                case HAPPENING_IO_2:
                    value = (currentPos[io] == getNV(NV_IO_MULTI_POS2(io));
                    break;
                // it is more logical to use servo rather than multi with <3 positions but check anyway GP//
                case HAPPENING_IO_3:
                    if (getNV(NV_IO_MULTI_NUM_POS(io)) > 2) { 
                        value = (currentPos[io] == getNV(NV_IO_MULTI_POS3(io));
                    }
                    break;
                case HAPPENING_IO_4:
                    if (getNV(NV_IO_MULTI_NUM_POS(io)) > 3) {
                        value = (currentPos[io] == getNV(NV_IO_MULTI_POS4(io));
                    }
                    break;
            }
            send_on_ok = TRUE;
            send_off_ok = TRUE;
            break;
#endif
#endif
#ifdef ANALOGUE
        case TYPE_ANALOGUE_IN:
        case TYPE_MAGNET:
            if (disable_SOD_response) {
                    return TIMED_RESPONSE_APP_NEXT;
            }
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    value = (analogueState[io].eventState == ANALOGUE_EVENT_LOWER);
                    break;
                case HAPPENING_IO_2:
                    value = (analogueState[io].eventState == ANALOGUE_EVENT_UPPER);
                    break;
            }
            break;
#endif
        }
    if (value != 255) {
        if (!sendInvertedProducedEvent(HAPPENING_IO_BASE(io)+happeningIndex, value, event_inverted, send_on_ok, send_off_ok)) {
            return TIMED_RESPONSE_APP_RETRY;
        }
    }
    return TIMED_RESPONSE_APP_NEXT;
}
#else
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
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_INPUT(io), outputState[io], event_inverted)) ;
                break;
            case TYPE_OUTPUT:
                state = (uint8_t)readNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io);
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_OUTPUT(io), state!=ACTION_IO_3, event_inverted));
                break;
#ifdef SERVO
            case TYPE_SERVO:
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_SERVO_START(io), currentPos[io] == getNV(NV_IO_SERVO_START_POS(io)), event_inverted));
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_SERVO_END(io), currentPos[io] == getNV(NV_IO_SERVO_END_POS(io)), event_inverted));
                // send the last mid
                midway = getNV(NV_IO_SERVO_END_POS(io))/2 + getNV(NV_IO_SERVO_START_POS(io))/2;
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_SERVO_MID(io), currentPos[io] >= midway, event_inverted));
                break;
#ifdef BOUNCE
            case TYPE_BOUNCE:
                state = (uint8_t)readNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io);
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_BOUNCE(io), state, event_inverted));
                break;
#endif
#ifdef MULTI
            case TYPE_MULTI:
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_MULTI_AT1(io), currentPos[io] == getNV(NV_IO_MULTI_POS1(io)), event_inverted));
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_MULTI_AT2(io), currentPos[io] == getNV(NV_IO_MULTI_POS2(io)), event_inverted));
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_MULTI_AT3(io), currentPos[io] == getNV(NV_IO_MULTI_POS3(io)), event_inverted));
                if (getNV(NV_IO_MULTI_NUM_POS(io)) > 3) {
                    while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_MULTI_AT4(io), currentPos[io] == getNV(NV_IO_MULTI_POS4(io)), event_inverted));
                }
                break;
#endif
#endif
#ifdef ANALOGUE
            case TYPE_ANALOGUE_IN:
            case TYPE_MAGNET:
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_MAGNETL(io), eventState[io] == ANALOGUE_EVENT_LOWER, event_inverted));
                while ( ! alwaysSendInvertedProducedEvent(HAPPENING_IO_MAGNETH(io), eventState[io] == ANALOGUE_EVENT_UPPER, event_inverted));
                break;
#endif
        }
    }
}


#endif
