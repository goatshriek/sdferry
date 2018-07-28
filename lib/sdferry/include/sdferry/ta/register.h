#ifndef __SDFERRY_TA_REGISTER_H
#define __SDFERRY_TA_REGISTER_H

/*
 * The SDFERRY_REGISTER_RECEIVE_COMMAND function defines a function that checks
 * and unwraps the parameters of an invocation, reconstitutes a struct from the
 * shared buffer provided as the first argument, and invokes the given function
 * with the given struct. After the invocation the memory related to the struct
 * is released, and the result of the function is returned.
 *
 * The registered function must return a TEE_Result describing the result, and
 * have a single parameter that is a pointer to a struct of the expected type.
 *
 * Note that this means that the called function should NOT store a pointer to
 * the struct or any of its members for later use, as they will be released and
 * will not be valid after the invocation. If information from the struct is
 * needed, it must be copied to some separate data structure for later use.
 */
#define SDFERRY_REGISTER_RECEIVE_COMMAND(TYPE, FUNCTION)                       \
TEE_Result __sdferry_send_to_##FUNCTION(uint32_t param_types, TEE_Param params[4]); \
TEE_Result __sdferry_send_to_##FUNCTION(uint32_t param_types, TEE_Param params[4]){ \
  struct TYPE thing;                                                           \
  TEE_Result result;                                                           \
  uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE); \
                                                                               \
  if (param_types != exp_param_types){                                         \
    return TEE_ERROR_BAD_PARAMETERS;                                           \
  }                                                                            \
                                                                               \
  SDFERRY_RECEIVE_STRUCT(TYPE, params[0].memref.buffer, &thing);               \
                                                                               \
  result = FUNCTION(&thing);                                                   \
  SDFERRY_FREE_STRUCT_MEMBERS(TYPE, &thing);                                   \
  return result;                                                               \
}

/*
 * The SDFERRY_REGISTER_SEND_COMMAND function defines a function that checks
 * and unwraps the parameters of an invocation, calls a given function, and 
 * serializes the resulting struct to send back to the normal world.
 *
 * The registered function must return a TEE_Result describing the result, and
 * have a single parameter that is a pointer to a struct of the expected type.
 * The struct will be a valid memory reference, but any members must have their
 * memory allocated by the function if they are to be populated. They will be
 * freed by the wrapper function, assuming that the struct has been properly
 * registered with SDFerry.
 */
#define SDFERRY_REGISTER_SEND_COMMAND(TYPE, FUNCTION)                          \
TEE_Result __sdferry_receive_from_##FUNCTION(uint32_t param_types, TEE_Param params[4]); \
TEE_Result __sdferry_receive_from_##FUNCTION(uint32_t param_types, TEE_Param params[4]){ \
  struct TYPE thing;                                                           \
  TEE_Result result;                                                           \
  uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE); \
                                                                               \
  if (param_types != exp_param_types){                                         \
    return TEE_ERROR_BAD_PARAMETERS;                                           \
  }                                                                            \
                                                                               \
  result = FUNCTION(&thing);                                                   \
                                                                               \
  SDFERRY_SEND_STRUCT(TYPE, params[0].memref.buffer, &thing);                  \
                                                                               \
  SDFERRY_FREE_STRUCT_MEMBERS(TYPE, &thing);                                   \
  return result;                                                               \
}

/* 
 * The SDFERRY_REGISTER_SEND_AND_RECEIVE_COMMAND function acts as a combination
 * of the SDFERRY_REGISTER_RECEIVE_COMMAND and SDFERRY_REGISTER_SEND_COMMAND
 * functions. The wrapped function will have two parameters, the first being
 * the struct that is received, and the second being the struct that will be
 * sent back to the normal world. All other behavior is identical to the other
 * two functions.
 */
#define SDFERRY_REGISTER_SEND_AND_RECEIVE_COMMAND(IN_TYPE, OUT_TYPE, FUNCTION) \
TEE_Result __sdferry_send_and_receive_from_##FUNCTION(uint32_t param_types, TEE_Param params[4]); \
TEE_Result __sdferry_send_and_receive_from_##FUNCTION(uint32_t param_types, TEE_Param params[4]){ \
  struct IN_TYPE in_thing;                                                     \
  struct OUT_TYPE out_thing;                                                   \
  TEE_Result result;                                                           \
  uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE); \
                                                                               \
  if (param_types != exp_param_types){                                         \
    return TEE_ERROR_BAD_PARAMETERS;                                           \
  }                                                                            \
                                                                               \
  SDFERRY_RECEIVE_STRUCT(IN_TYPE, params[0].memref.buffer, &in_thing);         \
                                                                               \
  result = FUNCTION(&in_thing, &out_thing);                                    \
                                                                               \
  SDFERRY_SEND_STRUCT(OUT_TYPE, params[1].memref.buffer, &out_thing);          \
                                                                               \
  SDFERRY_FREE_STRUCT_MEMBERS(IN_TYPE, &in_thing);                             \
  SDFERRY_FREE_STRUCT_MEMBERS(OUT_TYPE, &out_thing);                           \
  return result;                                                               \
}

/*
 * The SDFERRY_SEND_STRUCT_TO_COMMAND, SDFERRY_RECEIVE_STRUCT_FROM_COMMAND, and
 * SDFERRY_SEND_AND_RECEIVE_STRUCT_FROM_COMMAND functions can be used to invoke
 * functions registered by the SDFERRY_REGISTER_RECEIVE_COMMAND,
 * SDFERRY_REGISTER_SEND_COMMAND, and SDFERRY_REGISTER_SEND_AND_RECEIVE_COMMAND
 * functions, respectively. They should be used instead of invoking the
 * underlying functions defined by these macros so that the name mangling scheme
 * used by SDFerry remains transparent in the source code.
 */
#define SDFERRY_SEND_STRUCT_TO_COMMAND(FUNCTION, param_types, params) __sdferry_send_to_##FUNCTION(param_types, params)
#define SDFERRY_RECEIVE_STRUCT_FROM_COMMAND(FUNCTION, param_types, params) __sdferry_receive_from_##FUNCTION(param_types, params)
#define SDFERRY_SEND_AND_RECEIVE_STRUCT_FROM_COMMAND(FUNCTION, param_types, params) __sdferry_send_and_receive_from_##FUNCTION(param_types, params)

#endif /* __SDFERRY_TA_REGISTER_H */
