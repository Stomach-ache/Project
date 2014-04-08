#include <stdio.h>
#include <time.h>

#define DEBUGE 0

time_t
get_cur_time()
{
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	#ifdef DEBUGE
		printf("\007The current date/time is: %s", asctime(timeinfo));
	#endif

	return rawtime;
}
