/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "games/game2.h"
#include "display.h"
#include "draw.h"
#include "devices/oled.h"
#include "devices/buttons.h"
#include "watchface.h"
#include "devices/buzzer.h"
#include "devices/led.h"
#include "menu.h"
#include "resources.h"
#include "animation.h"

#define UPT_MOVE_NONE	0
#define UPT_MOVE_UP		1
#define UPT_MOVE_DOWN	2

#define CAR_COUNT	3
#define CAR_WIDTH	12
#define CAR_LENGTH	15
#define ROAD_SPEED	6

typedef struct {
//	bool ok;
	byte x;
	byte y;
	byte speed;
}s_otherCars;

typedef struct {
	bool hit;
	byte lane;
	byte y;
}s_myCar;

static const byte carImg[] PROGMEM ={
	0x40,0xF8,0xEC,0x2C,0x2C,0x38,0xF0,0x10,0xD0,0x30,0xE8,0x4C,0x4C,0x9C,0xF0,
	0x02,0x1F,0x37,0x34,0x34,0x1C,0x0F,0x08,0x0B,0x0C,0x17,0x32,0x32,0x39,0x0F,
};

static const byte roadMarking[] PROGMEM ={
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
};

static bool select(void);
static bool down(void);
static bool up(void);
static display_t draw(void);

static const uint eepHighscore EEMEM;

static uint highscore;
static uint score;
static byte uptMove;
static byte lives;
static s_otherCars cars[CAR_COUNT];

void game2_start()
{
	menu_close();

	srand(millis());

	display_setDrawFunc(draw);
	buttons_setFunc(BTN_SELECT,	select);
	buttons_setFunc(BTN_DOWN,	down);
	buttons_setFunc(BTN_UP,		up);

	LOOP(CAR_COUNT, i)
	{
		cars[i].y = i * 16;
		cars[i].speed = i + 1;
		cars[i].x = FRAME_WIDTH;
	}	

	eeprom_read_block(&highscore, &eepHighscore, sizeof(uint));
	score = 0;
	uptMove = UPT_MOVE_NONE;
	lives = 3;
}

static bool select()
{
	if(lives == 255)
		game2_start();
	else
		animation_start(watchface_loadFace, ANIM_MOVE_OFF);
	return true;
}

static bool down()
{
	uptMove = UPT_MOVE_DOWN;
	return true;
}

static bool up()
{
	uptMove = UPT_MOVE_UP;
	return true;
}
/*
static void music(void)
{
	static byte idx;
	static const uint tune[] PROGMEM = {
		TONE_2KHZ,
		TONE_2_5KHZ,
		TONE_3KHZ,
		TONE_3KHZ,
		TONE_3KHZ,
		TONE_2_5KHZ,
		TONE_2_5KHZ,
		TONE_2_5KHZ,
		TONE_3KHZ,
		TONE_4KHZ,
		TONE_3KHZ,
		TONE_4KHZ
		};

	if(!buzzer_buzzing())
	{
		buzzer_buzz(200, pgm_read_word(&tune[idx++]), VOL_UI);
		if(idx > (sizeof(tune) / sizeof(uint)) - 1)
			idx = 0;
	}
}
*/

static display_t draw()
{
	static s_myCar myCar;
	static millis_t hitTime;
	static bool newHighscore;

//	music();

	// Change lane
	if(uptMove == UPT_MOVE_UP && myCar.lane < 3)
		myCar.lane++;
	else if(uptMove == UPT_MOVE_DOWN&& myCar.lane > 0)
		myCar.lane--;
	uptMove = UPT_MOVE_NONE;

	// Move to new lane
	byte y = myCar.lane * 16;
	if(myCar.y > y)
		myCar.y -= 2;
	else if(myCar.y < y)
		myCar.y += 2;

	if(lives != 255)
	{
		// Move cars
		LOOP(CAR_COUNT, i)
		{
			// Move car
			cars[i].x -= cars[i].speed;

			// Gone off screen
			if(cars[i].x > 200 && cars[i].x < 255 - 16)
			{
				cars[i].x = FRAME_WIDTH;
				cars[i].y = (rand() % 4) * 16;
				score++;
			}
		}

		// Stop cars from going through each other
		// Its a little bit glitchy
		LOOP(CAR_COUNT, i)
		{
			LOOP(CAR_COUNT, c)
			{
				if(i != c && cars[i].y == cars[c].y && cars[i].x > cars[c].x && cars[i].x < cars[c].x + CAR_LENGTH)
					cars[i].x = cars[c].x + CAR_LENGTH + 1;
			}
		}

		// Collision
		if(!myCar.hit)
		{
			LOOP(CAR_COUNT, i)
			{
				if(cars[i].x < CAR_LENGTH)
				{
					byte carY = cars[i].y + 2;
					byte myCarY = myCar.y + 2;
					if((carY >= myCarY && carY <= myCarY + CAR_WIDTH) || (carY + CAR_WIDTH >= myCarY && carY + CAR_WIDTH <= myCarY + CAR_WIDTH))
					{
						myCar.hit = true;
						hitTime = millis();
						lives--;
						if(lives == 255)
						{
							// Check for new highscore
							if(score > highscore)
							{
								newHighscore = true;
								highscore = score;
								eeprom_update_block(&highscore, (uint*)&eepHighscore, sizeof(uint));
							}
							else
								newHighscore = false;

							led_flash(LED_RED, 250, 255);
							buzzer_buzz(250, TONE_2KHZ, VOL_UI);
						}	
						else
						{
							led_flash(LED_RED, 30, 255);
							buzzer_buzz(100, TONE_2KHZ, VOL_UI);
						}
					}
				}
			}
		}
	}

	millis_t now = millis();

	if(myCar.hit && now - hitTime >= 1000)
		myCar.hit = false;

	// Quake
	static byte quakeY;
	if(myCar.hit && now - hitTime < 350)
	{
		if(quakeY == 2)
			quakeY = -2;
		else
			quakeY = 2;
	}
	else
		quakeY = 0;
	
	// Draw my car
	s_image img = {0, myCar.y + quakeY, carImg, 15, 16, WHITE, NOINVERT, 0};
	if(!myCar.hit || (myCar.hit && (now & 64)))
		draw_bitmap_s2(&img);

	char buff[6];
	if(lives != 255)
	{
		// Draw other cars
		LOOPR(CAR_COUNT, i)
		{
			img.x = cars[i].x;
			img.y = cars[i].y + quakeY;
			draw_bitmap_s2(&img);
		}

		// Draw road markings
		img.bitmap = roadMarking;
		img.width = 8;
		img.height = 8;

		static byte dotX[3] = {0, 45, 90};
		LOOP(3, i)
		{
			dotX[i] -= ROAD_SPEED;

			if(dotX[i] >= FRAME_WIDTH && dotX[i] < 255 - 8)
				dotX[i] = FRAME_WIDTH;

			img.x = dotX[i];
			LOOP(3, x)
			{
				img.y = (x * 16) + quakeY + 16;
				draw_bitmap_s2(&img);
			}
		}

		// Draw score
		sprintf_P(buff, PSTR("%u"), score);
		draw_string(buff, false, FRAME_WIDTH - (7*5), 1);

		// Draw lives
		img.bitmap = livesImg;
		img.width = 7;
		img.y = 1;
		LOOP(lives, i)
		{
			img.x = 32 + (8*i);
			draw_bitmap_s2(&img);
		}
	}
	else
	{
		// Draw end game stuff

		draw_string("GAMEOVER!", false, 20, 0);
		draw_string("Score:", false, 20, 16);
		draw_string("Highscore:", false, 20, 32);

		if(newHighscore)
			draw_string("!NEW HIGHSCORE!", false, 20, 48);

		sprintf_P(buff, PSTR("%u"), score);
		draw_string(buff, false, 96, 16);

		sprintf_P(buff, PSTR("%u"), highscore);
		draw_string(buff, false, 96, 32);
	}

	return DISPLAY_BUSY;
}
