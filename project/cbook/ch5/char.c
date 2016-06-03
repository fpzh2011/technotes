//char.c gcc -S -masm=intel -m32 char.c
int 
f () {
	char c[6];
	
	c[0]++;
	c[1] = c[3];
	c[2] += 7;
	
	int i = c[5];
	
	c[4] = 'a';
	
	return 0;
}
