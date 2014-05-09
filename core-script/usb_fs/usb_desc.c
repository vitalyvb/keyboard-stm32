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

#include "usb_defs.h"
#include "usb_desc.h"
#include "usb_conf.h"

/* USB Standard Device Descriptor */
const uint8_t APP_DeviceDescriptor[USB_DEVICE_DESCRIPTOR_SIZE] = {
	USB_DEVICE_DESCRIPTOR_SIZE,  /* bLength */
	USB_DEVICE_DESCRIPTOR_TYPE,  /* bDescriptorType */
	0x00,                        /* bcdUSB, version 2.00 */
	0x02,
	0x00,                        /* bDeviceClass : See interface */
	0x00,                        /* bDeviceSubClass : See interface*/
	0x00,                        /* bDeviceProtocol : See interface */
	ENDP0_BUFSIZE,               /* bMaxPacketSize0 */
	LSB(USBD_VID_CORE),          /* idVendor */
	MSB(USBD_VID_CORE),
	LSB(USBD_PID_CORE),          /* idProduct */
	MSB(USBD_PID_CORE),
	0x00,                        /* bcdDevice*/
	0x02,

	APP_ID_STRING_VENDOR,        /* iManufacturer : index of string Manufacturer  */
	APP_ID_STRING_PRODUCT,       /* iProduct      : index of string descriptor of product*/
	APP_ID_STRING_SERIAL,        /* iSerialNumber : index of string serial number*/
	NUM_CONFIGURATIONS           /* bNumConfigurations */
};

const uint8_t Kbd_ReportDescriptor[] =
{
    /* 0x00 */
	0x05, 0x01,          // Usage Page (Generic Desktop),
	0x09, 0x06,          // Usage (Keyboard),
	0xA1, 0x01,          // Collection (Application),
	0x75, 0x01,          //   Report Size (1),
	0x95, 0x08,          //   Report Count (8),
	0x05, 0x07,          //   Usage Page (Key Codes),
	0x19, 0xE0,          //   Usage Minimum (224),
	0x29, 0xE7,          //   Usage Maximum (231),
    /* 0x10 */
	0x15, 0x00,          //   Logical Minimum (0),
	0x25, 0x01,          //   Logical Maximum (1),
	0x81, 0x02,          //   Input (Data, Variable, Absolute), ;Modifier byte
	0x95, 0x01,          //   Report Count (1),
	0x75, 0x08,          //   Report Size (8),
	0x81, 0x03,          //   Input (Constant),                 ;Reserved byte
	0x95, 0x05,          //   Report Count (5),
	0x75, 0x01,          //   Report Size (1),
    /* 0x20 */
	0x05, 0x08,          //   Usage Page (LEDs),
	0x19, 0x01,          //   Usage Minimum (1),
	0x29, 0x05,          //   Usage Maximum (5),
	0x91, 0x02,          //   Output (Data, Variable, Absolute), ;LED report
	0x95, 0x01,          //   Report Count (1),
	0x75, 0x03,          //   Report Size (3),
	0x91, 0x03,          //   Output (Constant),                 ;LED report padding
	0x95, KBD_REPORT_KEYS,    //   Report Count (),
    /* 0x30 */
	0x75, 0x08,          //   Report Size (8),
	0x15, 0x00,          //   Logical Minimum (0),
	0x25, 0xFF,          //   Logical Maximum(255),
	0x05, 0x07,          //   Usage Page (Key Codes),
	0x19, 0x00,          //   Usage Minimum (0),
	0x29, 0xFF,          //   Usage Maximum (255),
	0x81, 0x00,          //   Input (Data, Array),
	0xc0                 // End Collection
    /* 0x40 */
};

const uint8_t Kbd_Extra_ReportDescriptor[] = {
	/* system control */
	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	0x09, 0x80,                    // USAGE (System Control)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x85, REPORT_ID_SYSTEM,        //   REPORT_ID (2)
	0x15, 0x01,                    //   LOGICAL_MINIMUM (0x1)
	0x25, 0xb7,                    //   LOGICAL_MAXIMUM (0xb7)
	0x19, 0x01,                    //   USAGE_MINIMUM (0x1)
	0x29, 0xb7,                    //   USAGE_MAXIMUM (0xb7)
	0x75, 0x10,                    //   REPORT_SIZE (16)
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x81, 0x00,                    //   INPUT (Data,Array,Abs)
	0xc0,                          // END_COLLECTION
	/* consumer */
	0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
	0x09, 0x01,                    // USAGE (Consumer Control)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x85, REPORT_ID_CONSUMER,      //   REPORT_ID (3)
	0x15, 0x01,                    //   LOGICAL_MINIMUM (0x1)
	0x26, 0x9c, 0x02,              //   LOGICAL_MAXIMUM (0x29c)
	0x19, 0x01,                    //   USAGE_MINIMUM (0x1)
	0x2a, 0x9c, 0x02,              //   USAGE_MAXIMUM (0x29c)
	0x75, 0x10,                    //   REPORT_SIZE (16)
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x81, 0x00,                    //   INPUT (Data,Array,Abs)
	0xc0,                          // END_COLLECTION
};

const uint8_t Kbd_NKRO_ReportDescriptor[] = {
	0x05, 0x01,                     // Usage Page (Generic Desktop),
	0x09, 0x06,                     // Usage (Keyboard),
	0xA1, 0x01,                     // Collection (Application),
	// bitmap of modifiers
	0x75, 0x01,                     //   Report Size (1),
	0x95, 0x08,                     //   Report Count (8),
	0x05, 0x07,                     //   Usage Page (Key Codes),
	0x19, 0xE0,                     //   Usage Minimum (224),
	0x29, 0xE7,                     //   Usage Maximum (231),
	0x15, 0x00,                     //   Logical Minimum (0),
	0x25, 0x01,                     //   Logical Maximum (1),
	0x81, 0x02,                     //   Input (Data, Variable, Absolute), ;Modifier byte
	// LED output report
	0x95, 0x05,                     //   Report Count (5),
	0x75, 0x01,                     //   Report Size (1),
	0x05, 0x08,                     //   Usage Page (LEDs),
	0x19, 0x01,                     //   Usage Minimum (1),
	0x29, 0x05,                     //   Usage Maximum (5),
	0x91, 0x02,                     //   Output (Data, Variable, Absolute),
	0x95, 0x01,                     //   Report Count (1),
	0x75, 0x03,                     //   Report Size (3),
	0x91, 0x03,                     //   Output (Constant),
	// bitmap of keys
	0x95, KBD_NKRO_REPORT_KEYS*8,   //   Report Count (),
	0x75, 0x01,                     //   Report Size (1),
	0x15, 0x00,                     //   Logical Minimum (0),
	0x25, 0x01,                     //   Logical Maximum(1),
	0x05, 0x07,                     //   Usage Page (Key Codes),
	0x19, 0x00,                     //   Usage Minimum (0),
	0x29, KBD_NKRO_REPORT_KEYS*8-1, //   Usage Maximum (),
	0x81, 0x02,                     //   Input (Data, Variable, Absolute),
	0xc0                            // End Collection
};

const struct ConfigDescriptorStruct APP_ConfigDescriptor = {
     .config = {
	USB_CONFIGURATION_DESCRIPTOR_SIZE,	// bLength: Configuration Descriptor size
	USB_CONFIGURATION_DESCRIPTOR_TYPE,	// bDescriptorType: Configuration
	LSB(sizeof(struct ConfigDescriptorStruct)), // wTotalLength: Bytes returned
	MSB(sizeof(struct ConfigDescriptorStruct)),
	NUM_INTERFACES,				// bNumInterfaces
	0x01,					// bConfigurationValue
	0x00,					// iConfiguration
	0xA0,					// bmAttributes:  bus powered, remote wakeup
	0x32,					// MaxPower 100 mA
    },

    .kbd_interface = {
	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	USB_INTERFACE_DESCRIPTOR_SIZE,		// bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,		// bDescriptorType
	KBD_INTERFACE,				// bInterfaceNumber
	0,					// bAlternateSetting
	1,					// bNumEndpoints
	0x03,					// bInterfaceClass (0x03 = HID)
	0x01,					// bInterfaceSubClass (0x01 = Boot)
	0x01,					// bInterfaceProtocol (0x01 = Keyboard)
	0,					// iInterface
    },
    .kbd_hid = {
	// HID descriptor, HID 1.11 spec, section 6.2.1
	HID_DESCRIPTOR_SIZE,			// bLength
	HID_DESCRIPTOR_TYPE,			// bDescriptorType
	0x11, 0x01,				// bcdHID
	0,					// bCountryCode
	1,					// bNumDescriptors
	0x22,					// bDescriptorType
	sizeof(Kbd_ReportDescriptor),		// wDescriptorLength
	0,
    },
    .kbd_ep = {
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	USB_ENDPOINT_DESCRIPTOR_SIZE,		// bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,		// bDescriptorType
	KBD_ENDPOINT | 0x80,			// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	KBD_SIZE, 0,				// wMaxPacketSize
	10,					// bInterval
    },

    .kbd_extra_interface = {
	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	USB_INTERFACE_DESCRIPTOR_SIZE,		// bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,		// bDescriptorType
	KBD_EXTRA_INTERFACE,			// bInterfaceNumber
	0,					// bAlternateSetting
	1,					// bNumEndpoints
	0x03,					// bInterfaceClass (0x03 = HID)
	0x00,					// bInterfaceSubClass
	0x00,					// bInterfaceProtocol
	0,					// iInterface
    },
    .kbd_extra_hid = {
	// HID descriptor, HID 1.11 spec, section 6.2.1
	HID_DESCRIPTOR_SIZE,			// bLength
	HID_DESCRIPTOR_TYPE,			// bDescriptorType
	0x11, 0x01,				// bcdHID
	0,					// bCountryCode
	1,					// bNumDescriptors
	0x22,					// bDescriptorType
	sizeof(Kbd_Extra_ReportDescriptor),	// wDescriptorLength
	0,
    },
    .kbd_extra_ep = {
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	USB_ENDPOINT_DESCRIPTOR_SIZE,		// bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,		// bDescriptorType
	KBD_EXTRA_ENDPOINT | 0x80,		// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	KBD_EXTRA_SIZE, 0,			// wMaxPacketSize
	10,					// bInterval
    },


    .kbd_nkro_interface = {
	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	USB_INTERFACE_DESCRIPTOR_SIZE,		// bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,		// bDescriptorType
	KBD_NKRO_INTERFACE,			// bInterfaceNumber
	0,					// bAlternateSetting
	1,					// bNumEndpoints
	0x03,					// bInterfaceClass (0x03 = HID)
	0x00,					// bInterfaceSubClass
	0x00,					// bInterfaceProtocol
	0,					// iInterface
    },
    .kbd_nkro_hid = {
	// HID descriptor, HID 1.11 spec, section 6.2.1
	HID_DESCRIPTOR_SIZE,			// bLength
	HID_DESCRIPTOR_TYPE,			// bDescriptorType
	0x11, 0x01,				// bcdHID
	0,					// bCountryCode
	1,					// bNumDescriptors
	0x22,					// bDescriptorType
	sizeof(Kbd_NKRO_ReportDescriptor),	// wDescriptorLength
	0,
    },
    .kbd_nkro_ep = {
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	USB_ENDPOINT_DESCRIPTOR_SIZE,		// bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,		// bDescriptorType
	KBD_NKRO_ENDPOINT | 0x80,		// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	KBD_NKRO_SIZE, 0,			// wMaxPacketSize
	1,					// bInterval
    },


    .debug_interface_acm = {
	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	USB_INTERFACE_DESCRIPTOR_SIZE,		// bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,		// bDescriptorType
	DEBUG_INTERFACE_ACM,			// bInterfaceNumber
	0,					// bAlternateSetting
	1,					// bNumEndpoints
	0x02,					// bInterfaceClass (0x02 = Communication Interface Class)
	0x02,					// bInterfaceSubClass (0x02 = Abstract Control Model)
	0x01,					// bInterfaceProtocol (0x01 = Common AT commands)
	0,					// iInterface
    },

    .debug_1 = {
	/*Header Functional Descriptor*/
	0x05,   /* bLength: Endpoint Descriptor size */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x00,   /* bDescriptorSubtype: Header Func Desc */
	0x10,   /* bcdCDC: spec release number */
	0x01,
    },
    .debug_2 = {
	/*Call Management Functional Descriptor*/
	0x05,   /* bFunctionLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x01,   /* bDescriptorSubtype: Call Management Func Desc */
	0x00,   /* bmCapabilities: D0+D1 */
	DEBUG_INTERFACE_DATA,   /* bDataInterface */
    },
    .debug_3 = {
	/*ACM Functional Descriptor*/
	0x04,   /* bFunctionLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
	0x02,   /* bmCapabilities */
    },
    .debug_4 = {
	/*Union Functional Descriptor*/
	0x05,   /* bFunctionLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x06,   /* bDescriptorSubtype: Union func desc */
    	DEBUG_INTERFACE_ACM,    /* bMasterInterface: Communication class interface */
	DEBUG_INTERFACE_DATA,   /* bSlaveInterface0: Data Class Interface */
    },
    .debug_ep_acm = {
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	USB_ENDPOINT_DESCRIPTOR_SIZE,		// bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,		// bDescriptorType
	DEBUG_ENDPOINT_ACM | 0x80,		// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	DEBUG_SIZE_ACM, 0,			// wMaxPacketSize
	10,					// bInterval
    },


    .debug_interface_data = {
	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	USB_INTERFACE_DESCRIPTOR_SIZE,		// bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,		// bDescriptorType
	DEBUG_INTERFACE_DATA,			// bInterfaceNumber
	0,					// bAlternateSetting
	2,					// bNumEndpoints
	0x0a,					// bInterfaceClass (0x0a = CDC)
	0x00,					// bInterfaceSubClass
	0x00,					// bInterfaceProtocol
	0,					// iInterface
    },
    .debug_ep_data_out = {
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	USB_ENDPOINT_DESCRIPTOR_SIZE,		// bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,		// bDescriptorType
	DEBUG_ENDPOINT_DATA,			// bEndpointAddress
	0x02,					// bmAttributes (0x02=bulk)
	DEBUG_SIZE_DATA, 0,			// wMaxPacketSize
	1,					// bInterval
    },
    .debug_ep_data_in = {
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	USB_ENDPOINT_DESCRIPTOR_SIZE,		// bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,		// bDescriptorType
	DEBUG_ENDPOINT_DATA | 0x80,		// bEndpointAddress
	0x02,					// bmAttributes (0x02=bulk)
	DEBUG_SIZE_DATA, 0,			// wMaxPacketSize
	1,					// bInterval
    },

};

const struct usb_string_descriptor APP_StringLangId = {
    sizeof(struct usb_string_descriptor) + sizeof(uint16_t),
    USB_STRING_DESCRIPTOR_TYPE,
    { USBD_LANGID_STRING }			/* LangID */
};

const struct usb_string_descriptor APP_StringVendor = {
    sizeof(struct usb_string_descriptor) + USBD_MANUFACTURER_STRING_LENGTH*2,
    USB_STRING_DESCRIPTOR_TYPE,
    { USBD_MANUFACTURER_STRING },
};

const struct usb_string_descriptor APP_StringProduct = {
    sizeof(struct usb_string_descriptor) + USBD_PRODUCT_STRING_LENGTH*2,
    USB_STRING_DESCRIPTOR_TYPE,
    { USBD_PRODUCT_STRING },
};

uint8_t APP_StringSerial_Buffer[sizeof(struct usb_string_descriptor) + USBD_SERIALNUMBER_STRING_LENGTH*2];

