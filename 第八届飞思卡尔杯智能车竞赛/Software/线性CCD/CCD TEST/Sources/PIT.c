#include "derivative.h"
#include "PIT.h"

void PIT_Init(void) 
{
    PITCE_PCE0 = 1;             //Enable Timer Channel0
    PITMUX_PMUX0 = 0;           //16-bit timer counts with micro time base 0
//time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS
//1ms
    PITMTLD0 = 199;             //PIT Micro Timer Load Register
    PITLD0 = 199;               //PIT Load Register
    PITINTE_PINTE0 = 1;         //Enable Timer Channel0 Interrupt
    PITCFLMT_PITE = 1;          //Enable Timer
}