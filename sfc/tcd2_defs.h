/*
 * Tyler Anderson
 * Wed Oct  6 23:55:47 EDT 2010
 * This is the TCD command format
 */
#ifndef _TCD2_DEFS_H_
#define _TCD2_DEFS_H_

#define GETCONF_PARAMS_PER_NODE 18

/////////////////////////////////////////////////
// Target defines
/////////////////////////////////////////////////
// b7 is the recoverable error bit. Only set when FX2LP responds that the
// command has an error.
#define ERROR 0x80
#define ERRORMASK 0x80
#define ERROROFFSET 7

#define NUM_TCD_S3 9
#define NUM_ALL_NODES 10
// b6..b4 define action (GET, SET, etc.)
#define TCDNOP 0x00
#define GET 0x01
#define SET 0x02
#define INIT 0x03
#define EVENT 0x04
#define CLEAR 0x07 

#define ACTIONMASK 0x70 
#define ACTIONOFFSET 4
#define NACTIONS 4

#define NUM_ACTION 3

// Thu Dec 22 15:47:05 EST 2011
// These are other useful actions used only by FX2LP
#define SOFTRESET 0x05
#define FLUSH 0x06

// b3..b0 define which node. Target bits
#define PXL     0x00 
#define PXR     0x01 
#define PYL     0x02 
#define PYR     0x03 
#define MXL     0x04 
#define MXR     0x05 
#define MYL     0x06 
#define MYR     0x07 
#define S3      0x08 
#define CD      0x09 
#define ROSTACK 0x0A   // Readout stack
#define TCDMT PXR // CHANGELOG 2

#define NODEMASK 0x0F 
#define NODEOFFSET 0
#define NNODES 0x0B
#define NNODESDATA 0x0A


#define MINNODE 0
#define MAXNODE ROSTACK

// Tyler Anderson Thu Dec 16 14:17:36 EST 2010
// Per Nick Conklin's suggestion, add some node definitions for 
// setting CD values. These are the lines that should be changed
// if the relevant CD parameters are mapped to different stacks.

#define STK_CDHV1 MYL
#define STK_CDHV2 MYR
#define STK_CDTHR MXR 
#define CDHVMAX 4095 // Jason said no need for this
#define HVMAX 1800 // Max for XP2020's is 3kV, this is 3.1kV, so we'd run just a little
                   // north of this

//////////////////////////////////////////////////////////
// Parameter defines
/////////////////////////////////////////////////////////
#define THR00    0
#define THR01    1
#define THR02    2
#define THR03    3
#define LEVEL0   4
#define HV00     5
#define HV01     6
#define THR10    7
#define THR11    8
#define THR12    9
#define THR13    10
#define LEVEL1   11
#define HV10     12
#define HV11     13
#define DISC0    14
#define DISC1    15
#define DISC2    16
#define DISC3    17
#define CLEAR_LT 18
#define CLEAR_MT 19
#define MODE     20
#define CLEV     21
#define STKMSK   22


#define NUM_NODE_PARAMS 20

#define NPARAMS 23
#define MINPARAM 0
#define MAXPARAM STKMSK

// Here's a set of datargs that can correspond to MODE
#define INIT_MODE    0x00 // for initialization
#define CMD_MODE     0x01 // to transition to command mode
#define EVT_MODE     0x02 // to transition to event mode

// DACs are only 12 bits, so if you try to set larger, just max out
#define MAX_VALUE 4095 
#endif
