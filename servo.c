
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
 * File:   servo.c
 * Author: Ian
 * 
 * Handle the servo outputs. The output signal is a pulse between 1ms and 2ms where the width of the
 * pulse results in the servo moving to an angle. The outputs are driven by Timers to ensure that the
 * pulse width is accurate - although if interrupts are disabled then the width could be longer than
 * intended.
 * Pulses are output approximately every 20ms. Therefore we need more than 1 timer for all 16 possible 
 * servo outputs (16 * 2ms = 32ms which is greater than the 20ms available). A minimum of 2 timers 
 * (each handling 8 servos) is required but if we allow overdrive beyond 2ms then 3 (6 servos) or 
 * 4 (4 servos) timers would be better.
 * Here we use 2 timers. Timer1 and Timer3.
 * 
 * The timers are driven from Fosc/4 and use a 1:4 prescalar. With a 16MHz resonator and 4x PLL this 
 * equates to a timer increment every 0.25us. We require counts from 1ms to 2ms or 4000 - 8000 timer ticks.
 * We have an 8 bit position value and actually want to allow a bit of overdrive of the servo 0.9ms - 2.1ms.
 * (3600 ticks - 8400 ticks). This gives a range of 4800 ticks over the 8 bit range. Therefore each value
 * of the position is equivalent to 18.75 ticks - let's call it 19. The 3600 ticks at position 0 so to 
 * convert from position to ticks we need to use:
 *    Ticks = 3600 + 19 * position 
 * This is fine for the 16bit Timer1 and Timer3 but the 8 bit timers Timer2 and Timer4 need a bit more work.
 * 
 * The current servo position is saved in EEPROM at EE_OP_STATE so it can be restored on power up
 * and not cause a sudden jerk movement.
 *
 * Created on 17 April 2017, 13:14
 */

#include "module.h"
#ifdef SERVO
#include "universalNv.h"
#include "universalEvents.h"
#include "config.h"

#include "ticktime.h"
#include "nvm.h"
#include "universalEEPROM.h"
#include "servo.h"
#ifdef BOUNCE
#include "bounce.h"
#endif
#include "outputs.h"
#include "event_producer.h"


// These setting for 1ms to 2ms
#define POS2TICK_OFFSET                 3600    // change this to affect the min pulse width
#define POS2TICK_MULTIPLIER             19      // change this to affect the max pulse width
// These setting for 0.5ms to 2.5ms
#define POS2TICK_EXTENDED_OFFSET        1400    // change this to affect the min pulse width
#define POS2TICK_EXTENDED_MULTIPLIER    36      // change this to affect the max pulse width

#define MAX_SERVO_LOOP          250      // Max number of loops 
#define MAX_BOUNCE_LOOP         250      // Max number of loops 
//#define MAX_MULTI_LOOP          100      // Max number of loops 

#define SERVOS_IN_BLOCK         8

// forward definitions
void setupTimer1(uint8_t io);
void setupTimer3(uint8_t io);

// Externs
extern void setOutputPin(uint8_t io, Boolean state);
extern TickValue   lastServoStartTime;

// Variables
#pragma udata servo_data

ServoState servoState[NUM_IO];
uint8_t currentPos[NUM_IO];
uint8_t targetPos[NUM_IO];
uint8_t stepsPerPollSpeed[NUM_IO];
uint8_t pollsPerStepSpeed[NUM_IO];
uint8_t pollCount[NUM_IO];
int speed[NUM_IO];
uint8_t loopCount[NUM_IO];

#define MAX_BOUNCE_LOOPS    255

#define EVENT_FLAG_ON       0x01
#define EVENT_FLAG_OFF      0x02
#define EVENT_FLAG_MID      0x04
#define EVENT_FLAG_POS1     0x08
#define EVENT_FLAG_POS2     0x10
#define EVENT_FLAG_POS3     0x20
#define EVENT_FLAG_POS4     0x40
TickValue  ticksWhenStopped[NUM_IO];

#pragma udata

static uint8_t servoInBlock;
static uint8_t servoActive[2];  // one for each block of servos

void initServos(void) {
    uint8_t io;
    for (io=0; io<NUM_IO; io++) {
        // try STOPPED state to reduced or correct power on jump
        if (getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_STARTUP) {
            /* This will send 1sec of pulses at power up */
            servoState[io] = SS_STOPPED;
        } else {
            servoState[io] = SS_OFF;
        }
        ticksWhenStopped[io].val = tickGet();
        if (getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_STARTUP) {
            setOutputPosition(io, (uint8_t)readNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io), (uint8_t)getNV(NV_IO_TYPE(io)));   // restore last known positions
        } else {
            setOutputPosition(io, (uint8_t)getNV(NV_IO_SERVO_START_POS(io)), (uint8_t)getNV(NV_IO_TYPE(io)));    // START pos
        }
        stepsPerPollSpeed[io] = 0;
    }
    
#if defined(_18F66K80_FAMILY_)
    // initialise the timers for one-shot mode with interrupts and clocked from Fosc/4
    T1GCONbits.TMR1GE = 0;      // gating disabled
    T1CONbits.TMR1CS = 0;       // clock source Fosc/4
    T1CONbits.T1CKPS = 2;       // 1:4 prescalar
    T1CONbits.SOSCEN = 1;       // clock source Fosc
    T1CONbits.RD16 = 1;         // 16bit read/write
    PIE1bits.TMR1IE = 1;        // enable interrupt
    
    T3GCONbits.TMR3GE = 0;      // gating disabled
    T3CONbits.TMR3CS = 0;       // clock source Fosc/4
    T3CONbits.T3CKPS = 2;       // 1:4 prescalar
    T3CONbits.SOSCEN = 1;       // clock source Fosc
    T3CONbits.RD16 = 1;         // 16bit read/write
    PIE2bits.TMR3IE = 1;        // enable interrupt
#endif
#if defined(_18FXXQ83_FAMILY_)
    T1GCONbits.GE = 0;          // Gating disabled
    T1CLKbits.CS = 1;           // Fosc/4
    T1CONbits.CKPS = 2;         // 1:4 prescalar
    T1CONbits.RD16 = 1;         // 16bit mode
    TMR1 = 0;
    PIR3bits.TMR1IF = 0;
    PIE3bits.TMR1IE = 1;        // enable interrupts
    
    T3GCONbits.GE = 0;          // Gating disabled
    T3CLKbits.CS = 1;           // Fosc/4
    T3CONbits.CKPS = 2;         // 1:4 prescalar
    T3CONbits.RD16 = 1;         // 16bit mode
    TMR3 = 0;
    PIR5bits.TMR3IF = 0;
    PIE5bits.TMR3IE = 1;        // enable interrupts
#endif
    servoInBlock = io -1;
    servoActive[0] = 0;
    servoActive[1] = 0;
}
/**
 * This gets called every approx 2.5ms to start the next set of servo pulses.
 * Checks that the servo isn't OFF
 * @return whether we were able to start the servo
 * @param io
 */
uint8_t startServos(void) {
    uint8_t type;
    // increment block before calling setup so that block is left as the current block whilst the
    // timers expire
    if (servoActive[0] || servoActive[1]) return 0;
    servoInBlock++;
    if (servoInBlock >= SERVOS_IN_BLOCK) {
        servoInBlock = 0;
        pollServos();
    }
    type = (uint8_t)getNV(NV_IO_TYPE(servoInBlock));
    if ((type == TYPE_SERVO) || (type == TYPE_BOUNCE) || (type == TYPE_MULTI)) {
        if (servoState[servoInBlock] != SS_OFF) setupTimer1(servoInBlock);
    }
    type = (uint8_t)getNV(NV_IO_TYPE(servoInBlock+SERVOS_IN_BLOCK));
    if ((type == TYPE_SERVO) || (type == TYPE_BOUNCE) || (type == TYPE_MULTI)) {
        if (servoState[servoInBlock+SERVOS_IN_BLOCK] != SS_OFF) setupTimer3(servoInBlock+SERVOS_IN_BLOCK);
    }
    return 1;
}

/**
 * The setupTimer start the Timer as a on-shot for the servo output pulse of
 * a width of that required for the required position angle.
 * @param io
 */
void setupTimer1(uint8_t io) {
    uint16_t ticks = 0xFFFF - ((getNV(NV_IO_FLAGS(io)) & FLAG_SERVO_EXTENDED_TRAVEL) ? 
                (POS2TICK_EXTENDED_OFFSET + (uint16_t)POS2TICK_EXTENDED_MULTIPLIER * currentPos[io]) :
                (POS2TICK_OFFSET + (uint16_t)POS2TICK_MULTIPLIER * currentPos[io]) );
    TMR1H = ticks >> 8;     // set the duration. Negative to count up to 0x0000 when it generates overflow interrupt
    TMR1L = ticks & 0xFF;

    // turn on output
    servoActive[0] = 1;
    setOutputPin(io, !(getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_ACTION_INVERTED));
    T1CONbits.TMR1ON = 1;       // enable Timer1
}
void setupTimer3(uint8_t io) {
    uint16_t ticks = 0xFFFF - ((getNV(NV_IO_FLAGS(io)) & FLAG_SERVO_EXTENDED_TRAVEL) ? 
                (POS2TICK_EXTENDED_OFFSET + (uint16_t)POS2TICK_EXTENDED_MULTIPLIER * currentPos[io]) :
                (POS2TICK_OFFSET + (uint16_t)POS2TICK_MULTIPLIER * currentPos[io]) );
    TMR3H = ticks >> 8;
    TMR3L = ticks & 0xFF;     // set the duration. Negative to count up to 0x0000 when it generates overflow interrupt

    // turn on output
    servoActive[1] = 1;
    setOutputPin(io, !(getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_ACTION_INVERTED));
    T3CONbits.TMR3ON = 1;       // enable Timer3
}

#if defined(_18F66K80_FAMILY_)
/**
 * These TimerDone routines are called when the on-shot timer expires so we
 * disable the timer and turn the output pin off. 
 * Don't recheck IO type here as it shouldn't be necessary and we want to be as quick as possible.
 */
void timer1DoneInterruptHandler(void) {
    T1CONbits.TMR1ON = 0;       // disable Timer1
    setOutputPin(servoInBlock, (uint8_t)getNV(NV_IO_FLAGS(servoInBlock)) & FLAG_OUTPUT_ACTION_INVERTED);
    servoActive[0] = 0;
}

void timer3DoneInterruptHandler(void) {
    T3CONbits.TMR3ON = 0;       // disable Timer3
    setOutputPin(servoInBlock+SERVOS_IN_BLOCK, (uint8_t)getNV(NV_IO_FLAGS(servoInBlock+SERVOS_IN_BLOCK)) & FLAG_OUTPUT_ACTION_INVERTED);
    servoActive[1] = 0;
}
#endif
#if defined(_18FXXQ83_FAMILY_)
/**
 * The servo end pulse interrupt service routine. 
 */
void __interrupt(irq(TMR1), base(IVT_BASE)) TMR1_ISR(void)
{
    // Tick Timer interrupt
    //check to see if the symbol timer overflowed
    if(PIR3bits.TMR1IF) {
        /* there was a timer overflow */
        PIR3bits.TMR1IF = 0;
        T1CONbits.TMR1ON = 0;       // disable Timer1
        setOutputPin(servoInBlock, (uint8_t)getNV(NV_IO_FLAGS(servoInBlock)) & FLAG_OUTPUT_ACTION_INVERTED);
        servoActive[0] = 0;
    }
    return;
}
void __interrupt(irq(TMR3), base(IVT_BASE)) TMR3_ISR(void)
{
    // Tick Timer interrupt
    //check to see if the symbol timer overflowed
    if(PIR5bits.TMR3IF) {
        /* there was a timer overflow */
        PIR5bits.TMR3IF = 0;
        T3CONbits.TMR3ON = 0;       // disable Timer1
        setOutputPin(servoInBlock+SERVOS_IN_BLOCK, (uint8_t)getNV(NV_IO_FLAGS(servoInBlock+SERVOS_IN_BLOCK)) & FLAG_OUTPUT_ACTION_INVERTED);  
        servoActive[1] = 0;
    }
    return;
}
#endif

/**
 * This handles the servo state machine and moves the servo towards the required
 * position and generates the Produced events. Called approx every 100ms i.e. 10 times a second.
 * Therefore to move a servo through 250 positions using a speed of 5 will take 5 seconds.
 * Typical speeds might be:
 * For a turnout: 25 = 1 second
 * For a signal:  40 = 0.6 seconds
 * For a level crossing gate: 5 = 5 seconds
 */
void pollServos(void) {
    uint8_t midway;
    Boolean beforeMidway;
    uint8_t io;
    uint8_t target;
    
    for (io=0; io<NUM_IO; io++) {
        switch (getNV(NV_IO_TYPE(io))) {
            case TYPE_SERVO:
                midway = ((uint8_t)getNV(NV_IO_SERVO_END_POS(io)))/2 + 
                    ((uint8_t)getNV(NV_IO_SERVO_START_POS(io)))/2;
                beforeMidway=FALSE;
                switch (servoState[io]) {
                    case SS_STARTING:
                        if (currentPos[io]==getNV(NV_IO_SERVO_START_POS(io))) {
                            sendProducedEvent(HAPPENING_IO_SERVO_START(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                        } else {
                            sendProducedEvent(HAPPENING_IO_SERVO_END(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                        }
                        servoState[io] = SS_MOVING;
                        // fall through
                    case SS_MOVING:
                        if (targetPos[io] > currentPos[io]) {
                            if (currentPos[io] < midway) {
                                beforeMidway = TRUE;
                            }
                            
                            if (stepsPerPollSpeed[io]) {
                                if ((uint8_t)(currentPos[io] + stepsPerPollSpeed[io]) < currentPos[io]) {
                                    // will wrap over
                                    currentPos[io] =255;
                                } else {
                                    currentPos[io] += stepsPerPollSpeed[io];
                                }
                            } else {
                                pollCount[io]--;
                                if (pollCount[io] == 0) {
                                    if (currentPos[io] < 255) currentPos[io]++;
                                    pollCount[io] = pollsPerStepSpeed[io];
                                }
                            }
                            
                            if (currentPos[io] > targetPos[io]) {
                                currentPos[io] = targetPos[io];
                            }
                            if ((currentPos[io] >= midway) && beforeMidway) {
                                // passed through midway point
                                // we send an ACON/ACOF depending upon direction servo was moving
                                // This can then be used to drive frog switching relays
                                sendProducedEvent(HAPPENING_IO_SERVO_MID(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                            }
                        } else if (targetPos[io] < currentPos[io]) {
                            if (currentPos[io] > midway) {
                                beforeMidway = TRUE;
                            }
                            
                            if (stepsPerPollSpeed[io]) {
                                if (stepsPerPollSpeed[io] >= currentPos[io]) {
                                    // would under wrap
                                    currentPos[io] = 0;
                                } else {
                                    currentPos[io] -= stepsPerPollSpeed[io];
                                }
                            } else {
                                pollCount[io]--;
                                if (pollCount[io] == 0) {
                                    if (currentPos[io] > 0) currentPos[io]--;
                                    pollCount[io] = pollsPerStepSpeed[io];
                                }
                            }

                            if (currentPos[io] < targetPos[io]) {
                                currentPos[io] = targetPos[io];
                            }
                            if ((currentPos[io] <= midway) && beforeMidway) {
                                // passed through midway point
                                sendProducedEvent(HAPPENING_IO_SERVO_MID(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                            }
                        }
                        if (targetPos[io] == currentPos[io]) {
                            servoState[io] = SS_STOPPED;
                            ticksWhenStopped[io].val = tickGet();
                            // send ON event or OFF
                            if (currentPos[io] == getNV(NV_IO_SERVO_START_POS(io))) { //ON means move to End
                                sendProducedEvent(HAPPENING_IO_SERVO_START(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                            } else {
                                sendProducedEvent(HAPPENING_IO_SERVO_END(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                            }
                            writeNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io, currentPos[io]);
                        }
                        break;
                    case SS_STOPPED:
                    case SS_OFF:
                        break;
                }
                break;
            case TYPE_BOUNCE:
                switch (servoState[io]) {
                    case SS_STARTING:
                        initBounce(io);
                        servoState[io] = SS_MOVING;
                        loopCount[io] = 0;
                        
                        // fall through
                    case SS_MOVING:
                        loopCount[io]++;
                        if (loopCount[io] >= MAX_BOUNCE_LOOPS) {
                            servoState[io] = SS_STOPPED;
                            ticksWhenStopped[io].val = tickGet();
                            currentPos[io] = targetPos[io];
                            sendProducedEvent(HAPPENING_IO_BOUNCE(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                            writeNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io, currentPos[io]);
                            break;
                        }
                        // Implement the bounce algorithm here
                        target = (uint8_t)getNV(NV_IO_BOUNCE_UPPER_POS(io));
//                        if (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_ACTION_INVERTED) {
//                            target = getNV(NV_IO_BOUNCE_LOWER_POS(io));
//                        }
                        if (targetPos[io] == target) {
                            if (bounceUp(io)) {
                                servoState[io] = SS_STOPPED;
                                ticksWhenStopped[io].val = tickGet();
                                currentPos[io] = targetPos[io];
                                sendProducedEvent(HAPPENING_IO_BOUNCE(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                                writeNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io, currentPos[io]);
                            }
                        } else {
                            if (bounceDown(io)) {
                                servoState[io] = SS_STOPPED;
                                ticksWhenStopped[io].val = tickGet();
                                currentPos[io] = targetPos[io];
                                sendProducedEvent(HAPPENING_IO_BOUNCE(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                                writeNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io, currentPos[io]);
                            }
                        }
                        break;
                    case SS_STOPPED:
                    case SS_OFF:
                        break;
                }
                break;
            case TYPE_MULTI:
                switch (servoState[io]) {
                    case SS_STARTING:
                        if (currentPos[io] == getNV(NV_IO_MULTI_POS1(io))) {
                            sendProducedEvent(HAPPENING_IO_MULTI_AT1(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                        }
                        if (currentPos[io] == getNV(NV_IO_MULTI_POS2(io))) {
                            sendProducedEvent(HAPPENING_IO_MULTI_AT2(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                        }
                        if (currentPos[io] == getNV(NV_IO_MULTI_POS3(io))) {
                            sendProducedEvent(HAPPENING_IO_MULTI_AT3(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                        }
                        if (currentPos[io] == getNV(NV_IO_MULTI_POS4(io))) {
                            sendProducedEvent(HAPPENING_IO_MULTI_AT4(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                        }
                        servoState[io] = SS_MOVING;
                        // fall through
                    case SS_MOVING:
                        if (targetPos[io] > currentPos[io]) {
                            if (stepsPerPollSpeed[io]) {
                                if (currentPos[io] + stepsPerPollSpeed[io] < currentPos[io]) {
                                    // will wrap
                                    currentPos[io] =255;
                                } else {
                                    currentPos[io] += stepsPerPollSpeed[io];
                                }
                            } else {
                                pollCount[io]--;
                                if (pollCount[io] == 0) {
                                    currentPos[io]++;
                                    pollCount[io] = pollsPerStepSpeed[io];
                                }
                            }
                            if (currentPos[io] > targetPos[io]) {
                                currentPos[io] = targetPos[io];
                            }
                        } else if (targetPos[io] < currentPos[io]) {
                              
                            if (stepsPerPollSpeed[io]) {
                                if (currentPos[io] - stepsPerPollSpeed[io] > currentPos[io]) {
                                    // would under wrap
                                    currentPos[io] = 0;
                                } else {
                                    currentPos[io] -= stepsPerPollSpeed[io];
                                }
                            } else {
                                pollCount[io]--;
                                if (pollCount[io] == 0) {
                                    currentPos[io]--;
                                    pollCount[io] = pollsPerStepSpeed[io];
                                }
                            }
                            if (currentPos[io] < targetPos[io]) {
                                currentPos[io] = targetPos[io];
                            }
                        }
                        if (targetPos[io] == currentPos[io]) {
                            servoState[io] = SS_STOPPED;
                            ticksWhenStopped[io].val = tickGet();
                            // MULTI only sends ON events. Work out which event
                            if (currentPos[io] == getNV(NV_IO_MULTI_POS1(io))) {
                                sendProducedEvent(HAPPENING_IO_MULTI_AT1(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                            }
                            if (currentPos[io] == getNV(NV_IO_MULTI_POS2(io))) {
                                sendProducedEvent(HAPPENING_IO_MULTI_AT2(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                            }
                            if (currentPos[io] == getNV(NV_IO_MULTI_POS3(io))) {
                                sendProducedEvent(HAPPENING_IO_MULTI_AT3(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                            }
                            if (currentPos[io] == getNV(NV_IO_MULTI_POS4(io))) {
                                sendProducedEvent(HAPPENING_IO_MULTI_AT4(io), (getNV(NV_IO_FLAGS(io)) & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                            }
                            writeNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io, currentPos[io]);
                        }
                        break;
                    case SS_STOPPED:
                    case SS_OFF:
                        break;
                }
                break;
        }
        switch (getNV(NV_IO_TYPE(io))) {
            case TYPE_SERVO:
            case TYPE_BOUNCE:
            case TYPE_MULTI:
                switch (servoState[io]) {
                    case SS_STARTING:
                    case SS_MOVING:
                        break;
                    case SS_STOPPED:
                        // if we have been stopped for more than 1 sec then change to OFF
                        // If FLAG_CUTOFF isn't set then we never reach OFF
                        if (getNV(NV_IO_FLAGS(io)) & FLAG_SERVO_CUTOFF) {
                            if (tickTimeSince(ticksWhenStopped[io]) > ONE_SECOND) {
                                servoState[io] = SS_OFF;
                            }
                        }
                        break;
                    case SS_OFF:
                        // output off
                        // For better noise immunity set output high unless the output is inverted.
                        setOutputPin(io, !(getNV(NV_IO_FLAGS(io)) & FLAG_OUTPUT_ACTION_INVERTED));
                        // no need to do anything since if output is OFF we don't start the timer in startServos
                        break;
                }
                break;
        }
    }
}


/**
 * Start a servo moving to the required state. 
 * Called for SERVO types.
 * Handles inverted outputs and generates Produced events.
 * 
 * @param io
 * @param action
 */
void startServoOutput(uint8_t io, uint8_t action) {
    switch (action) {
        case ACTION_IO_3:  // SERVO OFF
            stepsPerPollSpeed[io] = (uint8_t)getNV(NV_IO_SERVO_ES_SPEED(io));
            break;
        case ACTION_IO_2:  // SERVO ON
            stepsPerPollSpeed[io] = (uint8_t)getNV(NV_IO_SERVO_SE_SPEED(io));
            break;
    }
    if (stepsPerPollSpeed[io] > PIVOT) {
        stepsPerPollSpeed[io] -= PIVOT;
        pollsPerStepSpeed[io] = 0;
    } else {
        pollsPerStepSpeed[io] = PIVOT - stepsPerPollSpeed[io] +1;
        pollCount[io] = 1;
        stepsPerPollSpeed[io] = 0;
    }
    servoState[io] = SS_STARTING;
}

/**
 * Start a servo moving to the required state. 
 * Called for BOUNCE types.
 * Handles inverted outputs and generates Produced events.
 * 
 * @param io
 * @param action
 */
void startBounceOutput(uint8_t io, uint8_t action) {
    switch (action) {
        case ACTION_IO_3:  // SERVO OFF
            speed[io] = 0;
            break;
        case ACTION_IO_2:  // SERVO ON
            speed[io] = PULL_SPEED;
            break;
    }
    servoState[io] = SS_STARTING;
    loopCount[io] = 0;
}

/**
 * Sets a servo multi-position output. generates produced events.
 * @param io
 * @param action
 */
void startMultiOutput(uint8_t io, uint8_t action) {

    stepsPerPollSpeed[io] = (uint8_t)getNV(NV_SERVO_SPEED);
    if (stepsPerPollSpeed[io] > PIVOT) {
        stepsPerPollSpeed[io] -= PIVOT;
        pollsPerStepSpeed[io] = 0;
    } else {
        pollsPerStepSpeed[io] = PIVOT - stepsPerPollSpeed[io] +1;
        pollCount[io] = 1;
        stepsPerPollSpeed[io] = 0;   
    }
    servoState[io] = SS_STARTING;
}

/**
 * Set a servo moving to the required state. 
 * Called for SERVO types.
 * Handles inverted outputs and generates Produced events.
 * 
 * @param io
 * @param action
 */
void setServoState(uint8_t io, uint8_t action) {
    switch (action) {
        case ACTION_IO_3:  // SERVO OFF
            if (getNV(NV_IO_FLAGS(io)) & FLAG_TRIGGER_INVERTED) {
                targetPos[io] = (uint8_t)getNV(NV_IO_SERVO_END_POS(io));
            } else {
                targetPos[io] = (uint8_t)getNV(NV_IO_SERVO_START_POS(io));
            }
            break;
        case ACTION_IO_2:  // SERVO ON
            if (getNV(NV_IO_FLAGS(io)) & FLAG_TRIGGER_INVERTED) {
                targetPos[io] = (uint8_t)getNV(NV_IO_SERVO_START_POS(io));
            } else {
                targetPos[io] = (uint8_t)getNV(NV_IO_SERVO_END_POS(io));
            }
            break;
    }
}

/**
 * Set a servo moving to the required state. 
 * Called for BOUNCE types.
 * Handles inverted outputs and generates Produced events.
 * 
 * @param io
 * @param action
 */
void setBounceState(uint8_t io, uint8_t action) {
    switch (action) {
        case ACTION_IO_3:  // SERVO OFF
            if (getNV(NV_IO_FLAGS(io)) & FLAG_TRIGGER_INVERTED) {
                targetPos[io] = (uint8_t)getNV(NV_IO_BOUNCE_UPPER_POS(io));
            } else {
                targetPos[io] = (uint8_t)getNV(NV_IO_BOUNCE_LOWER_POS(io));
            }
            break;
        case ACTION_IO_2:  // SERVO ON
            if (getNV(NV_IO_FLAGS(io)) & FLAG_TRIGGER_INVERTED) {
                targetPos[io] = (uint8_t)getNV(NV_IO_BOUNCE_LOWER_POS(io));
            } else {
                targetPos[io] = (uint8_t)getNV(NV_IO_BOUNCE_UPPER_POS(io));
            }
            break;
    }
}

/**
 * Sets a servo multi-position output. generates produced events.
 * @param io
 * @param action
 */
void setMultiState(uint8_t io, uint8_t action) {
    switch (action) {
        case ACTION_IO_1:  // SERVO Position 1
            targetPos[io] = (uint8_t)getNV(NV_IO_MULTI_POS1(io));
            break;
        case ACTION_IO_2:  // SERVO Position 2
            targetPos[io] = (uint8_t)getNV(NV_IO_MULTI_POS2(io));
            break;
        case ACTION_IO_3:  // SERVO Position 3
            if (getNV(NV_IO_MULTI_NUM_POS(io)) >= 3) {
                targetPos[io] = (uint8_t)getNV(NV_IO_MULTI_POS3(io));
            }
            break;
        case ACTION_IO_4:  // SERVO Position 4
            if (getNV(NV_IO_MULTI_NUM_POS(io)) >= 4) {
                targetPos[io] = (uint8_t)getNV(NV_IO_MULTI_POS4(io));
            }
            break;
    }
}

/**
 * explicitly set the recorded position of a servo.
 * @param io
 * @param pos
 */
void setServoPosition(uint8_t io, uint8_t pos) {
    targetPos[io] = pos;
    currentPos[io] = pos;
}

/**
 * Check if now is a good time to write to flash. This must not happen whilst
 * a servo pulse is being produced.
 * 
 * @return 
 */
Boolean isNoServoPulses(void){
    // check if either of the timers is running
    if (T1CONbits.TMR1ON) return FALSE;
    if (T3CONbits.TMR3ON) return FALSE;
    // Timer not running and won't timeout with interrupt which would need to be handled quickly
    return TRUE;
}

#endif

