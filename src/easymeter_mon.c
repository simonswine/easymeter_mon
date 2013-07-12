// Copyright 2011 Juri Glass, Mathias Runge, Nadim El Sayed
// DAI-Labor, TU-Berlin
//
// This file is part of libSML.
//
// libSML is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libSML is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libSML.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>

#include <sml/sml_file.h>
#include <sml/sml_transport.h>
#include <sml/sml_message.h>
#include <sml/sml_open_response.h>
#include <sml/sml_value.h>

#include "obis.h"
#include "common.h"

#define SML_BUFFER_LEN 8096

/* connect to serial device */
int serial_port_open(const char* device) {
	int bits;
	struct termios config;
	memset(&config, 0, sizeof(config));

	int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd < 0) {
		printf("error: open(%s): %s\n", device, strerror(errno));
		return -1;
	}

	// set RTS
	ioctl(fd, TIOCMGET, &bits);
	bits |= TIOCM_RTS;
	ioctl(fd, TIOCMSET, &bits);

	tcgetattr(fd, &config);

	// set 8-N-1
	config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR
			| ICRNL | IXON);
	config.c_oflag &= ~OPOST;
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	config.c_cflag &= ~(CSIZE | PARENB | PARODD | CSTOPB);
	config.c_cflag |= CS8;

	// set speed to 9600 baud
	cfsetispeed(&config, B9600);
	cfsetospeed(&config, B9600);

	tcsetattr(fd, TCSANOW, &config);
	return fd;
}

/* receive sml pdus */
void transport_receiver(unsigned char *buffer, size_t buffer_len) {
	// the buffer contains the whole message, with transport escape sequences.
	// these escape sequences are stripped here.

	sml_file *file;
	sml_get_list_response *body;
	sml_list *entry;

	file = sml_file_parse(buffer + 8, buffer_len - 16);
	// the sml file is parsed now

	/* obtain SML messagebody of type getResponseList */
	int i;
	for (i = 0; i < file->messages_len; i++) {
		sml_message *message = file->messages[i];

		if (*message->message_body->tag == SML_MESSAGE_GET_LIST_RESPONSE) {
			body = (sml_get_list_response *) message->message_body->data;
			entry = body->val_list;

			/* Loop over values */
			while (entry->next != NULL ) {

				/* get value obis */
				obis_id_t obis_id;
				obis_init(&obis_id, entry->obj_name->str);
				char obis_name[OBIS_STR_LEN];
				obis_unparse(obis_id, (char *) &obis_name, OBIS_STR_LEN);

				/* get values name */
				obis_alias_t * obis_alias = obis_get_alias(&obis_id);
				if (obis_alias != NULL ) {

					/* get scale */
					int scaler = (entry->scaler) ? *entry->scaler : 1;
					double value = sml_value_to_double(entry->value)
							* pow(10, scaler);

					printf("%s power.%s %.2f\n", "powermon.home.swine.de",
							obis_alias->name, value);

				}

				/* next entry */
				entry = entry->next;

			}

		}
	}

	// free the malloc'd memory
	sml_file_free(file);
}

int main(int argc, char **argv) {

	unsigned char buffer[SML_BUFFER_LEN];
	size_t bytes;

	// open serial device. Adjust as needed.
	char *device = "/dev/ttyUSB0";
	int fd = serial_port_open(device);

	if (fd > 0) {
		// listen on the serial device, this call is blocking.
		bytes = sml_transport_read(fd, (unsigned char *) &buffer, SML_BUFFER_LEN);
		if (bytes > 0) {
			transport_receiver((unsigned char *)&buffer, bytes);
			return 0;
		}
		close(fd);
	}
}

