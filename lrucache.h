#ifndef __LRUCACHE_H
#define __LRUCACHE_H

#include "hashmap.h"

#define lrucache_log(stream, format, args...) \
	fprintf(stream, "%s: %d: %s: " format, __FILE__, __LINE__, __func__, ##args); 

#define lrucache_error(format, args...) lrucache_log(stderr, format, ##args)

#define lrucache(name) name##_lrucache

#define lrucache_create(name, capacity) \
	name##_lrucache_create(capacity)
#define lrucache_destroy(name, cache) \
	name##_lrucache_destroy(&cache)
#define lrucache_set_hash_func(name, cache, hash_func) \
	name##_lrucache_set_hash_func(cache, hash_func)
#define lrucache_set_compare_func(name, cache, compare_func) \
	name##_lrucache_set_compare_func(cache, compare_func)
#define lrucache_set_key_funcs(name, cache, key_alloc_func, key_free_func) \
	name##_lrucache_set_key_funcs(cache, key_alloc_func, key_free_func)
#define lrucache_get(name, cache, key, pvalue) \
	name##_lrucache_get(cache, key, pvalue);
#define lrucache_put(name, cache, key, value) \
	name##_lrucache_put(cache, key, value);


#define LRUCACHE_DEFINE(name, key_type, value_type) \
	\
	/* Link Node, member prev and next are Link Node pointers*/ \
	typedef struct name##_LRUCACHE_NODE { \
		struct name##_LRUCACHE_NODE *prev; \
		struct name##_LRUCACHE_NODE *next; \
		key_type key; \
		value_type value; \
	} name##_lrucache_node, *name##_lrucache_link; \
	\
	/* Define hashmap */ \
	HASHMAP_DEFINE(name, key_type, name##_lrucache_link) \
	\
	/* LRUCache Entity, member map is a hashmap */ \
	typedef struct { \
		name##_lrucache_link head; \
		name##_lrucache_link tail; \
		hashmap(name) map; \
		size_t capacity; \
	} name##_lrucache_entity, *name##_lrucache; \
	\
	\
	/* Set hash function
	 */\
	int name##_lrucache_set_hash_func(name##_lrucache cache, \
		size_t (*hash_func)(key_type, size_t)) \
	{ \
		if (!cache) \
		{ \
			lrucache_error("Error: [%2d]: Invalid lru cache\n", ERR_USER) \
			return ERR_USER; \
		} \
		return hashmap_set_hash_func(name, cache->map, hash_func); \
	} \
	\
	/* Set compare function
	 */\
	int name##_lrucache_set_compare_func(name##_lrucache cache, \
		int (*compare_func)(key_type, key_type)) \
	{ \
		if (!cache) \
		{ \
			lrucache_error("Error: [%2d]: Invalid lru cache\n", ERR_USER) \
			return ERR_USER; \
		} \
		return hashmap_set_compare_func(name, cache->map, compare_func); \
	} \
	\
	/* Set key functions
	 */\
	int name##_lrucache_set_key_funcs(name##_lrucache cache, \
		key_type (*key_alloc_func)(key_type), void (*key_free_func)(key_type)) \
	{ \
		if (!cache) \
		{ \
			lrucache_error("Error: [%2d]: Invalid lru cache\n", ERR_USER) \
			return ERR_USER; \
		} \
		return hashmap_set_key_funcs(name, cache->map, key_alloc_func, key_free_func); \
	} \
	\
	/* Create cache
	 */\
	name##_lrucache name##_lrucache_create(size_t capacity) \
	{ \
		name##_lrucache cache = NULL; \
		cache = (name##_lrucache) malloc(sizeof(name##_lrucache_entity)); \
		if (!cache) \
		{ \
			lrucache_error("Error [%2d]: Can not allocate memory for new lru cache\n", ERR_SYS) \
			return NULL; \
		} \
		memset(cache, 0, sizeof(name##_lrucache_entity)); \
		cache->map = hashmap_create(name, capacity, DEF_FACTOR); \
		if (!cache->map) \
		{ \
			lrucache_error("Error [%2d]: Can not allocate memory for hash map\n", ERR_SYS) \
			return NULL; \
		} \
		cache->capacity = capacity; \
		return cache; \
	} \
	\
	/* Destroy cache
	 */\
	int name##_lrucache_destroy(name##_lrucache *pcache) \
	{ \
		if (!(*pcache) || !(*pcache)->map) \
		{ \
			lrucache_error("Error [%2d]: Invalid lru cache\n", ERR_USER); \
			return ERR_USER; \
		} \
		name##_lrucache_link l; \
		name##_lrucache_link tmp; \
		l = (*pcache)->head; \
		while (l) \
		{ \
			tmp = l->next; \
			free(l); \
			l = tmp; \
		} \
		(*pcache)->head = NULL; \
		(*pcache)->tail = NULL; \
		return hashmap_destroy(name, (*pcache)->map); \
	} \
	\
	/* Get cache
	 */\
	int name##_lrucache_get(name##_lrucache cache, \
		key_type key, value_type *pvalue) \
	{ \
		if (!cache || !cache->map) \
		{ \
			lrucache_error("Error [%2d]: Invalid lru cache\n", ERR_USER) \
			return ERR_USER; \
		} \
		name##_lrucache_link l; \
		int ret = hashmap_get(name, cache->map, key, &l); \
		if (ret == OK) \
		{ \
			if (pvalue) \
				*pvalue = l->value; \
			if (l->next) \
			{ \
				l->next->prev = l->prev; \
				if (l->prev) \
				{ \
					l->prev->next = l->next; \
				} \
				else \
				{ \
					cache->head = l->next; \
				} \
				cache->tail->next = l; \
				l->prev = cache->tail; \
				l->next = NULL; \
				cache->tail = l; \
			} \
		} \
		return ret; \
	} \
	\
	/* Putt cache
	 */\
	int name##_lrucache_put(name##_lrucache cache, \
		key_type key, value_type value) \
	{ \
		if (!cache || !cache->map) \
		{ \
			lrucache_error("Error [%2d]: Invalid lru cache\n", ERR_USER) \
			return ERR_USER; \
		} \
		name##_lrucache_link l; \
		name##_lrucache_link tmp; \
		int ret = hashmap_get(name, cache->map, key, &l); \
		printf("Get key: %s, ret: %d\n", key, ret); \
		if (ret == OK) \
		{ \
			l->value = value; \
			return OK; \
		} \
		if (hashmap_size(name, cache->map) >= cache->capacity) \
		{ \
			l = cache->head; \
			hashmap_remove(name, cache->map, l->key); \
			if (l->next) \
			{ \
				cache->head = l->next; \
				cache->head->prev = NULL; \
				cache->tail->next = l; \
				l->prev = cache->tail; \
				cache->tail = l; \
			} \
		} \
		else \
		{ \
			l = (name##_lrucache_link) malloc(sizeof(name##_lrucache_node)); \
			if (!cache->head) \
			{ \
				cache->head = l; \
			} \
			else \
			{ \
				cache->tail->next = l; \
			} \
			l->prev = cache->tail; \
			cache->tail = l; \
		} \
		l->key = key; \
		l->value = value; \
		l->next = NULL; \
		return hashmap_put(name, cache->map, key, l); \
	} \

#endif