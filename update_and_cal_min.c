#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* the max number of clients */
#define MAXNUM 1000
#define inf 0x3f3f3f3f
#define rate 2

/* hold the value of cpu and mem of each client */
double cpu[MAXNUM], mem[MAXNUM], _free[MAXNUM];

int vis[MAXNUM];
char ip_table[MAXNUM][40];
/* goal_machine is the client to be returned and count holds the number of clients */
int flag = 0, goal_machine, count = 0;

void init();
void add_client();
int get_min();
int find_ip(char* client_ip);

char* update(char* buf, char* client_ip){
	if(0 == flag){
		init();
	}

	int key = find_ip(client_ip);

	double cur_cpu, cur_mem;
	sscanf(buf, "%lf %lf", &cur_cpu, &cur_mem);
	cpu[key] = cur_cpu;
	mem[key] = cur_mem;

	goal_machine = get_min();

	return ip_table[goal_machine];
}

void init(){
	flag = 1;
	/* initialize random seed */
	srand(time(NULL));

	add_client();
	
	memset(cpu, 0, sizeof(cpu));
	memset(mem, 0, sizeof(mem));

	return ;
}

int find_ip(char* client_ip){
	int i;
	for(i = 0; i < count; i++){
		if(strcmp(client_ip, ip_table[i]) == 0){
			return i;
		}
	}

	return 0;
}

void add_client(){

	strcpy(ip_table[count++], "192.168.135.20");
	strcpy(ip_table[count++], "192.168.135.51");

	return ;
}

int get_min(){
	memset(vis, 0, sizeof(vis));
	int top_3[3], k = 3, i;

	for (i = 0; i < count; i++){
		_free[i] = cpu[i]*rate + mem[i];
	}
	for (k = 0; k < 1; k++){
		double res = inf;
		int tmp = 0;
		for(i = 0; i < count; i++){
			if(vis[i]){
				continue;
			}
			if(res > _free[i]){
				res = _free[i];
				tmp = i;
			}
		}
		vis[tmp] = 1;
		top_3[k] = tmp;
	}

	return top_3[rand()%1];
}

