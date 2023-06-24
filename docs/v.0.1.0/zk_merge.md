---
layout: page
title: zk_merge
parent: v0.1.0
---

``` c
size_t zk_merge(CONTAINER_1, CONTAINER_2, FUNC)
```

Merge the elements of `CONTAINER_2` into `CONTAINER_1` using the `FUNC` function.
In successful merge, `CONTAINER_2` will be empty pointing to NULL.

### Parameters

`CONTAINER_1` and `CONTAINER_2`, could be a pointer to any of the types below:

- {% include containers_url.md container="zk_slist"%}

`FUNC`:

- Signature: `int (*cmp)(const void const*, const void const *)`
- Merge ascending: `cmp(a, b) < 0` for `a < b`, `cmp(a, b) == 0` for `a == b`, `cmp(a, b) > 0` for `a > b`.
- Merge descending: `cmp(a, b) > 0` for `a < b`, `cmp(a, b) == 0` for `a == b`, `cmp(a, b) < 0` for `a > b`.

### Returns

`zk_status`:

- `ZK_OK` if the operation was successful
- `ZK_INVALID_ARGUMENT` if any of the containers is invalid.

### Time Complexity / Space Complexity

If `CONTAINER` is:

- {% include containers_url.md container="zk_slist"%}: **O(n) / O(1)**.

### Examples -->

- {% include examples_url.md container="zk_slist" file="merge.c" -%}
