#include<stdio.h>

void display(double cpu, double mem){
	puts("============================");
	puts("%CPU  %MEM");
	printf("%.2lf  %.2lf\n",cpu,mem);
}
