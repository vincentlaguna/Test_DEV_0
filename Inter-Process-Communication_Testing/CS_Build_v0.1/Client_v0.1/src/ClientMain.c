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

/* Typedefs: ****************************************************************/
/****************************************************************************/

/* Main Starts Here: ********************************************************/
//
int main(int argc, char *argv[])
{
  // Initialize Local Variables
  int sokFD; 
  // int r;
  // int sndBytes;
  char *IPbuffer;
  char buffer[MAX_LEN+1];
  // char *buffer;
  // buffer = (char *)malloc(MAX_LEN * sizeof(char));
  // uint8_t sndLine[MAX_LEN+1];
  char rcvLine[MAX_LEN+1];
  // char rcvLine;
  // rcvLine = (char *)malloc(MAX_LEN * sizeof(char));
  
  S_SADDR_IN  SrvAddr;
  
  TIME_V     Tv;
  Tv.tv_sec  = TIME_O; // Time-Out in Seconds
  Tv.tv_usec = 0;
  // Winsock
  #ifndef LIN
    
    WSADATA wsaData;
    
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
      fprintf(stderr,"[-]WSAStartup() = FAIL\n");
      exit(1);
    }
    
  #endif
  // Error Handling
  // if (argc != 2)
  // {
  //   printf("Usage: %s <SERVER ADDRESS>", argv[0]);
  //   return EXIT_FAILURE;
  // }
  printf("\n[-]CLIENT-Side Socket Initialization = in progress...\n");
  if ((sokFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  SLEEP
  printf("[+]CLIENT-Side Socket Initialization = OK\n");
  SLEEP
  // Setup Server struct Info
  bzero(&SrvAddr, sizeof(SrvAddr)); // Zero-out struct values
  SrvAddr.sin_family = AF_INET;
  SrvAddr.sin_port   = htons(REM_SRV_PORT);
  // Get remote server address
  if (inet_pton(AF_INET, LOCAL_IP, &SrvAddr.sin_addr) <= 0)
  {
    printf("[-]ERROR on remote address: %s\n", LOCAL_IP);
    return EXIT_FAILURE;
  }
  // Connect to server
  printf("[+]CONNECTION to Remote Server = in progress...\n");
  SLEEP
  if (connect(sokFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr)) < 0)
  {
    printf("[-]CONNECT to remote address: %s = FAIL\n", LOCAL_IP);
    return EXIT_FAILURE;
  }
  IPbuffer = inet_ntoa(SrvAddr.sin_addr);
  
  printf("[+]CONNECTION to Remote Server = OK\n");
  SLEEP
  printf("[+]CONNECTED to remote address: %s\n", IPbuffer);
  // Connected to server -> prepare the message to send
  // sprintf(sndLine, "This is the test string from the client");
  // sndBytes = strlen(sndLine);
  bzero(buffer, MAX_LEN);
  // strcpy(buffer, "Test string from client\n");
  strcpy(buffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 26
                 "abcdefghijklmnopqrstuvwxyz"							// 52
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 78
                 "abcdefghijklmnopqrstuvwxyz"							// 104
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 130
                 "abcdefghijklmnopqrstuvwxyz"							// 156
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 182
              	 "abcdefghijklmnopqrstuvwxyz"							// 208
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 234
                // "abcdefghijklmnopqrstuvwxyz"							// 260
          	     "\n");
  // strcpy(buffer, 
  //       // "\xff"													// NUM bytes
	 //      //"\x02"													// STX
  //     	 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 26
  //     	 "abcdefghijklmnopqrstuvwxyz"							// 52
  //     	 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 78
  //     	 "abcdefghijklmnopqrstuvwxyz"							// 104
  //     	 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 130
  //     	 "abcdefghijklmnopqrstuvwxyz"							// 156
  //     	 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 182
  //     	 "abcdefghijklmnopqrstuvwxyz"							// 208
  //     	 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"							// 234
  //     	 "abcdefghijklmnopqrstuvwxyz"							// 260
  //     	 //""
  //     	 "\n"); // \x03
  // Send data to the Remote Server
  SLEEP
  printf("[-]SENDING data in send buffer to server...\n");
  // if (write(sokFD, sndLine, sndBytes) != sndBytes)
  if (write(sokFD, buffer, strlen(buffer)) != strlen(buffer))
  {
    printf("[-]WRITE ERROR on socket file descriptor SEND = FAIL\n");
    return EXIT_FAILURE;
  }
  printf("[+]DATA sent to server = OK\n");
  bzero(rcvLine, MAX_LEN);
  // Output Server Response
  if (setsockopt(sokFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&Tv, sizeof(Tv)) < 0)
  {
    printf("[-]TIME OUT ERROR\n");
    return EXIT_FAILURE;
  }
  SLEEP
  // while ((r = read(sokFD, rcvLine, MAX_LEN-1)) > 0)
  // {
  //     fprintf(stdout, "\n%s\n\n%s", convertHex(rcvLine, r), rcvLine);
  //     // Look for end of message
  //     if (rcvLine[r-1] == '\n')
  //       break;
  //   }
  //   // Zero-out rcvLine
  //   // memset(rcvLine, 0, MAX_LEN);
  // // Check for errors and close the socket
  // if (r < 0)
  // {
  //   printf("\nREAD ERROR on socket file descriptor.\n");
  //   return EXIT_FAILURE;
  // }
  printf("[-]SERVER = RECEIVING DATA... %s\n", rcvLine);
  read(sokFD, rcvLine, MAX_LEN);
  SLEEP
  // rcvLine[strlen(rcvLine)-1] = '\0';
  while (rcvLine)
  {
    fprintf(stdout, "\n%s\n\n%s", convertHex(rcvLine, strlen(rcvLine)), rcvLine);
    // Look for end of message
    if (rcvLine[strlen(rcvLine)-1] == '\n')
    break;
  }
  SLEEP
  printf("\n[+]SERVER RESPONSE: %s\n", rcvLine);
  SLEEP
  printf("[+]DATA RECIEVED = OK\n\n");
 
  
  
  #ifndef LIN
    closesocket(sokFD);
    WSACleanup();
  #else
    close(sokFD);
  #endif
  
  return(0);

}

/****************************************************************************/

/****************************************************************************/
// End ClientMain.c
