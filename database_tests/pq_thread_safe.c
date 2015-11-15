#include <stdio.h>
#include <libpq-fe.h>

//Notes: compile with "-lpq" flag

int main() {
	int ret = PQisthreadsafe();
	if (ret == 1) {
		printf("libpq is built thread-safe\n");
	} else {
		printf("libpq is not build thread-safe. Need to use locks.\n");
	}
	return 1;
}
