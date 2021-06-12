#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    if (q->size == 0) {
        free(q);
        return;
    }
    list_ele_t *temp = q->head;
    list_ele_t *nex = temp->next;
    while (nex) {
        free(temp->value);
        free(temp);
        temp = nex;
        nex = nex->next;
    }
    free(temp->value);
    free(temp);
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q)
        return NULL;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    char *new_value = strdup(s);
    if (!new_value) {
        free(newh);
        return false;
    }
    newh->value = new_value;
    newh->next = q->head;
    if (!q->tail)
        q->tail = newh;
    q->head = newh;
    q->size += 1;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = strdup(s);
    if (!newt->value) {
        free(newt);
        return false;
    }
    newt->next = NULL;
    if (q->tail) {
        q->tail->next = newt;
        q->tail = newt;
    } else {
        q->head = q->tail = newt;
    }
    q->size += 1;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0)
        return false;
    int len = strlen(q->head->value);
    len = bufsize - 1 > len ? len : bufsize - 1;
    memcpy(sp, q->head->value, len);
    *(sp + len) = '\0';
    free(q->head->value);
    q->head->value = NULL;
    list_ele_t *temp = q->head;
    q->head = q->head->next;
    free(temp);
    temp = NULL;
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size == 0)
        return;
    list_ele_t *prev = NULL;
    list_ele_t *cur = q->head;
    list_ele_t *temp = cur->next;
    while (temp) {
        cur->next = prev;
        prev = cur;
        cur = temp;
        temp = cur->next;
    }
    cur->next = prev;
    q->tail = q->head;
    q->head = cur;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *merge_sort(list_ele_t *start)
{
    if (!start->next)
        return start;
    list_ele_t *left = start;
    list_ele_t *right = start->next;
    start->next = NULL;
    left = merge_sort(left);
    right = merge_sort(right);
    for (list_ele_t *merge = NULL; left || right;) {
        if (!right || (left && (strcmp(left->value, right->value) < 0))) {
            if (!merge) {
                start = merge = left;
            } else {
                merge->next = left;
                merge = merge->next;
            }
            left = left->next;
        } else {
            if (!merge) {
                start = merge = right;
            } else {
                merge->next = right;
                merge = merge->next;
            }
            right = right->next;
        }
    }
    return start;
}

list_ele_t *find_tail(list_ele_t *head)
{
    list_ele_t *temp = head;
    list_ele_t *res = temp;
    while (temp) {
        res = temp;
        temp = temp->next;
    }
    return res;
}

void q_sort(queue_t *q)
{
    if (!q || q_size(q) == 0)
        return;
    list_ele_t *head = merge_sort(q->head);
    list_ele_t *tail = find_tail(head);
    q->head = head;
    q->tail = tail;
}
