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
 * File:   outputs.h
 * Author: Ian
 *
 * Created on 17 December 2020, 19:36
 */

#ifndef OUTPUTS_H
#define	OUTPUTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
extern Boolean needsStarting(uint8_t io, uint8_t act, uint8_t type);
extern void startOutput(uint8_t io, uint8_t act, uint8_t type);
extern void setOutputPosition(uint8_t io, uint8_t pos, uint8_t type);
extern void setOutputState(uint8_t io, uint8_t action, uint8_t type);
extern Boolean completed(uint8_t io, Action action, uint8_t type);
extern void finaliseOutput(uint8_t io, uint8_t type);

#ifdef	__cplusplus
}
#endif

#endif	/* OUTPUTS_H */