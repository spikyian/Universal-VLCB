/**
 * @copyright Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 */
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

    This software is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

*/
/**
 * @author Ian Hogg 
 * @date Dec 2022
 * 
 */ 

#include <xc.h>
#include "vlcb.h"
#include "event_consumerDualActionQueue.h"
#include "event_teach.h"

#include "universalNv.h"
#include "universalEvents.h"

/**
 * @file
 * Implementation of the VLCB Event Consumer service.
 * @details
 * The service definition object is called eventConsumerService.
 * Process consumed events. Process Long and Short events.
 * Also handles events with data bytes if HANDLE_DATA_EVENTS is defined. The data is ignored.
 * 
 * EVs after the Happening are treated as Actions and are added to 
 * one of 2 Action queues, a normal queue and an expedited queue to be processed 
 * by the application.
 */

static void consumer2QPowerUp(void);
static Processed consumer2QProcessMessage(Message * m);
#ifdef VLCB_DIAG
static DiagnosticVal consumer2QDiagnostics[NUM_CONSUMER_DIAGNOSTICS];
static DiagnosticVal * consumer2QGetDiagnostic(uint8_t index);
#endif
        
/**
 * The service descriptor for the eventConsumer service. The application must include this
 * descriptor within the const Service * const services[] array and include the
 * necessary settings within module.h in order to make use of the event consumer
 * service.
 */
const Service eventConsumer2QService = {
    SERVICE_ID_CONSUMER,// id
    1,                  // version
    NULL,               // factoryReset
    consumer2QPowerUp,               // powerUp
    consumer2QProcessMessage,               // processMessage
    NULL,               // poll
#if defined(_18F66K80_FAMILY_)
    NULL,               // highIsr
    NULL,               // lowIsr
#endif
#ifdef VLCB_SERVICE
    NULL,               // Get ESD data
#endif
#ifdef VLCB_DIAG
    consumer2QGetDiagnostic                // getDiagnostic
#endif
};

uint8_t normalReadIndex;                   // index of the next to read
uint8_t normalWriteIndex;                  // index of the next to write
Action normalQueueBuf[ACTION_NORMAL_QUEUE_SIZE];   // the actual cyclic buffer space

uint8_t expeditedReadIndex;                   // index of the next to read
uint8_t expeditedWriteIndex;                  // index of the next to write
Action expeditedQueueBuf[ACTION_EXPEDITED_QUEUE_SIZE];   // the actual cyclic buffer space

static Boolean expedited;

static void consumer2QPowerUp(void) {
    normalReadIndex = 0;
    normalWriteIndex = 0;
    expeditedReadIndex = 0;
    expeditedWriteIndex = 0;
    
    expedited = FALSE;
}

/**
 * Process the consumed events. Extract the actions from the EVs. 
 * The actions are pushed onto the actionQueue for subsequent processing in
 * sequence.
 * 
 * If an event is defined to have actions A1, A2, A3, A4 and A2 has the SIMULANEOUS 
 * flag set then the sequence will be executed for ON event: A1, A2&A3, A4 and
 * we therefore put:
 *  A1 (sequential), A2 (simultaneous), A3 (sequential, A4 (sequential) 
 * into the action queue.
 * 
 * For an OFF event we want the sequence: A4, A3&A2, A1 and therefore we put:
 *  A4 (sequential), A3 (simultaneous), A2 (sequential), A1 (sequential)
 * into the action queue. Therefore when doing an OFF Event we need to fiddle
 * with the SIMULTANEOUS bit.
 * 
 * @param m the full CBUS message so that OPC  and DATA can be retrieved.
 * @return PROCESSED indicating that we processed the event so that the VLCB library need not
 */
static Processed consumer2QProcessMessage(Message *m) {
    uint8_t start, end;
    uint8_t tableIndex;
    int8_t change;
    uint8_t e;
    Action action;
    uint8_t masked_action;
    uint8_t ca;
    uint8_t io;
    
    if (m->len < 5) return NOT_PROCESSED;
    
    tableIndex = findEvent(((uint16_t)m->bytes[0])*256+m->bytes[1], ((uint16_t)m->bytes[2])*256+m->bytes[3]);
    if (tableIndex == NO_INDEX) return NOT_PROCESSED;

    switch (m->opc) {
        case OPC_ACON:
#ifdef HANDLE_DATA_EVENTS
        case OPC_ACON1:
        case OPC_ACON2:
        case OPC_ACON3:
#endif
        case OPC_ASON:
#ifdef HANDLE_DATA_EVENTS
        case OPC_ASON1:
        case OPC_ASON2:
        case OPC_ASON3:
#endif
            start = HAPPENING_SIZE;
            end = PARAM_NUM_EV_EVENT;
            change = ACTION_SIZE;
            break;
        case OPC_ACOF:
#ifdef HANDLE_DATA_EVENTS
        case OPC_ACOF1:
        case OPC_ACOF2:
        case OPC_ACOF3:
#endif
        case OPC_ASOF:
#ifdef HANDLE_DATA_EVENTS
        case OPC_ASOF1:
        case OPC_ASOF2:
        case OPC_ASOF3:
#endif
            start = PARAM_NUM_EV_EVENT-ACTION_SIZE;
            end = HAPPENING_SIZE-1;
            change = -ACTION_SIZE;
            break;
        default:
            return NOT_PROCESSED;
    }
    
    // get the list of actions ready to the action queue
    tableIndex = findEvent(((uint16_t)m->bytes[0])*256+m->bytes[1], ((uint16_t)m->bytes[2])*256+m->bytes[3]);
    if (tableIndex == NO_INDEX) return NOT_PROCESSED;

    uint8_t opc = getEVs(tableIndex);
#ifdef SAFETY
    if (opc != 0) {
        return PROCESSED; // error getting EVs. Can't report the error so just return
    }
#endif
    opc=m->opc;
    // check the OPC if this is an ON or OFF event
    if ( ! (opc&EVENT_ON_MASK)) {
        // ON events work up through the EVs
        // EV#0 is for produced event so start at 1
        // TODO would be more efficient to get all the EVs in one go and then work through them. getEV() isn't quick)
        for (e=1; e<EVperEVT ;e++) { 
            action = evs[e];  // we don't mask out the SEQUENTIAL flag so it could be specified in EVs
            if (action != NO_ACTION) {
                // check this is a consumed action
                masked_action = action&ACTION_MASK;
                if ((masked_action) <= NUM_ACTIONS) {
                    // check global consumed actions
                    if ((masked_action) == ACTION_STOP_PROCESSING) {
                        break;
                    }
                    if ((masked_action) < BASE_ACTION_IO) {
                        pushTwoAction(action);
                    } else {
                        io = ACTION_IO(masked_action);
                        ca = ACTION(masked_action);
                        switch (getNV(NV_IO_TYPE(io))) {
                            case TYPE_OUTPUT:
                                if (getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_EXPEDITED) {
                                    setExpeditedActions();
                                }
                                // fall through
                            case TYPE_SERVO:
                            case TYPE_BOUNCE:
                                if (ca == ACTION_IO_1) {
                                    // action 1 (Change) must be converted to 2(ON)
                                    action++;
                                }
                                if (ca == ACTION_IO_5) {
                                    // action 5 (!Change) must be converted to 3(OFF)
                                    action-=2;
                                }
                                pushTwoAction(action);
                                setNormalActions();
                                break;
                            case TYPE_MULTI:
                                pushTwoAction(action);
                                break;
                            default:
                                // shouldn't happen - just ignore
                                break;
                        }
                    }
                }
            }
        } 
    } else {
        // OFF events work down through the EVs
        //int nextAction = getEv(tableIndex, EVperEVT-1);
        Action nextAction = evs[EVperEVT-1];

        for (e=EVperEVT-1; e>=1 ;e--) { 
            uint8_t nextSimultaneous;
            uint8_t firstAction = NO_ACTION;  // used to determine simultaneous flag for the end of actions
            action = nextAction;  // we don't mask out the SIMULTANEOUS flag so it could be specified in EVs
            

            // get the Simultaneous flag from the next action
            if (e > 1) {
                nextAction = evs[e-1];
                nextSimultaneous = nextAction & ACTION_SIMULTANEOUS;
            } else {
                nextSimultaneous = firstAction & ACTION_SIMULTANEOUS;
            }
            if (action != NO_ACTION) {
                // record the first action we come to - which is actually the last action when doing an ON event
                if (firstAction == NO_ACTION) {
                    firstAction = action;
                }
                action &= ACTION_MASK;
                if (action <= NUM_ACTIONS) {
                    // check global consumed actions
                    if ((action) == ACTION_STOP_PROCESSING) {
                        break;
                    }            
                    if ((action < BASE_ACTION_IO) && (action != ACTION_SOD)) {  // Only do SoD on ON events
                        action |= nextSimultaneous;
                        pushTwoAction(action);
                    } else {
                        io = ACTION_IO(action);
                        ca = ACTION(action);
                        switch (getNV(NV_IO_TYPE(io))) {
                            case TYPE_OUTPUT:
                                if (getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_EXPEDITED) {
                                    setExpeditedActions();
                                }
                                if (ca == ACTION_IO_4) {
                                    // action 4 (Flash) must be converted to 3(OFF)
                                    action--;
                                }
                                // fall through
                            case TYPE_SERVO:
                            case TYPE_BOUNCE:
                                if (ca == ACTION_IO_1) {
                                    // action 1 (Change) must be converted to 3(OFF)
                                    action += 2;
                                }
                                if (ca == ACTION_IO_5) {
                                    // action 5 (!Change) must be converted to 3(ON)
                                    action -= 3;
                                }
                                action |= nextSimultaneous;
                                pushTwoAction(action);
                                setNormalActions();
                                break;
                            case TYPE_MULTI:
                                action |= nextSimultaneous;
                                pushTwoAction(action);
                                setNormalActions();
                                break;
                            default:
                                // shouldn't happen - just ignore
                                break;
                        }
                    }
                }
            }
        } // ignore getEv errors as we expect CMDERR_NO_EV
    }
#ifdef VLCB_DIAG
    consumer2QDiagnostics[CONSUMER_DIAG_NUMCONSUMED].asUint++;
#endif
    return PROCESSED;
}

#ifdef VLCB_DIAG
/**
 * Provide the means to return the diagnostic data.
 * @param index the diagnostic index
 * @return a pointer to the diagnostic data or NULL if the data isn't available
 */
static DiagnosticVal * consumer2QGetDiagnostic(uint8_t index) {
    if ((index<1) || (index>NUM_CONSUMER_DIAGNOSTICS)) {
        return NULL;
    }
    return &(consumer2QDiagnostics[index-1]);
}
#endif

/**
 * Put the action onto the list of actions to be processed.
 *
 * @param a the action to be processed
 */
Boolean pushTwoAction(Action a) {
    if (expedited) {
        if (((expeditedWriteIndex+1)&(ACTION_EXPEDITED_QUEUE_SIZE-1)) == expeditedReadIndex) 
            return FALSE;	// buffer full
        expeditedQueueBuf[expeditedWriteIndex++] = a;
        if (expeditedWriteIndex >= ACTION_EXPEDITED_QUEUE_SIZE) expeditedWriteIndex = 0;
        return TRUE;
    }
    if (((normalWriteIndex+1)&(ACTION_NORMAL_QUEUE_SIZE-1)) == normalReadIndex) 
        return FALSE;	// buffer full
    normalQueueBuf[normalWriteIndex++] = a;
    if (normalWriteIndex >= ACTION_NORMAL_QUEUE_SIZE) normalWriteIndex = 0;
    return TRUE;
}


/**
 * Get the action we need to be progressing.
 *
 * @return the action
 */
Action getTwoAction(void) {
	return peekTwoActionQueue(0);
}

/**
 * Pop the next action from the buffer.
 *
 * @return the next action
 */
Action popTwoAction(void) {
    Action ret;
    
	if (expeditedWriteIndex != expeditedReadIndex) {
        // buffer not empty
        ret = expeditedQueueBuf[expeditedReadIndex++];
        if (expeditedReadIndex >= ACTION_EXPEDITED_QUEUE_SIZE) expeditedReadIndex = 0;
        return ret;
    }
    
	if (normalWriteIndex == normalReadIndex) {
        return NO_ACTION;	// buffer empty
    }
	ret = normalQueueBuf[normalReadIndex++];
	if (normalReadIndex >= ACTION_NORMAL_QUEUE_SIZE) normalReadIndex = 0;
	return ret;
}

/**
 * Mark as having completed the current action.
 */
void doneTwoAction(void) {
	popTwoAction();
}

/**
 * Peek an item in the queue. Does not remove the item from the queue.
 * @param index the item index within the queue
 * @return the Action or NO_ACTION 
 */
Action peekTwoActionQueue(uint8_t index) {
    uint8_t qty;
    qty = (expeditedWriteIndex - expeditedReadIndex) & (ACTION_EXPEDITED_QUEUE_SIZE -1);
    if (index < qty) {
        if (expeditedReadIndex == expeditedWriteIndex) return NO_ACTION;    // empty
        index += expeditedReadIndex;
        if (index >= ACTION_EXPEDITED_QUEUE_SIZE) {
            index -= ACTION_EXPEDITED_QUEUE_SIZE;
        }
        if (index == expeditedWriteIndex) return NO_ACTION; // at end
        return expeditedQueueBuf[index];      
    }
    index -= qty;
    qty = (normalWriteIndex - normalReadIndex) & (ACTION_NORMAL_QUEUE_SIZE -1);
    if (index < qty) {
        if (normalReadIndex == normalWriteIndex) return NO_ACTION;    // empty
        index += normalReadIndex;
        if (index >= ACTION_NORMAL_QUEUE_SIZE) {
            index -= ACTION_NORMAL_QUEUE_SIZE;
        }
        if (index == normalWriteIndex) return NO_ACTION; // at end
        return normalQueueBuf[index];
    }
    return NO_ACTION;
}

/**
 * Delete an item in the queue. Replace the item with NO_ACTION.
 * @param index the item index within the queue
 */
void deleteTwoActionQueue(uint8_t index) {
    uint8_t qty;
    qty = (expeditedWriteIndex - expeditedReadIndex) & (ACTION_EXPEDITED_QUEUE_SIZE -1);
    if (index < qty) {
        index += expeditedReadIndex;
        if (index >= ACTION_EXPEDITED_QUEUE_SIZE) {
            index -= ACTION_EXPEDITED_QUEUE_SIZE;
        }
        if (index == expeditedWriteIndex) return; // at end
        expeditedQueueBuf[index] = NO_ACTION;
        return;
    }
    index -= qty;
    qty = (normalWriteIndex - normalReadIndex) & (ACTION_NORMAL_QUEUE_SIZE -1);
    if (index < qty) {
        if (normalReadIndex == normalWriteIndex) return;    // empty
        index += normalReadIndex;
        if (index >= ACTION_NORMAL_QUEUE_SIZE) {
            index -= ACTION_NORMAL_QUEUE_SIZE;
        }
        if (index == normalWriteIndex) return; // at end
        normalQueueBuf[index] = NO_ACTION;
        return;
    }
}

/**
 * Set the expedited flag so that subsequent pushes are added to the front of the queue.
 */
void setExpeditedActions(void) {
    expedited = TRUE;
}

/**
 * Clear the expedited flag so that subsequent pushes are added to the end of the queue.
 */
void setNormalActions(void) {
    expedited = FALSE;
}


/**
 * Delete all occurrences of the consumer action.
 * @param action
 */
void deleteActionRange(Action action, uint8_t number) {
    uint8_t tableIndex;
    for (tableIndex=0; tableIndex < NUM_EVENTS; tableIndex++) {
        if (validStart(tableIndex)) {
            Boolean updated = FALSE;
            uint8_t e;
            if (getEVs(tableIndex)) {
                return;
            }
                
            for (e=1; e<PARAM_NUM_EV_EVENT; e++) {
                if ((evs[e] >= action) && (evs[e] < action+number)) {
                    writeEv(tableIndex, e, EV_FILL);
                    updated = TRUE;
                }
            }
            if (updated) {
                checkRemoveTableEntry(tableIndex);
            }
        }
    }
    flushFlashBlock();
#ifdef HASH_TABLE
    rebuildHashtable();                
#endif
}
