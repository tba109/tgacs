#ifndef _HTCD2_
#define _HTCD2_

#include "TCD2Usb.h"
#include "TCD2Cmd.h"
#include "TCD2Evt.h"
#include "crmevt_fake.h" // TODO: replace this with the real header

/*
 * Tyler Anderson Tue Dec 13 10:28:29 EST 2011
 * This is the tcd2 USB class for the CREAM CDAQ
 */
class HTCD2
{
 public:
  //////////////////////////////////////////////////////////////////////////////
  // Default constructor
  //////////////////////////////////////////////////////////////////////////////
  HTCD2();

  /////////////////////////////////////////////////////////////////////////////
  // Default destructor
  /////////////////////////////////////////////////////////////////////////////
  virtual ~HTCD2();

  /////////////////////////////////////////////////////////////////////////////
  // Open the USB routines and libraries
  // Returns    0 : good
  //           <0 : error code
  /////////////////////////////////////////////////////////////////////////////
  int open(int vendor = 0x04b4, int product = 0x0098);

  /////////////////////////////////////////////////////////////////////////////
  // Close all the USB routine stuff
  // Returns    0 : good
  //           <0 : error code
  /////////////////////////////////////////////////////////////////////////////
  int close();

  /////////////////////////////////////////////////////////////////////////////
  // Reopen the USB connection
  // Returns    0 : good
  //           <0 : error code
  /////////////////////////////////////////////////////////////////////////////
  int reopen(int vendor = 0x04b4, int product = 0x0098);
  
  /////////////////////////////////////////////////////////////////////////////
  // Clear the TCD master trigger
  /////////////////////////////////////////////////////////////////////////////
  int clear_event();
  
  /////////////////////////////////////////////////////////////////////////////
  // Clear the TCD master trigger
  /////////////////////////////////////////////////////////////////////////////
  int clear_timeout();

  /////////////////////////////////////////////////////////////////////////////
  // Send the flush command, and then clear
  /////////////////////////////////////////////////////////////////////////////
  int flush_buffer();

  ////////////////////////////////////////////////////////////////////////////
  // Check a commands, send it, and read back the response
  // Parameters unsigned int cmd: the numerical command value
  //            unsigned int node, the numerical node number
  //            unsigned int param, the numerical parameter number
  //            unsigned short * value, pointer to the data value, used for
  //                 a SET and readback from a GET
  // Returns    0 : good
  //           <0 : error code
  ////////////////////////////////////////////////////////////////////////////
  int tcd2_execute(unsigned int cmd, unsigned int node, unsigned int param, unsigned short * value);

  // Reserved for future use
  int tcd2_execute(char * string_one_cmd, unsigned *value);

  // Reserved for future use
  int tcd2_execute(char *file_name, unsigned int *values, int number_of_lines);

  
  ///////////////////////////////////////////////////////////////////////////
  // Get the configuration data and update it to the tcf structures
  // Note that the CD is handled separately, as a request from the UMD CDAQ
  //      development team, so that interfacing routines do not have to
  //      be changed between the TCD and the TCD2
  // Parameters tcf_data &tcf_for_tcd, the TCD config (not including CD)
  //            tcf_data &tcf_for_cd, the CD config
  // Returns    0 : good
  //           <0 : error code
  //////////////////////////////////////////////////////////////////////////
  int tcd2_getconf(tcf_data &tcf_for_tcd, tcf_data &tcf_for_cd);
  
  ////////////////////////////////////////////////////////////////////////////
  // Readout TCD data and store the results in the character buffers
  // Parameters int, number of data from 8 concentrators and 1 s3
  //            unsigned char *, data from 8 concentrators and 1 s3
  //            int *, number of data from 1 cd
  //            unsigned char *, data from 1 cd
  // Returns    0 good
  //           <0 error code
  ////////////////////////////////////////////////////////////////////////////
  int tcd2_readout(int *count_tcddata, unsigned char *tcd_buf, int *count_cddata, unsigned char *cd_buf);

  /////////////////////////////////////////////////////////////////////////////
  // Sat Jan  7 11:07:42 EST 2012
  // This was added for ground diagnosics
  // Parameters unsigned char *, data buffer as it comes across the USB
  // Returns 0 good
  //        <0 error code
  ////////////////////////////////////////////////////////////////////////////
  int tcd2_readout_gse(unsigned char * buf);

  //////////////////////////////////////////////////////////////////////////////
  // USB initialization. Wrapper to the USB initialization function
  // Parameters unsigned int vendor, vendor ID number (0x04b4 in development)
  //            unsigned int product, product ID number (0x0098 in development)
  // Returns    0 everything worked okay
  //           -1 there was an error
  /////////////////////////////////////////////////////////////////////////////
  int tcd2_initialization(unsigned int vendor=0x04b4, unsigned int product=0x0098);
  
  /////////////////////////////////////////////////////////////////////////////
  // USB shutdown
  // Returns   0 shutdown successful
  // Returns  -1 shutdown unsuccessful
  ////////////////////////////////////////////////////////////////////////////
  int tcd2_shutdown();

  ////////////////////////////////////////////////////////////////////////////
  // Tue Jan 10 14:48:37 EST 2012
  // Wrapper for reinitializing the TCD USB board
  // Parameters const char * path, path for FX2LP firmware
  //            unsigned int vendor, vendor ID number (0x04b4 in development)
  //            unsigned int product, product ID number (0x0098 in development)
  // Returns    0 everything worked okay
  //           -1 there was an error
  ///////////////////////////////////////////////////////////////////////////
  int tcd2_fx2lp_reset(const char * path = "../../tcd_usb_development_Mar20_2012_private/development_firmware/tcd2_main.ihx", unsigned int vendor=0x04b4, unsigned int product=0x0098);

  ////////////////////////////////////////////////////////////////////////////
  // Data members
  ////////////////////////////////////////////////////////////////////////////
  TCD2Usb dev;
  TCD2Cmd cmdproc;
  TCD2Evt evtproc;
};

#endif // _TCD2_
