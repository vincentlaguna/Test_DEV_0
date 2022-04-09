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

// UNDER CONSTRUCTION -> Convert to Multi-Threaded Client
/*****************************************************************************

Name:	UDP_SrvConnection_Hndlr() thread version                                         
Purpose: Handles incoming connections to the server                 
Parameters: unsigned uint8_t pointer                                          
Returns: void                                        

*****************************************************************************
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
  // make buffer more dynamic so that it can read and parse from file input 04082022
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

  // TEST SECTION (#ifdef?)
  // test_data tst_data = malloc(1 * sizeof(test_data));
  
  // uint8_t *ptst_data = &tst_data;

  // uint8_t test_dst;
  
  test_buffer *ptst_buff;

  init_test_buffer(&ptst_buff);

  // test_serialize_data() continue here 02252022
  // test_insert_data(&ptst_buff, ptst_data, byte size from... ???);

  // test_read_data((uint8_t *)&test_dst, ptst_buff, size from... ???);

  // test_skip_data(ptst_buff, -size from... ???); example of rewinding back n size

  // test_reset_buffer(ptst_buff); // Reset buffer to ptst_buff->next = 0

  // test_free_buffer(ptst_buff); // Don't forget to free the buffer, please

  // END TEST SECTION (#endif?)
  
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
      
        puts("[+]Replying Back to Client Status: ACTIVE\n");
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
