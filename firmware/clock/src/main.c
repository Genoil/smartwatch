/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <cmd.h>
#include <sleep.h>
#include <watch.h>
#include <buttons.h>

#define MAX_RX_BUFFER_LENGTH 1
#define CMD_BUFFER_LENGTH 32

void usart_read_callback(const struct usart_module *const usart_module);
void usart_write_callback(const struct usart_module *const usart_module);
void configure_usart(void);
void configure_usart_callbacks(void);

void clear_buffer(void);
void do_cmd(void);

struct				usart_module usart_instance;
volatile uint8_t	rx_buffer[MAX_RX_BUFFER_LENGTH];uint8_t				cmd_buffer[CMD_BUFFER_LENGTH]; 
uint8_t				cmd_buffer_size;

void usart_read_callback(const struct usart_module *const usart_module)
{
	check_awake();
	
	if(cmd_buffer_size < CMD_BUFFER_LENGTH && rx_buffer[0] != 10) {
		cmd_buffer[cmd_buffer_size++] = rx_buffer[0];
	}
	else {
		do_cmd();
		clear_buffer();
	}
}

void do_cmd() {
	switch(cmd_buffer[0]) {
		case CMD_SET_TIME:
			set_time(cmd_buffer);
			break;	
		case CMD_SET_ALARM:
			set_alarm(cmd_buffer);
			break;
	}
} 

void clear_buffer() {
	cmd_buffer_size = 0;
	for(uint8_t i = 0; i < CMD_BUFFER_LENGTH; i++) {
		cmd_buffer[i] = 0;
	}
}

void usart_write_callback(const struct usart_module *const usart_module)
{
	port_pin_toggle_output_level(LED_0_PIN);
}
void configure_usart(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate = 9600;
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	while (usart_init(&usart_instance, EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
	}
	usart_enable(&usart_instance);
	clear_buffer();
}

void configure_usart_callbacks(void)
{
	usart_register_callback(&usart_instance, usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_instance, usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
}

int main (void)
{
	system_init();
	gfx_mono_init();
	
	configure_usart();
	configure_usart_callbacks();
	
	configure_rtc_calendar();
	struct rtc_calendar_time time;
	rtc_calendar_get_time_defaults(&time);
	rtc_calendar_set_time(&rtc_instance, &time);
	
	configure_tc();
	configure_tc_callbacks();
	
	configure_buttons();
	system_interrupt_enable_global();

	wake_up(); 

	uint8_t string[] = "time\n";
	usart_write_buffer_job(&usart_instance, string, sizeof(string));
	
	while (true) {
		usart_read_buffer_job(&usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
	}
}
