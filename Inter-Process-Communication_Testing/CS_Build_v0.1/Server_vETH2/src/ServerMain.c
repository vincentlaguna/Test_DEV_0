/*****************************************************************************
Author(s) or Contributor(s): Version 0.1  ~Vincent A. Laguna 2021                                                                 

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
  // Receive and Reply Buffers
  uint8_t *rcvBuffer  = NULL;
  uint8_t *rplyBuffer = NULL;
  rcvBuffer   = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  rplyBuffer  = (uint8_t *)malloc(sizeof(uint8_t) * MAX_LEN);
  // Initialize Local Variables
  socklen_t clAddrLen;
  int16_t listenSOKFD;
  S_SADDR_IN SrvAddr, ClAddr;  
  // Initialize Socket
  if ((listenSOKFD = UDP_SokInit_Handlr()) < 0) 
  {
    printf("[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  // Zero-out struct
  memset(&SrvAddr, 0, sizeof(SrvAddr));
  SrvAddr.sin_family = AF_INET;
  SrvAddr.sin_addr.s_addr = inet_addr(REM_SRV_IP_1);
  SrvAddr.sin_port = htons(REM_SRV_PORT_1);
  // Bind Server address to socket descriptor
  printf("[+]Binding to IP: %s on PORT: %d...\n", REM_SRV_IP_1, REM_SRV_PORT_1);
  if ((bind(listenSOKFD, (S_SADDR *)&SrvAddr, sizeof(SrvAddr))) < 0)
  {
    perror("[-]BIND = FAIL\n"); // Print the error message
  }
  else
  {
    printf("[+]Bind = OK\n");
  }
  //Receive the datagram
  clAddrLen = sizeof(ClAddr);
  printf("[-]WAITING FOR INCOMING CONNECTIONS...\n\n");
  // While-Loop to receive data from incomming connections
  while (1)
  {
    // receive message
    uint16_t sVal = recvfrom(listenSOKFD, rcvBuffer, MAX_LEN, 0,
                    (S_SADDR *)&ClAddr, &clAddrLen);
    rcvBuffer[sVal] = '\0';
    // Display Receive Buffer
    // puts("[+]Displaying Recieve Buffer:\n");
    puts(rcvBuffer);
    // Validate
    // printf("\n[-]Confirming receive values...\n");
    printf("\n%s", convertHex(rcvBuffer, strlen(rcvBuffer)));
    
    // puts("\n");
    strcpy(rplyBuffer, rcvBuffer);         
    // send the response
    // printf("[-]Sending Response to Client...\n");
    sendto(listenSOKFD, rplyBuffer, MAX_LEN, 0,
          (S_SADDR *)&ClAddr, sizeof(ClAddr));
    // Checksum Validation (for debugging)
    // if (bCheckSum(rcvBuffer, cSerialData, sizeof(cSerialData)))
    // {
    //   printf("[+]BYTES RECEIVED = %d\n", (strlen(rcvBuffer))/(sizeof(uint8_t)));
    //   printf("[+]CHECKSUM = PASS\n");
    // }
    // else
    // {
    //   printf("[+]CHECKSUM = FAIL\n");
    // }
    // puts("\n");
    // Zero-out receive buffer
    memset(rcvBuffer, '\0', MAX_LEN);
  }
  
  return(0);

}

/****************************************************************************/


/****************************************************************************/
// End ServerMain.c
