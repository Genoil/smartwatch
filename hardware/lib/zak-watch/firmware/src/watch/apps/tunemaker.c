/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */
/*
#include <avr/pgmspace.h>
#include <string.h>
#include "typedefs.h"
#include "apps/tunemaker.h"
#include "devices/buttons.h"
#include "display.h"
#include "resources.h"
#include "menu.h"
#include "watchface.h"

static bool down(void);
static bool up(void);
static bool select(void);
static byte draw(void);

void tunemakerOpen()
{
	menu_close();

	display_setDrawFunc(draw);
	buttons_setFunc(BTN_SELECT,	select);
	buttons_setFunc(BTN_DOWN,	down);
	buttons_setFunc(BTN_UP,		up);
}

static bool down()
{
	return true;
}

static bool up()
{
	return true;
}

static bool select()
{
	watchface_loadFace();
	return true;
}

static byte draw()
{
	return DISPLAY_DONE;
}
*/