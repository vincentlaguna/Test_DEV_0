/*****************************************************************************
Author(s) or Contributor(s): Version 0.1  ~<NAME> 2021                                                                 

File:	Server.c                                                      

Description: Server-side code                                              
*****************************************************************************/


/* Includes: ****************************************************************/

#include "../inc/CS_Common.h"

/****************************************************************************/

/* Globals: *****************************************************************/
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

Name: BindSrvSok_Handlr()                                            
Purpose: Handles the binding of a Socket to the Server                  
Parameters: signed 16-bit integer                                          
Returns: signed 16-bit integer                                        

*****************************************************************************/

int16_t	BindSrvSok_Hndlr(int16_t uSrvSok, const uint8_t *remIP)
{
  // Local Variables
  int16_t   retVal   = -1;
  uint16_t  remPort;
  // Assign remPort Port to corresponding port number
  if (remIP == uRem_Srv_IP[eREM_SRV_IP_0])
  {
    remPort = REM_SRV_PORT_0;
  }
  else if (remIP == uRem_Srv_IP[eREM_SRV_IP_1])
  {
    remPort = REM_SRV_PORT_1;
  }
  else if (remIP == uRem_Srv_IP[eREM_SRV_IP_2])
  {
    remPort = REM_SRV_PORT_2;
  }
  else if (remIP == uRem_Srv_IP[eREM_SRV_IP_3])
  {
    remPort = REM_SRV_PORT_3;
  }
  else if (remIP == uRem_Srv_IP[eREM_SRV_IP_4])
  {
    remPort = REM_SRV_PORT_4;
  }
  else
  {
    EXIT_FAILURE;
  }
  // sock_addr_in initialization
  S_SADDR_IN  Srv     = {0};
  // Struct Member Init
  Srv.sin_family      = AF_INET;
  Srv.sin_addr.s_addr = htonl(*remIP);
  Srv.sin_port        = htons(remPort);
  // Bind System Call
  retVal = bind(uSrvSok, (S_SADDR *)&Srv, sizeof(Srv));
  // Function Return
  return  retVal;    
}

// End BindSrvSok_Handlr() 
/****************************************************************************/


/*****************************************************************************

Name:	SrvConnection_Hndlr()                                            
Purpose: Handles incoming connections to the server                 
Parameters: Unsigned 32-bit integer                                          
Returns: Unsigned 32-bit integer                                        

*****************************************************************************

void  SrvConnection_Hndlr(uint32_t uSrvSok, uint16_t nConnections)
{
  // Local Variables
  uint16_t  sok   = 0; 
  uint16_t  clLen = 0;
  
  S_SADDR   cL;
  // Initialize buffers to store the data
  DBffr  *SrvDbuff;
  SrvDbuff = (DBffr *)malloc(sizeof(DBffr));
  clLen = sizeof(S_SADDR_IN);
  // Create Socket
  uSrvSok = SokInit_Handlr();
  // Bind
  if (BindSrvSok_Hndlr(uSrvSok) < 0)
  {
    perror("BIND Failed."); // Print the error message
  }
  
  SLEEP
  
  printf("\n<<< BIND Done >>>\n\n");
  // Listen
  listen(uSrvSok, nConnections); // Number of MAX connections
  
  printf("\n>>> Waiting for incoming connections...\n\n");
  
  while (1)
  {
    // Accept connection from an incoming client
    sok = accept(uSrvSok, (S_SADDR *)&cL, (socklen_t *)&cL);
    
    if (sok < 0)
    {
      perror("ACCEPT Failed.");
    }
        
    printf("\nConnection ACCEPTED\n\n");
    // Buffers
    uint32_t  DbuffSize = sizeof(DBffr);
    memset(&SrvDbuff->cPayload, '\0', MAX_STR_SZ);
    // Receive a reply from the Client
    if (recv(sok, &SrvDbuff->cPayload, (uint32_t)DbuffSize, 0) < 0)
    {
      printf("\nRECEIVE Failed.\n");
    }
      
    printf("Client Message: %s\n", SrvDbuff->cPayload);
    // Send some data
    if(send(sok, &SrvDbuff->cPayload, MAX_STR_SZ, 0) < 0)
    {
      printf("\nSEND Failed.\n");
    }
      
    printf("\n<<< Waiting for incoming connections...\n");
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

Name:	UDP_SrvConnection_Hndlr()                                            
Purpose: Handles incoming connections to the server                 
Parameters: unsigned uint8_t pointer                                          
Returns: void                                        

*****************************************************************************/

void  UDP_SrvConnection_Hndlr(const uint8_t *remIP)
{
  // Receive and Reply Buffers
  // uint8_t rcvBuffer[MAX_LEN];
  // uint8_t rplyBuffer[MAX_LEN];
  uint8_t *rcvBuffer = NULL;
  uint8_t *rplyBuffer = NULL;
  rcvBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  rplyBuffer = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  // Local Variables
  // uint16_t remPort;
  int16_t listenSOKFD; 
  int clAddrLen;
  // Local Structs
  S_SADDR_IN SrvAddr, ClAddr;
  // Assign remPort Port to corresponding port number
  // if (remIP == uRem_Srv_IP[eREM_SRV_IP_0])
  // {
  //   remPort = REM_SRV_PORT_0;
  // }
  // else if (remIP == uRem_Srv_IP[eREM_SRV_IP_1])
  // {
  //   remPort = REM_SRV_PORT_1;
  // }
  // else if (remIP == uRem_Srv_IP[eREM_SRV_IP_2])
  // {
  //   remPort = REM_SRV_PORT_2;
  // }
  // else if (remIP == uRem_Srv_IP[eREM_SRV_IP_3])
  // {
  //   remPort = REM_SRV_PORT_3;
  // }
  // else if (remIP == uRem_Srv_IP[eREM_SRV_IP_4])
  // {
  //   remPort = REM_SRV_PORT_4;
  // }
  // else
  // {
  //   EXIT_FAILURE;
  // }
  // Create a UDP Socket
  if ((listenSOKFD = UDP_SokInit_Handlr()) < 0) 
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    // return EXIT_FAILURE;
  }
  // Zero-out struct values
  memset(&SrvAddr, 0, sizeof(SrvAddr));
  
  SrvAddr.sin_addr.s_addr = htonl(*remIP);
  SrvAddr.sin_port = htons(remPort);
  SrvAddr.sin_family = AF_INET; 
  // bind server address to socket descriptor
  bind(listenSOKFD, (struct sockaddr*)&SrvAddr, sizeof(SrvAddr));
  //Receive the datagram
  clAddrLen = sizeof(ClAddr);
  while (1)
  {
    // receive message
    int n = recvfrom(listenSOKFD, rcvBuffer, MAX_LEN, 0,
    // int n = recvfrom(listenSOKFD, rcvBuffer, sizeof(rcvBuffer), 0,
                    (struct sockaddr*)&ClAddr,&clAddrLen);
    rcvBuffer[n] = '\0';
    puts(rcvBuffer);
    strcpy(rplyBuffer, rcvBuffer);         
    // send the response
    sendto(listenSOKFD, rplyBuffer, MAX_LEN, 0,
          (struct sockaddr*)&ClAddr, sizeof(ClAddr));
  }
  free(rcvBuffer);
  free(rplyBuffer);
}

// End UDP_SrvConnection_Handlr() 
/****************************************************************************/


/*****************************************************************************

Name: convertHex()                                               
Purpose:  Converts binary data to hexadecimal representation                               
Parameters: const unsigned char pointer and size_t for length                                                             
Returns:  Unsigned char pointer                                        

*****************************************************************************/

char  *convertHex(const char *src, size_t len)
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

// void  UDP_SrvConnection_Hndlr(const uint8_t *remIP)
// {
//   printf("\n%s\n\n", remIP);
//   printf("\n%s, %s\n\n", *(uRem_Srv_IP), *(uRem_Srv_IP+1));
// }