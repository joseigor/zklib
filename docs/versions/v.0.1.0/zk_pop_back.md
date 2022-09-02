---
layout: page
title: zk_pop_back
parent: 0.1.0
grand_parent: Releases
---

```
CONTAINER *zk_pop_back(CONTAINER, FUNC)
```

Removes the last element of the `CONTAINER`. If there are no elements in the container `NULL` is returned.

If `FUNC` is provided it will be called for every element of `CONTAINER`. `FUNC` receives as argument the element's data.
So if you need to free the element but also the data the element is pointing to you can do it by providing your own implementation for `FUNC`, see examples below.
If you just want to free the `CONTAINER` but not the data each element is pointing to then `FUNC` can be `NULL`.

References and iterators to the erased element becomes invalid.

### Parameters

`CONTAINER`:
- Could be a pointer to any of the types below:
  - {% include containers_url.md container="zk_slist"%}
  - {% include containers_url.md container="zk_dlist"%}
  - {% include containers_url.md container="zk_c_slist"%}
  - {% include containers_url.md container="zk_c_dlist"%}
- The data is owned by the caller of the function.

`FUNC`:
- Type: `zk_destructor_t`
- Function called for each element of the `CONTAINER`.
- This is your custom implementation to free each element`s data.
- It can be `NULL`.
- The data is owned by the caller of the function.

### Returns
A pointer to a `CONTAINER`. If `CONTAINER` is:
- {% include containers_url.md container="zk_slist"%}: returns the **start** of the `CONTAINER` or `NULL` if `CONTAINER` is empty after element was removed.
- {% include containers_url.md container="zk_dlist"%}: returns the **start** of the `CONTAINER` or `NULL` if `CONTAINER` is empty after element was removed.
- {% include containers_url.md container="zk_c_slist"%}: returns the **end** of the `CONTAINER` or `NULL` if `CONTAINER` is empty after element was removed.
- {% include containers_url.md container="zk_c_dlist"%}: returns the **start** of the `CONTAINER` or `NULL` if `CONTAINER` is empty after element was removed.
- The data is owned by the caller of the function.

### Time Complexity / Space Complexity:
If `CONTAINER` is:
- {% include containers_url.md container="zk_slist"%}: **O(N) / O(1)**.
- {% include containers_url.md container="zk_dlist"%}: **O(N) / O(1)**.
- {% include containers_url.md container="zk_c_slist"%}: **O(1) / O(1)**.
- {% include containers_url.md container="zk_c_dlist"%}: **O(1) / O(1)**.

### Examples:
- {% include examples_url.md container="zk_slist" file="pop_back.c" -%}
- {% include examples_url.md container="zk_dlist" file="pop_back.c" -%}
- {% include examples_url.md container="zk_c_slist" file="pop_back.c" -%}
- {% include examples_url.md container="zk_c_dlist" file="pop_back.c" -%}


