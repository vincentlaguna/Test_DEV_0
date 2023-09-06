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

/* Globals: *****************************************************************/

S_SADDR_IN SrvAddr;
S_SADDR_IN ClAddr;
int16_t listenSOKFD;

/****************************************************************************/

/* Function Definitions: ****************************************************/

/*****************************************************************************

Name:	UDP_ClConnection_Hndlr() thread version                                         
Purpose: Handles incoming connections to the server                 
Parameters: unsigned uint8_t pointer                                          
Returns: void                                        

*****************************************************************************/
// Clean Multi-Threaded Version
void  *UDP_ClConnection_Hndlr(void *sokData)
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
  uint8_t *sndBuffer = NULL;
  rcvBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  sndBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  // Local Variables
  socklen_t clAddrLen;
  int16_t connectSOKFD;
  
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
    
    if ((connectSOKFD = UDP_SokInit_Handlr()) < 0) 
    {
      printf("[-]Creation of SOCKET = FAIL\n");
    }

    // Clear ClAddr <- used from working Client1_v0.1 20230906-1130
    memset(&ClAddr, 0, sizeof(ClAddr));
    ClAddr.sin_addr.s_addr = inet_addr(lData->cIP);
    ClAddr.sin_port = htons(lData->uPort);
    ClAddr.sin_family = AF_INET;

    memset(sndBuffer, '\0', MAX_LEN);
    strcpy(sndBuffer, cREMDataSYSReset);    

    if(connect(connectSOKFD, (S_SADDR *)&ClAddr, sizeof(ClAddr)) < 0)
    {
        printf("\n[-]CONNECT TO SERVER Error: Connect Failed \n");
        exit(0);
    }

    // Bind Server address to socket descriptor
    // printf("[+]Binding to IP: %s on PORT: %d...\n", lData->cIP, lData->uPort);
    // if ((bind(listenSOKFD, (S_SADDR *)&lData->ipData->srvAddr, sizeof(lData->ipData->srvAddr))) < 0)
    #ifdef CLIENT_CONNECT

      if(connect(connectSOKFD, (S_SADDR *)&ClAddr, sizeof(ClAddr)) < 0)
      {
        perror("[-]CONNECT = FAIL\n"); // Print the error message
      }
      else
      {
        printf("[+]Bind = OK\n");
        printf("Inside Thread Handler...\n");
      }

    #else

      if ((bind(listenSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
      {
        perror("[-]BIND = FAIL\n"); // Print the error message
      }
      else
      {
        printf("[+]Bind = OK\n");
        printf("Inside Thread Handler...\n");
      }
    
    #endif // CLIENT_CONNECT

    clAddrLen = sizeof(ClAddr);

    //strcpy(sndBuffer, cREMDataSYSReset);      
    //strcpy(rplyBuffer, cREMDataRFOn);
    // Buffer w/active notifier
    sendto(connectSOKFD, sndBuffer, MAX_LEN, 0, (S_SADDR *)&ClAddr, sizeof(ClAddr));
    // While-Loop to receive data from incomming connections
    // while (1)
    // {
      // receive message
      //uint16_t sVal = recvfrom(connectSOKFD, rcvBuffer, MAX_LEN, 0,
        //            (S_SADDR *)&SrvAddr, &srvAddrLen);
      uint16_t sVal = recvfrom(connectSOKFD, rcvBuffer, MAX_LEN, 0, (S_SADDR *)NULL, NULL);
      // for (int reader = 0; reader < sVal; reader++)
      // {
      //   if (rcvBuffer[reader] == "")
      //   {
      //     rcvBuffer[reader] = '\0';
      //     break;
      //   }
      // }
      
      //rcvBuffer[sVal++] = '\0';
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
      // strcpy(rplyBuffer, cSerialData);
      strcpy(rcvBuffer, cREMDataSYSReset);      
      //strcpy(rplyBuffer, cREMDataRFOn);
      // Replying Buffer w/active notifier
      sendto(listenSOKFD, rcvBuffer, MAX_LEN, 0, (S_SADDR *)&ClAddr, sizeof(ClAddr));
      // sendto(listenSOKFD, rplyBuffer, MAX_LEN, 0, (S_SADDR *)NULL, sizeof(ClAddr));
      // {
      
      #ifndef DBG  
        
        puts("[-]DEBUG STATUS: DISABLED\n");
      
      #endif
      
        puts("[+]Replying Back to Client Status: ACTIVE\n");
      // }
          
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
      memset(sndBuffer, '\0', MAX_LEN);
  
      printf("This is where the magic would happen...\n");
    // }
    
    free(rcvBuffer);
    free(sndBuffer);
    
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

// End UDP_ClConnection_Handlr() 
/****************************************************************************/

// Helper Functions

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
