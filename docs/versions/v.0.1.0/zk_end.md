---
layout: page
title: zk_end
parent: 0.1.0
grand_parent: Releases
---

```
CONTAINER *zk_end(CONTAINER)
```

Returns an iterator to the element following the last element of the `CONTAINER`.

**Note:** For circular linked lists {% include containers_url.md container="zk_c_slist"%} and {% include containers_url.md container="zk_c_dlist"%} it returns the last element of the list **NOT** the element follwing it.

### Parameters

`CONTAINER`:
- Could be any of the types below:
  - {% include containers_url.md container="zk_slist"%}
  - {% include containers_url.md container="zk_dlist"%}
  - {% include containers_url.md container="zk_c_slist"%}
- The data is owned by the caller of the function.

### Returns
 If `CONTAINER` is:
 - {% include containers_url.md container="zk_slist"%}: Iterator to the element following the last element of the `CONTAINER` which in this case is `NULL`.
 - {% include containers_url.md container="zk_dlist"%}: Iterator to the element following the last element of the `CONTAINER` which in this case is `NULL`.
 - {% include containers_url.md container="zk_c_slist"%}: Iterator to the element last element of the `CONTAINER`.
 - The data is owned by the caller of the function.

### Time Complexity / Space Complexity:
If `CONTAINER` is:
 - {% include containers_url.md container="zk_slist"%}: **O(1) / O(1)**.
 - {% include containers_url.md container="zk_dlist"%}: **O(1) / O(1)**.
 - {% include containers_url.md container="zk_c_slist"%}: **O(1) / O(1)**.

### Examples:
 - {% include examples_url.md container="zk_slist" file="zk_end.c" -%}
 - {% include examples_url.md container="zk_dlist" file="zk_end.c" -%}
 - {% include examples_url.md container="zk_c_slist" file="zk_end.c" -%}


