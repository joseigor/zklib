---
layout: page
title: zk_slist_reverse
parent: zk_slist
grand_parent: v0.1.0
---

``` c
zk_slist *zk_slist_reverse(zk_slist *list);
```

Reverse the order of the elements in the `list`. The first element becomes the last and the last becomes the first.
References and iterators to the erased element becomes invalid.

### Parameters

`list`:

- Pointer to a `zk_slist` to be reversed.

### Returns

- Pointer to the reversed list.

### Time Complexity / Space Complexity

- **O(N) / O(1)**.

### Examples

- {% include examples_url.md container="zk_slist" file="reverse.c" -%}
