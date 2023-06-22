---
layout: page
title: zk_reverse
parent: 0.1.0
grand_parent: Releases
---

``` c
zk_status zk_reverse(CONTAINER)
```

Reverse the order of the elements in the `CONTAINER`. The first element becomes the last and the last becomes the first.
References and iterators to the erased element becomes invalid.

### Parameters

`CONTAINER`, could be a pointer to any of the types below:

- {% include containers_url.md container="zk_slist"%}

### Returns

`zk_status`:

- `ZK_OK` if the operation was successful
- `ZK_INVALID_ARGUMENT` if the container is invalid.

### Time Complexity / Space Complexity

If `CONTAINER` is:

- {% include containers_url.md container="zk_slist"%}: **O(N) / O(1)**.

### Examples

- {% include examples_url.md container="zk_slist" file="reverse.c" -%}
