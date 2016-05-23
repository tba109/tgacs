// Draw PD 0 channel for 16 "top" tubes.  Cut events without at TDC signal
// TBA


void PD0hist()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetPaperSize(20, 24);
  gStyle->SetTitleBorderSize(1);
  gStyle->SetTitleSize(0.06, "");
  gStyle->SetTitleX(0.25f);  // Center title
  gStyle->SetTitleW(0.5f);  // Center title

  char draw[256], cut[256], name[256], title[256];

  TCanvas *c1 = new TCanvas("c1","c1",600,800);
  c1->Divide(4,4);

  TH1F *h[16];
  
  int tube;
  for (int node=0; node<8; node++) {
    for (int right = 0; right < 2; right++)
      {
	printf("\n");
	tube = 2*node + right;
	c1->cd(tube+1);
		
	sprintf(title, "Tube %d: PD 0", tube);
	sprintf(name, "h_%d", tube);
	printf("Processing %s: %s\n",title,name);
	h[tube] = new TH1F(name, title, 50, 0, 4095);
	
	char draw[256], cut[256];
	sprintf(draw,"(fPD[%d][%d][0])>>h_%d", node, right,tube);
	sprintf(cut,"fPD[%d][%d][0] > 450 && fValid[%d][%d][0]==1 && fPD[%d][%d][0]<4095 && fTDC[%d][%d][0] > 200", node, right, node, right, node, right, node, right);
	// sprintf(cut,"fPD[%d][%d][0] > 450 && fPD[%d][%d][0]<4095 && fTDC[%d][%d][0] > 200", node, right, node, right, node, right, node, right);
	// sprintf(cut,"fPD[%d][%d][0] > 450 && fValid[%d][%d][0]==1 && fPD[%d][%d][0]<4095 && fTDC[%d][%d][0] > 200", node, right, node, right, node, right, node, right, node, right);
	// sprintf(cut,"fPD[%d][%d][0] > 450 && fPD[%d][%d][0]<4095", node, right, node, right, node, right, node, right);
	// sprintf(cut,"");
	
	printf("draw = %s, cut =  %s\n",draw,cut);

	t->Draw(draw, cut);
      }
  }

  //c1->Print("PD_muon.pdf","pdfPortrait");
}
