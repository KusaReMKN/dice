#include <sys/random.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "stack.h"

static void operate(STACK *st, const char *op);
static void usage(void);

int
main(int argc, char *argv[])
{
	if (argc < 2)
		usage();

	unsigned seed;
	if (getentropy(&seed, sizeof(seed)) == -1)
		err(EXIT_FAILURE, "getentropy");
	srandom(seed);

	STACK *st;
	st = newstack(sizeof(size_t));
	if (st == NULL)
		err(EXIT_FAILURE, "newstack");

	for (int i = 1; i < argc; i++) {
		size_t n, m;
		size_t sum = 0;
		switch (sscanf(argv[i], "%zu%*[dD]%zu", &n, &m)) {
		case 2:
			for (size_t j = 0; j < n; j++) {
				size_t dice;
				dice = 1 + random() % m;
				(void)printf("%s%zu", j > 0 ? " " : "", dice);
				sum += dice;
			}
			(void)printf(" => %zu\n", sum);
			if (pushstack(st, &sum) == -1)
				err(EXIT_FAILURE, "pushstack");
			break;
		case 1:
			(void)printf("%zu\n", n);
			if (pushstack(st, &n) == -1)
				err(EXIT_FAILURE, "pushstack");
			break;
		case 0:
			operate(st, argv[i]);
			break;
		default:
			err(EXIT_FAILURE, "sscanf");
		}
	}

	return 0;
}

static void
operate(STACK *st, const char *op)
{
	if (op[1] != '\0')
		goto unknown;

	size_t a, b, sum;
	switch (op[0]) {
	case '+':
		if (stacksize(st) < 2) {
			warnx("%s: stack empty", op);
			break;
		}
		(void)popstack(st, &b);
		(void)popstack(st, &a);
		a += b;
		(void)printf("%zu\n", a);
		(void)pushstack(st, &a);
		break;
	case '-':
		if (stacksize(st) < 2) {
			warnx("%s: stack empty", op);
			break;
		}
		(void)popstack(st, &b);
		(void)popstack(st, &a);
		a -= b;
		(void)printf("%zu\n", a);
		(void)pushstack(st, &a);
		break;
	case '*':
	case 'x':
	case 'X':
		if (stacksize(st) < 2) {
			warnx("%s: stack empty", op);
			break;
		}
		(void)popstack(st, &b);
		(void)popstack(st, &a);
		a *= b;
		(void)printf("%zu\n", a);
		(void)pushstack(st, &a);
		break;
	case '/':
		if (stacksize(st) < 2) {
			warnx("%s: stack empty", op);
			break;
		}
		(void)popstack(st, &b);
		(void)popstack(st, &a);
		a /= b;
		(void)printf("%zu\n", a);
		(void)pushstack(st, &a);
		break;
	case '%':
		if (stacksize(st) < 2) {
			warnx("%s: stack empty", op);
			break;
		}
		(void)popstack(st, &b);
		(void)popstack(st, &a);
		a %= b;
		(void)printf("%zu\n", a);
		(void)pushstack(st, &a);
		break;
	case 'd':
		if (stacksize(st) < 2) {
			warnx("%s: stack empty", op);
			break;
		}
		(void)popstack(st, &b);
		(void)popstack(st, &a);
		sum = 0;
		for (size_t i = 0; i < a; i++) {
			size_t dice;
			dice = 1 + random() % b;
			(void)printf("%s%zu", i > 0 ? " " : "", dice);
			sum += dice;
		}
		(void)printf(" => %zu\n", sum);
		(void)pushstack(st, &sum);;
		break;
	default:
unknown:
		warnx("%s: unknown operator", op);
	}
}

static void
usage(void)
{
	(void)fprintf(stderr, "usage: dice #d# [...]\n");
	(void)fprintf(stderr, "example: dice 1d100 1d10 3d6 2d6\n");
	exit(EXIT_FAILURE);
}
