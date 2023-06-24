---
layout: page
title: zk_slist_size
parent: zk_slist
grand_parent: v0.1.0
---

``` c
size_t zk_slist_size(const zk_slist *const list);
```

Get the size of the list.

### Parameters

`list`:

- Pointer to a `zk_slist`.

### Returns

- The size of the list.

### Time Complexity / Space Complexity

- **O(N) / O(1)**.

### Examples

- {% include examples_url.md container="zk_slist" file="size.c" -%}
