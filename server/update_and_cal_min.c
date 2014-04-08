#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
/*
 *	平衡策略：如果最大权值和最小权值之差小于设定的数值，则继续返回前一个
 *	client, 否则返回权值最小的client
 */

/*
 * 对于每个client首先检查是否为新添加的, 若是则添加到本地ip_table中
 */

/*
 * 如果某个ip_table中的client在设定的时间间隔内没有向server发送数据，则认为
 * 该client与server断开连接
 */

/* the max number of clients */
#define MAXNUM 1000
#define inf 0x3f3f3f3f
#define rate 2
/* 最大差值 */
#define MAX_DIFFERENCE  20
#define TIME_INTERVAL 10

struct node {
		char ip[20];
		double cpu, mem, free;
		int connect_ok;
		time_t last_time;

}ip_table[MAXNUM];

/* goal_machine is the client to be returned and count holds the number of clients */
int flag = 0, goal_machine, count = 0;

void check_new_ip(char *);
void add_client(char *);
void get_min();
int find_ip(char* client_ip);
void show();
void is_connect();

struct node previous;

//更新数据，并返回最优的client
void update(char* buf, char* client_ip, char *ans){
	double cur_cpu, cur_mem;
	time_t cur_time;
	sscanf(buf, "%lf %lf %ld", &cur_cpu, &cur_mem, &cur_time);

	//判断是否为新的client
	check_new_ip(client_ip);

	//是否第一次调用update函数
	if (flag == 0) {
			previous = ip_table[0];
			flag = 1;
	}

	//找到此次client对应的index
	int key = find_ip(client_ip);

	//更新对应client信息
	time_t rawtime;
	time(&rawtime);
	ip_table[key].last_time = rawtime;
	ip_table[key].cpu = cur_cpu;
	ip_table[key].mem = cur_mem;

	get_min();

	//显示当前ip_table中连接状态良好的client信息
	show();

	struct node first;
	int f = 0, i;
	/* 返回结果 */
	ans[0] = '\0';
	for (i = 0; i < count; i++) {
		if (ip_table[i].connect_ok == 0) continue;

		if (f == 0) first = ip_table[i];
		f = 1;

		if (strcmp(previous.ip, ip_table[i].ip) == 0) {
			if (ip_table[i].free - first.free <= MAX_DIFFERENCE + 0.0) {
				strncpy(ans, ip_table[i].ip, 20);
				return ;
			}
			previous = first;
			strncpy(ans, first.ip, 20);
			return ;
		}
	}
	if (f != 0)
			strncpy(ans, first.ip, 20);
	else {
		char *tmp = "no client!";
		strncpy(ans, tmp, 20);
	}

	return ;
}

//对已经断开连接的client作出判断
void is_connected() {
	time_t rawtime;
	time(&rawtime);
	int i;
	for (i = 0; i < count; i++) {
			if (ip_table[i].connect_ok && rawtime - ip_table[i].last_time >= TIME_INTERVAL) { 
					ip_table[i].connect_ok = 0;
					printf("\033[01;41m %s is dead! \033[0m\n", ip_table[i].ip); 
			}
	}

	return ;
}

//显示当前ip_table中所有client的信息
void show() {
	int i;
	for (i = 0; i < count; i++) {
		if (ip_table[i].connect_ok == 0) continue;

		printf("%s %lf %lf %ld\n", ip_table[i].ip, ip_table[i].cpu, ip_table[i].mem, ip_table[i].last_time);
	}

	return ;
}

//动态加入client
void check_new_ip(char *ip_addr) {
		int i;
		for (i = 0; i < count; i++) {
				if (strcmp(ip_table[i].ip, ip_addr) == 0) {
						if (ip_table[i].connect_ok == 0)
								ip_table[i].connect_ok = 1;
						return ;
				}
		}
		add_client(ip_addr);

		return ;
}

int find_ip(char* client_ip){
	int i;
	for(i = 0; i < count; i++){
		if(strcmp(client_ip, ip_table[i].ip) == 0){
			return i;
		}
	}

	return 0;
}

void add_client(char *ip_addr) {
	
	printf("\033[01;34m a new client: %s\033[0m\n", ip_addr); 

	strncpy(ip_table[count].ip, ip_addr, 20);
	ip_table[count++].connect_ok = 1;

	return ;
}

int cmp(const void *x, const void *y) {
		return ((struct node*)x)->free - ((struct node*)y)->free;
}

void get_min(){
	//memset(vis, 0, sizeof(vis));

	int i;

	for (i = 0; i < count; i++){
		ip_table[i].free = ip_table[i].cpu * rate + ip_table[i].mem;
	}
	qsort(ip_table, count, sizeof(struct node), cmp);

	return ;
}

