// Draw PD 0 channel for 16 "top" tubes.  Cut events without at TDC signal
// TBA


void PD_single_tile_only()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetPaperSize(20, 24);
  gStyle->SetTitleBorderSize(1);
  gStyle->SetTitleSize(0.06, "");
  gStyle->SetTitleX(0.25f);  // Center title
  gStyle->SetTitleW(0.5f);  // Center title

  char draw[256], cut[256], name[256], title[256];

  TCanvas *c1 = new TCanvas("c1","c1",600,800);
  c1->Divide(2,2);
  TH1F *h[16];
  
  int tube;
  
  // Look at tubes 1, 4, 10, 15

  printf("\n");
    
  sprintf(title, "Tube %d: PD 0", 1);
  sprintf(name, "h_%d", 1);
  printf("Processing %s: %s\n",title,name);
  h[1] = new TH1F("h_1", "Tube 1: PD0", 50, 0, 4095);
  h[1] = new TH1F("h_5", "Tube 5: PD0", 50, 0, 4095);
  h[1] = new TH1F("h_10", "Tube 10: PD0", 50, 0, 4095);
  h[1] = new TH1F("h_14", "Tube 14: PD0", 50, 0, 4095);

  /*
  TCut cut1 = "fPD[0][1][0] > 450 && fPD[0][1][0]<4095 && fValid[0][1][0] == 1 && fTDC[0][1][0] > 200";
  TCut cut2 = "fPD[2][0][0] > 450 && fPD[2][0][0]<4095 && fValid[2][0][0] == 1 && fTDC[2][0][0] > 200";
  TCut cut3 = "fPD[5][0][0] > 450 && fPD[5][0][0]<4095 && fValid[5][0][0] == 1 && fTDC[5][0][0] > 200";
  TCut cut4 = "fPD[7][1][0] > 450 && fPD[7][1][0]<4095 && fValid[7][1][0] == 1 && fTDC[7][1][0] > 200";
  */

  TCut cut1 = "fPD[0][1][0] > 450 && fValid[0][1][0] == 1 && fTDC[0][1][0] > 400";
  TCut cut2 = "fPD[2][1][0] > 450 && fValid[2][1][0] == 1 && fTDC[2][1][0] > 400";
  TCut cut3 = "fPD[5][0][0] > 450 && fValid[5][0][0] == 1 && fTDC[5][0][0] > 400";
  TCut cut4 = "fPD[7][0][0] > 450 && fValid[7][0][0] == 1 && fTDC[7][0][0] > 400";
  //TCut cut5 = "fValid[0][0][0] == 0 && fValid[1][0][0] == 0 && fValid[1][1][0] == 0 && fValid[2][0][0] == 0";
  //TCut cut6 = "fValid[2][0][0] == 0 && fValid[3][0][0] == 0 && fValid[3][1][0] == 0 && fValid[4][0][0] == 0";
  //TCut cut7 = "fValid[4][1][0] == 0 && fValid[5][1][0] == 0 && fValid[6][0][0] == 0 && fValid[6][1][0] == 0";
  //TCut cut8 = "fValid[7][1][0] == 0";
  
  TCut cut5 = "1";
  TCut cut6 = "1";
  TCut cut7 = "1";
  TCut cut8 = "1";
  
  TCut cutTotal = cut1 && cut2 && cut3 && cut4 && cut5 && cut6 && cut7 && cut8;

  char draw[256];
  c1->cd(1);
  sprintf(draw,"(fPD[0][1][0])>>h_%d", 1);
  t->Draw(draw, cutTotal);

  c1->cd(2);
  sprintf(draw,"(fPD[2][1][0])>>h_%d", 5);
  t->Draw(draw, cutTotal);

  c1->cd(3);
  sprintf(draw,"(fPD[5][0][0])>>h_%d", 10);
  t->Draw(draw, cutTotal);

  c1->cd(4);
  sprintf(draw,"(fPD[7][0][0])>>h_%d", 14);
  t->Draw(draw, cutTotal);

  //c1->Print("PD_muon.pdf","pdfPortrait");

  TCanvas * c2 = new TCanvas();
  c2->Divide(2,2);

  TH1F *h2[16];
  
  int tube;
  
  // Look at tubes 1, 4, 10, 15
  // This time without cuts
  printf("\n");
    
  sprintf(title, "Tube %d: PD 0", 1);
  sprintf(name, "h2_%d", 1);
  printf("Processing %s: %s\n",title,name);
  h[1] = new TH1F("h2_1", "Tube 1: PD0", 100, 0, 4095);
  h[1] = new TH1F("h2_5", "Tube 5: PD0", 100, 0, 4095);
  h[1] = new TH1F("h2_10", "Tube 10: PD0", 100, 0, 4095);
  h[1] = new TH1F("h2_14", "Tube 14: PD0", 100, 0, 4095);
  
  c2->cd(1);
  sprintf(draw,"(fPD[0][1][0])>>h2_%d", 1);
  t->Draw(draw, cut1);

  c2->cd(2);
  sprintf(draw,"(fPD[2][1][0])>>h2_%d", 5);
  t->Draw(draw, cut2);

  c2->cd(3);
  sprintf(draw,"(fPD[5][0][0])>>h2_%d", 10);
  t->Draw(draw, cut3);

  c2->cd(4);
  sprintf(draw,"(fPD[7][0][0])>>h2_%d", 14);
  t->Draw(draw, cut4);


    

}
