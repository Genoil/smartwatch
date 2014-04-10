/*
 * sleep.c
 *
 * Created: 8-4-2014 14:54:14
 *  Author: Jan Willem Penterman
 */ 

#include <asf.h>
#include <sleep.h>

uint8_t awake_time;
bool	awake = false;

void wake_up() {
	if(!awake) {
		ssd1306_sleep_disable();
		awake = true;
	}
	awake_time = 0;
}

void go_to_sleep() {
	ssd1306_sleep_enable();
	system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
	awake = false;
}

void check_awake() {
	if(!awake) {
		wake_up();
	}
}

void check_asleep() {
	awake_time++;
	if(awake_time >= SLEEP_TIMEOUT) {
		go_to_sleep();
	}
}