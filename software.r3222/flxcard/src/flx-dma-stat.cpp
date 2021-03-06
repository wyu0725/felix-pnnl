/*******************************************************************/
/*                                                                 */
/* This is the C++ source code of the FlxDmaStat application       */
/*                                                                 */
/* Author: Markus Joos, CERN                                       */
/*                                                                 */
/**C 2015 Ecosoft - Made from at least 80% recycled source code*****/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "DFDebug/DFDebug.h"
#include "cmem_rcc/cmem_rcc.h"
//#include "cmem_common.h" // EC, 20-Apr-2016
#include "FlxCard.h"
#include "FlxException.h"

#define APPLICATION_NAME    "flx-dma-stat"
#define APPLICATION_VERSION "1.0.0"

//Globals
FlxCard flxCard;

/*****************/
void display_help()
/*****************/
{
  printf("Usage: %s [OPTIONS]\n", APPLICATION_NAME);
  printf("Displays the status of an FLX-card's DMA controller\n\n");
  printf("Options:\n");
  printf("  -d NUMBER      Use card indicated by NUMBER. Default: 0.\n");
  printf("  -D level       Configure debug output at API level. 0=disabled, 5, 10, 20 progressively more verbose output. Default: 0.\n");
  printf("  -h             Display help.\n");
  printf("  -V             Display the version number.\n");
  printf("  -i NUMBER      DMA-id [0..7] status to display. Default: 0.\n");
}

/*****************************/
int main(int argc, char **argv)
/*****************************/
{
  int device_number = 0, dma_id = 0, opt, debuglevel;

  while((opt = getopt(argc, argv, "hd:D:i:V")) != -1) 
  {
    switch (opt) 
    {
      case 'd':
	device_number = atoi(optarg);
	break;
	
      case 'h':
	display_help();
	exit(0);
      
      case 'D':
	debuglevel = atoi(optarg);
        DF::GlobalDebugSettings::setup(debuglevel, DFDB_FELIXCARD);
	break;
      
      case 'i':
	dma_id = atoi(optarg);
	break;

      case 'V':
        printf("This is version %s of %s\n", APPLICATION_VERSION, APPLICATION_NAME);
	exit(0);
			
      default: 
	fprintf(stderr, "Usage: %s COMMAND [OPTIONS]\nTry %s -h for more information.\n", APPLICATION_NAME, APPLICATION_NAME);
	exit(-1);
    }
  }

  try
  {
    flxCard.card_open( device_number );
  
    u_long baraddr0 = flxCard.openBackDoor( 0 );

    flxcard_bar0_regs_t *bar0 = (flxcard_bar0_regs_t *) baraddr0;
    dma_descriptor_t    *desc = &bar0->DMA_DESC[dma_id];
    dma_status_t        *stat = &bar0->DMA_DESC_STATUS[dma_id];

    printf("DMA Enables: 0x%0x\n", bar0->DMA_DESC_ENABLE);
    printf("Start Ptr:   0x%016lx\n", desc->start_address);
    printf("End Ptr  :   0x%016lx\n", desc->end_address);
    printf("Read Ptr :   0x%016lx\n", desc->read_ptr);
    printf("Write Ptr:   0x%016lx\n", stat->current_address);
    printf("Descriptor done: 0x%lx\n", stat->descriptor_done);
    printf("Even Addr DMA  : 0x%lx\n", stat->even_addr_dma);
    printf("Even Addr PC   : 0x%lx\n", stat->even_addr_pc);

    // Buffer full/unread percentage:
    // how close is the write pointer 'behind' the read pointer
    // as a percentage of the buffer size
    u_long pwrite = stat->current_address;
    u_long pread  = desc->read_ptr;
    u_long pstart = desc->start_address;
    u_long pend   = desc->end_address;
    u_long percent;
    // Only if the write and read pointers make sense
    if( pend - pstart > 0 &&
	pwrite >= pstart && pwrite <= pend &&
	pread >= pstart && pread <= pend )
      {
	if( pread < pwrite )
	  {
	    percent = (100*(pwrite - pread))/(pend - pstart);
	  }
	else
	  {
	    percent = ((100*((pend - pread) + (pwrite - pstart))) /
		       (pend - pstart));
	  }
	printf("DMA size = 0x%08lX, %%unread: %ld\n",
	       pend - pstart, percent );
      }

    flxCard.card_close();
  }
  catch(FlxException ex)
  {
    std::cout << "ERROR. Exception thrown: " << ex.what() << std:: endl; 
    exit(-1);
  }

  exit(0);
}
