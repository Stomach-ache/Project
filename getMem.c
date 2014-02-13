#include<stdio.h>
#include<stdlib.h>

int get_process_mem(int pid){
	char buf[1024];
	sprintf(buf,"/proc/%d/statm",pid);
	FILE* fin = fopen(buf,"r");
	buf[0] = '\0';
	if(fin == NULL){
		puts("open file error!");
		exit(0);
	}

	int size, resident, shared, trs, lrs, drs, dt;
	fgets(buf,sizeof(buf),fin);
	sscanf(buf,"%d %d %d %d %d %d %d",&size,&resident,&shared,&trs,&lrs,&drs,&dt);
	int vmrss = resident * 4;

	fclose(fin);
	return vmrss;
}

int get_total_mem(){
	char buf[1024];
	FILE* fin = fopen("/proc/meminfo","r");
	if(fin == NULL){
		puts("open file error!");
		exit(0);
	}

	fgets(buf,sizeof(buf),fin);
	char str[20],str2[20];
	int TotalMem;
	sscanf(buf,"%s%d%s",str,&TotalMem,str2);

	fclose(fin);
	return TotalMem;
}
