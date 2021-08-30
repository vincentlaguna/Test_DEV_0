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

/* Globals: *****************************************************************/

static const uint8_t *cStringPayload = 
{
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 26
	"abcdefghijklmnopqrstuvwxyz"							// 52
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 78
	"abcdefghijklmnopqrstuvwxyz"							// 104
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 130
	"abcdefghijklmnopqrstuvwxyz"							// 156
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 182
	"abcdefghijklmnopqrstuvwxyz"							// 208
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 234
	"abcdefghijklmnopqrst"							// 260
	"\n",
};

/****************************************************************************/

/* Main Starts Here: ********************************************************
//
int main(int argc, char *argv[])
{
  // Initialize Local Variables
  int connectSOKFD; 
  int optVal;
  socklen_t optLen = sizeof(optVal);
  uint8_t *IPbuffer;
  uint8_t sndBuffer[MAX_LEN];
  uint8_t rcvBuffer[MAX_LEN];
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
  // strcpy(sndBuffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"					    // 26
  //                   "abcdefghijklmnopqrstuvwxyz"							// 52
  //                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 78
  //                   "abcdefghijklmnopqrstuvwxyz"							// 104
  //                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 130
  //                   "abcdefghijklmnopqrstuvwxyz"							// 156
  //                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 182
  //                 	"abcdefghijklmnopqrstuvwxyz"							// 208
  //                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 234
  //                   // "abcdefghijklmnopqrstuvwxyz"							// 260
  //             	    "abcdefghijklmnopqrst"
  //             	    "\n");
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
  printf("[+]DATA RECEIVED = OK\n");
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
  
  return(0);

}

/****************************************************************************/


/* Main Starts Here: ********************************************************/
//
int main(int argc, char *argv[])
{
  uint8_t sndBuffer[MAX_LEN];
  uint8_t rcvBuffer[MAX_LEN];
  
  int sockfd, n;
  struct sockaddr_in servaddr;
      
  // clear servaddr
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(REM_SRV_PORT);
  servaddr.sin_family = AF_INET;
      
  // create datagram socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(sndBuffer, '\0', MAX_LEN);
  strcpy(sndBuffer, cStringPayload);    
  // connect to server
  if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
      printf("\n Error : Connect Failed \n");
      exit(0);
  }
  
  // request to send datagram
  // no need to specify server address in sendto
  // connect stores the peers IP and port
  sendto(sockfd, message, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
      
  // waiting for response
  recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
  puts(buffer);
  
  // close the descriptor
  close(sockfd);
  
  return(0);

}

/****************************************************************************/


/****************************************************************************/
// End ClientMain.c
