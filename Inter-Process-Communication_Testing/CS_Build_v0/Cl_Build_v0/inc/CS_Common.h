/*****************************************************************************
* Author(s):                                                                 *
*                                                                            *
* File:		     CS_Common.h                                                   *
*                                                                            *
* Description: Common header file for Client-Server code                     *
*                                                                            *
*****************************************************************************/
// Header Include Guards

#ifndef		CS_COMMON_H
#define 	CS_COMMON_H

#define		LIN

/* Includes: ****************************************************************/

#ifndef		LIN
#include	<winsock.h>
#else
#include  <sys/socket.h>
#include  <arpa/inet.h>
#include  <unistd.h>
#endif
#include  <stdio.h>
#include  <stdlib.h>
#include	<stdint.h>
#include  <string.h>

/****************************************************************************/

/* Defines: *****************************************************************/

#define  LOCAL_TEST
#define	 TEST_PORT     					12345
#define  TIME_V	   struct				timeval
#define	 S_SADDR	   struct				sockaddr
#define  S_SADDR_IN struct    		sockaddr_in
#define  LOCAL_IP						    "127.0.0.1"
#define  MAX_STR_SZ						  256

/****************************************************************************/

/* Data Sructure Definitions: ***********************************************/

/* Data Buffer **************************************************************/

typedef	struct        					DataBuffer
{
	char								    			cPayload[MAX_STR_SZ];	// Storage for string	
	// uint32_t					  		    uBufferType;	        // Type of Buffer	
	uint32_t						  		  	uDataSize;	          // Size of data
	uint16_t					  		    	*pNext;			          // Next node *
	uint16_t					  		    	*pPrev;			          // Previous node *
	
} DataBuffer_t;

/****************************************************************************/

/* Typedefs: ****************************************************************/

// typedef		DataBuffer_t				*pDbuff;

/****************************************************************************/

/* Function Prototypes: *****************************************************/
// Helper Functions

uint16_t	SokInit_Handlr(void);
uint32_t	BindSrvSok_Hndlr(uint32_t uSrvSok);
uint32_t	SokConnect_Hndlr(uint32_t uClSok);
uint32_t	SokSend_Hndlr(uint32_t uClSok, char *pRqst, uint16_t pRqstLen);
uint32_t	SokRcv_Hndlr(uint32_t uClSok, char *pRsp, uint16_t rcvSize);

/****************************************************************************/
#endif // CS_COMMON_H
