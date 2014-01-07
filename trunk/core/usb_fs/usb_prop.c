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

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_endp.h"
#include "usb_pwr.h"
#include "hal_usb.h"
#include "defs.h"

#include "usb_desc.c"


#define INTERFACE_TYPE_NONE (0)
#define INTERFACE_TYPE_HID (1)
#define INTERFACE_TYPE_ACM (2)

union interface_handler {

    struct {
	/* these fields must be the same across union */
	int type;
    } generic;

    struct {
	int type;

	int (*set_idle)(int report_id, uint32_t time_ms);

	int (*get_report)(uint8_t *dst_ptr);
	void (*set_report)(uint8_t *src_ptr, size_t size);

	const uint8_t *hid_descriptor;
	const uint8_t *report_descriptor;

	uint8_t hid_descriptor_size;
	uint8_t report_descriptor_size;

	uint8_t have_idle;
	uint8_t have_protocol;

	uint8_t report_size;

    } hid;

    struct {
	int type;

    } acm;
};

#define interface_handler_t const union interface_handler

struct acm_linecoding {
  uint32_t bitrate;
  uint8_t format;
  uint8_t paritytype;
  uint8_t datatype;
};

/******************************************************************************/

static uint8_t HID_report_buffer[USB_HID_REPORT_BUFFER_SIZE];
static uint8_t HID_idle_values[NUM_INTERFACES];
static uint8_t HID_protocol_values[NUM_INTERFACES];

static __IO int HID_request;
static __IO interface_handler_t *HID_request_handler;

struct acm_linecoding linecoding = {
    .bitrate = 115200,
    .format = 0x00,	/* 1 stop bit */
    .paritytype = 0x00,	/* none */
    .datatype = 0x08	/* 8 bits */
};

/******************************************************************************/

const ONE_DESCRIPTOR Device_Descriptor = {
    .Descriptor = (uint8_t*)(APP_DeviceDescriptor),
    .Descriptor_Size = sizeof(APP_DeviceDescriptor)
};

const ONE_DESCRIPTOR Config_Descriptor = {
    .Descriptor = (uint8_t*)(&APP_ConfigDescriptor),
    .Descriptor_Size = sizeof(APP_ConfigDescriptor)
};

const struct usb_string_descriptor *String_Descriptor[] = {
    [APP_ID_STRING_LANGID] = &APP_StringLangId,
    [APP_ID_STRING_VENDOR] = &APP_StringVendor,
    [APP_ID_STRING_PRODUCT] = &APP_StringProduct,
    [APP_ID_STRING_SERIAL] = (struct usb_string_descriptor*)&APP_StringSerial_Buffer,
};
#define STRING_DESCRIPTOR_COUNT (sizeof(String_Descriptor)/sizeof(String_Descriptor[0]))


interface_handler_t interface_handlers[] = {

    [KBD_INTERFACE] = {
	.hid = {
	    .type = INTERFACE_TYPE_HID,

	    .hid_descriptor = APP_ConfigDescriptor.kbd_hid,
	    .hid_descriptor_size = sizeof(APP_ConfigDescriptor.kbd_hid),
	    .report_descriptor = Kbd_ReportDescriptor,
	    .report_descriptor_size = sizeof(Kbd_ReportDescriptor),

	    .set_idle = app_usb_set_kbd_idle,
	    .set_report = app_usb_set_kbd_report,
	    .get_report = app_usb_get_kbd_report,

	    .have_idle = 1,
	    .have_protocol = 1,

	    .report_size = KBD_SIZE,
	},
    },
    [KBD_EXTRA_INTERFACE] = {
	.hid = {
	    .type = INTERFACE_TYPE_HID,

	    .hid_descriptor = APP_ConfigDescriptor.kbd_extra_hid,
	    .hid_descriptor_size = sizeof(APP_ConfigDescriptor.kbd_extra_hid),
	    .report_descriptor = Kbd_Extra_ReportDescriptor,
	    .report_descriptor_size = sizeof(Kbd_Extra_ReportDescriptor),

	    .report_size = KBD_EXTRA_SIZE,
	},
    },
    [KBD_NKRO_INTERFACE] = {
	.hid = {
	    .type = INTERFACE_TYPE_HID,

	    .hid_descriptor = APP_ConfigDescriptor.kbd_nkro_hid,
	    .hid_descriptor_size = sizeof(APP_ConfigDescriptor.kbd_nkro_hid),
	    .report_descriptor = Kbd_NKRO_ReportDescriptor,
	    .report_descriptor_size = sizeof(Kbd_NKRO_ReportDescriptor),

	    .report_size = KBD_NKRO_SIZE,
	},
    },

    [DEBUG_INTERFACE_ACM] = {
	.acm = {
	    .type = INTERFACE_TYPE_ACM,
	},
    },
};

#define INTERFACE_HANDLERS_COUNT (sizeof(interface_handlers)/sizeof(interface_handlers[0]))

/*******************************************************************************/
/*******************************************************************************/

uint8_t *ACM_CopyLineCodingBuffer(uint16_t Length)
{
    if (Length == 0) {
	pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
	return NULL;
    }
    return (uint8_t *)&linecoding;
}

RESULT ACM_Data_Setup(uint8_t RequestNo, interface_handler_t *hnd)
{
    uint8_t *(*CopyRoutine)(uint16_t) = NULL;

    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)){

	switch (RequestNo){
	case GET_LINE_CODING:
	case SET_LINE_CODING:
	    CopyRoutine = ACM_CopyLineCodingBuffer;
	    break;
	}

    }

    if (CopyRoutine == NULL)
	return USB_UNSUPPORT;

    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    pInformation->Ctrl_Info.Usb_wOffset = 0;
    (*CopyRoutine)(0);

    return USB_SUCCESS;
}

RESULT ACM_NoData_Setup(uint8_t RequestNo, interface_handler_t *hnd)
{
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)){

	switch (RequestNo){
	    case SET_COMM_FEATURE:
	    case SET_CONTROL_LINE_STATE:
		/* no-op */
		return USB_SUCCESS;
	}

    }
    return USB_UNSUPPORT;
}

/*******************************************************************************/
/*******************************************************************************/

uint8_t *HID_CopyReportBuffer(uint16_t Length)
{
    uint8_t interface = pInformation->USBwIndex0;

    if (Length == 0){
	pInformation->Ctrl_Info.Usb_wLength = interface_handlers[interface].hid.report_size;
	return NULL;
    }

    return (uint8_t *)HID_report_buffer;
}

uint8_t *HID_GetProtocolValue(uint16_t Length)
{
    uint8_t interface = pInformation->USBwIndex0;

    if (Length == 0){
	pInformation->Ctrl_Info.Usb_wLength = 1;
	return NULL;
    }
    return (uint8_t *)&HID_protocol_values[interface];
}

uint8_t *HID_GetIdleValue(uint16_t Length)
{
    uint8_t interface = pInformation->USBwIndex0;

    if (Length == 0){
	pInformation->Ctrl_Info.Usb_wLength = 1;
	return NULL;
    }

    return (uint8_t *)&HID_idle_values[interface];
}

RESULT HID_SetProtocol(interface_handler_t *hnd)
{
    uint8_t interface = pInformation->USBwIndex0;
    uint8_t wValue0 = pInformation->USBwValue0;
    HID_protocol_values[interface] = wValue0;
    return USB_SUCCESS;
}

RESULT HID_SetIdle(interface_handler_t *hnd)
{
    int res = -1;
    uint32_t duration = pInformation->USBwValue0;
    int report_id = pInformation->USBwValue1;
    uint8_t interface = pInformation->USBwIndex0;

    if (report_id != 0)
	return USB_UNSUPPORT;

    if (HID_idle_values[interface] != duration){
	HID_idle_values[interface] = duration;
	if (hnd->hid.set_idle)
	    res = hnd->hid.set_idle(report_id, duration * 4); /* duration units are 4ms */
    }

    if (res < 0)
	return USB_UNSUPPORT;

    return USB_SUCCESS;
}

uint8_t *HID_GetAnyDescriptor(uint16_t Length)
{
    interface_handler_t *h;
    const uint8_t *descr;
    uint32_t descr_size;
    uint32_t wOffset;
    uint8_t interface = pInformation->USBwIndex0;
    uint8_t descriptor = pInformation->USBwValue1;

    h = &interface_handlers[interface];
    switch (descriptor){
	case REPORT_DESCRIPTOR_TYPE:
	    descr = h->hid.report_descriptor;
	    descr_size = h->hid.report_descriptor_size;
	    break;
	case HID_DESCRIPTOR_TYPE:
	    descr = h->hid.hid_descriptor;
	    descr_size = h->hid.hid_descriptor_size;
	    break;
	default:
	    descr = NULL;
	    descr_size = 0;
    }

    wOffset = pInformation->Ctrl_Info.Usb_wOffset;

    if (Length == 0) {
	pInformation->Ctrl_Info.Usb_wLength = descr_size - wOffset;
	return 0;
    }

    return ((uint8_t*)descr) + wOffset;
}

void HID_Status_In(interface_handler_t *hnd)
{
    int req = HID_request;

    if (likely(req == 0)){
	return;
    }

    HID_request = 0;

    if (req == SET_REPORT){
	if (HID_request_handler && HID_request_handler->hid.set_report){
	    size_t len = HID_request_handler->hid.report_size;
	    /* FIXME calculate actual transfer size*/
	    HID_request_handler->hid.set_report(HID_report_buffer, len);
	}
    }
}

void HID_Status_Out (interface_handler_t *hnd)
{
    /* noop */
}

RESULT HID_Data_Setup(uint8_t RequestNo, interface_handler_t *hnd)
{
    uint8_t *(*CopyRoutine)(uint16_t) = NULL;
    uint8_t descriptor;

    if ((RequestNo == GET_DESCRIPTOR) &&
	    (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))) {

	descriptor = pInformation->USBwValue1;

	if (descriptor == REPORT_DESCRIPTOR_TYPE || descriptor == HID_DESCRIPTOR_TYPE)
	    CopyRoutine = HID_GetAnyDescriptor;

    } else if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)){

	switch (RequestNo){
	case GET_PROTOCOL:
	    if (hnd->hid.have_protocol)
		CopyRoutine = HID_GetProtocolValue;
	    break;
	case GET_REPORT:
	    if (hnd->hid.get_report){
		if (hnd->hid.get_report(HID_report_buffer) >= 0)
		    CopyRoutine = HID_CopyReportBuffer;
	    }
	    break;
	case GET_IDLE:
	    if (hnd->hid.have_idle)
		CopyRoutine = HID_GetIdleValue;
	    break;
	case SET_REPORT:
	    CopyRoutine = HID_CopyReportBuffer;
	    HID_request = SET_REPORT;
	    HID_request_handler = hnd;
	    break;
	}

    }

    if (CopyRoutine == NULL)
	return USB_UNSUPPORT;

    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    pInformation->Ctrl_Info.Usb_wOffset = 0;
    (*CopyRoutine)(0);

    return USB_SUCCESS;
}

RESULT HID_NoData_Setup(uint8_t RequestNo, interface_handler_t *hnd)
{
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)){

	switch (RequestNo){
	    case SET_PROTOCOL:
		if (hnd->hid.have_protocol)
		    return HID_SetProtocol(hnd);
		break;
	    case SET_IDLE:
		if (hnd->hid.have_idle)
		    return HID_SetIdle(hnd);
		break;
	}
    }
    return USB_UNSUPPORT;
}

/*******************************************************************************/
/*******************************************************************************/

static void Setup_Serial_String_Descr()
{
    struct usb_string_descriptor *descr;

    descr = (struct usb_string_descriptor *)APP_StringSerial_Buffer;

    descr->type = USB_STRING_DESCRIPTOR_TYPE;

    /* Update the serial number string descriptor with the data from the unique ID*/
    if (!app_usb_get_serial_num((uint8_t*)descr->string)){
	descr->size = sizeof(APP_StringSerial_Buffer);
    } else {
	descr->size = sizeof(struct usb_string_descriptor);
    }
}

void APP_init(void)
{
    DEVICE_INFO *pInfo = &Device_Info;

    Setup_Serial_String_Descr();

    pInfo->Current_Configuration = 0;
    pInfo->Current_Feature = APP_ConfigDescriptor.config[7];

    bDeviceState = UNCONNECTED;

    /* Connect the device */
    PowerOn();

    /* Perform basic device initialization operations */
    USB_SIL_Init();

    /* Enable USB interrupts */
    app_usb_enable_interrupts();
}

void APP_Reset(void)
{
    /* Set APP_DEVICE as not configured */
    Device_Info.Current_Configuration = 0;

    /* Current Feature initialization */
    pInformation->Current_Feature = APP_ConfigDescriptor.config[7];

    SetBTABLE(BTABLE_ADDRESS);

    /* Initialize Endpoint 0 */
    SetEPType(ENDP0, EP_CONTROL);
    SetEPTxStatus(ENDP0, EP_TX_NAK);
    SetEPRxAddr(ENDP0, ENDP0_RXADDR);
    SetEPRxCount(ENDP0, ENDP0_BUFSIZE);
    SetEPTxAddr(ENDP0, ENDP0_TXADDR);
    SetEPTxCount(ENDP0, ENDP0_BUFSIZE);
    Clear_Status_Out(ENDP0);
    SetEPRxValid(ENDP0);

    /* Initialize Endpoint 1: KEYBOARD */
    SetEPType(ENDP1, EP_INTERRUPT);
    SetEPTxAddr(ENDP1, ENDP1_TXADDR);
    SetEPTxCount(ENDP1, KBD_SIZE);
    SetEPTxStatus(ENDP1, EP_TX_NAK);
    SetEPRxStatus(ENDP1, EP_RX_DIS);

    /* Initialize Endpoint 2: KEYBOARD - EXTRA */
    SetEPType(ENDP2, EP_INTERRUPT);
    SetEPTxAddr(ENDP2, ENDP2_TXADDR);
    SetEPTxCount(ENDP2, KBD_EXTRA_SIZE);
    SetEPTxStatus(ENDP2, EP_TX_NAK);
    SetEPRxStatus(ENDP2, EP_RX_DIS);

    /* Initialize Endpoint 3: KEYBOARD - NKRO */
    SetEPType(ENDP3, EP_INTERRUPT);
    SetEPTxAddr(ENDP3, ENDP3_TXADDR);
    SetEPTxCount(ENDP3, KBD_NKRO_SIZE);
    SetEPTxStatus(ENDP3, EP_TX_NAK);
    SetEPRxStatus(ENDP3, EP_RX_DIS);

    /* Initialize Endpoint 4: Mouse, todo */


    /* Initialize Endpoint 5 and 6: DEBUG */
    SetEPType(ENDP5, EP_INTERRUPT);
    SetEPTxAddr(ENDP5, ENDP5_TXADDR);
    SetEPTxCount(ENDP5, DEBUG_SIZE_ACM);
    SetEPRxStatus(ENDP5, EP_RX_DIS);
    SetEPTxStatus(ENDP5, EP_TX_NAK);

    SetEPType(ENDP6, EP_BULK);
    SetEPTxAddr(ENDP6, ENDP6_TXADDR);
    SetEPRxAddr(ENDP6, ENDP6_RXADDR);
    SetEPTxCount(ENDP6, DEBUG_SIZE_DATA);
    SetEPRxCount(ENDP6, DEBUG_SIZE_DATA);
    SetEPRxStatus(ENDP6, EP_RX_DIS);
    SetEPTxStatus(ENDP6, EP_TX_NAK);

    /* Set this device to response on default address */
    SetDeviceAddress(0);

    /* Set the new control state of the device to Attached */
    bDeviceState = ATTACHED;

    HID_request = 0;

    usb_ep_handlers_reset();
    app_usb_reset();
}

void APP_SetConfiguration(void)
{
    DEVICE_INFO *pInfo = &Device_Info;

    if (pInfo->Current_Configuration != 0) {
	/* Device configured */
	bDeviceState = CONFIGURED;
    }
}

void APP_SetDeviceAddress (void)
{
    bDeviceState = ADDRESSED;
}

void APP_Status_In(void)
{
    interface_handler_t *h;
    uint8_t interface;

    interface = pInformation->USBwIndex0;
    if (interface < INTERFACE_HANDLERS_COUNT){
	h = &interface_handlers[interface];
	switch (h->generic.type){
	    case INTERFACE_TYPE_HID:
		return HID_Status_In(h);
		break;
	    case INTERFACE_TYPE_ACM:
		//return ACM_Status_In(h);
		break;
	}
    }
}

void APP_Status_Out (void)
{
    interface_handler_t *h;
    uint8_t interface;

    interface = pInformation->USBwIndex0;
    if (interface < INTERFACE_HANDLERS_COUNT){
	h = &interface_handlers[interface];
	switch (h->generic.type){
	    case INTERFACE_TYPE_HID:
		return HID_Status_Out(h);
		break;
	    case INTERFACE_TYPE_ACM:
		//return ACM_Status_Out(h);
		break;
	}
    }
}

RESULT APP_Data_Setup(uint8_t RequestNo)
{
    interface_handler_t *h;
    uint8_t interface;

    interface = pInformation->USBwIndex0;
    if (interface < INTERFACE_HANDLERS_COUNT){
	h = &interface_handlers[interface];
	switch (h->generic.type){
	    case INTERFACE_TYPE_HID:
		return HID_Data_Setup(RequestNo, h);
		break;
	    case INTERFACE_TYPE_ACM:
		return ACM_Data_Setup(RequestNo, h);
		break;
	}
    }

    return USB_UNSUPPORT;
}

RESULT APP_NoData_Setup(uint8_t RequestNo)
{
    interface_handler_t *h;
    uint8_t interface;

    interface = pInformation->USBwIndex0;
    if (interface < INTERFACE_HANDLERS_COUNT){
	h = &interface_handlers[interface];
	switch (h->generic.type){
	    case INTERFACE_TYPE_HID:
		return HID_NoData_Setup(RequestNo, h);
		break;
	    case INTERFACE_TYPE_ACM:
		return ACM_NoData_Setup(RequestNo, h);
		break;
	}
    }

    return USB_UNSUPPORT;
}

uint8_t *APP_GetStringDescriptor(uint16_t Length)
{
    uint8_t wValue0 = pInformation->USBwValue0;
    const struct usb_string_descriptor *descr;
    uint32_t  wOffset;

    if (wValue0 >= STRING_DESCRIPTOR_COUNT) {
	return NULL;
    }

    descr = String_Descriptor[wValue0];
    wOffset = pInformation->Ctrl_Info.Usb_wOffset;

    if (Length == 0) {
	pInformation->Ctrl_Info.Usb_wLength = descr->size - wOffset;
	return 0;
    }

    return ((uint8_t*)descr) + wOffset;
}

uint8_t *APP_GetDeviceDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, (ONE_DESCRIPTOR*)&Device_Descriptor);
}

uint8_t *APP_GetConfigDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData (Length, (ONE_DESCRIPTOR*)&Config_Descriptor);
}

RESULT APP_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
    if (AlternateSetting >= NUM_CONFIGURATIONS) {
	return USB_UNSUPPORT;
    } else if (Interface >= NUM_INTERFACES) {
	return USB_UNSUPPORT;
    }

    return USB_SUCCESS;
}

/******************************************************************************/
/******************************************************************************/

DEVICE Device_Table =
{
    .Total_Endpoint = NUM_ENDPOINTS,
    .Total_Configuration = NUM_CONFIGURATIONS
};

DEVICE_PROP Device_Property =
{
    .Init = APP_init,
    .Reset = APP_Reset,
    .Process_Status_IN = APP_Status_In,
    .Process_Status_OUT = APP_Status_Out,
    .Class_Data_Setup = APP_Data_Setup,
    .Class_NoData_Setup = APP_NoData_Setup,
    .Class_Get_Interface_Setting = APP_Get_Interface_Setting,
    .GetDeviceDescriptor = APP_GetDeviceDescriptor,
    .GetConfigDescriptor = APP_GetConfigDescriptor,
    .GetStringDescriptor = APP_GetStringDescriptor,
    .MaxPacketSize = ENDP0_BUFSIZE,
};

USER_STANDARD_REQUESTS User_Standard_Requests =
{
    .User_GetConfiguration = NOP_Process,
    .User_SetConfiguration = APP_SetConfiguration,
    .User_GetInterface = NOP_Process,
    .User_SetInterface = NOP_Process,
    .User_GetStatus = NOP_Process,
    .User_ClearFeature = NOP_Process,
    .User_SetEndPointFeature = NOP_Process,
    .User_SetDeviceFeature = NOP_Process,
    .User_SetDeviceAddress = APP_SetDeviceAddress,
};
