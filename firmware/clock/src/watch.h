/*
 * clock.h
 *
 * Created: 8-4-2014 15:10:03
 *  Author: Jan Willem Penterman
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#include <asf.h>

struct rtc_module	rtc_instance;
struct tc_module	tc_instance;

void configure_rtc_calendar(void);
void rtc_match_callback(void);

void configure_tc(void);
void configure_tc_callbacks(void);
void tc_callback_second(struct tc_module *const module_inst);

void set_time(uint8_t * buf);
void set_alarm(uint8_t * buf);

#endif /* CLOCK_H_ */