#ifndef __USABUSE_H
#define __USABUSE_H
#include <LUFA/Drivers/Misc/RingBuffer.h>

#define TLV_MAX_PACKET 64

#define TLV_CONTROL 0
#define TLV_HID 1
#define TLV_PIPE 2
#define TLV_DEBUG 3

#define TLV_CONTROL_FLOW 0
#define TLV_CONTROL_CONNECT 1

/**
 * Enables the ESP8266 portion of the Cactus Micro Rev2, configures the UART to the specified baud rate, and
 * reads until it encounters a magic string of "d41d8cd98f00b204e9800998ecf8427e", signifying the beginning of
 * execution of the ESP code.
 *
 * Assumes that interrupts are not yet enabled, so it enables them to be able to receive UART characters,
 * and disables interrupts again on exit, to allow any USB initialisation to take place
 */
void initESP(uint32_t baud);

typedef struct {
	uint8_t channel;
	uint8_t length;
	uint8_t data[TLV_MAX_PACKET];
} ATTR_PACKED tlv_data_t;

void usabuse_task(void);

bool tlv_send_queue(uint8_t channel, uint8_t length, uint8_t *data);
void tlv_send_uart(void);

/**
 * Queries the internal ringbuffer for any available HID events.
 * If there are any available, it returns the event data in the data parameter
 * and indicates what sort of event it is in the return code.
 * If there is no HID Event available, it returns 0. 1 indicates a Keyboard
 * event, 2 indicates a mouse event.
 */
uint8_t usabuse_get_hid(uint8_t *data);
uint8_t usabuse_get_pipe(uint8_t *data, uint8_t max);
int16_t usabuse_peek_pipe(void);
int16_t usabuse_remove_pipe(void);

bool usabuse_put_pipe(uint8_t count, uint8_t *data);
void usabuse_debug(char *message);

bool usabuse_pipe_write_is_blocked(void);
void usabuse_victim_ready(bool ready);

#ifdef DEBUG
void tlv_initDebugBuffer(RingBuffer_t *buffer);
#endif
#endif
