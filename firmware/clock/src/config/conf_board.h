/**
 * \file
 *
 * \brief SAMD20 Xplained PRO board configuration.
 *
 * Copyright (c) 2012-2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

/* Buttons on OLED1 Xplained Pro */

#define BUTTON_1					   PIN_PA28
#define BUTTON_1_ACTIVE                false
#define BUTTON_1_INACTIVE              !BUTTON_1_ACTIVE
#define BUTTON_1_EIC_PIN               PIN_PA28A_EIC_EXTINT8
#define BUTTON_1_EIC_MUX               MUX_PA28A_EIC_EXTINT8
#define BUTTON_1_EIC_PINMUX			   PINMUX_PA28A_EIC_EXTINT8
#define BUTTON_1_EIC_LINE			   8

#define BUTTON_2					   PIN_PA02
#define BUTTON_2_ACTIVE                false
#define BUTTON_2_INACTIVE              !BUTTON_2_ACTIVE
#define BUTTON_2_EIC_PIN               PIN_PA02A_EIC_EXTINT2
#define BUTTON_2_EIC_MUX               MUX_PA02A_EIC_EXTINT2
#define BUTTON_2_EIC_PINMUX			   PINMUX_PA02A_EIC_EXTINT2
#define BUTTON_2_EIC_LINE			   2

#define BUTTON_3					   PIN_PA03
#define BUTTON_3_ACTIVE                false
#define BUTTON_3_INACTIVE              !BUTTON_3_ACTIVE
#define BUTTON_3_EIC_PIN               PIN_PA03A_EIC_EXTINT3
#define BUTTON_3_EIC_MUX               MUX_PA03A_EIC_EXTINT3
#define BUTTON_3_EIC_PINMUX			   PINMUX_PA03A_EIC_EXTINT3
#define BUTTON_3_EIC_LINE			   3

/* Height and width of LCD */
#  define LCD_WIDTH_PIXELS  128
#  define LCD_HEIGHT_PIXELS  32

#endif /* CONF_BOARD_H_INCLUDED */
