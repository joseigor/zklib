---
layout: page
title: zk_push_back
parent: 0.1.0
grand_parent: Releases
---

Adds a new element to the end of the `CONTAINER`.

```c
CONTAINER *zk_push_back(CONTAINER, DATA)
```

### Parameters

`CONTAINER`:
- Could be any of the types below:
  - `zk_slist_t *`
  - `zk_dlist *`
 - The data is owned by the caller of the function.

 `DATA`:
 - Type: `void*`
 - A pointer to the data to be added.
 - This argument can be `NULL`.
 - The data is owned by the caller of the function.

### Returns
 If `CONTAINER` is:
 - `zk_slist_t*`: The start of the `CONTAINER`.
 - `zk_dlist*`: The start of the `CONTAINER`.
 - The data is owned by the caller of the function.

### Time Complexity / Space Complexity:
If `CONTAINER` is:
 - `zk_slist_t*`: **O(N) / O(1)**.
 - `zk_dlist*`: **O(N) / O(1)**.

### Examples
 - [zk_slist/push_back](https://github.com/zklib/zklib/blob/main/examples/zk_slist/push_back.c)
  - [zk_dlist/push_back](https://github.com/zklib/zklib/blob/main/examples/zk_dlist/push_back.c)


