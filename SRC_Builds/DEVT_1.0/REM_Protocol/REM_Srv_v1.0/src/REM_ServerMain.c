/*****************************************************************************
Author(s) or Contributor(s): VVersion (DEV) 1.0  ~Vincent A. Laguna 2022                                                                 

File: ServerMain.c              

Description: REMOTE Protocol Server-side Main                                              

*****************************************************************************/


/* Includes: ****************************************************************/

#include  "../inc/CS_Common.h"

/****************************************************************************/

/* Main Starts Here: ********************************************************/
// Refactored Main (Multi-threaded)
int main(int argc, char *argv[])
{ 
  // Initialize Local Objects
  SOKData   sokData[NUM_SRV_THREADS];
  pthread_t SOKthread[NUM_SRV_THREADS];
  // Initialize Number of Server Thread Objects in a for-loop
  for (int i = 0; i < NUM_SRV_THREADS; i++)
  {
  #ifdef DBG
  
    sokData[i].SOKid = (i+1) * 10;
  
  #endif
  //Call Rem function here
    // Basic Assignment
    sokData[i].cIP   = malloc(sizeof(uint8_t) * IP_STR_SZ);
    sokData[i].cIP   = szRem_Srv_IP[i];
    sokData[i].uPort = uRem_Srv_Port[i];
    
    sokData[i].data  = malloc(sizeof(uint8_t) * IP_STR_SZ);
    sokData[i].data  = szData[i];
    // Initialize Thread Handlers  
    pthread_create(&SOKthread[i], NULL, UDP_SrvConnection_Hndlr, (void *)&sokData[i]);
  
  #ifdef DBG
  
    printf("\nIn Main: SOKid = %d\n", sokData[i].SOKid);
  
  #endif
  
    
  }
  // Join Threads (EXIT Thread Handlers)
  for (int j = 0; j < NUM_SRV_THREADS; j++)
  {
    pthread_join(SOKthread[j], NULL);
  }
   
  return(0);

}

/****************************************************************************/


/* Main Starts Here: ********************************************************

int main(int argc, char *argv[])
{
// #ifndef M_THREADED_SOKETS  
  // Initialize Local Variables
  // socklen_t   clAddrLen;
  // int16_t     listenSOKFD;
  // S_SADDR_IN  SrvAddr;
  // S_SADDR_IN  ClAddr;

// #endif
  // Receive and Reply Buffers
  // uint8_t *rcvBuffer  = NULL;
  // uint8_t *rplyBuffer = NULL;
  // rcvBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  // rplyBuffer = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);

// #ifdef USE_TCP
//   // Initialize Local Variables
//   int16_t listenSOKFD, connectSOKFD; 
//   S_SADDR_IN  SrvAddr; 
//   S_SADDR_IN  ClAddr;
//   // Winsock
//   #ifndef   LIN
    
//     WSADATA wsaData;
    
//     if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
//     {
//       fprintf(stderr,"[-]WSAStartup() = FAIL\n");
//       exit(1);
//     }
    
//   #endif // LIN
//   // Create Socket File Descriptor to listen on (Server)
//   // SLEEP
//   printf("\n[-]SERVER-Side Socket Initialization = in progress...\n");
//   if ((listenSOKFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) // TCP
//   {
//     printf("[-]Creation of SOCKET = FAIL\n");
//     return EXIT_FAILURE;
//   }
//   // SLEEP
//   printf("[+]SERVER-Side Socket Initialization = OK\n");
//   // SLEEP
//   // Setup Server Info
//   memset(&SrvAddr, 0, sizeof(SrvAddr)); // Zero-out struct values
//   memset(&ClAddr, 0, sizeof(ClAddr)); // Zero-out struct values
//   SrvAddr.sin_family      = AF_INET;
//   SrvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//   SrvAddr.sin_port        = htons(REM_SRV_PORT);
//   // Bind Call
//   // SLEEP
//   printf("[+]Binding to PORT: %d...\n", REM_SRV_PORT);
//   if ((bind(listenSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
//   {
//     perror("[-]BIND = FAIL\n"); // Print the error message
//   }
//   // SLEEP
//   printf("[+]Bind = OK\n");
//   // Listen Call
//   // SLEEP
//   if ((listen(listenSOKFD, MAX_CONN)) < 0)
//   {
//     printf("[-]LISTEN = FAIL\n");
//   }
//   printf("[+]# MAX CONNECTIONS = %d\n", MAX_CONN);
//   // SLEEP
//   printf("[+]LISTEN = OK\n");
//   // SLEEP
//   printf("[+]LISTENING ON PORT = %d\n", REM_SRV_PORT);
//   // SLEEP
//   printf("[-]Waiting for incoming connections...\n\n");
//   // SLEEP
//   // While-loop for server to listen on binded port for incoming connections
//   while(1)
//   {
//     // Initialize struct and variable for client info
//     // S_SADDR_IN  ClAddr;
//     socklen_t   clAddrLen;
//     // Accept will block until incoming connection arrives,
//     // returning Socket File Descriptor to the connection
//     // fflush(stdout);
//     // Accept Call
//     connectSOKFD = accept(listenSOKFD, (S_SADDR *)&ClAddr, (socklen_t *)&ClAddr);
//     if (connectSOKFD < 0)
//     {
//       printf("[-]INCOMING CONNECTION ACCEPT = FAIL\n");
//     }
//     printf("[+]INCOMING CONNECTION ACCEPT = OK\n");
//     // SLEEP
//     // Zero-out buffers
//     memset(rcvBuffer, '\0', MAX_LEN);
//     memset(rplyBuffer, '\0', MAX_LEN);
//     // Reading the client's message
//     printf("[-]SERVER = RECEIVING DATA...\n\n");
//     printf("[+]DATA RECEIVED: \n");
    
//     while ((uint16_t sVal = read(connectSOKFD, rcvBuffer, MAX_LEN-1)) > 0)
//     {
//       fprintf(stdout, "%s\n\n%s\n", convertHex(rcvBuffer, sVal), rcvBuffer);
//       // Look for end of message
//       if (rcvBuffer[strlen(rcvBuffer)-1] == ('\n') || ('\0'))
//         break;
//     }
//     printf("\nConfirm receive buffer: %s\n", rcvBuffer);
//     printf("[+]DATA RECIEVED = OK\n");
//     printf("[+]BYTES RECIEVED = %d\n", sizeof(rcvBuffer));
//     printf("[+]LENGTH RECEIVED = %d\n", strnlen(rcvBuffer, MAX_LEN));
//     strcpy(rplyBuffer, rcvBuffer);
//     printf("[-]SERVER = Replied data back to client...\n\n");
//     printf("[-]Waiting for incoming connections...\n\n");
//     write(connectSOKFD, rplyBuffer, strlen(rplyBuffer));
//     close(connectSOKFD);
//     // Zero-out buffers
//     memset(rcvBuffer, '\0', MAX_LEN);
//     memset(rplyBuffer, '\0', MAX_LEN);
//   }

// #else // USE UDP
  
  // #ifdef M_THREADED_SOKETS
    // // Local Variables
    // int listenSOKFD[2];
    // socklen_t clAddrLen[2];
    // // Local Structs
    // S_SADDR_IN SrvAddr[2];
    // S_SADDR_IN ClAddr[2];

  // #else // Non Multi-threaded code
    
  // #endif // M_THREADED_SOKETS
  
  // #ifdef M_THREADED_SOKETS
    // // Create a UDP Socket #1
    // // listenSOKFD = socket(AF_INET, SOCK_DGRAM, 0);        
    // if ((listenSOKFD[0] = UDP_SokInit_Handlr()) < 0) 
    // {
    //   printf("[-]Creation of SOCKET = FAIL\n");
    //   return EXIT_FAILURE;
    // }
    // // Create a UDP Socket #2
    // if ((listenSOKFD[1] = UDP_SokInit_Handlr()) < 0) 
    // {
    //   printf("[-]Creation of SOCKET = FAIL\n");
    //   return EXIT_FAILURE;
    // }
    // // Zero-out struct (1st socket)
    // memset(&SrvAddr[0], 0, sizeof(SrvAddr[0]));
    // SrvAddr[0].sin_family = AF_INET;
    // SrvAddr[0].sin_addr.s_addr = inet_addr(REM_SRV_IP_0);
    // SrvAddr[0].sin_port = htons(REM_SRV_PORT_0);
    
    // // Zero-out struct (2nd Socket)
    // memset(&SrvAddr[1], 0, sizeof(SrvAddr[1]));
    // SrvAddr[1].sin_family = AF_INET;
    // SrvAddr[1].sin_addr.s_addr = inet_addr(REM_SRV_IP_0);
    // SrvAddr[1].sin_addr.s_addr = inet_addr(REM_SRV_IP_0);
    // // Bind Server address to socket descriptor
    // printf("[+]Binding to IP: %s on PORT: %d...\n", REM_SRV_IP_0, REM_SRV_PORT_0);
    // if ((bind(listenSOKFD[1], (S_SADDR *)&SrvAddr[1], sizeof(SrvAddr[1]))) < 0)
    // {
    //   perror("[-]BIND = FAIL\n"); // Print the error message
    // }
    // else
    // {
    //   printf("[+]Bind = OK\n");
    // }
    // // Second Socket
    // printf("[+]Binding to IP: %s on PORT: %d...\n", REM_SRV_IP_0, REM_SRV_PORT_1);
    // if ((bind(listenSOKFD[1], (S_SADDR *)&SrvAddr[1], sizeof(SrvAddr[1]))) < 0)
    // {
    //   perror("[-]BIND = FAIL\n"); // Print the error message
    // }
    // else
    // {
    // printf("[+]Bind = OK\n");
    // }
    // //Receive the datagram
    // clAddrLen[0] = sizeof(ClAddr);
    
  // #else // Non-Thread code
    // Initialize Socket
    // if ((listenSOKFD = UDP_SokInit_Handlr()) < 0) 
    // {
    //   printf("[-]Creation of SOCKET = FAIL\n");
    //   return EXIT_FAILURE;
    // }
    // // Zero-out struct
    // memset(&SrvAddr, 0, sizeof(SrvAddr));
    // SrvAddr.sin_family = AF_INET;
    // SrvAddr.sin_addr.s_addr = inet_addr(REM_SRV_IP_0);
    // SrvAddr.sin_port = htons(REM_SRV_PORT_0);
    // // Bind Server address to socket descriptor
    // printf("[+]Binding to IP: %s on PORT: %d...\n", REM_SRV_IP_0, REM_SRV_PORT_0);
    // if ((bind(listenSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
    // {
    //   perror("[-]BIND = FAIL\n"); // Print the error message
    // }
    // else
    // {
    //   printf("[+]Bind = OK\n");
    // }
    
  // #endif // M_THREADED_SOKETS
     
  // #ifdef M_THREADED_SOKETS
    // Multi-threaded code
    // While-Loop to receive data from incomming connections
    // printf("[-]WAITING FOR INCOMING CONNECTIONS...\n\n");
    // while (1)
    // {
      // // receive message
      // int sVal = recvfrom(listenSOKFD[0], rcvBuffer, MAX_LEN, 0,
      //                 (S_SADDR *)&ClAddr[0], &clAddrLen[0]);
      // rcvBuffer[sVal] = '\0';
      // puts(rcvBuffer);
      // printf("\n[-]Confirming receive values...\n");
      // printf("\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)));
      
      // puts("\n");
      // strcpy(rplyBuffer, rcvBuffer);         
      // // send the response
      // sendto(listenSOKFD[0], rplyBuffer, MAX_LEN, 0,
      //       (struct sockaddr*)&ClAddr[0], sizeof(ClAddr));
            
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
    // #ifdef THREAD_TEST
     
      SOKData sokData0;
      // sokData0 = (SOKData *)malloc(sizeof(SOKData));
      // sokData0 = (SOKData *)sokData0;
      SOKData sokData1;
      
      sokData0.SOKid = 10;
      sokData1.SOKid = 20;
      
      // sokData0->cIP = malloc(sizeof(uint8_t) * IP_STR_SZ);
      // sokData0->cIP = szRem_Srv_IP[eREM_SRV_IP_0];
      // sokData0->uPort = REM_SRV_PORT_0;
      
      // sokData0.ipData->srvAddr.sin_family = AF_INET;
      
      // sokData0.ipData->srvAddr.sin_addr.s_addr =
      // *(uint8_t *)malloc(sizeof(uint8_t) * IP_STR_SZ);
      
      // This portion right here is causing the seg-fault...
      // sokData0.ipData->srvAddr.sin_addr.s_addr =
      // inet_addr(szRem_Srv_IP[eREM_SRV_IP_0]);
      // inet_addr("127.0.0.1");
      // End of first seg-fault section
      
      // sokData0.ipData->IPDataID = 127;
      
      // sokData0.ipData->srvAddr.sin_port = htons(REM_SRV_PORT_0);
      // sokData0.ipData->clAddr = {0};
      
      // Initi for-loop here?
    
      sokData0.cIP = malloc(sizeof(uint8_t) * IP_STR_SZ);
      sokData0.cIP = szRem_Srv_IP[eREM_SRV_IP_0];
      sokData0.uPort = REM_SRV_PORT_0;
      
      sokData1.cIP = malloc(sizeof(uint8_t) * IP_STR_SZ);
      sokData1.cIP = szRem_Srv_IP[eREM_SRV_IP_0];
      sokData1.uPort = REM_SRV_PORT_1;
      
      // sokData1.ipData->srvAddr.sin_family = AF_INET;
      
      // sokData1.ipData->srvAddr.sin_addr.s_addr =
      // *(uint8_t *)malloc(sizeof(uint8_t) * IP_STR_SZ);
      
      // sokData1.ipData->srvAddr.sin_addr.s_addr =
      // inet_addr(szRem_Srv_IP[eREM_SRV_IP_1]);
      
      // sokData1.ipData->srvAddr.sin_port = htons(REM_SRV_PORT_1);
      
      pthread_t SOKthread1;
      pthread_t SOKthread2;
      
      pthread_create(&SOKthread1, NULL, UDP_SrvConnection_Hndlr, (void *)&sokData0);
      // SLEEP
      pthread_create(&SOKthread2, NULL, UDP_SrvConnection_Hndlr, (void *)&sokData1);
      // SLEEP
      printf("\nIn Main: SOKid(0) = %d\n", sokData0.SOKid);
      printf("\nIn Main: SOKid(1) = %d\n", sokData1.SOKid);
      pthread_join(SOKthread1, NULL);
      // SLEEP
      pthread_join(SOKthread2, NULL);
      // printf("\nIn Main: SOKid(0) = %d\n", sokData0.SOKid);
      // printf("\nIn Main: SOKid(1) = %d\n", sokData1.SOKid);
      
      // pthread_t thread1;
      // pthread_t thread2;
      // int *pCl = (int *)malloc(sizeof(int));
      // int *pCl1 = (int *)malloc(sizeof(int));
      // *pCl = 10;
      // *pCl1 = 20;
      // pthread_create(&thread1, NULL, UDP_SrvConnection_Hndlr, pCl);
      // pthread_create(&thread2, NULL, UDP_SrvConnection_Hndlr, pCl1);
      // // SLEEP
      // pthread_join(thread1, NULL);
      // // SLEEP
      // pthread_join(thread2, NULL);
    
    // #else // Real Multi-threaded code
      
      
    
    // #endif // THREAD_TEST
      
    // } // End of while-loop
    
  // #else 
    // Non-Multi-threaded code
    // clAddrLen = sizeof(ClAddr);
    // printf("[-]WAITING FOR INCOMING CONNECTIONS...\n\n");
    // // While-Loop to receive data from incomming connections
    // while (1)
  // {
  //   // receive message
  //   uint16_t sVal = recvfrom(listenSOKFD, rcvBuffer, MAX_LEN, 0,
  //                   (S_SADDR *)&ClAddr, &clAddrLen);
  //   rcvBuffer[sVal] = '\0';
    
  // #ifdef DBG
  //   // Display Receive Buffer
  //   puts("[+]DEBUG STATUS: ENABLED\n");
  //   puts("[+]Displaying Recieve Buffer:\n");
  //   puts(rcvBuffer);
  //   // Validate
  //   printf("\n[-]Confirming receive values...\n");
  //   printf("\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)));
    
  //   puts("\n");
  //   printf("[-]Sending Response to Client...\n");
    
  // #endif
  //   // Copying to reply buffer for sending
  //   strcpy(rplyBuffer, rcvBuffer);
  //   // Replying buffer w/active notifier
  //   if (sendto(listenSOKFD, rplyBuffer, MAX_LEN, 0,
  //         (S_SADDR *)&ClAddr, sizeof(ClAddr)))
  //   {
    
  //   #ifndef DBG  
      
  //     puts("[-]DEBUG STATUS: DISABLED\n");
    
  //   #endif
    
  //     puts("[+]Replying Back to CLient: Active\n");
  //   }
          
  // #ifdef DBG
  
  //   printf("[-]CHECKSUM Validation...\n");
  //   // Checksum Validation (for debugging)
  //   if (bCheckSum(rcvBuffer, cSerialData, sizeof(cSerialData)))
  //   {
  //     printf("[+]BYTES RECEIVED = %d\n",
  //           (strlen(rcvBuffer))/(sizeof(uint8_t)) + 1);
            
  //     printf("[+]CHECKSUM = PASS\n");
  //   }
  //   else
  //   {
  //     printf("[+]CHECKSUM = FAIL\n");
  //   }
  //   puts("\n");
  
  // #endif  
  //   // Zero-out receive buffer
  //   memset(rcvBuffer, '\0', MAX_LEN);
      // UDP_SrvConnection_Hndlr(&sokData0);
  // }
  
  // puts("[+]Replying Back to CLient: INACTIVE (Connection Closed)\n");
  
  return(0);

}

/****************************************************************************/


/****************************************************************************/
// End ServerMain.c
