#ifndef ZK_DLIST_H
#define ZK_DLIST_H

typedef struct zk_dlist {
	void *data;
	struct zk_dlist *prev;
	struct zk_dlist *next;
} zk_dlist;

zk_dlist *zk_dlist_push_back(zk_dlist *list, void *const data);

#endif
