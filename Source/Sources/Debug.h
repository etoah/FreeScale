#include "derivative.h"      /* derivative-specific definitions */


#define SW1 PORTB_PB4
#define SW2 PORTB_PB6
#define SW3 PORTB_PB5
#define SW4 PORTB_PB7

#define CAR_STAND_ANGLE_MIN  -4
#define CAR_STAND_ANGLE_MAX  4
#define CAR_STAND_ANGLE_MIN  -4
#define CAR_STAND_ANGLE_MAX  4
#define OUTROADMIN   10

 extern int NowSpeed,NowAngel,D_Gyro,L[6], g_nCarSpeedSet;

void CarTumble(void);
void BeginBalance(void);
void SwitchDebug(void);
/*************************************************************/
/*                  ³öÈüµÀ                                                                          */
/*************************************************************/
void OutRoadTest(void);
