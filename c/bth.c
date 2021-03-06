#include "vstub.h"

//Bluetooth Configuration
typedef struct __attribute__ ((__packed__)) _CONFIG_BTH
{
	USB_CONFIGURATION_DESCRIPTOR	dev_conf;
	USB_INTERFACE_DESCRIPTOR	dev_int;
	USB_ENDPOINT_DESCRIPTOR		dev_ep1, dev_ep2, dev_ep3;
} CONFIG_BTH;

/* Device Descriptor */
const USB_DEVICE_DESCRIPTOR dev_dsc = {
	0x12,                   // Size of this descriptor in bytes
	0x01,                   // DEVICE descriptor type
	0x0200,                 // USB Spec Release Number in BCD format
	0xe0,                   // Class Code
	0x01,                   // Subclass code
	0x01,                   // Protocol code
	0x64,                   // Max packet size for EP0, see usb_config.h
	0x0b05,                 // Vendor ID
	0x1715,                 // Product ID: Mouse in a circle fw demo
	0x0100,                 // Device release number in BCD format
	0x01,                   // Manufacturer string index
	0x02,                   // Product string index
	0x00,                   // Device serial number string index
	0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
const char	configuration_bth[] = {
	0x09, 0x02, 0xd8, 0x00, 0x04, 0x01, 0x00, 0xa0,
	0x32, 0x09, 0x04, 0x00, 0x00, 0x03, 0xe0, 0x01, 
	0x01, 0x00, 0x07, 0x05, 0x81, 0x03, 0x10, 0x00, 
	0x01, 0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x01, 
	0x07, 0x05, 0x02, 0x02, 0x40, 0x00, 0x01, 0x09, 
	0x04, 0x01, 0x00, 0x02, 0xe0, 0x01, 0x01, 0x00, 
	0x07, 0x05, 0x83, 0x01, 0x00, 0x00, 0x01, 0x07, 
	0x05, 0x03, 0x01, 0x00, 0x00, 0x01, 0x09, 0x04, 
	0x01, 0x01, 0x02, 0xe0, 0x01, 0x01, 0x00, 0x07, 
	0x05, 0x83, 0x01, 0x09, 0x00, 0x01, 0x07, 0x05, 
	0x03, 0x01, 0x09, 0x00, 0x01, 0x09, 0x04, 0x01, 
	0x02, 0x02, 0xe0, 0x01, 0x01, 0x00, 0x07, 0x05, 
	0x83, 0x01, 0x11, 0x00, 0x01, 0x07, 0x05, 0x03, 
	0x01, 0x11, 0x00, 0x01, 0x09, 0x04, 0x01, 0x03, 
	0x02, 0xe0, 0x01, 0x01, 0x00, 0x07, 0x05, 0x83, 
	0x01, 0x19, 0x00, 0x01, 0x07, 0x05, 0x03, 0x01, 
	0x19, 0x00, 0x01, 0x09, 0x04, 0x01, 0x04, 0x02, 
	0xe0, 0x01, 0x01, 0x00, 0x07, 0x05, 0x83, 0x01, 
	0x21, 0x00, 0x01, 0x07, 0x05, 0x03, 0x01, 0x21, 
	0x00, 0x01, 0x09, 0x04, 0x01, 0x05, 0x02, 0xe0, 
	0x01, 0x01, 0x00, 0x07, 0x05, 0x83, 0x01, 0x31, 
	0x00, 0x01, 0x07, 0x05, 0x03, 0x01, 0x31, 0x00, 
	0x01, 0x09, 0x04, 0x02, 0x00, 0x02, 0xff, 0xff, 
	0xff, 0x00, 0x07, 0x05, 0x84, 0x02, 0x20, 0x00, 
	0x01, 0x07, 0x05, 0x04, 0x02, 0x20, 0x00, 0x01, 
	0x09, 0x04, 0x03, 0x00, 0x00, 0xfe, 0x01, 0x00, 
	0x00, 0x07, 0x21, 0x05, 0x88, 0x13, 0x40, 0x00
};

const char	*configuration = (char *)configuration_bth;
const USB_INTERFACE_DESCRIPTOR	*interfaces[] = { };
const unsigned char	*strings[] = {};
const USB_DEVICE_QUALIFIER_DESCRIPTOR	dev_qua = {};

typedef struct {
	char	data[100];
	unsigned	datalen;
} intr_reply_data_t;

static int	cnt_bulk_in;
static int	cnt_intr_in, cnt_intr_reply;
static USBIP_CMD_SUBMIT	*intr_cmd_submits[100];

static intr_reply_data_t	intr_reply_data[30] = {
	{ { 0x0e, 0x04, 0x01, 0x03, 0xc, 0x00 }, 6 },
	{ { 0x0e, 0x0a, 0x01, 0x09, 0x10, 0x00, 0x31, 0xe7, 0x0f, 0x72, 0x02, 0x00 }, 12 },
	{ { 0x0e, 0x44, 0x01, 0x02, 0x10, 0x00, 0xff, 0xff, 0xff, 0x03, 0xfe, 0xff,
	    0xcf, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xf2, 0x0f, 0xf8, 0xff, 0x3f, 0x00,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, 70 },
	{ { 0x0e, 0x0b, 0x01, 0x05, 0x10, 0x00, 0xf9, 0x03, 0x40, 0x08, 0x00, 0x00, 0x00}, 13 },
	{ { 0x0e, 0x0c, 0x01, 0x01, 0x10, 0x00, 0x03, 0x00, 0x40, 0x03, 0x0f, 0x00, 0x0e, 0x43 }, 14 },
	{ { 0x0e, 0x0c, 0x01, 0x03, 0x10, 0x00, 0xff, 0xff, 0x8d, 0xfe, 0x9b, 0xf9, 0x00, 0x80 }, 14 },
	{ { 0x0e, 0x04, 0x01, 0x57, 0x0c, 0x01 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x20, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x01, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x58, 0x0c, 0x01 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x18, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x1c, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x47, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x1e, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x43, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x45, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x24, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x33, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x13, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x1a, 0x0c, 0x00 }, 6 },
	{ { 0x0e, 0x04, 0x01, 0x24, 0x0c, 0x00 }, 6 },
};

void
handle_non_control_transfer(vstub_t *vstub, USBIP_CMD_SUBMIT *cmd_submit)
{
	if (cmd_submit->direction) {
		if (cmd_submit->ep == 2) {
			if (cnt_bulk_in < 3) {
				cnt_bulk_in++;
				printf("bulk: %d\n", cnt_bulk_in);
				return;
			}
		}
		else if (cmd_submit->ep == 1) {
			intr_cmd_submits[cnt_intr_in] = clone_cmd_submit(cmd_submit);
			cnt_intr_in++;
			printf("intr: %d\n", cnt_intr_in);
			return;
		}
	}
	printf("unhandled non-control\n");
}

void
handle_control_transfer(vstub_t *vstub, USBIP_CMD_SUBMIT *cmd_submit)
{
	setup_pkt_t	*setup_pkt = (setup_pkt_t *)cmd_submit->setup;

        if (setup_pkt->bmRequestType == 0x20 &&
	    setup_pkt->bRequest == 0x0 &&
	    setup_pkt->wValue.hiByte == 0x0) {
		char	*data;
		USBIP_CMD_SUBMIT	*cmd_reply;
		intr_reply_data_t	*reply_data;

		printf("CLASS command: %d\n", cnt_intr_reply);

		data = (char *)malloc(cmd_submit->transfer_buffer_length);
		if (!recv_data(vstub, data, cmd_submit->transfer_buffer_length)) {
			error("invalid format\n");
			free(data);
			return;
		}
		free(data);
		reply_cmd_submit(vstub, cmd_submit, NULL, 0);
		cmd_reply = intr_cmd_submits[cnt_intr_reply];
		reply_data = &intr_reply_data[cnt_intr_reply];
		reply_cmd_submit(vstub, cmd_reply, reply_data->data, reply_data->datalen);
		free(cmd_reply);
		cnt_intr_reply++;
	}
}

int
main(void)
{
	printf("bluetooth dongle started....\n");
	usbip_run(&dev_dsc);
}
