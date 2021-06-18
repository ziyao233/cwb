/*
	cwb
	File:/Lib/Ds_Dictionary.c
	Date:2021.05.02
	By MIT and Anti-996 License.
	Copyright(C) 2021 cwb developers.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdarg.h>

#include"Ds.h"

typedef struct __Ds_Dictionary_Pair{
	union {
		char *s;
		int i;
	}key;
	void *data;
	struct __Ds_Dictionary_Pair *next;
}__Ds_Dictionary_Pair;

// This macro will be undefined in the end of this file
#define Pair __Ds_Dictionary_Pair

typedef struct {
	int keyType;	// String:0 Integer:1
	Pair *pair;
}__Ds_Dictionary;

typedef struct {
	Pair *last;
	Pair *present;
}__Ds_Dictionary_Pair_Pointer;

// This macro will be undefine in the end of this file
#define Dictionary __Ds_Dictionary
#define Pair_Pointer __Ds_Dictionary_Pair_Pointer

#define ISSKEY(dictionary) ((dictionary)->keyType == CWB_DS_SKEY)
#define ISIKEY(dictionary) ((dictionary)->keyType == CWB_DS_IKEY)

static void *dictionary_new(va_list argList)
{
	Dictionary *dictionary = (Dictionary*)(malloc(sizeof(Dictionary)));
	if (!dictionary) {
		return NULL;
	}

	*dictionary = (Dictionary){
					.keyType = va_arg(argList,int),
					.pair    = NULL,
				  };

	return dictionary;
}

static void dictionary_destroy(void *in)
{
	Dictionary *dictionary = (Dictionary*)in;
	
	/*
	 *	For dictionaries that are using strings as keys,
	 *	we need to free the keys.
	 */
	Pair *last = dictionary->pair;
	if (ISSKEY(dictionary)) {
		for (Pair *pair = last->next ? last->next : NULL;
		     pair;
		     pair = pair->next) {
			free(last->key.s);
			free(last);
			last=pair;
		}
	} else {
		for (Pair *pair = last->next ? last->next : NULL;
		     pair;
		     pair = pair->next) {
			free(last);
			last=pair;
		}
	}

	if (last)  {
		if (ISSKEY(dictionary))
			free(last->key.s);
		free(last);
	}

	free(in);
	
	return;
}

static Pair_Pointer dictionary_search_ikey(Dictionary *dictionary,int key)
{
	Pair_Pointer pointer={.present=dictionary->pair};
	while (pointer.present) {
		if (pointer.present->key.i == key)
			return pointer;

		pointer.last	= pointer.present;
		pointer.present = pointer.present->next;
	}
	return pointer;
}

static Pair_Pointer dictionary_search_skey(Dictionary *dictionary,const char *key)
{
	Pair_Pointer pointer={.present=dictionary->pair};
	while (pointer.present) {
		if (!strcmp(pointer.present->key.s,key))
			return pointer;

		pointer.last	= pointer.present;
		pointer.present = pointer.present->next;
	}

	return pointer;
}

static int dictionary_delete_ikey(Dictionary *dictionary,int key)
{
	Pair_Pointer pointer = dictionary_search_ikey(dictionary,key);
	if (!pointer.present) 
		return -1;
	
	if (pointer.last) {
		pointer.last->next = pointer.present->next;
	}
	free(pointer.present);
	return 0;
}

static int dictionary_delete_skey(Dictionary *dictionary,const char *key)
{
	Pair_Pointer pointer = dictionary_search_skey(dictionary,key);
	if (!pointer.present)
		return -1;
	
	if (pointer.last)
		pointer.last->next = pointer.present->next;
	free(pointer.present);

	return -1;
}

static int dictionary_set_ikey(Dictionary *dictionary,int key,void *value)
{
	Pair *pair = dictionary_search_ikey(dictionary,key).present;
	if (!pair) {
		pair = (Pair*)malloc(sizeof(Pair));
		if (!pair)
			return -1;
		
		pair->next	 = dictionary->pair;
		dictionary->pair = pair;
		pair->key.i	 = key;
	}

	pair->data = value;

	return 0;
}

static int dictionary_set_skey(Dictionary *dictionary,const char *key,
			       void *value) {
	Pair *pair = dictionary_search_skey(dictionary,key).present;
	if (!pair) {
		pair = (Pair*)malloc(sizeof(Pair));
		if (!pair)
			return -1;
		
		pair->next	 = dictionary->pair;
		dictionary->pair = pair;
		pair->key.s	 = (char*)malloc(strlen(key)+1);
		if (!(pair->key.s))
			return -1;
		strcpy(pair->key.s,key);
	}

	pair->data = value;

	return 0;
}

static int dictionary_set(void *in,va_list argList)
{
	Dictionary *dictionary = (Dictionary*)in;

	if (ISSKEY(dictionary)) {
		const char *key = va_arg(argList,const char*);
		void *data	= va_arg(argList,void*);
		return data==CWB_DS_UNDEFINED ?
				dictionary_delete_skey(dictionary,key) :
				dictionary_set_skey(dictionary,key,data); 
	} else {
		int key		= va_arg(argList,int);
		void *data	= va_arg(argList,void*);
		return data==CWB_DS_UNDEFINED ?
				dictionary_delete_ikey(dictionary,key) :
				dictionary_set_ikey(dictionary,key,data);
	}
}

static void *dictionary_get_ikey(Dictionary *dictionary,int key) {
	Pair_Pointer pointer = dictionary_search_ikey(dictionary,key);
	return pointer.present ? pointer.present->data : CWB_DS_UNDEFINED;
}

static void *dictionary_get_skey(Dictionary *dictionary,const char *key) {
	Pair_Pointer pointer = dictionary_search_skey(dictionary,key);
	return pointer.present ? pointer.present->data : CWB_DS_UNDEFINED;
}

static void *dictionary_get(void *in,va_list argList)
{
	Dictionary *dictionary = (Dictionary*)in;
	return ISSKEY(dictionary) ? 
			dictionary_get_skey(dictionary,
					    va_arg(argList,const char*))
				  :
			dictionary_get_ikey(dictionary,
					    va_arg(argList,int));
}

#undef Pair
#undef Dictionary
#undef Pair_Pointer
