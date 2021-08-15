/*****************************************************************************
* Author(s):                                                                 *
*                                                                            *
* File:		     ServerMain.c                                                  *
*                                                                            *
* Description: Server-side Main                                              *
*                                                                            *
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
  uint16_t  uSrvSok = 0; 
  uint16_t  sok     = 0; 
  uint16_t  clLen   = 0;
  
  S_SADDR   cL;
  // Initialize buffers to store the data
  // char        msg[100]    = {0};
  // char        clMsg[200]  = {0};
  // const char  *pMsg       = clMsg;
  DataBuffer_t  SrvDbuff;
  // Create Socket
  uSrvSok = SokInit_Handlr();
  // Winsock
  #ifndef   LIN
    
    WSADATA wsaData;
    
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
      fprintf(stderr,"WSAStartup() failed");
      exit(1);
    }
    
  #endif
  // Error Handling
  if (uSrvSok == -1)
  {
    printf("\nCreation of SOCKET Failed.\n");
    return EXIT_FAILURE;
  }
  
  sleep(1);
  
  printf("\n>>> The SOCKET has been created >>>\n\n");
  // Bind
  if (BindSrvSok_Hndlr(uSrvSok) < 0)
  {
    perror("BIND Failed."); // Print the error message
    return EXIT_FAILURE;
  }
  
  SLEEP
  
  printf("\n<<< BIND Done >>>\n\n");
  // Listen
  listen(uSrvSok, 5); // Number of MAX connections
  
  sleep(1);
  
  // Accept incoming connections
  while (1)
  {
    printf("\n>>> Waiting for incoming connections...\n\n");
    clLen                 = sizeof(S_SADDR_IN);
    // Accept connection from an incoming client
    sok = accept(uSrvSok, (S_SADDR *)&cL, (socklen_t *)&cL);
    
    if (sok < 0)
    {
      perror("ACCEPT Failed.");
      return EXIT_FAILURE;
    }
    
    printf("\nConnection ACCEPTED\n\n");
    // Buffers
    
    // memset(clMsg, '\0', sizeof clMsg);
    // memset(msg, '\0', sizeof msg);
    // // Receive a reply from the Client
    // if (recv(sok, clMsg, 200, 0) < 0)
    // {
    //   printf("\nRECEIVE Failed.\n");
    //   break;
    // }
    
    // printf("Client Message: %s\n", clMsg);
    
    // if (strncmp(pMsg, clMsg, sizeof(pMsg)) == 0)
    // {
    //   strcpy(msg, "Message Received Successfully");
    // }
    // else
    // {
    //   strcpy(msg, "INVALID MESSAGE!");
    // }
    // // Send some data
    // if(send(sok, msg, strlen(msg), 0) < 0)
    // {
    //   printf("\nSEND Failed.\n");
    //   return EXIT_FAILURE;
    // }
    
    // printf("\n<<< Waiting for incoming connections...\n");
    // // Accept Connection from another incoming Client
    // sok = accept(uSrvSok, (S_SADDR *)&cL, (socklen_t*)&clLen);
   
    // if (sok < 0)
    // {
    //   perror("ACCEPT Failed.");
    //   return 1;
    // }
    // printf("\nConnection ACCEPTED\n\n");
    // // Send some Data
    // if (send(sok, msg, strlen(msg), 0) < 0)
    // {
    //   printf("\nSEND Failed.\n");
    //   return 1;
    // }
    
    uint32_t  DbuffSize    = sizeof(DataBuffer_t);
    memset(SrvDbuff.cPayload, '\0', MAX_STR_SZ);
    // Receive a reply from the Client
    if (recv(sok, SrvDbuff.cPayload, DbuffSize, 0) < 0)
    {
      printf("\nRECEIVE Failed.\n");
      break;
    }
    
    printf("Client Message: %s\n", SrvDbuff.cPayload);
    
    // if (strncmp(pMsg, clMsg, sizeof(pMsg)) == 0)
    // {
    //   strcpy(msg, "Message Received Successfully");
    // }
    // else
    // {
    //   strcpy(msg, "INVALID MESSAGE!");
    // }
    // Send some data
    if(send(sok, SrvDbuff.cPayload, MAX_STR_SZ, 0) < 0)
    {
      printf("\nSEND Failed.\n");
      return EXIT_FAILURE;
    }
    
    printf("\n<<< Waiting for incoming connections...\n");
    // Accept Connection from another incoming Client
    sok = accept(uSrvSok, (S_SADDR *)&cL, (socklen_t*)&clLen);
   
    if (sok < 0)
    {
      perror("ACCEPT Failed.");
      return 1;
    }
    printf("\nConnection ACCEPTED\n\n");
    // Send some Data
    // if (send(sok, msg, strlen(msg), 0) < 0)
    // {
    //   printf("\nSEND Failed.\n");
    //   return 1;
    // }
    #ifndef LIN
      closesocket(uSrvSok);
      WSACleanup();
    #else
      close(uSrvSok);
    #endif
      sleep(1);
    
  }
  
  return(0);

}

/****************************************************************************/

/****************************************************************************/
// End ServerMain.c
