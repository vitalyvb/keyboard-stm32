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

#ifndef USB_DEFS_H
#define USB_DEFS_H

#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_DEVICE_DESCRIPTOR_SIZE              0x12
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_CONFIGURATION_DESCRIPTOR_SIZE       0x09
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_INTERFACE_DESCRIPTOR_SIZE           0x09
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05
#define USB_ENDPOINT_DESCRIPTOR_SIZE            0x07

#define HID_DESCRIPTOR_TYPE                     0x21
#define HID_DESCRIPTOR_SIZE                     0x09

#define REPORT_DESCRIPTOR_TYPE                  0x22

/* report id */
#define REPORT_ID_MOUSE     1
#define REPORT_ID_SYSTEM    2
#define REPORT_ID_CONSUMER  3

#endif /* USB_DEFS_H */

