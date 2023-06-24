---
layout: page
title: zk_sort
parent: v0.1.0
---

``` c
size_t zk_sort(CONTAINER, FUNC)
```

Sort the `CONTAINER` in **ascending**  or **descending** order using the `FUNC` function.

### Parameters

`CONTAINER`, could be a pointer to any of the types below:

- {% include containers_url.md container="zk_slist"%}

`FUNC`:

- Signature: `int (*cmp)(const void const*, const void const *)`
- Sort ascending: `cmp(a, b) < 0` for `a < b`, `cmp(a, b) == 0` for `a == b`, `cmp(a, b) > 0` for `a > b`.
- Sort descending: `cmp(a, b) > 0` for `a < b`, `cmp(a, b) == 0` for `a == b`, `cmp(a, b) < 0` for `a > b`.

### Returns

`zk_status`:

- `ZK_OK` if the operation was successful
- `ZK_INVALID_ARGUMENT` if the container is invalid.

### Time Complexity / Space Complexity

If `CONTAINER` is:

- {% include containers_url.md container="zk_slist"%}: **O(n log n) / O(1)**.

### Examples -->

- {% include examples_url.md container="zk_slist" file="sort.c" -%}
