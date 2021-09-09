#pragma once
/*****************************************************************************
Author(s) or Contributor(s): Version 0.1  ~<NAME> 2021                                                               
  
File: CS_Common.h                                                          

Description: Common header file for Client-Server code                     

*****************************************************************************/
// Header Include Guards

#pragma	  comment(lib, "ws2_32.lib")

#ifndef	  CS_COMMON_H
#define	  CS_COMMON_H

#define	  LIN

/* Includes: ****************************************************************/

#ifndef	  LIN
#include  <winsock.h>
#include  <Windows.h>
#else
#include  <sys/socket.h>
#include  <arpa/inet.h>
#include  <unistd.h>
#endif
#include  <stdio.h>
#include  <stdlib.h>
#include  <stdint.h>
#include  <string.h>
#include  <strings.h>
#include  <stdbool.h>
#include  <stddef.h>
#include  <sys/time.h>
#include  <sys/types.h>
#include  <sys/ioctl.h>

/****************************************************************************/

/* Defines: *****************************************************************/

#define   TIME_O 5
#define   MAX_CONN 6
#define	  MAX_LEN 260//257
#define	  MAXLINE 1000
#define   SLEEP	sleep(1);
#define   REM_CL_PORT 11069
#define   REM_SRV_PORT_0 11000
#define   REM_SRV_PORT_1 11001
#define   REM_SRV_PORT_2 11002
#define   REM_SRV_PORT_3 11003
#define   REM_SRV_PORT_4 11004
#define	  REM_SRV_IP_0 "192.168.143.201"
#define	  REM_SRV_IP_1 "192.168.143.202"
#define	  REM_SRV_IP_2 "192.168.143.203"
#define	  REM_SRV_IP_3 "192.168.143.204"
#define	  REM_SRV_IP_4 "192.168.143.205"
#define   LOCAL_IP "127.0.0.1"
#define	  TIME_V struct timeval
#define	  S_SADDR struct sockaddr
#define	  S_SADDR_IN struct	sockaddr_in

/****************************************************************************/

/* Data Sructure Definitions: ***********************************************/

/* Data Buffer **************************************************************

struct  DataBuffer
{
  char  cPayload[MAX_STR_SZ]; // Storage for string	
  // uint32_t	uDataSize;	    // Size of data
  // uint16_t	*pNext;	        // Next node *
  // uint16_t  *pPrev;	    // Previous node *

} __attribute__((__packed__));

/****************************************************************************/

/* Typedefs: ****************************************************************

// typedef  DataBuffer_  *pDbuff;
typedef	struct DataBuffer DBffr;

/****************************************************************************/

/* Function Prototypes: *****************************************************/
// Helper Functions
char	    *convertHex(const char *src, size_t len);
bool      bCheckSum(const uint8_t *buff1, const uint8_t *buff2, size_t sZ);
// void	    SrvConnection_Hndlr(uint32_t uSrvSok, uint16_t nConnections);
// uint16_t  SokInit_Handlr(void);
// uint32_t  BindSrvSok_Hndlr(uint32_t uSrvSok);
// uint32_t  SokConnect_Hndlr(uint32_t uClSok, char* remIP, uint16_t remPort);
// uint32_t  SokSend_Hndlr(uint32_t uClSok, char *pRqst, uint16_t pRqstLen);
// uint32_t  SokRcv_Hndlr(uint32_t uClSok, char *pRsp, uint16_t rcvSize);

/****************************************************************************/
#endif // CS_COMMON_H
