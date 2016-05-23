#ifndef _CD_TCD_EVENT_H_
#define _CD_TCD_EVENT_H_
/*
 * Tyler Anderson Mon Dec 19 13:54:58 EST 2011
 * CD_tcd_event.h --- defines TCD event structure
 * Based on CVD_tcd-event.h
 */

/* DON'T change these, even if we use less boards */
#define CD_NUM_ANODE 2
#define CD_NUM_DYNODE 2
#define CD_NUM_PEAK 2  // Added by NBC 5/3/07
#define CD_NUM_ADC 4

/* Change THIS if we use less boards */
/* This is the test condition in a for (i=0;i<MAX_DYNODE;i++)
   so do whatever's needed.
   The board address is offset by NUM_ANODE, so dynodes start
   at "2" */
#define CD_MAX_DYNODE 0  // changed from 2 by NBC 5/3/07

typedef struct CD_tdc_data_struct
{
  unsigned char valid;
  unsigned short adc[CD_NUM_ADC];
} CD_tdc_data;

// New naming convention
typedef struct CD_tcd_event_struct
{
  unsigned char tag; /* 8-bit individual concentrator tag of event */
  unsigned int event_no; /* CREAM flight computer event # */
  CD_tdc_data anode[CD_NUM_ANODE]; /* Anode TDC data */
  CD_tdc_data dynode[CD_NUM_DYNODE]; /* Dynode TDC data */
  unsigned short peak_det[CD_NUM_ANODE][CD_NUM_ADC]; /* Peak detector data */
} CD_tcd_event;

// This is the *proper* flight configuration, with two anode boards,
// and two peak detectors, with a proper naming convention at this point.
typedef struct CD_tcd_cut_struct
{
  unsigned short anode[CD_NUM_ANODE][CD_NUM_ADC];
  unsigned short peak_det[CD_NUM_ANODE][CD_NUM_ADC];
} CD_tcd_cut;

typedef CD_tcd_event * CD_TCD_Event;
typedef CD_tdc_data * CD_TDC_Data;
typedef CD_tcd_cut * CD_TCD_Cut;

#define CD_TDC_MIN 200
//#define CD_PEAKDET_MIN 450 //12 June 2007
  #define CD_PEAKDET_MIN 0

// blanks
#define CD_TCD_EVENT_EMPTY { 0, 0, {{0,{0,0,0,0}}, {0,{0,0,0,0}}}, {{0,{0,0,0,0}}, {0,{0,0,0,0}}}, {{0,0,0,0}, {0,0,0,0}} }
#define CD_TDC_CUT_DEFAULT {CD_TDC_MIN, CD_TDC_MIN, CD_TDC_MIN, CD_TDC_MIN}
#define CD_PEAKDET_CUT_DEFAULT {CD_PEAKDET_MIN, CD_PEAKDET_MIN, CD_PEAKDET_MIN}
#define CD_TCD_CUT_DEFAULT {{CD_TDC_CUT_DEFAULT, CD_TDC_CUT_DEFAULT}, { CD_PEAKDET_CUT_DEFAULT, CD_PEAKDET_CUT_DEFAULT } }

#endif /* _CD_TCD_EVENT_H_ */
