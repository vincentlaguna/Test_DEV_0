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
  int listFD, connFD; 
  int n;
  S_SADDR_IN  SrvAddr; 
  // char buffer[MAX_LEN+1];
  uint8_t buffer[MAX_LEN+1];
  // uint8_t rcvLine[MAX_LEN+1];
  uint8_t replyLine[MAX_LEN+1];
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
  SLEEP
  printf("\n[-]SERVER-Side Socket Initialization = in progress...\n");
  if ((listFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  SLEEP
  printf("[+]SERVER-Side Socket Initialization = OK\n");
  SLEEP
  // Setup Server Info
  memset(&SrvAddr, 0, sizeof(SrvAddr)); // Zero-out struct values
  SrvAddr.sin_family      = AF_INET;
  SrvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  SrvAddr.sin_port        = htons(REM_SRV_PORT);
  // Bind Call
  SLEEP
  printf("[+]Binding to PORT: %d...\n", REM_SRV_PORT);
  if ((bind(listFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
  {
    perror("[-]BIND = FAIL\n"); // Print the error message
  }
  SLEEP
  printf("[+]Bind = OK\n");
  // Listen Call
  SLEEP
  if ((listen(listFD, MAX_CONN)) < 0)
  {
    perror("[-]LISTEN = FAIL\n");
  }
  printf("[+]# MAX CONNECTIONS = %d\n", MAX_CONN);
  SLEEP
  printf("[+]LISTEN = OK\n");
  SLEEP
  printf("[+]LISTENING ON PORT = %d\n", REM_SRV_PORT);
  SLEEP
  printf("[-]Waiting for incoming connections...\n\n");
  SLEEP
  // While-loop for server to listen on binded port for incoming connections
  while(1)
  {
    // Initialize struct and variable for client info
    S_SADDR_IN  ClAddr;
    socklen_t   clAddrLen;
    // Accept will block until incoming connection arrives,
    // returning Socket File Descriptor to the connection
    fflush(stdout);
    // Accept Call
    connFD = accept(listFD, (S_SADDR *)&ClAddr, (socklen_t *)&ClAddr);
    if (connFD < 0)
    {
      perror("[-]INCOMING CONNECTION ACCEPT = FAIL\n");
    }
    printf("[+]INCOMING CONNECTION ACCEPT = OK\n\n");
    SLEEP
    // Zero-out the receive buffer and null terminate it
    // memset(rcvLine, 0, MAX_LEN);
    memset(buffer, 0, MAX_LEN);
    memset(replyLine, 0, MAX_LEN);
    // Reading the client's message
    // read(connFD, buffer, MAX_LEN);
    // while ((n = read(connFD, rcvLine, MAX_LEN-1)) > 0)
    // {
    //   fprintf(stdout, "\n%s\n\n%s\n", convertHex(rcvLine, n), rcvLine);
    //   // Look for end of message
    //   if (rcvLine[n-1] == '\n')
    //     break;
    // }
    printf("[-]SERVER = RECEIVING DATA...\n");
    while ((n = read(connFD, buffer, MAX_LEN-1)) > 0)
    {
      fprintf(stdout, "%s\n\n%s\n", convertHex(buffer, n), buffer);
      // Look for end of message
      if (buffer[n-1] == '\n' || '\0')
        break;
    }
    // strncpy((char*)buffer, rcvLine, strlen((char *)buffer));
    // printf("\nbuffer: %s\n", rcvLine);
    printf("\nConfirm Buffer: %s\n", buffer);
    printf("[+]DATA RECIEVED = OK\n\n");
    strcpy(replyLine, buffer);
    // replyLine[strlen(replyLine)] = '\0';
    // Reply the message back to the client
    // snprintf((char *)buffer, sizeof(buffer), 
    //         "HTTP/1.0 200 OK\r\n\r\n SERVER REPLY SUCCESS");
    // Write to socket and close
    // write(connFD, (char *)buffer, strlen((char *)buffer));
    printf("[-]SERVER = Replied data back to client...\n\n");
    printf("[-]Waiting for incoming connections...\n\n");
    write(connFD, replyLine, strlen(replyLine));
    close(connFD);
    // Zero-out rcvLine
    // memset(rcvLine, 0, MAX_LEN);
  }
  // if (n < 0)
  // {
  //   perror("READ Failed.");
  // }
  // Copy received data into buffer
  // strncpy((char*)buffer, rcvLine, strlen((char *)buffer));
  // Reply the message back to the client
  // snprintf((char *)buffer, sizeof(buffer), 
  //         "HTTP/1.0 200 OK\r\n\r\n SERVER REPLY SUCCESS");
  // Write to socket and close
  // write(connFD, (char *)buffer, strlen((char *)buffer));
  // close(connFD);
           
  return(0);

}

/****************************************************************************/

/****************************************************************************/
// End ServerMain.c
