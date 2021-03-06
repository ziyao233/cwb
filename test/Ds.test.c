/*
	Cwb
	File:/test/Ds.test.c
	Date:2021.10.24
	By MIT License.
	Copyright(C) 2021 Cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include<cwb/Ds.h>

#include"Test.h"

#define TEST_PAIR_NUM 1024
#define TEST_DATA_SIZE 64

typedef struct {
	char *key;
	void *data;
}Data_Pair;

static Data_Pair *spawn_pair(void)
{
	Data_Pair *pairs = (Data_Pair*)malloc(sizeof(Data_Pair)*TEST_PAIR_NUM);
	assert(pairs);

	for (int count = 0;count < TEST_PAIR_NUM;count++) {
		pairs[count] = (Data_Pair) {
						.key = test_random_str(rand()%128+1),
						.data= test_random_data(TEST_DATA_SIZE)
					   };
		assert(pairs[count].key);
		assert(pairs[count].data);
	}
	return pairs;
}

static void destroy_pair(Data_Pair *pair)
{
	for (int i = 0;i < TEST_PAIR_NUM;i++) {
		free(pair[i].key);
		free(pair[i].data);
	}
	free(pair);
	return;
}


static void test_ds_skey(Cwb_Ds *ds)
{
	Data_Pair *pair = spawn_pair();

	for (int i = 0;i < TEST_PAIR_NUM;i++) {
		assert(cwb_ds_insert(ds,pair[i].key,pair[i].data));
	}

	for (int i = 0;i < TEST_PAIR_NUM;i++) {
		Cwb_Ds_Iter *p	= cwb_ds_search(ds,pair[i].key);
		assert(p);
		void *data	= cwb_ds_get(ds,p);
		assert(!memcmp(data,pair[i].data,TEST_DATA_SIZE));
	}

	destroy_pair(pair);
	cwb_ds_destroy(ds);

	return;
}

int main(void)
{
	test_init();

	test_ds_skey(cwb_ds_new(CWB_DS_HASHTABLE,CWB_DS_SKEY));

	return 0;
}
