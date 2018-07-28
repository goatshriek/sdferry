#ifndef __SDFERRY_CALL_HOST_H
#define __SDFERRY_CALL_HOST_H

#include <sdferry/register.h>

// this is an arbitrary value - it needs to be revisited
#define SDFERRY_SHARED_MEMORY_SIZE 21000

/*
 * The TEEC_InitializeSDFerry function must be called before the TEEC_Invoke
 * functions provided by sdferry can be used.
 *
 * This function allocates the shared memory that is used to pass the data
 * between the two worlds in a serial form. It's return code is therefore
 * identical to that of a call to TEEC_AllocatedSharedMemory.
 *
 * If the result is not TEEC_SUCCESS, then the initialization has failed and
 * the function will need to be called again before use.
 */
TEEC_Result TEEC_InitializeSDFerry(TEEC_Context *context);

/*
 * The TEEC_FinalizeSDFerry function cleans up the memory and other values used
 * by the sdferry library during operation. It should be called before
 * finalizing the context of the application itself.
 */
void TEEC_FinalizeSDFerry();

/* Note that the following InvokeStruct functions only work for structs that
 * have been registered with SDFerry - primitive types will NOT work (their
 * function signature is different) and therefore they must be passed via the
 * methods provided by OP-TEE itself.
 */

/*
 * The TEEC_InvokeStructInCommand function is similar to the TEEC_InvokeCommand
 * function provided by the OPTEE Client API. The primary difference is that
 * there are two additional parameters which describe, respectively, the name of
 * the struct type that will be sent (as named in the REGISTER function), and a
 * pointer to the struct that needs to be sent. All other behavior is identical
 * to the TEEC_InvokeCommand, with the ommision of the operation parameter as
 * this is created automatically by sdferry.
 */
TEEC_Result TEEC_SendStructToCommand(TEEC_Session *session, uint32_t commandID, uint32_t *returnOrigin, uint32_t (*send_func)(char *, void *, uint32_t), void *val);
#define TEEC_InvokeStructInCommand(SESSION, COMMANDID, RETURNORIGIN, TYPE, STRUCT) \
TEEC_SendStructToCommand(SESSION, COMMANDID, RETURNORIGIN, __sdferry_send_##TYPE, STRUCT)

/*
 * The TEEC_InvokeStructOutCommand function is similar to the TEEC_InvokeCommand
 * function provided by the OPTEE Client API. The primary difference is that
 * there are two additional parameters which describe, respectively, the name of
 * the struct type that will be received (as named in the REGISTER function),
 * and a pointer to the struct that will be populated with the received struct.
 * This must be a pointer to allocated memory, otherwise a segmentation fault
 * will occur. All other behavior is identical to the TEEC_InvokeCommand, with
 * the ommision of the operation parameter as this is created automatically by
 * sdferry. Note that the struct that is returned must have
 * SDFERRY_FREE_STRUCT_MEMBERS called on it in order to free the memory
 * allocated by this function.
 */
TEEC_Result TEEC_ReceiveStructFromCommand(TEEC_Session *session, uint32_t commandID, uint32_t *returnOrigin, uint32_t (*receive_func)(char *, void *), void *val);
#define TEEC_InvokeStructOutCommand(SESSION, COMMANDID, RETURNORIGIN, TYPE, STRUCT) \
TEEC_ReceiveStructFromCommand(SESSION, COMMANDID, RETURNORIGIN, __sdferry_receive_##TYPE##_struct, STRUCT)

/*
 * The TEEC_InvokeStructInAndOutCommand function acts as a combination of the
 * TEEC_InvokeStructInCommand and TEEC_InvokeStructOutCommand functions. The
 * function has parameters for both the struct to be passed in and the struct
 * that will be received. The out struct must have its memory allocations
 * released in the same way as the TEEC_InvokeStructOutCommand function.
 */
TEEC_Result TEEC_SendAndReceiveStructFromCommand(TEEC_Session *session, uint32_t commandID, uint32_t *returnOrigin, uint32_t (*send_func)(char *, void *, uint32_t), void *in_val, uint32_t (*recieve_func)(char *, void *), void *out_val);
#define TEEC_InvokeStructInAndOutCommand(SESSION, COMMANDID, RETURNORIGIN, IN_TYPE, IN_STRUCT, OUT_TYPE, OUT_STRUCT) \
TEEC_SendAndReceiveStructFromCommand(SESSION, COMMANDID, RETURNORIGIN, __sdferry_send_##IN_TYPE, IN_STRUCT, __sdferry_receive_##OUT_TYPE##_struct, OUT_STRUCT)

#endif /* __SDFERRY_CALL_HOST_H */
