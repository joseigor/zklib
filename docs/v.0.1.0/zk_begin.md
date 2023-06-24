---
layout: page
title: zk_begin
parent: v0.1.0
---

```
CONTAINER *zk_begin(CONTAINER)
```

Returns an iterator to the first element of the `CONTAINER`.

If the `CONTAINER` is empty, the returned iterator will be equal to [`zk_end()`](zk_end.html).

### Parameters

`CONTAINER`:
- Could be a pointer to any of the types below:
  - {% include containers_url.md container="zk_slist"%}
  - {% include containers_url.md container="zk_dlist"%}
  - {% include containers_url.md container="zk_c_slist"%}
  - {% include containers_url.md container="zk_c_dlist"%}
- The data is owned by the caller of the function.

### Returns
A pointer to a `CONTAINER`. If `CONTAINER` is:
- {% include containers_url.md container="zk_slist"%}: returns an iterator to the first element of the `CONTAINER`.
- {% include containers_url.md container="zk_dlist"%}: returns an iterator to the first element of the `CONTAINER`.
- {% include containers_url.md container="zk_c_slist"%}: returns an iterator to the first element of the `CONTAINER`.
- {% include containers_url.md container="zk_c_dlist"%}: returns an iterator to the first element of the `CONTAINER`.
- The data is owned by the caller of the function.

### Time Complexity / Space Complexity:
If `CONTAINER` is:
- {% include containers_url.md container="zk_slist"%}: **O(1) / O(1)**.
- {% include containers_url.md container="zk_dlist"%}: **O(1) / O(1)**.
- {% include containers_url.md container="zk_c_slist"%}: **O(1) / O(1)**.
- {% include containers_url.md container="zk_c_dlist"%}: **O(1) / O(1)**.

### Examples:
- {% include examples_url.md container="zk_slist" file="begin_end.c" -%}
- {% include examples_url.md container="zk_dlist" file="begin_end.c" -%}
- {% include examples_url.md container="zk_c_slist" file="begin_end.c" -%}
- {% include examples_url.md container="zk_c_dlist" file="begin_end.c" -%}


