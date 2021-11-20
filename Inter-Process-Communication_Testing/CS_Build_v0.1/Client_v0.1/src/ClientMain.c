/*****************************************************************************
Author(s) or Contributor(s): Version 0.1  ~<NAME> 2021                                                                  

File: ClientMain.c                                                  

Description: Client-side Main                                              

*****************************************************************************/


/* Includes: ****************************************************************/

#include  "../inc/CS_Common.h"

/****************************************************************************/

/* Defines: *****************************************************************/
/****************************************************************************/

/* Main Starts Here: ********************************************************/

int main(int argc, char *argv[])
{
  // Receive and Reply Buffers
  uint8_t *sndBuffer = NULL;
  uint8_t *rcvBuffer = NULL;
  sndBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  rcvBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  
#ifdef USE_TCP
  // Initialize Local Variables
  int connectSOKFD; 
  int optVal;
  socklen_t optLen = sizeof(optVal);
  uint8_t *IPbuffer;
  // For server data
  S_SADDR_IN  SrvAddr;
  // Time-out values for socket options
  // TIME_V       Tv;
  // Tv.tv_sec  = TIME_O; // Time-Out in Seconds
  // Tv.tv_usec = 0;
  // Winsock
  #ifndef LIN
    
    WSADATA wsaData;
    
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
      fprintf(stderr,"[-]WSAStartup() = FAIL\n");
      exit(1);
    }
    
  #endif
 
  printf("\n[-]CLIENT-Side Socket Initialization = in progress...\n");
  if ((connectSOKFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) // TCP
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  // SLEEP
  printf("[+]CLIENT-Side Socket Initialization = OK\n");
  // SLEEP
  // Setup Server struct Info
  memset(&SrvAddr, 0, sizeof(SrvAddr)); // Zero-out struct values
  SrvAddr.sin_family = AF_INET;
  SrvAddr.sin_port   = htons(REM_SRV_PORT);
  // Get remote server address
  if (inet_pton(AF_INET, REM_SRV_IP, &SrvAddr.sin_addr) <= 0)
  {
    printf("[-]ERROR on remote address: %s\n", REM_SRV_IP);
    return EXIT_FAILURE;
  }
  // Connect to server
  printf("[+]CONNECTION to Remote Server = in progress...\n");
  // SLEEP
  if (connect(connectSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr)) < 0)
  {
    printf("[-]CONNECT to remote address: %s = FAIL\n", REM_SRV_IP);
    return EXIT_FAILURE;
  }
  IPbuffer = inet_ntoa(SrvAddr.sin_addr);
  
  printf("[+]CONNECTION to Remote Server = OK\n");
  // SLEEP
  printf("[+]CONNECTED to remote address: %s\n", IPbuffer);
  // Connected to server -> prepare the message to send
  memset(sndBuffer, '\0', MAX_LEN);
  strcpy(sndBuffer, cStringPayload);
  // Send data to the Remote Server
  // SLEEP
  printf("[-]SENDING data in send sndBuffer to server...\n");
  // if (write(connectSOKFD, sndLine, sndBytes) != sndBytes)
  if (write(connectSOKFD, sndBuffer, strlen(sndBuffer)) != strlen(sndBuffer))
  {
    printf("[-]WRITE ERROR on socket file descriptor SEND = FAIL\n");
    return EXIT_FAILURE;
  }
  printf("[+]DATA sent to server = OK\n");
  // Zero-out buffer
  memset(rcvBuffer, '\0', MAX_LEN);
  optVal = 1;
  // Output Server Response
  // if (setsockopt(connectSOKFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&Tv, sizeof(Tv)) < 0)
  if (setsockopt(connectSOKFD, SOL_SOCKET, SO_KEEPALIVE, (char *)&optVal, optLen) < 0)
  {
    printf("[-]TIME OUT ERROR\n");
    return EXIT_FAILURE;
  }
  // SLEEP
  printf("[-]SERVER = RECEIVING DATA... %s\n", rcvBuffer);
  read(connectSOKFD, rcvBuffer, MAX_LEN-1);
  // SLEEP
  
  while (rcvBuffer)
  {
    fprintf(stdout, "\n%s\n\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)), rcvBuffer);
    // Look for end of message
    if (rcvBuffer[strlen(rcvBuffer)-1] == ('\n') || ('\0'))
    break;
  }
  
  // SLEEP
  printf("\n[+]SERVER RESPONSE: %s\n", rcvBuffer);
  // SLEEP
  printf("\n[+]DATA RECEIVED = OK\n");
  printf("[+]BYTES RECEIVED = %d\n", sizeof(rcvBuffer));
  printf("[+]LENGTH RECEIVED = %d\n", strlen(rcvBuffer));
  
  if (bCheckSum(sndBuffer, rcvBuffer, sizeof(rcvBuffer)))
  {
    printf("[+]CHECKSUM = PASS\n");
  }
  else
  {
    printf("[+]CHECKSUM = FAIL\n");
  }
  putchar('\n');
  // Zero-out receive buffer
  memset(rcvBuffer, '\0', MAX_LEN);
  // printf("[+]SIZE OF CHAR = %d, INT = %d, UINT8_T = %d\n\n", 
        // sizeof(char), sizeof(int), sizeof(uint8_t)); // For debug
  
  #ifndef LIN
    closesocket(connectSOKFD);
    WSACleanup();
  #else
    close(connectSOKFD);
  #endif

#else // UDP
  // Local Variables
  uint16_t connectSOKFD;
  S_SADDR_IN SrvAddr;
  // Create datagram socket
  // connectSOKFD = socket(AF_INET, SOCK_DGRAM, 0);
  if ((connectSOKFD = UDP_SokInit_Handlr()) < 0) 
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  // Clear SrvAddr
  memset(&SrvAddr, 0, sizeof(SrvAddr));
  // SrvAddr.sin_addr.s_addr = inet_addr(LOCAL_IP);
  SrvAddr.sin_addr.s_addr = inet_addr(REM_SRV_IP_0);
  SrvAddr.sin_port = htons(REM_SRV_PORT_0);
  SrvAddr.sin_family = AF_INET;
  memset(sndBuffer, '\0', MAX_LEN);
  strcpy(sndBuffer, cSerialData);    
  // strcpy(sndBuffer, cStringPayload);    
  // Connect to server
  printf("[-]CONNECTING TO SERVER: IP %s PORT %d\n", REM_SRV_IP_0, REM_SRV_PORT_0);
  if(connect(connectSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr)) < 0)
  {
      printf("\n[-]CONNECT TO SERVER Error: Connect Failed \n");
      exit(0);
  }
  printf("[+]CONNECTION TO SERVER: OK\n\n");
  // Request to send datagram
  // No need to specify server address in sendto
  // Connect stores the peers IP and port
  while (1)
  {
    sendto(connectSOKFD, sndBuffer, MAX_LEN, 0, (S_SADDR *)NULL, sizeof(SrvAddr));
    // Waiting for response
    uint16_t sVal = recvfrom(connectSOKFD, rcvBuffer, MAX_LEN, 0, (S_SADDR *)NULL, NULL);
    // sVal = recvfrom(listenSOKFD, rcvBuffer, MAX_LEN, 0,
                        // (S_SADDR *)&ClAddr, &clAddrLen);
    rcvBuffer[sVal] = '\0';
        
    puts("[+]Displaying Recieve Buffer:\n");
    puts(rcvBuffer);
        
    printf("\n[-]Confirming receive values...\n");
    printf("\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)));
    puts(rcvBuffer);
    // SLEEP
    printf("\n[+]SERVER RESPONSE: %s\n", rcvBuffer);
    // SLEEP
    printf("\n[-]Confirming receive values...\n");
    printf("\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)));
  }
  
    if (rcvBuffer != NULL)
    {
      printf("\n\n[+]DATA RECEIVED = OK\n");
    }
    else
    {
      printf("\n\n[-]DATA RECEIVED = FAIL\n");
    }
    printf("[+]BYTES RECEIVED = %d\n", (strlen(rcvBuffer))/(sizeof(uint8_t)));
    // printf("[+]LENGTH RECEIVED = %d\n", strlen(rcvBuffer));
    if (bCheckSum(sndBuffer, rcvBuffer, sizeof(rcvBuffer)))
    {
      printf("[+]CHECKSUM = PASS\n");
    }
    else
    {
      printf("[+]CHECKSUM = FAIL\n");
    }
    putchar('\n');
    // Zero-out receive buffer
    memset(rcvBuffer, '\0', MAX_LEN);
    // Close the descriptor
    printf("\n[-]CLOSING CONNECTION TO SERVER: IP %s PORT %d\n\n", REM_SRV_IP_0, REM_SRV_PORT_0);
    close(connectSOKFD);
  
#endif // TCP/UDP  

  return(0);
  
}

/****************************************************************************/


/****************************************************************************/
// End ClientMain.c