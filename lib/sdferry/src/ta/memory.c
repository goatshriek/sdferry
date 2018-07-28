#include <tee_api.h>
#include <sdferry/memory.h>

void *__sdferry_alloc_mem(size_t size){
  return TEE_Malloc(size, TEE_MALLOC_FILL_ZERO);
}

void __sdferry_free_mem(void *mem){
  TEE_Free(mem);
}
