/*
 * Tyler Anderson Wed Dec 14 19:32:34 EST 2011
 * TCD2Cmd implementation
 */
#include <stdio.h>
#include <string.h>
#include "TCD2Cmd.h"
#include "tcd2_err.h"

const char TCD2Cmd_nodenames[10][10] = {"pxl","pxr","pyl","pyr","mxl","mxr","myl","myr","s3","cd"};

TCD2Cmd::TCD2Cmd()
{
  dcmd = 0;
  dnode = 0;
  dparam = 0;
  dvalue = NULL;
  ddoresp = 1;
}

int TCD2Cmd::TCD2Cmd_GetConf(unsigned short * buf, TCD2Usb * dev)
{
  // tcf from past flights has this ordering:
  const unsigned short getconf_params[GETCONF_PARAMS_PER_NODE] = {THR00, THR01, THR02, THR03, THR10, THR11, THR12, THR13, LEVEL0, LEVEL1, DISC0, DISC1, DISC2, DISC3, HV00, HV01, HV10, HV11};
  
  unsigned short value[1] = {0};
  int retval = 0;
  unsigned short *p = buf;
    
  *p = 0;
  
  // First fill out the header by using NOPs
  for( int node = PXL; node <= S3; node++ )
    {
      
      value[0] = ((unsigned short) ( 0x0FFF & ~node));
      // printf("value[0] = %d\n",value[0]);
      if( (retval = TCD2Cmd_ProcessCommand(TCDNOP,node,0,value,dev)) )
	{
	  // printf("TCD2Cmd::TCD2Cmd_GetConf: Error sending command: %d %d %d %x \n",TCDNOP,node,0,value,dev);
	  return retval;
	}
      // check if it's right
      if( value[0] == ((unsigned short) ( node)) )
	{
	  *p |= (0x01 << node );
	}
      else
	printf("TCD2Cmd::TCD2Cmd_GetConf: Warning: %s concentrator not responding \n",TCD2Cmd_nodenames[node]);
    }
  p++;
  // Now get the configuration for PXL - S3
  for( int node = PXL; node <= S3; node++ )
    {
      for( int j = 0; j < GETCONF_PARAMS_PER_NODE; j++ )
	{
	  // printf("Running for node %d, param %d\n",node,getconf_params[j]);
	  *value = 0;
	  if( (retval = TCD2Cmd_ProcessCommand(GET,node,getconf_params[j],value,dev)) )
	    {
	      // printf("TCD2Cmd::TCD2Cmd_GetConf: Error sending command %d %d %d %x\n",GET,node,getconf_params[j],value,dev);
	      return retval;
	    }
	  *p++ = *value;
	}
    }
  return 0;
}

int TCD2Cmd::TCD2Cmd_GetConfCD(unsigned short * buf, TCD2Usb * dev)
{
  // tcf from past flights has this ordering:
  const unsigned short getconf_params[GETCONF_PARAMS_PER_NODE] = {THR00, THR01, THR02, THR03, THR10, THR11, THR12, THR13, LEVEL0, LEVEL1, DISC0, DISC1, DISC2, DISC3, HV00, HV01, HV10, HV11};
  
  unsigned short value[1] = {0};
  int retval = 0;
  unsigned short *p = buf;
  
  *p = 0;
  
  // First NOP to fill out the header  
  value[0] = ((unsigned short) (0x0FFF & ~CD));
  if( (retval = TCD2Cmd_ProcessCommand(TCDNOP,CD,0,value,dev)) )
    {
      // printf("TCD2Cmd::TCD2Cmd_GetConf: Error sending command: %d %d %d %x \n",TCDNOP,CD,0,value);
      return retval;
    }
  // check if it's right
  if( value[0] == ((unsigned short) (CD)) )
    *p |= (0x01 << CD );
  else
    printf("TCD2Cmd::TCD2Cmd_GetConfCD: Warning: %s concentrator not responding\n",TCD2Cmd_nodenames[CD]);
  p++;

  // Now get the configuration for CD
  for( int j = 0; j < GETCONF_PARAMS_PER_NODE; j++ )
    {
      *value = 0;
      if( (retval = TCD2Cmd_ProcessCommand(GET,CD,getconf_params[j],value,dev)) )
	{
	  // printf("TCD2Cmd::TCD2Cmd_GetConf: Error sending command %d %d %d %x\n",GET,CD,getconf_params[j],value);
	  return retval;
	}
      *p++ = *value;
    }
  
  return 0;
}

int TCD2Cmd::TCD2Cmd_ProcessCommand(unsigned int cmd, unsigned int node, unsigned int param, unsigned short * value, TCD2Usb * dev)
{
  int retval = 0;
  ddoresp = 1;

  // Assign to the data members so that we don't have to constantly
  dcmd = cmd;
  dnode = node;
  dparam = param;
  dvalue = value;
  char rawcmd[4] = {0,0,0,0};

  //printf("TCD2Cmd::TCD2Cmd_ConvertCommand: Entered\n");
  
  // If the VALUE is greater than what a 12 bit DAC will allow, set to MAX
  if( *dvalue > MAX_VALUE )
    *dvalue = MAX_VALUE;

    // Check cmd
  if( dcmd == TCDNOP )
    {
      if( (retval = checkNop()) )
	return retval;
    }
  else if( dcmd == GET )
    {
      if( ( retval = checkGet()) )
	return retval;
    }
  else if( dcmd == SET )
    {
      if( (retval = checkSet()) )
	return retval;
    }
  else if( dcmd == CLEAR ) 
    {
      ddoresp = 0;
      if( (retval = checkClr()) )
	return retval;
    }
  else if( dcmd == FLUSH )
    ddoresp = 0;
  else if( dcmd == SOFTRESET )
    ddoresp = 0;
  else if( dcmd == INIT )
    ddoresp = 0;

  // Check node 
  if( dnode < MINNODE || dnode > MAXNODE )
    return -TCD2CMD_CHECK_NODE_ERR;

  // Check param
  if( dparam < MINPARAM || dparam > MAXPARAM )
    return -TCD2CMD_CHECK_PARAM_ERR;

  // Do the conversion
  rawcmd[0] = (char) (rawcmd[0] | ((dcmd << 4) & 0x70));
  // printf("%x\n",rawcmd[0]);
  rawcmd[0] = (char) ( rawcmd[0] | (dnode & 0x0F) );
  rawcmd[1] = dparam;
  rawcmd[2] = (*dvalue & 0x0F00) >> 8;
  rawcmd[3] = *dvalue & 0x000000FF;
  
  //debug
  // if( dnode == 0 )
  //printf("TCD2Cmd_ProcessCommand: %x %x %x %x\n",rawcmd[0],rawcmd[1],rawcmd[2],rawcmd[3]);

  // Send the command
  if( (retval = dev->TCD2Usb_bulk_write_EP1OUT(rawcmd,4)) )
    return retval;

  // Send the response
  if( ddoresp )
    {
      if( (retval = dev->TCD2Usb_bulk_read_EP1IN(rawcmd,4)) )
	return retval;
      // printf("TCD2Cmd_ProcessCommand: %x %x %x %x\n",rawcmd[0],rawcmd[1],rawcmd[2],rawcmd[3]);
    }
  else
    return 0;

  *value = 0;

  // Note that since cmd, node and param are passed by value, they don't get changed
  cmd = (unsigned int) ( ( rawcmd[0] >> 4 ) & 0x0F );
  node = (unsigned int)  (rawcmd[0] & 0x0F );
  param  = (unsigned int) rawcmd[1];
  // Put the error bit in the most significant bite of value
  *value = (unsigned short)( ( ( (rawcmd[0] << 8 ) & 0x8000 ) | ( rawcmd[2] & 0x0F ) << 8 ) | (rawcmd[3] & 0x00FF ) );
  // printf("TCD2Cmd_ProcessCommand: response *value = %x\n",*value);
  //printf("TCD2Cmd::TCD2Cmd_ConvertCommand: Exiting\n");
  return 0;
}

int TCD2Cmd::checkNop()
{
  if( dnode < PXL || dnode > CD )
    return -TCD2CMD_CHECK_NOP_ERR;

  return 0;
}
  
  int TCD2Cmd::checkGet()
{
  if( dnode == ROSTACK )
    return checkGetRostack();
  
  if( dparam < THR00 || dparam > DISC3 )
    return -TCD2CMD_CHECK_PARAM_ERR;
  
  return 0;
}

int TCD2Cmd::checkSet()
{
  if( dnode == ROSTACK )
    return checkSetRostack();

  if( dparam < THR00 || dparam > DISC3 )
    return -TCD2CMD_CHECK_PARAM_ERR;

  // Check the CDHV separately
  if( ((dnode == STK_CDHV1) && (dparam == HV10)) || ((dparam == STK_CDHV2) && (dparam == HV10)) )
    if( *dvalue < 0 || *dvalue > CDHVMAX )
      return -TCD2CMD_CHECK_PARAM_ERR;

  if( dparam == HV00 )
    if( *dvalue < 0 || *dvalue > HVMAX )
      return -TCD2CMD_CHECK_HV_ERR;

  return 0;
   
}

int TCD2Cmd::checkGetRostack()
{
  if( dparam < MODE || dparam > STKMSK )
    return -TCD2CMD_CHECK_PARAM_ERR;

  return 0;
}

int TCD2Cmd::checkSetRostack()
{
  if( dparam < MODE || dparam > STKMSK )
    {
      // printf("TCD2Cmd::checkSetRostack: bad param\n");
      return -TCD2CMD_CHECK_ROSTACK_ERR;
    }
  if( dparam == MODE )
    if( !(*dvalue == INIT_MODE || *dvalue == CMD_MODE || *dvalue == EVT_MODE) )
      {
	// printf("TCD2Cmd::checkSetRostack: bad value\n");
	return -TCD2CMD_CHECK_ROSTACK_ERR;
      }

  return 0;
}

int TCD2Cmd::checkClr()
{
  if( dnode == ROSTACK )
    {
      // printf("TCD2Cmd::checkClr: bad node\n");
      return -TCD2CMD_CHECK_CLR_NODE_ERR;
    }
  if( !( (dparam != CLEAR_LT) || (dparam != CLEAR_MT) ) )
    {
      // printf("TCD2Cmd::checkClr: band param\n");
      return -TCD2CMD_CHECK_CLR_PARAM_ERR;
    }
  ddoresp = 0;
  return 0;
}

TCD2Cmd::~TCD2Cmd()
{
  ;
}
