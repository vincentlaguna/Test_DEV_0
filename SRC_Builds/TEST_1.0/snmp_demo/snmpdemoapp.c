#include  <net-snmp/net-snmp-config.h>
#include  <net-snmp/net-snmp-includes.h>
#include  <string.h>

//#define   OTHER_METHODS

/* change the word "define" to "undef" to try the (insecure) SNMPv1 version */
 #define   DEMO_USE_SNMP_VERSION_3
 
 #ifdef   DEMO_USE_SNMP_VERSION_3
  #include  "net-snmp/transform_oids.h"
  const char  *our_v3_passphrase = "The Net-SNMP Demo Password";
 #endif

 int  main(int argc, char **argv)
 {
  // Declare variables
  struct  snmp_session  session, *ss; // Info about who we're going to be talking to
  struct  snmp_pdu      *pdu;         // Info about what we're going to be sending
  struct  snmp_pdu      *response;    // Info about what is getting sent back

  oid     anOID[MAX_OID_LEN];         // Location of the information we want to retrieve
  size_t  anOID_len;// = MAX_OID_LEN;

  struct  variable_list *vars;        // List of variables we want to manipulate via SNMP
  int     status;
  int     count = 1;

  // Initialize the SNMP Library
  init_snmp("snmpdemoapp");

  // Initialize a "session" that defines who we're going to talk to
  snmp_sess_init(&session);           // Setup the defaults
  session.peername = strdup("test.net-snmp.org");

  // Setup authentication parameters for talking to the server
  #ifdef  DEMO_USE_SNMP_VERSION_3      // Use SNMPv3

    // Setup the SNMP version number
    session.version = SNMP_VERSION_3;

    // Setup the SNMP user name
    session.securityName    = strdup("MD5User");
    session.securityNameLen = strlen(session.securityName);

    // Set the security level to authenticated, but not encrypted
    session.securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV;

    // Set the authentication method to MD5
    session.securityAuthProto    = usmHMACMD5AuthProtocol;
    session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol) / sizeof(oid); 
    session.securityAuthKeyLen   = USM_AUTH_KU_LEN;

    // Set the authentication key to a MD5 hashed version of our passphrase 
    // "The Net-SNMP Demo Password" (which must be at least 8 characters long)
    if (generate_Ku(session.securityAuthProto,
                    session.securityAuthProtoLen,
                    (u_char *)our_v3_passphrase, strlen(our_v3_passphrase),
                    session.securityAuthKeyLen) != SNMPERR_SUCCESS)
    {
      snmp_error(argv[0]);
      snmp_log(LOG_ERR, "Error generating Ku from authentication passphrase. \n");
      exit(1);
    }

  #else // Use the unsecure (but simpler) SNMPv1

    // Setup the SNMP version number
    session.version = SNMP_VERSION_1;

    // Set the SNMPv1 community name used for authentication
    session.community     = "public";
    session.community_len = strlen(session.community);
   
  #endif // SNMPv1 

  // After establishing the session we will need to open it.
  // Opening it will return a pointer to another session
  // we should use for all of our future calls

  // Windows32 specific initialization (is a noop on unix)
  SOCK_STARTUP;

  // Open the session
  ss = snmp_open(&session);            // Establish the session

  // If failed...
  if (!ss)
  {
    snmp_sess_perror("ack", &session);
    //snmp_log(LOG_ERR, "Something horrible happened!!!\n");
    SOCK_CLEANUP;
    exit(1);
  }

  // Create the PDU we are sending when requesting information. 
  // In this case we are creating an SNMP-GET pdu, which is what 
  // the snmpget program uses. It retrieves a value for each oid
  // you initialize it with.

  // Create the PDU for the data (for our request).
  // 1) We're going to GET the system.sysDescr.0.node.
  pdu = snmp_pdu_create(SNMP_MSG_GET);
  anOID_len = MAX_OID_LEN;
  // So, let's fill it with our requested oid. 
  // Let's get the system.sysDescr.0 variable for this example. 
  // There are numerous ways you could create the oid in question. 
  // You could put in the numerical unsigned integer values yourself 
  // into the anOID array we created above, or you could use one of 
  // the following function calls to do it. We recommend the first one (get_node), 
  // as it is the most powerful and accepts more types of OIDs. 
  if (!snmp_parse_oid(".1.3.6.1.4.1.2021.13.4242.1.1.2", anOID, &anOID_len)) 
  {
    snmp_perror(".1.3.6.1.4.1.2021.13.4242.1.1.2");
    SOCK_CLEANUP;
    exit(1);
  }

  get_node(".1.3.6.1.4.1.2021.13.4242.1.1.2", anOID, &anOID_len);
  //read_objid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len); // <- This method times out 040523
  read_objid(".1.3.6.1.4.1.2021.13.4242.1.1.2", anOID, &anOID_len); // <- Timeout (Sub-id not found: (top) -> system)
  
  #if OTHER_METHODS
    get_node("sysDescr.0", anOID, &anOID_len);
    read_objid("system.sysDescr.0", anOID, &anOID_len);
  #endif

  // So we add this oid, with a NULL value to the PDU using the following statement:
  // (all oids should be paired with a NULL value for outgoing requests for information. 
  // For an SNMP-SET pdu, we'd put in the value we wanted to set the oid to).
  snmp_add_null_var(pdu, anOID, anOID_len);

  // Finally, we can send out the request using the session pointer and the pdu and we get 
  // back a status and the response, which is stored in the newly malloced response pdu pointer. 
  
  // Send the Request Out
  status = snmp_synch_response(ss, pdu, &response);

  // Now we can examine the information, assuming our request was fulfilled properly. 
  // If not, we'll print the error that caused the problem: 

  // Process the response
  if ((status == STAT_SUCCESS) && (response->errstat == SNMP_ERR_NOERROR))
  {
    // SUCCESS: Print the result variables
    for (vars = response->variables; vars; vars = vars->next_variable)
      print_variable(vars->name, vars->name_length, vars);

    // Then, for kicks, lets get the information and manipulate it ourselves
    // (checking to make sure it's the type we're expecting (a string) first): 
    
    // Manipulate the information ourselves
    for (vars = response->variables; vars; vars = vars->next_variable)
    {
      int count = 1;
      
      if (vars->type == ASN_OCTET_STR)
      {
        char *sp = malloc(1 + vars->val_len);
        sp[vars->val_len] = '\0';
        printf("Value #%d is a string: %s\n", count++, sp);
        free(sp);
      }
      else
        printf("Value #%d is NOT a string! Ack!\n", count++);
    }
  }
  else // For Errors...
  {
    if (status == STAT_SUCCESS)
      fprintf(stderr, "Error in packet\nReason: %s\n",
              snmp_errstring(response->errstat));
    else
      snmp_sess_perror("snmpget", ss);
  }

  // Free the response, as it is now our job to do so. 
  // Exit cleanly by calling snmp_close() as well:

  // Clean up:
  //  1) free the response.
  //  2) close the session.
  if (response)
    snmp_free_pdu(response);
  snmp_close(ss);

  // Windows32 Specific cleanup (is a noop in unix)
  //SOCK_CLEANUP;

 } // End of main()