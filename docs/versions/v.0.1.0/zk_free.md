---
layout: page
title: zk_free
parent: 0.1.0
grand_parent: Releases
---

```
void zk_free(CONTAINER, FUNC)
```

Frees a `CONTAINER` and set it to `NULL`. If `FUNC` is provided it will be called for every element of `CONTAINER`. `FUNC`
receives as argument the element's data. So if you need to free the element but also the data the element is pointing
to you can do it by providing your own implementation for `FUNC`, see examples below. If you just want to free the
`CONTAINER` but not the data each element is pointing to then `FUNC` can be `NULL`.

### Parameters

`CONTAINER`:
- The `CONTAINER` to be freed.
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
- (none)

### Time Complexity / Space Complexity:
If `CONTAINER` is:
  - {% include containers_url.md container="zk_slist"%}: **O(N) / O(1)**.
  - {% include containers_url.md container="zk_dlist"%}: **O(N) / O(1)**.
  - {% include containers_url.md container="zk_c_slist"%}: **O(N) / O(1)**.
  - {% include containers_url.md container="zk_c_dlist"%}: **O(N) / O(1)**.

### Examples:
- {% include examples_url.md container="zk_slist" file="for_each.c" -%}
- {% include examples_url.md container="zk_dlist" file="for_each.c" -%}
- {% include examples_url.md container="zk_c_slist" file="for_each.c" -%}
- {% include examples_url.md container="zk_c_dlist" file="for_each.c" -%}



