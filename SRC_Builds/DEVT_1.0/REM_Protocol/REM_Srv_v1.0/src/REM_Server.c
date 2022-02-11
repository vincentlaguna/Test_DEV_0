/*****************************************************************************
Author(s) or Contributor(s): Version (DEV) 1.0  ~Vincent A. Laguna 2022                                                                 

File:	Server.c                                                      

Description: Server-side code                                              
*****************************************************************************/


/* Includes: ****************************************************************/

#include "../inc/CS_Common.h"

/****************************************************************************/

/* Globals: *****************************************************************/

S_SADDR_IN SrvAddr;
S_SADDR_IN ClAddr;
int16_t listenSOKFD;

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
  // int16_t  *hSok;
  // hSok = malloc(sizeof(int16_t));
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

Name:	UDP_SrvConnection_Hndlr() thread version                                         
Purpose: Handles incoming connections to the server                 
Parameters: unsigned uint8_t pointer                                          
Returns: void                                        

*****************************************************************************/
// Clean Multi-Threaded Version
// void  UDP_SrvConnection_Hndlr(const uint16_t clSOKFD)
// void  *UDP_SrvConnection_Hndlr(void *clSOKFD)
// void  UDP_SrvConnection_Hndlr(void *sokData)
void  *UDP_SrvConnection_Hndlr(void *sokData)
{
  // Local Data
  SOKData      *lData;
  lData        = (SOKData *)malloc(sizeof(SOKData));
  lData        = (SOKData *)sokData;
  lData->sink1 = SYNC_1;
  lData->sink2 = SYNC_2; 
  lData->src   = SRC_ANY;
  lData->dst   = DST_SCU;
  lData->cmd   = PWR_CTRL;
  lData->len   = LEN;
  lData->msb   = MSB;
  lData->lsb   = LSB;
  lData->cs    = CS;

  // Receive and Reply Buffers
  uint8_t *rcvBuffer = NULL;
  uint8_t *rplyBuffer = NULL;
  rcvBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  rplyBuffer = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  // Local Variables
  socklen_t clAddrLen;
  int16_t listenSOKFD;
  
  // Local Structs
  S_SADDR_IN SrvAddr;
  S_SADDR_IN ClAddr;

  // #ifdef THREAD_TEST
  
    // pthread_mutex_t SOKlock;
    // pthread_mutex_init(&SOKlock, NULL);
    
    // pthread_mutex_lock(&SOKlock);
    // printf("\nIn Thread Handler: SOKid = %d\n", lData->SOKid);
    // SLEEP
    // srand(time(0));
    // lData->SOKid = rID_Gen();
    // printf("\nIn Thread Handler: changed SOKid = %d\n", lData->SOKid);
    printf("\nIn Thread Handler: cIP = %s\n", lData->cIP);
    printf("\nIn Thread Handler: uPort = %d\n", lData->uPort);
    // printf("\nIn Thread Handler: ipData->IP = %xd\n", lData->ipData->srvAddr.sin_addr.s_addr);
    // printf("\nIn Thread Handler: ipData->Port = %d\n", lData->ipData->srvAddr.sin_port);
    // printf("\nIn Thread Handler: ipData->IPDataID = %d\n", lData->ipData->IPDataID);
    
    if ((listenSOKFD = UDP_SokInit_Handlr()) < 0) 
    {
      printf("[-]Creation of SOCKET = FAIL\n");
    }
    // Zero-out struct
    memset(&SrvAddr, 0, sizeof(SrvAddr));
    SrvAddr.sin_family = AF_INET;
    SrvAddr.sin_addr.s_addr = inet_addr(lData->cIP);
    SrvAddr.sin_port = htons(lData->uPort);
    // Bind Server address to socket descriptor
    // printf("[+]Binding to IP: %s on PORT: %d...\n", lData->cIP, lData->uPort);
    // if ((bind(listenSOKFD, (S_SADDR *)&lData->ipData->srvAddr, sizeof(lData->ipData->srvAddr))) < 0)
    if ((bind(listenSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
    {
      perror("[-]BIND = FAIL\n"); // Print the error message
    }
    else
    {
      printf("[+]Bind = OK\n");
      printf("Inside Thread Handler...\n");
    }
    
    clAddrLen = sizeof(ClAddr);
    // While-Loop to receive data from incomming connections
    // while (1)
    // {
      // Testing placement of REMDataSnd() Function call
      REMDataSnd(lData->sink1, lData->sink2, lData->src, lData->dst, lData->cmd, lData->len, lData->data, lData->msb, lData->lsb, lData->cs);
      // receive message
      uint16_t sVal = recvfrom(listenSOKFD, rcvBuffer, MAX_LEN, 0,
                    (S_SADDR *)&ClAddr, &clAddrLen);
      rcvBuffer[sVal] = '\0';
    
    #ifdef DBG
      // Display Receive Buffer
      puts("[+]DEBUG STATUS: ENABLED\n");
      puts("[+]Displaying Recieve Buffer:\n");
      puts(rcvBuffer);
      // Validate
      printf("\n[-]Confirming receive values...\n");
      printf("\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)));
      
      puts("\n");
      printf("[-]Sending Response to Client...\n");
      
    #endif
      // Copying to reply buffer for sending
      // strcpy(rplyBuffer, cStringPayload);
      strcpy(rplyBuffer, cSerialData);      
      // Replying Buffer w/active notifier
      if (sendto(listenSOKFD, rplyBuffer, MAX_LEN, 0,
            (S_SADDR *)&ClAddr, sizeof(ClAddr)))
      {
      
      #ifndef DBG  
        
        puts("[-]DEBUG STATUS: DISABLED\n");
      
      #endif
      
        puts("[+]Replying Back to CLient Status: ACTIVE\n");
      }
          
    #ifdef DBG
  
      // printf("[-]CHECKSUM Validation...\n");
      // // Checksum Validation (for debugging)
      // if (bCheckSum(rcvBuffer, cSerialData, sizeof(cSerialData)))
      // {
      //   printf("[+]BYTES RECEIVED = %ld\n",
      //         (strlen(rcvBuffer))/(sizeof(uint8_t)) + 1);
              
      //   printf("[+]CHECKSUM = PASS\n");
      // }
      // else
      // {
      //   printf("[+]CHECKSUM = FAIL\n");
      // }

    #endif  
      
      puts("\n");
      // Zero-out receive buffer
      memset(rcvBuffer, '\0', MAX_LEN);
      memset(rplyBuffer, '\0', MAX_LEN);
  
      printf("This is where the magic would happen...\n");
    // }
    
    free(rcvBuffer);
    free(rplyBuffer);
    
    // pthread_mutex_unlock(&SOKlock);
    
    
    // int pClSOKFD = *((int *)clSOKFD);
    // free(clSOKFD);
    
    // int rID;
    
    // pthread_mutex_t SOKlock;
    // pthread_mutex_init(&SOKlock, NULL);
    
    // pthread_mutex_lock(&SOKlock);
    
    // if (pClSOKFD == 10)
    // {
    //   printf("\nSocket 10 = %d\n\n", pClSOKFD);
      
    //   srand(time(0));
    
    //   rID = rID_Gen();
    
    //   printf("Thread #%d\n\n", rID);
    
    // }
    // else if (pClSOKFD == 20)
    // {
    //   printf("\nSocket 20 = %d\n\n", pClSOKFD);
      
    //   SLEEP
      
    //   srand(time(0));
    
    //   rID = rID_Gen();
    
    //   printf("Thread #%d\n\n", rID++);
    // }
    
    // pthread_mutex_unlock(&SOKlock);
    
    // return NULL;
  
  // #else 
    
  return NULL;
}

// End UDP_SrvConnection_Handlr() 
/****************************************************************************/

void  init_test_buffer(test_buffer **d_pbuffer)
{
  // right now at least it is still compiling... 02112022
}

/*****************************************************************************

Name:	REM_DataSnd() function (for UDP threaded version server)                                        
Purpose:       
Parameters: Pointer to SOKData, unsigned uint8_t pointer to send buffer                                          
Returns: void                                        

*****************************************************************************/
//
void REMDataSnd(uint8_t *pSink1, uint8_t *pSink2, uint8_t *pSrc, uint8_t *pDst,
                uint8_t *pCmd, uint8_t *pLen, uint8_t *pData, 
                uint8_t *pMsb, uint8_t *pLsb, uint8_t *pCs)
{
  printf("\nTesting REMDataSnd() Function call >>> \n");
  printf("\nsink1 = %02X | sink2 = %02X | src = %02X | dst = %02X | cmd = %02X | len = %02X | data = %s | msb = %02X | lsb = %02X | cs = %02X\n\n", pSink1, pSink2, pSrc, pDst, pCmd, pLen, pData, pMsb, pLsb, pCs);

  // Build the message
  // Send the message in separate function
  // Notes to self: change the function arguments to specify the length
  // of data (bytes)
}

// End REM_DataSnd()
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
    // printf("%d ", ct);
    retVal = ct;
  }
  
  // puts("\n\n");
  
  return retVal;
}

// End rID_Gen()
/****************************************************************************/


/****************************************************************************/
// End Server.c
