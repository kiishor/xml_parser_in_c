/**
 * \file
 * \brief   Tests the XML parser library

 * \author  Nandkishor Biradar
 * \date    14 September 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "shiporder.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */
extern void print_shiporder(shiporder_t* const pShiporder);

//! Callback function to allocate dynamic memory to store content of "item" element
//! It also implements the linked-list to store the dynamically allocated memory.
void* allocate_item(uint32_t occurrence, void* context)
{
    shiporder_t* const pShiporder = (shiporder_t*)context;
    void* const target = calloc(sizeof(item_t), 1);

    // Is this first occurrence of "item" element?
    if(pShiporder->item == NULL)
    {
        pShiporder->item = target;
        return target;
    }

    // Traverse to last node of the linked-list
    item_t* node = pShiporder->item;
    while(node->Next)
    {
        node = node->Next;
    }

    node->Next = target;
    return target;
}

int main(int argc, char *argv[])
{

  if(argc < 2)
  {
    printf("Please enter the XML file name\n");
    return 1;
  }

  FILE* fXml = fopen(argv[1], "r");
  if(fXml == NULL )
  {
    printf("Error in opening XML file\n");
    return 2;
  }

  // Get the size of XML file to allocate the memory
  fseek(fXml, 0, SEEK_END);
  long size = ftell(fXml);
  assert(size > 0);
  fseek(fXml, 0, SEEK_SET);

  // Allocate the memory to copy XML source from file.
  char* const xml = malloc((size_t)size);
  fread(xml, 1, (size_t)size, fXml);
  fclose(fXml);

  shiporder_t book;   // Holds the extracted content of XML file.
  xml_parse_result_t result = parse_xml(&shiporder_root, xml, &book, &book);
  if(result == XML_PARSE_SUCCESS)
  {
    printf("Parsing completed successfully\n");
    print_shiporder(&book);
  }
  else
  {
    printf("Failed to parse XML file\n");
  }

  free(xml);

  return 0;
}


