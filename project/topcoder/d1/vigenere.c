/*
Vigenere加密是一个字符替换加密算法。

给定一个字符替换表如：
    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z

A   A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
B   B C D E F G H I J K L M N O P Q R S T U V W X Y Z A 
C   C D E F G H I J K L M N O P Q R S T U V W X Y Z A B
D   D E F G H I J K L M N O P Q R S T U V W X Y Z A B C 

加密、解密都需要指定code word，如"CODE"。在下面的例子中，给定code word和Encoded，加密过程如下：
1 将code word重复覆盖Message各个字符
2 对每一个Message字符，在上面的字符转换二维表格中查找对应加密字符：以code word字符为行，以Message字符为列，查到的字符作为Encoded字符

给定code word和Encoded，解密过程如下：
1 将code word重复覆盖Encoded各个字符
2 对每个Encoded字符，在上面的字符转换二维表格中查找对应解密字符：找到密文字符对应的code word字符，在该code word字符对应行中，查找该Encoded字符，找到的列就是Message字符

Code Word:
               CODECODECODECOD
Message:
               TOPCODERISGREAT
Encoded:
               VCSGQRHVKGJVGOW


The Vigenere Cipher is a simple but strong encryption algorithm.  Unlike many
simple encryption algorithms, the Vigenere Cipher is immune to frequency
analysis because it alters the normal frequency by using more than one alphabet
to encrypt the message.  Instead of there being a one-to-one relationship
between each letter and its substitute, there is a one-to-many relationship
between each letter and its substitute.

The Vigenere Cipher is based on the following table (which has a simple pattern
so you don't actually have to store the table).

    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z

A   A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
B   B C D E F G H I J K L M N O P Q R S T U V W X Y Z A 
C   C D E F G H I J K L M N O P Q R S T U V W X Y Z A B
D   D E F G H I J K L M N O P Q R S T U V W X Y Z A B C 
.           .           .            .               .
.           .           .            .               .
.           .           .            .               .

When encoding a message with the Vigenere Cipher, a code word is written
repeatedly over the message.  Each letter in the message is replaced with the
letter at the intersection of the row given by the corresponding code word
letter and the column given by the message letter.

Implement a class Vigenere that contains a method coder.  The method encodes
and decodes Strings of capital letters (A-Z) using the Vigenere Cipher.  coder
takes two Strings and an int as a parameter.  The first String is the message.
The second String is the code word.  The int specifies whether the message
should be encoded or decoded:
int=1: encode message
int=2: decode message
The method returns a String that is the resulting (encoded or decoded) message.

If the message or code word is not valid (They do not contain only capital
letters in the range A to Z, or the code word is of length 0) the method
returns "ERROR"
If the int is not 1 or 2, the method returns "ERROR"
If both the message and code word are of length 0, or if just the message has
length 0 (and the code word is valid), the method should return an empty string.

The method signature is:
public String coder(String message,String codeWord,int action); 

message and codeWord are Strings with length less than 1000.

Examples:
If the message is "TOPCODERISGREAT" and the codeWord is "CODE" and the action
is encode (1), write the code word over the message and look up letters in the
table:
Code Word:
               CODECODECODECOD  
Message:
               TOPCODERISGREAT
Encoded:
               VCSGQRHVKGJVGOW
because the letter in row C column T is V,
the letter in row O column O is C,
etc....

If the message is "HRWCQYRORWSCGKUO" and the codeWord is "OK" and the action is
decode (2), the decoded message is "THISCODEDMESSAGE".
If the message is "HowAreYou" and the codeWord is "GO" and the action is encode
(1), the method returns "ERROR" because the message isn't all capital letters.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 100
#define NUM_TO_STRING(x) #x
#define NUM_TO_S(x) NUM_TO_STRING(x)

//如果s都是大写字符或者空串，返回1；否则返回0。
static int
validate (const char *s) {
	if(NULL == s) {
		return 1;
	}
	for(; *s != '\0' && isupper(*s); s++)
		;
	return *s == '\0';
}

/*
 * 如果codeword长度小于message，新建一个与message等长的缓冲区，将codeword重复覆盖新缓冲区，返回新缓冲区；
 * 如果无法申请缓冲区，返回NULL；
 * 否则，返回codeword。
 */
static char* 
expand_str (const char *codeword, const char *message) {
	if(NULL == codeword || NULL == message) {
		return NULL;
	}
	size_t mlen = strlen(message);
	size_t clen = strlen(codeword);
	if(clen >= mlen) {
		return (char*)codeword;
	}
	char *p, *codeword2 = malloc(mlen+1);
	if(NULL == codeword2) {
		return NULL;
	}
	size_t m = mlen / clen;
	for(p = codeword2; m > 0; m--, p += clen) {
		memcpy(p, codeword, clen);
	}
	if(m = mlen % clen) {
		memcpy(p, codeword, m);
	}
	codeword2[mlen] = '\0';
	return codeword2;
}

static char t[26][27] = {'\0'};

static void 
init_table () {
	int i, j, c;
	for(i = 0; i < 26; i++) {
		for(j = 0; j < 26-i; j++) {
			t[i][j] = 'A' + i + j;
		}
		c = 'A';
		for(; j < 26; j++) {
			t[i][j] = c++;
		}
		t[i][26] = '\0';
	}
}

/*
 * 如果message为空串，返回""。
 * 如果输入的message, codeword不都是大写字符，或者codeword长度为0;或者action不是1、2，返回ERROR。
 */
void
coder (const char *message, const char *codeword, int action) {
	if('\0' == t[0][0]) {
		init_table();
	}
	if(NULL == message || '\0' == *message) {
		fprintf(stderr, "message is empty string.\n");
		return;
	}
	if(NULL == codeword || '\0' == *codeword || !validate(message) || !validate(codeword) || action < 1 || action > 2) {
		fprintf(stderr, "input error.\n");
		return;
	}
	char *codeword2 = expand_str(codeword, message);
	if(NULL == codeword2) {
		fprintf(stderr, "codeword缓冲区扩展失败。\n");
		return;
	}
	char result[MAX_LEN+1];
	int m, c, mlen = strlen(message);
	for(int i = 0; i < mlen; i++) {
		m = message[i];
		c = codeword2[i];
		if(1 == action) {
			result[i] = t[c-'A'][m-'A'];
		}
		else if(2 == action) {
			int j;
			for(j = 0; j < 26 && t[c-'A'][j] != m; j++) 
				;
			result[i] = 'A' + j;
		}
	}
	result[mlen] = '\0';
	printf("Reslt: %s\n", result); 
	if(codeword2 != codeword) {
		free(codeword2);
	}
}

int 
main () {
	int action;
	char message[MAX_LEN+1], codeword[MAX_LEN+1], *result;
	printf("Please input: message codeword action.\n");
	while(3 == scanf("%" NUM_TO_S(MAX_LEN) "s %" NUM_TO_S(MAX_LEN) "s %d", message, codeword, &action)) {
		printf("Input result: %s %s %d\n", message, codeword, action);
		coder(message, codeword, action);
	}
}
