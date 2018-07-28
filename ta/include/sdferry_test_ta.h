#ifndef TA_SDFERRY_TEST_H
#define TA_SDFERRY_TEST_H


/*
 * This UUID is generated with uuidgen
 * the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html
 */
#define TA_SDFERRY_TEST_UUID \
	{ 0xab7cd5a9, 0xee86, 0x4e28, \
		{ 0xaf, 0x73, 0x50, 0xdc, 0x3f, 0xfd, 0xb0, 0xfb} }

/* The function IDs implemented in this TA */
#define TA_SDFERRY_TEST_CMD_SEND_BASIC_TYPES          0
#define TA_SDFERRY_TEST_CMD_RECEIVE_BASIC_TYPES       1
#define TA_SDFERRY_TEST_CMD_SEND_POINTER_TYPES        2
#define TA_SDFERRY_TEST_CMD_RECEIVE_POINTER_TYPES     3
#define TA_SDFERRY_TEST_CMD_SEND_ROLLUP_TYPE          4
#define TA_SDFERRY_TEST_CMD_RECEIVE_ROLLUP_TYPE       5
#define TA_SDFERRY_TEST_CMD_SEND_AND_RECEIVE          6
#define TA_SDFERRY_TEST_CMD_SEND_PERF_VS_SIZE         7
#define TA_SDFERRY_TEST_CMD_RECEIVE_PERF_VS_SIZE      8
#define TA_SDFERRY_TEST_CMD_NATIVE_SEND_TRANSFER      9
#define TA_SDFERRY_TEST_CMD_SDFERRY_SEND_TRANSFER    10
#define TA_SDFERRY_TEST_CMD_NATIVE_RECEIVE_TRANSFER  11
#define TA_SDFERRY_TEST_CMD_SDFERRY_RECEIVE_TRANSFER 12

#endif /*TA_SDFERRY_TEST_H*/