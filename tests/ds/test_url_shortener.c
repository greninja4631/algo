#include <stddef.h>
#include "ds/url_shortener.h"
#include "util/logger.h"
#include <assert.h>
#include <string.h>

/*  DI allocator  */
static void* _alloc(size_t n,size_t sz){ return n?calloc(n,sz):NULL; }
static void  _free (void* p){ if(p) free(p); }
static const ds_allocator_t G_ALLOC_IMPL = { .alloc=_alloc, .free=_free };
#define G_ALLOC (&G_ALLOC_IMPL)

/*  tiny assert  */
#define TASSERT(c,m) do{ if(c) ds_log(DS_LOG_LEVEL_INFO,"[PASS] %s",(m)); \
                         else { ds_log(DS_LOG_LEVEL_ERROR,"[FAIL] %s",(m)); assert(0);} }while(0)

void test__url_shortener_basic(void)
{
    ds_url_shortener_t *us=NULL;
    TASSERT(ds_url_shortener_create(G_ALLOC, 16, &us)==DS_SUCCESS,"create");

    char short_id[8], url_out[64];
    const char *orig = "https://example.com/aaaaaaaa";

    TASSERT(ds_url_shortener_shorten(G_ALLOC, us, orig, short_id,sizeof short_id)==DS_SUCCESS,"shorten");
    TASSERT(ds_url_shortener_expand (G_ALLOC, us, short_id, url_out,sizeof url_out)==DS_SUCCESS,"expand");
    TASSERT(strcmp(orig,url_out)==0,"round-trip");

    ds_url_shortener_destroy(G_ALLOC, us);
}