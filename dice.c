#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
usage(void)
{
	(void)fprintf(stderr, "usage: dice #d# [...]\n");
	(void)fprintf(stderr, "example: dice 1d100 1d10 3d6 2d6\n");
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	if (argc < 2)
		usage();

	unsigned seed;
	if (getentropy(&seed, sizeof(seed)) == -1)
		err(EXIT_FAILURE, "getentropy");
	srandom(seed);

	long sum = 0;
	for (int i = 1; i < argc; i++) {
		size_t n, m;
		if (sscanf(argv[i], "%zu%*[dD]%zu", &n, &m) != 2) {
			warnx("I don't understand: %s", argv[i]);
			continue;
		}
		for (size_t j = 0; j < n; j++) {
			long dice;
			dice = 1 + random() % m;
			(void)printf("%s%ld", j > 0 ? " " : "", dice);
			sum += dice;
		}
		(void)printf("\n");
	}
	(void)printf("%ld\n", sum);

	return 0;
}
