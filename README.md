# HASHMAP

# Design

```
	Table                 Entities
	
	-------------         ---------    ------    ------
	| collision |         | key   |    |    |    |    |
	|           |         | value |    |    |    |    |
	| entities  |   ->    | next  | -> |    | -> |    |
	-------------         ---------    ------    ------
	|           |
	|           |
	-------------
	|           |
	|           |
	-------------
```

# Structure

	hashmap
		|-capacity
	  	|-size
	  	|-item_size
	  	|-load_factor
	  	|-table
	  	|-hash_func
	  	|-compare_func
	  	|-key_alloc_func
	  	|-key_free_func

	item
		|-collision
		|-entities

	entity
		|-key
		|-value
		|-next

# API

## Raw API

	Create hashmap. 

> * You can set `capacity=0`, `load_factor=0f` to use default values.
> * Return 0 for success.

```
	name##_hashmap name##_hashmap_create(size_t capacity, float load_factor);	
```

---

	Destroy hashmap.

> * Return 0 for success.

``` 
	int name##_hashmap_destroy(name##_hashmap *pmap);
```

---

	Set hash function to generate real hash key in hash table.

> * `hash_func` is supposed to generate evenly distributed keys between (0, MAX).
> * `MAX` is supposed to be larger than capacity or equal with capacity.
> * Default `hash_func` always return 0. 
> * Return 0 for success.

```
	int name##_hashmap_set_hash_func(name##_hash map, size_t (*hash_func)(key_type, size_t));
```

---

	Set compare function to distinguish keys.

> * `compare_func` is required to return 0 when two keys are the same, 
	and return non-zero when they are different.
> * Default `compare_func` always return `key1 - key2`.
> * Return 0 for success.

```
	int name##_hashmap_set_compare_func(name##_hash map, int (*compare_func)(key_type, key_type));
```

---

	Set key alloc and free functions to store and release keys in hash table.

> * Default `key_alloc_free` and `key_free_func` is `NULL`, so they do nothing.
> * Return 0 for success.

```
	int name##_hashmap_set_key_funcs(name##_hash map, key_type (*key_alloc_func)(key_type), void (*key_free_func)(key_type));
```

---

	Check whether `key` exists.

> * Return 0 for success.

```
	int name##_hashmap_has_key(name##_hash map, key_type key);
```

---

	Get `value` with `key`.

> * `value` will be written into `pvalue` if `key` exists. Otherwise, do nothing.
> * You'd better check return value in case `key` doesn't exist.
> * Return 0 for success.

```
	int name##_hashmap_get(name##_hashmap map, key_type key, value_type *pvalue);
```

---

	Put pair (`key`, `value`).

> * Return 0 for success.

```
	int name##_hashmap_put(name##_hashmap map, key_type key, value_type value);
```

---

	Remove pair with `key`.

> * Return 0 for success.

```
	int name##_hashmap_remove(name##_hash map, key_type key);
```

---

	Query hashmap size.

> * Return hashmap size for success, return negative for failure.

```
	int name##_hashmap_size(name##_hash map);
```

---

	Query hashmap collision mean.

> * Return collision mean for success, return negative for failure.

```
	double name##_hashmap_collision_mean(name##_hash map);
```

---

	Query hashmap collision variance.

> * Return collision variance for success, return negative for failure.

```
	double name##_hashmap_collision_variance(name##_hash map);
```

---

	Get hashmap iterator.

> * Return iterator for success, return `NULL` for failure.

```
	name##_iter name##_hashmap_iter(name##_hash map);
```

---

	Get next hashmap iterator.

> * Return next iterator for success, return `NULL` for failure.

```
	name##_iter name##_hashmap_next(name##_hash map, name##_iter it);
```

---

	Get pair (`key`, `value`) with iterator.

> * (`key`, `value`) will be written into (`pkey`, `pvalue`) if they are not `NULL`.
> * return 0 for success.

```
	int name##_hashmap_iter_get(name##_iter it, key_type *pkey, value_type *pvalue);
```

---

	Put new `value` with iterator.

> * return 0 for success.

```
	int name##_hashmap_iter_put(name##_iter it, value_type value);
```

---

	Visit all pairs of (`key`, `value`) with function `func` and extra parameter `args`.

> * If `func` is `NULL`, do nothing.
> * It will stop immediately if hashmap size is changed.
> * Return 0 for success.

```
	int name##_hashmap_foreach(name##_hashmap map, int (*func)(key_type, value_type, void *), void *args);
```

## Macro API

	Macro of function `name##_hashmap_create`

```
	hashmap(name) hashmap_create(name, capacity, load_facotor);
```

----

	Macro of function `name##_hashmap_destroy`

```
	int hashmap_destroy(name, phashmap);
```

---

	Macro of function `name##_hashmap_set_hash_func`

```
	int hashmap_set_hash_func(name, hashmap, hash_func);
```

---

	Macro of function `name##_hashmap_set_compare_func`

```
	int hashmap_set_compare_func(name, hashmap, compare_func);
```

---

	Macro of function `name##_hashmap_set_key_funcs`

```
	int hashmap_set_key_funcs(name, hashmap, key_alloc_func, key_free_func);
```

---

	Macro of function `name##_hashmap_has_key`

```
	int hashmap_has_key(name, hashmap, key);
```

---

	Macro of function `name##_hashmap_get`

```
	int hashmap_get(name, hashmap, key, pvalue);
```

---

	Macro of function `name##_hashmap_put`

```
	int hashmap_put(name, hashmap, key, value);
```

---

	Macro of function `name##_hashmap_remove`

```
	int hashmap_remove(name, hashmap, key);
```

---

	Macro of function `name##_hashmap_size`

```

	int hashmap_size(name, hashmap);
```

---

	Macro of function `name##_hashmap_collision_mean`

```	
	double hashmap_collision_mean(name, hashmap);
```

---

	Macro of function `name##_hashmap_collision_variance`

```	
	double hashmap_collision_variance(name, hashmap);
```

---

	Macro of function `name##_hashmap_iter`

```
	iter(name) hashmap_iter(name, hashmap);
```

---

	Macro of function `name##_hashmap_next`

```	
	iter(name) hashmap_next(name, hashmap, iter);
```

---

	Macro of function `name##_hashmap_iter_get`

```
	int hashmap_iter_get(name, iter, pkey, pvalue);
```

---

	Macro of function `name##_hashmap_iter_put`

```
	int hashmap_iter_put(name, iter, value);
```

---

	Macro of function `name##_hashmap_foreach`

```
	int hashmap_foreach(name, hashmap, func, args);
```

# Example

	[example.c](./example.c)