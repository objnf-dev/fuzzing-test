// C99 Standard

#include <modbus-tcp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(void) {
	// Variable
	modbus_t *slave = NULL;
	modbus_mapping_t *localData = NULL;
	int s = -1;

	// Initialize local data section
	localData = modbus_mapping_new(MODBUS_MAX_READ_BITS, MODBUS_MAX_WRITE_BITS, MODBUS_MAX_READ_REGISTERS, MODBUS_MAX_WRITE_REGISTERS);
	if(localData == NULL) {
		printf("Error occurred when mapping data section, Message: %s", modbus_strerror(errno));
		return -1;
	}

	// Create connection, Listening on all IPv4 address
	slave = modbus_new_tcp_pi("0.0.0.0", "502");
	s = modbus_tcp_pi_listen(slave, 1);
	modbus_tcp_pi_accept(slave, &s);
	if(s == -1) {
		printf("Error occurred when creating socket. Message: %s", modbus_strerror(errno));
		return -1;
	}

	// Set debug mode
	modbus_set_debug(slave, TRUE);

	// Space for master query
	uint8_t query[MODBUS_MAX_ADU_LENGTH];
	memset(query, 0, MODBUS_MAX_ADU_LENGTH);

	// Try to receive query
	int rc = -1;
	rc = modbus_receive(slave, query);
	if(rc > 0) {
		const int returnCode = modbus_reply(slave, query, rc, localData);
		if(returnCode == -1)
			printf("Failed to send reply. Message: %s", modbus_strerror(errno));
	} else if(rc == -1)
		printf("Connection maybe closed. Message: %s", modbus_strerror(errno));

	// Close Server
	closesocket(s);
	modbus_mapping_free(localData);
	modbus_close(slave);
	modbus_free(slave);
	return 0;
}
