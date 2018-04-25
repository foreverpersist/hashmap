#include <stdio.h>
#include "hashmap.h"

int print_str_iter(const char * key, int value, void *args)
{
	printf("[Foreach] key: %s, value-1: %d\n", key, value-1);
	return 0;
}

HASHMAP_DEFINE(STR, const char *, int)

int main(int argc, char *argv)
{
	int status;
	const char *k;
	int v;
	int size;
	double mean;
	double variance;

	int i;
	iter(STR) it;
	char* test_key[] = {"persist", "joy", "allen", "ever"};
	int test_value[] = {0, 1, 2, 3};

	printf("Init\n");
	hashmap(STR) map = hashmap_create(STR, 0, 0);
	hashmap_set_hash_func(STR, map, str_hash);
	hashmap_set_compare_func(STR, map, strcmp);
	hashmap_set_key_funcs(STR, map, str_key_alloc, str_key_free);

	printf("Put\n");
	for (i = 0; i < 4; i++)
	{
		hashmap_put(STR, map, test_key[i], test_value[i]);
		printf("[Put] key: %s, value: %d\n", test_key[i], test_value[i]);
	}


	printf("Remove\n");
	hashmap_remove(STR, map, test_key[2]);
	printf("[Remove] key: %s\n", test_key[2]);

	printf("Size\n");
	size = hashmap_size(STR, map);
	printf("[Size] size: %d\n", size);

	printf("Collision\n");
	mean = hashmap_collision_mean(STR, map);
	variance = hashmap_collision_variance(STR, map);
	printf("[Collision] mean: %f, variance: %f\n", mean, variance);

	printf("Iterate\n");
	for (it = hashmap_iter(STR, map); it != NULL; it = hashmap_next(STR, map, it))
	{
		hashmap_iter_get(STR, it, &k, NULL);
		hashmap_iter_get(STR, it, NULL, &v);
		printf("[Iterate] key: %s, value: %d\n", k, v);
	}

	printf("Foreach\n");
	hashmap_foreach(STR, map, print_str_iter, NULL);

	printf("Has_Key & Get\n");
	printf("Note: if key don't exists, value will not be return.\n");
	for (i = 0; i < 4; i++)
	{
		status = hashmap_has_key(STR, map, test_key[i]);
		printf("[Has_Key] key: %s, status: %d\n", test_key[i], status);
		hashmap_get(STR, map, test_key[i], &v);
		printf("[Get] key: %s, value: %d\n", test_key[i], v);
	}

	printf("Destroy\n");
	hashmap_destroy(STR, map);

	return 0;
}
