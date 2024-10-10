#ifndef MIKAN_STACK_H
#define MIKAN_STACK_H	1

#include <stddef.h>

struct stack {
	size_t cur;
	size_t nmemb;
	size_t size;
	void *stack;
};
typedef struct stack STACK;

#define clearstack(st)	((void)((st)->cur = 0))
#define stackempty(st)	((st)->cur == 0)
#define stacksize(st)	((st)->cur)

void freestack(STACK *st);
struct stack *newstack(size_t size);
int peekstack(const STACK *restrict st, void *restrict elem);
int popstack(STACK *restrict st, void *restrict elem);
int pushstack(STACK *restrict st, const void *restrict elem);

#endif	/* !MIKAN_STACK_H */
