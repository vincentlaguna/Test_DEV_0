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

*****************************************************************************

int32_t	BindSrvSok_Hndlr(int16_t SrvSok, const uint8_t *szRemIP)
{
  printf("\nBind Call\n\n");
  // Local Variables
  int32_t   retVal;//   = -1;
  uint32_t  remPort  =  0;
  socklen_t SOKsz;
  S_SADDR_IN *Srv = NULL;
  Srv = (S_SADDR_IN *)malloc(sizeof(S_SADDR_IN));
  printf("\nStruct Memory Allocation = PASS\n\n");
  // Assign remPort Port to corresponding port number
  if (strcmp(szRemIP, szRem_Srv_IP[eREM_SRV_IP_0]) == 0)
  {
    remPort = REM_SRV_PORT_0;
  }
  else if (strcmp(szRemIP, szRem_Srv_IP[eREM_SRV_IP_1]) == 0)
  {
    remPort = REM_SRV_PORT_1;
  }
  else if (strcmp(szRemIP, szRem_Srv_IP[eREM_SRV_IP_2]) == 0)
  {
    remPort = REM_SRV_PORT_2;
  }
  else if (strcmp(szRemIP, szRem_Srv_IP[eREM_SRV_IP_3]) == 0)
  {
    remPort = REM_SRV_PORT_3;
  }
  else if (strcmp(szRemIP, szRem_Srv_IP[eREM_SRV_IP_4]) == 0)
  {
    remPort = REM_SRV_PORT_4;
  }
  else
  {
    EXIT_FAILURE;
  }
  printf("\nszRempIP = %s uRem = %s\n\n", szRemIP, szRem_Srv_IP[eREM_SRV_IP_0]);
  printf("\nREM_SRV_PORT_0 = %d size = %d\n\n", REM_SRV_PORT_0, sizeof(REM_SRV_PORT_0));
  printf("\nremPort = %d size = %d\n\n", remPort, sizeof(remPort));
  printf("\nSrvSOK = %d\n\n", SrvSok);
  // sock_addr_in initialization
  // S_SADDR_IN  Srv;
  // Zero-out struct
  memset(Srv, 0, sizeof(S_SADDR_IN));
  printf("\n\nZero-out allocated struct = PASS\n\n");
  // Struct Member Init
  Srv->sin_family      = AF_INET;
  printf("\n\nsin_family = OK\n\n");
  Srv->sin_addr.s_addr = htonl(*szRemIP);
  printf("\n\ns_addr (szRempIP) = OK\n\n");
  Srv->sin_port        = htons(remPort);
  printf("\n\nsin_port (remPort) = OK\n\n");
  SOKsz = sizeof(Srv);
  printf("[+]Binding to IP: %s on PORT: %d...\n", szRemIP, remPort);
  // Bind System Call
  // retVal = bind(SrvSok, (S_SADDR *)&Srv, sizeof(Srv));
  retVal = bind(SrvSok, (S_SADDR *)Srv, SOKsz);
  printf("\n\nretVal = %d\n\n", retVal);
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

Name:	UDP_SrvConnection_Hndlr() thread version                                         
Purpose: Handles incoming connections to the server                 
Parameters: unsigned uint8_t pointer                                          
Returns: void                                        

*****************************************************************************/


// void  UDP_SrvConnection_Hndlr(const uint16_t clSOKFD)
void  *UDP_SrvConnection_Hndlr(void *clSOKFD)
{
  int pClSOKFD = *clSOKFD
  free(clSOKFD);
  // Receive and Reply Buffers
  // uint8_t *rcvBuffer = NULL;
  // uint8_t *rplyBuffer = NULL;
  // rcvBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  // rplyBuffer = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  // // Local Variables
  // int sVal;
  // uint16_t remPort;
  // int16_t listenSOKFD; 
  // int clAddrLen;
  // Local Structs
  // S_SADDR_IN SrvAddr, ClAddr;
  // Assign remPort Port to corresponding port number
  // if (szRempIP == szRem_Srv_IP[eREM_SRV_IP_0])
  // {
  //   remPort = REM_SRV_PORT_0;
  // }
  // else if (szRempIP == szRem_Srv_IP[eREM_SRV_IP_1])
  // {
  //   remPort = REM_SRV_PORT_1;
  // }
  // else if (szRempIP == szRem_Srv_IP[eREM_SRV_IP_2])
  // {
  //   remPort = REM_SRV_PORT_2;
  // }
  // else if (szRempIP == szRem_Srv_IP[eREM_SRV_IP_3])
  // {
  //   remPort = REM_SRV_PORT_3;
  // }
  // else if (szRempIP == szRem_Srv_IP[eREM_SRV_IP_4])
  // {
  //   remPort = REM_SRV_PORT_4;
  // }
  // else
  // {
  //   EXIT_FAILURE;
  // }
  // Create a UDP Socket
  // if ((listenSOKFD = UDP_SokInit_Handlr()) < 0) 
  // {
  //   printf("[-]Creation of SOCKET = FAIL\n");
  //   // return EXIT_FAILURE;
  // }
  // Zero-out struct values
  // memset(&SrvAddr, 0, sizeof(SrvAddr));
  // SrvAddr.sin_addr.s_addr = htonl(*szRempIP);
  // SrvAddr.sin_port = htons(remPort);
  // SrvAddr.sin_family = AF_INET; 
  // bind server address to socket descriptor
  // bind(listenSOKFD, (struct sockaddr*)&SrvAddr, sizeof(SrvAddr));
  // printf("[+]Binding to IP: %s on PORT: %d\n", REM_SRV_IP_0, REM_SRV_PORT);
  // if ((BindSrvSok_Hndlr(listenSOKFD, REM_SRV_IP_0)) < 0)
  // {
  //   perror("[-]BIND = FAIL\n"); // Print the error message
  // }
  // printf("[+]Bind = OK\n");
  // //Receive the datagram
  // clAddrLen = sizeof(ClAddr);
  // While-Loop to receive data from incomming connections
  // while (1)
  // {
    // // receive message
    // int sVal = recvfrom(clSOKFD, rcvBuffer, MAX_LEN, 0,
    //                 (S_SADDR *)&ClAddr, &clAddrLen);
    // rcvBuffer[sVal] = '\0';
    // puts(rcvBuffer);
    // printf("\n[-]Confirming receive values...\n");
    // printf("\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)));
    
    // puts("\n");
    // strcpy(rplyBuffer, rcvBuffer);         
    // // send the response
    // sendto(clSOKFD, rplyBuffer, MAX_LEN, 0,
    //       (struct sockaddr*)&ClAddr, sizeof(ClAddr));
          
    // if (bCheckSum(rcvBuffer, cSerialData, sizeof(cSerialData)))
    // {
    //   printf("[+]CHECKSUM = PASS\n");
    // }
    // else
    // {
    //   printf("[+]CHECKSUM = FAIL\n");
    // }
    // puts("\n");
    // // Zero-out receive buffer
    // memset(rcvBuffer, '\0', MAX_LEN);
    // printf("This is where the magic would happen...\n");
  // }
  
  // free(rcvBuffer);
  // free(rplyBuffer);
  printf("%d\n", rID_Gen());
}

// End UDP_SrvConnection_Handlr() 
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


/*****************************************************************************

Name: rID_Gen()                                               
Purpose:  Random number generator                               
Parameters: None                                                             
Returns:  Random integer

*****************************************************************************/

int rID_Gen(void)
{
  int i;
  int retVal;
  int l  = 10;
  int u  = 1000;
  
  for (i = 0; i < 10; i++) 
  {
    int ct = (rand() % (u - l + 1)) + l;
    printf("%d ", ct);
    retVal = ct;
  }
  
  puts("\n\n");
  
  return retVal;
}

// End rID_Gen()
/****************************************************************************/


/****************************************************************************/
// End Server.c
