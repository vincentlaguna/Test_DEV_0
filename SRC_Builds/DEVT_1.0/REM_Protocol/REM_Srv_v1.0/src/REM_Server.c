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

Name:	UDP_SrvConnection_Hndlr() thread version                                         
Purpose: Handles incoming connections to the server                 
Parameters: unsigned uint8_t pointer                                          
Returns: void                                        

*****************************************************************************/
// Clean Multi-Threaded Version
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
      for (int reader = 0; reader < sVal; reader++)
      {
        if (rcvBuffer[reader] == "\x03")
        {
          rcvBuffer[reader] = '\0';
          break;
        }
      }

      
      // rcvBuffer[sVal] = '\0';
    
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
      strcpy(rplyBuffer, cREMDataRFOn);
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

Name:	init_test_buffer()                                       
Purpose:  Test Prototype Function to test initializing a test buffer
Parameters: Double Pointer to test_buffer struct
Returns: void                                        

*****************************************************************************/
//
void  init_test_buffer(test_buffer **d_pbuffer)
{
  (*d_pbuffer) = (test_buffer *)calloc(1, sizeof(test_buffer));

  (*d_pbuffer)->tst_bffr_data = calloc(1, BUFF_SZ);

  (*d_pbuffer)->size = BUFF_SZ;

  (*d_pbuffer)->next = 0;
}

// End init_test_buffer() 
/****************************************************************************/


/*****************************************************************************

Name:	test_serialize_data()                                       
Purpose:  Test Prototype Function for serializing data struct for buffer
Parameters: Pointer to data, pointer to buffer                                         
Returns: void                                        

*****************************************************************************/
// Since the MIN_STR_SZ is 30, test_data object has 2B of padding...
void  test_serialize_data(test_data *p_data, test_buffer *p_buffer)
{
  if (!p_data)
  {
    uint16_t sntel_val = 0xFFFFFFFF; // 2B value
    test_insert_data(p_buffer, (uint8_t *)&sntel_val, sizeof(uint16_t));


    return;
  }
  
  test_insert_data(p_buffer, (uint8_t)p_data->u8_sz_data_0,
                   sizeof(uint8_t) * MIN_STR_SZ);
  test_insert_data(p_buffer, (uint8_t)&p_data->u8_data_1, sizeof(uint8_t));
  test_insert_nest_data(&p_data->nst_data_0, p_buffer); // Insert nest struct
  test_insert_data(p_buffer, (uint8_t)p_data->u16_data_2, sizeof(uint16_t));
  
  return; //
}

// End test_serialize_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_de_serialize_data()                                       
Purpose:  Test Prototype Function for de-serializing data struct for buffer
Parameters: Pointer to data, pointer to buffer                                         
Returns: Pointer to de-serialized data struct                                        

*****************************************************************************/
//
test_data *test_de_serialize_data(test_buffer *p_buffer)
{
  // x-val detection code here
  uint16_t sntel_val = 0; // 2B value
  test_parse_data((uint8_t *)&sntel_val, p_buffer, sizeof(uint16_t));

  if (sntel_val == 0xFFFFFFFF)
    return NULL;

  test_skip_data(p_buffer, -1 * sizeof(uint16_t));
  // Allocate memory to reconstruct the "object"
  test_data *p_tst_data = calloc(1, sizeof(test_data));
  test_parse_data((uint8_t *)p_tst_data->u8_sz_data_0,
                  p_buffer, sizeof(uint8_t) * MIN_STR_SZ);
  test_parse_data((uint8_t *)p_tst_data->u8_data_1, p_buffer, sizeof(uint8_t));
  // parse nested stuff...
  test_parse_data((uint8_t *)p_tst_data->u16_data_2, p_buffer, sizeof(uint16_t));
  
  return p_tst_data;
}

// End test_de_serialize_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_insert_data()                                       
Purpose:  Test Prototype Function for inserting data into data buffer
Parameters: Pointer to buffer struct, pointer to data, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_insert_data(test_buffer *p_buffer, uint8_t *p_data, uint8_t n_bytes)
{
  uint8_t alloc_sz = p_buffer->size - p_buffer->next;
  
  uint8_t rsz_ct = 0;

  while (alloc_sz < n_bytes)
  {
    p_buffer->size = p_buffer->size * 2;
    alloc_sz = p_buffer->size - p_buffer->next;
    rsz_ct = 1;
  }

  if (rsz_ct == 0)
  {
    memcpy((uint8_t *)p_buffer->tst_bffr_data + p_buffer->next, p_data, n_bytes);
  }
  // Size modification to buffer
  p_buffer->tst_bffr_data = realloc(p_buffer->tst_bffr_data, p_buffer->size);
  memcpy((uint8_t *)p_buffer->tst_bffr_data + p_buffer->next, p_data, n_bytes);
  p_buffer->next += n_bytes;

  return; //
}

// End test_insert_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_insert_nest_data()                                       
Purpose:  Test Prototype Function for inserting nested struct data into test buffer
Parameters: Pointer to test_buffer struct, pointer to data, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_insert_nest_data(nest_data *p_data, test_buffer *p_buffer)
{
  test_insert_data(p_buffer, (uint8_t)p_data->u8_sz_nst_data_0, sizeof(uint8_t) * MIN_STR_SZ);
  test_insert_data(p_buffer, (uint8_t)&p_data->u8_nst_data_ID, sizeof(uint8_t));
  test_serialize_data(p_data->p_test_data, p_buffer);
  
  return;
}

// End test_insert_nest_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_parse_data()                                       
Purpose:  Test Prototype Function for parsing data from data buffer
Parameters: Pointer to buffer struct, pointer to data, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_parse_data(test_buffer *p_buffer, uint8_t *p_data, uint8_t n_bytes)
{
  // Data extraction from buffer - does any of the previous stuff works? 04282022
  
  // uint8_t alloc_sz = p_buffer->size - p_buffer->next;
  
  // uint8_t rsz_ct = 0;

  // while (alloc_sz < n_bytes)
  // {
  //   p_buffer->size = p_buffer->size * 2;
  //   alloc_sz = p_buffer->size - p_buffer->next;
  //   rsz_ct = 1;
  // }

  // if (rsz_ct == 0)
  // {
  //   memcpy((uint8_t *)p_buffer->tst_bffr_data + p_buffer->next, p_data, n_bytes);
  // }
  // // Size modification to buffer
  // p_buffer->tst_bffr_data = realloc(p_buffer->tst_bffr_data, p_buffer->size);
  // memcpy((uint8_t *)p_buffer->tst_bffr_data + p_buffer->next, p_data, n_bytes);
  // p_buffer->next += n_bytes;

  return; //
}

// End test_parse_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_read_data()                                       
Purpose:  Test Prototype Function for reading data from test buffer
Parameters: Pointer to destination, pointer to test_buffer struct, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_read_data(uint8_t *p_dst, test_buffer *p_buffer, uint8_t size)
{
  memcpy(p_dst, p_buffer->tst_bffr_data + p_buffer->next, size);

  p_buffer->next += size;
}

// End test_read_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_skip_data()                                       
Purpose:  Test Prototype Function for skipping n data bytes from test buffer
Parameters: Pointer to test_buffer struct, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_skip_data(test_buffer *p_buffer, uint8_t skip_sz)
{
  if (p_buffer->next + skip_sz > 0 &&
       p_buffer->next + skip_sz < p_buffer->size)
    p_buffer->next += skip_sz;
}

// End test_skip_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_reset_buffer()                                       
Purpose:  Test Prototype Function for resetting test buffer
Parameters: Pointer to test_buffer struct                                          
Returns: void                                        

*****************************************************************************/
//
void  test_reset_buffer(test_buffer *p_buffer)
{
  if (p_buffer->next != 0)
    p_buffer->next = 0;
}

// End test_reset_buffer() 
/****************************************************************************/


/*****************************************************************************

Name:	test_free_buffer()                                       
Purpose:  Test Prototype Function for freeing test buffer
Parameters: Pointer to test_buffer struct                                          
Returns: void                                        

*****************************************************************************/
//
void  test_free_buffer(test_buffer *p_buffer)
{
  free(p_buffer->tst_bffr_data);
  free(p_buffer);
}

// End test_free_buffer() 
/****************************************************************************/


/*****************************************************************************

Name:	REM_DataSnd() function (for UDP threaded version server)                                        
Purpose:       
Parameters: Pointer to SOKData, unsigned uint8_t pointer to send buffer                                          
Returns: void                                        

*****************************************************************************/
// Only temp for testing
void REMDataSnd(uint8_t *pSink1, uint8_t *pSink2, uint8_t *pSrc, uint8_t *pDst,
                uint8_t *pCmd, uint8_t *pLen, uint8_t *pData, 
                uint8_t *pMsb, uint8_t *pLsb, uint8_t *pCs)
{
  printf("\nTesting REMDataSnd() Function call >>> \n");
  printf("\n<sink1 = %02X>|<sink2 = %02X>|<src = %02X>|<dst = %02X>|<cmd = %02X>|<len = %02X>|<data = %s>|<msb = %02X>|<lsb = %02X>|<cs = %02X>\n\n", pSink1, pSink2, pSrc, pDst, pCmd, pLen, pData, pMsb, pLsb, pCs);

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
