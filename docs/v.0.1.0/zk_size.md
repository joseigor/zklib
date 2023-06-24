---
layout: page
title: zk_size
parent: v0.1.0
---

``` c
size_t zk_size(CONTAINER)
```

Return the number of elements in the `CONTAINER`.

### Parameters

`CONTAINER`, could be a pointer to any of the types below:

- {% include containers_url.md container="zk_slist"%}

### Returns

`size_t`: the number of elements in the `CONTAINER`.

### Time Complexity / Space Complexity

If `CONTAINER` is:

- {% include containers_url.md container="zk_slist"%}: **O(N) / O(1)**.

### Examples -->

- {% include examples_url.md container="zk_slist" file="size.c" -%}
