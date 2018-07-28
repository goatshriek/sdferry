#ifndef __SDFERRY_MEMORY_H
#define __SDFERRY_MEMORY_H

/*
 * The __sdferry_alloc_mem and __sdferry_free_mem functions are declared so that
 * memory allocation and release operations can be carried out. These are
 * defined in the host and ta source trees separately, as there are separate
 * mechanisms for allocation in each of these. This is similar to the strategy
 * used for architecture-specific code in other applications.
 */

void *__sdferry_alloc_mem(size_t size);
void __sdferry_free_mem(void *mem);

#endif /* __SDFERRY_MEMORY_H */
