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

Name:	SokInit_Handlr()                                               
Purpose: Handles the creation of a Socket                               
Parameters: None                                                             
Returns: signed 16-bit integer                                        

*****************************************************************************

int16_t  SokInit_Handlr(void)
{
  // Local Variables
  int16_t  hSok;
  // Output
  printf("\n[-]SERVER-Side Socket Initialization = in progress...\n");
  
  SLEEP
  
  // Socket System Call
  hSok = socket(AF_INET, SOCK_STREAM, 0);
  // Output Validation
  printf("[+]SERVER-Side Socket Initialization = OK\n");

  SLEEP
  // Function Return
  return  hSok;
}

// End SokInit_Handlr()
/****************************************************************************/


/*****************************************************************************

Name:	UDP_SokInit_Handlr()                                               
Purpose: Handles the creation of a UDP Socket                               
Parameters: None                                                             
Returns: signed 16-bit integer                                        

*****************************************************************************/

int16_t  UDP_SokInit_Handlr(void)
{
  // Local Variables
  int16_t  hSok;
  // Output
  printf("\n[-]SERVER-Side Socket Initialization = in progress...\n");
  // SLEEP
  // Socket System Call
  hSok = socket(AF_INET, SOCK_DGRAM, 0);        
  // Output Validation
  // SLEEP
  printf("[+]SERVER-Side Socket Initialization = OK\n");
  // SLEEP
  // Function Return
  return  hSok;
}

// End UDP_SokInit_Handlr()
/****************************************************************************/


/*****************************************************************************

Name: SokConnect_Handlr()                                            
Purpose: Handles the Connection of a Socket to the Server               
Parameters: Unsigned 32-bit integer                                          
Returns: Unsigned 32-bit integer                                        

*****************************************************************************

uint32_t	SokConnect_Hndlr(uint32_t uClSok, char *remIP, uint16_t remPort)
{
  // Local Variables
  uint32_t  retVal    = -1;
  // sock_addr_in initialization
  S_SADDR_IN  Cl      = {0};
  // Struct Member Init
  Cl.sin_family       = AF_INET;
  Cl.sin_addr.s_addr  = inet_addr(remIP); // <- REMOTE SERVER IP
  Cl.sin_port         = htons(remPort); // <- REMOTE SERVER PORT
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

*****************************************************************************

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

*****************************************************************************

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


/*****************************************************************************

Name: convertHex()                                               
Purpose:  Converts binary data to hexadecimal representation                               
Parameters: const unsigned char pointer and size_t for length                                                             
Returns:  Unsigned char pointer                                        

*****************************************************************************/

uint8_t  *convertHex(uint8_t *src, size_t len)
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

Name: bCheckSum()                                               
Purpose:  Validates the checksum on 2 input buffers                               
Parameters: const unsigned char pointer and size_t for length                                                             
Returns:  Boolean                                        

*****************************************************************************/

bool  bCheckSum(const uint8_t *buff1, const uint8_t *buff2, size_t sZ)
{
  bool bRetVal  = false;
  uint8_t chkBuff1 = 0;
  uint8_t chkBuff2 = 0;
  
  if (buff1 == NULL || buff2 == NULL)
    return NULL;
  
  while (sZ-- != 0)
  {
    // For debugging...
    // printf("%c ", *buff1);
    // printf("\n");
    // printf("%c ", *buff2);
    chkBuff1 -= *buff1++;
    chkBuff2 -= *buff2++;
  }
  // For debugging...
  // printf("\nChecksum for buffer 1 is: 0x%02x = %d\n", chkBuff1, chkBuff1);
  // printf("\nChecksum for buffer 2 is: 0x%02x = %d\n\n", chkBuff2, chkBuff2);
  if (chkBuff1 == chkBuff2)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// End checkSum()
/****************************************************************************/


/****************************************************************************/
// End Client.c
