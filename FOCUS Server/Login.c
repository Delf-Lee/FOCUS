#include "header.h"

unsigned char Login(const char *Id, const char *Pwd) {
	FILE *fp;
	int cycle;
	int i;
	char buf1[256] = {0,};
	char buf2[256];

	memcpy(buf1, Id, sizeof(char) * 64);
	memcpy(buf1 + 64, Pwd, sizeof(char) * 64);

	fp = fopen("DB.txt", "r");

	fscanf(fp, "%d", &cycle);

	for (i = 0; i < cycle; i++) {
		fscanf(fp, "%s", buf2);
		if (strcmp(buf1, buf2) == 0) {
			fclose(fp);
			return TRUE;
		}
	}
	fclose(fp);
	return TRUE;
}