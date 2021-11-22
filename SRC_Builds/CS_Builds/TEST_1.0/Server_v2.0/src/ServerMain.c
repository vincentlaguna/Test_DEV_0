/*****************************************************************************
Author(s) or Contributor(s): Version 2.0 (TEST)  ~Vincent A. Laguna 2021                                                                 

File: ServerMain.c              

Description: Server-side Main  (Multi-Threaded)                                              

*****************************************************************************/


/* Includes: ****************************************************************/

#include  "../inc/CS_Common.h"

/****************************************************************************/

/* Main Starts Here: ********************************************************/

int main(int argc, char *argv[])
{ 
  // Initialize Local Objects
  SOKData   sokData[NUM_SRV_THREADS];
  pthread_t SOKthread[NUM_SRV_THREADS];
  // Initialize Number of Server Thread Objects in a for-loop
  for (int i = 0; i < NUM_SRV_THREADS; i++)
  {
  #ifdef DBG
  
    sokData[i].SOKid = (i+1) * 10;
  
  #endif
    // Basic Assignment
    sokData[i].cIP   = malloc(sizeof(uint8_t) * IP_STR_SZ);
    sokData[i].cIP   = szRem_Srv_IP[i];
    sokData[i].uPort = uRem_Srv_Port[i];
    // Initialize Thread Handlers  
    pthread_create(&SOKthread[i], NULL, UDP_SrvConnection_Hndlr, (void *)&sokData[i]);
  
  #ifdef DBG
  
    printf("\nIn Main: SOKid = %d\n", sokData[i].SOKid);
  
  #endif
  
  }
  
  for (int j = 0; j < NUM_SRV_THREADS; j++)
  {
    pthread_join(SOKthread[j], NULL);
  }
   
  return(0);

}

/****************************************************************************/
// End ServerMain.c
