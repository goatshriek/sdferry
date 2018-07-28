#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <sdferry_test_ta.h>

#include <sdferry_ta.h>
#include <3rdparty.h>

static uint32_t send_buffer_size_i=1;
static uint32_t send_buffer_size_j=0;

// declare the structures that we'll use
// this would be done in some common header file in larger applications
SDFERRY_DECLARE_STRUCT(basic_types)
SDFERRY_DECLARE_STRUCT(pointer_types)
SDFERRY_DECLARE_STRUCT(rollup_type)
SDFERRY_DECLARE_STRUCT(teec_value_clone)

TEE_Result test_receive_basic_types(struct basic_types *bt);
TEE_Result test_send_basic_types(struct basic_types *bt);
TEE_Result test_receive_pointer_types(struct pointer_types *pt);
TEE_Result test_send_pointer_types(struct pointer_types *pt);
TEE_Result test_receive_rollup_type(struct rollup_type *rt);
TEE_Result test_send_rollup_type(struct rollup_type *rt);
TEE_Result test_send_and_receive(struct basic_types *bt, struct pointer_types *pt);
TEE_Result receive_performance_vs_size(struct pointer_types *pt);
TEE_Result send_performance_vs_size(struct pointer_types *pt);
TEE_Result native_receive_performance(uint32_t param_types, TEE_Param params[4]);
TEE_Result sdferry_receive_performance(struct teec_value_clone *tee_value);
TEE_Result native_send_performance(uint32_t param_types, TEE_Param params[4]);
TEE_Result sdferry_send_performance(struct teec_value_clone *tee_value);

// register the third party structures we'll use
SDFERRY_REGISTER_STRUCT2(basic_types, i, uint32_t, i, c, char, c)
SDFERRY_REGISTER_STRUCT3(pointer_types, str, cstring, buff_len, buff_len, uint32_t, buff_len, buff, buffer, buff_len)
SDFERRY_REGISTER_NOBUFFER_STRUCT2(
  rollup_type,
  basic, basic_types,
  pointer, pointer_types
)

SDFERRY_REGISTER_NOBUFFER_STRUCT2(
  teec_value_clone,
  a, uint32_t,
  b, uint32_t
)

SDFERRY_REGISTER_RECEIVE_COMMAND(basic_types, test_receive_basic_types)
SDFERRY_REGISTER_SEND_COMMAND(basic_types, test_send_basic_types)

SDFERRY_REGISTER_RECEIVE_COMMAND(pointer_types, test_receive_pointer_types)
SDFERRY_REGISTER_SEND_COMMAND(pointer_types, test_send_pointer_types)

SDFERRY_REGISTER_RECEIVE_COMMAND(rollup_type, test_receive_rollup_type)
SDFERRY_REGISTER_SEND_COMMAND(rollup_type, test_send_rollup_type)

SDFERRY_REGISTER_SEND_AND_RECEIVE_COMMAND(basic_types, pointer_types, test_send_and_receive)

SDFERRY_REGISTER_RECEIVE_COMMAND(pointer_types, receive_performance_vs_size)
SDFERRY_REGISTER_SEND_COMMAND(pointer_types, send_performance_vs_size)

SDFERRY_REGISTER_RECEIVE_COMMAND(teec_value_clone, sdferry_receive_performance)
SDFERRY_REGISTER_SEND_COMMAND(teec_value_clone, sdferry_send_performance)

TEE_Result test_receive_basic_types(struct basic_types *bt){
  IMSG("received character '%c' and integer '%u'", bt->c, bt->i);

  return TEE_SUCCESS;
}

TEE_Result test_send_basic_types(struct basic_types *bt){
  bt->c = 'c';
  bt->i = 604;

  IMSG("sent character '%c' and integer '%u'", bt->c, bt->i);

  return TEE_SUCCESS;
}

TEE_Result test_receive_pointer_types(struct pointer_types *pt){
  uint32_t i;

  IMSG("received string '%s' and buffer of length %u", pt->str, pt->buff_len);

  for(i=0; i < pt->buff_len; i++){
    IMSG("character %u is '%c'", i, pt->buff[i]);
  }

  return TEE_SUCCESS;
}

TEE_Result test_send_pointer_types(struct pointer_types *pt){
  uint32_t i;

  pt->buff_len = 4;
  pt->buff = TEE_Malloc(pt->buff_len, TEE_MALLOC_FILL_ZERO);
  if(!pt->buff){
    return TEE_ERROR_OUT_OF_MEMORY;
  }
  pt->buff[0] = 'h';
  pt->buff[1] = 'i';
  pt->buff[2] = 'j';
  pt->buff[3] = 'k';

  pt->str = TEE_Malloc(5, TEE_MALLOC_FILL_ZERO);
  pt->str[0] = 't';
  pt->str[1] = 'e';
  pt->str[2] = 's';
  pt->str[3] = 't';
  pt->str[4] = '\0';

  IMSG("sending string '%s' and buffer of length %u", pt->str, pt->buff_len);

  for(i=0; i < pt->buff_len; i++){
    IMSG("sending character %u: '%c'", i, pt->buff[i]);
  }

  return TEE_SUCCESS;
}

TEE_Result test_receive_rollup_type(struct rollup_type *rt){
  uint32_t i;

  IMSG("rollup received character '%c' and integer '%u'", rt->basic->c, rt->basic->i);

  IMSG("rollup received string '%s' and buffer of length %u", rt->pointer->str, rt->pointer->buff_len);

  for(i=0; i < rt->pointer->buff_len; i++){
    IMSG("character %u is '%c'", i, rt->pointer->buff[i]);
  }

  return TEE_SUCCESS;
}

TEE_Result test_send_rollup_type(struct rollup_type *rt){
  uint32_t i;

  rt->basic = TEE_Malloc(sizeof(struct basic_types), TEE_MALLOC_FILL_ZERO);
  if(!rt->basic){
    return TEE_ERROR_OUT_OF_MEMORY;
  }
  rt->basic->c = 'c';
  rt->basic->i = 604;

  IMSG("sending character '%c' and integer '%u'", rt->basic->c, rt->basic->i);

  rt->pointer = TEE_Malloc(sizeof(struct pointer_types), TEE_MALLOC_FILL_ZERO);
  if(!rt->pointer){
    return TEE_ERROR_OUT_OF_MEMORY;
  }
  rt->pointer->buff_len = 4;
  rt->pointer->buff = TEE_Malloc(rt->pointer->buff_len, TEE_MALLOC_FILL_ZERO);
  if(!rt->pointer->buff){
    return TEE_ERROR_OUT_OF_MEMORY;
  }
  rt->pointer->buff[0] = 'h';
  rt->pointer->buff[1] = 'i';
  rt->pointer->buff[2] = 'j';
  rt->pointer->buff[3] = 'k';

  rt->pointer->str = TEE_Malloc(5, TEE_MALLOC_FILL_ZERO);
  rt->pointer->str[0] = 't';
  rt->pointer->str[1] = 'e';
  rt->pointer->str[2] = 's';
  rt->pointer->str[3] = 't';
  rt->pointer->str[4] = '\0';

  IMSG("sending string '%s' and buffer of length %u", rt->pointer->str, rt->pointer->buff_len);

  for(i=0; i < rt->pointer->buff_len; i++){
    IMSG("sending character %u: '%c'", i, rt->pointer->buff[i]);
  }

  return TEE_SUCCESS;
}

TEE_Result test_send_and_receive(struct basic_types *bt, struct pointer_types *pt){
  uint32_t i;

  IMSG("received character '%c' and integer '%u'", bt->c, bt->i);

  pt->buff_len = 4;
  pt->buff = TEE_Malloc(pt->buff_len, TEE_MALLOC_FILL_ZERO);
  if(!pt->buff){
    return TEE_ERROR_OUT_OF_MEMORY;
  }
  pt->buff[0] = 's';
  pt->buff[1] = 'e';
  pt->buff[2] = 'n';
  pt->buff[3] = 'd';

  pt->str = TEE_Malloc(8, TEE_MALLOC_FILL_ZERO);
  pt->str[0] = 'r';
  pt->str[1] = 'e';
  pt->str[2] = 'c';
  pt->str[3] = 'e';
  pt->str[4] = 'i';
  pt->str[5] = 'v';
  pt->str[6] = 'e';
  pt->str[7] = '\0';

  IMSG("sending string '%s' and buffer of length %u", pt->str, pt->buff_len);

  for(i=0; i < pt->buff_len; i++){
    IMSG("character %u is '%c'", i, pt->buff[i]);
  }

  return TEE_SUCCESS;
}

TEE_Result receive_performance_vs_size(struct pointer_types *pt){
  (void)pt;

  return TEE_SUCCESS;
}

TEE_Result send_performance_vs_size(struct pointer_types *pt){

  pt->buff_len = send_buffer_size_i * 1000;
  pt->buff = TEE_Malloc(send_buffer_size_i * 1000, TEE_MALLOC_FILL_ZERO);
  if(!pt->buff){
    return TEE_ERROR_OUT_OF_MEMORY;
  }

  pt->str = TEE_Malloc(5, TEE_MALLOC_FILL_ZERO);
  pt->str[0] = 't';
  pt->str[1] = 'e';
  pt->str[2] = 's';
  pt->str[3] = 't';
  pt->str[4] = '\0';

  send_buffer_size_j++;
  if(send_buffer_size_j >= 100){
    send_buffer_size_j = 0;
    send_buffer_size_i++;
  }

  return TEE_SUCCESS;
}

TEE_Result native_receive_performance(uint32_t param_types, TEE_Param params[4]){
  uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);

  if (param_types != exp_param_types){
    return TEE_ERROR_BAD_PARAMETERS;
  }

  if(params[0].value.a != 33 || params[0].value.b != 66){
    return TEE_ERROR_NO_DATA;
  }

  return TEE_SUCCESS;
}

TEE_Result sdferry_receive_performance(struct teec_value_clone *tee_value){
  if(!tee_value || tee_value->a != 33 || tee_value->b != 66){
    return TEE_ERROR_NO_DATA;
  }
  
  return TEE_SUCCESS;
}

TEE_Result native_send_performance(uint32_t param_types, TEE_Param params[4]){
  uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);

  if (param_types != exp_param_types){
    return TEE_ERROR_BAD_PARAMETERS;
  }

  params[0].value.a = 33;
  params[0].value.b = 66;

  return TEE_SUCCESS;
}

TEE_Result sdferry_send_performance(struct teec_value_clone *tee_value){
  if(!tee_value){
    return TEE_ERROR_NO_DATA;
  }
  
  tee_value->a = 33;
  tee_value->b = 66;

  return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
  DMSG("has been called");

  return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
  DMSG("has been called");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
  TEE_Param __maybe_unused params[4],
  void __maybe_unused **sess_ctx)
{
  uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
                                             TEE_PARAM_TYPE_NONE,
                                             TEE_PARAM_TYPE_NONE,
                                             TEE_PARAM_TYPE_NONE);

  DMSG("has been called");

  if (param_types != exp_param_types){
    return TEE_ERROR_BAD_PARAMETERS;
  }

  /* Unused parameters */
  (void)&params;
  (void)&sess_ctx;

  /* If return value != TEE_SUCCESS the session will not be created. */
  return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
  (void)&sess_ctx; /* Unused parameter */
}

TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
  (void)&sess_ctx; /* Unused parameter */

  switch (cmd_id) {
    
  case TA_SDFERRY_TEST_CMD_SEND_BASIC_TYPES:
    return SDFERRY_SEND_STRUCT_TO_COMMAND( // send to a receive command
             test_receive_basic_types,     // the function to send to
             param_types,                  // pass on the param types
             params                        // pass on the params
	   );

  case TA_SDFERRY_TEST_CMD_RECEIVE_BASIC_TYPES:
    return SDFERRY_RECEIVE_STRUCT_FROM_COMMAND(test_send_basic_types, param_types, params);

  case TA_SDFERRY_TEST_CMD_SEND_POINTER_TYPES:
    return SDFERRY_SEND_STRUCT_TO_COMMAND(test_receive_pointer_types, param_types, params);

  case TA_SDFERRY_TEST_CMD_RECEIVE_POINTER_TYPES:
    return SDFERRY_RECEIVE_STRUCT_FROM_COMMAND(test_send_pointer_types, param_types, params);

  case TA_SDFERRY_TEST_CMD_SEND_ROLLUP_TYPE:
    return SDFERRY_SEND_STRUCT_TO_COMMAND(test_receive_rollup_type, param_types, params);

  case TA_SDFERRY_TEST_CMD_RECEIVE_ROLLUP_TYPE:
    return SDFERRY_RECEIVE_STRUCT_FROM_COMMAND(test_send_rollup_type, param_types, params);

  case TA_SDFERRY_TEST_CMD_SEND_AND_RECEIVE:
    return SDFERRY_SEND_AND_RECEIVE_STRUCT_FROM_COMMAND(test_send_and_receive, param_types, params);

  case TA_SDFERRY_TEST_CMD_SEND_PERF_VS_SIZE:
    return SDFERRY_SEND_STRUCT_TO_COMMAND(receive_performance_vs_size, param_types, params);

  case TA_SDFERRY_TEST_CMD_RECEIVE_PERF_VS_SIZE:
    return SDFERRY_RECEIVE_STRUCT_FROM_COMMAND(send_performance_vs_size, param_types, params);

  case TA_SDFERRY_TEST_CMD_NATIVE_SEND_TRANSFER:
    return native_receive_performance(param_types, params);

  case TA_SDFERRY_TEST_CMD_SDFERRY_SEND_TRANSFER:
    return SDFERRY_SEND_STRUCT_TO_COMMAND(sdferry_receive_performance, param_types, params);

  case TA_SDFERRY_TEST_CMD_NATIVE_RECEIVE_TRANSFER:
    return native_send_performance(param_types, params);

  case TA_SDFERRY_TEST_CMD_SDFERRY_RECEIVE_TRANSFER:
    return SDFERRY_RECEIVE_STRUCT_FROM_COMMAND(sdferry_send_performance, param_types, params);

  default:
    return TEE_ERROR_BAD_PARAMETERS;
  }
}
