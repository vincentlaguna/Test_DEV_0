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
  // Recent testing for initializing generic examples... 01232023
  person_t p_person_t;
  memset(&p_person_t, 0, sizeof(person_t));

  p_person_t.age    = 34;
  p_person_t.height = 180;
  p_person_t.last_sal_amounts[0] = NULL;
  p_person_t.last_sal_amounts[1] = NULL;
  p_person_t.last_sal_amounts[2] = calloc(1, sizeof(unsigned int));
  p_person_t.last_sal_amounts[2] = 10000;
  p_person_t.last_sal_amounts[3] = calloc(1, sizeof(unsigned int));
  p_person_t.last_sal_amounts[3] = 20000;
  p_person_t.last_sal_amounts[4] = NULL;

  strncpy(p_person_t.name, "Dummy_No_1", strlen("Dummy_No_1"));
  strncpy(p_person_t.company.comp_name, "Stumpany", strlen("Stumpany"));
  p_person_t.company.emp_strength = 10000;

  p_person_t.CEO = NULL;
  

  // TEST SECTION (#ifdef?)
  test_data *tst_data;// = (test_data *)malloc(1 * sizeof(test_data));
  tst_data  = (test_data *)malloc(1 * sizeof(test_data));
  // *tst_data->u8_sz_data_0  = malloc(sizeof(uint8_t) * MIN_STR_SZ);
  // uint8_t *ptst_data = &tst_data;

  // uint8_t test_dst;

  uint8_t size_tst_data;
  size_tst_data = sizeof(tst_data);
  
  test_buffer *ptst_buff = NULL;

  init_test_buffer(&ptst_buff);

  test_insert_data(ptst_buff, tst_data, size_tst_data);

  // test_serialize_data() // continue here 02252022 - continuing 08142022 (welcome back...)

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
    #ifdef SERVER_CONNECT
    
      if(connect(listenSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr)) < 0)
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
    
    #endif // SERVER_CONNECT

    clAddrLen = sizeof(ClAddr);
    // While-Loop to receive data from incomming connections
    // while (1)
    // {
      // Testing placement of REMDataSnd() Function call
      REMDataSnd(lData->sink1, lData->sink2, lData->src, lData->dst, lData->cmd, lData->len, lData->data, lData->msb, lData->lsb, lData->cs);
      // receive message
      uint16_t sVal = recvfrom(listenSOKFD, rcvBuffer, MAX_LEN, 0,
                    (S_SADDR *)&ClAddr, &clAddrLen);
      // for (int reader = 0; reader < sVal; reader++)
      // {
      //   if (rcvBuffer[reader] == "")
      //   {
      //     rcvBuffer[reader] = '\0';
      //     break;
      //   }
      // }
      
      rcvBuffer[sVal++] = '\0';
    
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
      strcpy(rplyBuffer, cREMDataSYSReset);      
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

// END Server.c