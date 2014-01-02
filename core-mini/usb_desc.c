#include "usb_defs.h"
#include "usb_desc.h"
#include "usb_conf.h"
#include "platform_config.h"

/* USB Standard Device Descriptor */
const uint8_t DFU_DeviceDescriptor[USB_DEVICE_DESCRIPTOR_SIZE] = {
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

	DFU_ID_STRING_VENDOR,        /* iManufacturer : index of string Manufacturer  */
	DFU_ID_STRING_PRODUCT,       /* iProduct      : index of string descriptor of product*/
	DFU_ID_STRING_SERIAL,        /* iSerialNumber : index of string serial number*/
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

const int Kbd_ReportDescriptor_Size =  sizeof(Kbd_ReportDescriptor);

const struct ConfigDescriptorStruct DFU_ConfigDescriptor = {
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

};

const struct usb_string_descriptor DFU_StringLangId = {
    sizeof(struct usb_string_descriptor) + sizeof(uint16_t),
    USB_STRING_DESCRIPTOR_TYPE,
    { USBD_LANGID_STRING }			/* LangID */
};

const struct usb_string_descriptor DFU_StringVendor = {
    sizeof(struct usb_string_descriptor) + USBD_MANUFACTURER_STRING_LENGTH*2,
    USB_STRING_DESCRIPTOR_TYPE,
    { USBD_MANUFACTURER_STRING },
};

const struct usb_string_descriptor DFU_StringProduct = {
    sizeof(struct usb_string_descriptor) + USBD_PRODUCT_STRING_LENGTH*2,
    USB_STRING_DESCRIPTOR_TYPE,
    { USBD_PRODUCT_STRING },
};

uint8_t DFU_StringSerial_Buffer[sizeof(struct usb_string_descriptor) + USBD_SERIALNUMBER_STRING_LENGTH*2];

