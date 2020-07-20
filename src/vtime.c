#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NSEC	1000000000ul

unsigned long
arg_to_nsec(char *arg)
{
	char *s = strchr(arg, '.');
	unsigned long n, f;

	if (!s)
		return -1;
	s[0] = '\0';
	s++;
	n = atol(arg);
	f = atol(s);

	return n * NSEC + f;
}

#ifdef __aarch64__
unsigned long
get_time_cntvct(void)
{
	unsigned long cntvct, cntfrq;

	asm volatile("mrs %0, cntvct_el0" : "=r" (cntvct));
	asm volatile("mrs %0, cntfrq_el0" : "=r" (cntfrq));

	return cntvct * (NSEC / cntfrq);
}
#endif

unsigned long
get_time_realtime(void)
{
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	return spec.tv_sec * NSEC + spec.tv_nsec;
}

int
main(int argc, char *argv[])
{
	int c, real_time = 0, no_newline = 0;
	unsigned long offset = 0, t;

	while ((c = getopt(argc, argv, "hrno:")) != -1) {
		switch (c) {
		case 'h':
			printf("vtime [-rn] [-o offset]\n"
				"	-r	Use CLOCK_REALTIME instead\n"
				"		of cntvct_el0\n"
				"	-n	don't print a newline\n"
				"	-o	offset to subtract\n");
			exit(0);
		case 'r':
			real_time = 1;
			break;
		case 'n':
			no_newline = 1;
			break;
		case 'o':
			offset = arg_to_nsec(optarg);
			break;
		case '?':
			fprintf(stderr, "Unknown option: '-%c'", c);
			exit(1);
		}
	}

#ifdef __aarch64__
	t = (real_time ? get_time_realtime() : get_time_cntvct());
#else
	t = get_time_realtime();
#endif
	if (offset > t) {
		fprintf(stderr, "Bad offset argument\n");
		exit(1);
	}
	t -= offset;
	printf("%ld", t);
	//printf("%ld.%09ld", t / NSEC, t % NSEC);
	if (!no_newline)
		printf("\n");
	fflush(stdout);

	return 0;
}
