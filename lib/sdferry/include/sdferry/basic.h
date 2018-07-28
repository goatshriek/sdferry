#ifndef __SDFERRY_BASIC_H
#define __SDFERRY_BASIC_H

/*
 * We provide some basic send, receive, alloc, and free functions.
 * If others are needed, they can be directly implemented by an application
 * developer.
 *
 * The 'send' class of functions takes three arguments:
 *   the buffer to copy the values into
 *   the value to copy
 *   the size of the buffer supplied (not used unless the type is a buffer)
 *
 * The return value describes the number of chars needed to store the
 * information into the buffer. This allows the buffer to be used for multiple
 * types with the return value as the offset of the next value.
 *
 * The 'receive' family of functions does the opposite of the copy functions,
 * pulling the values out of a buffer and back into their original type. The
 * parameters of this class of functions are as follows:
 *   the buffer containing the data to be reconstituted
 *   a pointer to the type that is being retrieved
 *
 * The return value describes the number of chars read from the buffer in order
 * reconstitute this struct. This allows the buffer pointer to be incremented
 * in order to read the next member of the struct.
 *
 * A third family of functions, the 'alloc' group, is used to allocate memory
 * for a member if needed. For primitive data types this is unnecessary, but
 * for types such as nested structs, the struct itself must be allocated. In
 * other cases this may be necessary during struct reconstitution.
 *
 * The fourth and final group of functions is also defined, the 'free' family.
 * These are used to free any memory allocated by the receive function when it
 * created the struct from the buffer. If this is not necessary, the function
 * must still be defined, but it can simply return with no action taken. This is
 * the obvious partner to the alloc group of functions.
 * 
 * If a custom type is needed, or a struct that cannot be adequately described
 * using the REGISTER family of functions, then these three functions will need
 * to be defined for the type to allow it to be used in SDFerry, where <name> is
 * the name used to refer to the type in invocation function calls:
 *
 *   uint32_t __sdferry_send_<name>(char *data, void *val, uint32_t member_size)
 *   uint32_t __sdferry_receive_<name>(char *data, void *val)
 *   void * __sdferry_alloc_<name>(void)
 *   void __sdferry_free_<name>(void *val)
 *
 * The behavior of each of these functions will need to match that described for
 * the basic types above.
 */

#include <stddef.h>
#include <stdint.h>

uint32_t __sdferry_send_char(char *data, char c, uint32_t buffer_size);
uint32_t __sdferry_receive_char(char *data, void *val);
char __sdferry_alloc_char(void);
void __sdferry_free_char(char c);

uint32_t __sdferry_send_uint32_t(char *data, uint32_t i, uint32_t buffer_size);
uint32_t __sdferry_receive_uint32_t(char *data, void *val);
uint32_t __sdferry_alloc_uint32_t(void);
void __sdferry_free_uint32_t(uint32_t i);

uint32_t __sdferry_send_cstring(char *data, char *str, uint32_t buffer_size);
uint32_t __sdferry_receive_cstring(char *data, void *val);
char *__sdferry_alloc_cstring(void);
void __sdferry_free_cstring(char *str);

uint32_t __sdferry_send_buffer(char *data, char *buffer, uint32_t buffer_size);
uint32_t __sdferry_receive_buffer(char *data, void *val);
char *__sdferry_alloc_buffer(void);
void __sdferry_free_buffer(char *buffer);

uint32_t __sdferry_send_size_t(char *data, size_t i, uint32_t buffer_size);
uint32_t __sdferry_receive_size_t(char *data, void *val);
size_t __sdferry_alloc_size_t(void);
void __sdferry_free_size_t(size_t i);

#endif /* __SDFERRY_BASIC_H */
