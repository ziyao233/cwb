#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<stdint.h>

#define HEXCH2NUM(ch) ((ch)>57?(ch)-55:(ch)-48)

static inline int uri_needcode(uint8_t ch)
{
	return isalnum((char)ch) ||
		   ch=='.' || ch=='-' || ch=='_' || ch=='~';
}

char *cwb_encode_uri(const void *data,size_t length,
		     void *output,size_t bufSize)
{
	char *result=output?output:(char*)malloc(sizeof(char)*length*3+1);
	if(!result)
		return NULL;
	if(bufSize<length*3)
		return NULL;

	char *dest=result;
	static const char *changeTable="0123456789ABCDEF";
	uint8_t *p=(uint8_t*)data;
	for(size_t i=0;i<length;i++)
	{
		if(!uri_needcode(*p))
		{
			*dest='%';
			dest[1]=changeTable[*p/16];
			dest[2]=changeTable[*p%16];
			dest+=3;
		}
		else
		{
			*dest=*p;
			dest++;
		}
		p++;
	}

	*dest='\0';

	return result;
}
