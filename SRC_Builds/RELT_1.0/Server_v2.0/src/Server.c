/*****************************************************************************
Author(s) or Contributor(s): Version 2.0 (Release)  ~Vincent A. Laguna 2022                                                                 

File:	Server.c                                                      

Description: Server-side code (Multi-Threaded)                                              
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

Name:	UDP_SrvConnection_Hndlr() thread version                                         
Purpose: Handles incoming connections to the server                 
Parameters: unsigned uint8_t pointer                                          
Returns: void                                        

*****************************************************************************/

void  *UDP_SrvConnection_Hndlr(void *sokData)
{
  // Local Data
  SOKData *lData;
  lData = (SOKData *)malloc(sizeof(SOKData));
  lData = (SOKData *)sokData;
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
  // Initialize UDP SOCKET
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
  if ((bind(listenSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
  {
    perror("[-]BIND = FAIL\n"); // Print the error message
  }
  else
  {
    printf("[+]Bind = OK\n");
    // printf("Inside Thread Handler...\n");
  }
    
  clAddrLen = sizeof(ClAddr);
  // While-Loop to receive data from incomming connections
  while (1)
  {
    // Receive Message
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
      strcpy(rplyBuffer, rcvBuffer);
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
  
      printf("[-]CHECKSUM Validation...\n");
      // Checksum Validation (for debugging)
      if (bCheckSum(rcvBuffer, cSerialData, sizeof(cSerialData)))
      {
        printf("[+]BYTES RECEIVED = %d\n",
              (strlen(rcvBuffer))/(sizeof(uint8_t)) + 1);
              
        printf("[+]CHECKSUM = PASS\n");
      }
      else
      {
        printf("[+]CHECKSUM = FAIL\n");
      }

    #endif  
      
      puts("\n");
      // Zero-out receive buffer
      memset(rcvBuffer, '\0', MAX_LEN);
      memset(rplyBuffer, '\0', MAX_LEN);
      // printf("This is where the magic would happen...\n");
    }
    
  free(rcvBuffer);
  free(rplyBuffer);

  return NULL;
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
