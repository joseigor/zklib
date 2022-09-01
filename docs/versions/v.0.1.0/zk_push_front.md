---
layout: page
title: zk_push_front
parent: 0.1.0
grand_parent: Releases
---

```
CONTAINER *zk_push_front(CONTAINER, DATA)
```

Prepends the given element value to the beginning of the `CONTAINER`.

### Parameters

`CONTAINER`:
- Could be a pointer to any of the types below:
  - {% include containers_url.md container="zk_slist"%}
  - {% include containers_url.md container="zk_dlist"%}
  - {% include containers_url.md container="zk_c_slist"%}
  - {% include containers_url.md container="zk_c_dlist"%}
- The data is owned by the caller of the function.

`DATA`:
- Type: `void*`
- A pointer to the data to be added.
- This argument can be `NULL`.
- The data is owned by the caller of the function.

### Returns
A pointer to a `CONTAINER`. If `CONTAINER` is:
- {% include containers_url.md container="zk_slist"%}: returns the **start** of the `CONTAINER`.
- {% include containers_url.md container="zk_dlist"%}: returns the **start** of the `CONTAINER`.
- {% include containers_url.md container="zk_c_slist"%}: returns the **end** of the `CONTAINER`.
- {% include containers_url.md container="zk_c_dlist"%}: returns the **start** of the `CONTAINER`.
- The data is owned by the caller of the function.

### Time Complexity / Space Complexity:
If `CONTAINER` is:
- {% include containers_url.md container="zk_slist"%}: **O(1) / O(1)**.
- {% include containers_url.md container="zk_dlist"%}: **O(1) / O(1)**.
- {% include containers_url.md container="zk_c_slist"%}: **O(1) / O(1)**.
- {% include containers_url.md container="zk_c_dlist"%}: **O(1) / O(1)**.

### Examples:
- {% include examples_url.md container="zk_slist" file="push_front.c" -%}
- {% include examples_url.md container="zk_dlist" file="push_front.c" -%}
- {% include examples_url.md container="zk_c_slist" file="push_front.c" -%}
- {% include examples_url.md container="zk_c_dlist" file="push_front.c" -%}


