/*
 * buttons.c
 *
 * Created: 8-4-2014 16:52:55
 *  Author: Jan Willem Penterman
 */ 
#include <asf.h>
#include <buttons.h>
#include <sleep.h>

void configure_buttons(void)
{
	uint32_t			button_eic_pin[4]  = { BUTTON_0_EIC_PIN, BUTTON_1_EIC_PIN, BUTTON_2_EIC_PIN, BUTTON_3_EIC_PIN };
	uint32_t			button_eic_mux[4]  = { BUTTON_0_EIC_MUX, BUTTON_1_EIC_MUX, BUTTON_2_EIC_MUX, BUTTON_3_EIC_MUX };	
	uint32_t			button_eic_li0ne[4] = { BUTTON_0_EIC_LINE, BUTTON_1_EIC_LINE, BUTTON_2_EIC_LINE, BUTTON_3_EIC_LINE };
	extint_callback_t 	button_callback[4] = { button_0_pressed, button_1_pressed, button_2_pressed, button_3_pressed};
	
	struct extint_chan_conf config_extint_chan[4];
	
	for(int i = 0; i < 4; i++) {
		extint_chan_get_config_defaults(config_extint_chan + i);
		config_extint_chan[i].gpio_pin = button_eic_pin[i];
		config_extint_chan[i].gpio_pin_mux = button_eic_mux[i];
		config_extint_chan[i].gpio_pin_pull = EXTINT_PULL_UP;
		config_extint_chan[i].detection_criteria = EXTINT_DETECT_RISING;
		extint_chan_set_config(button_eic_line[i], config_extint_chan + i);
		
		extint_register_callback(button_callback[i], button_eic_line[i], EXTINT_CALLBACK_TYPE_DETECT);
		extint_chan_enable_callback(button_eic_line[i], EXTINT_CALLBACK_TYPE_DETECT);
	}
}

void button_0_pressed() {
	wake_up();
	gfx_mono_draw_string("button 0",0, 16, &sysfont);
}

void button_1_pressed() {
	wake_up();
	gfx_mono_draw_string("button 1",0, 16, &sysfont);
}

void button_2_pressed() {
	wake_up();
	gfx_mono_draw_string("button 2",0, 16, &sysfont);
}

void button_3_pressed() {
	wake_up();
	gfx_mono_draw_string("button 3",0, 16, &sysfont);
}