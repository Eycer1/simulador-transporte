#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

int initList(List *list)
{
    list->content = NULL;
    list->next = NULL;
    list->lenght = 0;
}

List *listPush(List *old, ListContent *content)
{
    List *new = (List *)malloc(sizeof(List));
    new->next = old;
    new->content = content;
    new->lenght = old->lenght + 1;
    return new;
};

List *listPushAtEnd(List *old, ListContent *content)
{
    List *head = old;
    while (old->next)
        old = old->next;
    List *new = listPush(old, content);
    old = head;
    while (old->next && old->next->next)
        old = old->next;
    if (old->next)
    {
        old->next = new;
        new->lenght = old->lenght + 1;
    }
    else
    {
        head = new;
    }
    return head;
}

List *reverseList(List *list)
{
    List *new = (List *)malloc(sizeof(List));
    while (list->next)
    {
        new = listPushAtEnd(new, list->content);
        list = list->next;
    }
    return new;
}

void printList(List *list)
{
    List *i = list;
    while (i)
    {

        if (i && i->content == NULL && i->next == NULL)
        {
            printf("ENDLIST\n");
        }
        else
        {
            printf("List Content at %p\n", i->content);
        }
        i = i->next;
    }
}

void mergeList(List **answer, List *list)
{
    if (answer && (*answer)->content == NULL && (*answer)->next == NULL)
    {
        *answer = list;
    }
    else
    {
        int l1 = (*answer)->lenght;
        int l2 = list->lenght;
        int l = l1 + l2;
        List *dummy = (*answer);
        while (dummy->next->content)
        {
            dummy->lenght = l;
            l--;
            dummy = dummy->next;
        }
        dummy->next = list;
    }
}