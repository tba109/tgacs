#ifndef _TCD2CMD_H_
#define _TCD2CMD_H_

#include "tcd2_defs.h"
#include "TCD2Usb.h"
#include "TCD2Cmd.h"

/* 
 * Tyler Anderson Wed Dec 14 18:20:05 EST 2011
 * This is the class for command process, including checking and conversion from a 
 * given incomming command specified as "cmd, node, param, value" to actual bytes. 
 * This includes error checking to be sure we don't set the HV's too high
 * 
 * This function also checks the response to commands, I suppose. Although, that's 
 * not quite as important right now
 */

class TCD2Cmd
{
 public:
  /////////////////////////////////////////////////////////////////////////
  // Default constructor
  /////////////////////////////////////////////////////////////////////////
  TCD2Cmd();
  
  /////////////////////////////////////////////////////////////////////////
  // Default destructor
  ////////////////////////////////////////////////////////////////////////
  virtual ~TCD2Cmd();

  ///////////////////////////////////////////////////////////////////////
  // Check the command, convert it, send it, receive the response
  // and convert it.
  ///////////////////////////////////////////////////////////////////////
  int TCD2Cmd_ProcessCommand(unsigned int cmd, unsigned int node, unsigned int param, unsigned short * value, TCD2Usb * dev );

  /////////////////////////////////////////////////////////////////////
  // Get the configuration info for the TCD
  /////////////////////////////////////////////////////////////////////
  int TCD2Cmd_GetConf(unsigned short * buf, TCD2Usb * dev);

  /////////////////////////////////////////////////////////////////////
  // Get the configuration info for the CD
  /////////////////////////////////////////////////////////////////////
  int TCD2Cmd_GetConfCD(unsigned short * buf, TCD2Usb * dev);

 private:
  int cmdtoraw();

  int rawtocmd();

  int checkNop();
  
  int checkGet();
  
  int checkSet();
  
  int checkGetRostack();
  
  int checkSetRostack();
  
  int checkClr();

  //////////////////////////////////////////////////////////////////
  // Data members
  /////////////////////////////////////////////////////////////////
  unsigned int dcmd;
  unsigned dnode;
  unsigned dparam;
  unsigned short * dvalue;
  unsigned char ddoresp; // only read the response when this is 1 otherwise don't
};

#endif // _TCD2CMDPROC_
