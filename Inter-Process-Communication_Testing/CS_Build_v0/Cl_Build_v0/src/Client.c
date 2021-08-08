/*****************************************************************************
* Author(s):                                                                 *
*                                                                            *
* File:		Client.c                                                           *
*                                                                            *
* Description: Client-side code                                              *
*                                                                            *
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
*                                                                            *
* Name:			  SokInit_Handlr()                                               *
* Purpose:    Handles the creation of a Socket                               *
* Parameters: None                                                           *  
* Returns:		Unsigned 16-bit integer                                        *
*                                                                            *
*****************************************************************************/

uint16_t	SokInit_Handlr(void)
{
  // Local Variables
  uint32_t                  hSok;
  // Output
  printf("\n>>> Client-Side Socket Initialization >>>\n\n");
  
  sleep(1);
  
  // Socket System Call
  hSok                   = socket(AF_INET, SOCK_STREAM, 0);
  // Output Validation
  printf("\n<<< Client-Side Socket Init Success <<<\n\n");
  sleep(1);
  // Function Return
  return                    hSok;
}

// End SokInit_Handlr()
/****************************************************************************/


/*****************************************************************************
*                                                                            *
* Name:			  SokConnect_Handlr()                                            *
* Purpose:    Handles the Connection of a Socket to the Server               *
* Parameters: Unsigned 32-bit integer                                        *  
* Returns:		Unsigned 32-bit integer                                        *
*                                                                            *
*****************************************************************************/

uint32_t	SokConnect_Hndlr(uint32_t uClSok)
{
  // Local Variables
  uint32_t    retVal      = -1;
  uint32_t    sPort       = TEST_PORT;
  // sock_addr_in initialization
  S_SADDR_IN  Cl          = {0};
  // Struct Member Init
  Cl.sin_family          = AF_INET;
  Cl.sin_addr.s_addr     = inet_addr(LOCAL_IP);
  Cl.sin_port            = htons(sPort);
  // Connect System Call
  retVal    = connect(uClSok, (S_SADDR *)&Cl, sizeof(Cl));
  // Function Return
  return      retVal;    
}

// End SokConnect_Hndlr() 
/****************************************************************************/


/*****************************************************************************
*                                                                            *
* Name:			  SokSend_Handlr()                                               *
* Purpose:    Handles sending Data to the Server                             *
* Parameters: Unsigned 32-bit integer for Client Socket, (char) Pointer to   *
*             Request, Unsigned 16-bit integer for Length of the Request     *
* Returns:		Unsigned 32-bit integer                                        *
*                                                                            *
*****************************************************************************/

uint32_t	SokSend_Hndlr(uint32_t uClSok, char *pRqst, uint16_t pRqstLen)
{
  // Local Variables
  uint32_t    retVal      = -1;
  TIME_V      Tv;
  Tv.tv_sec               = 20; // Time-Out in Seconds
  Tv.tv_usec              = 0;
  // Set Socket Options
  if (setsockopt(uClSok, SOL_SOCKET, SO_SNDTIMEO, (char *)&Tv, sizeof(Tv)) < 0)
  {
    printf("\nTIME OUT.\n");
    return EXIT_FAILURE;
  }
  // Send System Call to send request (parameters) to the Server
  retVal = send(uClSok, pRqst, pRqstLen, 0);
  // Function Return
  return      retVal;    
}

// End SokSend_Hndlr() 
/****************************************************************************/


/*****************************************************************************
*                                                                            *
* Name:			  SokRcv_Handlr()                                                *
* Purpose:    Handles receiving Data form the Server                         *
* Parameters: Unsigned 32-bit integer for Client Socket, (char) Pointer to   *
*             Response, Unsigned 16-bit integer for Size of the Response     *
* Returns:		Unsigned 32-bit integer                                        *
*                                                                            *
*****************************************************************************/

uint32_t	SokRcv_Hndlr(uint32_t uClSok, char *pRsp, uint16_t rcvSize)
{
  // Local Variables
  uint32_t    retVal      = -1;
  TIME_V      Tv;
  Tv.tv_sec               = 20; // Time-Out in Seconds
  Tv.tv_usec              = 0;
  // Set Socket Options
  if (setsockopt(uClSok, SOL_SOCKET, SO_RCVTIMEO, (char *)&Tv, sizeof(Tv)) < 0)
  {
    printf("\nTIME OUT.\n");
    return EXIT_FAILURE;
  }
  // Receive System Call to  receieve (parameters) from the Server
  retVal = recv(uClSok, pRsp, rcvSize, 0);
  // Output Response
  printf("\nResponse: %s\n\n", pRsp);
  // Function Return
  return      retVal;    
}

// End SokRcv_Hndlr() 
/****************************************************************************/

/****************************************************************************/
// End Client.c
