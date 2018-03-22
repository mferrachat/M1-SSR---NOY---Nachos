#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int
main()
{
	/*
	SemId sem1 = SemCreate("sem1", 5);
	SemId sem2 = SemCreate("sem2", 0);

	producer();
	consumer();
	*/
	ThreadId thread1 = Exec("/hello");
	Join(thread1);
	return 0;
}
/*
void producer(){
	while(1){
		n_printf("Produce\n");
		sem1.P();
		n_printf("Put Into Buffer\n");
		sem2.V();
	}
}
void consumer(){
	while(1){
		sem2.P();
		n_printf("Remove From Buffer\n");
		sem1.V();
		n_printf("Consume\n");
	}
}
*/

