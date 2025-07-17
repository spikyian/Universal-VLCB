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
 * File:   analogue.c
 * Author: Ian
 *
 * Created on 16 August 2017, 13:14
 *
 * Process the analogue inputs (include magnet)
 */
#include "module.h"
#include "config.h"
#include "analogue.h"

#include "nvm.h"
#include "universalNv.h"
#include "event_producer.h"
#include "mns.h"
#include "universalEvents.h"

#ifdef ANALOGUE
static unsigned char portInProgress;
AnalogueStates  analogueState[NUM_IO];

unsigned char setupIo;
unsigned char setupState;
static unsigned char haveRequestedAdc;    // indicates if the software has requested an ADC so that when ADCON0.GO is clear then a result is available
extern void configAnalogue(uint8_t i, uint8_t ana);
extern void configDirection(uint8_t i, uint8_t dir); 

#ifdef LEDSW
extern uint8_t outputState[NUM_IO];
#endif


void initAnalogue(void) {
#if defined(_18F66K80_FAMILY_)
    //ADCON1bits.VCFG = 0;    // AVdd + reference
    //ADCON1bits.VNCFG = 0;   // AVss - reference
    ADCON1bits.TRIGSEL = 1; // special trigger CTMU (not used))
    ADCON1bits.CHSN = 0;    // Negative channel AVss
    ADCON2bits.ADFM = 1;    // right justified  values 0 ~ 0x0FFF
    ADCON2bits.ACQT = 2;    // Acquisition 4 Tad cycles 
    ADCON2bits.ADCS = 6;    // Fosc/64 : Tad = 1us
    ADCON0bits.ADON = 1;
#endif
#if defined(_18FXXQ83_FAMILY_)
    //ADREFbits.PREF = 0;     // AVdd + reference
    //ADREFbits.NREF = 0;     // AVss - reference
    ADCON0bits.FM = 1;        // right justified  values 0 ~ 0x0FFF
    ADACQ = 2;                // Acquisition 4 Tad cycles 
    ADCLKbits.CS = 6;         // Fosc/64 : Tad = 1us
    ADCON0bits.ON = 1;
#endif
    portInProgress = 0;
    setupState = SETUP_NONE;
    haveRequestedAdc = 0;
}

void initAnaloguePort(unsigned char io) {
    analogueState[io].eventState = ANALOGUE_EVENT_OFF;
    analogueState[io].portState = ANALOGUE_PORT_INITIALISING;
}


void pollAnalogue(void) {
    uint16_t adc;   // used to store the latest voltage reading (calling it current voltage would be confusing!))
    // these are signed since if the offset is small then lthreshold and lhysteresis can be negative
    short lthreshold;
    short hthreshold;
    short lhysteresis;
    short hhysteresis;
    uint8_t type;
    uint8_t flags;
    
    if (ADCON0bits.GO) {
        // awaiting ADC to complete
        return;
    }


    // are we currently doing a conversion on a valid IO?
    type = (uint8_t)getNV(NV_IO_TYPE(portInProgress));
#ifdef LEDSW
    if (type == TYPE_LEDSW) {
        // turn on the digital output port
        configDirection(portInProgress, 0);
        configAnalogue(portInProgress, 0);
    }
#endif
    if ((type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)
#ifdef LEDSW
            || (type == TYPE_LEDSW)
#endif
            ) {
        // has conversion finished?
        if (haveRequestedAdc) {
            flags = (uint8_t)getNV(NV_IO_FLAGS(portInProgress));
            // get the 12 bit result
            adc = ADRESH;
            adc = adc << 8;
            adc |= ADRESL;
            /*
             * Adc readings are 12 bit 0 to 0x0FFF
             */
            if ((setupState == SETUP_NONE) || (portInProgress != setupIo)) {
                if (type == TYPE_MAGNET) {
                    // TYPE MAGNET
                    // calculate thresholds
                    lthreshold = getNV(NV_IO_MAGNET_OFFSETH(portInProgress));
                    lthreshold = lthreshold << 8;
                    lthreshold |= getNV(NV_IO_MAGNET_OFFSETL(portInProgress));
                    hthreshold = lthreshold; // the offset
                    lthreshold -= getNV(NV_IO_MAGNET_THRES(portInProgress));
                    hthreshold += getNV(NV_IO_MAGNET_THRES(portInProgress));
                    lhysteresis = lthreshold + getNV(NV_IO_MAGNET_HYST(portInProgress));
                    hhysteresis = hthreshold - getNV(NV_IO_MAGNET_HYST(portInProgress));
                    // compare with thresholds
                    if ((analogueState[portInProgress].eventState != ANALOGUE_EVENT_UPPER) && (adc >= hthreshold)) {
                        //High on
                        if (analogueState[portInProgress].portState == ANALOGUE_PORT_READY) {
                            sendProducedEvent(HAPPENING_IO_MAGNETH(portInProgress), (flags & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                        }
                        analogueState[portInProgress].eventState = ANALOGUE_EVENT_UPPER;
                    } else if ((analogueState[portInProgress].eventState == ANALOGUE_EVENT_UPPER) && (adc <= hhysteresis)) {
                        //High Off
                        if (analogueState[portInProgress].portState == ANALOGUE_PORT_READY) {
                            if (!(flags & FLAG_DISABLE_OFF)) {
                                sendProducedEvent(HAPPENING_IO_MAGNETH(portInProgress), (flags & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                            }
                        }
                        analogueState[portInProgress].eventState = ANALOGUE_EVENT_OFF;
                    }
                } else {
                    // TYPE_ANALOGUE or TYPE_LEDSW
                    adc = adc >> 4; // convert to 8 bit
                    if (type == TYPE_ANALOGUE_IN) {
                        // TYPE_ANALOGUE
                        // calculate threshold
                        lthreshold = getNV(NV_IO_ANALOGUE_THRES(portInProgress));
                        lhysteresis = lthreshold - getNV(NV_IO_ANALOGUE_HYST(portInProgress));
                    }
#ifdef LEDSW
                    else {
                        // TYPE_LEDSW
                        lthreshold = getNV(NV_IO_LEDSW_THRESHOLD(portInProgress));
                        lhysteresis = lthreshold - 6;
                    }
#endif
                }
                // This is common between analogue and magnet despite the names
#ifdef LEDSW
                if (flags & FLAG_INPUT_TOGGLE) {
                    // Handle the toggle operation. We'll use the outputState to maintain the
                    // toggle state. No need to use EVENT_INVERTED.
                    if ((analogueState[portInProgress].eventState != ANALOGUE_EVENT_LOWER) && (adc <= lthreshold)) {
                        // Low on 
                        if ((flags & FLAG_TRIGGER_INVERTED) == 0) {
                            if (analogueState[portInProgress].portState == ANALOGUE_PORT_READY) {
                                outputState[portInProgress] = ! outputState[portInProgress];
                                sendProducedEvent(HAPPENING_IO_MAGNETL(portInProgress), outputState[portInProgress]);
                            }
                        } // else ignore the on
                        analogueState[portInProgress].eventState = ANALOGUE_EVENT_LOWER;
                    } else if ((analogueState[portInProgress].eventState == ANALOGUE_EVENT_LOWER) && (adc >= lhysteresis)) {
                        //Low Off
                        if (flags & FLAG_TRIGGER_INVERTED) {
                            if (analogueState[portInProgress].portState == ANALOGUE_PORT_READY) {
                                outputState[portInProgress] = ! outputState[portInProgress];
                                sendProducedEvent(HAPPENING_IO_MAGNETL(portInProgress), outputState[portInProgress]);
                            }
                        } // else ignore the off
                        analogueState[portInProgress].eventState = ANALOGUE_EVENT_OFF;
                    }
                } else {
#endif
                    if ((analogueState[portInProgress].eventState != ANALOGUE_EVENT_LOWER) && (adc <= lthreshold)) {
                        // Low on 
                        if (analogueState[portInProgress].portState == ANALOGUE_PORT_READY) {
                            sendProducedEvent(HAPPENING_IO_MAGNETL(portInProgress), (flags & FLAG_RESULT_EVENT_INVERTED)?EVENT_OFF:EVENT_ON);
                        }
                        analogueState[portInProgress].eventState = ANALOGUE_EVENT_LOWER;
                    } else if ((analogueState[portInProgress].eventState == ANALOGUE_EVENT_LOWER) && (adc >= lhysteresis)) {
                        //Low Off
                        if (analogueState[portInProgress].portState == ANALOGUE_PORT_READY) {
                            if (!(flags & FLAG_DISABLE_OFF)) {
                                sendProducedEvent(HAPPENING_IO_MAGNETL(portInProgress), (flags & FLAG_RESULT_EVENT_INVERTED)?EVENT_ON:EVENT_OFF);
                            }
                        }
                        analogueState[portInProgress].eventState = ANALOGUE_EVENT_OFF;
                    }
#ifdef LEDSW
                }
#endif
            } else {
                // we are setting up this channel
                sendMessage7(OPC_ARSON3, nn.bytes.hi, nn.bytes.lo, 0, portInProgress + 1, portInProgress + 1, adc&0xff, adc>>8);
                if (setupState == SETUP_REPORT_AND_SAVE) {
                    // save the offset
                    saveNV(NV_IO_MAGNET_OFFSETH(portInProgress), adc&0xff);
                    saveNV(NV_IO_MAGNET_OFFSETL(portInProgress), adc>>8);
                }
                setupState = SETUP_NONE;
            }
            analogueState[portInProgress].portState = ANALOGUE_PORT_READY;
        }
    }
    // try next IO
    portInProgress++;
    if (portInProgress >= NUM_IO) {
        portInProgress = 0;
    }
    // If necessary start a conversion
    type = (uint8_t)getNV(NV_IO_TYPE(portInProgress));
#ifdef LEDSW
    if (type == TYPE_LEDSW) {
        // turn off the digital port
        configDirection(portInProgress, 1);
        configAnalogue(portInProgress, 1);
    }
#endif
    if ((type == TYPE_ANALOGUE_IN) || (type == TYPE_MAGNET)
#ifdef LEDSW
            || (type == TYPE_LEDSW)
#endif
            ) {
        // start a conversion
#if defined(_18F66K80_FAMILY_)
        ADCON0bits.CHS = configs[portInProgress].an;
#endif
#if defined(_18FXXQ83_FAMILY_)
        ADPCH = (uint8_t)(((configs[portInProgress].port - 'A') << 3) | (configs[portInProgress].no));
#endif
        ADCON0bits.GO = 1;
        haveRequestedAdc = 1;
    }
}
/*
uint16_t getChannel(unsigned char ch) {
    if ((getNV(NV_IO_TYPE(ch)) == TYPE_ANALOGUE_IN) || (getNV(NV_IO_TYPE(ch)) == TYPE_MAGNET)) {
        return lastReading[ch];
    }
    return 0;
}
*/
#endif
