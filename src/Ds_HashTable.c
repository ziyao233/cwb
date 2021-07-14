/*
	cwb
	File:/src/Ds_HashTable.c
	Date:2021.07.14
	By MIT License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"cwb/Ds.h"

// Overload: slotNum*0.75 < usedNum
#define OVERLOAD(ht) (((ht)->slotNum >> 1) + ((ht)->slotNum >> 2) <= ((ht)->slotUsed))
#define create_slotvalue() ((Ht_SlotValue*)malloc(sizeof(Ht_SlotValue)))

#define __Ds_Ht_Slot Ht_Slot
#define __Ds_HashTable HashTable
#define __Ds_Ht_SlotValue Ht_SlotValue

typedef struct __Ds_Ht_SlotValue{
	char *key;
	void *data;
	struct __Ds_Ht_SlotValue *next,*prev;
}__Ds_Ht_SlotValue;

typedef struct {
	Ht_SlotValue *value;
}__Ds_Ht_Slot;

typedef struct {
	uint32_t slotNum,slotUsed;
	Ht_Slot *slot;
	Cwb_Ds_FreeFunc free;
}__Ds_HashTable;

static uint32_t ht_hash(char const *str,size_t length)
{
	uint32_t hash = (uint32_t)length;
	
	while (*str) {
		hash = (hash << 4) - (hash >> 3) + *str;
		str++;
	}

	return hash;
}

static void *hashtable_new(va_list argList)
{
	int keyType = va_arg(argList,int);
	assert(keyType == CWB_DS_SKEY);

	HashTable *ht = (HashTable*)malloc(sizeof(HashTable));
	if (!ht)
		return NULL;
	
	*ht = (HashTable) {
				.slotNum = 0,
				.slotUsed= 0,
				.slot	 = NULL
			  };
	
	return ht;
}

static int ht_init_slot(HashTable *ht)
{
	// Prealloc four slots
	*ht = (HashTable){
				.slotNum = 4,
				.slotUsed= 0,
				.slot	 = (Ht_Slot*)malloc(sizeof(Ht_Slot) << 2)
			 };
	if (!ht->slot)
		return -1;

	memset(ht->slot,0,sizeof(Ht_Slot) << 2);

	return 0;
}

static void hashtable_destroy(void *ds)
{
	HashTable *ht = (HashTable*)ds;

	for (uint32_t i = 0;i < ht->slotNum;i++) {
		Ht_SlotValue *tmp = NULL;
		for (Ht_SlotValue *slotValue = ht->slot[i].value;
		     slotValue;
		     slotValue = tmp) {
			tmp = slotValue->next;
			free(slotValue->key);
			if (ht->free)
				free(slotValue->data);
			free(slotValue);
		}
	}
	
	free(ht->slot);
	free(ht);

	return;
}

static Cwb_Ds_Pair *hashtable_search(void *ds,va_list argList)
{
	HashTable *ht = (HashTable*)ds;
	if (ht->slotNum == 0)
		return NULL;

	char const *key = va_arg(argList,char const*);

	uint32_t keyHash = ht_hash(key,strlen(key)) % (ht->slotNum);
	for (Ht_SlotValue *slotValue = ht->slot[keyHash].value;
	     slotValue;
	     slotValue = slotValue->next) {
		if (!strcmp(key,slotValue->key)) {
			return (Cwb_Ds_Pair*)slotValue;
		}
	}

	return NULL;
}

static int ht_extend_slot(HashTable *ht)
{
	/*	Create new slots	*/
	ht->slotNum = ht->slotNum << 1;
	Ht_Slot *newSlot = (Ht_Slot*)malloc(sizeof(Ht_Slot)*(ht->slotNum));
	if (!newSlot)
		return -1;
	memset(newSlot,0,sizeof(Ht_Slot)*(ht->slotNum));
	
	/*	Rehash the old slots	*/
	// For all the old slots
	for (uint32_t i = 0;i < (ht->slotNum >> 1);i++) {
		Ht_SlotValue *tmp = NULL;
		for (Ht_SlotValue *slotValue = ht->slot[i].value;
		     slotValue;
		     slotValue = tmp) {
			char const *key = slotValue->key;
			uint32_t hash = ht_hash(key,strlen(key)) % (ht->slotNum);
			
			tmp = slotValue->next;
			slotValue->next = newSlot[hash].value;
			slotValue->prev = NULL;
			newSlot[hash].value = slotValue;
		}
	}

	/*	Release the old slots and set the new ones	*/
	free(ht->slot);
	ht->slot = newSlot;
	return 0;
}

static char *copy_str(char const *str)
{
	char *result = (char*)malloc(strlen(str)+1);
	if (!result)
		return NULL;
	strcpy(result,str);
	return result;
}

static Cwb_Ds_Pair *hashtable_insert(void *ds,va_list argList)
{
	HashTable *ht	= (HashTable*)ds;
	char *key = copy_str(va_arg(argList,char const*));
	void *value	= va_arg(argList,void*);
	if (!key)
		return NULL;

	if (ht->slotNum == 0) {
		if (ht_init_slot(ht))
			return NULL;
	}

	if (OVERLOAD(ht)) {
		if (ht_extend_slot(ht))
			return NULL;
	}

	uint32_t keyHash = ht_hash(key,strlen(key)) % (ht->slotNum);
	Ht_SlotValue *slotValue = create_slotvalue();
	if (!slotValue)
		return NULL;
	
	slotValue->data	= value;
	slotValue->prev = NULL;
	slotValue->next = ht->slot[keyHash].value;
	slotValue->key	= key;
	if (ht->slot[keyHash].value) {
		ht->slot[keyHash].value->prev = slotValue;
		ht->slot[keyHash].value = slotValue;
	}
	ht->slot[keyHash].value = slotValue;

	ht->slotUsed++;

	return (Cwb_Ds_Pair*)slotValue;
}

static void *hashtable_get(void *ds,Cwb_Ds_Pair *pair)
{
	(void)ds;
	return ((Ht_SlotValue*)pair)->data;
}

static int hashtable_set(void *ds,Cwb_Ds_Pair *pair,void *data)
{
	(void)ds;
	((Ht_SlotValue*)pair)->data = data;
	return 0;
}

static int hashtable_delete(void *ds,Cwb_Ds_Pair *pair)
{
	HashTable *ht = (HashTable*)ds;
	Ht_SlotValue *slotValue = (Ht_SlotValue*)pair;

	if (slotValue->prev)
		slotValue->prev->next = slotValue->next;
	if (slotValue->next)
		slotValue->next->prev = slotValue->prev;
	
	free(slotValue);

	ht->slotUsed--;

	return 0;
}

static Cwb_Ds_Pair *hashtable_first(void *ds)
{
	HashTable *ht = (HashTable*)ds;

	for (uint32_t i = 0;i < ht->slotNum;i++) {
		if (ht->slot[i].value)
			return (Cwb_Ds_Pair*)(ht->slot[i].value);
	}
	
	return NULL;
}

static Cwb_Ds_Pair *hashtable_next(void *ds,Cwb_Ds_Pair *pair)
{
	HashTable *ht = (HashTable*)ds;

	Ht_SlotValue *slotValue = (Ht_SlotValue*)pair;
	if (slotValue->next)
		return (Cwb_Ds_Pair*)slotValue->next;
	
	for (uint32_t i = ht_hash(slotValue->key,strlen(slotValue->key));
	     i < ht->slotNum;
	     i++) {
		if (ht->slot[i].value)
			return (Cwb_Ds_Pair*)(ht->slot[i].value);
	}
	
	return NULL;
}

static void hashtable_freefunc(void *ds,Cwb_Ds_FreeFunc freeFunc)
{
	((HashTable*)ds)->free = freeFunc;
	return;
}

static intptr_t hashtabel_getkey(void *ds,Cwb_Ds_Pair *pair)
{
	(void)ds;
	return (intptr_t)(((Ht_SlotValue*)pair)->key);
}
