#pragma once
/*****************************************************************************
Author(s) or Contributor(s): Version 0.0 (IN TEST)  ~Vincent A. Laguna 2023

File: CS_Common.h

Description: Common header file for REMOTE Server code

*****************************************************************************/
// Header Include Guards

#pragma comment(lib, "ws2_32.lib")

#ifndef CS_COMMON_H
#define CS_COMMON_H

#define LIN

/* Includes: ****************************************************************/

#ifndef LIN

#include <winsock.h>
#include <Windows.h>

#else

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>

/****************************************************************************/

/* Defines: *****************************************************************/

#define DBG
#define TIME_O 5
#define MAX_CONN 6
// #define		USE_TCP
// #define		THREAD_TEST
// #define		M_THREADED_SOKETS
#define LOCAL_TEST
#define SERVER_CONNECT
#define MAX_LEN 254
#define MIN_LEN 16
#define MAX_STR_SZ 128
#define MIN_STR_SZ 30
#define BUFF_SZ 128
#define IP_STR_SZ 16
#define SLEEP sleep(1);
#define NUM_SRV_THREADS 2
#define NUM_CL_THREADS 2
#define REM_CL_PORT 11069

#ifdef LOCAL_TEST
  #define REM_SRV_PORT_0 11000
  #define REM_SRV_PORT_1 11001
  #define REM_SRV_IP_0 "127.0.0.1" // For quick local test
  #define REM_SRV_IP_1 "127.0.0.1" // Local Tests
#else
  #define REM_SRV_PORT_1 11001
  #define REM_SRV_PORT_0 2077 // Remote Protocol Test Port
  #define REM_SRV_IP_1 "127.0.0.1" // Local Tests
  #define REM_SRV_IP_0 "192.168.128.131" // Remote Device IP
#endif

// #define REM_SRV_PORT_1 11001
#define REM_SRV_PORT_2 11002
#define REM_SRV_PORT_3 11003
#define REM_SRV_PORT_4 11004
#define REM_SRV_PORT_5 11005
// #define REM_SRV_IP_0 "192.168.128.229" // Remote Device IP
// #define REM_SRV_IP_0 "127.0.0.1" // For quick local test
// #define REM_SRV_IP_0 "192.168.143.20"
// #define REM_SRV_IP_0 "192.168.128.250"
// #define REM_SRV_IP_1 "127.0.0.1" // Local Tests
#define REM_SRV_IP_2 "192.168.128.252"
#define REM_SRV_IP_3 "192.168.128.253"
#define REM_SRV_IP_4 "192.168.128.254"
#define REM_SRV_IP_5 "192.168.128.255"
#define LOCAL_IP "127.0.0.1"
#define TIME_V struct timeval
#define S_SADDR struct sockaddr
#define S_SADDR_IN struct sockaddr_in

#define SRC_ANY   0xFF
#define DST_SCU   0x20
#define PWR_CTRL  0x40
#define SYNC_1    0x0A
#define SYNC_2    0x0B
#define LEN       0x04

#define DATA_0    "DATA 0"
#define DATA_1    "DATA 1"

#define MSB       0xFF
#define LSB       0xFF00

#define CS        0x00


#define SENTINEL_INSERTION_CODE(p_person_t, p_buffer)                           \
{                                                                               \
  if (!p_person_t)                                                              \
  {                                                                             \
    unsigned int sentinel_val = 0xFFFFFFFF;                                     \
    test_insert_data(p_buffer, (uint8_t *)&sentinel_val, sizeof(unsigned int)); \
    if (sentinel_val == 0xFFFFFFFF) return NULL;                                \
    test_buffer_skip(p_buffer, -1 * sizeof(unsigned int));                      \
    return;                                                                     \
  }                                                                             \
}

#define SENTINEL_DETECTION_CODE(p_buffer)                                       \
{                                                                               \
    unsigned int sentinel_val = 0;                                              \
    test_parse_data((uint8_t *)&sentinel_val, p_buffer, sizeof(unsigned int));  \
    if(sentinel_val == 0xFFFFFFFF)  return NULL;                                \
    test_buffer_skip(p_buffer, -1 * sizeof(unsigned int));                      \
}

/****************************************************************************/

/* Globals: *****************************************************************/

static uint8_t *szRem_Srv_IP[] =
{
  REM_SRV_IP_0,
  REM_SRV_IP_1,
  REM_SRV_IP_2,
  REM_SRV_IP_4,
  REM_SRV_IP_5
};

enum
{
  eREM_SRV_IP_0,
  eREM_SRV_IP_1,
  eREM_SRV_IP_2,
  eREM_SRV_IP_3,
  eREM_SRV_IP_4,
  eREM_SRV_IP_5
};

static uint16_t uRem_Srv_Port[] =
{
  REM_SRV_PORT_0,
  REM_SRV_PORT_1,
  REM_SRV_PORT_2,
  REM_SRV_PORT_3,
  REM_SRV_PORT_4,
  REM_SRV_PORT_5
};

enum
{
  eREM_SRV_PORT_0,
  eREM_SRV_PORT_1,
  eREM_SRV_PORT_2,
  eREM_SRV_PORT_3,
  eREM_SRV_PORT_4,
  eREM_SRV_PORT_5
};

static uint8_t *szData[] =
{
  DATA_0,
  DATA_1
};

enum
{
  eData_0,
  eData_1,
};

static const uint8_t *cSerialData =
{
  "\xff"                                               // NUM bytes
  "\x02"                                               // STX
  "01234567890123456789012345678901234567890123456789" // 50 chrs
  "01234567890123456789012345678901234567890123456789" // 50 chrs
  "01234567890123456789012345678901234567890123456789" // 50 chrs
  "01234567890123456789012345678901234567890123456789" // 50 chrs
  "01234567890123456789012345678901234567890123456789" // 50 chrs
  "\x03",                                              // ETX
                                                       // 255 total
};

static const uint8_t *cStringPayload =
{
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" // 26
  "abcdefghijklmnopqrstuvwxyz" // 52
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" // 78
  "abcdefghijklmnopqrstuvwxyz" // 104
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" // 130
  "abcdefghijklmnopqrstuvwxyz" // 156
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" // 182
  "abcdefghijklmnopqrstuvwxyz" // 208
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" // 234
  "abcdefghijklmnopqrstuvwxyz" // 260
  "",
};

static const uint8_t *cREMDataRFOn =
{
  // "\xff" 
  // "\x02"  
  "b6 c5 65 d9 71 71 9c eb e8 f4 ed a6 08 00 45 00 00"
  "55 bf 52 00 00 80 11 00 00 c0 a8 42 45 c0 a8 42 34"
  "fe ef 08 1d 00 41 06 1d 0a 0b 20 11 52 32 ff ff 00"
  "00 0a 00 00 00 ff 60 00 00 00 00 ff ff ff ff 00 00"
  "00 00 00 00 00 00 10 00 00 00 ff ff ff ff 00 00 00"
  "00 00 00 00 00 00 00 00 00 f0 ff ff ff 26"
  // "\x03",
  "",
};

static const uint8_t *cREMDataSYSReset =
{ 
  "\x0A"
  "\x0B"
  "\x20"
  "\x11"
  "\x38"
  "\x04"
  "\x01"
  "\x38"
  "\x01"
  "\x01" // FIX ME -> if "\x00": it skips the CS and keeps sending (n) "00's"...
  "\xBD",
  ""
};

// static const uint8_t *cREMDataRFOn =
// static const uint8_t *cREMDataSYSReset =
// {
//   "\x0A"
//   "\x0B"
//   "\x20"
//   "\x11"
//   "\x52"
//   "\x32"
//   "\xFF"
//   "\xFF"
//   "\x00"
//   "\x00"
//   "\x0A"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\xFF"
//   "\x00"
//   "\x60"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\xFF"
//   "\xFF"
//   "\xFF"
//   "\xFF"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\xFF"
//   "\xFF"
//   "\xFF"
//   "\xFF"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\x00"
//   "\xF0"
//   "\xFF"
//   "\xFF"
//   "\xFF"
//   "\x15"
//   "",
// };

/****************************************************************************/

/* Data Sructure Definitions: ***********************************************/

/* Data Buffer **************************************************************/

// typedef struct IPData
// {
//   // IP Data Struct
//   S_SADDR_IN srvAddr;
//   S_SADDR_IN clAddr;
//   uint8_t IPDataID;

// } IPData;

typedef struct SOKData
{
  // Data to pass as argument to thread handler
  uint8_t  *cIP;   //[IP_STR_SZ];	// IP (c-String)
  uint16_t uPort; // Port Number
  uint16_t SOKid; // SOK ID
  uint8_t  sink1;
  uint8_t  sink2;
  uint8_t  src;
  uint8_t  dst;
  uint8_t  cmd;
  uint8_t  len;
  uint8_t  *data;
  uint8_t  msb;
  uint8_t  lsb;
  uint8_t  cs;
  // struct IPData *ipData;
} SOKData;

typedef struct Data
{
  // Data field structure (variable length fields <field N> -> implement in Server.c)
  uint8_t header;
  uint8_t length;
  uint8_t fMask;
  uint8_t f1;
  uint8_t f2;
  uint8_t fN;
  uint8_t LMSB;
  // uint8_t CS;
} Data;

typedef struct REMDataM // REM Protocol Packet Structure
{
  // Byte-oriented (multi-byte fields are little-endian)
  uint8_t sink1;
  uint8_t sink2;
  uint8_t src;
  uint8_t dst;
  uint8_t cmd;
  uint8_t len;
  uint8_t LMSB;
  // uint8_t				      LLSB;
  uint8_t *Data;
  uint8_t cs; // <sink1-last data byte % 256>
              // 	struct PayloadData  *PyldData;
} REMDataM;

typedef struct REMDataL
{
  // REM Protocol Packet Structure
  uint8_t sink1;
  uint8_t sink2;
  uint8_t src;
  uint8_t dst;
  uint8_t cmd;
  uint8_t len;
  // uint8_t   			    LMSB;
  uint8_t LLSB;
  uint8_t *Data;
  uint8_t cs; // <sink1-last data byte % 256>
  struct PayloadData  *PyldData;

} REMDataL;

// Forward struct declaration
typedef struct test_data test_data;
typedef struct test_list_node test_list_node; 
typedef struct _person_t person_t;
// typedef struct _company_t company_t;

typedef struct nest_data
{
  // Data fields
  uint8_t   u8_sz_nst_data_0[MIN_STR_SZ];
  uint8_t   u8_nst_data_ID;
  test_data *p_test_data;
  test_data *p_test_data_arr[MIN_LEN];

} nest_data;

struct test_data
{
  // Data fields
  uint8_t   u8_sz_data_0[MIN_STR_SZ];
  uint8_t   u8_data_1;
  nest_data nst_data_0;
  nest_data *p_nst_data_0;
  uint16_t  u16_data_2;
  
};

typedef struct test_buffer
{
  // Data fields
  uint8_t *tst_bffr_data;
  uint8_t size;
  uint8_t next;
  
} test_buffer;

typedef struct test_ll
{
  
  test_list_node *head;

} test_ll;


typedef struct test_list_node
{
  // Data fields
  test_data       *pNode_tst_data; // Pointer member to "P-Object"
  test_list_node  *pRight;

};

// Serializing example structures 12312022
typedef struct _company_t
{
  // Data fields
  uint8_t   comp_name[32];
  int       emp_strength;
  person_t  *CEO;
} company_t;

struct _person_t
{
  // Data fields
  unsigned int      vehicle_no[4];
  int               age;
  int               *height;
  unsigned int      *last_sal_amounts[5];
  uint8_t           name[32];
  company_t         company;
  company_t         dream_companies[3];
  struct _person_t  *CEO;
  struct _person_t  *admin_staff[5];
};


// struct  DataBuffer
// {
//   char  cPayload[MAX_STR_SZ]; // Storage for string
//   // uint32_t	uDataSize;	   // Size of data
//   // uint16_t	*pNext;	       // Next node *
//   // uint16_t  *pPrev;	       // Previous node *
// } __attribute__((__packed__));
/****************************************************************************/

/* Typedefs: ****************************************************************
// typedef  DataBuffer_  *pDbuff;
typedef	struct DataBuffer DBffr;
/****************************************************************************/

/* Function Prototypes: *****************************************************/
// Helper Functions
int             rID_Gen(void);
void            *UDP_SrvConnection_Hndlr(void *SOKData);
void            init_test_buffer(test_buffer **d_pbuffer);
void            test_serialize_data(test_data *p_data, test_buffer *p_buffer);
void            test_serialize_nest_data(nest_data *p_nst_data, test_buffer *p_buffer);
void            test_serialize_llist(test_ll *p_test_ll, test_buffer *p_buffer);
void            test_serialize_ll_node(test_list_node *p_ll_node, test_buffer *p_buffer);
test_data       *test_de_serialize_data(test_buffer *p_buffer);
nest_data       *test_de_serialize_nest_data(test_buffer *p_buffer);
test_ll         *test_de_serialize_llist(test_buffer *p_buffer);
test_list_node  *test_de_serialize_ll_node(test_buffer *p_buffer);
void            test_insert_data(test_buffer *p_buffer, uint8_t *p_data, uint8_t n_bytes);
void            test_insert_nest_data(nest_data *p_data, test_buffer *p_buffer);
void            test_read_data(test_buffer *p_buffer, uint8_t *p_data, uint8_t n_bytes);
void            test_parse_data(uint8_t *p_dst, test_buffer *p_buffer, uint8_t size);
void            test_skip_data(test_buffer *p_buffer, uint8_t skip_sz);
void            test_buffer_skip(test_buffer *p_buffer, uint8_t skip_sz);
void            test_reset_buffer(test_buffer *p_buffer);
void            test_free_buffer(test_buffer *p_buffer);
void            serialize_person_t(person_t *p_person_t, test_buffer *p_buffer);
void            serialize_company_t(company_t *p_company_t, test_buffer *p_buffer);
person_t        *de_serialize_person_t(test_buffer *p_buffer);
company_t       *de_serialize_company_t(test_buffer *p_buffer);
void            REMDataSnd(uint8_t *pSink1, uint8_t *pSink2, uint8_t *pSrc, uint8_t *pDst, uint8_t *pCmd, uint8_t *pLen, uint8_t *pData, uint8_t *pMsb, uint8_t *pLsb, uint8_t *pCs);
bool            bCheckSum(const uint8_t *buff1, const uint8_t *buff2, size_t sZ);
uint8_t         *convertHex(uint8_t *src, size_t len);
int16_t         UDP_SokInit_Handlr(void);

/****************************************************************************/
#endif // CS_COMMON_H
