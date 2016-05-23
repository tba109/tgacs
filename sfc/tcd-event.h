#ifndef _TCD_EVENT_H_
#define _TCD_EVENT_H_
/*
 * tcd_event.h --- defines TCD event structure
 *
 * 2004/4/1 From Patrick @ Penn. State Univ.
 *
 * Copyright (C) 2003-2004 by the CREAM project.  All rights reserved.
 * 
 * This file is part of CDAQ, the CREAM data acquisition and processing system.
 * 
 * This work may not be reproduced, displayed, modified or distributed
 * without the express prior written permission of the copyright holder.
 * For permission, contact Prof. Eun-suk Seo at seo@umd.edu.  The above
 * copyright notice must appear in all copies and in supporting documentation.
 * 
 * The software is provided without any warranty, express or implied, including
 * but not limited to the warranties of merchantability, fitness for a
 * particular purpose and noninfringement.  In no event shall the author or
 * the CREAM project group be liable for any claim, damages or other liability,
 * whether in an action of contract, tort or otherwise, arising from, out of
 * or in connection with the software or the use or other dealings in the
 * software.
 *
 * $Log: tcd-event.h,v $
 * Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
 * Importing SCDL bug fixes.
 *
 * Revision 1.1.1.1  2011/06/07 19:10:20  dja1987
 * Includes both TRD and SCDL updates
 *
 * Revision 1.1.1.1  2010/06/11 15:14:31  spaced4
 * CREAM6
 *
 * Revision 5.1  2009/03/02 04:42:02  creamop
 * Pcm Encoder and TDRSS w/o SEI
 *
 * Revision 5.0  2009/02/02 03:05:22  creamop
 * S.L 5.2 and FC3 both.
 *
 * Revision 1.1.1.1  2009/02/02 03:01:13  creamop
 * hahaha
 *
 * Revision 5.0  2009/01/24 04:33:25  creamop
 * S.L 5.2 Version
 *
 * Revision 1.1.1.1  2009/01/24 04:30:19  creamop
 * hahaha
 *
 * Revision 5.0  2009/01/24 03:48:23  creamop
 * S.L 5.2 Version.
 *
 * Revision 1.1.1.1  2009/01/24 03:44:33  creamop
 * hahaha
 *
 * Revision 4.0  2009/01/20 08:13:50  creamop
 * CREAM IV flight version.
 *
 * Revision 1.1.1.1  2009/01/20 08:12:09  creamop
 * hahaha
 *
 * Revision 1.1.1.1  2005/02/04 05:02:17  daq
 * initial version for CREAM2.
 *
 * Revision 1.3  2004/04/16 15:26:49  syzinn
 * Updated.
 *
 * Revision 1.2  2004/04/15 21:03:07  syzinn
 * added copyright notice.
 *
 * Revision 1.1  2004/04/02 13:22:53  syzinn
 * Added TCD interface.
 *
 */

/* DON'T change these, even if we use less boards */
#define NUM_ANODE 2
#define NUM_DYNODE 2
#define NUM_ADC 4

/* Change THIS if we use less boards */
/* This is the test condition in a for (i=0;i<MAX_DYNODE;i++)
   so do whatever's needed.
   The board address is offset by NUM_ANODE, so dynodes start
   at "2" */
#define MAX_DYNODE 2

typedef struct tdc_data_struct
{
  unsigned char valid;
  unsigned short adc[NUM_ADC];
} tdc_data;

/*
 *
 * eClient v2.0:
 * - adjusted moronic naming convention
 *   from the structure's point of view, nothing changed!
 *
 */

/* OLD NAMING CONVENTION
typedef struct tcd_event_struct
{
  unsigned char tag; 
  unsigned int event_no; 
  tdc_data anode[NUM_ANODE]; 
  tdc_data dynode[NUM_DYNODE]; 
  unsigned short peak_det[NUM_ADC]; 
  unsigned short csa[NUM_ADC];
} tcd_event;
*/

// New naming convention
typedef struct tcd_event_struct
{
  unsigned char tag; /* 8-bit individual concentrator tag of event */
  unsigned int event_no; /* CREAM flight computer event # */
  tdc_data anode[NUM_ANODE]; /* Anode TDC data */
  tdc_data dynode[NUM_DYNODE]; /* Dynode TDC data */
  unsigned short peak_det[NUM_ANODE][NUM_ADC]; /* Peak detector data */
} tcd_event;

// This is the *proper* flight configuration, with two anode boards,
// and two peak detectors, with a proper naming convention at this point.
typedef struct tcd_cut_struct
{
  unsigned short anode[NUM_ANODE][NUM_ADC];
  unsigned short peak_det[NUM_ANODE][NUM_ADC];
} tcd_cut;

typedef tcd_event * TCD_Event;
typedef tdc_data * TDC_Data;
typedef tcd_cut * TCD_Cut;

#define TDC_MIN 200
#define PEAKDET_MIN 450 //12 June 2007 PGB
//#define PEAKDET_MIN  0 // 02 Jul 2007

// blanks
#define TCD_EVENT_EMPTY { 0, 0, {{0,{0,0,0,0}}, {0,{0,0,0,0}}}, {{0,{0,0,0,0}}, {0,{0,0,0,0}}}, {{0,0,0,0}, {0,0,0,0}} }
#define TDC_CUT_DEFAULT {TDC_MIN, TDC_MIN, TDC_MIN, TDC_MIN}
#define PEAKDET_CUT_DEFAULT {PEAKDET_MIN, PEAKDET_MIN, PEAKDET_MIN}
#define TCD_CUT_DEFAULT {{TDC_CUT_DEFAULT, TDC_CUT_DEFAULT}, { PEAKDET_CUT_DEFAULT, PEAKDET_CUT_DEFAULT } }

#endif /* _TCD_EVENT_H_ */
