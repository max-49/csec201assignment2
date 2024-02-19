#pragma once
#include <stdio.h>
#include "node.h"

struct LinkedList {
	int size;
	struct node* head;
};

void initialize(struct LinkedList* L);
void addItem(struct LinkedList* L, int dataToAdd);
void printList(struct LinkedList* L);
void deleteItem(struct LinkedList* L, int dataToDelete);
int searchList(struct LinkedList* L, int data);
void updateData(struct LinkedList* L, int oldData, int newData);
void insert(struct LinkedList* L, int previousData, int nextData, int newData);