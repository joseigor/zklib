---
layout: page
title: zk_slist_find_index
parent: zk_slist
grand_parent: v0.1.0
---

```c
zk_slist *zk_slist_find_index(zk_slist *list, size_t const index);
```

Find the element at the given index.

### Parameters

`list`:

- A pointer to a `zk_slist`.

`index`:

- The index of the element to be found.

### Returns

- Pointer to the element at the given index or `NULL` if no match is found.

### Time Complexity / Space Complexity

- **O(n) / O(1)**.

### Examples

- {% include examples_url.md container="zk_slist" file="find_index.c" -%}
