#include "net.h"
#include "lib.h"

#define NULL (void *)0

void delay(int n)
{
	volatile int i, j;

	for (i = 0; i < n; i++) {
		for (j = 0; j < 10000; j++) {
			;
		}
	}
}

/*求16的N次方*/
unsigned int exp16(unsigned int n)
{
	int i = 0, mul = 1;

	for (i = 0; i < n; i++) {
		mul *= 16;
	}

	return mul;
}

/*把字符串转换为整数*/
/*"12345678"----->0x12345678*/
unsigned int atoi(char *arr)
{
	unsigned sum = 0, i = 0, j = 0;

	while (*arr != '\0') {
		arr++;
		i++;
	}
	arr--;
	for (j = 0; j < i; j++) {
		sum += (*arr - '0') * exp16(j);
		arr--;
	}

	return sum;
}

unsigned int exp10(unsigned int n)
{
	int i = 0, mul = 1;

	for (i = 0; i < n; i++) {
		mul *= 10;
	}

	return mul;
}

/*把字符串转换为整数*/
/*"12345678"-----> 12345678*/
unsigned int atod(char *arr)
{
	unsigned sum = 0, i = 0, j = 0;

	while (*arr != '\0') {
		arr++;
		i++;
	}
	arr--;
	for (j = 0; j < i; j++) {
		sum += (*arr - '0') * exp10(j);
		arr--;
	}

	return sum;
}

char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */ ;
	return tmp;
}

char *strncpy(char *dest, const char *src, int count)
{
	char *tmp = dest;

	while (count-- && (*dest++ = *src++) != '\0')
		/* nothing */ ;

	return tmp;
}

char *strcat(char *dest, const char *src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0') ;

	return tmp;
}

char *strncat(char *dest, const char *src, int count)
{
	char *tmp = dest;

	if (count) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++)) {
			if (--count == 0) {
				*dest = '\0';
				break;
			}
		}
	}

	return tmp;
}

int strcmp(const char *cs, const char *ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

int strncmp(const char *cs, const char *ct, int count)
{
	register signed char __res = 0;

	while (count) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
		count--;
	}

	return __res;
}

char *strchr(const char *s, int c)
{
	for (; *s != (char)c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *)s;
}

int strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */ ;
	return sc - s;
}

int strnlen(const char *s, int count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */ ;
	return sc - s;
}

int strspn(const char *s, const char *accept)
{
	const char *p;
	const char *a;
	int count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return count;
		++count;
	}

	return count;
}

char *strpbrk(const char *cs, const char *ct)
{
	const char *sc1, *sc2;

	for (sc1 = cs; *sc1 != '\0'; ++sc1) {
		for (sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *)sc1;
		}
	}
	return NULL;
}

char *strsep(char **s, const char *ct)
{
	char *sbegin = *s;
	char *end;

	if (sbegin == NULL)
		return NULL;

	end = strpbrk(sbegin, ct);
	if (end)
		*end++ = '\0';
	*s = end;
	return sbegin;
}

void *memset(void *s, int c, int count)
{
	char *xs = (char *)s;

	while (count--)
		*xs++ = c;

	return s;
}

void *memcpy(void *dest, const void *src, int count)
{
	char *tmp = (char *)dest, *s = (char *)src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}

void *memmove(void *dest, const void *src, int count)
{
	char *tmp, *s;

	if (dest <= src) {
		tmp = (char *)dest;
		s = (char *)src;
		while (count--)
			*tmp++ = *s++;
	} else {
		tmp = (char *)dest + count;
		s = (char *)src + count;
		while (count--)
			*--tmp = *--s;
	}

	return dest;
}

int memcmp(const void *cs, const void *ct, int count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

void *memscan(void *addr, int c, int size)
{
	unsigned char *p = (unsigned char *)addr;

	while (size) {
		if (*p == c)
			return (void *)p;
		p++;
		size--;
	}
	return (void *)p;
}

char *strstr(const char *s1, const char *s2)
{
	int l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *)s1;
	l1 = strlen(s1);
	while (l1 >= l2) {
		l1--;
		if (!memcmp(s1, s2, l2))
			return (char *)s1;
		s1++;
	}
	return NULL;
}

void *memchr(const void *s, int c, int n)
{
	const unsigned char *p = s;
	while (n-- != 0) {
		if ((unsigned char)c == *p++) {
			return (void *)(p - 1);
		}
	}
	return NULL;
}
