---
layout: page
title: zk_slist_find
parent: zk_slist
grand_parent: v0.1.0
---

```c
zk_slist *zk_slist_find(zk_slist *list, const void *const data, zk_compare_func const func)
```

Find the first element in the list that matches the given data using the given comparison function.

### Parameters

`list`:

- A pointer to a `zk_slist`.

`data`:

- A pointer to the data to be matched.

`func`:

- A pointer to a comparison function. On match, the function should return `0` see [`zk_compare_func`](../zk_compare_func.html).

### Returns

- Pointer to the first element in the list that matches the given data or `NULL` if no match is found.

### Time Complexity / Space Complexity

- **O(n) / O(1)**.

### Examples

- {% include examples_url.md container="zk_slist" file="find.c" -%}
