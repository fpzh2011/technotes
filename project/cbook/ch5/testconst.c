	#include <string.h>
	#include <stdio.h>
	
	int 
	main () {
		char s[50000] = "12345678901"; 
		memcpy(s+5000, s, 40970);
		printf("%s\n", s+5000);
		return 0;
	}
