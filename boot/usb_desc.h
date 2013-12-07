/**
  ******************************************************************************
  * @file    usb_desc.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptor Header for Device Firmware Upgrade (DFU)
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H
#include "platform_config.h"
#include "usb_defs.h"




#define DFU_ID_STRING_LANGID              0
#define DFU_ID_STRING_VENDOR              1
#define DFU_ID_STRING_PRODUCT             2
#define DFU_ID_STRING_SERIAL              3
#define DFU_ID_STRING_INTERFACE0          4




/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define DFU_SIZ_STRING_LANGID           4
#define DFU_SIZ_STRING_VENDOR           38
#define DFU_SIZ_STRING_PRODUCT          20
#define DFU_SIZ_STRING_SERIAL           26
#define DFU_SIZ_STRING_INTERFACE0       98    /* Flash Bank 0 */

extern  uint8_t DFU_StringSerial     [DFU_SIZ_STRING_SERIAL];

#define wTransferSize               0x0400   /* wTransferSize   = 1024 bytes */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/

struct ConfigDescriptorStruct {
    uint8_t	config[USB_CONFIGURATION_DESCRIPTOR_SIZE];

    /* basic keyboard */
    uint8_t	kbd_interface[USB_INTERFACE_DESCRIPTOR_SIZE];
    uint8_t	kbd_hid[HID_DESCRIPTOR_SIZE];
    uint8_t	kbd_ep[USB_ENDPOINT_DESCRIPTOR_SIZE];

    /* DFU */
    uint8_t	dfu_interface1[USB_INTERFACE_DESCRIPTOR_SIZE];
    uint8_t	dfu_func1[0x9];
} __attribute__((packed));

#endif /* __USB_DESC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

