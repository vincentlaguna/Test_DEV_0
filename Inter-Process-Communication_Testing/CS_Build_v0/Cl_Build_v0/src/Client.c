/*****************************************************************************
Author(s) or Contributor(s): Version .0  ~<NAME> 2021 

File: Client.c

Description: Client-side code                                              

*****************************************************************************/


/* Includes: ****************************************************************/

#include "../inc/CS_Common.h"

/****************************************************************************/

/* Defines: *****************************************************************/
/****************************************************************************/

/* Typedefs: ****************************************************************/
/****************************************************************************/

/* Function Definitions: ****************************************************/
// Helper Functions

/*****************************************************************************

Name: SOKInit_Handlr()                                               
Purpose:  Handles the creation of a Socket                               
Parameters:                                                            
Returns:  

*****************************************************************************/

int16_t	SOKInit_Handlr(void)
{
  // Local Variables
  int16_t  hSOK;
  // Output
  printf("\n[-]CLIENT-Side Socket Initialization = in progress...\n\n");
  SLEEP
  // Socket System Call
  hSOK = socket(AF_INET, SOCK_STREAM, 0);
  // Output Validation
  printf("\n[+]CLIENT-Side Socket Initialization = OK\n\n");
  SLEEP
  // Function Return
  return  hSOK;
}

// End SOKInit_Handlr()
/****************************************************************************/


/*****************************************************************************

Name: SOKConnect_Handlr()                                            
Purpose: Handles the Connection of a Socket to the Server               
Parameters:                          
Returns:

*****************************************************************************/

int16_t	SOKConnect_Hndlr(int16_t sClSOK, char *remIP, uint16_t remPort)
{
  // Local Variables
  int16_t  retVal    = -1;
  // Initialize Server struct info (sockaddr_in)
  S_SADDR_IN  SrvAddr;
  bzero(&SrvAddr, sizeof(SrvAddr)); // Zero-out struct values
  // Struct Member Init
  SrvAddr.sin_family = AF_INET;
  SrvAddr.sin_port   = htons(remPort); // <- REMOTE SERVER PORT
  // Get remote server address
  if (inet_pton(AF_INET, remIP, &SrvAddr.sin_addr) <= 0)
  {
    printf("[-]Error for remote address: %s\n", remIP);
    // return EXIT_FAILURE;
  }
  // Connect to server
  // if (connect(sokFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr)) < 0)
  // {
  //   printf("\nError conecting to remote address: %s\n", REM_SRV_IP);
  //   return EXIT_FAILURE;
  // }
  // IPbuffer = inet_ntoa(SrvAddr.sin_addr);
  
  // printf("Connection to Remote Server = SUCCESS\n\n");
  // printf("Connected to remote address: %s\n", IPbuffer);
  // Connect System Call
  printf("[-]CONNECTING to remote address: %s...\n", remIP);
  retVal = connect(uClSok, (S_SADDR *)&SrvAddr, sizeof(SrvAddr));
  // Function Return
  return  retVal;    
}

// End SOKConnect_Hndlr() 
/****************************************************************************/


/*****************************************************************************

Name: SOKSend_Handlr()                                               
Purpose: Handles sending Data to the Server                             
Parameters: 
Returns: 

*****************************************************************************/

uint32_t	SokSend_Hndlr(uint32_t uClSok, char *pRqst, uint16_t pRqstLen)
{
  // Local Variables
  uint32_t   retVal = -1;
  TIME_V     Tv;
  Tv.tv_sec  = 20; // Time-Out in Seconds
  Tv.tv_usec = 0;
  // Set Socket Options
  if (setsockopt(uClSok, SOL_SOCKET, SO_SNDTIMEO, (char *)&Tv, sizeof(Tv)) < 0)
  {
    printf("[-]CONNECTION = TIME OUT\n");
    return EXIT_FAILURE;
  }
  // Send System Call to send request (parameters) to the Server
  retVal = send(uClSok, pRqst, pRqstLen, 0);
  // Function Return
  return  retVal;    
}

// End SOKSend_Hndlr() 
/****************************************************************************/


/*****************************************************************************

Name:	SOKRcv_Handlr()                                                
Purpose: Handles receiving Data form the Server                         
Parameters:     
Returns:                                         

*****************************************************************************/

uint32_t	SokRcv_Hndlr(uint32_t uClSok, char *pRsp, uint16_t rcvSize)
{
  // Local Variables
  uint32_t   retVal = -1;
  TIME_V     Tv;
  Tv.tv_sec  = 20; // Time-Out in Seconds
  Tv.tv_usec = 0;
  // Set Socket Options
  if (setsockopt(uClSok, SOL_SOCKET, SO_RCVTIMEO, (char *)&Tv, sizeof(Tv)) < 0)
  {
    printf("[-]CONNECTION = TIME OUT\n");
    return EXIT_FAILURE;
  }
  // Receive System Call to  receieve (parameters) from the Server
  retVal = recv(uClSok, pRsp, rcvSize, 0);
  // Output Response
  printf("\nServer Reply: %s\n\n", pRsp);
  // Function Return
  return  retVal;    
}

// End SOKRcv_Hndlr() 
/****************************************************************************/


/*****************************************************************************

Name: convertHex()                                               
Purpose:  Converts binary data to hexadecimal representation                               
Parameters:                                                            
Returns:                

*****************************************************************************/

char  *convertHex(const uint8_t *src, size_t len)
{
  char  *retVal;
  char  *hexBits = "0123456789ABCDEF";
  
  if (src == NULL || len <= 0)
    return NULL;
    
  uint32_t  retValLen = (len * 3) + 1;
  retVal = malloc(retValLen);
  // sets the first n bytes of the area starting at retVal 
  // to zero (bytes containing '\0')
  bzero(retVal, retValLen);
  
  for (uint8_t i = 0; i < len; i++)
  {
    retVal[i*3]     = hexBits[src[i] >> 4]; // Right shift all bits 4 places
    retVal[(i*3)+1] = hexBits[src[i] & 0x0F];
    retVal[(i*3)+2] = ' '; // Space each value
  }
  
  return retVal;
  
}

// End convertHex()
/****************************************************************************/


/****************************************************************************/
// End Client.c
