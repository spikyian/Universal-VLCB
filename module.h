#ifndef _MODULE_H_
#define _MODULE_H_

// comment out for CBUS
#define VLCB


#include "statusLeds.h"

//
// VLCB Service options first
//
// The data version stored at NV#0
#define APP_NVM_VERSION 1
#define NUM_SERVICES 9

// The hardware
#define CANMIO
#define NUM_IO 16
#if defined(_18FXXQ83_FAMILY_)
#define IVT_BASE      0x900
#define IVT_BASE_U    0x00
#define IVT_BASE_H    0x09
#define IVT_BASE_L    0x00
#endif

//
// NV service
//
#define NV_NUM  127
#if defined(_18F66K80_FAMILY_)
#define NV_ADDRESS  0xFF80
#define NV_NVM_TYPE FLASH_NVM_TYPE
#endif
#if defined(_18FXXQ83_FAMILY_)
#define NV_ADDRESS  0x200
#define NV_NVM_TYPE EEPROM_NVM_TYPE
#endif
#define NV_CACHE

//
// CAN service
//
#define CANID_ADDRESS  0x3FE    // 1 byte
#define CANID_NVM_TYPE EEPROM_NVM_TYPE
#define CAN_INTERRUPT_PRIORITY 0    // all low priority
#define CAN_CLOCK_MHz   64
// Number of buffers
#if defined(_18F66K80_FAMILY_)
#define CAN_NUM_RXBUFFERS   32  
#define CAN_NUM_TXBUFFERS   8
#endif
#if defined(_18FXXQ83_FAMILY_)
#define CAN_NUM_RXBUFFERS   8
#endif
//
// BOOT service
//
#define BOOT_FLAG_ADDRESS   0x3FF
#define BOOT_FLAG_NVM_TYPE EEPROM_NVM_TYPE
#define BOOTLOADER_PRESENT

//
// EVENT TEACH SERVICE
//
//
#define EVENT_TABLE_WIDTH   10  // This the the width of the table - not the 
                                // number of EVs per event as multiple rows in
                                // the table can be used to store an event
#define NUM_EVENTS          255  // The number of rows in the event table. The
                                // actual number of events may be less than this
                                // if any events use more the 1 row.
#define EVperEVT            20  // number of EVs per event
#define EV_FILL             0
#define NO_ACTION           0

#if defined(_18FXXQ83_FAMILY_)
#define EVENT_TABLE_ADDRESS               0x1E000
#endif
#if defined(_18F66K80_FAMILY_)
#ifdef __18F25K80
#define EVENT_TABLE_ADDRESS               0x6F80      //(AT_NV - sizeof(EventTable)*NUM_EVENTS) Size=256 * 22 = 5632(0x1600) bytes
#endif
#ifdef __18F26K80
    #ifdef CANXIO
        #define EVENT_TABLE_ADDRESS       0xE940      //(AT_NV - sizeof(EventTable)*NUM_EVENTS) Size=256 * 22 = 4096(0x1000) bytes   FF40 - 1600 = E940 (note eventable size currently 16 but allow for max of 22)
    #else
        #define EVENT_TABLE_ADDRESS       0xEF80      //(AT_NV - sizeof(EventTable)*NUM_EVENTS) Size=256 * 22 = 5632(0x1600) bytes
    #endif    
#endif
#endif

#define EVENT_TABLE_NVM_TYPE    FLASH_NVM_TYPE
#define EVENT_HASH_TABLE
// These are chosen so we don't use too much memory 32*20 = 640 bytes.
// Used to size the hash table used to lookup events in the events2actions table.
#define EVENT_HASH_LENGTH  32
#define EVENT_CHAIN_LENGTH    20
#ifdef CANXIO
#define MAX_HAPPENING       103
#else
#define MAX_HAPPENING       71
#endif
#define CONSUMED_EVENTS

//
// EVENT PRODUCER SERVICE
#define PRODUCED_EVENTS
#define HAPPENING_SIZE  1   // Happenings are 1 byte
//
// EVENT CONSUMER2Q SERVICE
#define HANDLE_DATA_EVENTS
#define ACTION_NORMAL_QUEUE_SIZE 	64	// MUST BE A POWER OF TWO
                                // The size needs to be big enough to store all the pending actions 
                                // Need to allow +1 to separate the ends of the cyclic buffer so need to 
                                // move the next power of two since cyclic wrapping is done with a bitmask.
                                // 64 is safer as we have wait actions
#define ACTION_EXPEDITED_QUEUE_SIZE 8   // MUST BE A POWER OF TWO
#define ACTION_SIZE 1       // Actions are 1 byte

//
// MNS service
//
// Processor clock speed
#define clkMHz      64
// 2 bytes for the module's node number
#define NN_ADDRESS  0x3FC 
#define NN_NVM_TYPE EEPROM_NVM_TYPE
// 1 byte for the mode
#define MODE_ADDRESS    0x3FB
#define MODE_NVM_TYPE   EEPROM_NVM_TYPE
// 1 byte for the mode flags
#define MODE_FLAGS_ADDRESS    0x3FA
#define MODE_FLAGS_NVM_TYPE   EEPROM_NVM_TYPE
// Parameters
#define PARAM_MANU              MANU_MERG
#define PARAM_MODULE_ID         MTYP_CANMIO
#define PARAM_MAJOR_VERSION     3
#define PARAM_MINOR_VERSION     'e'
#define PARAM_BUILD_VERSION     3
#define PARAM_NUM_NV            NV_NUM
#define PARAM_NUM_EVENTS        NUM_EVENTS
#define PARAM_NUM_EV_EVENT      20
// Module name - must be 7 characters
#define NAME    "MIO    "
// LEDs and PB                                 // GREEN is 0 YELLOW is 1
#if defined(_18F66K80_FAMILY_)
#define APP_setPortDirections(){ANCON0=ANCON1=0; TRISBbits.TRISB6=TRISBbits.TRISB7=0,TRISAbits.TRISA2=1;}
#endif
#if defined(_18FXXQ83_FAMILY_)
#define APP_setPortDirections(){ANSELA=ANSELB=0; TRISBbits.TRISB6=TRISBbits.TRISB7=0,TRISAbits.TRISA2=1;}
#endif
#define APP_writeLED1(state)   (LATBbits.LATB7=state)   // GREEN true is on
#define APP_writeLED2(state)   (LATBbits.LATB6=state)   // YELLOW true is on 
#define APP_pbPressed()        (!(PORTAbits.RA2))       // where the push button is connected. True when pressed

/******************************************************************************
* Now the Universal options
*******************************************************************************/
/*********************** MODULE OPTIONS **************************/
    
// Whether to support AREQ and ASRQ commands
#define AREQ_SUPPORT

// Whether to enable servos
#define SERVO

// Whether to enable BOUNCE
#define BOUNCE

// Whether to enable MULTI and ANALOGUE inputs
#define MULTI
#if defined(_18FXXQ83_FAMILY_)
#define ANALOGUE
#endif


// enable this for additional validation checks
//#define SAFETY

#endif