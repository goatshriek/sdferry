#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* To the the UUID (found the the TA's h-file(s)) */
#include <sdferry_test_ta.h>

#include <sdferry_host.h>
#include <3rdparty.h>

// declare the structures that we'll use sdferry for
// this would be done in some common header file
SDFERRY_DECLARE_STRUCT(basic_types)
SDFERRY_DECLARE_STRUCT(pointer_types)
SDFERRY_DECLARE_STRUCT(rollup_type)
SDFERRY_DECLARE_STRUCT(teec_value_clone)

SDFERRY_REGISTER_STRUCT2( // register a struct with 2 members
  basic_types,    // struct name
  i, uint32_t, i, // member #1
  c, char, c      // member #2
)
SDFERRY_REGISTER_STRUCT3( // register a struct with 3 members
  pointer_types,                // struct name
  str, cstring, buff_len,       // member #1
  buff_len, uint32_t, buff_len, // member #2
  buff, buffer, buff_len        // member #3 
)

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

void test_send_basic_types(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin;
  clock_t begin, end;
  struct basic_types bt = {.c = 'a', .i = 33};

  begin = clock();
  res = TEEC_InvokeStructInCommand(sess, TA_SDFERRY_TEST_CMD_SEND_BASIC_TYPES, &err_origin, basic_types, &bt);
  end = clock();
  printf("time to invoke send with basic_types struct: %.5f\n", (double)(end-begin) / CLOCKS_PER_SEC);

  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
  }
}

void test_receive_basic_types(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin;
  clock_t begin, end;
  struct basic_types bt;

  begin = clock();
  res = TEEC_InvokeStructOutCommand(sess, TA_SDFERRY_TEST_CMD_RECEIVE_BASIC_TYPES, &err_origin, basic_types, &bt);
  end = clock();
  printf("time to invoke receive with basic_types struct: %.5f\n", (double)(end-begin) / CLOCKS_PER_SEC);

  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
  }

  printf("received character '%c' and integer '%u'\n", bt.c, bt.i);

  SDFERRY_FREE_STRUCT_MEMBERS(basic_types, &bt);
}

void test_send_pointer_types(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin;
  clock_t begin, end;
  char str[] = {'a', 'b', 'c', '\0'};
  char buffer[] = {'x', 'y', 'z'};
  struct pointer_types pt = {.str = str, .buff = buffer, .buff_len = 3};

  begin = clock();
  res = TEEC_InvokeStructInCommand(sess, TA_SDFERRY_TEST_CMD_SEND_POINTER_TYPES, &err_origin, pointer_types, &pt);
  end = clock();
  printf("time to invoke send with pointer_types struct: %.5f\n", (double)(end-begin) / CLOCKS_PER_SEC);

  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
  }
}

void test_receive_pointer_types(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin, i;
  clock_t begin, end;
  struct pointer_types pt;

  begin = clock();
  res = TEEC_InvokeStructOutCommand(sess, TA_SDFERRY_TEST_CMD_RECEIVE_POINTER_TYPES, &err_origin, pointer_types, &pt);
  end = clock();
  printf("time to invoke receive with pointer_types struct: %.5f\n", (double)(end-begin) / CLOCKS_PER_SEC);

  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
  }

  printf("received string '%s' and buffer of length '%u'\n", pt.str, pt.buff_len);
  for(i=0; i < pt.buff_len; i++){
    printf("character %u is '%c'\n", i, pt.buff[i]);
  }

  SDFERRY_FREE_STRUCT_MEMBERS(pointer_types, &pt);
}

void test_send_rollup_type(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin;
  clock_t begin, end;
  struct basic_types bt = {.c = 'a', .i = 33};
  char str[] = {'a', 'b', 'c', '\0'};
  char buffer[] = {'x', 'y', 'z'};
  struct pointer_types pt = {.str = str, .buff = buffer, .buff_len = 3};
  struct rollup_type rt = {.basic = &bt, .pointer = &pt};

  begin = clock();
  res = TEEC_InvokeStructInCommand(sess, TA_SDFERRY_TEST_CMD_SEND_ROLLUP_TYPE, &err_origin, rollup_type, &rt);
  end = clock();
  printf("time to invoke send with rollup_type struct: %.5f\n", (double)(end-begin) / CLOCKS_PER_SEC);

  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
  }
}

void test_receive_rollup_type(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin, i;
  clock_t begin, end;
  struct rollup_type rt;

  begin = clock();
  res = TEEC_InvokeStructOutCommand(sess, TA_SDFERRY_TEST_CMD_RECEIVE_ROLLUP_TYPE, &err_origin, rollup_type, &rt);
  end = clock();
  printf("time to invoke receive with rollup_type struct: %.5f\n", (double)(end-begin) / CLOCKS_PER_SEC);

  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
  }

  printf("received character '%c' and integer '%u'\n", rt.basic->c, rt.basic->i);

  printf("received string '%s' and buffer of length '%u'\n", rt.pointer->str, rt.pointer->buff_len);
  for(i=0; i < rt.pointer->buff_len; i++){
    printf("character %u is '%c'\n", i, rt.pointer->buff[i]);
  }

  SDFERRY_FREE_STRUCT_MEMBERS(rollup_type, &rt);
}

void test_send_and_receive(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin, i;
  clock_t begin, end;
  struct basic_types bt = {.c = 'f', .i = 12};
  struct pointer_types pt;

  begin = clock();
  res = TEEC_InvokeStructInAndOutCommand(sess, TA_SDFERRY_TEST_CMD_SEND_AND_RECEIVE, &err_origin, basic_types, &bt, pointer_types, &pt);
  end = clock();
  printf("time to invoke send and receive: %.5f\n", (double)(end-begin) / CLOCKS_PER_SEC);

  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
  }

  printf("received string '%s' and buffer of length '%u'\n", pt.str, pt.buff_len);
  for(i=0; i < pt.buff_len; i++){
    printf("character %u is '%c'\n", i, pt.buff[i]);
  }

  SDFERRY_FREE_STRUCT_MEMBERS(pointer_types, &pt);
}

void send_performance_vs_size(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin, i, j, buffer_size;
  clock_t begin, end;
  char str[] = {'a', '\0'};
  struct pointer_types pt;
  FILE *times;

  pt.str = str;
  times = fopen("send_vs_size_results.csv", "w");
  for(i=1; i<= 20; i++){
    buffer_size = i*1000;

    fprintf(times, "%d", buffer_size);

    pt.buff = malloc(buffer_size);
    pt.buff_len = buffer_size;
    memset(pt.buff, 'x', buffer_size);

    for(j=0; j < 100; j++){
      begin = clock();
      res = TEEC_InvokeStructInCommand(sess, TA_SDFERRY_TEST_CMD_SEND_PERF_VS_SIZE, &err_origin, pointer_types, &pt);
      end = clock();

      if (res != TEEC_SUCCESS){
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
      } else {
        fprintf(times, ",%.5f", (double)(end-begin) / CLOCKS_PER_SEC);
      }

    }

    fprintf(times, "\n");
    free(pt.buff);
  }

  fclose(times);
}

void receive_performance_vs_size(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin, i, j, buffer_size;
  clock_t begin, end;
  struct pointer_types pt;
  FILE *times;

  times = fopen("receive_vs_size_results.csv", "w");
  for(i=1; i<= 20; i++){
    buffer_size = i*1000;

    fprintf(times, "%d", buffer_size);

    for(j=0; j < 100; j++){
      begin = clock();
      res = TEEC_InvokeStructOutCommand(sess, TA_SDFERRY_TEST_CMD_RECEIVE_PERF_VS_SIZE, &err_origin, pointer_types, &pt);
      end = clock();

      if (res != TEEC_SUCCESS){
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
      } else {
        fprintf(times, "%d,%.5f", i*1000, (double)(end-begin) / CLOCKS_PER_SEC);
      }
    }

    fprintf(times, "\n");
    SDFERRY_FREE_STRUCT_MEMBERS(pointer_types, &pt);
  }

  fclose(times);
}

void native_send_performance(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin, i;
  clock_t begin, end;
  TEEC_Value tee_value = {.a = 33, .b = 66};
  FILE *times;
  TEEC_Operation op;

  memset(&op, 0, sizeof(op));
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
  op.params[0].value = tee_value;

  times = fopen("native_send_results.csv", "w");
  for(i=1; i<= 1000; i++){
    begin = clock();
    res = TEEC_InvokeCommand(sess, TA_SDFERRY_TEST_CMD_NATIVE_SEND_TRANSFER, &op, &err_origin);
    end = clock();

    if (res != TEEC_SUCCESS){
      errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
    } else {
      fprintf(times, "%d,%.5f\n", i, (double)(end-begin) / CLOCKS_PER_SEC);
    }
  }

  fclose(times);
}

void sdferry_send_performance(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin, i;
  clock_t begin, end;
  struct teec_value_clone tee_value = {.a = 33, .b = 66};
  FILE *times;

  times = fopen("sdferry_send_results.csv", "w");
  for(i=1; i<= 1000; i++){
    begin = clock();
    res = TEEC_InvokeStructInCommand(sess, TA_SDFERRY_TEST_CMD_SDFERRY_SEND_TRANSFER, &err_origin, teec_value_clone, &tee_value);
    end = clock();

    if (res != TEEC_SUCCESS){
      errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
    } else {
      fprintf(times, "%d,%.5f\n", i, (double)(end-begin) / CLOCKS_PER_SEC);
    }
  }

  fclose(times);
}

void native_receive_performance(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin, i;
  clock_t begin, end;
  FILE *times;
  TEEC_Operation op;

  memset(&op, 0, sizeof(op));
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);

  times = fopen("native_receive_results.csv", "w");
  for(i=1; i<= 1000; i++){
    begin = clock();
    res = TEEC_InvokeCommand(sess, TA_SDFERRY_TEST_CMD_NATIVE_RECEIVE_TRANSFER, &op, &err_origin);
    end = clock();

    if (res != TEEC_SUCCESS){
      errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
    } else if(op.params[0].value.a != 33 || op.params[0].value.b != 66) {
      errx(1, "the values were not properly received from the trusted application in native send");
    } else {
      fprintf(times, "%d,%.5f\n", i, (double)(end-begin) / CLOCKS_PER_SEC);
    }
  }

  fclose(times);
}

void sdferry_receive_performance(TEEC_Session *sess){
  TEEC_Result res;
  uint32_t err_origin, i;
  clock_t begin, end;
  struct teec_value_clone tee_value;
  FILE *times;

  times = fopen("sdferry_receive_results.csv", "w");
  for(i=1; i<= 1000; i++){
    begin = clock();
    res = TEEC_InvokeStructOutCommand(sess, TA_SDFERRY_TEST_CMD_SDFERRY_RECEIVE_TRANSFER, &err_origin, teec_value_clone, &tee_value);
    end = clock();

    if (res != TEEC_SUCCESS){
      errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
    } else if(tee_value.a != 33 || tee_value.b != 66) {
      errx(1, "the values were not properly received from the trusted application in sdferry send");
    } else {
      fprintf(times, "%d,%.5f\n", i, (double)(end-begin) / CLOCKS_PER_SEC);
    }
  }

  fclose(times);
}

int main(int argc, char *argv[])
{
  TEEC_Context ctx;
  TEEC_Session sess;
  TEEC_Result res;
  uint32_t err_origin;
  TEEC_UUID uuid = TA_SDFERRY_TEST_UUID;
  clock_t begin, end;

  // Initialize a context connecting us to the TEE
  res = TEEC_InitializeContext(NULL, &ctx);
  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
  }

  res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",res, err_origin);
  }

  // setting up the sdferry environment
  begin = clock();
  res = TEEC_InitializeSDFerry(&ctx);
  end = clock();
  printf("time to initialize sdferry: %.5f\n", (double)(end-begin) / CLOCKS_PER_SEC);
  if (res != TEEC_SUCCESS){
    errx(1, "TEEC_InitializeSendStruct failed with code 0x%x",res);
  }

  // running tests
  test_send_basic_types(&sess);
  test_receive_basic_types(&sess);
  test_send_pointer_types(&sess);
  test_receive_pointer_types(&sess);
  test_send_and_receive(&sess);
  test_send_rollup_type(&sess);
  test_receive_rollup_type(&sess);

  //gathering performance data
  send_performance_vs_size(&sess);
  receive_performance_vs_size(&sess);
  native_send_performance(&sess);
  sdferry_send_performance(&sess);
  native_receive_performance(&sess);
  sdferry_receive_performance(&sess);

  // tearing down the sdferry environment
  begin = clock();
  TEEC_FinalizeSDFerry();
  end = clock();
  printf("time to finalize sdferry: %.5f\n", (double)(end-begin) / CLOCKS_PER_SEC);

  // cleaning up session
  TEEC_CloseSession(&sess);
  TEEC_FinalizeContext(&ctx);

  return 0;
}
