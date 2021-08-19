/*****************************************************************************
Author(s) or Contributor(s): Version 0.1  ~<NAME> 2021 

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

Name: convertHex()                                               
Purpose:  Converts binary data to hexadecimal representation                               
Parameters: const unsigned char pointer and size_t for length                                                             
Returns:  Unsigned char pointer                                        

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


/*****************************************************************************

Name: SokInit_Handlr()                                               
Purpose:  Handles the creation of a Socket                               
Parameters: None                                                             
Returns:  Unsigned 16-bit integer                                        

*****************************************************************************/

uint16_t	SokInit_Handlr(void)
{
  // Local Variables
  uint32_t  hSok;
  // Output
  printf("\n>>> Client-Side Socket Initialization >>>\n\n");
  
  SLEEP
  
  // Socket System Call
  hSok = socket(AF_INET, SOCK_STREAM, 0);
  // Output Validation
  printf("\n<<< Client-Side Socket Init Success <<<\n\n");
  
  SLEEP
  // Function Return
  return  hSok;
}

// End SokInit_Handlr()
/****************************************************************************/


/*****************************************************************************

Name: SokConnect_Handlr()                                            
Purpose: Handles the Connection of a Socket to the Server               
Parameters: Unsigned 32-bit integer                                          
Returns: Unsigned 32-bit integer                                        

*****************************************************************************/

uint32_t	SokConnect_Hndlr(uint32_t uClSok, char *remIP)
{
  // Local Variables
  uint32_t  retVal    = -1;
  uint32_t  sPort     = TEST_PORT;
  // sock_addr_in initialization
  S_SADDR_IN  Cl      = {0};
  // Struct Member Init
  Cl.sin_family       = AF_INET;
  Cl.sin_addr.s_addr  = inet_addr(remIP); // <- REMOTE SERVER IP
  Cl.sin_port         = htons(sPort); // <- REMOTE SERVER PORT
  // Connect System Call
  retVal = connect(uClSok, (S_SADDR *)&Cl, sizeof(Cl));
  // Function Return
  return  retVal;    
}

// End SokConnect_Hndlr() 
/****************************************************************************/


/*****************************************************************************

Name: SokSend_Handlr()                                               
Purpose: Handles sending Data to the Server                             
Parameters: Unsigned 32-bit integer for Client Socket, (char) Pointer to   
            Request, Unsigned 16-bit integer for Length of the Request     
Returns: Unsigned 32-bit integer                                        

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
    printf("\nTIME OUT.\n");
    return EXIT_FAILURE;
  }
  // Send System Call to send request (parameters) to the Server
  retVal = send(uClSok, pRqst, pRqstLen, 0);
  // Function Return
  return  retVal;    
}

// End SokSend_Hndlr() 
/****************************************************************************/


/*****************************************************************************

Name:	SokRcv_Handlr()                                                
Purpose: Handles receiving Data form the Server                         
Parameters: Unsigned 32-bit integer for Client Socket, (char) Pointer to   
            Response, Unsigned 16-bit integer for Size of the Response     
Returns: Unsigned 32-bit integer                                        

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
    printf("\nTIME OUT.\n");
    return EXIT_FAILURE;
  }
  // Receive System Call to  receieve (parameters) from the Server
  retVal = recv(uClSok, pRsp, rcvSize, 0);
  // Output Response
  printf("\nServer Reply: %s\n\n", pRsp);
  // Function Return
  return  retVal;    
}

// End SokRcv_Hndlr() 
/****************************************************************************/


/****************************************************************************/
// End Client.c
