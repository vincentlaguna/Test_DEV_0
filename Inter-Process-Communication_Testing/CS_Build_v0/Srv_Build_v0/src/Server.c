/*****************************************************************************
Author(s) or Contributor(s): Version .0  ~<NAME> 2021                                                                 

File:	Server.c                                                      

Description: Server-side code                                              
*****************************************************************************/


/* Includes: ****************************************************************/

#include "../inc/CS_Common.h"

/****************************************************************************/


/* Function Definitions: ****************************************************/
// Helper Functions

/*****************************************************************************

Name:	SOKInit_Handlr()                                               
Purpose: Handles the creation of a Socket                               
Parameters:                                                             
Returns: 

*****************************************************************************/

int16_t  SOKInit_Handlr(void)
{
  // Local Variables
  int16_t  hSOK;
  // Output
  printf("[-]SERVER-Side Socket Initialization = in progress...\n");
  SLEEP
  // Socket System Call
  hSOK = socket(AF_INET, SOCK_STREAM, 0);
  // Output Validation
  printf("[+]SERVER-Side Socket Initialization = OK\n");
  SLEEP
  // Function Return
  return  hSOK;
}

// End SOKInit_Handlr()
/****************************************************************************/


/*****************************************************************************

Name: BindSrvSOK_Handlr()                                            
Purpose: Handles the binding of a Socket to the Server                  
Parameters:                                          
Returns:

*****************************************************************************/

uint32_t	BindSrvSOK_Hndlr(int16_t sSrvSOK, uint16_t sPort)
{
  // Local Variables
  int16_t  retVal    = -1;
  // sock_addr_in initialization
  S_SADDR_IN  Srv;
  // Struct Member Init
  bzero(&Srv, sizeof(Srv)); // Zero-out struct values
  Srv.sin_family      = AF_INET;
  Srv.sin_addr.s_addr = htonl(INADDR_ANY);
  Srv.sin_port        = htons(sPort);
  // Bind System Call
  retVal = bind(sSrvSOK, (S_SADDR *)&Srv, sizeof(Srv));
  printf("[+]Binding to PORT: %d...\n", sPort);
  // Function Return
  return  retVal;    
}

// End BindSrvSOK_Handlr() 
/****************************************************************************/


/*****************************************************************************

Name:	SrvConnection_Hndlr()                                            
Purpose: Handles incoming connections to the server                 
Parameters:                                          
Returns:                                        

*****************************************************************************/

void  SrvConnection_Hndlr(int16_t sSrvSOK, uint16_t nConnections, uint16_t sPort)
{
  // Local Variables
  int16_t connSOK;
  int16_t sVal;
  // Initialize buffers to store the data (struct data)
  // DBffr  *SrvDbuff;
  // SrvDbuff = (DBffr *)malloc(sizeof(DBffr));
  // No struct
  uint8_t buffer[MAX_LEN+1];
  uint8_t replyLine[MAX_LEN+1];
  // Create Socket
  sSrvSOK = SokInit_Handlr();
  // Bind
  printf("[-]Binding = in progress...\n");
  if (BindSrvSok_Hndlr(sSrvSOK, sPort) < 0)
  {
    perror("[-]BIND = FAIL\n"); // Print the error message
  }
  
  SLEEP
  
  printf("[+]Bind = OK\n");
  // Listen
  if ((listen(sSrvSOK, nConnections)) < 0) // MAX number of connections
  {
    perror("[-]LISTEN = FAIL");
  }
  printf("[+]LISTEN = OK\n");
  printf("[+]LISTENING ON PORT = %d\n", sPort);
  printf("[+]MAX # CONNECTIONS = %d\n", nConnections);
  printf("[-]Waiting for incoming connections...\n");
  
  while (1)
  {
    // Initialize these local variables in function stack -> per/connection
    socklen_t   clAddrLen;
    S_SADDR_IN  cL;
    // Accept connection from an incoming client
    connSOK = accept(sSrvSOK, (S_SADDR *)&cL, (socklen_t *)&cL);
    // Connection Error Handling
    if (connSOK < 0)
    {
      perror("[-]INCOMING CONNECTION ACCEPT = FAIL\n");
    }
    printf("[+]INCOMING CONNECTION ACCEPT = OK\n");
    // Buffers (struct data)
    // uint16_t  DbuffSize = sizeof(DBffr);
    // memset(&SrvDbuff->cPayload, '\0', MAX_STR_SZ);
    // Zero-out the receive buffer and null terminate it (no struct data)
    bzero(buffer, MAX_LEN);
    bzero(replyLine, MAX_LEN);
    // Receive a reply from the Client
    // if (recv(sok, &SrvDbuff->cPayload, (uint32_t)DbuffSize, 0) < 0)
    // {
    //   printf("\nRECEIVE Failed.\n");
    // }
      
    // printf("Client Message: %s\n", SrvDbuff->cPayload);
    // // Send some data
    // if(send(sok, &SrvDbuff->cPayload, MAX_STR_SZ, 0) < 0)
    // {
    //   printf("\nSEND Failed.\n");
    // }
    while ((sVal = read(connSOK, buffer, MAX_LEN-1)) > 0)
    {
      fprintf(stdout, "%s\n\n%s\n", convertHex(buffer, sVal), buffer);
      // Look for end of message
      if (buffer[n-1] == '\n' || '\0')
        break;
    }
    // Output and prepare server reply
    printf("buffer: %s\n", buffer);
    strcpy(replyLine, buffer);
    // Write and close connection socket
    write(connSOK, replyLine, strlen(replyLine));
    close(connSOK);
    // printf("\n<<< Waiting for incoming connections...\n");
    // // Accept Connection from another incoming Client
    // sok = accept(uSrvSok, (S_SADDR *)&cL, (socklen_t*)&clLen);
     
    // if (sok < 0)
    // {
    //   perror("ACCEPT Failed.");
    // }
    // printf("\nConnection ACCEPTED\n\n");
      
    // #ifndef LIN
    //   closesocket(uSrvSok);
    //   WSACleanup();
    // #else
    //   close(uSrvSok);
    // #endif
  }
  
  SLEEP
  
}

// End SrvConnection_Handlr() 
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
// End Server.c
