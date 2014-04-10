/*
 * Project: Digital Wristwatch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef OLED_H_
#define OLED_H_

#include "typedefs.h"

#define OLED_PWR_OFF		false
#define OLED_PWR_ON			true

#define FRAME_WIDTH			128
#define FRAME_HEIGHT		64
#define FRAME_BUFFER_SIZE	((FRAME_WIDTH * FRAME_HEIGHT) / 8)

extern byte oledBuffer[];

void oled_init(void);
void oled_flush(void);
void oled_power(bool);
void oled_setBrightness(byte);
void oled_setInvert(bool);
void oled_set180(bool);

#endif /* OLED_H_ */