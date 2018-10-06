#define swap(a, b) {typeof(a) c = a; a = b; b = c;}

unsigned int htonl(unsigned int host)
{
	char *p = (void *)(&host);

	swap(p[0], p[3]);
	swap(p[1], p[2]);

	return host;
}

unsigned short htons(unsigned short host)
{
	char *p = (void *)(&host);

	swap(p[0], p[1]);

	return host;
}

short cal_sum(unsigned short *buf, int len)
{
	unsigned int sum = 0;

	while (len > 1) {
		sum += *buf;
		buf++;
		len -= 2;
	}

	if (len == 1)
		sum += *(unsigned char *)buf;

	sum = (sum >> 16) + (sum & 0xffff);

	sum += (sum >> 16);

	return (~sum) & 0xffff;
}
