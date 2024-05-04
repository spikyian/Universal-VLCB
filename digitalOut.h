/* 
 * File:   digitalOut.h
 * Author: Ian
 *
 * Created on 05 August 2017, 16:55
 */

#ifndef DIGITALOUT_H
#define	DIGITALOUT_H

#ifdef	__cplusplus
extern "C" {
#endif

    
extern void initOutputs(void);
extern void processOutputs(void);
extern void startDigitalOutput(uint8_t io, uint8_t state);
extern void setDigitalOutput(uint8_t io, uint8_t state);
extern void setOutputPin(uint8_t io, Boolean state);

// Pulsed output special values
#define NEEDS_STARTING  0
#define COMPLETED       1

#ifdef	__cplusplus
}
#endif

#endif	/* DIGITALOUT_H */

