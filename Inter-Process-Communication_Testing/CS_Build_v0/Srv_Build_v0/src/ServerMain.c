/*****************************************************************************
Author(s) or Contributor(s): Version .0  ~<NAME> 2021                                                                 

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
  uint16_t  uSrvSok = 0; 
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
  
  SLEEP
  
  printf("\n>>> The SOCKET has been created >>>\n\n");

  SrvConnection_Hndlr(uSrvSok, MAX_CONN);
  
  return(0);

}

/****************************************************************************/

/****************************************************************************/
// End ServerMain.c
