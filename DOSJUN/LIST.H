#ifndef DOSJUN_LIST_H
#define DOSJUN_LIST_H

typedef enum {
	ltString,
	ltInteger,
	ltObject
} list_type;

typedef struct {
	list_type type;
	unsigned int size;
	unsigned int capacity;
	void **items;
} list;

void Add_to_List(list *l, void *item);
void Add_String_to_List(list *l, char *s);
void Clear_List(list *l);
void Free_List(list *l);
bool In_List(list *l, void *match);
void *List_At(list *l, unsigned int index);
list *New_List(list_type type, char *tag);
bool Remove_from_List(list *l, void *match);

#endif