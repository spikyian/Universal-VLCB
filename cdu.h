/* 
 * File:   cdu.h
 * Author: Ian
 *
 * Created on 11 June 2024, 17:55
 */

#ifndef CDU_H
#define	CDU_H

#ifdef	__cplusplus
extern "C" {
#endif

extern void initCdus(void);
extern void processCduPulses(void);
extern void processCduRecharges(void);
extern void startCduOutput(uint8_t io, uint8_t state);
extern void setCduState(uint8_t io, uint8_t act);
extern void setCduOutput(uint8_t io, uint8_t pos);
extern void finaliseCduOutput(uint8_t io);

#ifdef	__cplusplus
}
#endif

#endif	/* CDU_H */