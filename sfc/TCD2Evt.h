/*
 * Tyler Anderson Fri Dec 16 11:18:29 EST 2011
 * This is the class for event processing, including separating it into tcd and cd 
 * parts, as requested by the UMD CDAQ developers. This is so that routines interfacing
 * to the TCD do not have to be changed very much
 */
#ifndef _TCD2EVT_H_
#define _TCD2EVT_H_

#include "tcd-event.h"
#include "cd_tcd-event.h"
#include "tcd2_defs.h"
#include "TCD2Usb.h"

#define NUM_RAW_BYTES 440 // number of raw bytes from USB
#define NUM_TCD_NODES 9

// This is for the TCD event data
typedef struct TCD2Evt_struct_t {
  int id;
  char * name;
  tcd_event currevent;
  tcd_cut cut;
} TCD2Event;

// This is for the CD event
typedef struct CD_TCD2Evt_struct_t { 
  int id;
  char * name;
  CD_tcd_event currevent;
  CD_tcd_cut cut;
} CD_TCD2Event;

class TCD2Evt
{
 public:
  //////////////////////////////////////////////////////////////////
  // Default constructor
  //////////////////////////////////////////////////////////////////
  TCD2Evt();

  /////////////////////////////////////////////////////////////////
  // Default destructor
  /////////////////////////////////////////////////////////////////
  virtual ~TCD2Evt();

  ////////////////////////////////////////////////////////////////
  // Convert the incoming event to the proper format
  // Parameters int nbytes, number of bytes to read in
  //            unsigned char * bufin, incoming buffer
  //            int * count_tcddata, number of bytes from TCD
  //            unsigned char * tcd_buf, TCD event buffer
  //            int * count_cd, number of bytes from CD
  //            unsigned char * cd_buf, number of butes from CD
  //            TCD2Usb * dev, the TCD2 USB device to read from
  // Returns    0 : good
  //           <0 : error code
  ////////////////////////////////////////////////////////////////
  int TCD2Evt_ProcessEvent(int nbytesin, unsigned char *bufin, int * count_tcddata, unsigned char * tcd_buf, int *count_cd, unsigned char * cd_buf, TCD2Usb * dev);

 private:
  //////////////////////////////////////////////////////////////
  // Initilaize the data members
  //////////////////////////////////////////////////////////////
  void initialize();

  ///////////////////////////////////////////////////////////////
  // Fill the data into tcd2_cream and CD_tcd2_cream
  // Parameters int nbytes, number of bytes in buffer to convert
  //            unsigned char * bufin, buffer to convert
  // Returns    0 : good
  //           <0 : error code
  ///////////////////////////////////////////////////////////////
  int conv_buf(int nbytes, unsigned char * bufin);

  ///////////////////////////////////////////////////////////////
  // Adpated from the old eClient::tcd_pack routine
  // Perform all the operations that we used to do with the
  // TCD data, like event number voting, data stuffing, etc,
  // and then pack everything into the character buffer
  //
  // Note the format of the buffer is as follows
  // Bytes 0-1: Number of bytes in data packet
  // Bytes 2-5: event number voted on by all concentrators
  // Bytes 6-7:
  //   bits 15-12: first "good" concentrator's number of items - 1
  //   bits 11-0 : 010V VVVV VVVV, where the high nybble
  //               says this is format version 2, and the V's
  //               are a bit mask for which concentrators have
  //               "good" data and which don't, with bit position
  //               denoting concentrator id
  // Bytes 8-9     
  //   bits 3-0: second good concentrator's number of items - 1
  //   bits 7-4: third good concentrator's number of items - 1
  //   bits 11-8: etc.
  // Next avaliable 2 bytes: data for first "good" concentrator with
  //                         upper nybble giving "address" of data
  //                         value
  //
  // Similar for all the remain "good" concentrators
  // 
  // If you're using compression == 2, the next 2 bytes are timed_out
  // mask (just skipped here) and the 2 bytes after that are slip_mask
  // These are not included for compression == 1
  //  
  //
  // Last two bytes: result of CRC calc on buffer
  //
  // Parameters int * count_tcddata, number of bytes in buffer
  //            unsigned char * tcd_buf, TCD data buffer
  // Returns    0 : good 
  //           <0 : error code
  //
  // TODO: Wed Dec 21 00:38:02 EST 2011 I had to implement
  // a fake crc because I don't have the code for that. We'll
  // need to implement a real one later.
  // 
  ///////////////////////////////////////////////////////////////
  int tcd2_pack(int * count_tcddata, unsigned char * tcd_buf);

  ///////////////////////////////////////////////////////////////
  // Adapted from the old eClient::tcd_haveany routine
  // Checks to see if a given concentrator as any good data.
  // Parameters tcd_event * currevent, some event data
  //            tcd_cut * cut, the values on which to cut
  // Returns    number of items with data passing the cuts
  ///////////////////////////////////////////////////////////////
  unsigned int tcd2_haveany(tcd_event * currevent, tcd_cut * cut);
  
  ///////////////////////////////////////////////////////////////
  // Adapted from the old eClient::tcd_writevalid routine
  // Checks to see if a given data value passes cuts, and if it
  // does, writes it to the data buffer. Note that the highest
  // nybble of the 16-bit word is used as a header to define
  // what type of data it is. The high nybbles mean the following:
  // TDC00 : 0x0
  // TDC01 : 0x1
  // TDC02 : 0x2
  // TDC03 : 0x3
  // TDC10 : 0x4
  // TDC11 : 0x5
  // TDC12 : 0x6
  // TDC13 : 0x7
  // PD00  : 0x8
  // PD01  : 0x9
  // PD02  : 0xA
  // PD03  : 0xB
  // PD10  : 0xC
  // PD11  : 0xD
  // PD12  : 0xE
  // PD13  : 0xF
  //
  // Parameters tcd_event * currevent, the current event to write
  //            tcd_cut * cut, the values on which to cut
  // Returns : Number of bytes written via difference in new 
  //           pointer location and old pointer location
  ///////////////////////////////////////////////////////////////
  unsigned int tcd2_writevalid(tcd_event * currevent, unsigned short * bufP, tcd_cut * cut);

  ///////////////////////////////////////////////////////////////
  // Like tcd2_pack, but for the CD. The buffer is filled in 
  // essentially the same way
  ///////////////////////////////////////////////////////////////
  int CD_tcd2_pack(int * count_cddata, unsigned char * cd_buf);

  ///////////////////////////////////////////////////////////////
  // Like tcd2_haveany
  ///////////////////////////////////////////////////////////////
  unsigned int CD_tcd2_haveany(CD_tcd_event * currevent, CD_tcd_cut * cut);
  
  ///////////////////////////////////////////////////////////////
  // Like tcd2_writevalid
  //////////////////////////////////////////////////////////////
  unsigned int CD_tcd2_writevalid(CD_tcd_event * currevent, unsigned short * bufP, CD_tcd_cut * cut);

  //////////////////////////////////////////////////////////////
  // Initialize all the data members
  //////////////////////////////////////////////////////////////
  void intialize();

  ///////////////////////////////////////////////////////////////
  // Data members
  ///////////////////////////////////////////////////////////////
  TCD2Event tcd2_cream[NUM_TCD_NODES];
  CD_TCD2Event CD_tcd2_cream;
  unsigned int evno_array[NUM_TCD_NODES];
  unsigned int confidence_array[NUM_TCD_NODES];
  unsigned int nitems_array[NUM_TCD_NODES];
  unsigned int valid_array[NUM_TCD_NODES];
  unsigned int CD_nitems;
  unsigned int CD_valid;
  int compression;
  int CD_compression;
};

#endif //_TCD2EVTPROC_H_
