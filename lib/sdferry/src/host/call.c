#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tee_client_api.h>
#include <sdferry/host/call.h>

static short initialized=0;
static TEEC_SharedMemory shared_mem_in;
static TEEC_SharedMemory shared_mem_out;

TEEC_Result TEEC_InitializeSDFerry(TEEC_Context *context){
  TEEC_Result result;

  if(initialized){
    return TEEC_SUCCESS;
  }

  shared_mem_in.size = SDFERRY_SHARED_MEMORY_SIZE;
  shared_mem_in.flags = TEEC_MEM_INPUT;
  result = TEEC_AllocateSharedMemory(context, &shared_mem_in);
  if(result != TEEC_SUCCESS){
    goto cleanup_in;
  }

  shared_mem_out.size = SDFERRY_SHARED_MEMORY_SIZE;
  shared_mem_out.flags = TEEC_MEM_OUTPUT;
  result = TEEC_AllocateSharedMemory(context, &shared_mem_out);
  if( result != TEEC_SUCCESS){
    goto cleanup_out;
  }

  initialized = 1;
  return TEEC_SUCCESS;

cleanup_out:
  TEEC_ReleaseSharedMemory(&shared_mem_in);

cleanup_in:
  return result;
}

void TEEC_FinalizeSDFerry(){
  if(initialized){
    TEEC_ReleaseSharedMemory(&shared_mem_in);
    TEEC_ReleaseSharedMemory(&shared_mem_out);
    initialized = 0;
  }
}

TEEC_Result TEEC_SendStructToCommand(TEEC_Session *session, uint32_t commandID, uint32_t *returnOrigin, uint32_t (*send_func)(char *, void *, uint32_t), void *val){
  TEEC_Operation op;
  TEEC_RegisteredMemoryReference mem_ref;

  mem_ref.size = send_func(shared_mem_in.buffer, val, 0);
  mem_ref.parent = &shared_mem_in;
  mem_ref.offset = 0;

  memset(&op, 0, sizeof(op));
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
  op.params[0].memref = mem_ref;
  
  return TEEC_InvokeCommand(session, commandID, &op, returnOrigin);
}

TEEC_Result TEEC_ReceiveStructFromCommand(TEEC_Session *session, uint32_t commandID, uint32_t *returnOrigin, uint32_t (*recieve_func)(char *, void *), void *val){
  TEEC_Operation op;
  TEEC_RegisteredMemoryReference mem_ref;
  TEEC_Result result;

  mem_ref.size = SDFERRY_SHARED_MEMORY_SIZE;
  mem_ref.parent = &shared_mem_out;
  mem_ref.offset = 0;

  memset(&op, 0, sizeof(op));
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_OUTPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
  op.params[0].memref = mem_ref;
  result = TEEC_InvokeCommand(session, commandID, &op, returnOrigin);
  recieve_func(shared_mem_out.buffer, val);

  return result;
}

TEEC_Result TEEC_SendAndReceiveStructFromCommand(TEEC_Session *session, uint32_t commandID, uint32_t *returnOrigin, uint32_t (*send_func)(char *, void *, uint32_t), void *in_val, uint32_t (*recieve_func)(char *, void *), void *out_val){
  TEEC_Operation op;
  TEEC_RegisteredMemoryReference mem_ref_in, mem_ref_out;
  TEEC_Result result;

  mem_ref_in.size = send_func(shared_mem_in.buffer, in_val, 0);
  mem_ref_in.parent = &shared_mem_in;
  mem_ref_in.offset = 0;

  mem_ref_out.size = SDFERRY_SHARED_MEMORY_SIZE;
  mem_ref_out.parent = &shared_mem_out;
  mem_ref_out.offset = 0;

  memset(&op, 0, sizeof(op));
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT, TEEC_MEMREF_PARTIAL_OUTPUT, TEEC_NONE, TEEC_NONE);
  op.params[0].memref = mem_ref_in;
  op.params[1].memref = mem_ref_out;
  result = TEEC_InvokeCommand(session, commandID, &op, returnOrigin);
  recieve_func(shared_mem_out.buffer, out_val);

  return result;
}
