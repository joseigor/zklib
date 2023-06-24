---
layout: page
title: zk_compare_func
parent:  v0.1.0
---

```c
typedef int (*zk_compare_func)(const void *const a, const void *const b);
```

Defines a comparison function used to compare two elements.

- If `a < b`, the function should return a `negative` value.
- If `a == b`, the function should return `0`.
- If `a > b`, the function should return a `positive` value.
