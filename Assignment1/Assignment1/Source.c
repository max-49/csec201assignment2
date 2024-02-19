#include <stdio.h>
#include <stdlib.h> // memory management
#include <string.h>
#include <ctype.h> // for toupper()

int main(void) {
	/*
	 * Get mode
	 */
	
	// read mode from stdin
	char mode[9];
	printf("Would you like to encrypt or decrypt?: ");
	fgets(mode, sizeof(mode), stdin);

	// replace terminating newline with null byte
	mode[strcspn(mode, "\n")] = 0;

	// assert a valid mode was entered
	unsigned short isEncrypt;
	if (!(strcmp(mode, "encrypt") == 0 || strcmp(mode, "decrypt") == 0)) {
		printf("Please input a valid mode.\n");
		return;
	}
	else {
		if (strcmp(mode, "encrypt") == 0) {
			isEncrypt = 1;
		}
		else {
			isEncrypt = 0;
		}
	}

	/*
	 * Get cipher file
	 */

	// read filename from stdin and get rid of newline character
	char filename[50];
	printf("Please enter the name of the cipher file: ");
	fgets(filename, sizeof(filename), stdin);
	filename[strcspn(filename, "\n")] = 0;

	/*
	 * Get ciphertext/plaintext
	 */
	char string[25];
	printf("Please enter the string you want to %s: ", mode);
	fgets(string, sizeof(string), stdin);
	string[strcspn(string, "\n")] = 0;

	// create buffer for cipher file lines on the heap
	char* cipher = (char*)malloc(5*sizeof(char));
	// create string for converted string (ct/pt)
	char convertedString[sizeof(string)];

	// open file and verify it exists
	FILE* fptr;

	fptr = fopen(filename, "r");
	if (fptr == NULL) {
		printf("File not found.\n");
		return;
	}

	/*
	* Validate cipher file
	*/

	// Hold left side of mappings
	char plaintext[26];
	// Hold right side of mappings
	char mappings[26];
	// Hold index of spot to insert mappings
	int index = 0;

	// iterate over each line in the file
	while (fgets(cipher, sizeof(cipher), fptr) != NULL) {
		// for every character already in the list of plaintext..
		for (int i = 0; i < sizeof(plaintext); i++) {
			// if this character has already appeared before, the cipher file is invalid
			if (toupper(plaintext[i]) == toupper(*cipher)) {
				printf("Invalid cipher file.");
				return;
			}
		}
		// for every character already in the list of mappings...
		for (int j = 0; j < sizeof(mappings); j++) {
			// if this character has already appeared before, the cipher file is invalid
			if (toupper(mappings[j]) == toupper(*(cipher+2))) {
				printf("Invalid cipher file.");
				return;
			}
		}

		// if this line caused no errors, add them to the list of known mappings and increment the index
		plaintext[index] = *cipher;
		mappings[index] = *(cipher+2);
		index++;
	}

	// iterate over the cipher mappings
	for (int j = 0; j < sizeof(plaintext); j++) {
		// for each line in the file, check to see if there are any letters that need to be changed
		for (int i = 0; i < strlen(string); i++) {
			// If the user wants to encrypt, search for the substitution that corresponds to each character
			if (isEncrypt == 1) {
				if (toupper(plaintext[j]) == toupper(string[i])) {
					convertedString[i] = mappings[j];
				}
			}
			// If the user wants to decrypt, search for the plaintext character that corresponds to the substitution
			else {
				if (toupper(mappings[j]) == toupper(string[i])) {
					convertedString[i] = plaintext[j];
				}
			}
		}
	}

	// close the cipher file
	fclose(fptr);
	
	// add a terminating null byte to complete the string
	convertedString[strlen(string)] = 0;

	// print the resulting string to stdout
	printf("Output: %s", convertedString);

}