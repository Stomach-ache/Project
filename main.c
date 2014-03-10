#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getCpu.h"
#include "getMem.h"
#include "show.c"
#include "unistd.h"
#include "send_messg.c"

#define INTERVAL 3
#define DEBUGE 1

int 
main(int argc, char **argv)
{

	int cpu_num;
	cpu_num = get_cpu_num();

	printf("the number of cpu is: %d\n", cpu_num);

	LL totalCpuTime1, totalCpuTime2;
	LL processCpuTime1, processCpuTime2;
	LL user1, user2, sys1, sys2;
	int totalMem, processMem;
	int memfree, buffers, cached;
	double pcpu, pmem, total_cpu, total_mem;
	int pid;
	//printf("please input pid: ");
	//scanf("%d", &pid);


	while ( 1 ) {
		if ( 1 != argc ) {
				sscanf(argv[1], "%d", &pid);
				processCpuTime1 = get_process_cpu(pid);
				processMem = get_process_mem(pid);
		}
		totalCpuTime1 = get_total_cpu(&user1, &sys1);

		totalMem = get_total_mem(&memfree, &buffers, &cached);

		sleep(INTERVAL);

		totalCpuTime2 = get_total_cpu(&user2, &sys2);

		if ( 1 != argc ) {

				processCpuTime2 = get_process_cpu(pid);
				pcpu = cpu_num * 100.0 * (processCpuTime2 - processCpuTime1) / (totalCpuTime2 - totalCpuTime1);
				pmem = 100.0 * processMem / totalMem;
 				#ifdef DEBUGE
					display(pid, pcpu, pmem);
				#endif
		}

		total_cpu = (0.0 + (user2 - user1 + sys2 - sys1)) / (totalCpuTime2 - totalCpuTime1) * 100 * cpu_num;
		total_mem = (0.0 + totalMem - memfree - buffers - cached) / totalMem * 100;
#ifdef DEBUGE
		printf("====================\n");
		printf("total cpu: %lf%%\n", total_cpu);
		printf("total mem: %lf%%\n", total_mem);
#endif

		/* send_to_server(pcpu, pmem); */
	}

	return 0;
}
