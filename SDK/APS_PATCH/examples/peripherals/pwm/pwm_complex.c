// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include "sys_init.h"
#include "sys_init_patch.h"
#include "hal_system.h"
#include "hal_pin.h"
#include "hal_pin_def.h"
#include "hal_pin_config_project.h"
#include "hal_dbg_uart.h"
#include "hal_vic.h"
#include "boot_sequence.h"
#include "Hal_pinmux_pwm.h"
#include "pwm_complex.h"
#include "cmsis_os.h"
#include "hal_tmr.h"
#include "hal_pwm.h"

uint8_t gduty = 0,gduty_tgt = 0;
uint8_t gduty_flag = 0;

void pwm_complex_duty_set(uint8_t d);

/*************************************************************************
* FUNCTION:
*   pwm_timer_start
*
* DESCRIPTION:
*   Start the timer with index 1.
*   Clear duty flag to indicate that starts adjusting duty ratio.
*
* PARAMETERS
*   1. time     : [In] the timer value in us( 1/1000000 S).
*
* RETURNS
*   none
*
*************************************************************************/
void pwm_timer_start(uint32_t time)
{ 
    Hal_Tmr_Start(1, time);
    gduty_flag = 0;    
}

/*************************************************************************
* FUNCTION:
*   pwm_timer_stop
*
* DESCRIPTION:
*   Stop the timer with index 1.
*   Set duty flag to indicate that target duty ratio is reached.
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void pwm_timer_stop(void)
{
    Hal_Tmr_Stop(1);
      gduty_flag = 1;
}

/*************************************************************************
* FUNCTION:
*   pwm_complex_get_duty_target
*
* DESCRIPTION:
*   Get the target duty value in complex mode.
*   duty ratio = gduty_tgt / PWM_COMPLEX_PERIOD_DEF.
*
* PARAMETERS
*   none
*
* RETURNS
*   gduty_tgt -- globe variable value.
*
*************************************************************************/
uint8_t pwm_complex_get_duty_target(void)
{
      return gduty_tgt;
}

/*************************************************************************
* FUNCTION:
*   pwm_complex_get_status
*
* DESCRIPTION:
*   Get the duty flag which is used to indicate 
*   whether the duty target is reached or not in complex mode.
*
* PARAMETERS
*   none
*
* RETURNS
*   gduty_flag -- globe variable value.
*
*************************************************************************/
uint8_t pwm_complex_get_status(void)
{
      return gduty_flag;
}

/*************************************************************************
* FUNCTION:
*   timer_periodic_callback
*
* DESCRIPTION:
*   Callback function of timer 1. 
*   adjust the duty step by step( 1 per 6ms by default) until it reaches the target duty.
*
* PARAMETERS
*   ulTimerIdx, not use it here.
*
* RETURNS
*   none
*
*************************************************************************/
void timer_periodic_callback(uint32_t ulTimerIdx)
{
      if(gduty != gduty_tgt)
      {
            if(gduty > gduty_tgt)
                {
                    gduty--;
                }
                else if(gduty < gduty_tgt)
                {
                    gduty++;
                }
                
            
        pwm_complex_duty_set(gduty);    
        }
        else
        {
            pwm_timer_stop();    
        }
}

/*************************************************************************
* FUNCTION:
*   pwm_timer_init
*
* DESCRIPTION:
*   Initialize timer with index 1 for pwm complex mode testing. 
*
* PARAMETERS
*   ulTimerIdx, not use it here.
*
* RETURNS
*   none
*
*************************************************************************/
void pwm_timer_init(void)
{
    Hal_Tmr_Init(1);    
    Hal_Tmr_CallBackFuncSet(1, timer_periodic_callback);    
}

/*************************************************************************
* FUNCTION:
*   pwm_complex_init
*
* DESCRIPTION:
*   Initialize function for pwm complex mode testing. 
*
* PARAMETERS
*   ulTimerIdx, not use it here.
*
* RETURNS
*   none
*
*************************************************************************/
void pwm_complex_init(void)
{
    pwm_timer_init();    
    gduty_flag = 0;
      pwm_complex_set_duty_target(PWM_COMPLEX_DUTY_DEF);
}

/*************************************************************************
* FUNCTION:
*   pwm_complex_duty_set
*
* DESCRIPTION:
*   Call HAL API function to set duty in complex mode. 
*
* PARAMETERS
*   d  -- the duty value.
*
* RETURNS
*   none
*
*************************************************************************/
void pwm_complex_duty_set(uint8_t d)
{
    S_Hal_Pwm_Config_t tpwm_config;
      uint32_t period = PWM_COMPLEX_PERIOD_DEF;
    
      tpwm_config.ulPeriod = period;
      tpwm_config.ulDutyBright = d;
      tpwm_config.ulDutyDull = d;     
         tpwm_config.ulRampUp = d; 
        tpwm_config.ulRampDown = 0;
        tpwm_config.ulHoldBright = 1; 
        tpwm_config.ulHoldDull = 1;    
    
    // Fixed mapping to OPL1000_IO20_PIN <--> HAL_PWM_IDX_3.
      Hal_Pwm_ComplexConfigSet(PWM_COMPLEX_TEST_PIN,tpwm_config);
}

/*************************************************************************
* FUNCTION:
*   pwm_complex_set_duty_target
*
* DESCRIPTION:
*   Set target duty value. 
*   and restart step timer.
*
* PARAMETERS
*   duty_target  -- target duty value.
*
* RETURNS
*   none
*
*************************************************************************/
void pwm_complex_set_duty_target(uint8_t duty_target)
{
      if(duty_target > PWM_COMPLEX_PERIOD_DEF)
        {
              printf("[WARN]pwm: invalid duty value, set it to default. \r\n");
              gduty_tgt = PWM_COMPLEX_DUTY_DEF;
        }
        else
        {
          gduty_tgt = duty_target;
        }

      pwm_timer_stop();
      pwm_timer_start(PWM_COMPLEX_STEP_TIME);    
}
