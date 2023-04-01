/*****************************************************************************

Author(s) or Contributor(s): Version 0.0  ~<NAME> 2023 

File: snmp_simple.c

Description: Sandbox                                              

*****************************************************************************/


/* Includes: ****************************************************************/

#include "../inc/snmp_simple.h"

/****************************************************************************/

/* Defines: *****************************************************************/
/****************************************************************************/

/* Typedefs: ****************************************************************/
/****************************************************************************/

/* Globals: *****************************************************************/
/****************************************************************************/

/* Function Definitions: ****************************************************/

/*****************************************************************************

Name:	bin_search()                                         
Purpose: SNMP Simple Server
Parameters: Sandbox                                          
Returns: Sandbox                                        

*****************************************************************************/

void snmp(void)
{
  
  printf("\nTESTING INSIDE FUNCTION CALL\n");

  netsnmp_session session, *ss;
  netsnmp_pdu *pdu;
  netsnmp_pdu *response;
  oid anOID[MAX_OID_LEN];
  size_t anOID_len;
  netsnmp_variable_list *vars;

  /* Initialize the SNMP library */
  init_snmp("snmpapp");

  /* Initialize a "session" that defines who we're going to talk to */
  snmp_sess_init(&session);
  session.peername = strdup("localhost"); /* Address of the SNMP agent */
  session.version = SNMP_VERSION_2c; /* Use SNMPv2c */
  session.community = "public"; /* Set the community string */

  /* Open the session */
  SOCK_STARTUP;
  ss = snmp_open(&session);

  /* Build the PDU for the request */
  pdu = snmp_pdu_create(SNMP_MSG_GET);
  anOID_len = MAX_OID_LEN;
  if (!snmp_parse_oid("system.sysDescr.0", anOID, &anOID_len)) {
      snmp_perror("snmp_parse_oid");
      exit(1);
  }
  snmp_add_null_var(pdu, anOID, anOID_len);

  /* Send the request */
  if (snmp_synch_response(ss, pdu, &response) != STAT_SUCCESS) {
      snmp_sess_perror("snmp_synch_response", ss);
      exit(1);
  }

  /* Process the response */
  for (vars = response->variables; vars; vars = vars->next_variable) {
      print_variable(vars->name, vars->name_length, vars);
  }

  /* Clean up */
  snmp_close(ss);
  SOCK_CLEANUP;

}

// End snmp() 
/****************************************************************************/

/****************************************************************************/
// End snmp.c