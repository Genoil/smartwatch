/*
 * clock.c
 *
 * Created: 8-4-2014 15:10:18
 *  Author: Jan Willem Penterman
 */ 

#include <sleep.h>
#include <watch.h>
	
void set_time(uint8_t * buf) {
	struct rtc_calendar_time time;
	
	//if(cmd_buffer_size == 15) {
		time.year   = (buf[1]-48)*1000 + (buf[2]-48) * 100 + (buf[3]-48) * 10 + (buf[4]-48);
		time.month  = (buf[5]-48) * 10 + (buf[6]-48);
		time.day    = (buf[7]-48) * 10 + (buf[8]-48);
		time.hour	= (buf[9]-48) * 10 + (buf[10]-48);
		time.minute = (buf[11]-48) * 10 + (buf[12]-48);
		time.second	= (buf[13]-48) * 10 + (buf[14]-48);
		
		rtc_calendar_set_time(&rtc_instance, &time);
	//}
}

void set_alarm(uint8_t * buf) {
	struct rtc_calendar_alarm_time alarm;
	
	//if(cmd_buffer_size == 7) {
		alarm.time.hour	  = (buf[1]-48) * 10 + (buf[2]-48);
		alarm.time.minute = (buf[3]-48) * 10 + (buf[4]-48);
		alarm.time.second  = (buf[5]-48) * 10 + (buf[6]-48);
		alarm.mask = RTC_CALENDAR_ALARM_MASK_HOUR;
		rtc_calendar_set_alarm(&rtc_instance, &alarm, RTC_CALENDAR_ALARM_0);
		rtc_calendar_enable_callback(&rtc_instance, RTC_CALENDAR_CALLBACK_ALARM_0);
		
		char alarmdisplay[10];
		sprintf(alarmdisplay, "A: %02d:%02d:%02d", alarm.time.hour, alarm.time.minute, alarm.time.second);
		gfx_mono_draw_string(alarmdisplay,0, 16, &sysfont);
	//}
}

void configure_rtc_calendar(void)
{
	/* Initialize RTC in calendar mode. */
	struct rtc_calendar_config config_rtc_calendar;
	rtc_calendar_get_config_defaults(&config_rtc_calendar);
	config_rtc_calendar.clock_24h = true;
	rtc_calendar_init(&rtc_instance, RTC, &config_rtc_calendar);
	rtc_calendar_enable(&rtc_instance);
	
	/* Configure and enable callback */
	rtc_calendar_register_callback(&rtc_instance, rtc_match_callback, RTC_CALENDAR_CALLBACK_ALARM_0);
}

void rtc_match_callback(void)
{
	check_awake();
	gfx_mono_draw_string("ALARM!     ",0, 16, &sysfont);
}

void tc_callback_second(struct tc_module *const module_inst)
{
	static int compare_value = 0;
	
	struct rtc_calendar_time time;
	rtc_calendar_get_time(&rtc_instance, &time);
	
	char display_time[8];
	sprintf(display_time, "%02d:%02d:%02d", time.hour, time.minute, time.second);
	gfx_mono_draw_string(display_time,0, 0, &sysfont);
	
	compare_value += 0x8000;
	tc_set_compare_value(&tc_instance, TC_COMPARE_CAPTURE_CHANNEL_0, compare_value);
	
	check_asleep();
}
void configure_tc(void)
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
	config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_FREQ;
	config_tc.counter_16_bit.compare_capture_channel[0] = 0xFFFF;
	
	config_tc.clock_source = GCLK_GENERATOR_1;
	tc_init(&tc_instance, EXT1_PWM_MODULE, &config_tc);
	tc_enable(&tc_instance);
}

void configure_tc_callbacks(void)
{
	tc_register_callback(&tc_instance, tc_callback_second, TC_CALLBACK_CC_CHANNEL0);
	tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
}