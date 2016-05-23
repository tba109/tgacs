/*
 * Tyler Anderson Sun Jan  8 11:05:15 EST 2012
 * This program converts the raw output file of 
 * event_gse to a ROOT tree
 *
 * Based on Nick Conklin's text2root
 */

// ROOT Headers
#include "TApplication.h"
#include "TTimeStamp.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TArc.h"
#include "TH1.h"

// System Headers
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

TROOT root("root","raw2tree");

const int NUM_PMT = 2;
const int NUM_STACKS = 10;

using namespace std;

int main(int argc, const char *argv[])
{
  string inFile, outFile;

  ///////////////////////////////////////////////////////////////////
  // Get the output file from user.  If not specified,
  // use the default
  ///////////////////////////////////////////////////////////////////
  if (argc < 3) {
    outFile = "tree.root";
  } else {
    outFile = argv[2];
  }
  
  TApplication theApp("raw2tree", NULL, NULL);
  
  //////////////////////////////////////////////////////////////////
  // Create output file
  //////////////////////////////////////////////////////////////////
  TFile tree_file(outFile.c_str(),"RECREATE");
  
  ///////////////////////////////////////////////////////////////////
  // Set up the Tree
  ///////////////////////////////////////////////////////////////////
  // Tree data
  int fPD[NUM_STACKS][NUM_PMT][4];
  int fTDC[NUM_STACKS][NUM_PMT][4];
  int fValid[NUM_STACKS][NUM_PMT][4]; // array entry => 1 bit (inefficient but easy!)
  int fStatus[NUM_STACKS][4]; // array entry => 1 bit (inefficient but easy!)
  int fEcount[NUM_STACKS];
  int fEventNo[NUM_STACKS];
  
  char temp[100];
  TTree *t = new TTree("t","");

  sprintf(temp,"fPD[%d][%d][4]/I",NUM_STACKS,NUM_PMT);
  t->Branch("fPD", fPD, temp);
  
  sprintf(temp,"fTDC[%d][%d][4]/I",NUM_STACKS,NUM_PMT);
  t->Branch("fTDC", fTDC, temp);
  
  sprintf(temp,"fValid[%d][%d][4]/I",NUM_STACKS,NUM_PMT);
  t->Branch("fValid", fValid, temp);
  
  sprintf(temp,"fStatus[%d][4]/I",NUM_STACKS);
  t->Branch("fStatus", fStatus, temp);
  
  sprintf(temp,"fEcount[%d]/I",NUM_STACKS);
  t->Branch("fEcount", fEcount, temp);

  sprintf(temp,"fEventNo[%d]/I",NUM_STACKS);
  t->Branch("fEventNo", fEventNo, temp);  
  
  // Get number of lines in text file
  int nlines=0;
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  
  // Get the input file from user.  If not specified,
  // use the default
  if (argc < 2) 
    inFile = "data.txt";
  else 
    inFile = argv[1];
    
  fp = fopen(inFile.c_str(),"r");
  if (fp == NULL) {
    printf("Failure opening file '%s'\n",inFile.c_str());
    exit(EXIT_FAILURE);
  }  
  while ((read = getline(&line, &len, fp)) != -1) nlines++;
  
  printf("Reading '%s'\n",inFile.c_str());

  // Have to "rewind" the file
  rewind(fp);

  // Loop over lines in text file  
  int dummy = 0;
  for (int l = 0; l < nlines; l++) 
    {   
      // PD0
      for(int j = 0; j < NUM_STACKS; j++)
	for(int i = 0; i < 4; i++)
	  {
	    fscanf(fp,"%d ", &fPD[j][0][i]);
	    // printf("%d ", fPD[j][0][i]);
	  }

      // PD1
      for(int j = 0; j < NUM_STACKS; j++)
	for(int i = 0; i < 4; i++)
	  {
	    fscanf(fp,"%d ", &fPD[j][1][i]);
	    // printf("%d ", fPD[j][1][i]);
	  }
      
      // TDC0
      for(int j = 0; j < NUM_STACKS; j++)
	{
	  for(int i = 0; i < 4; i++)
	    {
	      fscanf(fp,"%d ", &fTDC[j][0][i]);
	      // printf("%d ", fTDC[j][0][i]);
	    }
	  fscanf(fp,"%d ", &dummy);
	  // printf("%d ", dummy);
	  fValid[j][0][0] = dummy & 0x1;
	  fValid[j][0][1] = (dummy & 0x2) >> 1;
	  fValid[j][0][2] = (dummy & 0x4) >> 2;
	  fValid[j][0][3] = (dummy & 0x8) >> 3;
	}
      
      // TDC1
      for(int j = 0; j < NUM_STACKS; j++)
	{
	  for(int i = 0; i < 4; i++)
	    {
	      fscanf(fp,"%d ", &fTDC[j][1][i]);
	      // printf("%d ",fTDC[j][1][i]);
	    }	  
	  fscanf(fp,"%d ", &dummy);
	  // printf("%d ", dummy);
	  fValid[j][1][0] = dummy & 0x1;
	  fValid[j][1][1] = (dummy & 0x2) >> 1;
	  fValid[j][1][2] = (dummy & 0x4) >> 2;
	  fValid[j][1][3] = (dummy & 0x8) >> 3;
	}
      
      // Trigger stuff
      for( int j = 0; j < NUM_STACKS; j++ )
	{
	  fscanf(fp,"%d ",&dummy);
	  // printf("%d ",dummy);
	  fStatus[j][0] = (dummy & 0x1);
	  fStatus[j][1] = (dummy & 0x2) >> 1;
	  fStatus[j][2] = (dummy & 0x4) >> 2;
	  fStatus[j][3] = (dummy & 0x8) >> 3;
	  
	  fscanf(fp,"%d ",&fEcount[j]);
	  // printf("%d ",fEcount[j]);
	  
	  fscanf(fp,"%d ",&fEventNo[j]);
	  // printf("%d ",fEventNo[j]);
	}
      fscanf(fp,"\n");
      // printf("\n");
      t->Fill(); 
    }
  
  tree_file.Write();
  tree_file.Close();
  
  cout << "Data written to '" << outFile << "'" << endl;
  
  return 0;
  
  
}
