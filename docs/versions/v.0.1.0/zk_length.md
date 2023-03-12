---
layout: page
title: zk_length
parent: 0.1.0
grand_parent: Releases
---

``` c
size_t zk_length(CONTAINER)
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

- {% include examples_url.md container="zk_slist" file="length.c" -%}
