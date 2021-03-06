/**
  ******************************************************************************
  * @file    USB_Example/usb_desc.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Descriptors for Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

#include "usb_desc.h"

#define ONE_DESC_DATA(d) { .Descriptor = ((uint8_t*)d), .Descriptor_Size = sizeof(d) }

/* USB Standard Device Descriptor */
#define JOYSTICK_SIZ_DEVICE_DESC (18)
const uint8_t Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] = {
	JOYSTICK_SIZ_DEVICE_DESC,  /*bLength */
	USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
	0x00,                       /*bcdUSB */
	0x02,
	0x00,                       /*bDeviceClass*/
	0x00,                       /*bDeviceSubClass*/
	0x00,                       /*bDeviceProtocol*/
	0x40,                       /*bMaxPacketSize 64*/

	/* I'm cheating by using another vendor's idVendor and idProduct. This is
	 * because I need the multireport quirk enabled in Linux, and there doesn't
	 * seem to be a way to enable it generically. This VID/DID pair is for the
	 * GreenAsia Dual USB joypad device. The core USB HID driver in Linux sets
	 * HID_QUIRK_MULTI_REPORT with it sees this VID/UID pair.
	 *
	 * There's got to be a better way to do this without abusing the idVendor
	 * and idProduct fields */
	0x8f, 0x0e, /*idVendor (0x0e8f)*/
	0x13, 0x30, /*idProduct (0x3013)*/
	0x00, 0x01, /*bcdDevice rel. 1.00*/
	1,          /*Index of string descriptor describing manufacturer */
	2,          /*Index of string descriptor describing product*/
	3,          /*Index of string descriptor describing the device serial number */
	0x01        /*bNumConfigurations*/
};

ONE_DESCRIPTOR Device_Descriptor = ONE_DESC_DATA(Joystick_DeviceDescriptor);

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
#define JOYSTICK_SIZ_CONFIG_DESC  (34)
#define JOYSTICK_SIZ_HID_DESC     (9)
#define JOYSTICK_OFF_HID_DESC     (12)
#define JOYSTICK_SIZ_REPORT_DESC  ((44*NUM_JOYSTICKS)+52)

const uint8_t Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] = {
	0x09, /* bLength: Configuration Descriptor size */
	USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
	JOYSTICK_SIZ_CONFIG_DESC, 0x00, /* wTotalLength: Bytes returned */
	0x01,         /*bNumInterfaces: 1 interface*/
	0x01,         /*bConfigurationValue: Configuration value*/
	0x00,         /*iConfiguration: Index of string descriptor describing the configuration*/
	0xE0,         /*bmAttributes: bus powered */
	0xf0,         /*MaxPower: 480mA */

	/************** Interface Descriptor ****************/
	0x09,         /*bLength: Interface Descriptor size*/
	USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
	0x00,         /*bInterfaceNumber: Number of Interface*/
	0x00,         /*bAlternateSetting: Alternate setting*/
	0x01,         /*bNumEndpoints*/
	0x03,         /*bInterfaceClass: HID*/
	0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
	0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
	0,            /*iInterface: Index of string descriptor*/

	/******************** HID Descriptor ********************/
	JOYSTICK_SIZ_HID_DESC,         /*bLength: HID Descriptor size*/
	HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
	0x10, 0x01,   /*bcdHID: HID Class Spec release number v1.1 */
	0x00,         /*bCountryCode: Hardware target country*/
	0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
	0x22,         /*bDescriptorType*/
	JOYSTICK_SIZ_REPORT_DESC, 0x00, /*wItemLength: Total length of Report descriptor*/

	/******************** Endpoint Descriptor ********************/
	0x07,          /*bLength: Endpoint Descriptor size*/
	USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/
	0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
	0x03,          /*bmAttributes: Interrupt endpoint*/
	0x08, 0x00,    /*wMaxPacketSize: 8 Byte max */
	0x20,          /*bInterval: Polling Interval (32 ms)*/
};

ONE_DESCRIPTOR Config_Descriptor = ONE_DESC_DATA(Joystick_ConfigDescriptor);
ONE_DESCRIPTOR Mouse_Hid_Descriptor = {
	.Descriptor = (uint8_t *) Joystick_ConfigDescriptor + JOYSTICK_OFF_HID_DESC,
	.Descriptor_Size = JOYSTICK_SIZ_HID_DESC
};

const uint8_t Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] = {
#if (NUM_JOYSTICKS >= 1)
	0x05, 0x01,    /*Usage Page(Generic Desktop)*/
	0x09, 0x04,    /*Usage(Joystick)*/
	0xA1, 0x01,    /*Collection(Logical)*/
		0xA1, 0x00,    /*Collection(Physical)*/
			0x85, 0x01,    /* Report ID 1 */
			/* Buttons */
			0x05, 0x09,    /*Usage Page(Buttons)*/
			0x19, 0x01,    /*Usage Minimum(1)*/
			0x29, 0x08,    /*Usage Maximum(8)*/
			0x15, 0x00,    /*Logical Minimum(0)*/
			0x25, 0x01,    /*Logical Maximum(1)*/
			0x75, 0x01,    /*Report Size(1)*/
			0x95, 0x08,    /*Report Count(8)*/
			0x81, 0x02,    /*Input(Variable)*/
			/* X/Y Axis */
			0x05, 0x01,    /*Usage Page(Generic Desktop)*/
			0x09, 0x30,    /*Usage(X axis)*/
			0x09, 0x31,    /*Usage(Y axis)*/
			0x15, 0x80,    /*Logical Minimum(-128)*/
			0x25, 0x7f,    /*Logical Maximum(127)*/
			0x75, 0x08,    /*Report Size(8)*/
			0x95, 0x02,    /*Report Count(2)*/
			0x81, 0x02,    /*Input(Variable, Relative)*/
		0xC0,          /*End Collection*/
	0xC0,          /*End Collection*/
#endif

#if (NUM_JOYSTICKS >= 2)
	0x05, 0x01,    /*Usage Page(Generic Desktop)*/
	0x09, 0x04,    /*Usage(Joystick)*/
	0xA1, 0x01,    /*Collection(Logical)*/
		0xA1, 0x00,    /*Collection(Physical)*/
			0x85, 0x02,    /* Report ID 2 */
			/* Buttons */
			0x05, 0x09,    /*Usage Page(Buttons)*/
			0x19, 0x01,    /*Usage Minimum(1)*/
			0x29, 0x08,    /*Usage Maximum(8)*/
			0x15, 0x00,    /*Logical Minimum(0)*/
			0x25, 0x01,    /*Logical Maximum(1)*/
			0x75, 0x01,    /*Report Size(1)*/
			0x95, 0x08,    /*Report Count(8)*/
			0x81, 0x02,    /*Input(Variable)*/
			/* X/Y Axis */
			0x05, 0x01,    /*Usage Page(Generic Desktop)*/
			0x09, 0x30,    /*Usage(X axis)*/
			0x09, 0x31,    /*Usage(Y axis)*/
			0x15, 0x80,    /*Logical Minimum(-128)*/
			0x25, 0x7f,    /*Logical Maximum(127)*/
			0x75, 0x08,    /*Report Size(8)*/
			0x95, 0x02,    /*Report Count(2)*/
			0x81, 0x02,    /*Input(Variable, Relative)*/
		0xC0,          /*End Collection*/
	0xC0,          /*End Collection*/
#endif

#if (NUM_JOYSTICKS >= 3)
	0x05, 0x01,    /*Usage Page(Generic Desktop)*/
	0x09, 0x04,    /*Usage(Joystick)*/
	0xA1, 0x01,    /*Collection(Logical)*/
		0xA1, 0x00,    /*Collection(Physical)*/
			0x85, 0x03,    /* Report ID 3 */
			/* Buttons */
			0x05, 0x09,    /*Usage Page(Buttons)*/
			0x19, 0x01,    /*Usage Minimum(1)*/
			0x29, 0x08,    /*Usage Maximum(8)*/
			0x15, 0x00,    /*Logical Minimum(0)*/
			0x25, 0x01,    /*Logical Maximum(1)*/
			0x75, 0x01,    /*Report Size(1)*/
			0x95, 0x08,    /*Report Count(8)*/
			0x81, 0x02,    /*Input(Variable)*/
			/* X/Y Axis */
			0x05, 0x01,    /*Usage Page(Generic Desktop)*/
			0x09, 0x30,    /*Usage(X axis)*/
			0x09, 0x31,    /*Usage(Y axis)*/
			0x15, 0x80,    /*Logical Minimum(-128)*/
			0x25, 0x7f,    /*Logical Maximum(127)*/
			0x75, 0x08,    /*Report Size(8)*/
			0x95, 0x02,    /*Report Count(2)*/
			0x81, 0x02,    /*Input(Variable, Relative)*/
		0xC0,          /*End Collection*/
	0xC0,          /*End Collection*/
#endif

#if (NUM_JOYSTICKS >= 4)
	0x05, 0x01,    /*Usage Page(Generic Desktop)*/
	0x09, 0x04,    /*Usage(Joystick)*/
	0xA1, 0x01,    /*Collection(Logical)*/
		0xA1, 0x00,    /*Collection(Physical)*/
			0x85, 0x04,    /* Report ID 4 */
			/* Buttons */
			0x05, 0x09,    /*Usage Page(Buttons)*/
			0x19, 0x01,    /*Usage Minimum(1)*/
			0x29, 0x08,    /*Usage Maximum(8)*/
			0x15, 0x00,    /*Logical Minimum(0)*/
			0x25, 0x01,    /*Logical Maximum(1)*/
			0x75, 0x01,    /*Report Size(1)*/
			0x95, 0x08,    /*Report Count(8)*/
			0x81, 0x02,    /*Input(Variable)*/
			/* X/Y Axis */
			0x05, 0x01,    /*Usage Page(Generic Desktop)*/
			0x09, 0x30,    /*Usage(X axis)*/
			0x09, 0x31,    /*Usage(Y axis)*/
			0x15, 0x80,    /*Logical Minimum(-128)*/
			0x25, 0x7f,    /*Logical Maximum(127)*/
			0x75, 0x08,    /*Report Size(8)*/
			0x95, 0x02,    /*Report Count(2)*/
			0x81, 0x02,    /*Input(Variable, Relative)*/
		0xC0,          /*End Collection*/
	0xC0,          /*End Collection*/
#endif
	0x05, 0x01,    /*Usage Page(Generic Desktop)*/
	0x09, 0x02,    /*Usage(Mouse)*/
	0xA1, 0x01,    /*Collection(Logical)*/
		0x09, 0x01,    /*Usage(Pointer)*/
		0xA1, 0x00,    /*Collection(Physical)*/
			0x85, 0x05,    /* Report ID 5 */
			/* Buttons */
			0x05, 0x09,    /*Usage Page(Buttons)*/
			0x19, 0x01,    /*Usage Minimum(1)*/
			0x29, 0x03,    /*Usage Maximum(3)*/
			0x15, 0x00,    /*Logical Minimum(0)*/
			0x25, 0x01,    /*Logical Maximum(1)*/
			0x75, 0x01,    /*Report Size(1)*/
			0x95, 0x03,    /*Report Count(8)*/
			0x81, 0x02,    /*Input(Variable)*/
			/* NULL */
			0x75, 0x05,    /*Report Size(5)*/
			0x95, 0x01,    /*Report Count(1)*/
			0x81, 0x01,    /*Input(Constant)*/
			/* X/Y Axis */
			0x05, 0x01,    /*Usage Page(Generic Desktop)*/
			0x09, 0x30,    /*Usage(X axis)*/
			0x09, 0x31,    /*Usage(Y axis)*/
			0x15, 0x80,    /*Logical Minimum(-128)*/
			0x25, 0x7f,    /*Logical Maximum(127)*/
			0x75, 0x08,    /*Report Size(8)*/
			0x95, 0x02,    /*Report Count(2)*/
			0x81, 0x06,    /*Input(Variable, Relative)*/
		0xC0,          /*End Collection*/
	0xC0           /*End Collection*/
};

ONE_DESCRIPTOR Joystick_Report_Descriptor = ONE_DESC_DATA(Joystick_ReportDescriptor);

/* USB String Descriptors (optional) */
#define JOYSTICK_SIZ_STRING_LANGID  (4)
const uint8_t Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] = {
	JOYSTICK_SIZ_STRING_LANGID,
	USB_STRING_DESCRIPTOR_TYPE,
	0x09, 0x04 /* LangID = 0x0409: U.S. English */
};

#define JOYSTICK_SIZ_STRING_VENDOR  (20)
const uint8_t Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] = {
	JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
	USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
	/* Manufacturer: "SecretLab         " */
	'S', 0, 'e', 0, 'c', 0, 'r', 0, 'e', 0, 't', 0, 'L', 0, 'a', 0, 'b', 0,
};

#define JOYSTICK_SIZ_STRING_PRODUCT (30)
const uint8_t Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] = {
	JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
	USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
	'4', 0, 'P', 0, 'l', 0, 'y', 0, 'r', 0, ' ', 0, 'J', 0,
	'o', 0, 'y', 0, 's', 0, 't', 0, 'i', 0, 'c', 0, 'k', 0
};

#define JOYSTICK_SIZ_STRING_SERIAL  (26)
uint8_t Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] = {
	JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
	USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
	'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
};

ONE_DESCRIPTOR String_Descriptor[] = {
	ONE_DESC_DATA(Joystick_StringLangID),
	ONE_DESC_DATA(Joystick_StringVendor),
	ONE_DESC_DATA(Joystick_StringProduct),
	ONE_DESC_DATA(Joystick_StringSerial),
};

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
