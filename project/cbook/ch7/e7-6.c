/* 比较两个文件并打印它们第一个不相同的行。 */
//支持超长行，但超长行差异之前的部分可能无法打印。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 10

void 
print (const char *f1, const char *f2) {
	char buf1[BUFLEN], buf2[BUFLEN], *s1, *s2;
	FILE *fp1, *fp2;
	int n1, n2, linenum = 1;

	if((fp1 = fopen(f1,"r")) == NULL) {
		fprintf(stderr, "%s open failed\n", f1);
		exit(2);
	}
	if((fp2 = fopen(f2,"r")) == NULL) {
		fprintf(stderr, "%s open failed\n", f2);
		exit(2);
	}

	do {
		s1 = fgets(buf1, BUFLEN, fp1);
		s2 = fgets(buf2, BUFLEN, fp2);
		if(s1[strlen(s1)-1] == '\n') {
			linenum++;
		}
	} while (s1 != NULL && s2 != NULL && strcmp(s1,s2) == 0);

	if(s1 == NULL && s2 == NULL) {
		printf("the same\n");
	}
	else {
		printf("%d\n1:%s\n2:%s\n", linenum, s1 == NULL ? "":s1, s2 == NULL ? "":s2);
	}

	fclose(fp1);
	fclose(fp2);
}


int 
main (int argc, char *argv[]) {
	if(argc < 3) {
		fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
		return 1;
	}
	print(argv[1], argv[2]);
}
