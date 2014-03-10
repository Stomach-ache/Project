#include<stdio.h>
#include<stdlib.h>
typedef long long LL;
//#define DEBUGE	1 

LL get_process_cpu(int pid){
	char buf[64];
	char stat;
	LL pstat[35];
	sprintf(buf,"/proc/%d/stat",pid);
	FILE* fin = fopen(buf,"r");
	if(fin == NULL){
		puts("open file error at get_process_cpu!");
		exit(0);
	}
	fscanf(fin,"%lld %s %c %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld",&pstat[0],buf,&stat,&pstat[3],&pstat[4],&pstat[5],&pstat[6],&pstat[7],&pstat[8],&pstat[9],&pstat[10],&pstat[11],&pstat[12],&pstat[13],&pstat[14],&pstat[15],&pstat[16],&pstat[17],&pstat[18],&pstat[19],&pstat[20],&pstat[21],&pstat[22],&pstat[24],&pstat[25],&pstat[26],&pstat[27],&pstat[28],&pstat[29],&pstat[30]);
	LL utime, stime, cutime, cstime;
	utime = pstat[13];
	stime = pstat[14];
	cutime = pstat[15];
	cstime = pstat[16];

	LL processCpuTime = utime + stime + cutime + cstime;

	fclose(fin);
	return processCpuTime;
}

LL get_total_cpu(LL *user, LL *sys){
	char buf[128];
	char cpu[5];
	FILE* fin;
	LL nice, idle, iowait, irq, softirq;
	fin = fopen("/proc/stat","r");
	if(fin == NULL){
		printf("open file error at get_total_cpu!");
		exit(0);
	}

	fgets(buf,sizeof(buf),fin);
	
#ifdef DEBUGE
	//printf("buf = %s\n",buf);
#endif
	sscanf(buf,"%s%lld%lld%lld%lld%lld%lld%lld",cpu, user,&nice, sys,&idle,&iowait,&irq,&softirq);
#ifdef DEBUGE
	printf("%s %lld %lld %lld %lld %lld %lld %lld\n",cpu,*user,nice,*sys,idle,iowait,irq,softirq);
#endif
	LL all;
	all = *user + nice + *sys + idle + iowait + irq + softirq;

	fclose(fin);
	return all;
}

int get_cpu_num(){
	char buf[1024];
	int cnt = 0;
	FILE* fin = fopen("/proc/cpuinfo","r");
	if(fin == NULL){
		puts("open file error at get_cpu_num!");
		exit(0);
	}
	while(fgets(buf,sizeof(buf),fin) != NULL){
		//buf[10] = '\0';
		//puts(buf);
		if((buf[0] == 'p') && (buf[1] == 'r')){
	//	if(strcmp(buf,"processor") == 0){
			cnt++;
		}
	}

	fclose(fin);
	return cnt;
}
