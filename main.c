#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getCpu.h"
#include "getMem.h"
#include "show.c"
#include "unistd.h"
#include "send_messg.c"

typedef long long ll;
#define INTERVAL 3
#define DEBUGE 1

int main(){

	int cpu_num;
	cpu_num = get_cpu_num();
#ifdef DEBUGE
	printf("the number of cpu is: %d\n", cpu_num);
#endif

	ll totalCpuTime1, totalCpuTime2;
	ll processCpuTime1, processCpuTime2;
	int totalMem, processMem;
	double pcpu, pmem;
	int pid;
#ifdef DEBUGE
	printf("the number of cpu is: ", cpu_num);
#endif
	printf("please input pid: ");
	scanf("%d", &pid);

	while (1){
		processCpuTime1 = get_process_cpu(pid);
		totalCpuTime1 = get_total_cpu();

		processMem = get_process_mem(pid);
		totalMem = get_total_mem();

		sleep(INTERVAL);

		processCpuTime2 = get_process_cpu(pid);
		totalCpuTime2 = get_total_cpu();


		pcpu = cpu_num * 100.0 * (processCpuTime2 - processCpuTime1) / (totalCpuTime2 - totalCpuTime1);
		pmem = 100.0 * processMem / totalMem;

#if DEBUG
		display(pcpu, pmem);
#endif
		send_to_server(pcpu, pmem);
	}

	return 0;
}
