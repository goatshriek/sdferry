#include <stdint.h>
#include <sdferry/basic.h>
#include <sdferry/memory.h>

/*
 * we implement this to avoid the need for a library call
 */
uint32_t sdferry_memcpy(void *dest, void *source, uint32_t size);
uint32_t sdferry_memcpy(void *dest, void *source, uint32_t size){
  uint32_t i=0;

  while(i < size){
    ((char *)(dest))[i] = ((char *)(source))[i];
    i++;
  }

  return i;
}

/*
 * basic char types
 */
uint32_t __sdferry_send_char(char *data, char c, uint32_t buffer_size){
  (void)buffer_size;

  if(!data){
    return 0;
  }

  *data = c;

  return sizeof(c);
}

uint32_t __sdferry_receive_char(char *data, void *val){
  char *result;

  if(!data){
    return 0;
  }

  result = (char *)val;
  *result = *data;

  return sizeof(*result);
}

char __sdferry_alloc_char(void){
  return '\0';
}

void __sdferry_free_char(char c){
  (void)c;
  return;
}

/*
 * the uint32_t type
 */
uint32_t __sdferry_send_uint32_t(char *data, uint32_t i, uint32_t buffer_size){
  (void)buffer_size;

  if(!data){
    return 0;
  }

  return sdferry_memcpy(data, &i, sizeof(i));
}

uint32_t __sdferry_receive_uint32_t(char *data, void *val){
  if(!data || !val){
    return 0;
  }

  return sdferry_memcpy(val, data, sizeof(uint32_t));
}

uint32_t __sdferry_alloc_uint32_t(void){
  return 0;
}

void __sdferry_free_uint32_t(uint32_t i){
  (void)i;
  return;
}

/*
 * NULL-terminated C string
 */
uint32_t __sdferry_send_cstring(char *data, char *str, uint32_t buffer_size){
  uint32_t i=0;

  (void)buffer_size;

  if(!data){
    return 0;
  }

  while(str[i] != '\0'){
    data[sizeof(i)+i] = str[i];
    i++;
  }

  if(__sdferry_send_uint32_t(data, i, 0) == 0){
    return 0;
  } else {
    return sizeof(i)+i;
  }
}

uint32_t __sdferry_receive_cstring(char *data, void *val){
  uint32_t i, str_len, result_len;
  char **str = (char **)val;

  if(!data || !val || !str){
    return 0;
  }

  result_len = __sdferry_receive_uint32_t(data, &str_len);

  *str = __sdferry_alloc_mem(str_len+1);
  if(*str == NULL){
    return 0;
  }

  for(i=0; i < str_len; i++){
    (*str)[i] = data[i+result_len];
  }
  (*str)[str_len] = '\0';

  return result_len+str_len;
}

char *__sdferry_alloc_cstring(void){
  return NULL;
}

void __sdferry_free_cstring(char *str){
  __sdferry_free_mem(str);
}

/*
 * generic buffers
 */
uint32_t __sdferry_send_buffer(char *data, char *buffer, uint32_t buffer_size){
  uint32_t i;

  if(!data){
    return 0;
  }

  if(__sdferry_send_uint32_t(data, buffer_size, 0) == 0){
    return 0;
  }

  for(i=0; i<buffer_size; i++){
    data[sizeof(buffer_size)+i] = buffer[i];
  }

  return sizeof(buffer_size)+buffer_size;
}

uint32_t __sdferry_receive_buffer(char *data, void *val){
  uint32_t i, buffer_len;
  char **buffer = (char **)val;

  if(!data || !buffer){
    return 0;
  }

  if(__sdferry_receive_uint32_t(data, &buffer_len) == 0){
    return 0;
  }

  *buffer = __sdferry_alloc_mem(buffer_len);
  if(*buffer == NULL){
    return 0;
  }

  for(i=0; i < buffer_len; i++){
    (*buffer)[i] = data[i+sizeof(buffer_len)];
  }

  return sizeof(buffer_len)+buffer_len;
}
char *__sdferry_alloc_buffer(void){
  return NULL;
}

void __sdferry_free_buffer(char *buffer){
  __sdferry_free_mem(buffer);
}

/*
 * size_t type
 */
uint32_t __sdferry_send_size_t(char *data, size_t i, uint32_t buffer_size){
  (void) buffer_size;
  (void) i;

  if(!data){
    return 0;
  }

  return 0;
}

uint32_t __sdferry_receive_size_t(char *data, void *val){
  (void)val;

  if(!data){
    return 0;
  }

  return 0;
}

size_t __sdferry_alloc_size_t(void){
  return 0;
}

void __sdferry_free_size_t(size_t i){
  (void)i;
  return;
}
