/*
 * sleep.h
 *
 * Created: 8-4-2014 14:54:48
 *  Author: Jan Willem Penterman
 */ 


#ifndef SLEEP_H_
#define SLEEP_H_

#define SLEEP_TIMEOUT	10

void wake_up(void);
void go_to_sleep(void);
void check_awake(void);
void check_asleep(void);

#endif /* SLEEP_H_ */