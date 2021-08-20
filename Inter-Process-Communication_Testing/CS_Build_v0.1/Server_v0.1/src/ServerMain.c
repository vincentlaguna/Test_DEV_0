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
  int listFD, connFD, n;
  S_SADDR_IN  SrvAddr; 
  uint8_t buffer[MAX_LEN+1];
  uint8_t rcvLine[MAX_LEN+1];
  uint8_t replyLine[MAX_LEN+1];
  // Winsock
  #ifndef   LIN
    
    WSADATA wsaData;
    
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
      fprintf(stderr,"WSAStartup() failed");
      exit(1);
    }
    
  #endif
  // Create Socket File Descriptor to listen on (Server)
  if ((listFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\nCreation of SOCKET Failed.\n");
    return EXIT_FAILURE;
  }
  // Setup Server Info
  bzero(&SrvAddr, sizeof(SrvAddr)); // Zero-out struct values
  SrvAddr.sin_family      = AF_INET;
  SrvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  SrvAddr.sin_port        = htons(REM_SRV_PORT);
  // Bind Call
  if ((bind(listFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
  {
    perror("BIND Failed."); // Print the error message
  }
  printf("\n<<< BIND Done >>>\n\n");
  // Listen Call
  if ((listen(listFD, MAX_CONN)) < 0)
  {
    perror("LISTEN Failed.");
  }
  printf("\n>>> Listening on PORT %d\n\n", REM_SRV_PORT);
  while(1)
  {
    // Initialize struct and variable for client info
    S_SADDR_IN  ClAddr;
    socklen_t   clAddrLen;
    // Accept will block until incoming connection arrives,
    // returning Socket File Descriptor to the connection
    printf("\n\n>>> Waiting for incoming connections...\n");
    fflush(stdout);
    // Accept Call
    connFD = accept(listFD, (S_SADDR *)&ClAddr, (socklen_t *)&ClAddr);
    if (connFD < 0)
    {
      perror("ACCEPT Failed.");
    }
    printf("\nConnection ACCEPTED\n\n");
    // Zero-out the receive buffer and null terminate it
    memset(rcvLine, 0, MAX_LEN);
    // Reading the client's message
    while ((n = read(connFD, rcvLine, MAX_LEN-1)) > 0)
    {
      fprintf(stdout, "\n%s\n\n%s\n", convertHex(rcvLine, n), rcvLine);
      // Look for end of message
      if (rcvLine[n-1] == '\n')
        break;
    }
    strncpy((char*)buffer, rcvLine, strlen((char *)buffer));
    printf("\nbuffer: %s\n", rcvLine);
    // Reply the message back to the client
    // snprintf((char *)buffer, sizeof(buffer), 
    //         "HTTP/1.0 200 OK\r\n\r\n SERVER REPLY SUCCESS");
    // Write to socket and close
    write(connFD, (char *)buffer, strlen((char *)buffer));
    close(connFD);
    // Zero-out rcvLine
    // memset(rcvLine, 0, MAX_LEN);
  }
  if (n < 0)
  {
    perror("READ Failed.");
  }
  // Copy received data into buffer
  strncpy((char*)buffer, rcvLine, strlen((char *)buffer));
  // Reply the message back to the client
  // snprintf((char *)buffer, sizeof(buffer), 
  //         "HTTP/1.0 200 OK\r\n\r\n SERVER REPLY SUCCESS");
  // Write to socket and close
  write(connFD, (char *)buffer, strlen((char *)buffer));
  close(connFD);
           
  return(0);

}

/****************************************************************************/

/****************************************************************************/
// End ServerMain.c
