#include "lrucache.h"

LRUCACHE_DEFINE(STR, const char *, int)

int main(int argc, char *argv[])
{
	int i;
	int status;
	int v = -1;
	char* test_key[] = {"persist", "joy", "allen", "ever", "a", "b", "c"};
	int test_value[] = {0, 1, 2, 3, 4, 5};

	printf("Init\n");
	lrucache(STR) cache = lrucache_create(STR, 5);
	lrucache_set_hash_func(STR, cache, str_hash);
	lrucache_set_compare_func(STR, cache, strcmp);
	lrucache_set_key_funcs(STR, cache, str_key_alloc, str_key_free);
	printf("Cache: %p\n", cache);

	printf("Put\n");
	for (i = 0; i < 6; i++)
	{
		lrucache_put(STR, cache, test_key[i], test_value[i]);
		printf("[Put] key: %s, value: %d\n", test_key[i], test_value[i]);
	}

	printf("Get\n");
	printf("Note: if key don't exists, value will not be return.\n");
	for (i = 0; i < 6; i++)
	{
		status = lrucache_get(STR, cache, test_key[i], &v);
		printf("[Get] key: %s, value: %d, status: %d\n", test_key[i], v, status);
	}

	printf("Destroy\n");
	lrucache_destroy(STR, cache);
	
	return 0;
}