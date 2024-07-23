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
 *	The Main CANMIO program supporting configurable I/O.
 *
 * @author Ian Hogg 
 * @date Apr 2024
 * 
 */ 
/**
 * @copyright Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 */

#include <xc.h>
#include "module.h"
#include "vlcb.h"
// the services
#include "mns.h"
#include "nv.h"
#include "can.h"
#include "boot.h"
#include "event_teach.h"
#include "event_consumerDualActionQueue.h"
#include "event_producer.h"
#include "event_acknowledge.h"
#include "event_coe.h"
// others
#include "ticktime.h"
#include "timedResponse.h"


/**************************************************************************
 * Application code packed with the bootloader must be compiled with options:
 * XC8 linker options -> Additional options --CODEOFFSET=0x800 
 * This generates an error
 * ::: warning: (2044) unrecognized option "--CODEOFFSET=0x800"
 * but this can be ignored as the option works
 * 
 * Then the Application project must be made dependent on the Bootloader 
 * project by adding the Bootloader to project properties->Conf:->Loading
 ***************************************************************************/

/*
 * File:   main.c
 * Author: Ian Hogg
 * 
 * This is the main for the Configurable CANMIO module.
 * 
 * Timer usage:
 * TMR0 used in ticktime for symbol times. Used to trigger next set of servo pulses
 * TMR1 Servo outputs 0,1,2,3,4,5,6,7
 * TMR3 Servo outputs 8,9,10,11,12,13,14,15
 * TMR2 CANCDU charge pump
 *
 * Created on 10 April 2017, 10:26
 */
/** TODOs
 * Check handling of REQEV events.c
 * Consider option to set outputs on or off on powerup in addition to restore previous state
 * Randomise bounce
 * RFID input
 * CHECK DOCS
 *
 */

#include "devincs.h"
#include <stddef.h>
#include "module.h"
#include "config.h"
#include "statusLeds.h"
#include "inputs.h"
#include "universalEEPROM.h"
#include "universalNv.h"
#include "nvm.h"
#include "universalEvents.h"
#ifdef SERVO
#include "servo.h"
#endif
#ifdef ANALOGUE
#include "analogue.h"
#endif
#include "digitalOut.h"
#ifdef CANCDU
#include "cdu.h"
#endif
#include "outputs.h"
#include "timedResponse.h"



// PIN configs
const Config configs[NUM_IO] = {
    //PIN, PORT, PORT#, AN#
#ifdef CANXIO
    {32, 'C', 0, 0xFF}, //0
    {35, 'C', 1, 0xFF}, //1
    {36, 'C', 2, 0xFF}, //2
    {37, 'C', 3, 0xFF}, //3
    {42, 'C', 4, 0xFF}, //4
    {43, 'C', 5, 0xFF}, //5
    {44, 'C', 6, 0xFF}, //6
    {1,  'C', 7, 0xFF}, //7
    {8,  'B', 0, 10},   //8
    {9,  'B', 1, 8},    //9
    {14, 'B', 4, 9},    //10
    {15, 'B', 5, 0xFF}, //11
    {20, 'A', 1, 1},    //12
    {19, 'A', 0, 0},    //13
    {22, 'A', 3, 3},    //14
    {24, 'A', 5, 4},    //15    
    {38, 'D', 0, 0xFF}, //16
    {39, 'D', 1, 0xFF}, //17
    {40, 'D', 2, 0xFF}, //18
    {41, 'D', 3, 0xFF}, //19
    {2,  'D', 4, 0xFF}, //20
    {3,  'D', 5, 0xFF}, //21
    {25, 'E', 0, 5},    //22
    {26, 'E', 1, 6}     //23   
#else
    {11, 'C', 0, 0xFF},   //0
    {12, 'C', 1, 0xFF},   //1
    {13, 'C', 2, 0xFF},   //2
    {14, 'C', 3, 0xFF},   //3
    {15, 'C', 4, 0xFF},   //4
    {16, 'C', 5, 0xFF},   //5
    {17, 'C', 6, 0xFF},   //6
    {18, 'C', 7, 0xFF},   //7
    {21, 'B', 0, 10},   //8
    {22, 'B', 1, 8},   //9
    {25, 'B', 4, 9},   //10
    {26, 'B', 5, 0xFF},   //11
    {3,  'A', 1, 1},   //12
    {2,  'A', 0, 0},   //13
#ifndef CANCDU
    {5,  'A', 3, 3},   //14
    {7,  'A', 5, 4}    //15
#endif
#endif
};

// forward declarations
void __init(void);
uint8_t checkCBUS( void);
void ISRHigh(void);
void initialise(void);
void configIO(uint8_t io);
void factoryReset(void);
void setType(uint8_t i, uint8_t type);
void factoryResetEE(void);
void factoryResetFlash(void);


static TickValue   startTime;
static uint8_t        started;
TickValue   lastServoStartTime;
static TickValue   lastInputScanTime;
static TickValue   lastActionPollTime;
#ifdef CANCDU
static TickValue   lastCduPollTime;
#endif

static uint8_t io;

extern const Service eventConsumer2QService;

const Service * const services[] = {
    &canService,
    &mnsService,
    &nvService,
    &bootService,
    &eventTeachService,
    &eventConsumer2QService,
    &eventProducerService,
    &eventCoeService,
    &eventAckService
};

extern uint8_t outputState[NUM_IO];

/**
 * Called at first run to initialise all the non volatile memory. 
 * Also called if the PB hold down special sequence at power up is done.
 * Also called as a result of a NNRSM request.
 */
void APP_factoryReset(void) {
    uint8_t io;
    
    factoryResetGlobalEvents();
    // perform other actions based upon type
    for (io=0; io<NUM_IO; io++) {
        setType(io, (io < NUM_IO_MAIN) ? TYPE_DEFAULT_MAIN : TYPE_DEFAULT_EXP);
    } 
    flushFlashBlock();
}

/**
 * Called if the PB is held down during power up.
 * Normally would perform any test functionality to help a builder check the hardware.
 */
void APP_testMode(void) {
    
}

/**
 * Called upon power up.
 */
void setup(void) {
#if defined(_18FXXQ83_FAMILY_)
    uint8_t pu;
#endif
    // use CAN as the module's transport
    transport = &canTransport;

    /**
     * The order of initialisation is important.
     */
#if defined(_18F66K80_FAMILY_)
    // Enable PORT B weak pullups
    INTCON2bits.RBPU = 0;
    // RB bits 0,1,4,5 need pullups
    WPUB = (uint8_t)getNV(NV_PULLUPS);
#endif
#if defined(_18FXXQ83_FAMILY_)
    // enable pullups on all channels
    pu = 0xFF;
    for (io=0; io<NUM_IO; io++) {
        if (io == 0) {
            pu = (uint8_t)getNV(NV_LOW_CHANNELS_PULLUPS);
        }
        if (io == 8) {
            pu = (uint8_t)getNV(NV_MID_CHANNELS_PULLUPS);
        }
        if (io == 16) {
            pu = (uint8_t)getNV(NV_TOP_CHANNELS_PULLUPS);
        }
        switch (configs[io].port) {
            case 'A':
                if (pu && (1 << io % 8))
                    WPUA |= 1<<(configs[io].no);
                else
                    WPUA &= ~(1<<(configs[io].no));
                break;
            case 'B':
                if (pu && (1 << io % 8))
                    WPUB |= 1<<(configs[io].no);
                else
                    WPUB &= ~(1<<(configs[io].no));
                break;
            case 'C':
                if (pu && (1 << io % 8))
                    WPUC |= 1<<(configs[io].no);
                else
                    WPUC &= ~(1<<(configs[io].no));
                break;
#ifdef CANXIO
            case 'D':
                if (pu && (1 << io % 8))
                    WPUD |= 1<<(configs[io].no);
                else
                    WPUD &= ~(1<<(configs[io].no));
                break;
            case 'E':
                if (pu && (1 << io % 8))
                    WPUE |= 1<<(configs[io].no);
                else
                    WPUE &= ~(1<<(configs[io].no));
                break;
#endif
        }
    }
#endif
    setTimedResponseDelay((uint8_t)getNV(NV_RESPONSE_DELAY));
    universalEventsInit();

#ifdef ANALOGUE
    initAnalogue();
#endif
#if defined(_18F66K80_FAMILY_)
    // default to all digital IO
    ANCON0 = 0x00;
    ANCON1 = 0x00; 
#endif
#if defined(_18FXXQ83_FAMILY_)
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
#ifdef CANXIO
    ANSELD = 0x00;
    ANSELE = 0x00;
#endif
#endif
    // set the servo state and positions before configuring IO so we reduce the startup twitch
#ifdef SERVO
    initServos();
#endif
    initOutputs();
    // set up io pins based upon type
    // set the ports to the correct type
    for (io=0; io< NUM_IO; io++) {
        configIO(io);
    }
    initInputScan();
#ifdef CANCDU
    initCdus();
#endif

    // enable interrupts, all init now done
    ei(); 

    startTime.val = tickGet();
    lastServoStartTime.val = startTime.val;
    lastInputScanTime.val = startTime.val;
    lastActionPollTime.val = startTime.val;
#ifdef CANCDU
    lastCduPollTime.val = startTime.val;
#endif

    started = FALSE;
}

void loop(void) {
    // Startup delay for CBUS about 2 seconds to let other modules get powered up - ISR will be running so incoming packets processed
    if (!started && (tickTimeSince(startTime) > (getNV(NV_SOD_DELAY) * HUNDRED_MILI_SECOND) + TWO_SECOND)) {
        started = TRUE;
        sendProducedEvent(HAPPENING_SOD, EVENT_ON);
    }

#ifdef SERVO
    if (tickTimeSince(lastServoStartTime) > 5*HALF_MILLI_SECOND) {
        startServos();  // call every 2.5ms
        lastServoStartTime.val = tickGet();
    }
#endif
    if (started) {
        if (tickTimeSince(lastInputScanTime) > 5*ONE_MILI_SECOND) {
            inputScan();    // Strobe inputs for changes
            lastInputScanTime.val = tickGet();
        }
        if (tickTimeSince(lastActionPollTime) > 100*ONE_MILI_SECOND) {
            processActions();
            processOutputs();
            lastActionPollTime.val = tickGet();
        }
        
#ifdef CANCDU
        if (tickTimeSince(lastCduPollTime) > 10*ONE_MILI_SECOND) {
            processCduPulses();
            lastCduPollTime.val = tickGet();
        }
#endif
        
#ifdef ANALOGUE
        pollAnalogue();
#endif
    }
}

// Application functions required by MERGLCB library


/**
 * Check to see if now is a good time to start a flash write.
 * It is a bad time if we are currently doing a servo pulse.
 * 
 * If a servo pulse timer is currently running then the NVM routine will keep 
 * calling this until the timer expires.
 * 
 * @return GOOD_TIME if OK else BAD_TIME
 */
ValidTime APP_isSuitableTimeToWriteFlash(void){
#ifdef SERVO
    return isNoServoPulses() ? GOOD_TIME : BAD_TIME;
#else
    return GOOD_TIME;
#endif
}

/**
 * This application doesn't need to process any messages in a special way.
 */
Processed APP_preProcessMessage(Message * m) {
    return NOT_PROCESSED;
}

/**
 * This application doesn't need to process any messages in a special way.
 */
Processed APP_postProcessMessage(Message * m) {
    return NOT_PROCESSED;
}

/**
 * This is needed by the library to get the current event state. 
 */
EventState APP_GetEventState(Happening h) {
    uint8_t flags;
    uint8_t happeningIndex;
    Boolean disable_off;
    
    io = HAPPENING_IO(h);
    if (io >= NUM_IO) {
        return EVENT_UNKNOWN;
    }
    happeningIndex = HAPPENING(h);
    flags = (uint8_t)getNV(NV_IO_FLAGS(io));
    disable_off = flags & FLAG_DISABLE_OFF;
        
    switch((uint8_t)getNV(NV_IO_TYPE(io))) {
        case TYPE_INPUT:
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    // The TRIGGER_INVERTED has already been taken into account when saved in outputState. No need to check again
                    return outputState[io]?EVENT_ON:EVENT_OFF;
                case HAPPENING_IO_2:
                    // TWO_ON is only sent if DISABLE_OFF is set GP//
                    if (disable_off) {
                        return (outputState[io]==0)?EVENT_ON:EVENT_OFF;
                    }
                    break;
            }
            break;
        case TYPE_OUTPUT:
#ifdef CANCDU
        case TYPE_CDU:
#endif
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    return (readNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io)!=ACTION_IO_3)?EVENT_ON:EVENT_OFF;
            }
            break;
#ifdef SERVO
        case TYPE_SERVO:
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    return (currentPos[io] == (uint8_t)getNV(NV_IO_SERVO_START_POS(io)))?EVENT_ON:EVENT_OFF;
                case HAPPENING_IO_3:
                    return (currentPos[io] == (uint8_t)getNV(NV_IO_SERVO_END_POS(io)))?EVENT_ON:EVENT_OFF;
                    // send the last mid
                case HAPPENING_IO_2:
                    return (currentPos[io] >= ((uint8_t)getNV(NV_IO_SERVO_END_POS(io)))/2 + 
                         ((uint8_t)getNV(NV_IO_SERVO_START_POS(io)))/2)?EVENT_ON:EVENT_OFF;
            }
            break;
#ifdef BOUNCE
        case TYPE_BOUNCE:
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    return ((uint8_t)readNVM(EEPROM_NVM_TYPE, EE_OP_STATE+io))?EVENT_ON:EVENT_OFF;
            }
            break;
#endif
#ifdef MULTI
        case TYPE_MULTI:
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    return (currentPos[io] == getNV(NV_IO_MULTI_POS1(io)))?EVENT_ON:EVENT_OFF;
                case HAPPENING_IO_2:
                    return (currentPos[io] == getNV(NV_IO_MULTI_POS2(io)))?EVENT_ON:EVENT_OFF;
                // it is more logical to use servo rather than multi with <3 positions but check anyway GP//
                case HAPPENING_IO_3:
                    if (getNV(NV_IO_MULTI_NUM_POS(io)) > 2) { 
                        return (currentPos[io] == getNV(NV_IO_MULTI_POS3(io)))?EVENT_ON:EVENT_OFF;
                    }
                    break;
                case HAPPENING_IO_4:
                    if (getNV(NV_IO_MULTI_NUM_POS(io)) > 3) {
                        return (currentPos[io] == getNV(NV_IO_MULTI_POS4(io)))?EVENT_ON:EVENT_OFF;
                    }
                    break;
            }
            break;
#endif
#endif
#ifdef ANALOGUE
        case TYPE_ANALOGUE_IN:
        case TYPE_MAGNET:
            switch (happeningIndex) {
                case HAPPENING_IO_1:
                    return (analogueState[io].eventState == ANALOGUE_EVENT_LOWER)?EVENT_ON:EVENT_OFF;
                case HAPPENING_IO_2:
                    return (analogueState[io].eventState == ANALOGUE_EVENT_UPPER)?EVENT_ON:EVENT_OFF;
            }
            break;
#endif
    }
    return EVENT_UNKNOWN;
}


/**
 * Set the Type of the IO.
 * @param i the IO
 * @param type the new Type
 */
void setType(uint8_t io, uint8_t type) {
    uint8_t index;
    
    index = NV_IO_TYPE(io);
    saveNV((uint8_t)index, type);

    // set to default NVs
    defaultNVs(io, type);
    
    // set the pin input/output
    configIO(io);
#ifdef SERVO
    if ((type == TYPE_SERVO) || (type== TYPE_BOUNCE) || (type == TYPE_MULTI)) {
        currentPos[io] = 128;
    }
#endif
    // set up the default events. 
    defaultEvents(io, type);
#ifdef ANALOGUE
    if ((type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)) {
        initAnaloguePort(io);
    }
#endif
}


/**
 * Set up an IO based upon the specified type.
 * Set the port to input or output then call setOutput for the currently remembered state.
 * @param i the IO
 */
void configIO(uint8_t i) {
    uint8_t action;
    uint8_t type;
    
    if (i >= NUM_IO) return;
    // If this is an output (OUTPUT, SERVO, BOUNCE) set the value to valued saved in EE
    // servos will do this in servo.c
    type = (uint8_t)getNV(NV_IO_TYPE(i));
    switch (type) {
        case TYPE_OUTPUT:
            if (getNV(NV_IO_FLAGS(i)) & FLAG_OUTPUT_STARTUP) {
                setDigitalOutput(i, (uint8_t)readNVM(EEPROM_NVM_TYPE, (eeprom_address_t)EE_OP_STATE+i)); // saved state
            } else {
                action = (getNV(NV_IO_FLAGS(i)) & FLAG_OUTPUT_ACTION_INVERTED) ? ACTION_IO_2 : ACTION_IO_3;
                setDigitalOutput(i, action);  // OFF
                // save the current state of output as OFF so 
                writeNVM(EEPROM_NVM_TYPE, (eeprom_address_t)(EE_OP_STATE+i), action);       // Unsure if this is required or if it should be deleted. Therefore leaving it for now.
            }
            break;
#ifdef SERVO
        case TYPE_SERVO:
#ifdef BOUNCE
        case TYPE_BOUNCE:
#endif
            // start with the polarity defined in flag PULLUP
            setDigitalOutput(i, getNV(NV_IO_FLAGS(i)) & FLAG_SERVO_START_PULLUP);
            break;
#endif
#ifdef CANCDU
        case TYPE_CDU:
            action = (getNV(NV_IO_FLAGS(i)) & FLAG_OUTPUT_ACTION_INVERTED) ? ACTION_IO_2 : ACTION_IO_3;
            setDigitalOutput(i, action);  // OFF
            // save the current state of output as OFF so 
            writeNVM(EEPROM_NVM_TYPE, (eeprom_address_t)(EE_OP_STATE+i), action);       // Unsure if this is required or if it should be deleted. Therefore leaving it for now.
            break;
#endif
    }
    // Now actually set it
    switch (configs[i].port) {
        case 'A':
            if ((type == TYPE_INPUT) || (type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)) {
                TRISA |= (1 << configs[i].no);  // input
            } else {
                TRISA &= ~(1 << configs[i].no); // output
            }
            break;
        case 'B':
            if ((type == TYPE_INPUT) || (type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)) {
                TRISB |= (1 << configs[i].no);  // input
            } else {
                TRISB &= ~(1 << configs[i].no); // output
            }
            break;
        case 'C':
            if ((type == TYPE_INPUT) || (type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)) {
                TRISC |= (1 << configs[i].no);  // input
            } else {
                TRISC &= ~(1 << configs[i].no); // output
            }
            break; 
#ifdef CANXIO        
        case 'D':
            if ((type == TYPE_INPUT) || (type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)) {
                TRISD |= (1 << configs[i].no);  // input
            } else {
                TRISD &= ~(1 << configs[i].no); // output
            }
            break;  
            
        case 'E':
            if ((type == TYPE_INPUT) || (type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)) {
                TRISE |= (1 << configs[i].no);  // input
            } else {
                TRISE &= ~(1 << configs[i].no); // output
            }
            break;             
#endif
    }
#ifdef ANALOGUE
#if defined(_18F66K80_FAMILY_)
    if ((type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)) {
        // set analogue
        if (configs[i].an < 8) {
            ANCON0 |= (1 << configs[i].an);
        } else if (configs[i].an < 16) {
            ANCON1 |= (1 << (configs[i].an - 8));
        }
    } else {
        // set digital
        if (configs[i].an < 8) {
            ANCON0 &= ~(1 << configs[i].an);
        } else if (configs[i].an < 16) {
            ANCON1 &= ~(1 << (configs[i].an - 8));
        }
    }
#endif
#if defined(_18FXXQ83_FAMILY_)
    if ((type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)) {
        // set analogue
        switch (configs[i].port) {
            case 'A':
                ANSELA |= (1 << configs[i].no);
                break;
            case 'B':
                ANSELB |= (1 << configs[i].no);
                break;
            case 'C':
                ANSELC |= (1 << configs[i].no);
                break;
#ifdef CANXIO
            case 'D':
                ANSELD |= (1 << configs[i].no);
                break;
            case 'E':
                ANSELE |= (1 << configs[i].no);
                break;
#endif
        }
    } else {
        // set digital
        switch (configs[i].port) {
            case 'A':
                ANSELA &= ~(1 << configs[i].no);
                break;
            case 'B':
                ANSELB &= ~(1 << configs[i].no);
                break;
            case 'C':
                ANSELC &= ~(1 << configs[i].no);
                break;
#ifdef CANXIO
            case 'D':
                ANSELD &= ~(1 << configs[i].no);
                break;
            case 'E':
                ANSELE &= ~(1 << configs[i].no);
                break;
#endif
        }
    }
#endif
#endif
}



#if defined(_18F66K80_FAMILY_)

// APP Interrupt service routines
void APP_lowIsr(void) {
}

// Interrupt service routines
void APP_highIsr(void) {
#if defined(SERVO) || defined(BOUNCE) || defined (MULTI)
 /* service the servo pulse width timers */
    if (PIR1bits.TMR1IF) {
        timer1DoneInterruptHandler();
        PIR1bits.TMR1IF = 0;
    }
    if (PIR2bits.TMR3IF) {
        timer3DoneInterruptHandler();
        PIR2bits.TMR3IF = 0;
    }
#endif
}
#endif
