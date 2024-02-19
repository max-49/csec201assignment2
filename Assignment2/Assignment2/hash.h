#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIGEST_SIZE 5 //The size of SHA_40 hash in bytes
#define BLOCK_SIZE sizeof(struct Block) // Block size in bytes
#define BYTE unsigned char // if you want use BYTE in place of unsigned char

//SHA_40 Digest
struct Digest {
    BYTE hash0, hash1, hash2, hash3, hash4;
};

unsigned char* SHA_40(const unsigned char* data, size_t size); // SHA_40 hash function
int digest_equal(struct Digest* digest1, struct Digest* digest2); //return 1 (true) or 0 (false)
void printDigest(struct Digest* digest1);
