/*****************************************************************************
Author(s) or Contributor(s): Version 0.1  ~<NAME> 2021                                                                 

File: ServerMain.c              

Description: Server-side Main                                              

*****************************************************************************/


/* Includes: ****************************************************************/

#include  "../inc/CS_Common.h"

/****************************************************************************/

/* Main Starts Here: ********************************************************
//
int main(int argc, char *argv[])
{

  // Initialize Local Variables
  int listenSOKFD, connectSOKFD; 
  int sVal;
  S_SADDR_IN  SrvAddr; 
  S_SADDR_IN  ClAddr;
  
  uint8_t rcvBuffer[MAX_LEN];
  uint8_t rplyBuffer[MAX_LEN];
  // Winsock
  #ifndef   LIN
    
    WSADATA wsaData;
    
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
      fprintf(stderr,"[-]WSAStartup() = FAIL\n");
      exit(1);
    }
    
  #endif
  // Create Socket File Descriptor to listen on (Server)
  // SLEEP
  printf("\n[-]SERVER-Side Socket Initialization = in progress...\n");
  if ((listenSOKFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) // TCP
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  // SLEEP
  printf("[+]SERVER-Side Socket Initialization = OK\n");
  // SLEEP
  // Setup Server Info
  memset(&SrvAddr, 0, sizeof(SrvAddr)); // Zero-out struct values
  memset(&ClAddr, 0, sizeof(ClAddr)); // Zero-out struct values
  SrvAddr.sin_family      = AF_INET;
  SrvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  SrvAddr.sin_port        = htons(REM_SRV_PORT);
  // Bind Call
  // SLEEP
  printf("[+]Binding to PORT: %d...\n", REM_SRV_PORT);
  if ((bind(listenSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
  {
    perror("[-]BIND = FAIL\n"); // Print the error message
  }
  // SLEEP
  printf("[+]Bind = OK\n");
  // Listen Call
  // SLEEP
  if ((listen(listenSOKFD, MAX_CONN)) < 0)
  {
    printf("[-]LISTEN = FAIL\n");
  }
  printf("[+]# MAX CONNECTIONS = %d\n", MAX_CONN);
  // SLEEP
  printf("[+]LISTEN = OK\n");
  // SLEEP
  printf("[+]LISTENING ON PORT = %d\n", REM_SRV_PORT);
  // SLEEP
  printf("[-]Waiting for incoming connections...\n\n");
  // SLEEP
  // While-loop for server to listen on binded port for incoming connections
  while(1)
  {
    // Initialize struct and variable for client info
    // S_SADDR_IN  ClAddr;
    socklen_t   clAddrLen;
    // Accept will block until incoming connection arrives,
    // returning Socket File Descriptor to the connection
    // fflush(stdout);
    // Accept Call
    connectSOKFD = accept(listenSOKFD, (S_SADDR *)&ClAddr, (socklen_t *)&ClAddr);
    if (connectSOKFD < 0)
    {
      printf("[-]INCOMING CONNECTION ACCEPT = FAIL\n");
    }
    printf("[+]INCOMING CONNECTION ACCEPT = OK\n");
    // SLEEP
    // Zero-out buffers
    memset(rcvBuffer, '\0', MAX_LEN);
    memset(rplyBuffer, '\0', MAX_LEN);
    // Reading the client's message
    printf("[-]SERVER = RECEIVING DATA...\n\n");
    printf("[+]DATA RECEIVED: \n");
    
    while ((sVal = read(connectSOKFD, rcvBuffer, MAX_LEN-1)) > 0)
    {
      fprintf(stdout, "%s\n\n%s\n", convertHex(rcvBuffer, sVal), rcvBuffer);
      // Look for end of message
      if (rcvBuffer[strlen(rcvBuffer)-1] == ('\n') || ('\0'))
        break;
    }
    printf("\nConfirm receive buffer: %s\n", rcvBuffer);
    printf("[+]DATA RECIEVED = OK\n");
    printf("[+]BYTES RECIEVED = %d\n", sizeof(rcvBuffer));
    printf("[+]LENGTH RECEIVED = %d\n", strnlen(rcvBuffer, MAX_LEN));
    strcpy(rplyBuffer, rcvBuffer);
    printf("[-]SERVER = Replied data back to client...\n\n");
    printf("[-]Waiting for incoming connections...\n\n");
    write(connectSOKFD, rplyBuffer, strlen(rplyBuffer));
    close(connectSOKFD);
    // Zero-out buffers
    memset(rcvBuffer, '\0', MAX_LEN);
    memset(rplyBuffer, '\0', MAX_LEN);
  }
  
  return(0);

}

/****************************************************************************/


/* Main Starts Here: ********************************************************/
//
int main(int argc, char *argv[])
{
  // UDP_SrvConnection_Hndlr(REM_SRV_IP_0);
  // Receive and Reply Buffers
  // uint8_t rcvBuffer[MAX_LEN];
  // uint8_t rplyBuffer[MAX_LEN];
  uint8_t *rcvBuffer  = NULL;
  uint8_t *rplyBuffer = NULL;
  rcvBuffer   = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  rplyBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  // Local Variables
  // int listenSOKFD, clAddrLen;
  int16_t listenSOKFD;
  int sVal;
  int clAddrLen;
  // Local Structs
  S_SADDR_IN SrvAddr, ClAddr;
  // S_SADDR_IN ClAddr;
  // Create a UDP Socket
  // listenSOKFD = socket(AF_INET, SOCK_DGRAM, 0);        
  if ((listenSOKFD = UDP_SokInit_Handlr()) < 0) 
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  // Zero-out struct
  memset(&SrvAddr, 0, sizeof(SrvAddr));
  SrvAddr.sin_family = AF_INET;
  // SrvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  SrvAddr.sin_addr.s_addr = inet_addr(REM_SRV_IP_0);
  SrvAddr.sin_port = htons(REM_SRV_PORT_0);
  // bind server address to socket descriptor
  // printf("[+]Binding to IP: %s on PORT: %d\n", REM_SRV_IP_0, REM_SRV_PORT_0);
  // printf("[+]Binding to PORT: %d...\n", REM_SRV_PORT_0);
  printf("[+]Binding to IP: %s on PORT: %d...\n", REM_SRV_IP_0, REM_SRV_PORT_0);
  if ((bind(listenSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
  {
    perror("[-]BIND = FAIL\n"); // Print the error message
  }
  else
  {
    printf("[+]Bind = OK\n");
  }
  // SLEEP
  // if ((BindSrvSok_Hndlr(listenSOKFD, REM_SRV_IP_0)) < 0)
  // {
  //   printf("[-]BIND = FAIL\n"); // Print the error message
  // }
  //Receive the datagram
  clAddrLen = sizeof(ClAddr);
  // While-Loop to receive data from incomming connections
  printf("[-]WAITING FOR INCOMING CONNECTIONS...\n\n");
  while (1)
  {
    // receive message
    int n = recvfrom(listenSOKFD, rcvBuffer, MAX_LEN, 0,
                    (S_SADDR *)&ClAddr, &clAddrLen);
    rcvBuffer[n] = '\0';
    puts(rcvBuffer);
    printf("\n[-]Confirming receive values...\n");
    printf("\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)));
    
    puts("\n");
    strcpy(rplyBuffer, rcvBuffer);         
    // send the response
    sendto(listenSOKFD, rplyBuffer, MAX_LEN, 0,
          (struct sockaddr*)&ClAddr, sizeof(ClAddr));
    if (bCheckSum(rcvBuffer, cSerialData, sizeof(cSerialData)))
    {
      printf("[+]CHECKSUM = PASS\n");
    }
    else
    {
      printf("[+]CHECKSUM = FAIL\n");
    }
    puts("\n");
    // Zero-out receive buffer
    memset(rcvBuffer, '\0', MAX_LEN);
  }
  
  return(0);

}

/****************************************************************************/


/****************************************************************************/
// End ServerMain.c
