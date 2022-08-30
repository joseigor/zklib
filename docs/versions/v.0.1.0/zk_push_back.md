---
layout: page
title: zk_push_back
parent: 0.1.0
grand_parent: Releases
---

```
CONTAINER *zk_push_back(CONTAINER, DATA)
```

Adds a new element to the end of the `CONTAINER`.

### Parameters

`CONTAINER`:
- Could be any of the types below:
  - {% include containers_url.md container="zk_slist"%}
  - {% include containers_url.md container="zk_dlist"%}
  - {% include containers_url.md container="zk_c_slist"%}
- The data is owned by the caller of the function.

 `DATA`:
 - Type: `void*`
 - A pointer to the data to be added.
 - This argument can be `NULL`.
 - The data is owned by the caller of the function.

### Returns
 If `CONTAINER` is:
 - {% include containers_url.md container="zk_slist"%}: The start of the `CONTAINER`.
 - {% include containers_url.md container="zk_dlist"%}: The start of the `CONTAINER`.
 - {% include containers_url.md container="zk_c_slist"%}: The end of the `CONTAINER`.
 - The data is owned by the caller of the function.

### Time Complexity / Space Complexity:
If `CONTAINER` is:
 - {% include containers_url.md container="zk_slist"%}: **O(N) / O(1)**.
 - {% include containers_url.md container="zk_dlist"%}: **O(N) / O(1)**.
 - {% include containers_url.md container="zk_c_slist"%}: **O(1) / O(1)**.

### Examples:
 - {% include examples_url.md container="zk_slist" file="zk_push_back.c" -%}
 - {% include examples_url.md container="zk_dlist" file="zk_push_back.c" -%}
 - {% include examples_url.md container="zk_c_slist" file="zk_push_back.c" -%}


