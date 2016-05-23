#include <string.h>
#include <stdio.h>
#include "TCD2Evt.h"
#include "tcd-event.h"
#include "cd_tcd-event.h"
#include "tcd2_err.h"

// Tyler Anderson Sat Dec 17 12:58:43 EST 2011
// These behavioral variables are pretty much copied from
// eClient.c
/**********************************************
 * TCD2Evt behavior variables                 *
 **********************************************/

/* With this set to zero, TCD2Evt will not try to read again
   from concentrators with bad event numbers. */
unsigned int TCD2EvtDOSYNCING = 1;
unsigned int CD_TCDEvtProcDOSYNCING = 1;
/* With this set to zero, TCD2Evt won't
   A: save events from concentrators with bad evnos (sync slipped detectors)
   B: send the mask of timed out/sync slipped concentrators with events when needed
*/
unsigned int TCD2EvtDOFMTVERSION2 = 1;
unsigned int CD_TCD2EvtDOFMTVERSION2 = 1;
/**********************************************/

// With PSU testing, write a dummy function for this purpose
// Note: Need to ask HyunGue about this
extern unsigned short calc_crc16(unsigned char *, unsigned short);

// "bad bit in an event number" mask
unsigned int evno[NUM_TCD_NODES] =
  {
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0
  };

/* "bad bit in an event number" mask */
unsigned int CD_evno = 0x0;
 
TCD2Evt::TCD2Evt()
{
  initialize();
}

void TCD2Evt::initialize()
{
  // initialize the data structures
  for( int n = 0; n < NUM_TCD_NODES; n++ )
    {
      tcd2_cream[n].id = CD;
      tcd2_cream[n].name = "cd";
      tcd2_cream[n].currevent.tag = 0;
      tcd2_cream[n].currevent.event_no = 0;

      // initilaize the event structure
      for( int i = 0; i < CD_NUM_ANODE; i++ )
	for( int j = 0; j < CD_NUM_ADC; j++ )
	  {
	    tcd2_cream[n].currevent.anode[i].adc[j] = 0;
	    tcd2_cream[n].currevent.anode[i].valid = 0;
	  }
  
      for( int i = 0; i < CD_NUM_DYNODE; i++ )
	for( int j = 0; j < CD_NUM_ADC; j++ )
	  {
	    tcd2_cream[n].currevent.dynode[i].adc[j] = 0;
	    tcd2_cream[n].currevent.dynode[i].valid = 0;
	  }
  
      for( int i = 0; i < CD_NUM_ANODE; i++ )
	for( int j = 0; j < CD_NUM_ADC; j++ )
	  tcd2_cream[n].currevent.peak_det[i][j] = 0;

      // initilaize the cut structure
      for( int i = 0; i < CD_NUM_ANODE; i++ )
	for( int j = 0; j < CD_NUM_ADC; j++ )
	  {
	    tcd2_cream[n].cut.anode[i][j] = TDC_MIN;
	    tcd2_cream[n].cut.peak_det[i][j] = PEAKDET_MIN;
	  }
    }
    
  // Handle CD separately
  CD_tcd2_cream.id = CD;
  CD_tcd2_cream.name = "cd";
  CD_tcd2_cream.currevent.tag = 0;
  CD_tcd2_cream.currevent.event_no = 0;

  // initilaize the event structure
  for( int i = 0; i < CD_NUM_ANODE; i++ )
    for( int j = 0; j < CD_NUM_ADC; j++ )
      {
	CD_tcd2_cream.currevent.anode[i].adc[j] = 0;
	CD_tcd2_cream.currevent.anode[i].valid = 0;
      }
  
  for( int i = 0; i < CD_NUM_DYNODE; i++ )
    for( int j = 0; j < CD_NUM_ADC; j++ )
      {
	CD_tcd2_cream.currevent.dynode[i].adc[j] = 0;
	CD_tcd2_cream.currevent.dynode[i].valid = 0;
      }
  
  for( int i = 0; i < CD_NUM_ANODE; i++ )
    for( int j = 0; j < CD_NUM_ADC; j++ )
      CD_tcd2_cream.currevent.peak_det[i][j] = 0;

  // initilaize the cut structure
  for( int i = 0; i < CD_NUM_ANODE; i++ )
    for( int j = 0; j < CD_NUM_ADC; j++ )
      {
	CD_tcd2_cream.cut.anode[i][j] = CD_TDC_MIN;
	CD_tcd2_cream.cut.peak_det[i][j] = CD_PEAKDET_MIN;
      }
    
  bzero(evno_array,sizeof(int)*NUM_TCD_NODES);
  bzero(confidence_array,sizeof(int)*NUM_TCD_NODES);
  bzero(nitems_array, sizeof(int)*NUM_TCD_NODES);
  bzero(valid_array, sizeof(int)*NUM_TCD_NODES);

  CD_nitems = 0;
  CD_valid = 0;

  compression = 1;
  CD_compression = 1;
  return;
}

int TCD2Evt::TCD2Evt_ProcessEvent(int nbytesin, unsigned char *bufin, int * count_tcddata, unsigned char * tcd_buf, int *count_cddata, unsigned char * cd_buf, TCD2Usb * dev)
{
  int retval = 0;
  initialize();
  
  if( (retval = dev->TCD2Usb_bulk_read_EP6IN(bufin,nbytesin)) )
    {
      // printf("TCD2Evt::TCD2Evt_ProccessEvent: Read from TCD USB device failed: %d\n",retval);
      return retval;
    }
  
  if( (retval = conv_buf(nbytesin,bufin)) )
    {
      // printf("TCD2Evt_ConvertEvent: conv_buf failed, returned %d\n", retval);
      return retval;
    }
  

  if( (retval = tcd2_pack(count_tcddata,tcd_buf)) )
    {
      // printf("TCD2Evt_ConvertEvent: tcd2_pack failed, returned %d\n",retval);
      return retval;
    }
  
  if( (retval = CD_tcd2_pack(count_cddata,cd_buf)) )
    {
      // printf("TCD2Evt_ConvertEvent: CD_tcd2_pack failed, returned %d\n",retval);
      return retval;
    }
  
  return retval;
}

/* Minimum size:
   2 bytes for number of bytes
   4 bytes for event number
   2 bytes for event header
   2 bytes for CRC */
#define TCD_EVENT_DATA_MIN_SIZE 10

#define NUM_BYTES 0
#define EVENT_NO 2
#define EVENT_HEADER 6
#define NITEMS_START 8

#define FMT_POS 9
#define NITEMS0_POS 12
#define NITEMS_LENGTH 4


int TCD2Evt::tcd2_pack(int * count_tcddata, unsigned char * tcd_buf)
{
  // This is pretty much a respinning of eClient::tcd_pack()
  unsigned short nbytes = TCD_EVENT_DATA_MIN_SIZE;
  unsigned char nvalid = 0, nitems = 0;
  unsigned int i, j, max = 0, slipcount = 0, eventno = 0;
  unsigned short valid_mask = 0;
  unsigned short * curP;
  
  // FMTVERSION2 stuff
  unsigned short slip_mask = 0;

  bzero(evno_array,sizeof(int)*NUM_TCD_NODES);
  bzero(confidence_array,sizeof(int)*NUM_TCD_NODES);
  bzero(nitems_array, sizeof(int)*NUM_TCD_NODES);
  bzero(valid_array, sizeof(int)*NUM_TCD_NODES);

  /////////////////////////////////////////////////////////////////////////
  // Vote on what the event number should be:
  // For each concentrator, walk thru to see if it's event number is in the
  // evno_array. If it isn't, add it and increase the confidence_array at
  // that spot. If it's event number is already in evno_array, just increase
  // the corresponding confidence_array entry. Then, assign the event number
  // with the max confidence_array entry as the official TCD event number.
  // slipcount holds the number of used spaces in evno_array.
  /////////////////////////////////////////////////////////////////////////

  // debug
    // printf("\n\n");
  // for( int i = 0; i < 9; i++)
  //   printf("%d ",tcd2_cream[i].currevent.event_no);
  // printf("\n\n");
    
  for( i = 0; i < NUM_TCD_NODES; i++ )
    {
      if( !evno[i] ) // Just to make sure this isn't one of the event numbers with a problem
	{
	  for( j = 0; j < slipcount; j++ ) // Check if current event number is found in evno_array
	    {
	      if( tcd2_cream[i].currevent.event_no == evno_array[j] )
		{
		  confidence_array[j]++;
		  break;
		}
	    }
	  if( j==slipcount ) // current event number not found in evno_array, so add it
	    {
	      evno_array[slipcount] = tcd2_cream[i].currevent.event_no;
	      confidence_array[j]++;
	      slipcount++;
	    }
	}
    }  

  // debug
  // printf("slipcount = %d\n",slipcount);
  // for(int i = 0; i < slipcount; i++)
  // {
  //  printf("evno_array[%d] = %d ", i, evno_array[i]);
  //  printf("confidence_array[%d] = %d\n",i,confidence_array[i]);
  //   }      
  for(j = 0; j < slipcount; j++ ) // now vote on the event number with the maximum commonality
    if( confidence_array[j] > max )
      {
	max = confidence_array[j];
	eventno = evno_array[j];
      }
     
  // debug
  // printf("EVENTNO is : %d\n\n",eventno);

  ////////////////////////////////////////////////////////////////////////////////////
  // eClient had put an resyncing routine somewhere around here. With the USB, we 
  // don't really have the option for something like that, but we can just check to
  // see if any concentrators have bad event numbers
  ///////////////////////////////////////////////////////////////////////////////////
  if( TCD2EvtDOSYNCING ) 
    {
      for( i = 0; i < NUM_TCD_NODES; i++ )
	if( (tcd2_cream[i].currevent.event_no & ~evno[i]) < (eventno & ~evno[i]) )
	  {
	    // TODO: Make this message a waning in the CDAQ
	    // printf("TCD2Evt::tcd2_pack: %s slipped event number\n",tcd2_cream[i].name);
	    if( TCD2EvtDOFMTVERSION2 )
	      slip_mask |= 1 << i; // eClient has i << 1, but I think it's a typo
	    // Looks like we have a loss of synchronization if we're here.
	  }
    }
  
  // assign the voted on event number in the proper position in the array
  *((unsigned int *) (tcd_buf+EVENT_NO)) = eventno;
  
  //////////////////////////////////////////////////////////////////////////////////
  // Do check to see if the concentrators have data above the thresholds set in
  // cuts. valid_array will have the ids of concentrators which have valid data. 
  // nitems_array will have the number of valid TDCs + PDs as it corresponds to
  // the entries in valid_array. 
  // valid mask is a mask where bit position indexes whether or not
  // the concentrator with that id was found to have ANY valid data. nvalid will
  // have the number of concentrators with valid data
  //////////////////////////////////////////////////////////////////////////////////
  for( i = 0; i < NUM_TCD_NODES; i++ )
    {
      if( (nitems = tcd2_haveany(&(tcd2_cream[i].currevent), &(tcd2_cream[i].cut))) )
	{
	  valid_array[nvalid] = i;
	  nitems_array[nvalid++] = nitems;
	  valid_mask |= 1 << i;
	}
    }
  
  /////////////////////////////////////////////////////////////////////////////////
  // Now check if we need to do the FMTVERSION2, which basically excludes
  // concentrators with bad data, so that you only write good data in the event
  // packets
  /////////////////////////////////////////////////////////////////////////////////
  if( slip_mask && TCD2EvtDOFMTVERSION2 )
    {
      compression = 2;
    }
  else
    {
      compression = 1;
    }
  
  /////////////////////////////////////////////////////////////////////////////////
  // The valid_mask should have something like "10vvvvvvvvv" in binary,  since we 
  // catch the compression == 2 case, where v is the valid bit in the corresponding 
  // bit position for that concentrator
  ////////////////////////////////////////////////////////////////////////////////
  valid_mask |= compression << FMT_POS; // #define FMT_POS 9
  
  ////////////////////////////////////////////////////////////////////////////////
  // curP jumps over the header stuff (6 bytes) in the data packet 
  ///////////////////////////////////////////////////////////////////////////////
  curP = (unsigned short *) (tcd_buf+EVENT_HEADER); // #define EVENT_HEADER 6 
  
  //////////////////////////////////////////////////////////////////////////////
  // First entry in curP is valid_mask
  /////////////////////////////////////////////////////////////////////////////
  *curP = valid_mask; // So, the valid mask is in the first position

  
  /////////////////////////////////////////////////////////////////////////////
  // Okay, so here's any example of what's going on here, assuming all items 
  // are good. Realize that nbytes starts out as 10, also realize that we use
  // nitems_array[i]-1 because if you have 16 goods, you can't fit that in 4
  // bits, so "1111" in one of the nybbles means there were 16 goods, and "0000"
  // means there was 1 good
  // i == 0
  //   increase nbytes by 2 bytes x nitems_array[i]
  //   put nitems_array[i]-1 in curP bit 12-16
  // i == 1
  //   increment curP and initialize it to 0
  //   increase nbytes by 2 bytes
  //   put nitems_array[i]-1 in curP bits 0-3
  // etc
  ////////////////////////////////////////////////////////////////////////////
  for( i = 0; i < nvalid; i++ )
    {
      // Patrick's original note here:
      /* ok, this is way complicated. */
      /* This is the same as "if (!((i-1)%4))" but we can't
	 decrement past 0 because of wraparound, so we rollover
	 the mod on the positive side.
	 this is because Nitems0 is unique, so we want to shift by one
         so now, every fourth item, we'll increment one.
      */
      if (!((i+3) % 4)) 
	{
	  /* Increment the pointer, as there's no space left in the current one */
	  curP++;
	  *curP = 0; /* 2004/7/9 Bug fix by Patrick */
	  /* We're adding 1 short to the packet */
	  nbytes += sizeof(unsigned short);
	}
      
      /* increment the total byte count */
      nbytes += nitems_array[i]*sizeof(unsigned short);
      /* If you thought the above was bad...
	 We start at the low nybble and work upwards,
	 but "0" is special and is counted as "max".
	 so "0" is written into (0+3)%4 = 3*4 = 12-16
	 "1" increments pointer
	 "1" is written into (1+3)%4 = 0*4 = 0-3
	 "2" is written into (2+3)%4 = 1*4 = 4-7
	 "3" is written into (3+3)%4 = 2*4 = 8-11
	 "4" is written into (4+3)%4 = 3*4 = 12-16
	 "5" increments pointer
	 "5" is written into (5+3)%4 = 0*4 = 0-3
	 "6" is written into (6+3)%4 = 1*4 = 4-7
	 "7" is written into (7+3)%4 = 2*4 = 8-11
	 "8" is written into (8+3)%4 = 3*4 = 12-16
	 done!
	 again we wrap around the positive side of mod
	 for safety
	 
	 It's nitems_array[i]-1 because 16 valid data
	 items won't fit in 4 bits.
      */
      *curP |= (nitems_array[i]-1) <<
	(((i+3)%4)*NITEMS_LENGTH);
    }
  curP++;
  if (compression == 2) // FMTVERSION2
    {
      *curP++ = 0; // In eClient, this is the timed_out mask, but with USB we don't
                   // look at this
      *curP++ = slip_mask;
    }
  
  // Write the data
  for( i = 0; i < nvalid; i++ )
    {
      tcd2_writevalid(&(tcd2_cream[valid_array[i]].currevent), curP, &(tcd2_cream[valid_array[i]].cut));
      curP += nitems_array[i];
    }

  *((unsigned short *) tcd_buf) = nbytes;
  *count_tcddata = nbytes;
  // TODO: get the real calc_crc16 function and implement it here
  *curP = calc_crc16(tcd_buf, nbytes-sizeof(unsigned short));
  
  return 0;
}

// Check to see if there is data in the TCD that passes cuts
unsigned int TCD2Evt::tcd2_haveany(tcd_event * currevent, tcd_cut * cut)
{
  int i, j;
  int nvalid = 0;

  // check for good anode and valid
  for( i = 0; i < NUM_ANODE; i++ )
    for( j = 0; j < NUM_ADC; j++ )
      if( (currevent->anode[i].valid & (1<<j)) && (currevent->anode[i].adc[j] > cut->anode[i][j]) )
	nvalid++;
  
  // check for good peak detector
  for( i = 0; i < NUM_ANODE; i++ )
    for( j = 0; j < NUM_ADC; j++ )
      if( currevent->peak_det[i][j] > cut->peak_det[i][j] )
	nvalid++;

  return nvalid;
	
}

// Write the valid data to the character buffer array
/* address offset of the peak detectors: 
   start at 8 (8,9,10,11 are peaks, 12,13,14,15 are csa) */
#define PEAK_OFFSET NUM_ANODE*NUM_ADC
#define SUBADDR_POS 12 /* subaddresses are written in bits 12,13,14,15 */

unsigned int TCD2Evt::tcd2_writevalid(tcd_event *currevent, unsigned short *bufP, tcd_cut *cut)
{
  unsigned short *p;
  int i, j;
  
  p=bufP;
  for (i=0;i<NUM_ANODE;i++)
    {
      for (j=0;j<NUM_ADC;j++)
	{
	  if (currevent->anode[i].valid & (1<<j) &&
	      currevent->anode[i].adc[j] > cut->anode[i][j])
	    {
	      *bufP = currevent->anode[i].adc[j];
	      *bufP |= ((i*NUM_ADC)+j) << SUBADDR_POS;
	      bufP++;
	    }
	}
    }
  for (i=0;i<NUM_ANODE;i++)
    {
      for (j=0;j<NUM_ADC;j++)
	{
	  if (currevent->peak_det[i][j] > cut->peak_det[i][j])
	    {
	      *bufP = currevent->peak_det[i][j];
	      *bufP |= (PEAK_OFFSET+(i*NUM_ADC)+j) << SUBADDR_POS;
	      bufP++;
	    }
	}
    }
  return (bufP-p);
}


//////////////////////////////////////////////////////////////////////////////////////
// Check that the CD has valid data and pack it into a character buffer
// This is in large part based on CVD_eClient.c in the original TCD CDAQ code
// This works similarly to 
//////////////////////////////////////////////////////////////////////////////////////
/* Minimum size:
   2 bytes for number of bytes
   4 bytes for event number
   2 bytes for event header
   2 bytes for CRC */
#define CD_TCD_EVENT_DATA_MIN_SIZE 10

#define CD_NUM_BYTES 0
#define CD_EVENT_NO 2
#define CD_EVENT_HEADER 6
#define CD_NITEMS_START 8

#define CD_FMT_POS 9
#define CD_NITEMS0_POS 12
#define CD_NITEMS_LENGTH 4

int TCD2Evt::CD_tcd2_pack(int * count_cddata, unsigned char * cd_buf)
{
  unsigned short nbytes=CD_TCD_EVENT_DATA_MIN_SIZE;
  unsigned char nvalid=0;
  unsigned short valid_mask=0;
  unsigned int eventno=0;
  unsigned short *curP;

  // FMTVERSION2 stuff
  CD_nitems = 0;
  CD_valid = 0;

  // No need to check event number slips or vote on event numbers--that's just plain
  // silly
  // I'm not going to do any sort of slip mask check or anything, since I'm processing
  // CD separately.
  eventno = CD_tcd2_cream.currevent.event_no;
  *((unsigned int *) (cd_buf+CD_EVENT_NO)) = eventno;
  
  if( (CD_nitems = CD_tcd2_haveany(&(CD_tcd2_cream.currevent),&(CD_tcd2_cream.cut))) )
    {
      nvalid++;
      valid_mask |= 1;
    }

  if( CD_TCD2EvtDOFMTVERSION2 )
    CD_compression = 2; // FMTVERSION2: yes
  else
    CD_compression = 1; // FMT_VERSION2: no
  valid_mask |= CD_compression << CD_FMT_POS;
  curP  = (unsigned short *) (cd_buf+CD_EVENT_HEADER);
  *curP = valid_mask;
  
  if( nvalid )
    {
      nbytes += CD_nitems*sizeof(unsigned short);
      *curP |= (CD_nitems - 1) << 3*CD_NITEMS_LENGTH;
    }
  curP++;
  if( CD_compression == 2 )
    {
      *curP++ = 0; // timed out mask
      *curP++ = 0; // slip_mask
    }
  if( nvalid )
    {
      CD_tcd2_writevalid(&(CD_tcd2_cream.currevent),curP,&(CD_tcd2_cream.cut));
      curP += CD_nitems;
    }
  *((unsigned short *) cd_buf) = nbytes;
  *count_cddata = nbytes;
  // TODO: Put the real calc_crc16 function here
  *curP = calc_crc16(cd_buf, nbytes - sizeof(unsigned short));
 
  return 0;
}

unsigned int TCD2Evt::CD_tcd2_haveany(CD_tcd_event *currevent, CD_tcd_cut *cut)
{
  int i, j;
  int nvalid = 0;
  
  for (i=0;i<CD_NUM_PEAK;i++)
    {
      for (j=0;j<CD_NUM_ADC;j++)
	{
	  if (currevent->peak_det[i][j] > cut->peak_det[i][j])
	    nvalid++;
	}
    }
  return nvalid;
}

/* address offset of the peak detectors: 
   start at 8 (8,9,10,11 are peaks, 12,13,14,15 are csa) */
   
#define CD_PEAK_OFFSET CD_NUM_ANODE*CD_NUM_ADC
#define CD_SUBADDR_POS 12 /* subaddresses are written in bits 12,13,14,15 */

unsigned int TCD2Evt::CD_tcd2_writevalid(CD_tcd_event *currevent, unsigned short *bufP, CD_tcd_cut *cut)
{
  unsigned short *p;
  int i, j;
  p = bufP;
  
  for (i=0;i<CD_NUM_PEAK;i++)
    {
      for (j=0;j<CD_NUM_ADC;j++)
	{
	  if (currevent->peak_det[i][j] > cut->peak_det[i][j])
	    {
	      *bufP = currevent->peak_det[i][j];
	      *bufP |= (CD_PEAK_OFFSET+(i*CD_NUM_ADC)+j) << CD_SUBADDR_POS;
	      bufP++;
	    }
	}
    }
  return (bufP-p);
}



int TCD2Evt::conv_buf(int nbytesin, unsigned char *bufin)
{
   // Make sure we have the right number of bytes in
  if( nbytesin != NUM_RAW_BYTES ) // raw number of data bytes
    {
      // printf("TCD2Evt::TCD2Evt_ConvertEvent: Wrong number of bytes %d, needs to be %d\n",nbytesin, NUM_RAW_BYTES);
      return -TCD2EVT_EVENT_BYTE_NUM_ERR;
    }

  // This is mostly borrowed from the gse_daq's TCD2Data::FillData routine
  int rbc = 0;
  
  // PD0, standard ordering of PXL, PXR,...S3
  for(int j = 0; j < NUM_TCD_NODES; j++)
    for(int i = 0; i < NUM_ADC; i++)
      {
	tcd2_cream[j].currevent.peak_det[0][i] = 
	  ((bufin[rbc+1] << 8) & 0x0F00) | (bufin[rbc] & 0x00FF);
	rbc+=2;
      }
  
  // PD0 CD
  for(int i = 0; i < NUM_ADC; i++)
    {
      CD_tcd2_cream.currevent.peak_det[0][i] = 
	((bufin[rbc+1] << 8) & 0x0F00) | (bufin[rbc] & 0x00FF);
      rbc+=2;
    }

  // PD1, standard ordering of PXL, PXR,...S3
  for(int j = 0; j < NUM_TCD_NODES; j++)
    {
      for(int i = 0; i < NUM_ADC; i++)
	{
	  tcd2_cream[j].currevent.peak_det[1][i] = 
	    ((bufin[rbc+1] << 8) & 0x0F00) | (bufin[rbc] & 0x00FF);
	  rbc+=2;
	}
    }

  // PD1 CD
  for(int i = 0; i < NUM_ADC; i++)
    {
      CD_tcd2_cream.currevent.peak_det[1][i] = 
	((bufin[rbc+1] << 8) & 0x0F00) | (bufin[rbc] & 0x00FF);
      rbc+=2;
    }

  // TDC0, standard ordering of PXL, PXR,...S3
  for(int j = 0; j < NUM_TCD_NODES; j++)
    {
      // Parse anodes
      for(int i = 0; i < NUM_ADC; i++)
	{
	  tcd2_cream[j].currevent.anode[0].adc[i] = 
	    ((bufin[rbc+1] << 8) & 0x0F00) | (bufin[rbc] & 0x00FF);
	  rbc+=2;
	  
	}
      // NO DYNODE BOARDS!
      // Get VALID0
      tcd2_cream[j].currevent.anode[0].valid = bufin[rbc];
      rbc+=2;
    }
  
  // Need to skip TDC0 CD stuff
  for( int i = 0; i < NUM_ADC; i++) 
    rbc+=2; // anode
  rbc+=2; // valid
  
  // TDC1, standard ordering of PXL, PXR,...S3,
  for(int j = 0; j < NUM_TCD_NODES; j++)
    {
      // Parse anodes
      for(int i = 0; i < NUM_ADC; i++)
	{
	  tcd2_cream[j].currevent.anode[1].adc[i] = 
	    ((bufin[rbc+1] << 8) & 0x0F00) | (bufin[rbc] & 0x00FF);
	  rbc+=2;
	  
	}
      // NO DYNODE BOARDS!
      // Get VALID1
      tcd2_cream[j].currevent.anode[1].valid = bufin[rbc];
      rbc+=2;
    }
    
  // Need to skip TDC1 CD stuff
  for( int i = 0; i < NUM_ADC; i++) 
    rbc+=2; // anode
  rbc+=2; // valid
  
  // Trigger stuff. Need to skip status, as it's not recorded in previous flight versions
  for(int j = 0; j < NUM_TCD_NODES; j++)
    {
      rbc+=2; // skip status
      tcd2_cream[j].currevent.tag = bufin[rbc]; // event count/tag
      rbc+=2;
      tcd2_cream[j].currevent.event_no = // Event number, ordered as LSW MSW in bufin 
	( (bufin[rbc+3] << 24) & 0xFF000000 ) |
	( (bufin[rbc+2] << 16) & 0x00FF0000 ) |
	( (bufin[rbc+1] <<  8) & 0x0000FF00 ) |
	( (bufin[rbc]        ) & 0x000000FF );
      rbc += 4;
    }
  
  // Do the CD trigger stuff
  rbc+=2; // skip status
  CD_tcd2_cream.currevent.tag = bufin[rbc]; // event count/tag
  rbc+=2;
  CD_tcd2_cream.currevent.event_no = // Event number, ordered as LSW MSW in bufin 
    ( (bufin[rbc+3] << 24) & 0xFF000000 ) |
    ( (bufin[rbc+2] << 16) & 0x00FF0000 ) |
    ( (bufin[rbc+1] <<  8) & 0x0000FF00 ) |
    ( (bufin[rbc]        ) & 0x000000FF );
  rbc += 4;

  return 0;
}

TCD2Evt::~TCD2Evt()
{
  ;
}
