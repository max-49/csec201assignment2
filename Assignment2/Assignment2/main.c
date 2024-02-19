#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "blockchain.h"

int main(void) {
	/*char test1[] = "Rob";
	char test2[] = "James";
	char test3[] = "CSEC";
	char test4[] = "Massimo";

	unsigned char* test1res = SHA_40(test1, strlen(test1));
	unsigned char* test2res = SHA_40(test2, strlen(test2));
	unsigned char* test3res = SHA_40(test3, strlen(test3));
	unsigned char* test4res = SHA_40(test4, strlen(test4));

	printf("The SHA-40 hash of %s is %d %d %d %d %d\n\n", test1, test1res[0], test1res[1], test1res[2], test1res[3], test1res[4]);
	printf("The SHA-40 hash of %s is %d %d %d %d %d\n\n", test2, test2res[0], test2res[1], test2res[2], test2res[3], test2res[4]);
	printf("The SHA-40 hash of %s is %d %d %d %d %d\n\n", test3, test3res[0], test3res[1], test3res[2], test3res[3], test3res[4]);
	printf("The SHA-40 hash of %s is %d %d %d %d %d\n\n", test4, test4res[0], test4res[1], test4res[2], test4res[3], test4res[4]);

	printf("digest_equal(\"Rob\", \"Rob\") = %d\n", digest_equal(SHA_40("Rob", 3), SHA_40("Rob", 3)));
	printf("digest_equal(\"James\", \"Rob\") = %d\n", digest_equal(SHA_40("James", 5), SHA_40("Rob", 3)));*/

	testBlockChain();
}