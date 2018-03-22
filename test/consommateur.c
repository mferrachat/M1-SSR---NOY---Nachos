#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int
main()
{
	char* msg;
	n_printf("Receiver\n");
	int n = TtyReceive(&msg, 256);
	if(n == 0)
		n_printf("TtyReceive error.\n");
	else
		n_printf("%i bytes received\n");
	return 0;
}