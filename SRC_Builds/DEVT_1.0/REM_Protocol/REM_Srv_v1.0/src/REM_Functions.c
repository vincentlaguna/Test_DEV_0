/*****************************************************************************
Author(s) or Contributor(s): Version (DEV) 1.0  ~Vincent A. Laguna 2022                                                                 

File:	REM_Functions.c                                                      

Description: Functions and Utilites used REM_Server.c for Remote Calls                                             
*****************************************************************************/


/* Includes: ****************************************************************/

#include "../inc/CS_Common.h"

/****************************************************************************/

/* Globals: *****************************************************************/

/****************************************************************************/

/* Function Definitions: ****************************************************/
// Helper Functions

/*****************************************************************************

Name:	UDP_SokInit_Handlr()                                               
Purpose: Handles the creation of a UDP Socket                               
Parameters: None                                                             
Returns: signed 16-bit integer                                        

*****************************************************************************/

int16_t  UDP_SokInit_Handlr(void)
{
  // Local Variables
  int16_t  hSok;
  // Output
  printf("\n[-]SERVER-Side Socket Initialization = in progress...\n");
  // Socket System Call
  hSok = socket(AF_INET, SOCK_DGRAM, 0);        
  // Output Validation
  printf("[+]SERVER-Side Socket Initialization = OK\n");
  // Function Return
  return  hSok;
}

// End UDP_SokInit_Handlr()
/****************************************************************************/


/*****************************************************************************

Name:	init_test_buffer()                                       
Purpose:  Test Prototype Function to test initializing a test buffer
Parameters: Double Pointer to test_buffer struct
Returns: void                                        

*****************************************************************************/
//
void  init_test_buffer(test_buffer **d_pbuffer)
{
  (*d_pbuffer) = (test_buffer *)calloc(1, sizeof(test_buffer));

  (*d_pbuffer)->tst_bffr_data = calloc(1, BUFF_SZ);

  (*d_pbuffer)->size = BUFF_SZ;

  (*d_pbuffer)->next = 0;
}

// End init_test_buffer() 
/****************************************************************************/


/*****************************************************************************

Name:	test_serialize_data() AKA "Serialize P-Object data"                              
Purpose:  Test Prototype Function for serializing data struct for buffer
Parameters: Pointer to data, pointer to buffer                                         
Returns: void                                        

*****************************************************************************/
// Since the MIN_STR_SZ is 30, test_data object has 2B of padding...
void  test_serialize_data(test_data *p_data, test_buffer *p_buffer)
{
  if (!p_data)
  {
    uint16_t sntel_val = 0xFFFFFFFF; // 2B value
    test_insert_data(p_buffer, (uint8_t *)&sntel_val, sizeof(uint16_t));


    return;
  }
  
  test_insert_data(p_buffer, (uint8_t *)p_data->u8_sz_data_0,
                   sizeof(uint8_t) * MIN_STR_SZ);
  test_insert_data(p_buffer, (uint8_t *)&p_data->u8_data_1, sizeof(uint8_t));
  test_insert_nest_data(&p_data->nst_data_0, p_buffer); // Insert nest struct
  test_insert_data(p_buffer, (uint8_t *)&p_data->u16_data_2, sizeof(uint16_t));
  
  return; //
}

// End test_serialize_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_serialize_nest_data()                                  
Purpose:  Prototype Function for serializing nested data structs in buffer
Parameters: Pointer to data, pointer to buffer                                         
Returns: Pointer to serialized nested data struct                                        

*****************************************************************************/
//
void  test_serialize_nest_data(nest_data *p_nst_data, test_buffer *p_buffer)
{
  if (!p_nst_data)
  {
    uint16_t sntel_val = 0xFFFFFFFF; // 2B value
    test_insert_data(p_buffer, (uint8_t *)&sntel_val, sizeof(uint16_t));


    return;
  }
  
  test_insert_data(p_buffer, (uint8_t *)p_nst_data->u8_sz_nst_data_0,
                   sizeof(uint8_t) * MIN_STR_SZ);
  test_insert_data(p_buffer, (uint8_t *)&p_nst_data->u8_nst_data_ID, sizeof(uint8_t));
  test_serialize_data(&p_nst_data->p_test_data, p_buffer);
  test_serialize_data(p_nst_data->p_test_data_arr, p_buffer);
  
  return; //
}

// End test_serialize_nest_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_serialize_llist() AKA "Serialize list"                              
Purpose:  Test Prototype Function for serializing linked-lists
Parameters: Pointer to linked-list object, pointer to (serialized) buffer                                         
Returns: void                                        

*****************************************************************************/
// 
void  test_serialize_llist(test_ll *p_test_ll, test_buffer *p_buffer)
{
  if (!p_test_ll)
  {
    uint16_t sntel_val = 0xFFFFFFFF; // 2B value
    test_insert_data(p_buffer, (uint8_t *)&sntel_val, sizeof(uint16_t));


    return;
  }
  
  test_serialize_ll_node(p_test_ll->head, p_buffer);

  return; //
}

// End test_serialize_llist() 
/****************************************************************************/


/*****************************************************************************

Name:	test_serialize_ll_node() AKA "Serialize list node"                              
Purpose:  Test Prototype Function for serializing linked-list nodes
Parameters: Pointer to linked-list node object, pointer to (serialized) buffer                                         
Returns: void                                        

*****************************************************************************/
// 
void  test_serialize_ll_node(test_list_node *p_ll_node, test_buffer *p_buffer)
{
  if (!p_ll_node)
  {
    uint16_t sntel_val = 0xFFFFFFFF; // 2B value
    test_insert_data(p_buffer, (uint8_t *)&sntel_val, sizeof(uint16_t));


    return;
  }

  test_serialize_data(p_ll_node->pNode_tst_data, p_buffer);
  // Use of recursion
  test_serialize_ll_node(p_ll_node->pRight, p_buffer);

  return; //
}

// End test_serialize_ll_node() 
/****************************************************************************/


/*****************************************************************************

Name:	test_de_serialize_data() AKA de-serialize "P-Object" data                                      
Purpose:  Test Prototype Function for de-serializing data struct for buffer
Parameters: Pointer to data, pointer to buffer                                         
Returns: Pointer to de-serialized data struct                                        

*****************************************************************************/
//
test_data *test_de_serialize_data(test_buffer *p_buffer)
{
  // x-val detection code here
  uint16_t sntel_val = 0; // 2B value
  test_parse_data((uint8_t *)&sntel_val, p_buffer, sizeof(uint16_t));

  if (sntel_val == 0xFFFFFFFF)
    return NULL;

  test_buffer_skip(p_buffer, -1 * sizeof(uint16_t));
  // Allocate memory to reconstruct the "object"
  test_data *p_tst_data = calloc(1, sizeof(test_data));

  test_parse_data((uint8_t *)&p_tst_data->u8_sz_data_0,
                  p_buffer, sizeof(uint8_t) * MIN_STR_SZ);
  test_parse_data((uint8_t *)&p_tst_data->u8_data_1, p_buffer, sizeof(uint8_t));
  // parse nested stuff...
  nest_data *p_nest_data = test_de_serialize_nest_data(p_buffer);
  p_tst_data->nst_data_0 = *p_nest_data; // Shallow copy -> no internal objects
  // parse pointer nested object
  p_tst_data->p_nst_data_0 = test_de_serialize_nest_data(p_buffer);
   // Shallow free as well...
  free(p_nest_data);
  // Last item in object
  test_parse_data((uint8_t *)&p_tst_data->u16_data_2, p_buffer, sizeof(uint16_t));
  
  return p_tst_data;
}

// End test_de_serialize_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_de_serialize_nest_data()                                  
Purpose:  Prototype Function for de-serializing nested data structs in buffer
Parameters: Pointer to data, pointer to buffer                                         
Returns: Pointer to de-serialized nested data struct                                        

*****************************************************************************/
//
nest_data *test_de_serialize_nest_data(test_buffer *p_buffer)
{
  // x-val detection code here
  uint16_t sntel_val = 0; // 2B value
  test_parse_data((uint8_t *)&sntel_val, p_buffer, sizeof(uint16_t));

  if (sntel_val == 0xFFFFFFFF)
    return NULL;

  test_buffer_skip(p_buffer, -1 * sizeof(uint16_t));
  // Allocate memory to reconstruct the "object"
  nest_data *p_nest_data = calloc(1, sizeof(nest_data));

  test_parse_data((uint8_t *)&p_nest_data->u8_sz_nst_data_0,
                  p_buffer, sizeof(uint8_t) * MIN_STR_SZ);
  test_parse_data((uint8_t *)&p_nest_data->u8_nst_data_ID, p_buffer, sizeof(uint8_t));
  // parse pointer elements
  // p_nest_data->p_test_data = test_de_serialize_data(p_buffer);
  // p_nest_data->p_test_data_arr = test_de_serialize_data(p_buffer); // Loop?
  
  return p_nest_data;
}

// End test_de_serialize_nest_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_de_serialize_llist() AKA "De-serialize list"                              
Purpose:  Test Prototype Function for de-serializing linked-lists
Parameters: Pointer to (serialized) buffer                                         
Returns: Pointer to de-serialized linked-list                                        

*****************************************************************************/
// 
test_ll *test_de_serialize_llist(test_buffer *p_buffer)
{
  if (!p_buffer)
  {
    uint16_t sntel_val = 0xFFFFFFFF; // 2B value
    test_insert_data(p_buffer, (uint8_t *)&sntel_val, sizeof(uint16_t));


    return;
  }
  
  test_ll *p_test_ll = calloc(1, sizeof(test_ll));
  p_test_ll->head = test_de_serialize_ll_node(p_buffer);

  return p_test_ll; //
}

// End test_de_serialize_llist() 
/****************************************************************************/


/*****************************************************************************

Name:	test_de_serialize_ll_node() AKA "De-serialize list node"                              
Purpose:  Test Prototype Function for de-serializing linked-list nodes
Parameters: Pointer to (serialized) buffer                                         
Returns: Pointer to de-serialized linked-list node object                                      

*****************************************************************************/
// 
test_list_node *test_de_serialize_ll_node(test_buffer *p_buffer)
{
  if (!p_buffer)
  {
    uint16_t sntel_val = 0xFFFFFFFF; // 2B value
    test_insert_data(p_buffer, (uint8_t *)&sntel_val, sizeof(uint16_t));


    return;
  }

  test_list_node *p_test_list_node = calloc(1, sizeof(test_list_node));
  p_test_list_node->pNode_tst_data = test_de_serialize_data(p_buffer);
  // Use of recursion
  p_test_list_node->pRight = test_de_serialize_ll_node(p_buffer);

  return p_test_list_node; //
}

// End test_de_serialize_ll_node() 
/****************************************************************************/


/*****************************************************************************

Name:	test_insert_data() AKA "Serialize (x) data for buffer" - SER-D                                 
Purpose:  Test Prototype Function for inserting data into data buffer
Parameters: Pointer to buffer struct, pointer to data, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_insert_data(test_buffer *p_buffer, uint8_t *p_data, uint8_t n_bytes)
{
  uint8_t alloc_sz = p_buffer->size - p_buffer->next;
  
  uint8_t rsz_ct = 0;

  while (alloc_sz < n_bytes)
  {
    p_buffer->size = p_buffer->size * 2;
    alloc_sz = p_buffer->size - p_buffer->next;
    rsz_ct = 1;
  }

  if (rsz_ct == 0)
  {
    memcpy((uint8_t *)p_buffer->tst_bffr_data + p_buffer->next, p_data, n_bytes);
  }
  // Size modification to buffer
  p_buffer->tst_bffr_data = realloc(p_buffer->tst_bffr_data, p_buffer->size);
  memcpy((uint8_t *)p_buffer->tst_bffr_data + p_buffer->next, p_data, n_bytes);
  p_buffer->next += n_bytes;

  return; //
}

// End test_insert_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_insert_nest_data()                                       
Purpose:  Test Prototype Function for inserting nested struct data into test buffer
Parameters: Pointer to test_buffer struct, pointer to data, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_insert_nest_data(nest_data *p_data, test_buffer *p_buffer)
{
  test_insert_data(p_buffer, (uint8_t)p_data->u8_sz_nst_data_0, sizeof(uint8_t) * MIN_STR_SZ);
  test_insert_data(p_buffer, (uint8_t)&p_data->u8_nst_data_ID, sizeof(uint8_t));
  test_serialize_data(p_data->p_test_data, p_buffer);
  
  return;
}

// End test_insert_nest_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_read_data() - Print the data?                                       
Purpose:  Test Prototype Function for reading data from data buffer
Parameters: Pointer to buffer struct, pointer to data, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_read_data(test_buffer *p_buffer, uint8_t *p_data, uint8_t n_bytes)
{
  // Data extraction from buffer - does any of the previous stuff works? 04282022
  
  // uint8_t alloc_sz = p_buffer->size - p_buffer->next;
  
  // uint8_t rsz_ct = 0;

  // while (alloc_sz < n_bytes)
  // {
  //   p_buffer->size = p_buffer->size * 2;
  //   alloc_sz = p_buffer->size - p_buffer->next;
  //   rsz_ct = 1;
  // }

  // if (rsz_ct == 0)
  // {
  //   memcpy((uint8_t *)p_buffer->tst_bffr_data + p_buffer->next, p_data, n_bytes);
  // }
  // // Size modification to buffer
  // p_buffer->tst_bffr_data = realloc(p_buffer->tst_bffr_data, p_buffer->size);
  // memcpy((uint8_t *)p_buffer->tst_bffr_data + p_buffer->next, p_data, n_bytes);
  // p_buffer->next += n_bytes;

  return; //
}

// End test_read_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_parse_data() AKA de-serialize data...                                       
Purpose:  Test Prototype Function for de-serializing data from test buffer
Parameters: Pointer to destination, pointer to test_buffer struct, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_parse_data(uint8_t *p_dst, test_buffer *p_buffer, uint8_t size)
{
  // if (!p_buffer) assert(0); // include assert at some point 091122
  if (!size) return;
  // if ((p_buffer->size - p_buffer->next) < size) assert(0);

  memcpy(p_dst, p_buffer->tst_bffr_data + p_buffer->next, size);

  p_buffer->next += size;
}

// End test_parse_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_skip_data()                                       
Purpose:  Test Prototype Function for skipping n data bytes from test buffer
Parameters: Pointer to test_buffer struct, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_skip_data(test_buffer *p_buffer, uint8_t skip_sz)
{
  if (p_buffer->next + skip_sz > 0 &&
      p_buffer->next + skip_sz < p_buffer->size)
      
      p_buffer->next += skip_sz;
}

// End test_skip_data() 
/****************************************************************************/


/*****************************************************************************

Name:	test_buffer_skip() AKA ser-buff-skip                                       
Purpose:  Test Prototype Function for skipping n bytes serialized test buffer
Parameters: Pointer to test_buffer struct, size                                          
Returns: void                                        

*****************************************************************************/
//
void  test_buffer_skip(test_buffer *p_buffer, uint8_t skip_sz)
{
  u_int16_t available_sz = p_buffer->size - p_buffer->next;

  if (available_sz >= skip_sz)
  {
    p_buffer->next += skip_sz;
    return;
  }

  while (available_sz < skip_sz)
  {
    p_buffer->size = p_buffer->size * 2;
    available_sz = p_buffer->size - p_buffer->next;
  }
  
  p_buffer->tst_bffr_data = realloc(p_buffer->tst_bffr_data, p_buffer->size);
  p_buffer->next += skip_sz;
      
}

// End test_buffer_skip() 
/****************************************************************************/


/*****************************************************************************

Name:	test_reset_buffer()                                       
Purpose:  Test Prototype Function for resetting test buffer
Parameters: Pointer to test_buffer struct                                          
Returns: void                                        

*****************************************************************************/
//
void  test_reset_buffer(test_buffer *p_buffer)
{
  if (p_buffer->next != 0)
    p_buffer->next = 0;
}

// End test_reset_buffer() 
/****************************************************************************/


/*****************************************************************************

Name:	test_free_buffer()                                       
Purpose:  Test Prototype Function for freeing test buffer
Parameters: Pointer to test_buffer struct                                          
Returns: void                                        

*****************************************************************************/
//
void  test_free_buffer(test_buffer *p_buffer)
{
  free(p_buffer->tst_bffr_data);
  free(p_buffer);
}

// End test_free_buffer() 
/****************************************************************************/

/*****************************************************************************

Name:	serialize_person_t()                              
Purpose:  Function for serializing person_t struct for buffer
Parameters: Pointer to person_t struct, pointer to buffer                                         
Returns: void                                        

*****************************************************************************/
// 
void  serialize_person_t(person_t *p_person_t, test_buffer *p_buffer)
{
  int i = 0;
  unsigned int sentinel = 0xFFFFFFFF;
  SENTINEL_INSERTION_CODE(p_person_t, p_buffer);
  // Use a For-Loop to serialize each element in the vehicle_no[4] array
  for (i = 0; i < 4; i++)
  {
    test_insert_data(p_buffer, (uint8_t *)&p_person_t->vehicle_no[i], sizeof(unsigned int));
  }
  
  test_insert_data(p_buffer, (uint8_t *)&p_person_t, sizeof(int));

  if (p_person_t->height)
  {
    test_insert_data(p_buffer, (uint8_t *)p_person_t->height, sizeof(int));
  }
  else // Use Sentinel value to fill the space
  {
    test_insert_data(p_buffer, (uint8_t *)&sentinel, sizeof(unsigned int));
  }
  // For-Loop to serialize each element in the *last_sal_amounts[5] array
  for (i = 0; i < 5; i++)
  {
    if (p_person_t->last_sal_amounts[i])
    {
      test_insert_data(p_buffer, (uint8_t *)p_person_t->last_sal_amounts[i],
                       sizeof(unsigned int));
    }
    else
    {
      test_insert_data(p_buffer, (uint8_t *)&sentinel, sizeof(unsigned int));
    }
  }

  test_insert_data(p_buffer, (uint8_t *)p_person_t->name, 32);
  serialize_company_t(&p_person_t->company, p_buffer);
  // For-Loop to serialize each element in the *dream_companies[3] array
  for (i = 0; i < 3; i++)
  {
    serialize_company_t(&p_person_t->dream_companies[i], p_buffer);
  }
  // Recursive
  serialize_person_t(p_person_t->CEO, p_buffer);
  // For-Loop to serialize each element in the admin_staff[5] array
  for (i = 0; i < 5; i++)
  {
    serialize_person_t(p_person_t->admin_staff[i], p_buffer);
  }

  return; //
}

// End serialize_person_t() 
/****************************************************************************/


/*****************************************************************************

Name:	serialize_company_t()                              
Purpose:  Function for serializing company_t struct for buffer
Parameters: Pointer to company_t struct, pointer to buffer                                         
Returns: void                                        

*****************************************************************************/
// 
void  serialize_company_t(company_t *p_company_t, test_buffer *p_buffer)
{
  unsigned int sentinel = 0xFFFFFFFF;
  SENTINEL_INSERTION_CODE(p_company_t, p_buffer);
  
  test_insert_data(p_buffer, (uint8_t *)p_company_t->comp_name, 32);
  test_insert_data(p_buffer, (uint8_t *)p_company_t->emp_strength, sizeof(int));
  serialize_person_t(p_company_t->CEO, p_buffer);

  return; //
}

// End serialize_company_t() 
/****************************************************************************/


/*****************************************************************************

Name:	de_serialize_person_t()                              
Purpose:  Function for de-serializing person_t struct from buffer
Parameters: Pointer to buffer                                         
Returns: Pointer to person_t structure                                        

*****************************************************************************/
// 
person_t  *de_serialize_person_t(test_buffer *p_buffer)
{
  int i = 0;
  unsigned int sentinel = 0;

  SENTINEL_DETECTION_CODE(p_buffer);

  person_t *p_person_t = calloc(1, sizeof(person_t));
  // Use a For-Loop to parse each element of the vehicle_no[4] array
  for (i = 0; i < 4; i++)
  {
    test_parse_data((uint8_t *)&p_person_t->vehicle_no[i], p_buffer, sizeof(unsigned int));
  }
  
  test_parse_data((uint8_t *)&p_person_t->age, p_buffer, sizeof(int));

  // Next member field is a pointer! Test for Sentinel value (pad-filler)
  // in the next 4 Bytes in the serialized buffer you are parsing from
  test_parse_data((uint8_t *)&sentinel, p_buffer, sizeof(unsigned int));

  if (sentinel == 0xFFFFFFFF)
  {
    p_person_t->height = NULL;
  }
  else
  {
    test_buffer_skip(p_buffer, -1 * sizeof(unsigned int));
    p_person_t->height = calloc(1, sizeof(int));
    test_parse_data((uint8_t *)p_person_t->height, p_buffer, sizeof(int));
  }

  // For-Loop to parse each element in the *last_sal_amounts[5] array
  for (i = 0; i < 5; i++)
  {
    test_parse_data((uint8_t *)&sentinel, p_buffer, sizeof(unsigned int));

    if (sentinel == 0xFFFFFFFF)
    {
      p_person_t->last_sal_amounts[i] = NULL;
    }
    else
    {
      test_buffer_skip(p_buffer, -1 * sizeof(unsigned int));
      p_person_t->last_sal_amounts[i] = calloc(1, sizeof(unsigned int));
      test_parse_data((uint8_t *)p_person_t->last_sal_amounts[i], p_buffer, sizeof(unsigned int));
    }
  }

  test_parse_data((uint8_t *)p_person_t->name, p_buffer, 32);

  company_t *company = de_serialize_company_t(p_buffer);
  // Shallow-copy (not a pointer) - Free it right after!
  p_person_t->company = *company;
  free(company);

  // For-Loop to parse each element in the *dream_companies[3] array
  for (i = 0; i < 3; i++)
  {
    company = de_serialize_company_t(p_buffer);
    p_person_t->dream_companies[i] = *company;
    free(company);
  }
  // Recursive
  p_person_t->CEO = de_serialize_person_t(p_buffer);
  // For-Loop to serialize each element in the admin_staff[5] array
  for (i = 0; i < 5; i++)
  {
    p_person_t->admin_staff[i] = de_serialize_person_t(p_buffer);
  }

  return; //
}

// End de_serialize_person_t() 
/****************************************************************************/


/*****************************************************************************

Name:	de_serialize_company_t()                              
Purpose:  Function for de-serializing company_t struct from buffer
Parameters: pointer to buffer                                         
Returns: pointer to company_t data structure                                        

*****************************************************************************/
// 
company_t  *de_serialize_company_t(test_buffer *p_buffer)
{  
  SENTINEL_DETECTION_CODE(p_buffer);
  
  company_t *p_company_t = calloc(1, sizeof(company_t));

  test_parse_data((uint8_t *)p_company_t->comp_name, p_buffer, 32);
  test_parse_data((uint8_t *)p_company_t->emp_strength, p_buffer, sizeof(int));
  p_company_t->CEO = de_serialize_person_t(p_buffer);

  return p_company_t; //
}

// End de_serialize_company_t() 
/****************************************************************************/


/*****************************************************************************

Name:	REM_DataSnd() function (for UDP threaded version server)                                        
Purpose:       
Parameters: Pointer to SOKData, unsigned uint8_t pointer to send buffer                                          
Returns: void                                        

*****************************************************************************/
// Only temp for testing
void REMDataSnd(uint8_t *pSink1, uint8_t *pSink2, uint8_t *pSrc, uint8_t *pDst,
                uint8_t *pCmd, uint8_t *pLen, uint8_t *pData, 
                uint8_t *pMsb, uint8_t *pLsb, uint8_t *pCs)
{
  printf("\nTesting REMDataSnd() Function call >>> \n");
  printf("\n<sink1 = %02X>|<sink2 = %02X>|<src = %02X>|<dst = %02X>|<cmd = %02X>|<len = %02X>|<data = %s>|<msb = %02X>|<lsb = %02X>|<cs = %02X>\n\n", pSink1, pSink2, pSrc, pDst, pCmd, pLen, pData, pMsb, pLsb, pCs);

  // Build the message
  // Send the message in separate function
  // Notes to self: change the function arguments to specify the length
  // of data (bytes)
}

// End REM_DataSnd()
/****************************************************************************/

// Utility Functions

/*****************************************************************************

Name: convertHex()                                               
Purpose:  Converts binary data to hexadecimal representation                               
Parameters: const unsigned char pointer and size_t for length                                                             
Returns:  Unsigned char pointer                                        

*****************************************************************************/

uint8_t  *convertHex(uint8_t *src, size_t len)
{
  char  *retVal;
  char  *hexBits = "0123456789ABCDEF";
  
  if (src == NULL || len <= 0)
    return NULL;
    
  uint32_t  retValLen = (len * 3) + 1;
  retVal = malloc(retValLen);
  // sets the first n bytes of the area starting at retVal 
  // to zero (bytes containing '\0')
  bzero(retVal, retValLen);
  
  for (uint8_t i = 0; i < len; i++)
  {
    retVal[i*3]     = hexBits[src[i] >> 4]; // Right shift all bits 4 places
    retVal[(i*3)+1] = hexBits[src[i] & 0x0F];
    retVal[(i*3)+2] = ' '; // Space each value
  }
  
  return retVal;
  
}

// End convertHex()
/****************************************************************************/


/*****************************************************************************

Name: bCheckSum()                                               
Purpose:  Validates the checksum on 2 input buffers                               
Parameters: const unsigned char pointer and size_t for length                                                             
Returns:  Boolean                                        

*****************************************************************************/

bool  bCheckSum(const uint8_t *buff1, const uint8_t *buff2, size_t sZ)
{
  bool bRetVal  = false;
  uint8_t chkBuff1 = 0;
  uint8_t chkBuff2 = 0;
  
  if (buff1 == NULL || buff2 == NULL)
    return NULL;
  
  while (sZ-- != 0)
  {
    // For debugging...
    // printf("%c ", *buff1);
    // printf("\n");
    // printf("%c ", *buff2);
    chkBuff1 -= *buff1++;
    chkBuff2 -= *buff2++;
  }
  // For debugging...
  // printf("\nChecksum for buffer 1 is: 0x%02x = %d\n", chkBuff1, chkBuff1);
  // printf("\nChecksum for buffer 2 is: 0x%02x = %d\n\n", chkBuff2, chkBuff2);
  if (chkBuff1 == chkBuff2)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// End checkSum()
/****************************************************************************/

// END REM_Functions.c