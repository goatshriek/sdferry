#ifndef __3RDPARTY_H
#define __3RDPARTY_H

#include <stdint.h>

struct teec_value_clone {
  uint32_t a;
  uint32_t b;
};

struct basic_types {
  char c;
  uint32_t i;
  // long l;
  // float f;
};

struct pointer_types {
  char *str;
  char *buff;
  uint32_t buff_len;
};

struct rollup_type {
  struct basic_types *basic;
  struct pointer_types *pointer;
};

#endif /* __3RDPARTY_H */
