#include <stdio.h>
#include <string.h>

int main(void) {
	char* ptr;
	char name[5];
	ptr = name;


	printf("Enter your name: ");
	scanf("%s", name);
	printf("Hello %s\n\n\n", name);

	for (int i = 0; i < 5; i++) {
		printf("%c - %p\n", name[i], &name[i]);
	}

	printf("\n\n\n");
	printf("ptr: %p\n", ptr);

	for (int j = 0; j < 10; j++) {
		printf("%c - %p\n", *(ptr + j), ptr + j);
	}

	printf("ptr: %p - %p\n", ptr, &ptr);



	return 0;
}