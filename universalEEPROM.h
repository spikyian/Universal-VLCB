
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
 * File:   universalEEPROM.h
 * Author: Ian
 *
 * Created on 17 April 2017, 16:56
 */

#ifndef universalEEPROM_H
#define	universalEEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/*
 * If the value in EEPROM doesn't match this then either initialise or upgrade.
 */
#define EEPROM_VERSION  0x01
#define EE_APPLICATION  (EE_TOP-8)
    /*
     * Any additional EEPROM storage requirements above that required by the application
     * is defined here. I.e. module specific storage.
     * Module specific stuff starts at EE_TOP-8
     */
#define EE_SPARE            EE_APPLICATION    // spare location
    /**
     * Record the current output state for all the IO.
     */
#define EE_OP_STATE         ((eeprom_address_t)(EE_APPLICATION)-25)    // Space to store current state of up to 16 outputs (24 for CANXIO)
                                                 // You'll need to do ee_read(EE_OP_STATE + io)
    

#ifdef	__cplusplus
}
#endif

#endif	/* universalEEPROM_H */

