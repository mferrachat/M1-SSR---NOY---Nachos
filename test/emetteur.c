#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int
main()
{
	n_printf("Sender\n");
	int n = TtySend("Testing lighting equipment: LED screens. Testing blinders. Testing lasers. Testing 3D video. Testing pyro-technics. Stand by.\n")
	if(n == 0)
		n_printf("TtySend error.\n");
	else
		n_printf("%i bytes sent\n");
	return 0;
}