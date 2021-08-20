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
  int sokFD, r;
  int sndBytes;
  char *IPbuffer;
  
  S_SADDR_IN  SrvAddr;
  
  TIME_V     Tv;
  Tv.tv_sec  = TIME_O; // Time-Out in Seconds
  Tv.tv_usec = 0;
  
  uint8_t sndLine[MAX_LEN+1];
  uint8_t rcvLine[MAX_LEN+1];
  // Winsock
  #ifndef LIN
    
    WSADATA wsaData;
    
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
      fprintf(stderr,"WSAStartup() failed");
      exit(1);
    }
    
  #endif
  // Error Handling
  // if (argc != 2)
  // {
  //   printf("Usage: %s <SERVER ADDRESS>", argv[0]);
  //   return EXIT_FAILURE;
  // }
  if ((sokFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\nCreation of SOCKET Failed.\n");
    return EXIT_FAILURE;
  }
  printf("\n>>> The SOCKET has been created >>>\n\n");
  // Setup Server struct Info
  bzero(&SrvAddr, sizeof(SrvAddr)); // Zero-out struct values
  SrvAddr.sin_family      = AF_INET;
  SrvAddr.sin_port        = htons(REM_SRV_PORT);
  // Get remote server address
  if (inet_pton(AF_INET, LOCAL_IP, &SrvAddr.sin_addr) <= 0)
  {
    printf("\nError for remote address: %s\n", LOCAL_IP);
    return EXIT_FAILURE;
  }
  // Connect to server
  if (connect(sokFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr)) < 0)
  {
    printf("\nError conecting to remote address: %s\n", argv[1]);
    return EXIT_FAILURE;
  }
  IPbuffer = inet_ntoa(SrvAddr.sin_addr);
  
  printf("Connection to Remote Server = SUCCESS\n\n");
  printf("Connected to remote address: %s\n", IPbuffer);
  // Connected to server -> prepare the message to send
  sprintf(sndLine, "This is the test string from the client");
  sndBytes = strlen(sndLine);
  // Send data to the Remote Server 
  if (write(sokFD, sndLine, sndBytes) != sndBytes)
  {
    printf("\nWRITE ERROR on socket file descriptor.\n");
    return EXIT_FAILURE;
  }
  memset(rcvLine, 0, MAX_LEN);
  // Output Server Response
  if (setsockopt(sokFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&Tv, sizeof(Tv)) < 0)
  {
    printf("\nTIME OUT.\n");
    return EXIT_FAILURE;
  }
  while ((r = read(sokFD, rcvLine, MAX_LEN-1)) > 0)
  {
      fprintf(stdout, "\n%s\n\n%s", convertHex(rcvLine, r), rcvLine);
      // Look for end of message
      if (rcvLine[r-1] == '\n')
        break;
    }
    // Zero-out rcvLine
    // memset(rcvLine, 0, MAX_LEN);
  // Check for errors and close the socket
  if (r < 0)
  {
    printf("\nREAD ERROR on socket file descriptor.\n");
    return EXIT_FAILURE;
  }
  
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
