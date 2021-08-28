/*****************************************************************************
Author(s) or Contributor(s): Version 0.1  ~<NAME> 2021                                                                 

File: ServerMain.c              

Description: Server-side Main                                              

*****************************************************************************/


/* Includes: ****************************************************************/

#include  "../inc/CS_Common.h"

/****************************************************************************/

/* Main Starts Here: ********************************************************/
//
int main(int argc, char *argv[])
{
  // Initialize Local Variables
  int listenSOKFD, connectSOKFD; 
  int sVal;
  S_SADDR_IN  SrvAddr; 
  
  uint8_t rcvBuffer[MAX_LEN+1];
  uint8_t rplyBuffer[MAX_LEN+1];
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
  if ((listenSOKFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  // SLEEP
  printf("[+]SERVER-Side Socket Initialization = OK\n");
  // SLEEP
  // Setup Server Info
  memset(&SrvAddr, 0, sizeof(SrvAddr)); // Zero-out struct values
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
    S_SADDR_IN  ClAddr;
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
    memset(rcvBuffer, 0, MAX_LEN);
    memset(rplyBuffer, 0, MAX_LEN);
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
    memset(rcvBuffer, 0, MAX_LEN);
    memset(rplyBuffer, 0, MAX_LEN);
  }
  
  return(0);

}

/****************************************************************************/

/****************************************************************************/
// End ServerMain.c
