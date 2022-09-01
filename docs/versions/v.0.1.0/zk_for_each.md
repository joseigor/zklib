---
layout: page
title: zk_for_each
parent: 0.1.0
grand_parent: Releases
---

```
void zk_for_each(CONTAINER, FUNC, USER_DATA)
```

Iterates over each element of the `CONTAINER` and calls the function `FUNC`. This function takes two arguments, the `CONTAINER` element data as the first argument and the given  `USER_DATA` as second argument.

### Parameters

`CONTAINER`:
- Could be a pointer to any of the types below:
  - {% include containers_url.md container="zk_slist"%}
  - {% include containers_url.md container="zk_dlist"%}
  - {% include containers_url.md container="zk_c_slist"%}
  - {% include containers_url.md container="zk_c_dlist"%}
- The data is owned by the caller of the function.

`FUNC`:
 - Type: `zk_for_each_func`
 - Function called for each node of the list.
 - The data is owned by the caller of the function.

`USER_DATA`:
- Type: `void*`
- User data to be passed as second argument of `FUNC`.
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



