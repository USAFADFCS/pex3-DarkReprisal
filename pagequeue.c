/** pagequeue.c
 * ===========================================================
 * Name: _______________________, __ ___ 2026
 * Section: CS483 / ____
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue *pageQueue = (PageQueue * )malloc(sizeof(PageQueue));
    if (pageQueue == NULL) {
        return NULL;
    }
    pageQueue->head = NULL;
    pageQueue->tail = NULL;
    pageQueue->size = 0;
    pageQueue->maxSize = maxSize;
    return pageQueue;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    PqNode *current = pq->tail;
    long depth = 0;

    while (current != NULL) {
        if (current->pageNum == pageNum){

            if(current != pq->tail){
                if(current->prev != NULL){
                    current->prev->next = current->next;
                }
                if(current->next != NULL){
                    current->next->prev = current->prev;
                }
                if(current == pq->head){
                    pq->head = current->next;
                }

                current->next = NULL;
                current->prev = pq->tail;
                pq->tail->next = current;
                pq->tail = current;
            }
            return depth;
        }
        current = current->prev;
        depth++;
    }

    PqNode *newNode = (PqNode *)malloc(sizeof(PqNode));
    if (newNode == NULL) {
        return (-1);
    }

    newNode->pageNum = pageNum;
    newNode->prev = pq->tail;
    newNode->next = NULL;

    if (pq->tail == NULL){
        pq->head = newNode;
        pq->tail = newNode;
    }
    else{
        pq->tail->next = newNode;
        pq->tail = newNode;
    }
    pq->size++;

    if(pq->size > pq->maxSize){
        PqNode *removeNode = pq->head;
        pq->head = pq->head->next;

        if(pq->head){
            pq->head->prev = NULL;
        }
        else{
            pq->tail = NULL;
        }

        free(removeNode);
        pq->size--;
    }
    return (-1);
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    if (pq == NULL) {
        return;
    }

    PqNode *current = pq->head;
    PqNode *nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
}
