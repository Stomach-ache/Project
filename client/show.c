
void 
display(int pid, double cpu, double mem)
{
	puts("============================");
	puts("PID   %CPU   %MEM");
	printf("%d  %.2lf  %.2lf\n", pid, cpu, mem);
}
