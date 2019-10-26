// C99 Standard

#include <modbus/modbus-tcp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(void) {
	// Variable
	modbus_t *slave = NULL;
	modbus_mapping_t *localdata = NULL;
	int s = -1;

	// Initalize local data section
	localdata = modbus_mapping_new(MODBUS_MAX_READ_BITS, MODBUS_MAX_WRITE_BITS, MODBUS_MAX_READ_REGISTERS, MODBUS_MAX_WRITE_REGISTERS);
	if(localdata == NULL) {
		printf("Error occoured when mapping data section, Message: %s", modbus_strerror(errno));
		return -1;
	}

	// Create connection, Listening on all IPv4 addr
	slave = modbus_new_tcp_pi("0.0.0.0", "503");
	s = modbus_tcp_pi_listen(slave, 1);
	modbus_tcp_pi_accept(slave, &s);
	if(s == -1) {
		printf("Error occoured when creating socket. Message: %s", modbus_strerror(errno));
		return -1;
	}

	// Set debug mode
	modbus_set_debug(slave, TRUE);

	// Space for master query
	while(TRUE) {
		uint8_t query[MODBUS_MAX_ADU_LENGTH];
		memset(query, 0, MODBUS_MAX_ADU_LENGTH);

		// Try to receive query
		int rc = modbus_receive(slave, query);
		if(rc == -1) {
			printf("Message error or connection closed. Message: %s", modbus_strerror(errno));
			break;
		}

		// Reply query
		int rec = modbus_reply(slave, query, rc, localdata);
		if(rec == -1)
			printf("Failed to send reply. Message: %s", modbus_strerror(errno));
	}

	// Close Server
	close(s);
	modbus_mapping_free(localdata);
	modbus_close(slave);
	modbus_free(slave);
	return 0;
}
