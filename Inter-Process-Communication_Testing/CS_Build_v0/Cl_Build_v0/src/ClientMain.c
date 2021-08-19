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
  
  uint16_t  uClSok  = 0; 
  uint16_t  sok     = 0; 
  uint16_t  clLen   = 0;
  // This is where we fill-in the Server-Side address info
  S_SADDR_IN  Srv; 

  DBffr  ClDbuff;
  DBffr  SrvRspDbuff;
  // Create Socket
  uClSok = SokInit_Handlr();
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
  if (uClSok == -1)
  {
    printf("\nCreation of SOCKET Failed.\n");
    return EXIT_FAILURE;
  }
  
  SLEEP
  
  printf("\n>>> The SOCKET has been created >>>\n\n");
  // Bind
  // if (SokConnect_Hndlr(uClSok, SrvAddr, SrvPort) < 0)
  if (SokConnect_Hndlr(uClSok, LOCAL_IP, REM_SRV_PORT) < 0)
  {
    perror("CONNECT Failed."); // Print the error message
    return EXIT_FAILURE;
  }
  
  SLEEP
  
  printf("Connection to Remote Server = SUCCESS\n\n");
  
  uint32_t  DbuffSize = sizeof(DBffr);
  printf("Please Enter Message to Send: ");
  fgets(ClDbuff.cPayload, DbuffSize, stdin);
  
  SLEEP
  // Send data to the Remote Server 
  SokSend_Hndlr(uClSok, ClDbuff.cPayload, DbuffSize);
  // Received the data from the Remote Server
  if (SokRcv_Hndlr(uClSok, SrvRspDbuff.cPayload, DbuffSize))
  // Output Server Response
    printf("Server Status: Payload (%d Bytes) RECIEVED->SENT = SUCCESS\n\n", 
            strlen(SrvRspDbuff.cPayload));
  // Close the Client Socket
  #ifndef LIN
    closesocket(uClSok);
    WSACleanup();
  #else
    close(uClSok);
  #endif
  
  return(0);

}

/****************************************************************************/

/****************************************************************************/
// End ClientMain.c
