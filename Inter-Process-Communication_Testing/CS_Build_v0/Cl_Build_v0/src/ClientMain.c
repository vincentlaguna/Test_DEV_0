/*****************************************************************************
Author(s) or Contributor(s): Version .0  ~<NAME> 2021                                                                

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
  // char *SrvAddr = argv[1];
  // char *SrvAddr = REM_SRV_IP;
  // uint16_t  SrvPort = REM_SRV_PORT;
  // printf("\nConnecting to Server Address: %s\n", SrvAddr);
  
  int16_t  sClSOK; 
  int16_t  connSOK; 
  // This is where we fill-in the Server-Side address info
  // S_SADDR_IN  Srv; 
  // Client connection time-out (s)
  TIME_V       Tv;
  Tv.tv_sec  = TIME_O; // Time-Out in Seconds
  Tv.tv_usec = 5;
  // Initialize buffers to store the data (struct data)
  // DBffr  ClDbuff;
  // DBffr  SrvRspDbuff;
  // No struct
  uint8_t sndBuffer[MAX_LEN+1];
  uint8_t rcvBuffer[MAX_LEN+1];
  // Create Socket
  sClSOK = SOKInit_Handlr();
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
  if (sClSOK == -1)
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  
  SLEEP
  // if (SokConnect_Hndlr(uClSok, SrvAddr, SrvPort) < 0)
  if (SOKConnect_Hndlr(sClSOK, LOCAL_IP, REM_SRV_PORT) < 0)
  {
    perror("[-]CONNECT = FAIL\n"); // Print the error message
    return EXIT_FAILURE;
  }
  
  printf("[-]CONNECT = OK\n");
  
  SLEEP
  // Connected to server -> prepare the message to send
  
  // Using struct data
  // uint32_t  DbuffSize = sizeof(DBffr);
  // printf("Please Enter Message to Send: ");
  // fgets(ClDbuff.cPayload, DbuffSize, stdin);
  
  // SLEEP
  // // Send data to the Remote Server 
  // SokSend_Hndlr(uClSok, ClDbuff.cPayload, DbuffSize);
  // // Received the data from the Remote Server
  // if (SokRcv_Hndlr(uClSok, SrvRspDbuff.cPayload, DbuffSize))
  // // Output Server Response
  //   printf("Server Status: Payload (%d Bytes) RECIEVED->SENT = SUCCESS\n\n", 
  //           strlen(SrvRspDbuff.cPayload));
  
  // No struct data
  // Zero-out the send buffer
  bzero(sndBuffer, MAX_LEN);
  strcpy(sndBuffer, "Test string from client\n");
  // Send data to the Remote Server 
  printf("[-]SENDING data in send buffer to server...\n");
  // if (write(sokFD, sndLine, sndBytes) != sndBytes)
  if (write(connSOK, sndBuffer, strlen(sndBuffer)) != strlen(sndBuffer))
  {
    printf("[-]WRITE ERROR on socket file descriptor SEND = FAIL\n");
    return EXIT_FAILURE;
  }
  // Zero-out the receive buffer
  bzero(rcvBuffer, MAX_LEN);
  // Set socket options (i.e.: time-out values)
  if (setsockopt(connSOK, SOL_SOCKET, SO_RCVTIMEO, (char *)&Tv, sizeof(Tv)) < 0)
  {
    printf("[-]RESPONSE = TIME OUT\n");
    return EXIT_FAILURE;
  }
  // Read server data into receive buffer
  read(connSOK, rcvBuffer, MAX_LEN);
  // Output server response
  while (rcvBuffer)
  {
    fprintf(stdout, "\n%s\n\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)), rcvBuffer);
    // Look for end of message
    if (rcvBuffer[strlen(rcvBuffer)-1] == '\n')
    break;
  }
  printf("[+]SERVER RESPONSE: %s\n\n", rcvBuffer);
  // Close the Client Socket
  #ifndef LIN
    closesocket(sClSOK);
    WSACleanup();
  #else
    close(sClSOK);
  #endif
  
  return(0);

}

/****************************************************************************/

/****************************************************************************/
// End ClientMain.c
