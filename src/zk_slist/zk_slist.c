#include <stddef.h>
#include <stdlib.h>

#include "zk_common/zk_utils.h"
#include "zk_slist/zk_slist.h"

// SECTION: Private functions

static zk_slist_t *_zk_slist_new_node(void)
{
	zk_slist_t *list = malloc(sizeof(zk_slist_t));
	list->data = NULL;
	list->next = NULL;

	return list;
}

// SECTION END: Private functions

zk_slist_t *zk_slist_append(zk_slist_t *list, void *data)
{
	zk_slist_t *node = _zk_slist_new_node();
	node->data = data;
	node->next = NULL;

	if (list == NULL) {
		list = node;
		return list;
	}

	// add new node to end of the list
	zk_slist_t *node_i = list;
	while (node_i != NULL) {
		if (node_i->next == NULL) {
			node_i->next = node;
			break;
		}
		node_i = node_i->next;
	}
	return list;
}

zk_slist_t *zk_slist_concat(zk_slist_t *list_dest, zk_slist_t *list_src)
{
	if (list_dest == NULL || list_src == NULL) {
		return NULL;
	}

	zk_slist_t *node_i = list_dest;
	while (node_i != NULL) {
		if (node_i->next == NULL) {
			node_i->next = list_src;
			break;
		}
		node_i = node_i->next;
	}

	return list_dest;
}

zk_slist_t *zk_slist_copy(const zk_slist_t *list)
{
	if (list == NULL) {
		return NULL;
	}

	zk_slist_t *cp = NULL;

	while (list != NULL) {
		cp = zk_slist_append(cp, list->data);
		list = list->next;
	}

	return cp;
}

zk_slist_t *zk_slist_copy_deep(const zk_slist_t *list, zk_copy_data_t func, void *user_data)
{
	if (list == NULL || func == NULL) {
		return NULL;
	}

	zk_slist_t *cp = NULL;

	while (list != NULL) {
		void *data = func(list->data, user_data);
		cp = zk_slist_append(cp, data);
		list = list->next;
	}

	return cp;
}

zk_slist_t *zk_slist_delete_node(zk_slist_t *list, zk_slist_t *node, zk_destructor_t func)
{
	if (list == NULL) {
		return NULL;
	}
	if (node == NULL) {
		return list;
	}

	zk_slist_t *head = list;
	zk_slist_t *node_prev = NULL;
	while (list != NULL) {
		if (list == node) {
			// node to be deleted is the head of the list
			if (node_prev == NULL) {
				head = list->next;
			} else {
				node_prev->next = list->next;
			}

			if (func != NULL) {
				func(list->data);
			}
			free(list);
			break;
		}
		node_prev = list;
		list = list->next;
	}

	return head;
}

zk_slist_t *zk_slist_find_by_data(zk_slist_t *list, const void *const data)
{
	while (list != NULL) {
		if (list->data == data) {
			break;
		}
		list = list->next;
	}
	return list;
}

void zk_slist_free(zk_slist_t **list_p)
{
	if (list_p == NULL || *list_p == NULL) {
		return;
	}

	while ((*list_p) != NULL) {
		zk_slist_t *node = *list_p;
		*list_p = (*list_p)->next;
		free(node);
	}

	return;
}

void zk_slist_free_full(zk_slist_t **list_p, zk_destructor_t destructor)
{
	if (list_p == NULL || *list_p == NULL || destructor == NULL) {
		return;
	}

	while ((*list_p) != NULL) {
		zk_slist_t *node = *list_p;
		*list_p = (*list_p)->next;
		destructor(node->data);
		free(node);
	}

	return;
}

zk_slist_t *zk_slist_prepend(zk_slist_t *list, void *data)
{
	zk_slist_t *node = _zk_slist_new_node();
	node->data = data;
	node->next = NULL;

	if (list == NULL) {
		list = node;
	} else {
		node->next = list;
		list = node;
	}

	return list;
}
