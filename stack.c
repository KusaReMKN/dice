#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

static int extendstack(STACK *st);

static int
extendstack(STACK *st)
{
	void *tmp;

#define EXTENDED_NMEMB	16
	tmp = realloc(st->stack, st->size * (st->nmemb + EXTENDED_NMEMB));
	if (tmp == NULL)
		return -1;
	st->nmemb += EXTENDED_NMEMB;
	st->stack = tmp;

	return 0;
}

void
freestack(STACK *st)
{
	free(st->stack);
	free(st);
}

STACK *
newstack(size_t size)
{
	STACK *st;
	int error;

	st = malloc(sizeof(*st));
	if (st == NULL)
		return NULL;

#define INITIAL_NMEMB	16
	st->cur = 0;
	st->nmemb = INITIAL_NMEMB;
	st->size = size;
	st->stack = malloc(st->size * st->nmemb);
	if (st->stack == NULL) {
		error = errno;
		free(st);
		errno = error;
		return NULL;
	}

	return st;
}

int
peekstack(const STACK *restrict st, void *restrict elem)
{
	if (st->cur < 1) {
		/* TODO: set errno */
		return -1;
	}
	if (elem != NULL)
		memcpy(elem, &st->stack[st->size * (st->cur - 1)], st->size);

	return 0;
}

int
popstack(STACK *restrict st, void *restrict elem)
{
	if (peekstack(st, elem) == -1)
		return -1;
	st->cur--;

	return 0;
}

int
pushstack(STACK *restrict st, const void *restrict elem)
{
	if (st->cur == st->nmemb)
		if (extendstack(st) == -1)
			return -1;
	memcpy(&st->stack[st->size * st->cur++], elem, st->size);

	return 0;
}
