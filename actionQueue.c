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
 * File:   actionQueue.c
 * Author: Ian
 *
 * Created on 1 June 2017, 13:14
 *
 * A queue of actions waiting to be processed by the main loop.
 * Implemented as two separated queues - a normal and an expedited priority queue.
 */

#include "mioEvents.h"
#include "actionQueue.h"

uint8_t normalReadIndex;                   // index of the next to read
uint8_t normalWriteIndex;                  // index of the next to write
ActionAndState normalQueueBuf[ACTION_NORMAL_QUEUE_SIZE];   // the actual cyclic buffer space

uint8_t expeditedReadIndex;                   // index of the next to read
uint8_t expeditedWriteIndex;                  // index of the next to write
ActionAndState expeditedQueueBuf[ACTION_EXPEDITED_QUEUE_SIZE];   // the actual cyclic buffer space

static Boolean expedited;

/**
 * Initialise the action queue.
 */
void actionQueueInit(void) {
	normalReadIndex = 0;
	normalWriteIndex = 0;
    
    expeditedReadIndex = 0;
	expeditedWriteIndex = 0;
    
    expedited = FALSE;
}

/**
 * Put the action onto the list of actions to be processed.
 *
 * @param a the action to be processed
 */
Boolean pushTwoAction(ActionAndState a) {
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
ActionAndState * getAction(void) {
	return peekTwoActionQueue(0);
}

/**
 * Pop the next action from the buffer.
 *
 * @return the next action
 */
ActionAndState * popTwoAction(void) {
    ActionAndState * ret;
    
	if (expeditedWriteIndex != expeditedReadIndex) {
        // buffer not empty
        ret = &(expeditedQueueBuf[expeditedReadIndex++]);
        if (expeditedReadIndex >= ACTION_EXPEDITED_QUEUE_SIZE) expeditedReadIndex = 0;
        return ret;
    }
    
	if (normalWriteIndex == normalReadIndex) {
        return NULL;	// buffer empty
    }
	ret = &(normalQueueBuf[normalReadIndex++]);
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
ActionAndState * peekTwoActionQueue(uint8_t index) {
    uint8_t qty;
    qty = (expeditedWriteIndex - expeditedReadIndex) & (ACTION_EXPEDITED_QUEUE_SIZE -1);
    if (index < qty) {
        if (expeditedReadIndex == expeditedWriteIndex) return NULL;    // empty
        index += expeditedReadIndex;
        if (index >= ACTION_EXPEDITED_QUEUE_SIZE) {
            index -= ACTION_EXPEDITED_QUEUE_SIZE;
        }
        if (index == expeditedWriteIndex) return NULL; // at end
        return &(expeditedQueueBuf[index]);      
    }
    index -= qty;
    qty = (normalWriteIndex - normalReadIndex) & (ACTION_NORMAL_QUEUE_SIZE -1);
    if (index < qty) {
        if (normalReadIndex == normalWriteIndex) return NULL;    // empty
        index += normalReadIndex;
        if (index >= ACTION_NORMAL_QUEUE_SIZE) {
            index -= ACTION_NORMAL_QUEUE_SIZE;
        }
        if (index == normalWriteIndex) return NULL; // at end
        return &(normalQueueBuf[index]);
    }
    return NULL;
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
        expeditedQueueBuf[index].a.value = NO_ACTION;
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
        normalQueueBuf[index].a.value = NO_ACTION;
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