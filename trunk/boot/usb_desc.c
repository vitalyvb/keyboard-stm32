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
	ENDP0_BUFSIZE,               /* bMaxPacketSize0 0x40 = 64 */
	0x83,                        /* idVendor     (0483) */
	0x04,
	0x11,                        /* idProduct (0xDF11) DFU PiD*/
	0xDF,
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

    .dfu_interface1 = {
	USB_INTERFACE_DESCRIPTOR_SIZE,		// bLength: Interface Descriptor size
	USB_INTERFACE_DESCRIPTOR_TYPE,		// bDescriptorType
	DFU1_INTERFACE,				// bInterfaceNumber: Number of Interface
	0x00,					// bAlternateSetting: Alternate setting
	0x00,					// bNumEndpoints
	0xFE,					// bInterfaceClass: Application Specific Class Code
	0x01,					// bInterfaceSubClass: Device Firmware Upgrade Code
	0x02,					// nInterfaceProtocol: DFU mode protocol
	DFU_ID_STRING_INTERFACE0,		// iInterface
    },

    .dfu_func1 = {
	// DFU Functional Descriptor
	0x09,					// bLength = 9 Bytes
	0x21,					// DFU Functional Descriptor
	0x0B,					// bmAttribute
						//	bitCanDnload             = 1      (bit 0)
						//	bitCanUpload             = 1      (bit 1)
						//	bitManifestationTolerant = 0      (bit 2)
						//	bitWillDetach            = 1      (bit 3)
						//	Reserved                          (bit4-6)
						//	bitAcceleratedST         = 0      (bit 7)
	0xFF,					// DetachTimeOut= 255 ms
	0x00,
	LSB(wTransferSize),			// TransferSize
	MSB(wTransferSize),
	0x1A,					// bcdDFUVersion: 1.1a
	0x01,
    },
};


const uint8_t DFU_StringLangId[DFU_SIZ_STRING_LANGID] =
{
    DFU_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04    /* LangID = 0x0409: U.S. English */
};

const uint8_t DFU_StringVendor[DFU_SIZ_STRING_VENDOR] =
{
    DFU_SIZ_STRING_VENDOR,
    USB_STRING_DESCRIPTOR_TYPE,
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
};

const uint8_t DFU_StringProduct[DFU_SIZ_STRING_PRODUCT] =
{
    DFU_SIZ_STRING_PRODUCT,
    USB_STRING_DESCRIPTOR_TYPE,
    /* Product name: "STM32 DFU" */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'D', 0, 'F', 0, 'U', 0
};

uint8_t DFU_StringSerial[DFU_SIZ_STRING_SERIAL] =
{
    DFU_SIZ_STRING_SERIAL,
    USB_STRING_DESCRIPTOR_TYPE,
    /* Serial number */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0  
};

const uint8_t DFU_StringInterface0[DFU_SIZ_STRING_INTERFACE0] =
{
    DFU_SIZ_STRING_INTERFACE0,
    USB_STRING_DESCRIPTOR_TYPE,
    // Interface 0: "@Internal Flash   /0x08000000/12*001Ka,116*001Kg"
    '@', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'n', 0, 'a', 0, 'l', 0,  /* 18 */
    ' ', 0, 'F', 0, 'l', 0, 'a', 0, 's', 0, 'h', 0, ' ', 0, ' ', 0, /* 16 */

    '/', 0, '0', 0, 'x', 0, '0', 0, '8', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, /* 22 */

    '/', 0, '1', 0, '2', 0, '*', 0, '0', 0, '0', 0, '1', 0, 'K', 0, 'a', 0, /* 18 */
    ',', 0, '1', 0, '1', 0, '6', 0, '*', 0, '0', 0, '0', 0, '1', 0, 'K', 0, 'g', 0, /* 20 */
};

