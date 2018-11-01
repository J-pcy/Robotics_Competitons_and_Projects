#ifndef   EVENTS_H
#define   EVENTS_H

extern unsigned char TimerFlag20ms;

#pragma CODE_SEG __NEAR_SEG NON_BANKED

__interrupt void PITCh0IntISR(void);

#pragma CODE_SEG DEFAULT


#endif