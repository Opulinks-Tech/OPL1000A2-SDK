#ifndef __PWM_COMPLEX_H__
#define __PWM_COMPLEX_H__

#include "OPL1000_pin_mux_define.h"

#ifdef  __cplusplus
extern "C" {
#endif
    
#define W_DEBUG 0
//use pin IO20 for complex mode testing.
#define PWM_COMPLEX_TEST_PIN HAL_PWM_IDX_3
//step timer time is 6ms.
#define PWM_COMPLEX_STEP_TIME    6000
//duty ratio = PWM_COMPLEX_DUTY_DEF / PWM_COMPLEX_PERIOD_DEF by default.
#define PWM_COMPLEX_PERIOD_DEF   255
#define PWM_COMPLEX_DUTY_DEF     204    

uint8_t pwm_complex_get_duty_target(void);
uint8_t pwm_complex_get_status(void);
void pwm_complex_init(void);
void pwm_complex_set_duty_target(uint8_t duty_target);

#ifdef __cplusplus
}
#endif
#endif  // __PWM_COMPLEX_H__
