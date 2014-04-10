/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// LED control

#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include "common.h"
#include "devices/led.h"
#include "millis/millis.h"
#include "watchconfig.h"
#include "pwrmgr.h"

#define RED_OCR		OCR0A
#define RED_COM		COM0A1
#define RED_PIN		PD6
#define GREEN_OCR	OCR0B
#define GREEN_COM	COM0B1
#define GREEN_PIN	PD5

typedef struct {
	byte flashLen;
	millis8_t startTime;
}led_s;

static led_s ledRed;
static led_s ledGreen;

static void flash(led_s*, byte, byte, volatile byte*, byte, byte);
static bool update(led_s*, byte, byte);

void led_init()
{
	TCCR0A = _BV(WGM01)|_BV(WGM00);
	TCCR0B = _BV(CS01)|_BV(CS00);
	power_timer0_disable();

	pinMode(D5, OUTPUT);
	pinMode(D6, OUTPUT);
	pinWrite(D5, LOW);
	pinWrite(D6, LOW);
}

static void flash(led_s* led, byte len, byte brightness, volatile byte* ocr, byte com, byte pin)
{
	led->flashLen = len;
	led->startTime = millis();

	if(brightness == 255 || brightness == 0)
	{
		TCCR0A &= ~com;
		brightness == 255 ? (PORTD |= pin) : (PORTD &= ~pin);
	}
	else
	{
		power_timer0_enable();
		TCCR0A |= com;
		*ocr = brightness;
	}
}

void led_flash(byte led, byte len, byte brightness)
{
	switch(led)
	{
		case LED_RED:
			flash(&ledRed, len, brightness, &RED_OCR, _BV(RED_COM), _BV(RED_PIN));
			break;
		case LED_GREEN:
			flash(&ledGreen, len, brightness, &GREEN_OCR, _BV(GREEN_COM), _BV(GREEN_PIN));
			break;
		default:
			break;
	}

	pwrmgr_setState(PWR_ACTIVE_LED, PWR_STATE_IDLE);
}
/*
bool led_flashing()
{
	return ledRed.flashLen || ledGreen.flashLen;
}
*/
static bool update(led_s* led, byte com, byte pin)
{
	if(led->flashLen && (millis8_t)(millis() - led->startTime) >= led->flashLen)
	{
		PORTD &= pin;
		TCCR0A &= com;
		led->flashLen = 0;
	}

	return led->flashLen;
}

void led_update()
{
	bool red = update(&ledRed, (byte)~_BV(RED_COM), ~_BV(RED_PIN));
	bool green = update(&ledGreen, ~_BV(GREEN_COM), ~_BV(GREEN_PIN));

	if(!red && !green)
	{
		power_timer0_disable();
		pwrmgr_setState(PWR_ACTIVE_LED, PWR_STATE_NONE);
	}
}