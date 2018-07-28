#include <stdlib.h>
#include <sdferry/memory.h>

void *__sdferry_alloc_mem(size_t size){
  return malloc(size);
}

void __sdferry_free_mem(void *mem){
  free(mem);
}
