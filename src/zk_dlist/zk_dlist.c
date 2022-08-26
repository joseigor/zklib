#include <stdlib.h>

#include "zk_dlist/zk_dlist.h"

// SECTION: Private functions
static zk_dlist *zk_dlist_new_node(void *const data)
{
	zk_dlist *node = malloc(sizeof(zk_dlist));
	node->data = data;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

static zk_dlist *zk_dlist_back(zk_dlist *list)
{
	if (list != NULL) {
		while (list->next != NULL) {
			list = list->next;
		}
	}

	return list;
}

// SECTION END: Private functions
zk_dlist *zk_dlist_push_back(zk_dlist *list, void *const data)
{
	zk_dlist *node = zk_dlist_new_node(data);
	if (list == NULL) {
		list = node;
	} else {
		zk_dlist *tail = zk_dlist_back(list);
		tail->next = node;
		node->prev = tail;
	}
	return list;
}
