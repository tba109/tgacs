#ifndef CRMEVENT_FAKE_H_
#define CRMEVENT_FAKE_H_
/*
 * Tyler Anderson Tue Dec 20 15:03:49 EST 2011
 * This is my specially adapted version of crmevt.h just so that I can use the
 * tcf_data struct. 
 *
 * NOTE: THIS WILL NEED TO BE REPLACED IN THE REAL CDAQ!!!!
 */

/* TCD configuration data */
#define TCF_OVRHD	22 /* size of overhead */
#define TCF_MAXSIZ	2048	/* change to 2K from 300 12/07/2011 by hyungue huh */
#define TCF_DATSIZ	(TCF_OVRHD + 2*TCF_MAXSIZ)
typedef struct {
	unsigned short length;
	unsigned short crc16;
	unsigned char  type;
	unsigned char fmt;
	unsigned short reserved;
	unsigned int serial;
  // timestamp8_t	ts8;

	unsigned short h; /* header */
	unsigned short data[TCF_MAXSIZ]; /* configuration data */
} tcf_data;

#endif
