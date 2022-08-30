---
layout: page
title: zk_begin
parent: 0.1.0
grand_parent: Releases
---

```
CONTAINER *zk_begin(CONTAINER)
```

Returns an iterator to the first element of the `CONTAINER`.

If the `CONTAINER` is empty, the returned iterator will be equal to [`zk_end()`](zk_end.html).

### Parameters

`CONTAINER`:
- Could be any of the types below:
  - {% include containers_url.md container="zk_slist"%}
  - {% include containers_url.md container="zk_dlist"%}
  - {% include containers_url.md container="zk_c_slist"%}
- The data is owned by the caller of the function.

### Returns
 If `CONTAINER` is:
 - {% include containers_url.md container="zk_slist"%}: Iterator to the first element of the `CONTAINER`.
 - {% include containers_url.md container="zk_dlist"%}: Iterator to the first element of the `CONTAINER`.
 - {% include containers_url.md container="zk_c_slist"%}: Iterator to the first element of the `CONTAINER`.
 - The data is owned by the caller of the function.

### Time Complexity / Space Complexity:
If `CONTAINER` is:
 - {% include containers_url.md container="zk_slist"%}: **O(1) / O(1)**.
 - {% include containers_url.md container="zk_dlist"%}: **O(1) / O(1)**.
 - {% include containers_url.md container="zk_c_slist"%}: **O(1) / O(1)**.

### Examples:
 - {% include examples_url.md container="zk_slist" file="zk_begin.c" -%}
 - {% include examples_url.md container="zk_dlist" file="zk_begin.c" -%}
 - {% include examples_url.md container="zk_c_slist" file="zk_begin.c" -%}


