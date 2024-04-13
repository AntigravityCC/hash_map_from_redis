#include <stdio.h>
#include <stdint.h>
#include "dict.h"

#define sdslen strlen

/* -------------------------- hash functions -------------------------------- */

/* Generic hash function (a popular one from Bernstein).
 * I tested a few and this was the best. */
// static unsigned int dictGenHashFunction(const unsigned char *buf, int len) {
//     unsigned int hash = 5381;

//     while (len--)
//         hash = ((hash << 5) + hash) + (*buf++); /* hash * 33 + c */
//     return hash;
// }


uint64_t dictSdsHash(const void *key) {
    return dictGenHashFunction((unsigned char*)key, sdslen((char*)key));
}

int dictSdsKeyCompare(void *privdata, const void *key1,
        const void *key2)
{
    int l1,l2;
    DICT_NOTUSED(privdata);

    // l1 = sdslen((sds)key1);
    // l2 = sdslen((sds)key2);
    // if (l1 != l2) return 0;
    return memcmp(key1, key2, l1) == 0;
}

/* Dict type without destructor */
dictType sdsReplyDictType = {
    dictSdsHash,                /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    dictSdsKeyCompare,          /* key compare */
    NULL,                       /* key destructor */
    NULL,                       /* val destructor */
    NULL                        /* allow to expand */
};

#define serverAssert(_e) (_e)?(void)0 : (fprintf(stderr, "==> '%s' is not true [%s:%d]\n", #_e,__FILE__,__LINE__))

typedef struct _MapEntry 
{
    char *key;
    char *value;
}MapEntry_t;


int main()
{
    int index = 0;
    dict *dictHd = dictCreate(&sdsReplyDictType, NULL);
    if(NULL == dictHd)
    {
        printf("NULL == entry\n");
        return -1;
    }

    dictExpand(dictHd, 1024);
    MapEntry_t mapEntry[] = 
    {
        [0] = {"key1", "1"},
        [1] = {"key2", "2"},
        [2] = {"key3", "3"},
    };

    int numcommands = sizeof(mapEntry)/sizeof(MapEntry_t);
    printf("numcommands %d \n", numcommands);
    for(index = 0; index < numcommands; index++)
    {
        printf("add:  key: %s, value: %s\n", mapEntry[index].key, mapEntry[index].value);
        int retVal  = dictAdd(dictHd, (void *)mapEntry[index].key, (void *)mapEntry[index].value);
        serverAssert(retVal == DICT_OK);
    }

    dictEntry *find = NULL;

    find =  dictFind(dictHd, mapEntry[0].key);
    serverAssert(find != NULL); printf("key %s, value %s\n", mapEntry[0].key, find->v.val);
    if (find != NULL)
        

    dictReplace(dictHd, mapEntry[0].key, "11");
    find =  dictFind(dictHd, mapEntry[0].key);
    serverAssert(find != NULL);
    if (find != NULL) printf("key %s, value %s\n", mapEntry[0].key, find->v.val);

    dictDelete(dictHd, mapEntry[0].key);
    find =  dictFind(dictHd, mapEntry[0].key);
    serverAssert(find != NULL);
    if (find != NULL) printf("key %s, value %s\n", mapEntry[0].key, find->v.val);

    find =  dictFind(dictHd, mapEntry[2].key);
    serverAssert(find != NULL);
    if (find != NULL) printf("key %s %s, value %s\n", mapEntry[2].key, find->key,find->v.val);

    return 0;
}
