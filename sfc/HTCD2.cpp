/*
 * Tyler Anderson Tue Dec 13 10:27:31 EST 2011
 * HTCD2
 */

// These are necessary system headers
#include <stdio.h>
#include <string.h>

// These are necessary program headers
#include "HTCD2.h"
#include "tcd2_defs.h"

HTCD2::HTCD2()
{
  ;
}

int HTCD2::open(int vendor, int product)
{
  return dev.TCD2Usb_initialization(vendor, product);
}

int HTCD2::close()
{
  return dev.TCD2Usb_shutdown();
}

int HTCD2::reopen(int vendor, int product)
{
  return dev.TCD2Usb_reinitialization(vendor, product);
}

int HTCD2::clear_event()
{
  char dummy[4] = {0,0,0,0}; // Quick write, no command checking
  dummy[0] = (CLEAR << 4) | TCDMT;
  dummy[1] = (CLEAR_MT);
  dummy[2] = 0;
  dummy[3] = 0;
  return dev.TCD2Usb_bulk_write_EP1OUT(dummy,4);
  
}

int HTCD2::clear_timeout()
{
  return clear_event();
}

int HTCD2::flush_buffer()
{
  char clearbuf[4*10];
  char flushbuf[4];
  char clearmtbuf[4];
  int retval = 0;

  // First clear all the local triggers
  for(int i = 0; i < 40; i++)
    {
      clearbuf[i*4] = (CLEAR << 4) | i;
      clearbuf[i*4+1] = CLEAR_LT;
      clearbuf[i*4+2] = 0;
      clearbuf[i*4+3] = 0;
    }
  
  if( (retval = dev.TCD2Usb_bulk_write_EP1OUT(clearbuf,40)) )
    return retval;

  // Now flush the system
  flushbuf[0] = FLUSH << 4 | ROSTACK;
  flushbuf[1] = 0;
  flushbuf[2] = 0;
  flushbuf[3] = 0;

  if( (retval =  dev.TCD2Usb_bulk_write_EP1OUT(flushbuf,4)) )
    return retval;
  
  // Now clear the master trigger
  clearmtbuf[0] = CLEAR << 4 | TCDMT;
  clearmtbuf[1] = CLEAR_MT;
  clearmtbuf[2] = 0;
  clearmtbuf[3] = 0;
  
  if( (retval = dev.TCD2Usb_bulk_write_EP1OUT(clearmtbuf,4)) )
    return retval;

  return 0;
}


int HTCD2::tcd2_execute(unsigned int cmd, unsigned int node, unsigned int param, unsigned short *value)
{
  int retval = 0;
  
  //printf("HTCD2::tcd2_execute: Entering TCD2CmdProc::TCD2CmdProc_ConvertCommand\n");
  if( (retval = cmdproc.TCD2Cmd_ProcessCommand(cmd,node,param,value,&dev)) )
    {
      // printf("HTCD2::tcd2_execute: Error processing command\n");
      return retval;
    }
  
  return 0;
}

// Requested for later use by UMD
int HTCD2::tcd2_execute(char *file_name, unsigned int *values, int number_of_lines)
{
  return 0;
}

// Requested for later use by UMD
int HTCD2::tcd2_execute(char *string_one_cmd, unsigned *value)
{
  return 0;
}

int HTCD2::tcd2_getconf(tcf_data &tcf_for_tcd, tcf_data &tcf_for_cd)
{
  int retval = 0;

  if( (retval = cmdproc.TCD2Cmd_GetConf(&(tcf_for_tcd.h),(&dev))) )
    {
      printf("HTCD2::tcd2_getconf: error getting configuration for TCD: %d \n",retval);
      return retval;
    }
  
  // TODO need to fill out the rest of the tcf_data struct
  
  if( (retval = cmdproc.TCD2Cmd_GetConfCD(&(tcf_for_cd.h),(&dev))) )
    {
      printf("HTCD2::tcd2_getconf: error getting configuration for CD: %d \n",retval);
      return retval;
    }

  // TODO need to fill out the rest of the tcf_data struct 

  return 0;
}

int HTCD2::tcd2_readout(int *count_tcddata, unsigned char *tcd_buf, int *count_cddata, unsigned char *cd_buf)
{
  int retval = 0;
  unsigned char buf[512]; // raw bytes from the USB port
  int nbytes = 440; //Number of bytes we read in

  memset(buf,0,512);

  if( (retval = evtproc.TCD2Evt_ProcessEvent(nbytes,buf,count_tcddata,tcd_buf,count_cddata,cd_buf,&(dev))) )
    {
      printf("HTCD2::tcd2_readout: Error reading in TCD bytes\n");
      return retval;
    }
    
  return 0;
}

int HTCD2::tcd2_readout_gse(unsigned char * buf)
{
  int retval = 0;
  int nbytes = 440; //Number of bytes we read in
  
  int count_tcddata;
  unsigned char tcd_buf[1000];
  int count_cddata;
  unsigned char cd_buf[1000];

  if( (retval = evtproc.TCD2Evt_ProcessEvent(nbytes,buf,&count_tcddata,tcd_buf,&count_cddata,cd_buf,&(dev))) )
    {
      printf("HTCD2::tcd2_readout: Error reading in TCD bytes\n");
      return retval;
    }

  return 0;
}

int HTCD2::tcd2_fx2lp_reset(const char * path, unsigned int vendor, unsigned int product)
{
  return dev.TCD2Usb_fx2lp_reset(path,vendor,product);
}

int HTCD2::tcd2_initialization(unsigned int vendor, unsigned int product)
{
  return 0;
}

HTCD2::~HTCD2()
{
  ; 
}
