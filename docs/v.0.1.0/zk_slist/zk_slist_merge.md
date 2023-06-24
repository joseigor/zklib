---
layout: page
title: zk_slist_merge
parent: zk_slist
grand_parent: v0.1.0
---

``` c
zk_slist *zk_slist_merge(zk_slist *first, zk_slist *second, zk_compare_func const func);
```

Merges two sorted lists. If function succeeds it returns a pointer to the merged list, otherwise it returns `NULL`.
Note that `first` and `second` are not valid after the merge and could point to invalid memory.

Merges ascending order if `func(a, b) <= 0` and descending order if `func(a, b) > 0`.

Merge is stable and in-place.

### Parameters

`first`:

- Pointer to the first list.

`second`:

- Pointer to the second list.

`func`:

- A pointer to a comparison function, see [`zk_compare_func`](../zk_compare_func.html).
- Must not be `NULL`.

### Returns

- Pointer to the merged list or `NULL` if function fails.

### Time Complexity / Space Complexity

- **O(n) / O(1)**.

### Examples -->

- {% include examples_url.md container="zk_slist" file="merge.c" -%}
