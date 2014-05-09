/* Copyright (c) 2013, Vitaly Bursov <vitaly<AT>bursov.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef USB_DESC_H
#define USB_DESC_H

#include "usb_defs.h"
#include "hwusb_defs.h"

#define APP_ID_STRING_LANGID              0
#define APP_ID_STRING_VENDOR              1
#define APP_ID_STRING_PRODUCT             2
#define APP_ID_STRING_SERIAL              3

struct usb_string_descriptor {
    uint8_t size;
    uint8_t type;
    uint16_t string[];
} __attribute__((packed));

extern uint8_t APP_StringSerial_Buffer[sizeof(struct usb_string_descriptor) + USBD_SERIALNUMBER_STRING_LENGTH*2];

struct ConfigDescriptorStruct {
    uint8_t	config[USB_CONFIGURATION_DESCRIPTOR_SIZE];

    /* basic keyboard */
    uint8_t	kbd_interface[USB_INTERFACE_DESCRIPTOR_SIZE];
    uint8_t	kbd_hid[HID_DESCRIPTOR_SIZE];
    uint8_t	kbd_ep[USB_ENDPOINT_DESCRIPTOR_SIZE];

    /* extended, multimedia keys */
    uint8_t	kbd_extra_interface[USB_INTERFACE_DESCRIPTOR_SIZE];
    uint8_t	kbd_extra_hid[HID_DESCRIPTOR_SIZE];
    uint8_t	kbd_extra_ep[USB_ENDPOINT_DESCRIPTOR_SIZE];

    /* nkro keyboard */
    uint8_t	kbd_nkro_interface[USB_INTERFACE_DESCRIPTOR_SIZE];
    uint8_t	kbd_nkro_hid[HID_DESCRIPTOR_SIZE];
    uint8_t	kbd_nkro_ep[USB_ENDPOINT_DESCRIPTOR_SIZE];

    /* serial debug */
    uint8_t	debug_interface_acm[USB_INTERFACE_DESCRIPTOR_SIZE];
    uint8_t	debug_1[5];
    uint8_t	debug_2[5];
    uint8_t	debug_3[4];
    uint8_t	debug_4[5];
    uint8_t	debug_ep_acm[USB_ENDPOINT_DESCRIPTOR_SIZE];

    uint8_t	debug_interface_data[USB_INTERFACE_DESCRIPTOR_SIZE];
    uint8_t	debug_ep_data_out[USB_ENDPOINT_DESCRIPTOR_SIZE];
    uint8_t	debug_ep_data_in[USB_ENDPOINT_DESCRIPTOR_SIZE];
} __attribute__((packed));

#endif /* USB_DESC_H */
