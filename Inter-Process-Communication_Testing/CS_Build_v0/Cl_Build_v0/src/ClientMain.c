/*****************************************************************************
Author(s):                                                                 

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
  uint16_t  uClSok = 0; 
  uint16_t  sok    = 0; 
  uint16_t  clLen  = 0;
  // This is where we fill-in the Server-Side address info
  S_SADDR_IN  Srv; 
  // Initialize buffers to store the data
  // char        sndToSrv[100] = {"Client (n) Connected"};
  // char        srvReply[200] = {0};
  DataBuffer_t  ClDbuff;
  DataBuffer_t  SrvRspDbuff;
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
  if (SokConnect_Hndlr(uClSok) < 0)
  {
    perror("CONNECT Failed."); // Print the error message
    return EXIT_FAILURE;
  }
  
  SLEEP
  
  printf("Connection to Remote Server = Successful\n\n");
  
  // printf("Please Enter the Message: ");
  // fgets(sndToSrv, 100, stdin);
  
  // sleep(1);
  // // Send data to the Remote Server 
  // SokSend_Hndlr(uClSok, sndToSrv, strlen(sndToSrv));
  // // Received the data from the Remote Server
  // SokRcv_Hndlr(uClSok, srvReply, 200);
  // // Output Server Response
  // printf("\nServer Response: %s\n\n", srvReply);
  
  uint32_t  DbuffSize = sizeof(DataBuffer_t);
  printf("Please Enter Message to Send: ");
  fgets(ClDbuff.cPayload, DbuffSize, stdin);
  
  SLEEP
  // Send data to the Remote Server 
  SokSend_Hndlr(uClSok, ClDbuff.cPayload, DbuffSize);
  // Received the data from the Remote Server
  SokRcv_Hndlr(uClSok, SrvRspDbuff.cPayload, DbuffSize);
  // Output Server Response
  printf("\nServer Response: %s\n\n", SrvRspDbuff.cPayload);
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
