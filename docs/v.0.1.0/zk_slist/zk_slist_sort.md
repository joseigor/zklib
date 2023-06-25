---
layout: page
title: zk_slist_sort
parent: zk_slist
grand_parent: v0.1.0
---

``` c
zk_slist *zk_slist_sort(zk_slist *list, zk_compare_func const func);
```

Sorts list in ascending order if func(a, b) <= 0 and in descending order if func(a, b) > 0.

The original list is invalid after the sort as it is sorted in place.

Sorts the list using iterative merge sort.

### Parameters

`list`:

- Pointer to the list to sort.

`func`:

- Pointer to the comparison function.
- The comparison function must return a `negative` value if `a < b`, `0` if `a == b`, and a `positive` value if `a > b`.
- If `NULL`, the list is returned unsorted.

### Returns

- Pointer to the sorted list.

### Time Complexity / Space Complexity

- **O(n log n) / O(1)**.

### Examples

- {% include examples_url.md container="zk_slist" file="sort.c" -%}
