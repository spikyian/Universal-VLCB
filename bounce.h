/* 
 * File:   bounce.h
 * Author: Ian
 *
 * Created on 22 June 2017, 21:49
 */

#ifndef BOUNCE_H
#define	BOUNCE_H

#ifdef	__cplusplus
extern "C" {
#endif

extern void initBounce(uint8_t io);
extern Boolean bounceDown(uint8_t io);
extern Boolean bounceUp(uint8_t io);
/**
 * Indicates how fast the virtual signal operator pulls the signal on.
 */
#define PULL_SPEED 12

#ifdef	__cplusplus
}
#endif

#endif	/* BOUNCE_H */

