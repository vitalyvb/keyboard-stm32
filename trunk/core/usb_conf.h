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

#ifndef USB_CONF_H
#define USB_CONF_H

#define NUM_CONFIGURATIONS      (1)
#define NUM_INTERFACES          (5)
#define NUM_ENDPOINTS           (7)


#define KBD_INTERFACE           0
#define KBD_ENDPOINT            1
#define KBD_SIZE                8
#define KBD_REPORT_KEYS         (KBD_SIZE - 2)

#define DEBUG_INTERFACE_ACM     1  /* debug control endpoint, unused but required (?) */
#define DEBUG_ENDPOINT_ACM      5
#define DEBUG_SIZE_ACM          8
#define DEBUG_INTERFACE_DATA    2
#define DEBUG_ENDPOINT_DATA     6
#define DEBUG_SIZE_DATA         0x20

#define KBD_EXTRA_INTERFACE     3
#define KBD_EXTRA_ENDPOINT      4
#define KBD_EXTRA_SIZE          8

#define KBD_NKRO_INTERFACE      4
#define KBD_NKRO_ENDPOINT       5
#define KBD_NKRO_SIZE           0x10
#define KBD_NKRO_REPORT_KEYS    (KBD_NKRO_SIZE - 1)

#define USB_HID_REPORT_BUFFER_SIZE (0x10)

#if (KBD_SIZE>USB_HID_REPORT_BUFFER_SIZE) || \
    (KBD_EXTRA_SIZE>USB_HID_REPORT_BUFFER_SIZE) || \
    (KBD_NKRO_SIZE>USB_HID_REPORT_BUFFER_SIZE)
#error USB_HID_REPORT_BUFFER_SIZE must be increased.
#endif

/************************************************************/
/* Platform-specific configuration
 */

/* buffer table base address */
#define BTABLE_ADDRESS      (0x00)

/* EP0 - Control */
#define ENDP0_BUFSIZE       (0x40)
#define ENDP0_RXADDR        (0x80)
#define ENDP0_TXADDR        (0xC0)

/* EP1 - Keyboard  */
#define ENDP1_TXADDR        (0x100)	/* KBD_SIZE */
#define ENDP1_RXADDR        (0x108)	/* KBD_SIZE, not used */

/* EP2 - Keyboard Extra */
#define ENDP2_TXADDR        (0x110)	/* KBD_EXTRA_SIZE */
#define ENDP2_RXADDR        (0x118)	/* KBD_EXTRA_SIZE, not used */

/* EP3 - Keyboard NKRO */
#define ENDP3_TXADDR        (0x120)	/* KBD_NKRO_SIZE */
#define ENDP3_RXADDR        (0x130)	/* KBD_NKRO_SIZE, not used */

/* EP4 - Mouse */
#define ENDP4_TXADDR        (0x140)	/*  */
#define ENDP4_RXADDR        (0x148)	/*  */

/* EP5 - Debug, interrupt */
#define ENDP5_TXADDR        (0x150)	/* DEBUG_SIZE1 */
#define ENDP5_RXADDR        (0x158)	/* DEBUG_SIZE1, unused */
/* EP6 - Debug, bulk */
#define ENDP6_TXADDR        (0x160)	/* DEBUG_SIZE2 */
#define ENDP6_RXADDR        (0x180)	/* DEBUG_SIZE2 */

#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM /* | CNTR_ERRM */ | CNTR_SOFM \
                 | CNTR_ESOFM | CNTR_RESETM )

#endif /* USB_CONF_H */
