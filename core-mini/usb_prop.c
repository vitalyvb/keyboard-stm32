/**
  ******************************************************************************
  * @file    usb_prop.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   All processings related to DFU demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"

#include "main_dfukbd.h"

#include "usb_desc.c"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/******************************************************************************/

void Kbd_Status_In(void);
void Kbd_Status_Out(void);

RESULT Kbd_SetProtocol(void);
RESULT Kbd_SetIdle(void);

uint8_t *Kbd_SetReport(uint16_t Length);
uint8_t *Kbd_GetReport(uint16_t Length);

uint8_t *Kbd_GetProtocolValue(uint16_t Length);
uint8_t *Kbd_GetIdleValue(uint16_t Length);

uint8_t *Kbd_GetHIDDescriptor(uint16_t Length);
uint8_t *Kbd_GetReportDescriptor(uint16_t Length);

__IO int Kbd_Request;
__IO uint8_t KbdGetReport[KBD_SIZE];
__IO uint8_t KbdSetReport[KBD_SIZE];
uint32_t ProtocolValue;
uint32_t ProtocolIdle;

/******************************************************************************/

DEVICE Device_Table =
{
    .Total_Endpoint = EP_NUM,
    .Total_Configuration = NUM_CONFIGURATIONS
};

DEVICE_PROP Device_Property =
{
    .Init = DFU_init,
    .Reset = DFU_Reset,
    .Process_Status_IN = DFU_Status_In,
    .Process_Status_OUT = DFU_Status_Out,
    .Class_Data_Setup = DFU_Data_Setup,
    .Class_NoData_Setup = DFU_NoData_Setup,
    .Class_Get_Interface_Setting = DFU_Get_Interface_Setting,
    .GetDeviceDescriptor = DFU_GetDeviceDescriptor,
    .GetConfigDescriptor = DFU_GetConfigDescriptor,
    .GetStringDescriptor = DFU_GetStringDescriptor,
    .MaxPacketSize = ENDP0_BUFSIZE,
};

USER_STANDARD_REQUESTS User_Standard_Requests =
{
    .User_GetConfiguration = NOP_Process,
    .User_SetConfiguration = DFU_SetConfiguration,
    .User_GetInterface = NOP_Process,
    .User_SetInterface = NOP_Process,
    .User_GetStatus = NOP_Process,
    .User_ClearFeature = NOP_Process,
    .User_SetEndPointFeature = NOP_Process,
    .User_SetDeviceFeature = NOP_Process,
    .User_SetDeviceAddress = DFU_SetDeviceAddress,
};


#define MK_ADESCRIPTOR(_p) { .Descriptor = (uint8_t*)(_p), .Descriptor_Size = sizeof(_p) }
#define MK_SDESCRIPTOR(_p) { .Descriptor = (uint8_t*)(&_p), .Descriptor_Size = sizeof(_p) }

ONE_DESCRIPTOR Device_Descriptor = MK_ADESCRIPTOR(DFU_DeviceDescriptor);
const struct usb_string_descriptor *String_Descriptor[] = {
    [DFU_ID_STRING_LANGID] = &DFU_StringLangId,
    [DFU_ID_STRING_VENDOR] = &DFU_StringVendor,
    [DFU_ID_STRING_PRODUCT] = &DFU_StringProduct,
    [DFU_ID_STRING_SERIAL] = (struct usb_string_descriptor*)&DFU_StringSerial_Buffer,
};
#define STRING_DESCRIPTOR_COUNT (sizeof(String_Descriptor)/sizeof(String_Descriptor[0]))

ONE_DESCRIPTOR Config_Descriptor = MK_SDESCRIPTOR(DFU_ConfigDescriptor);


ONE_DESCRIPTOR Kbd_Report_Descriptor = MK_ADESCRIPTOR(Kbd_ReportDescriptor);
ONE_DESCRIPTOR Kbd_Hid_Descriptor = MK_SDESCRIPTOR(DFU_ConfigDescriptor.kbd_hid);


/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Setup_Serial_String_Descr()
{
    struct usb_string_descriptor *descr;

    descr = (struct usb_string_descriptor *)DFU_StringSerial_Buffer;

    descr->type = USB_STRING_DESCRIPTOR_TYPE;

    /* Update the serial number string descriptor with the data from the unique ID*/
    if (!Get_SerialNum((uint8_t*)descr->string)){
	descr->size = sizeof(DFU_StringSerial_Buffer);
    } else {
	descr->size = sizeof(struct usb_string_descriptor);
    }
}

/*******************************************************************************
* Function Name  : DFU_init.
* Description    : DFU init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_init(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  Setup_Serial_String_Descr();

  pInfo->Current_Configuration = 0;
  pInfo->Current_Feature = DFU_ConfigDescriptor.config[7];

  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  /* Enable USB interrupts */
  USB_Interrupts_Config();

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : DFU_Reset.
* Description    : DFU reset routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_Reset(void)
{
  Kbd_Request = 0;

  /* Set DFU_DEVICE as not configured */
  Device_Info.Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = DFU_ConfigDescriptor.config[7];

  _SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  _SetEPType(ENDP0, EP_CONTROL);
  _SetEPTxStatus(ENDP0, EP_TX_NAK);
  _SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  _SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  SetEPTxCount(ENDP0, Device_Property.MaxPacketSize);
  Clear_Status_Out(ENDP0);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1: KEYBOARD */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, KBD_SIZE);
  SetEPTxStatus(ENDP1, EP_TX_NAK);
  SetEPRxStatus(ENDP1, EP_RX_DIS);

  /* Set this device to response on default address */
  SetDeviceAddress(0);

  /* Set the new control state of the device to Attached */
  bDeviceState = ATTACHED;

  dfu_usb_reset();
}
/*******************************************************************************
* Function Name  : DFU_SetConfiguration.
* Description    : Update the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}
/*******************************************************************************
* Function Name  : DFU_SetConfiguration.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : DFU_Status_In.
* Description    : DFU status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_Status_In(void)
{
  DEVICE_INFO *pInfo = &Device_Info; // XXX same as pInformation ?

  if (pInfo->USBwIndex == KBD_INTERFACE){
    Kbd_Status_In();
    return;
  }
}

/*******************************************************************************
* Function Name  : DFU_Status_Out.
* Description    : DFU status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_Status_Out (void)
{
  DEVICE_INFO *pInfo = &Device_Info; // XXX same as pInformation ?

  if (pInfo->USBwIndex == KBD_INTERFACE){
    Kbd_Status_Out();
    return;
  }

}

/*******************************************************************************/
/*******************************************************************************/

void Kbd_Status_In(void)
{
    int req = Kbd_Request;

    if (likely(req == 0)){
	return;
    }

    Kbd_Request = 0;

    if (req == SET_REPORT){
	uint8_t leds = KbdSetReport[0];
	set_usb_kbd_leds(leds);
    }
}

void Kbd_Status_Out (void)
{

}

RESULT Kbd_Data_Setup(uint8_t RequestNo)
{
    // interface 0
    uint8_t *(*CopyRoutine)(uint16_t) = NULL;
    uint8_t descriptor;

    if ((RequestNo == GET_DESCRIPTOR) &&
	    (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))) {

	descriptor = pInformation->USBwValue1;

	switch (descriptor){
	    case REPORT_DESCRIPTOR_TYPE:
		CopyRoutine = Kbd_GetReportDescriptor;
		break;
	    case HID_DESCRIPTOR_TYPE:
		CopyRoutine = Kbd_GetHIDDescriptor;
		break;
	}

    } else if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)){

	switch (RequestNo){
	case GET_PROTOCOL:
	    CopyRoutine = Kbd_GetProtocolValue;
	    break;
	case GET_REPORT:
	    __disable_irq();
	    memcpy((void*)KbdGetReport, (void*)kbd_report_fullscan, KBD_SIZE);
	    __enable_irq();
	    CopyRoutine = Kbd_GetReport;
	    break;
	case GET_IDLE:
	    CopyRoutine = Kbd_GetIdleValue;
	    break;
	case SET_REPORT:
	    CopyRoutine = Kbd_SetReport;
	    Kbd_Request = SET_REPORT;
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

RESULT Kbd_NoData_Setup(uint8_t RequestNo)
{
    // interface 0

    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)){

	switch (RequestNo){
	    case SET_PROTOCOL:
		return Kbd_SetProtocol();
	    case SET_IDLE:
		return Kbd_SetIdle();
	}
    }
    return USB_UNSUPPORT;
}

uint8_t *Kbd_GetReport(uint16_t Length)
{
    if (Length == 0){
	pInformation->Ctrl_Info.Usb_wLength = KBD_SIZE;
	return NULL;
    }

    return (uint8_t *)KbdGetReport;
}

uint8_t *Kbd_SetReport(uint16_t Length)
{
    if (Length == 0){
	pInformation->Ctrl_Info.Usb_wLength = KBD_SIZE;
	return NULL;
    }

    return (uint8_t *)KbdSetReport;
}

uint8_t *Kbd_GetProtocolValue(uint16_t Length)
{
    if (Length == 0){
	pInformation->Ctrl_Info.Usb_wLength = 1;
	return NULL;
    }

    return (uint8_t *)(&ProtocolValue);
}

uint8_t *Kbd_GetIdleValue(uint16_t Length)
{
    if (Length == 0){
	pInformation->Ctrl_Info.Usb_wLength = 1;
	return NULL;
    }

    return (uint8_t *)(&ProtocolIdle);
}

RESULT Kbd_SetProtocol(void)
{
    uint8_t wValue0 = pInformation->USBwValue0;
    ProtocolValue = wValue0;
    return USB_SUCCESS;
}

RESULT Kbd_SetIdle(void)
{
    uint8_t duration = pInformation->USBwValue0;
    uint8_t report_id = pInformation->USBwValue1;

    if (report_id != 0)
	return USB_UNSUPPORT;

    if (ProtocolIdle != duration){
	ProtocolIdle = duration;
	set_usb_kbd_idle(ProtocolIdle * 4); /* duration units are 4ms */
    }

    return USB_SUCCESS;
}

uint8_t *Kbd_GetReportDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Kbd_Report_Descriptor);
}

uint8_t *Kbd_GetHIDDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Kbd_Hid_Descriptor);
}

/*******************************************************************************
* Function Name  : DFU_Data_Setup.
* Description    : Handle the data class specific requests.
* Input          : RequestNb.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT DFU_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);
  CopyRoutine = NULL;

  if (pInformation->USBwIndex == KBD_INTERFACE){ // XXX USBwIndex0 ?
    return Kbd_Data_Setup(RequestNo);
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);

  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : DFU_NoData_Setup.
* Description    : Handle the No data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT DFU_NoData_Setup(uint8_t RequestNo)
{
  if (pInformation->USBwIndex == KBD_INTERFACE){ // XXX USBwIndex0 ?
    return Kbd_NoData_Setup(RequestNo);
  }

  return USB_UNSUPPORT;

} /* End of DFU_NoData_Setup */

/*******************************************************************************
* Function Name  : DFU_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *DFU_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : DFU_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *DFU_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData (Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : DFU_GetStringDescriptor.
* Description    : Gets the string descriptors according to the needed index.
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *DFU_GetStringDescriptor(uint16_t Length)
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

/*******************************************************************************
* Function Name  : DFU_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT DFU_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > NUM_CONFIGURATIONS)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > NUM_INTERFACES)
  {
    return USB_UNSUPPORT;
  }

  return USB_SUCCESS;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
