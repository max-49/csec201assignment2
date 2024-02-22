/*
 * CSEC-201
 * Authors: CSEC-201 Instructors
 * Semester: 2221
 * Purpose: Blockchain implementation
 */

#include "blockchain.h"

 /*
  * Name: initialize
  * Purpose: Creates and returns a pointer to an empty blockchain
  * Parameters: None
  * Returns: pointer to Blockchain
  */
struct Blockchain* initialize()
{
    struct Blockchain* chain = (struct Blockchain*)malloc(sizeof(struct Blockchain));
    chain->size = 0;
    chain->head = NULL;
    return chain;
}

/*
 * Name: print_blockchain
 * Purpose: Prints blockchain
 * Parameters: Pointer to Blockchain
 * Returns: None
 */
void print_blockchain(struct Blockchain* blockchain)
{
    printf("Printing...\n");
    struct Block* current = blockchain->head;
    while (current) {
        printf("{height: %d, data: %d, prev_hash: %d %d %d %d %d}\n",
            current->height, current->data, current->prev_hash.hash0, current->prev_hash.hash1,
            current->prev_hash.hash2, current->prev_hash.hash3, current->prev_hash.hash4);
        current = current->prev_block;
    }
}

void set_digest(struct Digest* digest, struct Block* block) {
    struct Digest* computedHash = (struct Digest*)SHA_40(block, sizeof(struct Block));
    for (int i = 0; i < sizeof(struct Digest); i++) {
        digest[i] = computedHash[i];
    }
}

void add(struct Blockchain* chain, int data) {

    struct Block* newHead = (struct Block*)malloc(sizeof(struct Block));
    if (!chain->head) {
        newHead->height = 0;
    }
    else {
        newHead->height = chain->head->height + 1;
    }
    newHead->data = data;
    newHead->prev_block = chain->head;

    struct Digest prevHash[sizeof(struct Digest)];

    set_digest(prevHash, chain->head);

    newHead->prev_hash = *prevHash;

    chain->head = newHead;
    chain->size++;

}

void verify(struct Blockchain* chain) {
    printf("Verifying...\n");
    struct Block* temp = chain->head;
    while (temp != NULL) {
        struct Digest* tempDigest[sizeof(struct Digest)];
        set_digest(tempDigest, temp->prev_block);
        if (!digest_equal(tempDigest, &temp->prev_hash)) {
            printf("Block %d failed\n", temp->height);
            return;
        }
        printf("Block %d passed\n", temp->height);
        temp = temp->prev_block;
    }
    printf("All blocks have been verified.\n");
}

void delete_at(struct Blockchain* chain, int height) {
    printf("Deleting...\n");
    if (chain->head->height == height) {
        struct Block* temp = chain->head;
        chain->head = temp->prev_block;
        chain->size--;
        free(temp);
        printf("Block %d deleted\n", height);
    }
    else {
        struct Block* temp = chain->head;
        while (temp->prev_block != NULL) {
            if (temp->prev_block->height == height) {
                struct Block* blockToDelete = temp->prev_block;
                temp->prev_block = blockToDelete->prev_block;
                free(blockToDelete);
                printf("Block %d deleted\n", height);
                chain->size--;
                return;
            }
            temp = temp->prev_block;
        }
    }
}

void alter_block(struct Blockchain* chain, int height, int data) {
    printf("Altering a block...\n");
    struct Block* temp = chain->head;
    while (temp != NULL) {
        if (temp->height == height) {
            temp->data = data;
            printf("Block %d altered now containing %d\n", height, data);
        }
        temp = temp->prev_block;
    }
}

void alter_2_blocks(struct Blockchain* chain, int height, int data) {
    printf("Altering two blocks...\n");
    if (chain->head->height == height) {
        struct Block* temp = chain->head;
        temp->data = data;
    }
    else {
        struct Block* temp = chain->head;
        while (temp->prev_block != NULL) {
            if (temp->prev_block->height == height) {
                struct Block* blockToUpdate = temp->prev_block;
                blockToUpdate->data = data;

                printf("Block %d altered now containing %d\n", height, data);

                struct Digest* prevHash = (struct Digest*)malloc(sizeof(struct Digest));
                set_digest(prevHash, blockToUpdate);
                temp->prev_hash = *prevHash;

                printf("Block %d altered too.\n", temp->height);

                free(prevHash);
            }
            temp = temp->prev_block;
        }
    }
}


/*
 * Name: test
 * Purpose: Test functions above
 */
int testBlockChain(void)
{

    //Create, print, and verify blockchain
    struct Blockchain* blockchain = initialize();
    for (int i = 1; i <= 5; i++)
    {
        add(blockchain, 10 * i);
    }

    //Test cases
    //Test 1
    print_blockchain(blockchain);
    verify(blockchain);

    //Test 2
    //Alter the value in a block, print, and verify blockchain
    /*alter_block(blockchain, 2, 300);
    print_blockchain(blockchain);
    verify(blockchain);*/

    //Test 3
    //Comment out Test 2, and uncomment the following three code lines
    //Alter the value in a block and the hash value tracked for
    //that altered block
    /*alter_2_blocks(blockchain, 1, 200);
    print_blockchain(blockchain);
    verify(blockchain);*/

    //Test 4
    //Comment out Test 2 and 3, and uncomment the following three code lines
    /*delete_at(blockchain, 1);
    print_blockchain(blockchain);
    verify(blockchain);*/

    return 0;
}