{
// monster macro containing all useful functionality

//  Matt Fritts
//  MCF version
//  (for RMS version namepref should be "d:/root/user/" and psdpref should be "f:/")
//  Initializing routine for Root session 

//  initialization is done just once, while macro is used repeatedly
int initdone;
if(initdone!=1){
   gROOT->Reset();
   int initdone=1;
   printf("initializing\n");

   char psdpref[30],namepref[50],namesuff[30];
//   strcpy(psdpref,"c:/mike/");
   strcpy(psdpref,"c:/Rob Learning Stuff/2019-06-24/");
   strcpy(namepref,"c:/Rob Learning Stuff/Root Outputs/");
   strcpy(namesuff,".root");

 //  namepref determines where macro will look to find ntuple files
 //  and where spectra and header files will be saved
 //  psdpref determines where macro will look for raw data

   Float_t c0,c1,c2,c3; 
   c0=-6.791; 
   c1=302.18;
   c2=-71.80;
   c3=341.23;

   Float_t mono_offset=0.0;

// default calibration parameters above. These can be changed with with the NTUP2HIST macros.
// SPECFIT and DECFIT record these with fit information. If the values are changed any time 
// between the creation or loading of a histogram and a fit to that histogram,
// incorrect values will be recorded.
//   c0=-.680; 
//   c1=150.61;
//   c2=-25.8;
//   c3=243;
// above are appr. params for "Artest" datasets

   int vbot,vtop,vsbot,vstop;
   vbot=0;// unfiltered=0
   vtop=4094;// unfiltered=4095
   vsbot=0;// unfiltered=0
   vstop=8190;// unfiltered=8190
   int k1,k2;
   k1=12;// uncorrected=0
   k2=21;// uncorrected=0

// default raw data filtering and correcting criteria above

   gStyle.SetOptStat(11);
   gStyle->SetOptFit(1111);

 //  above sets info appearing when spectra are drawn

   Int_t hx=0,cx=-10,cy=-20;  
   struct HP{
    TH1D *hh;
    char hhn[30];
    TCanvas *hhc;
    TPaveText hhf[10];
    int hhnf;
    TLine hhl[2];
   }histp[90];

 //  The structured array of variables histp[n] packages a spectrum or decay curve with
 //  its name, canvas, fit information, number of saved fits, and limit lines (these are 
 //  2 vertical lines which can be moved with the mouse to set limits). The integer hx
 //  keeps track of how many 1d histos have been created and sets the index of histp[n]

   int sumx=0;TH1D hsum[70];

   Int_t mhx=0;
   struct HP2D{
    TH2F *mh;
    char mhn[30];
   }mhp[30];

   int comx=0;TH2F hcom[30];
   TH2F *hmm[30];

   int normx=0;TH1D hnorm[70];
   
}

int macchoice;

printf("\nThe following are preliminary operations:\n");
//printf("  1: PSD data -> HEADER file                                   1:psd2head.c\n");
//printf("  2: PSD data -> NTUPLE file                                   2:psd2ntup.c\n");
printf("  3: Raw data -> OM/FC plot                                    3:monitor.c\n");
printf("  4: Raw data -> GENERAL INFO                                  4:raw2info.c\n");
//printf("  5: New CALIBRATION                                           5:calib.c\n");
printf("The following operations create histograms:\n");
//printf("  6: Single-position ntuple         -> SPECTRUM                6:ntup2hist1d.c\n");
//printf("  7: Multi-position ntuple          -> 2D HISTOGRAM            7:ntup2hist2d.c\n");
//printf("  8: Several multi-position ntuples -> 2D HISTOGRAM            8:ntups2hist2d.c\n");
//printf("  9: 2d histo -> SPECTRUM                                      9:poscut.c\n");
//printf(" 10: 2d histo -> DECAY CURVE                                  10:lamcut.c\n");
printf(" 11: PMT data -> SPECTRUM or DECAY CURVE                      11:pmt2hist.c\n");
printf(" 12: OPEN a saved histogram                                   12:openhist.c\n");
printf("The following operations manipulate histograms:\n");
printf(" 13: ZOOM                                                     13:histzoom.c\n");
printf(" 14: FIT a SPECTRUM                                           14:specfit.c\n");
printf(" 15: FIT a DECAY CURVE                                        15:decfit.c\n");
printf(" 16: View FIT INFORMATION                                     16:fitinfo.c\n");
printf(" 17: NORMALIZE/CORRECT a histogram                            17:histnorm.c\n");
printf(" 18: ADD two histograms and view                              18:histsum.c\n");
printf(" 19: SAVE a histogram                                         19:histsave.c\n");
printf("Enter option: ");
scanf("%d",&macchoice);
printf("\n");


switch (macchoice){
 case 6://=======================================================================================
//   ntup2hist1d.c                                                                  ntup2hist1d.c
//   take a single position PSD ntuple and create master histogram (1D)
//   then draw spectrum
//   matt fritts
//////////////////////////////////////////////////////////

   Float_t posn;
   char namebit[50], nameroot[50], hmtitle[60], yesorno[10], nm[30]; 
   char hpname[30],cpname[30],hplabel[50],fid[5];
     
//   interrogate user for data file name
   
   printf("Enter desired dataset name(I'll get the ntuple): ");
   scanf("%s",namebit);
   sprintf(nameroot,"%s%s%s",namepref,namebit,namesuff);  

//   open Root file containing ntuple, produced by psd2ntup.c
   TFile *fin = (TFile*)gROOT->GetListOfFiles()->FindObject(nameroot);
   if (!fin) {
      printf("retrieving file...\n");
      fin = new TFile(nameroot);
   }
   else printf("already open...\n");
   
//   set directory name (probably not necessary here, but necessary in ntup(s)2hist2d.c
   sprintf(nm,"%sf",namebit);
   fin.SetName(nm);
   
   TTree *ntuple = (TTree*)gDirectory->Get("ntuple");
//   gets ntuple from active directory

//Declaration of leaves types
   Float_t         lam;
   Float_t         pos,pos0,pos1;
   Float_t         sig;
   Float_t         om;
   Float_t         fc;
   Float_t         time;
   Float_t         v1;
   Float_t         v2;
   Float_t         alpha;   
   Float_t         lambda;

//Set branch addresses
   ntuple->SetBranchAddress("lam",&lam);
   ntuple->SetBranchAddress("pos",&pos);
   ntuple->SetBranchAddress("sig",&sig);
   ntuple->SetBranchAddress("om",&om);
   ntuple->SetBranchAddress("fc",&fc);
   ntuple->SetBranchAddress("time",&time);
   ntuple->SetBranchAddress("v1",&v1);
   ntuple->SetBranchAddress("v2",&v2);
   ntuple->SetBranchAddress("alpha",&alpha);  
                                                                                 // ntup2hist1d.c             
   strcpy(fid,"");
   printf("Filtering criteria: v>=%d, v<=%d, sum>=%d, sum<=%d\n",vbot,vtop,vsbot,vstop);
   printf("Would you like to change these? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
    printf("Enter bottom limit for v1 and v2 (0=unfiltered): ");
    scanf("%d",&vbot);
    printf("Enter top limit for v1 and v2 (4095=unfiltered): ");
    scanf("%d",&vtop);
    printf("Enter bottom limit for sum of v1 and v2 (0=unfiltered): ");
    scanf("%d",&vsbot);
    printf("Enter top limit for sum of v1 and v2 (8190=unfiltered): ");
    scanf("%d",&vstop);
    printf("Enter identifier for histogram: ");
    scanf("%s",fid);
   }
   printf("Offset criteria: %d subtracted from v1, %d from v2\n",k1,k2);
   printf("Would you like to change these? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
    printf("Enter constant subtracted from v1 (0=uncorrected): ");
    scanf("%d",&k1);
    printf("Enter constant subtracted from v2 (0=uncorrected): ");
    scanf("%d",&k2);
    printf("Enter identifier for histogram: ");
    scanf("%s",fid);
   }

// define formula to calculate "lambda"
   printf("Calibration parameters: c0=%.3f, c1=%.3f, c2=%.3f, c3=%.3f\n",c0,c1,c2,c3);
   printf("Would you like to change these? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
     printf("Enter new c0: ");
     scanf("%f",&c0);
     printf("Enter new c1: ");
     scanf("%f",&c1);
     printf("Enter new c2: ");
     scanf("%f",&c2);
     printf("Enter new c3: ");
     scanf("%f",&c3);
   }
   TFormula *lamform=new TFormula("lamform","y+[0]+[1]*(x-.5)+[2]*(x-.5)^2+[3]*(x-.5)^3");
//  "x" in formula is "alpha", "y" in formula is "lam"
//  Formula essentially redefines alpha to represent center of detector
   lamform.SetParameter(0,c0);
   lamform.SetParameter(1,c1);
   lamform.SetParameter(2,c2);
   lamform.SetParameter(3,c3);

// set histogram parameters
   Axis_t startx,stopx,stepx;
   Axis_t starty,stopy,stepy;
   Int_t nbinsx,nbinsy;

// check to see if this is a multiple-position histogram by comparing first pos
// to the value of pos sig events later. 
   ntuple.GetEvent(0);
   pos0 = pos;
   ntuple.GetEvent(sig);
   pos1 = pos;
   if(pos1!=pos0){
     printf("This is a multiple foil position dataset.\n");
     printf("Use ntup2hist2d.c instead.\n");
     break;
   }
   posn = pos0;
                                                                                 // ntup2hist1d.c             
// find min and max wavelength, rounded to angstrom
// (min rounded down, max rounded up)
   printf("getting minima and maxima...\n");
   startx = int(lamform.Eval(ntuple.GetMinimum("alpha"),ntuple.GetMinimum("lam")));
   stopx = int(lamform.Eval(ntuple.GetMaximum("alpha"),ntuple.GetMaximum("lam")))+1.0;
   stepx = .2;
   
   printf("wavelength min:%.2f max:%.2f bin size:%.2f\n",startx,stopx,stepx);

   printf("Would you like to change histogram wavelength parameters? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
     printf("Enter new minimum wavelength: ");
     scanf("%f",&startx);
     printf("Enter new maximum wavelength: ");
     scanf("%f",&stopx);
     printf("Enter wavelength resolution: ");
     scanf("%f",&stepx);
   }

   printf("position: %.2f\n",posn);
   
   nbinsx=(int)((stopx-startx)/stepx+.5);
   stopx=startx+int((stopx-startx)/stepx+.5)*stepx;   
// last line insures that bin sizes are preserved by insuring max differs from min by 
// an integral number of steps

// abort option
   printf("Enter \"a\" to abort, anything else to continue: ");
   scanf("%s",yesorno);
   if(yesorno[0]=='a')break;
  
// name and title histogram and name canvas

   sprintf(hpname,"%s%sp%d",namebit,fid,int(posn*10+.5));
   sprintf(cpname,"%sc",hpname);
   sprintf(hplabel,"%s%s, Spectrum @ position = %.1f",namebit,fid,posn);
                                                                                 // ntup2hist1d.c             
// define histogram for spectrum
   printf("creating 1D histogram \"%s\"...\n",hpname);

   hx++;
   
   histp[hx].hh = new TH1D(hpname,hplabel,nbinsx,startx,stopx);
// set name in structure and initialize number of fits
   strcpy(histp[hx].hhn,hpname); 
   histp[hx].hhnf=0;
	
   Int_t nentries = ntuple->GetEntries();

   int nacc=0;

   Int_t nbytes = 0;
   printf("percent finished:    ");
   for (Int_t i=0; i<nentries;i++) {
      nbytes += ntuple->GetEvent(i);
      if(v1>=vbot&&(v2-4096)>=vbot&&v1<=vtop&&(v2-4096)<=vtop&&(v1+(v2-4096))>=vsbot&&(v1+(v2-4096))<=vstop){
        if((v1-k1)+(v2-4096.0-k2)>0){
	  alpha=(v1-k1)/((v1-k1)+(v2-4096.0-k2));
          histp[hx].hh->Fill(lamform.Eval(alpha,lam));
          nacc++;
        }
      }
      if(i%1000==0)printf("\b\b\b%3d",100*i/nentries+1);  
   }
   printf("\n");
   printf("finished\007\n");

   printf("percent included after filtering = %d\n",int(100.0*nacc/nentries+.5));

// draw spectrum and set info

// values for canvas position defined at initialization and incremented for cascade effect
// canvas title is hist name, canvas name is hist name plus "c"
   cx+=20;cy+=30;if(cy>280){cx=360;cy=10;}
   histp[hx].hhc=new TCanvas(cpname,hpname,cx,cy,703,503);

   histp[hx].hh.Draw("e");

// create and draw limit lines
   histp[hx].hhl[0]=new TLine(histp[hx].hh.GetBinLowEdge(1),-500,histp[hx].hh.GetBinLowEdge(1),1.5*histp[hx].hh.GetMaximum());
   histp[hx].hhl[0].SetLineColor(46);
   histp[hx].hhl[0].Draw();
   histp[hx].hhl[1]=new TLine(histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),-500,histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),1.5*histp[hx].hh.GetMaximum());
   histp[hx].hhl[1].SetLineColor(38);
   histp[hx].hhl[1].Draw();

//                                                                           end of ntup2hist1d.c
 break;//=======================================================================================

 case 7://=======================================================================================
//   ntup2hist2d.c                                                                  ntup2hist2d.c
//   take a PSD ntuple and create master histogram (2D)
//   matt fritts
//////////////////////////////////////////////////////////
  
   char namebit[50], nameroot[50], hmtitle[60], yesorno[10], nm[30], namehead[50], st[10], fid[5];      
   mhx++;
   
//   interrogate user for data file name

   printf("Enter desired dataset name(I'll get the ntuple): ");
   scanf("%s",namebit);
   sprintf(nameroot,"%s%s%s",namepref,namebit,namesuff);
  
//   open Root file containing ntuple, produced by psd2ntup.c
   TFile *fin = (TFile*)gROOT->GetListOfFiles()->FindObject(nameroot);
   if (!fin) {
      printf("retrieving file...\n");
      fin = new TFile(nameroot);
   }
   else printf("already open...\n");
   fin.cd();
   
   TTree *ntuple = (TTree*)gDirectory->Get("ntuple");
//   gets ntuple from active directory

//Declaration of leaves types
   Float_t         lam;
   Float_t         pos,pos0,pos1;
   Float_t         sig;
   Float_t         om;
   Float_t         fc;
   Float_t         time;
   Float_t         v1;
   Float_t         v2;
   Float_t         alpha;   
   Float_t         lambda;
                                                                                 // ntup2hist2d.c             
//Set branch addresses
   ntuple->SetBranchAddress("lam",&lam);
   ntuple->SetBranchAddress("pos",&pos);
   ntuple->SetBranchAddress("sig",&sig);
   ntuple->SetBranchAddress("om",&om);
   ntuple->SetBranchAddress("fc",&fc);
   ntuple->SetBranchAddress("time",&time);
   ntuple->SetBranchAddress("v1",&v1);
   ntuple->SetBranchAddress("v2",&v2);
   ntuple->SetBranchAddress("alpha",&alpha);    

   strcpy(fid,"");
   printf("Filtering criteria: v>=%d, v<=%d, sum>=%d, sum<=%d\n",vbot,vtop,vsbot,vstop);
   printf("Would you like to change these? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
    printf("Enter bottom limit for v1 and v2 (0=unfiltered): ");
    scanf("%d",&vbot);
    printf("Enter top limit for v1 and v2 (4095=unfiltered): ");
    scanf("%d",&vtop);
    printf("Enter bottom limit for sum of v1 and v2 (0=unfiltered): ");
    scanf("%d",&vsbot);
    printf("Enter top limit for sum of v1 and v2 (8190=unfiltered): ");
    scanf("%d",&vstop);
    printf("Enter identifier for histogram: ");
    scanf("%s",fid);
   }
   printf("Offset criteria: %d subtracted from v1, %d from v2\n",k1,k2);
   printf("Would you like to change these? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
    printf("Enter constant subtracted from v1 (0=uncorrected): ");
    scanf("%d",&k1);
    printf("Enter constant subtracted from v2 (0=uncorrected): ");
    scanf("%d",&k2);
    printf("Enter identifier for histogram: ");
    scanf("%s",fid);
   }

// define formula to calculate "lambda"
   printf("Calibration parameters: c0=%.3f, c1=%.3f, c2=%.3f, c3=%.3f\n",c0,c1,c2,c3);
   printf("Would you like to change these? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
     printf("Enter new c0: ");
     scanf("%f",&c0);
     printf("Enter new c1: ");
     scanf("%f",&c1);
     printf("Enter new c2: ");
     scanf("%f",&c2);
     printf("Enter new c3: ");
     scanf("%f",&c3);
   }
   TFormula *lamform=new TFormula("lamform","y+[0]+[1]*(x-.5)+[2]*(x-.5)^2+[3]*(x-.5)^3");
//  "x" in formula is "alpha", "y" in formula is "lam"
//  Formula essentially redefines alpha to represent center of detector
   lamform.SetParameter(0,c0);
   lamform.SetParameter(1,c1);
   lamform.SetParameter(2,c2);
   lamform.SetParameter(3,c3);
                                                                                 // ntup2hist2d.c             
// set histogram parameters
   Axis_t startx,stopx,stepx;
   Axis_t starty,stopy,stepy;
   Int_t nbinsx,nbinsy;

// check to see if this is a multiple-position histogram by comparing first pos
// to the value of pos sig events later. Also sets stepy based on first two pos
   ntuple.GetEvent(0);
   pos0 = pos;
   ntuple.GetEvent(sig);
   pos1 = pos;
   stepy = pos1-pos0;
   if(stepy==0){
     printf("This is a single foil position dataset.\n");
     printf("Use ntup2hist1d.c instead.\n");
     break;
   }
   starty = ntuple.GetMinimum("pos")-stepy/2;
   stopy = ntuple.GetMaximum("pos")+stepy/2;

// find min and max wavelength, rounded to angstrom
// (min rounded down, max rounded up)
   printf("getting minima and maxima...\n");
   startx = int(lamform.Eval(ntuple.GetMinimum("alpha"),ntuple.GetMinimum("lam")));
   stopx = int(lamform.Eval(ntuple.GetMaximum("alpha"),ntuple.GetMaximum("lam")))+1.0;
   stepx = .2;
    
   printf("wavelength min:%.2f max:%.2f bin size:%.2f\n",startx,stopx,stepx);

   printf("Would you like to change histogram wavelength parameters? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
     printf("Enter new minimum wavelength: ");
     scanf("%f",&startx);
     printf("Enter new maximum wavelength: ");
     scanf("%f",&stopx);
     printf("Enter wavelength resolution: ");
     scanf("%f",&stepx);
  }
                                                                                 // ntup2hist2d.c             
   printf("position min:%.2f max:%.2f bin size:%.2f\n",starty,stopy,stepy);

   nbinsx=(int)((stopx-startx)/stepx+.5);
   nbinsy=(int)((stopy-starty)/stepy+.5);
   stopy=starty+int((stopy-starty)/stepy+.5)*stepy;
   stopx=startx+int((stopx-startx)/stepx+.5)*stepx;   
// last two lines insure that bin sizes are preserved by insuring max differs from min by 
// an integral number of steps

// abort option
   printf("Enter \"a\" to abort, anything else to continue: ");
   scanf("%s",yesorno);
   if(yesorno[0]=='a')break;
  
// name and title master histogram
   sprintf(nm,"%s%smh",namebit,fid);
   printf("creating 2D histogram \"%s\"...\n",nm);
   sprintf(hmtitle,"%s%s",namebit,fid);

// define master histogram
   mhp[mhx].mh = new TH2F(nm,hmtitle,nbinsx,startx,stopx,nbinsy,starty,stopy);
   strcpy(mhp[mhx].mhn,nm); 

   Int_t nentries = ntuple->GetEntries();
 
   int nacc=0;

   Int_t nbytes = 0;
   printf("percent finished:    ");
   Axis_t apos,alam;
// above became necessary with root 2.22/09; TH2F::Fill must have Axis_t parameters
   for (Int_t i=0; i<nentries;i++) {
      nbytes += ntuple->GetEvent(i);        
      if(v1>=vbot&&(v2-4096)>=vbot&&v1<=vtop&&(v2-4096)<=vtop&&(v1+(v2-4096))>=vsbot&&(v1+(v2-4096))<=vstop){
        if((v1-k1)+(v2-4096.0-k2)>0){
	  alpha=(v1-k1)/((v1-k1)+(v2-4096.0-k2));
          apos=pos;alam=lamform.Eval(alpha,lam);
	  mhp[mhx].mh->Fill(alam,apos);
          nacc++;
        }
      }
      if(i%1000==0)printf("\b\b\b%3d",100*i/nentries+1);  
   }
   printf("\n");

   printf("percent included after filtering = %d\n",int(100.0*nacc/nentries+.5));

   printf("normalizing\n");
                                                                                 // ntup2hist2d.c             
// open header file
   sprintf(namehead,"%s%shead.dat",namepref,namebit);
   FILE *fhead = fopen(namehead,"r");
   if (fhead <= 0) {printf("Sorry, cannot open header file\n");break;}
 
// normalization routine: normalizes data at each position to OM
   fscanf(fhead,"%s %s %s %s %s %s",st,st,st,st,st,st);
   fscanf(fhead,"%f %f %f %f %f %f", &lam, &pos, &sig, &om, &fc, &time);
   float om0=om;
   Int_t ncols;
// read headers one at a time
   while (fhead) {
    float omnow=om;
//  determine position bin corresponding to header
    int iy=int((pos-starty)/stepy)+1;
//  normalize value and reset error of each wavelength bin
    for(int ix=1;ix<=nbinsx;ix++){
     float bcnow=mhp[mhx].mh.GetBinContent(mhp[mhx].mh.GetBin(ix,iy));
     float benow=mhp[mhx].mh.GetBinError(mhp[mhx].mh.GetBin(ix,iy));
     mhp[mhx].mh.SetBinContent(mhp[mhx].mh.GetBin(ix,iy),om0/omnow*bcnow);
     mhp[mhx].mh.SetBinError(mhp[mhx].mh.GetBin(ix,iy),om0/omnow*benow);
    }
    ncols = fscanf(fhead,"%f %f %f %f %f %f", &lam, &pos, &sig, &om, &fc, &time);
    if (ncols < 0) break;
   }

   printf("finished\007\n");
   if(strcmp(fid,"")!=0)printf("When creating spectra and decay curves from this 2d histogram,\n use \"%s%s\" as the dataset name\n",namebit,fid);


// 2d histogram is now filled, named (data name + "mh"), titled (data name),
// and in a file named (data name + "f")

//                                                                           end of ntup2hist2d.c
 break;//=======================================================================================

 case 8://=======================================================================================
//   ntups2hist2d.c                                                                ntups2hist2d.c             
//   take multiple PSD ntuples and create master histogram (2D)
//   can handle up to 10 datasets, uses a single calibration
//   matt fritts
//////////////////////////////////////////////////////////
  
char namebit[50], nameroot[50], hmtitle[60], yesorno[10], nm[30], nm2[30], namehead[50], st[10];       
TTree *ntup[10];
int numsets;
FILE *fheadm[10];

mhx++;
comx++;

printf("Enter number of datasets to be combined: ");
scanf("%d",&numsets);   

for(int l=1;l<=numsets;l++){

 // interrogate user for each data file name
 printf("Enter desired dataset name(I'll get the ntuple): ");
 scanf("%s",namebit);
 sprintf(nameroot,"%s%s%s",namepref,namebit,namesuff);
  
 // open each Root file containing ntuple, produced by psd2ntup.c
 TFile *fin = (TFile*)gROOT->GetListOfFiles()->FindObject(nameroot);
 if (!fin) {
      printf("retrieving file...\n");
      fin = new TFile(nameroot);
 }
 else printf("already open...\n");
 fin.cd();
                                                                                // ntups2hist2d.c             
 ntup[l] = (TTree*)gDirectory->Get("ntuple");
 // gets each ntuple from active directory

 // open each header file
 sprintf(namehead,"%s%shead.dat",namepref,namebit);
 fheadm[l]=fopen(namehead,"r");
 if (fheadm[l]<=0)printf("sorry, cannot open header file\n");
}

//Declaration of leaves types
Float_t         lam;
Float_t         pos,pos0,pos1;
Float_t         sig;
Float_t         om;
Float_t         fc;
Float_t         time;
Float_t         v1;
Float_t         v2;
Float_t         alpha;   
Float_t         lambda;

//Set branch addresses

for(int l=1;l<=numsets;l++){
 ntup[l]->SetBranchAddress("lam",&lam);
 ntup[l]->SetBranchAddress("pos",&pos);
 ntup[l]->SetBranchAddress("sig",&sig);
 ntup[l]->SetBranchAddress("om",&om);
 ntup[l]->SetBranchAddress("fc",&fc);
 ntup[l]->SetBranchAddress("time",&time);
 ntup[l]->SetBranchAddress("v1",&v1);
 ntup[l]->SetBranchAddress("v2",&v2);
 ntup[l]->SetBranchAddress("alpha",&alpha);    
}
                                                                                // ntups2hist2d.c             
printf("Filtering criteria: v>=%d, v<=%d, sum>=%d, sum<=%d\n",vbot,vtop,vsbot,vstop);
printf("Would you like to change these? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y'){
 printf("Enter bottom limit for v1 and v2 (0=unfiltered): ");
 scanf("%d",&vbot);
 printf("Enter top limit for v1 and v2 (4095=unfiltered): ");
 scanf("%d",&vtop);
 printf("Enter bottom limit for sum of v1 and v2 (0=unfiltered): ");
 scanf("%d",&vsbot);
 printf("Enter top limit for sum of v1 and v2 (8190=unfiltered): ");
 scanf("%d",&vstop);
}
printf("Offset criteria: %d subtracted from v1, %d from v2\n",k1,k2);
printf("Would you like to change these? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y'){
 printf("Enter constant subtracted from v1 (0=uncorrected): ");
 scanf("%d",&k1);
 printf("Enter constant subtracted from v2 (0=uncorrected): ");
 scanf("%d",&k2);
}

// define formula to calculate "lambda"     
printf("Calibration parameters: c0=%.3f, c1=%.3f, c2=%.3f, c3=%.3f\n",c0,c1,c2,c3);
printf("Would you like to change these? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y'){
 printf("Enter new c0: ");
 scanf("%f",&c0);
 printf("Enter new c1: ");
 scanf("%f",&c1);
 printf("Enter new c2: ");
 scanf("%f",&c2);
 printf("Enter new c3: ");
 scanf("%f",&c3);
}
TFormula *lamform=new TFormula("lamform","y+[0]+[1]*(x-.5)+[2]*(x-.5)^2+[3]*(x-.5)^3");
// "x" in formula is "alpha", "y" in formula is "lam"
// Formula essentially redefines alpha to represent center of detector
lamform.SetParameter(0,c0);
lamform.SetParameter(1,c1);
lamform.SetParameter(2,c2);
lamform.SetParameter(3,c3);

// set histogram parameters

Axis_t startx,stopx,stepx;
Axis_t starty,stopy,stepy;
Int_t nbinsx,nbinsy;
                                                                                // ntups2hist2d.c             
printf("getting minima and maxima...\n");

// find min and max wavelength, rounded to angstrom
// checks each ntuple and returns larger of the minima, smaller of the maxima
startx=-1000;stopx=100000;stepy=100;starty=-100;stopy=1000;stepx=.2;
for(int l=1;l<=numsets;l++){
 if(startx<int(lamform.Eval(ntup[l].GetMinimum("alpha"),ntup[l].GetMinimum("lam"))))
  startx=int(lamform.Eval(ntup[l].GetMinimum("alpha"),ntup[l].GetMinimum("lam")));     

 if(stopx>int(lamform.Eval(ntup[l].GetMaximum("alpha"),ntup[l].GetMaximum("lam")))+1.0)     
  stopx=int(lamform.Eval(ntup[l].GetMaximum("alpha"),ntup[l].GetMaximum("lam")))+1.0;   

 // check to see if this is a multiple-position histogram by comparing first pos
 // to the value of pos sig events later. Also sets stepy based on first two pos
 ntup[l].GetEvent(0);
 pos0 = pos;
 ntup[l].GetEvent(sig);
 pos1 = pos;
 if(stepy>(pos1-pos0))stepy=(pos1-pos0);
 if(pos1-pos0==0){
  printf("This is a single foil position dataset.\n");
  printf("Use ntup2hist1d.c instead.\n");
  break;
 }
 // assumption: individual datasets taken at identical positions
}

starty=ntup[1].GetMinimum("pos")-stepy/2;
stopy=ntup[1].GetMaximum("pos")+stepy/2;

printf("wavelength min:%.2f max:%.2f bin size:%.2f\n",startx,stopx,stepx);

printf("Would you like to change histogram wavelength parameters? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y'){
 printf("Enter new minimum wavelength: ");
 scanf("%f",&startx);
 printf("Enter new maximum wavelength: ");
 scanf("%f",&stopx);
 printf("Enter wavelength resolution: ");
 scanf("%f",&stepx);
}
                                                                                // ntups2hist2d.c             
printf("position min:%.2f max:%.2f bin size:%.2f\n",starty,stopy,stepy);
  
nbinsx=(int)((stopx-startx)/stepx+.5);
nbinsy=(int)((stopy-starty)/stepy+.5);
stopy=starty+int((stopy-starty)/stepy+.5)*stepy;
stopx=startx+int((stopx-startx)/stepx+.5)*stepx;   
// last two lines insure that bin sizes are preserved by insuring max differs from min by 
// an integral number of steps
   
printf("Enter data name for this combined set: ");
scanf("%s",namebit);

// abort option
   printf("Enter \"a\" to abort, anything else to continue: ");
   scanf("%s",yesorno);
   if(yesorno[0]=='a')break;
  
// name and title master histogram
sprintf(nm,"%smh",namebit);
strcpy(hmtitle,namebit);

printf("creating individual histograms\n");

for(int l=1;l<=numsets;l++){
 sprintf(nm2,"hm%d",l);
 hmm[l] = new TH2F(nm2,hmtitle,nbinsx,startx,stopx,nbinsy,starty,stopy);

 Int_t nentries = ntup[l]->GetEntries();

 int nacc=0;

 Int_t nbytes = 0;
 Axis_t alam,apos;
 // above became necessary with root 2.22/09; TH2F::Fill must have Axis_t parameters
 printf("number %d percent finished:    ",l);
 for (Int_t i=0; i<nentries;i++) {
  nbytes += ntup[l]->GetEvent(i);
  if(v1>=vbot&&(v2-4096)>=vbot&&v1<=vtop&&(v2-4096)<=vtop&&(v1+(v2-4096))>=vsbot&&(v1+(v2-4096))<=vstop){
   if((v1-k1)+(v2-4096.0-k2)>0){
    alpha=(v1-k1)/((v1-k1)+(v2-4096.0-k2));
    apos=pos;alam=lamform.Eval(alpha,lam);
    hmm[l]->Fill(alam,apos);
    nacc++;
   }
  }
  if(i%1000==0)printf("\b\b\b%3d",100*i/nentries+1);  
 }
 printf("\n");
 printf("percent included after filtering = %d\n",int(100.0*nacc/nentries+.5));
}
                                                                                // ntups2hist2d.c             
printf("calculating normalization factors...\n");

float omnc[10][500];
// array to store normalization factors of each dataset
// first index is dataset, second index is position bin, calculated below
for(int l=1;l<=numsets;l++){ 
 fscanf(fheadm[l],"%s %s %s %s %s %s",st,st,st,st,st,st);
 fscanf(fheadm[l],"%f %f %f %f %f %f", &lam, &pos, &sig, &om, &fc, &time);
 float om0=om;
 Int_t ncols;
 while (fheadm[l]) {
  float omnow=om;
  omnc[l][int((pos-starty)/stepy)+1]=om0/om;
  ncols = fscanf(fheadm[l],"%f %f %f %f %f %f", &lam, &pos, &sig, &om, &fc, &time);
  if (ncols < 0) break;
 }
}

printf("creating final 2D histogram \"%s\"...\n",nm);

// define master histogram
hcom[comx]=(*hmm[1]);
// master histogram initially defined equal to histogram of first dataset
// this sets bins and title
// normalization done while contents and errors are set
for(int iy=1;iy<=nbinsy;iy++){
 for(int ix=1;ix<=nbinsx;ix++){
  float bcnow=0;
  float benow=0;
  for(int l=1;l<=numsets;l++){ 
   bcnow+=omnc[l][iy]*hmm[l].GetBinContent(hmm[l].GetBin(ix,iy));
   benow+=pow(omnc[l][iy],2)*hmm[l].GetBinContent(hmm[l].GetBin(ix,iy));
  }
  benow=pow(benow,.5);
  hcom[comx].SetBinContent(hcom[comx].GetBin(ix,iy),bcnow);
  hcom[comx].SetBinError(hcom[comx].GetBin(ix,iy),benow);
 }
}

hcom[comx].SetName(nm);

mhp[mhx].mh = (TH2F*)gROOT.FindObject(nm);
strcpy(mhp[mhx].mhn,nm); 

printf("finished\007\n");
printf("When creating spectra and decay curves from this 2d histogram,\n use \"%s\" as the dataset name\n",namebit);

// 2d histogram is now filled, named (data name + "mh"), titled (data name),
// and in a file (data name + "f")

//                                                                          end of ntups2hist2d.c
 break;//=======================================================================================

 case 9://=======================================================================================
// poscut.c                                                                              poscut.c
// creates a position cut (spectrum) of a 2d histogram (mh)
// matt fritts

Float_t posn;
Int_t numbins,firstybin,lastybin;
char hpname[30],cpname[30],hplabel[50],dname[30],nm[30];

hx++;

// find 2d histogram

printf("Enter dataset name: ");
scanf("%s",dname);
sprintf(nm,"%smh",dname);

int mhindex,hfd=0;
for(int l=1;l<10;l++)if(strcmp(mhp[l].mhn,nm)==0){mhindex=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find 2d histogram\n");break;}

// get histogram position information

stepy=mhp[mhindex].mh.GetYaxis().GetBinWidth(1);
starty=mhp[mhindex].mh.GetYaxis().GetBinLowEdge(1);
numbins=mhp[mhindex].mh.GetNbinsY();
stopy=mhp[mhindex].mh.GetYaxis().GetBinLowEdge(numbins)+stepy;

// choose position

printf("Enter desired foil position (min= %.2f, max= %.2f, inc= %.2f)\n",starty+stepy/2,stopy-stepy/2,stepy);
scanf("%f",&posn);
firstybin=int((posn-starty)/stepy)+1;
lastybin=firstybin;
                                                                                      // poscut.c             
// create spectrum

sprintf(hpname,"%sp%d",dname,int(posn*10+.5));
sprintf(cpname,"%sc",hpname);
sprintf(hplabel,"%s, Spectrum @ position = %.1f",dname,posn);

histp[hx].hh=mhp[mhindex].mh.ProjectionX(hpname,firstybin,lastybin);
// set name in structure and initialize number of fits
strcpy(histp[hx].hhn,hpname); 
histp[hx].hhnf=0;

histp[hx].hh.SetTitle(hplabel);

// values for canvas position defined at initialization and incremented for cascade effect
// canvas title is hist name, canvas name is hist name plus "c"
cx+=20;cy+=30;if(cy>280){cx=360;cy=10;}
histp[hx].hhc=new TCanvas(cpname,hpname,cx,cy,703,503);

histp[hx].hh.Draw("e");

// create and draw limit lines
histp[hx].hhl[0]=new TLine(histp[hx].hh.GetBinLowEdge(1),-500,histp[hx].hh.GetBinLowEdge(1),1.5*histp[hx].hh.GetMaximum());
histp[hx].hhl[0].SetLineColor(46);
histp[hx].hhl[0].Draw();
histp[hx].hhl[1]=new TLine(histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),-500,histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),1.5*histp[hx].hh.GetMaximum());
histp[hx].hhl[1].SetLineColor(38);
histp[hx].hhl[1].Draw();

// end of poscut.c                                                                end of poscut.c
 break;//=======================================================================================

 case 10://=======================================================================================
// lamcut.c                                                                              lamcut.c
// creates a wavelength cut (decay curve) of a 2d histogram (mh)
// matt fritts

Float_t lamblo,lambhi,lamb;
Int_t numbins,firstxbin,lastxbin;
char hlname[30],clname[30],hllabel[50],dname[30],nm[30],yesorno[10],hpname[30];
int hindex;

hx++;

// find 2d histogram

printf("Enter dataset name: ");
scanf("%s",dname);
sprintf(nm,"%smh",dname);

int mhindex,hfd=0;
for(int l=1;l<10;l++)if(strcmp(mhp[l].mhn,nm)==0){mhindex=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find 2d histogram\n");break;}

// get histogram wavelength information

stepx=mhp[mhindex].mh.GetXaxis().GetBinWidth(1);
startx=mhp[mhindex].mh.GetXaxis().GetBinLowEdge(1);
numbins=mhp[mhindex].mh.GetNbinsX();
stopx=mhp[mhindex].mh.GetXaxis().GetBinLowEdge(numbins)+stepx;
                                                                                      // lamcut.c             
// choose wavelength range

printf("  l: Use line limits\n");
printf("  e: Enter limits numerically\n");
printf("Enter option: ");
scanf("%s",yesorno);

if(yesorno[0]=='e' || yesorno[0]=='E'){
  printf("Enter minimum wavelength (min= %.2f, max= %.2f)\n",startx,stopx);
  scanf("%f",&lamblo);
  printf("Enter maximum wavelength \n");
  scanf("%f",&lambhi);
}

if(yesorno[0]=='l' || yesorno[0]=='L'){
  printf("Enter name of spectrum whose limit lines you wish to use: ");
  scanf("%s",hpname);
  for(int l=1;l<40;l++)if(strcmp(histp[l].hhn,hpname)==0)hindex=l;
  lamblo=histp[hindex].hhl[0].GetX1();
  lambhi=histp[hindex].hhl[1].GetX1();
}

firstxbin=int((lamblo-startx)/stepx)+1;
lastxbin=int((lambhi-startx)/stepx)+1;

// create decay curve

sprintf(hlname,"%sw%d",dname,int((lambhi+lamblo)/2+.5));

sprintf(clname,"%sc",hlname);

sprintf(hllabel,"%s, Decay @ mid = %.1f, width = %.1f",dname,(lambhi+lamblo)/2,lambhi-lamblo);

histp[hx].hh=mhp[mhindex].mh.ProjectionY(hlname,firstxbin,lastxbin);
// set name in structure and initialize number of fits
strcpy(histp[hx].hhn,hlname); 
histp[hx].hhnf=0;

histp[hx].hh.SetTitle(hllabel);

// values for canvas position defined at initialization and incremented for cascade effect
// canvas title is hist name, canvas name is hist name plus "c"
cx+=20;cy+=30;if(cy>280){cx=360;cy=10;}
histp[hx].hhc=new TCanvas(clname,hlname,cx,cy,703,503);

histp[hx].hh.Draw("e");

// create and draw limit lines
histp[hx].hhl[0]=new TLine(histp[hx].hh.GetBinLowEdge(1),-500,histp[hx].hh.GetBinLowEdge(1),1.5*histp[hx].hh.GetMaximum());
histp[hx].hhl[0].SetLineColor(46);
histp[hx].hhl[0].Draw();
histp[hx].hhl[1]=new TLine(histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),-500,histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),1.5*histp[hx].hh.GetMaximum());
histp[hx].hhl[1].SetLineColor(38);
histp[hx].hhl[1].Draw();

// end of lamcut.c                                                                end of lamcut.c
 break;//=======================================================================================

 case 12://=======================================================================================
// openhist.c                                                                          openhist.c   
// load saved spectrum or decay curve into memory and draw it

   char namebit[50], nameroot[50], cpname[30], fbname[10], cflabel[20], htype[1];        
   int l;
   Float_t posn;   

   hx++;

// find histogram file

   printf("Enter desired histogram file name (w/out extension): ");
   scanf("%s",namebit);
   sprintf(nameroot,"%s%s%s",namepref,namebit,namesuff);
  
   TFile *fin = (TFile*)gROOT->GetListOfFiles()->FindObject(nameroot);
   if (!fin) {
      printf("retrieving file...\n");
      fin = new TFile(nameroot);
   }
   else printf("already open...\n");
  
   histp[hx].hh= (TH1D*)gROOT.FindObject(namebit);
// set name in structure and initialize number of fits
   strcpy(histp[hx].hhn,namebit); 
   histp[hx].hhnf=0;

   sprintf(cpname,"%sc",namebit);
// values for canvas position defined at initialization and incremented for cascade effect
// canvas title is hist name, canvas name is hist name plus "c"
   cx+=20;cy+=30;if(cy>280){cx=360;cy=10;}
   histp[hx].hhc= new TCanvas(cpname,namebit,cx,cy,703,503);
   gPad->SetTicks(1,1);
   gPad->SetGridx();
   gPad->SetGridy();                                                                                 // openhist.c             
   histp[hx].hh.Draw("e");

// create and draw limit lines
   histp[hx].hhl[0]=new TLine(histp[hx].hh.GetBinLowEdge(1),-500,histp[hx].hh.GetBinLowEdge(1),1.5*histp[hx].hh.GetMaximum());
   histp[hx].hhl[0].SetLineColor(46);
   histp[hx].hhl[0].Draw();
   histp[hx].hhl[1]=new TLine(histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),-500,histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),1.5*histp[hx].hh.GetMaximum());
   histp[hx].hhl[1].SetLineColor(38);
   histp[hx].hhl[1].Draw();

// look for PaveTexts named "fitbox1", "fitbox2" etc. and update histp[hx]
   l=0;
   sprintf(fbname,"fitbox ");
   for(int i='1';i<='9';i++){
    fbname[6]=i; 
    TPaveText *fbcurrent=(TPaveText*)gROOT.FindObject(fbname);
    if(fbcurrent){histp[hx].hhnf++;histp[hx].hhf[histp[hx].hhnf]=fbcurrent;l++;}
   }

   printf("%d fits\n",l);

//  end of openhist.c                                                           end of openhist.c   
 break;//=======================================================================================

 case 13://=======================================================================================
//  histzoom.c                                                                         histzoom.c
//  zooms in on a region of a spectrum or decay curve

if(hx==0){
  printf("sorry, no spectra or decay curves exist\n");
  break;
}

Float_t xzlo, xzhi;
char yesorno[10],dname[30],nm[30],hname[30];
Axis_t startx,stepx;
int hindex,hfd=0;

// find histogram
printf("Enter histogram name: ");
scanf("%s",hname);
for(int l=1;l<40;l++)if(strcmp(histp[l].hhn,hname)==0){hindex=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find that histogram\n");break;}

// axis limits
printf("  p: Use previous limits (%.2f,%.2f)\n",xzlo,xzhi);
printf("  l: Use line limits\n");
printf("  e: Enter limits numerically\n");
printf("  f: Show full range\n");
printf("Enter option: ");
scanf("%s",yesorno);

if(yesorno[0]=='e' || yesorno[0]=='E'){
  printf("Enter lower x-axis limit\n");
  scanf("%f",&xzlo);
  printf("Enter upper x-axis limit\n");
  scanf("%f",&xzhi);
}
                                                                                    // histzoom.c             
if(yesorno[0]=='l' || yesorno[0]=='L'){
  xzlo=histp[hindex].hhl[0].GetX1();
  xzhi=histp[hindex].hhl[1].GetX1();
}

startx=histp[hindex].hh.GetBinLowEdge(1);
stepx=histp[hindex].hh.GetBinWidth(1);

if(yesorno[0]=='f' || yesorno[0]=='F'){
  xzlo=startx;
  xzhi=histp[hindex].hh.GetBinLowEdge(histp[hindex].hh.GetNbinsX())+stepx;
}

histp[hindex].hh.GetXaxis().SetRange(int((xzlo-startx)/stepx)+1,int((xzhi-startx)/stepx)+1);

// draw histogram
histp[hindex].hhc.cd();
histp[hindex].hh.Draw("e");

// reset limit line postions and draw limit lines
histp[hindex].hhl[0].SetX1(xzlo);
histp[hindex].hhl[0].SetX2(xzlo);
histp[hindex].hhl[1].SetX1(xzhi);
histp[hindex].hhl[1].SetX2(xzhi);
histp[hindex].hhl[0].Draw();
histp[hindex].hhl[1].Draw();

//                                                                              end of histzoom.c
 break;//=======================================================================================

 case 14://=======================================================================================
//  specfit.c                                                                           specfit.c
//  fitting options for a spectrum
//  also creates fit information text boxes, viewable with fitinfo.c

if(hx==0){
  printf("sorry, no spectra exist\n");
  break;
}

int saveprev,fitchoice,seplim,widlim1,widlim2,widequ,l,hindex,findex,fitclr=2,hfd=0,conlim;
Float_t wavelo,wavehi,widthlo,widthhi,width1,width2,height[10],center[10],sep[9],gconst;
char yesorno[10],tl[100],tlb[50],dname[30],nm[30],hpname[30];

//  select histogram to fit  //
printf("Enter histogram name: ");
scanf("%s",hpname);
for(int l=1;l<40;l++)if(strcmp(histp[l].hhn,hpname)==0){hindex=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find that histogram\n");break;}

//  select appropriate canvas //
histp[hindex].hhc.cd();

//  if previous fits are not to be saved, set number of fits to zero
//  findex is the number of the current fit
printf("Save previous fits? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y')saveprev=1;
else{saveprev=0;histp[hindex].hhnf=0;}
findex=++histp[hindex].hhnf;

//  create fit info box
//  give it a random position in canvas
float xsh=8.0/32767.0*rand()-4;float ysh=8.0/32767.0*rand()-4;
histp[hindex].hhf[findex]=new TPaveText(5+xsh,5+ysh,95+xsh,95+ysh);
//  set color of fit
fitclr=(histp[hindex].hhnf-1)%6+2;
if(fitclr>4)fitclr++;
//  add title of histo to fit box and calibration parameters
//  (note: cal params written are most recent settings and may not truly apply to histo)
strcpy(tl,histp[hindex].hh.GetTitle());
TText *t1=histp[hindex].hhf[findex].AddText(tl); 
t1.SetTextFont(100);
sprintf(tl,"            Cal params: c0=%.3f, c1=%.3f, c2=%.3f, c3=%.3f            ",c0,c1,c2,c3);
TText *t1=histp[hindex].hhf[findex].AddText(tl); 
t1.SetTextFont(100);

//  set fit appearance options
gStyle->SetOptFit(1);
gStyle->SetFuncColor(fitclr);
gStyle->SetFuncWidth(2);
                                                                                     // specfit.c             
//  fit options  //
printf("  1: Fit single peak (quick)\n");
printf("  2: Fit 2 peaks\n");
printf("  3: Fit 3-9 peaks\n");
printf("  4: Fit single peak (slow)\n");
printf("  5: Fit single peak to Voigt profile\n");
printf("Enter option: ");
scanf("%d",&fitchoice);

switch (fitchoice){
 case 1:          // *********** SINGLE PEAK FIT (QUICK) ***************** //

  FILE *fout;
  char nameout[80],*no;
  sprintf(nameout,"C:/Rob Learning Stuff/Root Outputs/fitout.dat");
  no=nameout;
  printf("output: %s\n",nameout);
  fout=fopen(no,"a");

  //  print type of fit to fitbox
  strcpy(tl,"Single peak fit"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);
  
  //  set x range //
  printf("  p: Use previous wavelength limits (%.2f,%.2f)\n",wavelo,wavehi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower wavelength limit: ");
   scanf("%f",&wavelo);
   printf("Enter upper wavelength limit: ");
   scanf("%f",&wavehi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   wavelo=histp[hindex].hhl[0].GetX1();
   wavehi=histp[hindex].hhl[1].GetX1();
  }
                                                                                     // specfit.c             
  //  fit info
  sprintf(tl,"Wavelength limits: %.3f - %.3f",wavelo,wavehi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  define fit function and set initial parameter guesses  //
  TF1 *g1 = new TF1("g1","gaus(0)+[3]",wavelo,wavehi);
  g1.SetParName(0,"height");
  g1.SetParName(1,"center");
  g1.SetParName(2,"sigma");
  g1.SetParName(3,"const");
  g1.SetParameter(0,histp[hindex].hh.GetBinContent(int(((wavelo+wavehi)/2-histp[hindex].hh.GetBinLowEdge(1))/histp[hindex].hh.GetBinWidth(1))+1));
  g1.SetParameter(1,(wavelo+wavehi)/2);
  g1.SetParameter(2,(wavehi-wavelo)/3);
  g1.SetParameter(3,0.0);
  
  //  fit info
  sprintf(tl,"            Fit guesses: height=%.1f, center=%.2f, sigma=%.2f, const=0            ",g1.GetParameter(0),g1.GetParameter(1),g1.GetParameter(2));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  choose whether to limit width  //
  printf("Restrict width? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   printf("Enter lower sigma limit: ");
   scanf("%f",&widthlo);
   printf("Enter upper sigma limit: ");
   scanf("%f",&widthhi);    
   g1.SetParLimits(2,widthlo,widthhi);
   g1.SetParameter(2,(widthlo+widthhi)/2);

   //  fit info
   sprintf(tl,"sigma restricted: lo=%.4f, hi=%.4f, guess=%.4f",widthlo,widthhi,(widthlo+widthhi)/2);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }   
                                                                                     // specfit.c             
  // choose whether to fix constant //
  printf("Fix constant? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   conlim=1;
   printf("Enter constant: ");
   scanf("%f",&gconst);
   g1.SetParLimits(3,1,1);
   g1.SetParameter(3,gconst);
   //  fit info
   sprintf(tl,"constant fixed at %.4f",gconst);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  else conlim=0;

  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("g1","RB+");
  else histp[hindex].hh.Fit("g1","RB");

  //  fit info
  strcpy(tl,"fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"height = %f +/- %f",g1.GetParameter(0),g1.GetParError(0));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"center = %f +/- %f",g1.GetParameter(1),g1.GetParError(1));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"sigma = %f +/- %f",g1.GetParameter(2),g1.GetParError(2));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"constant = %f +/- %f",g1.GetParameter(3),g1.GetParError(3));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",g1.GetChisquare(),g1.GetNDF(),g1.GetChisquare()/g1.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  fprintf(fout,"%s\t",histp[hindex].hh.GetTitle());
  fprintf(fout,"%f\t%f\t",g1.GetParameter(0),g1.GetParError(0));
  fprintf(fout,"%f\t%f\t",g1.GetParameter(1),g1.GetParError(1));
  fprintf(fout,"%f\t%f\t",g1.GetParameter(2),g1.GetParError(2));
  fprintf(fout,"%f\t%f\t",g1.GetParameter(3),g1.GetParError(3));
  fprintf(fout,"%f\n",g1.GetChisquare()/g1.GetNDF());
  fclose(fout);

 break;

 case 4:          // *********** SINGLE PEAK FIT (SLOW) ***************** //
  //  print type of fit to fitbox
  strcpy(tl,"Single peak fit"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);
  
  //  set x range //
  printf("  p: Use previous wavelength limits (%.2f,%.2f)\n",wavelo,wavehi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower wavelength limit: ");
   scanf("%f",&wavelo);
   printf("Enter upper wavelength limit: ");
   scanf("%f",&wavehi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   wavelo=histp[hindex].hhl[0].GetX1();
   wavehi=histp[hindex].hhl[1].GetX1();
  }
                                                                                     // specfit.c             
  //  fit info
  sprintf(tl,"Wavelength limits: %.3f - %.3f",wavelo,wavehi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  define fit function and set initial parameter guesses  //
  TF1 *g1 = new TF1("g1","gaus(0)+[3]",wavelo,wavehi);
  g1.SetParName(0,"height");
  g1.SetParName(1,"center");
  g1.SetParName(2,"sigma");
  g1.SetParName(3,"const");
  g1.SetParameter(0,histp[hindex].hh.GetBinContent(int(((wavelo+wavehi)/2-histp[hindex].hh.GetBinLowEdge(1))/histp[hindex].hh.GetBinWidth(1))+1));
  g1.SetParameter(1,(wavelo+wavehi)/2);
  g1.SetParameter(2,(wavehi-wavelo)/3);
  g1.SetParameter(3,0.0);
  
  //  choose whether to limit width  //
  printf("Restrict width? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   printf("Enter lower sigma limit: ");
   scanf("%f",&widthlo);
   printf("Enter upper sigma limit: ");
   scanf("%f",&widthhi);    
   g1.SetParLimits(2,widthlo,widthhi);
   g1.SetParameter(2,(widthlo+widthhi)/2);

   //  fit info
   sprintf(tl,"sigma restricted: lo=%.4f, hi=%.4f, guess=%.4f",widthlo,widthhi,(widthlo+widthhi)/2);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  else{
   printf("Enter approximate sigma: ");
   scanf("%f",&width1);
   g1.SetParameter(2,width1);
  }
                                                                                     // specfit.c             
  // choose whether to fix constant //
  printf("Fix constant? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   conlim=1;
   printf("Enter constant: ");
   scanf("%f",&gconst);
   g1.SetParLimits(3,1,1);
   g1.SetParameter(3,gconst);
   //  fit info
   sprintf(tl,"constant fixed at %.4f",gconst);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  else{
   conlim=0;
   printf("Enter approximate constant: ");
   scanf("%f",&gconst);
   g1.SetParameter(3,gconst);
  }

  //guess center
  printf("Enter approximate center: ");
  scanf("%f",&center[1]);
  g1.SetParameter(1,center[1]);

  //guess height
  printf("Enter approximate height: ");
  scanf("%f",&height[1]);
  g1.SetParameter(0,height[1]);


  //  fit info
  sprintf(tl,"            Fit guesses: height=%.1f, center=%.2f, sigma=%.2f, const=%.2f            ",g1.GetParameter(0),g1.GetParameter(1),g1.GetParameter(2),g1.GetParameter(3));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("g1","RB+");
  else histp[hindex].hh.Fit("g1","RB");

  //  fit info
  strcpy(tl,"fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"height = %f +/- %f",g1.GetParameter(0),g1.GetParError(0));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"center = %f +/- %f",g1.GetParameter(1),g1.GetParError(1));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"sigma = %f +/- %f",g1.GetParameter(2),g1.GetParError(2));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"constant = %f +/- %f",g1.GetParameter(3),g1.GetParError(3));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",g1.GetChisquare(),g1.GetNDF(),g1.GetChisquare()/g1.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

 break;

 case 2:              // ************* DOUBLE PEAK FIT **************** //
  //  fit info
  strcpy(tl,"Double peak fit"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);
  
  //  set x range  //
  printf("  p: Use previous wavelength limits (%.2f,%.2f)\n",wavelo,wavehi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower wavelength limit: ");
   scanf("%f",&wavelo);
   printf("Enter upper wavelength limit: ");
   scanf("%f",&wavehi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   wavelo=histp[hindex].hhl[0].GetX1();
   wavehi=histp[hindex].hhl[1].GetX1();
  }
                                                                                     // specfit.c             
  //  fit info
  sprintf(tl,"Wavelength limits: %.3f - %.3f",wavelo,wavehi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  ask for parameter guess values and give limiting options  //

  //  heights  //
  printf("Enter approximate height of left peak: ");
  scanf("%f",&height[1]);
  printf("Enter approximate height of right peak: ");
  scanf("%f",&height[2]);

  //  fit info
  sprintf(tl,"Height guesses: left=%.1f, right=%.1f",height[1],height[2]);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  locations  //
  printf("Fix separation? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   seplim=1;
   printf("Enter center of left peak (unfixed): ");
   scanf("%f",&center[1]);
   printf("Enter center of right peak: ");
  }
  else{
   seplim=0;
   printf("Enter approximate center of left peak: ");
   scanf("%f",&center[1]);
   printf("Enter approximate center of right peak: ");
  }
  scanf("%f",&center[2]);
  sep[1] = center[2]-center[1];
                                                                                     // specfit.c             
  //  fit info
  if(seplim==1){
   sprintf(tl,"Center guesses: left=%.3f, right=%.3f, separation fixed",center[1],center[2]);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  else{
   sprintf(tl,"Center guesses: left=%.3f, right=%.3f, separation unfixed",center[1],center[2]);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }

  //  widths  //
  printf("Widths equal but not fixed? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   widequ=1;
   width1=.5;
   widlim1=0;
   widlim2=0;

   //  fit info
   sprintf(tl,"Sigmas guess: .1, equal and unfixed");
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  else{ 
   widequ=0;
   printf("Fix width of left peak? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
    widlim1=1;
    printf("Enter sigma of left peak: ");
    scanf("%f",&width1);
    //  fit info
    sprintf(tl,"Left sigma fixed: %.4f",width1);
    TText *t1=histp[hindex].hhf[findex].AddText(tl); 
    t1.SetTextFont(100);
   }
   else{
    widlim1=0;
    width1=.5;
    //  fit info
    sprintf(tl,"Left sigma unfixed, guess = %.4f",width1);
    TText *t1=histp[hindex].hhf[findex].AddText(tl); 
    t1.SetTextFont(100);
   }
                                                                                     // specfit.c             
   printf("Fix width of right peak? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='y'){
    widlim2=1;
    printf("Enter sigma of right peak: ");
    scanf("%f",&width2);
    //  fit info
    sprintf(tl,"Right sigma fixed: %.4f",width2);
    TText *t1=histp[hindex].hhf[findex].AddText(tl); 
    t1.SetTextFont(100);
   }
   else{
    widlim2=0;
    width2=.1;   
    //  fit info
    sprintf(tl,"Right sigma unfixed, guess = %.4f",width2);
    TText *t1=histp[hindex].hhf[findex].AddText(tl); 
    t1.SetTextFont(100);
   }
  }

  // choose whether to fix constant //
  printf("Fix constant? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   conlim=1;
   printf("Enter constant: ");
   scanf("%f",&gconst);
   //  fit info
   sprintf(tl,"constant fixed at %.4f",gconst);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  else{
   printf("Enter approximate constant: ");
   scanf("%f",&gconst);
   conlim=0;
   sprintf(tl,"constant guess = %.4f",gconst);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
 
  //  define fit function and set initial parameters and limits  //
  if (widequ==0){
   TF1 *g2 = new TF1("g2","[0]*exp(-0.5*((x-[01])/[2])^2)+[3]*exp(-0.5*((x-[1]-[4])/[5])^2)+[6]",wavelo,wavehi);
  }
  else{
   TF1 *g2 = new TF1("g2","[0]*exp(-0.5*((x-[01])/[2])^2)+[3]*exp(-0.5*((x-[1]-[4])/[2])^2)+[5]",wavelo,wavehi);
  }
  g2.SetParName(0,"height1");
  g2.SetParName(1,"center1");
  g2.SetParName(2,"sigma1");
  g2.SetParName(3,"height2");
  g2.SetParName(4,"separation");
  if (widequ==0) {g2.SetParName(5,"sigma2"); g2.SetParName(6,"const");}
  else g2.SetParName(5,"const");
  g2.SetParameter(0,height[1]);
  g2.SetParameter(1,center[1]);
  g2.SetParameter(2,width1);
  g2.SetParameter(3,height[2]);
  g2.SetParameter(4,sep[1]);
  if (widequ==0) g2.SetParameter(5,width2);
  if(seplim==1){g2.SetParLimits(4,1,1);g2.SetParameter(4,sep[1]);}
  if(widlim1==1)g2.SetParLimits(2,width1,width1);
  if(widlim2==1)g2.SetParLimits(5,width2,width2);
  if(widequ==0){ g2.SetParameter(6,gconst);
   if(conlim==1) {g2.SetParLimits(6,1,1); g2.SetParameter(6,gconst);}
  }
  else{ g2.SetParameter(5,gconst);
   if(conlim==1) {g2.SetParLimits(5,1,1); g2.SetParameter(5,gconst);}
  }
  

                                                                                     // specfit.c             
  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("g2","RB+");
  else histp[hindex].hh.Fit("g2","RB");
  printf("      center2     %.5e  %.5e\n",g2.GetParameter(1)+g2.GetParameter(4),
         sqrt(pow(g2.GetParError(1),2)+pow(g2.GetParError(4),2)));

  //  fit info
  strcpy(tl,"fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"height #1 = %f +/- %f, #2 = %f +/- %f",g2.GetParameter(0),g2.GetParError(0),g2.GetParameter(3),g2.GetParError(3));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"             center #1 = %f +/- %f, #2 = %f +/- %f             ",g2.GetParameter(1),g2.GetParError(1),g2.GetParameter(1)+g2.GetParameter(4),sqrt(pow(g2.GetParError(1),2)+pow(g2.GetParError(4),2)));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  if(widequ==0)sprintf(tl,"sigma #1 = %f +/- %f, #2 = %f +/- %f",g2.GetParameter(2),g2.GetParError(2),g2.GetParameter(5),g2.GetParError(5));
  else sprintf(tl,"sigma = %f +/- %f",g2.GetParameter(2),g2.GetParError(2));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  if(widequ==0)sprintf(tl,"constant = %f +/- %f",g2.GetParameter(6),g2.GetParError(6));
  else sprintf(tl,"constant = %f +/- %f",g2.GetParameter(5),g2.GetParError(5));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",g2.GetChisquare(),g2.GetNDF(),g2.GetChisquare()/g2.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

 break;

 case 3:              // ************* MULTIPLE PEAK FIT **************** //
  int numpeaks;
  char funcexp[400], funcbit[40], pname[20];

  printf("Enter number of peaks: ");
  scanf("%d",&numpeaks);
                                                                                     // specfit.c             
  //  fit info
  sprintf(tl,"%d peak fit",numpeaks);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);

  //  set x range  //
  printf("  p: Use previous wavelength limits (%.2f,%.2f)\n",wavelo,wavehi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower wavelength limit: ");
   scanf("%f",&wavelo);
   printf("Enter upper wavelength limit: ");
   scanf("%f",&wavehi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   wavelo=histp[hindex].hhl[0].GetX1();
   wavehi=histp[hindex].hhl[1].GetX1();
  }

  //  fit info
  sprintf(tl,"Wavelength limits: %.3f - %.3f",wavelo,wavehi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  height guesses  //
  for(l=1;l<=numpeaks;l++){
   printf("Enter approximate height of peak %d: ",l);
   scanf("%f",&height[l]);
  }
                                                                                     // specfit.c             
  //  fit info
  sprintf(tl,"Height guesses: ");
  for(l=1;l<=numpeaks;l++){sprintf(tlb,"%d: %d,  ",l,height[l]);strcat(tl,tlb);}
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  
  //  centers  //
  printf("Fix separations? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   seplim=1;
   for(l=1;l<=numpeaks;l++){
    if(l==1) printf("Enter center of peak %d (unfixed): ",l);
    else printf("Enter center of peak %d: ",l);
    scanf("%f",&center[l]);    
   }
  }
  else{
   seplim=0;
   for(l=1;l<=numpeaks;l++){
    printf("Enter approximate center of peak %d: ",l);
    scanf("%f",&center[l]); 
   }   
  }

  //  fit info
  sprintf(tl,"Center guesses: ");
  for(l=1;l<=4;l++)if(l<=numpeaks){sprintf(tlb,"%d: %.3f, ",l,center[l]);strcat(tl,tlb);}
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  strcpy(tl,"");
  if(numpeaks>4){
   for(l=5;l<=numpeaks;l++){sprintf(tlb,"%d: %.3f, ",l,center[l]);strcat(tl,tlb);}
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
                                                                                     // specfit.c             
  //  separations  //
  for(l=1;l<=numpeaks-1;l++){
   sep[l] = center[l+1]-center[1];
  }

  //  widths  //
  printf("Fix width of peaks? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   widlim1=1;
   printf("Enter sigma of peaks: ");
   scanf("%f",&width1);
  }
  else{
   widlim1=0;
   printf("Enter approximate sigma of peaks: ");
   scanf("%f",&width1);
  }

  // constant //
  printf("Fix constant? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   conlim=1;
   printf("Enter constant: ");
   scanf("%f",&gconst);
   //  fit info
   sprintf(tl,"constant fixed at %.4f",gconst);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  else{
   printf("Enter approximate constant: ");
   scanf("%f",&gconst);
   conlim=0;
   sprintf(tl,"constant guess = %.4f",gconst);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }  

  //  fit info
  sprintf(tl,"Sigma guess: %.4f",width1);
  if(widlim1==1){strcpy(tlb," fixed");strcat(tl,tlb);}
  if(seplim==1){strcpy(tlb,"; separations fixed");strcat(tl,tlb);}
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  construct string for function  //
  strcpy(funcexp,"[2]*exp(-0.5*((x-[1])/[0])^2)");
  for(l=2;l<=numpeaks;l++){
   sprintf(funcbit,"+[%d]*exp(-0.5*((x-[1]-[%d])/[0])^2)",2*l,2*l-1);
   strcat(funcexp,funcbit);   
  }
  sprintf(funcbit,"+[%d]",2*numpeaks+1);
  strcat(funcexp,funcbit);
                                                                                     // specfit.c             
  //  define fit function and set initial parameters and limits  //
  TF1 *gm = new TF1("gm",funcexp,wavelo,wavehi);
  //  parameter names  //
  gm.SetParName(0,"sigma");
  gm.SetParName(1,"center1");
  gm.SetParName(2,"height1");
  for(l=2;l<=numpeaks;l++){
   sprintf(pname,"sep %d",l-1);
   gm.SetParName(2*l-1,pname);
   sprintf(pname,"height %d",l);
   gm.SetParName(2*l,pname);
  }
  gm.SetParName(2*numpeaks+1,"const");
  //  parameter initial values  //
  gm.SetParameter(0,width1);
  gm.SetParameter(1,center[1]);
  gm.SetParameter(2,height[1]);
  for(l=2;l<=numpeaks;l++){
   gm.SetParameter(2*l-1,sep[l-1]);
   gm.SetParameter(2*l,height[l]);
  }
  gm.SetParameter(2*numpeaks+1,gconst);
  //  parameter limits  //
  if(seplim==1)for(l=2;l<=numpeaks;l++)gm.SetParLimits(2*l-1,sep[l-1],sep[l-1]);
  if(widlim1==1)gm.SetParLimits(0,width1,width1);
  if(conlim==1){
    gm.SetParLimits(2*numpeaks+1,1,1);
    gm.SetParameter(2*numpeaks+1,gconst);
  }

  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("gm","RB+");
  else histp[hindex].hh.Fit("gm","RB");
 
  //  display center info  //  
  for(l=2;l<=numpeaks;l++){
   printf("      center%-2d    %.5e  %.5e\n",l,gm.GetParameter(1)+gm.GetParameter(2*l-1),
          sqrt(pow(gm.GetParError(1),2)+pow(gm.GetParError(2*l-1),2)));
  }
                                                                                     // specfit.c             
  //  fit info
  strcpy(tl,"Fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  //  heights  *************
  sprintf(tl,"            Height ");
  for(l=1;l<=3;l++){
   if(l<=numpeaks){
    sprintf(tlb,"#%d=%f+/-%f, ",l,gm.GetParameter(2*l),gm.GetParError(2*l));
    strcat(tl,tlb);
   }
  }
  strcat(tl,"           ");
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  if(numpeaks>3){
   strcpy(tl,"");
   for(l=4;l<=6;l++){
    if(l<=numpeaks){
     sprintf(tlb,"#%d=%f+/-%f, ",l,gm.GetParameter(2*l),gm.GetParError(2*l));
     strcat(tl,tlb);
    } 
   }
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }  
  if(numpeaks>6){
   strcpy(tl,"");
   for(l=7;l<=numpeaks;l++){
    sprintf(tlb,"#%d=%f+/-%f, ",l,gm.GetParameter(2*l),gm.GetParError(2*l));
    strcat(tl,tlb);
   }
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }  
  //  centers  **************
  sprintf(tl,"            Center ");
  sprintf(tlb,"#1=%f+/-%f, ",gm.GetParameter(1),gm.GetParError(1));
  strcat(tl,tlb);
  for(l=2;l<=3;l++){
   if(l<=numpeaks){
    sprintf(tlb,"#%d=%f+/-%f, ",l,gm.GetParameter(1)+gm.GetParameter(2*l-1),sqrt(pow(gm.GetParError(1),2)+pow(gm.GetParError(2*l-1),2)));
    strcat(tl,tlb);
   }
  }
  strcat(tl,"           ");
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  if(numpeaks>3){
   strcpy(tl,"");
   for(l=4;l<=6;l++){
    if(l<=numpeaks){
     sprintf(tlb,"#%d=%f+/-%f, ",l,gm.GetParameter(1)+gm.GetParameter(2*l-1),sqrt(pow(gm.GetParError(1),2)+pow(gm.GetParError(2*l-1),2)));
     strcat(tl,tlb);
    } 
   }
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }  
  if(numpeaks>6){
   strcpy(tl,"");
   for(l=7;l<=numpeaks;l++){
    sprintf(tlb,"#%d=%f+/-%f, ",l,gm.GetParameter(1)+gm.GetParameter(2*l-1),sqrt(pow(gm.GetParError(1),2)+pow(gm.GetParError(2*l-1),2)));
    strcat(tl,tlb);
   }
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }  
  //  width etc.  **************
  sprintf(tl,"sigma = %f +/- %f",gm.GetParameter(0),gm.GetParError(0));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"constant = %f +/- %f",gm.GetParameter(2*numpeaks+1),gm.GetParError(2*numpeaks+1));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",gm.GetChisquare(),gm.GetNDF(),gm.GetChisquare()/gm.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
                                                                                     // specfit.c             
 break;


 case 5:          // *********** VOIGT PROFILE FIT ***************** //
  //  print type of fit to fitbox
  strcpy(tl,"Voigt profile fit"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);
  
  //  set x range //
  printf("  p: Use previous wavelength limits (%.2f,%.2f)\n",wavelo,wavehi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower wavelength limit: ");
   scanf("%f",&wavelo);
   printf("Enter upper wavelength limit: ");
   scanf("%f",&wavehi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   wavelo=histp[hindex].hhl[0].GetX1();
   wavehi=histp[hindex].hhl[1].GetX1();
  }
                                                                                     // specfit.c             
  //  fit info
  sprintf(tl,"Wavelength limits: %.3f - %.3f",wavelo,wavehi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  define fit function and set initial parameter guesses  //
  TF1 *v1 = new TF1("v1","[0]*((1-[3])*exp(-2.773*(x-[1])^2/[2]^2) + [3] / (1+4*(x-[1])^2/[2]^2)+0.016*(1-[3])*[3]*(exp(-0.4*abs((x-[1])/[2])^2.25)-10/(10+abs((x-[1])/[2])^2.25)))+[4]",wavelo,wavehi);
  v1.SetParName(0,"height");
  v1.SetParName(1,"center");
  v1.SetParName(2,"gwidth");
  v1.SetParName(3,"lfactor");
  v1.SetParName(4,"const");
  v1.SetParameter(0,histp[hindex].hh.GetBinContent(int(((wavelo+wavehi)/2-histp[hindex].hh.GetBinLowEdge(1))/histp[hindex].hh.GetBinWidth(1))+1));
  v1.SetParameter(1,(wavelo+wavehi)/2);
  v1.SetParameter(2,(wavehi-wavelo)/3);
  v1.SetParameter(3,0.5);
  v1.SetParameter(4,0.0);
  
                                                                                     // specfit.c             
  // choose whether to fix constant //
  printf("Fix constant? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   conlim=1;
   printf("Enter constant: ");
   scanf("%f",&gconst);
   v1.SetParLimits(4,1,1);
   v1.SetParameter(4,gconst);
   //  fit info
   sprintf(tl,"constant fixed at %.4f",gconst);
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  else{
   conlim=0;
   printf("Enter approximate constant: ");
   scanf("%f",&gconst);
   v1.SetParameter(4,gconst);
  }



  //  fit info
  sprintf(tl,"            Fit guesses: height=%.1f, center=%.2f, gwidth=%.2f, lfactor=%.2f, const=%.2f            ",v1.GetParameter(0),v1.GetParameter(1),v1.GetParameter(2),v1.GetParameter(3),v1.GetParameter(4));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("v1","VRB+");
  else histp[hindex].hh.Fit("v1","VRB");

  //  fit info
  strcpy(tl,"fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"height = %f +/- %f",v1.GetParameter(0),v1.GetParError(0));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"center = %f +/- %f",v1.GetParameter(1),v1.GetParError(1));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"gwidth = %f +/- %f",v1.GetParameter(2),v1.GetParError(2));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"lfactor = %f +/- %f",v1.GetParameter(3),v1.GetParError(3));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"constant = %f +/- %f",v1.GetParameter(4),v1.GetParError(4));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",v1.GetChisquare(),v1.GetNDF(),v1.GetChisquare()/v1.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

 break;
}  


//  display fit info
TCanvas *cex = (TCanvas*)gROOT->FindObject("cfit");
if (!cex) {
 TCanvas cfit("cfit","fit info",480,280,703,503);
 cfit.Range(0,0,100,100);
}
else cfit.cd();
cfit.Clear();
for(l=1;l<=histp[hindex].hhnf;l++)histp[hindex].hhf[l].Draw();
  
//  redraw histogram in appropriate canvas  //
histp[hindex].hhc.cd();
histp[hindex].hh.Draw("e");
histp[hindex].hhl[0].Draw();
histp[hindex].hhl[1].Draw();

//                                                                               end of specfit.c
 break;//=======================================================================================

 case 16://=======================================================================================
// fitinfo.c                                                                            fitinfo.c
// displays fit information for a histogram
// (this is also done automatically after a fit)

if(hx==0){
  printf("sorry, no spectra or decay curves exist\n");
  break;
}

// find histogram
char dname[30],htype[1];
int hfd=0;

printf("Enter histogram name: ");
scanf("%s",dname);

int hindex,l,findx=0;
for(int l=1;l<40;l++)if(strcmp(histp[l].hhn,dname)==0){hindex=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find that histogram\n");break;}

// create or activate canvas for fit information
TCanvas *cex = (TCanvas*)gROOT->FindObject("cfit");
if (!cex) {
 TCanvas cfit("cfit","fit info",480,280,703,503);
 cfit.Range(0,0,100,100);
}
else cfit.cd();

// draw fit information boxes
cfit.Clear();
for(l=1;l<=histp[hindex].hhnf;l++){histp[hindex].hhf[l].Draw();findx++;}

printf("%d fits found\n",findx);

// end of fitinfo.c                                                              end of fitinfo.c
 break;//=======================================================================================

 case 18://=======================================================================================
// histsum.c                                                                            histsum.c
// add two histograms with weighting factors
// histograms must have identical binning


if(hx==0){
  printf("sorry, no spectra or decay curves exist\n");
  break;
}

char hpname[30],cpname[30],hplabel[50],dname[30],nm[30];
float posn1,posn2,wf1,wf2,bc,be;
int l,hindex1,hindex2,hfd;

hx++;
sumx++;

//first histo to be added *****************************************************

printf("Enter name of first histogram: ");
scanf("%s",hpname);
printf("Enter weighting factor: ");
scanf("%f",&wf1);

// set first half of title for new histogram
sprintf(nm,"%.3f*",wf1);
strcpy(hplabel,nm);
strcat(hplabel,hpname);
strcat(hplabel," + ");
                                                                                     // histsum.c             
hfd=0;
for(int l=1;l<70;l++)if(strcmp(histp[l].hhn,hpname)==0){hindex1=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find that histogram\n");break;}

//second histo to be added *****************************************************

printf("Enter name of second histogram: ");
scanf("%s",hpname);
printf("Enter weighting factor: ");
scanf("%f",&wf2);

// set second half of title for new histogram
sprintf(nm,"%.3f*",wf2);
strcat(hplabel,nm);
strcat(hplabel,hpname);

hfd=0;
for(int l=1;l<70;l++)if(strcmp(histp[l].hhn,hpname)==0){hindex2=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find that histogram\n");break;}

//new histo *******************************************************************

// create new histo identical to first to set bins
hsum[sumx]=(*histp[hindex1].hh);

printf("Enter identifier for new histogram: ");
scanf("%s",nm);
sprintf(hpname,"sum%s",nm);
//-----------------------------
FILE *fout;
  char nameout[80],*no;
  sprintf(nameout,"C:/Rob Learning Stuff/Root Outputs/sum.txt");
  no=nameout;
  printf("output: %s\n",nameout);
  fout=fopen(no,"w");
//----------------------------------  


// new histo name is "sum" + user-chosen identifier 

hsum[sumx].SetName(hpname);
                                                                                     // histsum.c
histp[hx].hh = (TH1D*)gROOT.FindObject(hpname);
// set name in structure and initialize number of fits
strcpy(histp[hx].hhn,hpname); 
histp[hx].hhnf=0;

// set bin contents and errors of new histogram
Int_t nx=histp[hindex1].hh.GetNbinsX();
for(l=1;l<=nx;l++){
 //bc=wf1*(histp[hindex1].hh.GetBinContent(l)) + wf2*(histp[hindex2].hh.GetBinContent(l));
 //histp[hx].hh.SetBinContent(l,bc);
 //histp[hx].hh.SetBinError(l,sqrt(pow(wf1*histp[hindex1].hh.GetBinError(l),2)+pow(wf2*histp[hindex2].hh.GetBinError(l),2)));
 float bc=wf1*(histp[hindex1].hh.GetBinContent(l)) + wf2*(histp[hindex2].hh.GetBinContent(l));
 float be=sqrt(pow(wf1*histp[hindex1].hh.GetBinError(l),2)+pow(wf2*histp[hindex2].hh.GetBinError(l),2));
 histp[hx].hh.SetBinContent(l,bc);
 histp[hx].hh.SetBinError(l,be);
 //printf("%f\t %f\t %f\n",l,bc,be);
 fprintf(fout,"%d\t%f\t%f\n",l,bc,be);


}
 fclose(fout);
histp[hx].hh.SetTitle(hplabel);

sprintf(cpname,"%sc",hpname);

// values for canvas position defined at initialization and incremented for cascade effect
// canvas title is hist name, canvas name is hist name plus "c"
cx+=20;cy+=30;if(cy>280){cx=360;cy=10;}
histp[hx].hhc=new TCanvas(cpname,hpname,cx,cy,703,503);
 gPad->SetTicks(1,1);
   gPad->SetGridx();
   gPad->SetGridy();
histp[hx].hh.Draw("e");

// create and draw limit lines
histp[hx].hhl[0]=new TLine(histp[hx].hh.GetBinLowEdge(1),-500,histp[hx].hh.GetBinLowEdge(1),1.5*histp[hx].hh.GetMaximum());
histp[hx].hhl[0].SetLineColor(46);
histp[hx].hhl[0].Draw();
histp[hx].hhl[1]=new TLine(histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),-500,histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),1.5*histp[hx].hh.GetMaximum());
histp[hx].hhl[1].SetLineColor(38);
histp[hx].hhl[1].Draw();

// end of histsum.c                                                              end of histsum.c
 break;//=======================================================================================

 case 19://=======================================================================================
//  histsave.c                                                                         histsave.c
//  saves a spectrum or decay curve along with fit information text boxes
//  into a file; file is given same name as histogram plus a letter code
//  which increments if identically-named file already exists

if(hx==0 && hx==0){
  printf("sorry, no spectra or decay curves exist\n");
  break;
}

char fname[60], fbname[30], hname[30], nm[30], newname[30];
int l,num,hindex,hfd=0;    

// find histogram to be saved
printf("Enter histogram name: ");
scanf("%s",hname);
for(int l=1;l<70;l++)if(strcmp(histp[l].hhn,hname)==0){hindex=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find that histogram\n");break;}

// give it a new name (tack on an "a")
sprintf(newname,"%sa",hname);
sprintf(fname,"%s%s%s",namepref,newname,namesuff);

// search for identically named saved file and increment last letter if such a file exists
FILE *ftest;
while(ftest=fopen(fname,"r")){
 fclose(ftest);
 newname[strlen(newname)-1]++;
 sprintf(fname,"%s%s%s",namepref,newname,namesuff);
}

// create new file and save histo with new name
TFile f1(fname,"NEW");
f1.cd();
histp[hindex].hh.SetName(newname);
histp[hindex].hh.Write();

// restore original name of histo for remainder of session
histp[hindex].hh.SetName(hname);

// give names to fitboxes (fitbox0, fitbox1 etc) and save them as well
strcpy(fbname,"fitbox0");
for(l=1;l<=histp[hindex].hhnf;l++){
 fbname[6]++;
 histp[hindex].hhf[l].SetName(fbname);
 histp[hindex].hhf[l].Write();
}
 
f1.Close();
printf("Name of histogram file: %s\n",fname);
// end of histsave.c                                                            end of histsave.c
 break;//=======================================================================================

 case 2://=======================================================================================
//                                                                                     psd2ntup.c
//   routine to get PSD data from an ascii file and
//   to create a new root file and an ntuple
//   location of PSD data and Root file determined by 
//   "psdpref" and "namepref", respectively, defined in initialization
//   calculates "alpha" from which lambda is calculated elsewhere
//   also creates a new ascii file with just headers (prev. psd2head.c)

   char namebit[50], namepsd[50], namert[50], yesorno[10], namehead[50], nameext[5];   
   
//   determine names for files: raw data, ntuple, header
   printf("Enter desired PSD dataset name (w/out extension): ");
   scanf("%s",namebit);
   printf("Enter extension (e.g. \"dc\",\"sp\",\"pl\"): ");
   scanf("%s",nameext);
   sprintf(namepsd,"%s%s.%s",psdpref,namebit,nameext);
   sprintf(namert,"%s%s.root",namepref,namebit);
   sprintf(namehead,"%s%shead.dat",namepref,namebit);

//   open raw data file in read mode
   FILE *fp = fopen(namepsd,"r");
   if (fp <= 0) {
     printf("Sorry, cannot open file\n");
     break;
   }

   Float_t lam,pos,sig,om,fc,time,v1,v2,alpha;
   Int_t ncols, nlines,ndat;
                                                                                    // psd2ntup.c             
//   create Root file and ntuple
   TFile *f = new TFile(namert,"RECREATE");
   
   TNtuple *ntupll = new TNtuple("ntuple","data from ascii file","lam:pos:sig:om:fc:time:v1:v2:alpha");
 
//   create header file (ascii)
   FILE *fhead = fopen(namehead,"w");
   fprintf(fhead,"lam        pos      sig     om       fc      time       \n");
   
//   read raw data and fill ntuple and header
   int numz=0;
   while (fp) {
      //   get a header
      ncols = fscanf(fp,"%f %f %f %f %f %f",&lam, &pos, &sig, &om, &fc, &time);
      if (ncols < 0) break;
      printf("lam=%8.3f, pos=%8.3f, sig=%8.0f\n",lam,pos,sig);

      if (pos>100) {
         printf("\nThese look wrong. It's possible that the previous \"sig\" value\n");
         printf("is wrong. Do you wish to abort(y/n)? ");
         scanf("%s",yesorno);
         if (yesorno[0]=='y') break;
      } 
      //   macro makes use of the value "sig" from the header and expects this many pairs
      //   of v1 and v2 before next header. Above attempts to detect incorrect sig; 
      //   "pos" values are generally smallest, other values tend to be large integers
      //   assumption: no pos greater than 50 mm      
                  
      //   print header values to header file
      fprintf(fhead,"%-11.3f%-9.3f%-8d%-9d%-8d%-10d\n",lam,pos,sig,om,fc,time);    

      //   collect events and fill ntuple
      nlines=(int)sig;
	for (int i=0; i<nlines;i++){
		ndat =  fscanf(fp,"%f %f", &v1, &v2);
		if (i<5) printf("v1=%8.0f, v2=%8.0f,\n",v1,v2);
                if(v1+v2-4096==0){alpha=v1;printf("ZERO VALUE:v1=%d,v2=%d\n",v1,v2);numz++;}
		else alpha=v1/(v1+v2-4096.0);
                //   alpha defined here as v1 over sum, in effect redefined in ntup(s)2hist2d.c
		ntupll->Fill(lam,pos,sig,om,fc,time,v1,v2,alpha);
	}
   }
   if(numz>0)printf("\nNumber of zero corrections = %d\n",numz); 

   //   close files
   fclose(fp);
   f->Write();
   f.Close();
   fclose(fhead);

   printf("\ninput file: %s\nntuple file (root): %s\n",namepsd,namert);
   printf("header file (ascii): %s\n\n",namehead);
   
//                                                                              end of psd2ntup.c
 break;//=======================================================================================

 case 3://=======================================================================================
//   monitor.c                                                                                   monitor.c
//   macro to read data from header file (created in psd2ntup.c) or raw PMT file and
//   graph OM,FC and the ratio OM/FC vs foil position.
 
   Float_t xmon[500], optical[500], faraday[500], ratio[500];
   Float_t fc0, om0;
   Float_t xmin,xmax,ymin=0.75,ymax=1.25;
//   Axis_t xmin,xmax,ymin=0.75,ymax=1.25;
   int iscomplex=0;
 
   cmon = new TCanvas("cmon","Monitor",200,10,700,500);

   cmon->SetFillColor(19);
   cmon->SetTicks(1,1);
   cmon->SetGridx();
   cmon->SetGridy();
   cmon->GetFrame()->SetFillColor(19);
   cmon->GetFrame()->SetBorderSize(2);
 
//   interrogate user for data file name

   char namebit[50], namehead[50], st[10], yesorno[10], nameext[5], ispsd[5];   
//   printf("Is this a PSD dataset? (y/n): ");
//   scanf("%s",ispsd);
//   if(ispsd[0]=='y'){
//    printf("Enter desired PSD dataset name (w/out extension): ");
//    scanf("%s",namebit);
//    sprintf(namehead,"%s%shead.dat",namepref,namebit);
//   }
//   else{
    printf("Enter desired PMT dataset name (w/out extension): ");
    scanf("%s",namebit);
    strcpy(nameext,"sp");
    sprintf(namehead,"%s%s.%s",psdpref,namebit,nameext);
    FILE *fp = fopen(namehead,"r");
    if (fp <= 0){
     strcpy(nameext,"dc");
     sprintf(namehead,"%s%s.%s",psdpref,namebit,nameext);
     FILE *fp = fopen(namehead,"r");
//    }
    if (fp <= 0){
     strcpy(nameext,"pl");
     sprintf(namehead,"%s%s.%s",psdpref,namebit,nameext);
     FILE *fp = fopen(namehead,"r");
     iscomplex=1;
    } 
    if (fp <= 0) printf("Sorry, cannot open file\n");
    printf("\ninput file: %s\n\n",namehead);
   }

   printf("\ninput file: %s\n\n",namehead);
    
   FILE *fp = fopen(namehead,"r");
   if (fp <= 0) printf("Sorry, cannot open file\n");
 
   Float_t lam,pos,sig,om,fc,time;
   Int_t ncols;
   Int_t nlines = 0;
   
//   get rid of first line of header file 
   if(ispsd[0]=='y')fscanf(fp,"%s %s %s %s %s %s",st,st,st,st,st,st);
                                                                                     // monitor.c             
//   fill arrays with data
   Int_t i=0;
   while (fp) {
      ncols = fscanf(fp,"%f %f %f %f %f %f", &lam, &pos, &sig, &om, &fc, &time);
      if (ncols < 0) break;
      if (nlines < 5) printf("lam=%.2f, pos=%.2f, sig=%.2f, om=%.2f\n",lam,pos,sig,om);
 	   if (i==0){om0=om; fc0=fc;}
	   if(ispsd[0]!='y' && nameext[0]=='s') xmon[i]=lam;
           else xmon[i]=pos;
           if(iscomplex==1)xmon[i]=i;
	   optical[i]=om/om0;
	   faraday[i]=fc/fc0;
	   if(faraday[i]!=0)ratio[i]=optical[i]/faraday[i];
           else ratio[i]=2.0;
           if(ymin>optical[i])ymin=optical[i];
           if(ymin>faraday[i])ymin=faraday[i];
           if(ymin>ratio[i])ymin=ratio[i];
           if(ymax<optical[i])ymax=optical[i];
           if(ymax<faraday[i])ymax=faraday[i];
           if(ymax<ratio[i])ymax=ratio[i];
	   i++;
	   nlines++;
   }

   printf(" found %d lines\n",nlines);
   xmax = xmon[nlines-1];
   xmin = xmon[0];
//   assumption: dataset positions in increasing order
   
//   create 2-bin histogram to set axis limits
   TH2F *haxes = new TH2F("haxes", namebit, 2,xmin,xmax,2,ymin,ymax);
   if(ispsd[0]!='y' && nameext[0]=='s') haxes->SetXTitle("monochromator wavelength");
   else haxes->SetXTitle("foil position");
   if(iscomplex==1)haxes->SetXTitle("step");
   haxes->SetYTitle("monitors");
   haxes->Draw();
	
   gr = new TGraph(nlines,xmon,ratio);
   gr->SetFillColor(19);
   gr->SetLineColor(2);
   gr->SetLineWidth(1);
   gr->SetLineStyle(3);	
   gr->SetMarkerColor(2);
   gr->SetMarkerStyle(21);
   gr->SetMarkerSize(0.3);
   gr->SetTitle("monitor");
   gr->Draw("CP");
                                                                                     // monitor.c             
   gr = new TGraph(nlines,xmon,faraday);
   gr->SetFillColor(19);
   gr->SetLineColor(8);
   gr->SetLineWidth(1);
   gr->SetLineStyle(3);	
   gr->SetMarkerColor(8);
   gr->SetMarkerStyle(21);
   gr->SetMarkerSize(0.3);
   gr->Draw("CP");

   gr = new TGraph(nlines,xmon,optical);
   gr->SetFillColor(19);
   gr->SetLineColor(4);
   gr->SetLineWidth(1);
   gr->SetLineStyle(3);	
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->SetMarkerSize(0.3);
   gr->Draw("CP");
 
//TGraph::Paint is called by TCanvas::Update. This function is called by default
//when typing <CR> at the keyboard. In a macro, one must force TCanvas::Update.
 
   cmon->Update();
   
   fclose(fp);

   printf("blue: optical monitor\n");
   printf("green: faraday cup\n");
   printf("red: optical monitor/faraday cup\n");

// end of monitor.c                                                              end of monitor.c
 break;//=======================================================================================

 case 5://=======================================================================================
   // calib.c                                                                             calib.c  
   // calculates new calibration parameters based on old parameters and comparisons 
   // between fit values and actual line values
   // fit to 3rd degree polynomial, 4 parameters

   Int_t n,echoice;
   float cc0,cc1,cc2,cc3,alpha,lam,lambda,lamreal,lamerr;
   float ar,br,ac,bc;
   char yesorno[10];

   printf("Old calibration parameters: c0=%.3f, c1=%.3f, c2=%.3f, c3=%.3f\n",c0,c1,c2,c3);
   printf("Are these correct? (y/n): ");
   scanf("%s",yesorno);
   if(yesorno[0]=='n'){
     printf("Enter new c0: ");
     scanf("%f",&cc0);
     printf("Enter new c1: ");
     scanf("%f",&cc1);
     printf("Enter new c2: ");
     scanf("%f",&cc2);
     printf("Enter new c3: ");
     scanf("%f",&cc3);
   }
   else{cc0=c0;cc1=c1;cc2=c2;cc3=c3;}

   printf("Enter # of points (min 4): ");
   scanf("%d",&n);
   if(n<4){printf("illegal number of points\n");break;}

   printf("Enter 1 if you wish to use errors, otherwise enter 0: ");
   scanf("%d",&echoice);
// errors used in fit if chosen

   switch (echoice){

case 1:   // with errors
                                                                                       // calib.c             
   Float_t x[200], y[200], xe[200], ye[200];
   for (Int_t i=0;i<n;i++) {
    printf("enter lam0: ");
    scanf("%f",&lam);
//  this is "lam" from dataset

    printf("enter calculated wavelength: ");
    scanf("%f",&lambda);

//  the following solves for alpha from calibration parameters
    ac=-(2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27/2
    +sqrt(pow((2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27,2)/4
    +pow((3*cc1/cc3-pow(cc2/cc3,2))/3,3)/27);
    bc=-(2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27/2
    -sqrt(pow((2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27,2)/4
    +pow((3*cc1/cc3-pow(cc2/cc3,2))/3,3)/27);
    if(ac<0)ar=-pow(-ac,(1.0/3.0));
    else ar=pow(ac,(1.0/3.0));
    if(bc<0)br=-pow(-bc,(1.0/3.0));
    else br=pow(bc,(1.0/3.0));
    alpha=ar+br-cc2/cc3/3;
    x[i]=alpha;

    printf("alpha: %f\n",alpha);

    printf("enter error in calculated wavelength: ");
    scanf("%f",&lamerr);
    lambda+=lamerr;
                                                                                       // calib.c             
//  error in alpha calculated below
    ac=-(2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27/2
    +sqrt(pow((2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27,2)/4
    +pow((3*cc1/cc3-pow(cc2/cc3,2))/3,3)/27);
    bc=-(2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27/2
    -sqrt(pow((2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27,2)/4
    +pow((3*cc1/cc3-pow(cc2/cc3,2))/3,3)/27);
    if(ac<0)ar=-pow(-ac,(1.0/3.0));
    else ar=pow(ac,(1.0/3.0));
    if(bc<0)br=-pow(-bc,(1.0/3.0));
    else br=pow(bc,(1.0/3.0));
    alpha=ar+br-cc2/cc3/3;

    xe[i]=alpha-x[i];

    printf("enter actual wavelength: ");
    scanf("%f",&lamreal);
    y[i]=lamreal-lam;
    ye[i]=0;
   }
   gre = new TGraphErrors(n,x,y,xe,ye);
 
   gre.Fit("pol3");

   break;

case 0:

   Float_t x[200], y[200];
   for (Int_t i=0;i<n;i++) {
    printf("enter lam0: ");
    scanf("%f",&lam);
//  this is "lam" from dataset
                                                                                       // calib.c             
    printf("enter calculated wavelength: ");
    scanf("%f",&lambda);

//  the following solves for alpha from calibration parameters
    ac=-(2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27/2
    +sqrt(pow((2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27,2)/4
    +pow((3*cc1/cc3-pow(cc2/cc3,2))/3,3)/27);
    bc=-(2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27/2
    -sqrt(pow((2*pow(cc2/cc3,3)-9*cc2/cc3*cc1/cc3+27*(cc0-lambda+lam)/cc3)/27,2)/4
    +pow((3*cc1/cc3-pow(cc2/cc3,2))/3,3)/27);
    if(ac<0)ar=-pow(-ac,(1.0/3.0));
    else ar=pow(ac,(1.0/3.0));
    if(bc<0)br=-pow(-bc,(1.0/3.0));
    else br=pow(bc,(1.0/3.0));
    alpha=ar+br-cc2/cc3/3;
    x[i]=alpha;
    
    printf("alpha: %f\n",alpha);

    printf("enter actual wavelength: ");
    scanf("%f",&lamreal);
    y[i]=lamreal-lam;
   }
   gr = new TGraph(n,x,y);

   gr.Fit("pol3");
 
   break;

}
// end of calib.c                                                                  end of calib.c
 break;//=======================================================================================

 case 15://=======================================================================================
//  decfit.c                                                                             decfit.c
//  fitting options for a decay curve
//  also creates fit information text boxes, viewable with fitinfo.c

if(hx==0){
  printf("sorry, no decay curves exist\n");
  break;
}

int saveprev,fitchoice,l,hindex,findex,conlim,fitclr=2,hfd=0,rislim,taulim;
Float_t poslo, poshi, lamb, max[10], tau[10], exconst, avval, risex, yconst;
char yesorno[10], tl[100], tlb[50],dname[30],nm[30],hlname[30],clname[30];

//  select histogram to fit  //
printf("Enter histogram name: ");
scanf("%s",hlname);
for(int l=1;l<90;l++)if(strcmp(histp[l].hhn,hlname)==0){hindex=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find that histogram\n");break;}

//  select appropriate canvas //
histp[hindex].hhc.cd();

//  if previous fits are not to be saved, set number of fits to zero
//  findex is the number of the current fit
printf("Save previous fits? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y')saveprev=1;
else{saveprev=0;histp[hindex].hhnf=0;}
findex=++histp[hindex].hhnf;
                                                                                      // decfit.c             
//  create fit info box
//  give it a random position in canvas
float xsh=8.0/32767.0*rand()-4;float ysh=8.0/32767.0*rand()-4;
histp[hindex].hhf[findex]=new TPaveText(5+xsh,5+ysh,95+xsh,95+ysh);
//  set color of fit
fitclr=(histp[hindex].hhnf-1)%6+2;
if(fitclr>4)fitclr++;
//  add title of histo to fit box and calibration parameters
//  (note: cal params written are most recent settings and may not truly apply to histo)
strcpy(tl,histp[hindex].hh.GetTitle());
TText *t1=histp[hindex].hhf[findex].AddText(tl); 
t1.SetTextFont(100);
sprintf(tl,"                Cal params: c0=%.3f, c1=%.3f, c2=%.3f, c3=%.3f                ",c0,c1,c2,c3);
TText *t1=histp[hindex].hhf[findex].AddText(tl); 
t1.SetTextFont(100);

//  set fit appearance options
gStyle->SetOptFit(1);
gStyle->SetFuncColor(fitclr);
gStyle->SetFuncWidth(2);

//  fit options  //
printf("  1: Single exponential plus constant\n");
printf("  2: 2-9 exponentials plus constant\n");
printf("  3: Horizontal line\n");
printf("  4: as 1, with risetime\n");
printf("  5: as 1, with risetime (v2)\n");
printf("Enter option: ");
scanf("%d",&fitchoice);

switch (fitchoice){
 case 1:          // *********** SINGLE EXPONENTIAL ***************** //
  //  fit info
  strcpy(tl,"Single exponential plus constant fit"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);
                                                                                      // decfit.c             
  //  set x range //
  printf("  p: Use previous position limits (%.2f,%.2f)\n",poslo,poshi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower position limit: ");
   scanf("%f",&poslo);
   printf("Enter upper position limit: ");
   scanf("%f",&poshi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   poslo=histp[hindex].hhl[0].GetX1();
   poshi=histp[hindex].hhl[1].GetX1();
  }

  //  fit info
  sprintf(tl,"Position limits: %.3f - %.3f",poslo,poshi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  define fit function  //
  TF1 *e1 = new TF1("e1","[0]*exp(-x/[1])+[2]",poslo,poshi);
  e1.SetParName(0,"max");
  e1.SetParName(1,"tau");
  e1.SetParName(2,"const");

  //  set guess parameters  //
  printf("Enter approximate maximum: ");
  scanf("%f",&max[0]);
  printf("Enter approximate tau: ");
  scanf("%f",&tau[0]);
                                                                                      // decfit.c             
  printf("Fix constant? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   conlim=1;
   printf("Enter constant: ");
  }
  else{
   conlim=0;
   printf("Enter approximate constant: ");
  }
  scanf("%f",&exconst);

  e1.SetParameter(0,max[0]);
  e1.SetParameter(1,tau[0]);
  if(conlim==1)e1.SetParLimits(2,1,1);
  e1.SetParameter(2,exconst);

  //  fit info
  sprintf(tl,"Fit guesses: max=%.1f, tau=%.2f, constant=%.1f",max[0],tau[0],exconst);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  if(conlim==1){
   sprintf(tl,"Constant fixed");
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }

  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("e1","RB+");
  else histp[hindex].hh.Fit("e1","RB");
                                                                                      // decfit.c             
  //  fit info
  strcpy(tl,"fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"max = %f +/- %f",e1.GetParameter(0),e1.GetParError(0));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"tau = %f +/- %f",e1.GetParameter(1),e1.GetParError(1));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"constant = %f +/- %f",e1.GetParameter(2),e1.GetParError(2));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",e1.GetChisquare(),e1.GetNDF(),e1.GetChisquare()/e1.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

 break;


 case 2:              // ************* MULTIPLE EXPONENTIALS **************** //
  int numexps;
  char funcexp[400], funcbit[40], pname[20];

  printf("Enter number of exponentials: ");
  scanf("%d",&numexps);

  //  fit info
  sprintf(tl,"%d exponential plus constant fit",numexps);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);
                                                                                      // decfit.c             
  //  set x range //
  printf("  p: Use previous position limits (%.2f,%.2f)\n",poslo,poshi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower position limit: ");
   scanf("%f",&poslo);
   printf("Enter upper position limit: ");
   scanf("%f",&poshi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   poslo=histp[hindex].hhl[0].GetX1();
   poshi=histp[hindex].hhl[1].GetX1();
  }

  //  fit info
  sprintf(tl,"Position limits: %.3f - %.3f",poslo,poshi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  max and tau guesses  //
  for(l=1;l<=numexps;l++){
   printf("Enter approximate maximum of exponential %d: ",l);
   scanf("%f",&max[l]);
   printf("Enter approximate tau of exponential %d: ",l);
   scanf("%f",&tau[l]);
  }
                                                                                      // decfit.c             
  //  fit info
  sprintf(tl,"  Max guesses: ");
  for(l=1;l<=numexps;l++){sprintf(tlb,"%d: %d,  ",l,max[l]);strcat(tl,tlb);}
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"  Tau guesses: ");
  for(l=1;l<=numexps;l++){sprintf(tlb,"%d: %.2f,  ",l,tau[l]);strcat(tl,tlb);}
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  
  printf("Fix constant? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   conlim=1;
   printf("Enter constant: ");
  }
  else{
   conlim=0;
   printf("Enter approximate constant: ");
  }
  scanf("%f",&exconst);

  //  fit info
  sprintf(tl,"Constant guess: %.1f",exconst);
  if(conlim==1)strcat(tl," fixed");
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  construct string for function  //
  sprintf(funcexp,"[0]");
  for(l=1;l<=numexps;l++){
   sprintf(funcbit,"+[%d]*exp(-x/[%d])",2*l-1,2*l); 
   strcat(funcexp,funcbit);   
  }
                                                                                      // decfit.c             
  //  define fit function and set initial parameters and limits  //
  TF1 *em = new TF1("em",funcexp,poslo,poshi);
  //  parameter names  //
  em.SetParName(0,"const");
  for(l=1;l<=numexps;l++){
   sprintf(pname,"max%d",l);
   em.SetParName(2*l-1,pname);
   em.SetParameter(2*l-1,max[l]);
   sprintf(pname,"tau%d",l);
   em.SetParName(2*l,pname);
   em.SetParameter(2*l,tau[l]);
  }
  if(conlim==1)em.SetParLimits(0,1,1);
  em.SetParameter(0,exconst);
  
  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("em","RB+");
  else histp[hindex].hh.Fit("em","RB");
 
  //  fit info
  strcpy(tl,"Fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  //  maxima  *************
  sprintf(tl,"            Max ");
  for(l=1;l<=3;l++){
   if(l<=numexps){
    sprintf(tlb,"#%d=%f+/-%f, ",l,em.GetParameter(2*l-1),em.GetParError(2*l-1));
    strcat(tl,tlb);
   }
  }
  strcat(tl,"            ");
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  if(numexps>3){
   strcpy(tl,"");
   for(l=4;l<=6;l++){
    if(l<=numexps){
     sprintf(tlb,"#%d=%f+/-%f, ",l,em.GetParameter(2*l-1),em.GetParError(2*l-1));
     strcat(tl,tlb);
    } 
   }
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }  
  if(numexps>6){
   strcpy(tl,"");
   for(l=7;l<=numexps;l++){
    sprintf(tlb,"#%d=%f+/-%f, ",l,em.GetParameter(2*l-1),em.GetParError(2*l-1));
    strcat(tl,tlb);
   }
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }  
  //  taus  **************
  sprintf(tl,"            Tau ");
  for(l=1;l<=3;l++){
   if(l<=numexps){
    sprintf(tlb,"#%d=%f+/-%f, ",l,em.GetParameter(2*l),em.GetParError(2*l));
    strcat(tl,tlb);
   }
  }
  strcat(tl,"            ");
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  if(numexps>3){
   strcpy(tl,"");
   for(l=4;l<=6;l++){
    if(l<=numexps){
     sprintf(tlb,"#%d=%f+/-%f, ",l,em.GetParameter(2*l),em.GetParError(2*l));
     strcat(tl,tlb);
    }
   }
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }  
  if(numexps>6){
   strcpy(tl,"");
   for(l=7;l<=numexps;l++){
    sprintf(tlb,"#%d=%f+/-%f, ",l,em.GetParameter(2*l),em.GetParError(2*l));
    strcat(tl,tlb);
   }
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }  
                                                                                      // decfit.c             
  //  constant  **************
  sprintf(tl,"constant = %f +/- %f",em.GetParameter(0),em.GetParError(0));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",em.GetChisquare(),em.GetNDF(),em.GetChisquare()/em.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

 break;


 case 3:          // *********** HORIZONTAL LINE ***************** //
  //  fit info
  strcpy(tl,"Horizontal line fit"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);
                                                                                      // decfit.c             
  //  set x range //
  printf("  p: Use previous position limits (%.2f,%.2f)\n",poslo,poshi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("  f: Use full range\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower position limit: ");
   scanf("%f",&poslo);
   printf("Enter upper position limit: ");
   scanf("%f",&poshi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   poslo=histp[hindex].hhl[0].GetX1();
   poshi=histp[hindex].hhl[1].GetX1();
  }
  if(yesorno[0]=='f' || yesorno[0]=='F'){
   poslo=histp[hindex].hh.GetBinLowEdge(1);
   poshi=histp[hindex].hh.GetBinLowEdge(histp[hindex].hh.GetNbinsX())+histp[hindex].hh.GetBinWidth(1);
  }

  //  fit info
  sprintf(tl,"Position limits: %.3f - %.3f",poslo,poshi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  define fit function  //
  TF1 *hl = new TF1("hl","[0]",poslo,poshi);
  hl.SetParName(0,"value");

  //  set guess parameters  //
  avval=10; 
  hl.SetParameter(0,avval);

  //  fit info
  sprintf(tl,"Fit guess: value=%.1f",avval);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("hl","RB+");
  else histp[hindex].hh.Fit("hl","RB");
                                                                                      // decfit.c             
  //  fit info
  strcpy(tl,"fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"value = %f +/- %f",hl.GetParameter(0),hl.GetParError(0));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",hl.GetChisquare(),hl.GetNDF(),hl.GetChisquare()/hl.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

 break;

 case 4:          // *********** SINGLE EXPONENTIAL WITH RISETIME ***************** //
  //  fit info
  strcpy(tl,"Single exponential plus constant with risetime fit"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);
                                                                                      // decfit.c             
  //  set x range //
  printf("  p: Use previous position limits (%.2f,%.2f)\n",poslo,poshi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower position limit: ");
   scanf("%f",&poslo);
   printf("Enter upper position limit: ");
   scanf("%f",&poshi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   poslo=histp[hindex].hhl[0].GetX1();
   poshi=histp[hindex].hhl[1].GetX1();
  }

  //  fit info
  sprintf(tl,"Position limits: %.3f - %.3f",poslo,poshi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  define fit function  //
  TF1 *er = new TF1("er","(x-[3]+abs(x-[3]))/(2*(x-[3]))*([0]*[1]*(1-exp(-[4]/[1]))*exp(-x/[1])+[2]*[4])+(x-[3]-abs(x-[3]))/(2*(x-[3]))*(x-[3]+[4]+abs(x-[3]+[4]))/(2*(x-[3]+[4]))*(-[0]*[1]*exp(-[4]/[1])*exp(-x/[1])+[2]*x+[0]*exp(-[3]/[1])*[1]+[2]*([4]-[3]))",poslo,poshi);
  er.SetParName(0,"max");
  er.SetParName(1,"tau");
  er.SetParName(2,"const");
  er.SetParName(3,"xmax");
  er.SetParName(4,"riselength");


  //  set guess parameters  //
  float xmax=0;
  printf("Enter approximate x-posn of maximum: ");
  scanf("%f",&xmax);
  er.SetParameter(3,xmax);

  printf("Fix riselength? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   rislim=1;
   printf("Enter riselength: ");
  }
  else{
   rislim=0;
   printf("Enter approximate riselength: ");
  }
  scanf("%f",&risex);
  if(rislim==1)er.SetParLimits(4,1,1);
  er.SetParameter(4,risex);

  printf("Enter approximate x=infinity constant: ");
  scanf("%f",&yconst);
  er.SetParLimits(2,0,1000000);
  er.SetParameter(2,yconst/risex);

  printf("Fix tau? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   taulim=1;
   printf("Enter tau: ");
  }
  else{
   taulim=0;
   printf("Enter approximate tau (where ht above const is reduced to 37%): ");
  }
  scanf("%f",&tau[0]);
  if(taulim==1)er.SetParLimits(1,1,1);
  er.SetParameter(1,tau[0]);
  float ymax=0;
  printf("Enter approximate height above constant: ");
  scanf("%f",&ymax);
  er.SetParameter(0,ymax*exp(xmax/tau[0])/(tau[0]*(1-exp(-risex/tau[0]))));

  //  fit info
  sprintf(tl,"Fit guesses: height=%.1f, tau=%.2f, constant=%.1f",ymax,tau[0],yconst);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"    x-zero=%.2f, rise=%.2f",xmax,risex);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  if(rislim==1){
   sprintf(tl,"Riselength fixed");
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  if(taulim==1){
   sprintf(tl,"Tau fixed");
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }

  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("er","RB+");
  else histp[hindex].hh.Fit("er","RB");
                                                                                      // decfit.c             
  //  fit info
  strcpy(tl,"fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"max = %f +/- %f",er.GetParameter(0),er.GetParError(0));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"tau = %f +/- %f",er.GetParameter(1),er.GetParError(1));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"constant = %f +/- %f",er.GetParameter(2),er.GetParError(2));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"x-zero = %f +/- %f",er.GetParameter(3),er.GetParError(3));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"rise length = %f +/- %f",er.GetParameter(4),er.GetParError(4));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",er.GetChisquare(),er.GetNDF(),er.GetChisquare()/er.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"Apparent height=%.1f; const =%.1f",er.GetParameter(0)*er.GetParameter(1)*(1-exp(-er.GetParameter(4)/er.GetParameter(1)))*exp(-er.GetParameter(3)/er.GetParameter(1)),er.GetParameter(2)*er.GetParameter(4));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);


 break;

 case 5:          // *********** SINGLE EXPONENTIAL WITH RISETIME (V2)***************** //
  //  fit info
  strcpy(tl,"Single exponential plus constant with risetime fit"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  t1.SetTextColor(fitclr);
                                                                                      // decfit.c             
  //  set x range //
  printf("  p: Use previous position limits (%.2f,%.2f)\n",poslo,poshi);
  printf("  l: Use line limits\n");
  printf("  e: Enter limits numerically\n");
  printf("Enter option: ");
  scanf("%s",yesorno);
  if(yesorno[0]=='e' || yesorno[0]=='E'){
   printf("Enter lower position limit: ");
   scanf("%f",&poslo);
   printf("Enter upper position limit: ");
   scanf("%f",&poshi);    
  }
  if(yesorno[0]=='l' || yesorno[0]=='L'){
   poslo=histp[hindex].hhl[0].GetX1();
   poshi=histp[hindex].hhl[1].GetX1();
  }

  //  fit info
  sprintf(tl,"Position limits: %.3f - %.3f",poslo,poshi);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

  //  define fit function  //
  TF1 *er = new TF1("er","(x>=[3])*([0]*exp(([3]-x)/[1])+[2])+(x<[3])*(x>([3]-[4]))*([0]*(1-exp(([3]-[4]-x)/[1]))/(1-exp(-[4]/[1]))-[2]/[4]*([3]-[4]-x))",poslo,poshi);
  er.SetParName(0,"height");
  er.SetParName(1,"tau");
  er.SetParName(2,"const");
  er.SetParName(3,"xzero");
  er.SetParName(4,"riselength");


  //  set guess parameters  //
  printf("Enter approximate x-posn of maximum: ");
  scanf("%f",&xmax);
  er.SetParameter(3,xmax);

  printf("Fix riselength? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   rislim=1;
   printf("Enter riselength: ");
  }
  else{
   rislim=0;
   printf("Enter approximate riselength: ");
  }
  scanf("%f",&risex);
  if(rislim==1)er.SetParLimits(4,1,1);
  er.SetParameter(4,risex);

  printf("Enter approximate x=infinity constant: ");
  scanf("%f",&yconst);
  er.SetParLimits(2,0,1000000);
  er.SetParameter(2,yconst);

  printf("Enter approximate height above constant: ");
  scanf("%f",&ymax);
  er.SetParameter(0,ymax);

  printf("Fix tau? (y/n): ");
  scanf("%s",yesorno);
  if(yesorno[0]=='y'){
   taulim=1;
   printf("Enter tau: ");
  }
  else{
   taulim=0;
   printf("Enter approximate tau (where ht above const is reduced to 37%): ");
  }
  scanf("%f",&tau[0]);
  if(taulim==1)er.SetParLimits(1,1,1);
  er.SetParameter(1,tau[0]);

  //  fit info
  sprintf(tl,"Fit guesses: height=%.1f, tau=%.2f, constant=%.1f",ymax,tau[0],yconst);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"    x-zero=%.2f, rise=%.2f",xmax,risex);
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  if(rislim==1){
   sprintf(tl,"Riselength fixed");
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }
  if(taulim==1){
   sprintf(tl,"Tau fixed");
   TText *t1=histp[hindex].hhf[findex].AddText(tl); 
   t1.SetTextFont(100);
  }

  //  fit histogram  //
  if (saveprev==1) histp[hindex].hh.Fit("er","RB+");
  else histp[hindex].hh.Fit("er","RB");
                                                                                      // decfit.c             
  //  fit info
  strcpy(tl,"fit values:"); 
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"height = %f +/- %f",er.GetParameter(0),er.GetParError(0));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"tau = %f +/- %f",er.GetParameter(1),er.GetParError(1));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"constant = %f +/- %f",er.GetParameter(2),er.GetParError(2));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"x-zero = %f +/- %f",er.GetParameter(3),er.GetParError(3));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"rise length = %f +/- %f",er.GetParameter(4),er.GetParError(4));
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);
  sprintf(tl,"chisquare/ndf = %f / %d = %f",er.GetChisquare(),er.GetNDF(),er.GetChisquare()/er.GetNDF());
  TText *t1=histp[hindex].hhf[findex].AddText(tl); 
  t1.SetTextFont(100);

 break;

}  

//  display fit info
TCanvas *cex = (TCanvas*)gROOT->FindObject("cfit");
if (!cex) {
 TCanvas cfit("cfit","fit info",480,280,703,503);
 cfit.Range(0,0,100,100);
}
else cfit.cd();
cfit.Clear();
for(l=1;l<=histp[hindex].hhnf;l++)histp[hindex].hhf[l].Draw();
  
//  redraw histogram in appropriate canvas  //
histp[hindex].hhc.cd();
histp[hindex].hh.Draw("e");
histp[hindex].hhl[0].Draw();
histp[hindex].hhl[1].Draw();

// end of decfit.c                                                                end of decfit.c
 break;//=======================================================================================

 case 1://=======================================================================================
//   psd2head.c                                                                        psd2head.c
//   routine to get PSD data from an ascii file and
//   to create a new ascii file with just headers
//   name new file e.g. artest004head.dat
//   location of PSD data and header file determined by 
//   "psdpref" and "namepref", respectively, defined elsewhere
 
//   interrogate user for data file name

   char namebit[50], nameext[5], namepsd[50], nameout[50], yesorno[10];   
   
   printf("Enter desired PSD raw file name (without extension): ");
   scanf("%s",namebit);
   printf("Enter extension (e.g. \"dc\",\"sp\",\"pl\"): ");
   scanf("%s",nameext);
   sprintf(namepsd,"%s%s.%s",psdpref,namebit,nameext);
   sprintf(nameout,"%s%shead.dat",namepref,namebit);
   printf("\ninput file: %s\noutput file: %s\n\n",namepsd,nameout);
   
   FILE *ffin = fopen(namepsd,"r");
   if (ffin <= 0) {
     printf("Sorry, cannot open file\n");
     break;
   }

   Float_t lam,pos,sig,om,fc,time,v1,v2;
   Int_t ncols, nlines,ndat;
   
   FILE *fout = fopen(nameout,"w");
   fprintf(fout,"lam        pos      sig     om       fc      time       \n");
   
   while (ffin) {
      ncols = fscanf(ffin,"%f %f %f %f %f %f",&lam, &pos, &sig, &om, &fc, &time);
      if (ncols < 0) break;
      printf("lam=%8.3f, pos=%8.3f, sig=%8.0f\n",lam,pos,sig);
      if (pos>100) {
         printf("\nThese look wrong. It's possible that the previous \"sig\" value\n");
         printf("is wrong. You can use \"psdcheck.c\" to find out.");
         printf("Do you wish to abort(y/n)? ");
         scanf("%s",yesorno);
         if (yesorno[0]=='y') break;
      } 
      fprintf(fout,"%-11.3f%-9.3f%-8d%-9d%-8d%-10d\n",lam,pos,sig,om,fc,time);
      nlines=(int)sig;
	for (int i=0; i<nlines;i++) ndat = fscanf(ffin,"%f %f",&v1,&v2);
		
   }

  fclose(ffin);
  fclose(fout);
// end of psd2head.c                                                            end of psd2head.c
 break;//=======================================================================================

 case 11://=======================================================================================
//  pmt2hist.c                                                                         pmt2hist.c
//  
//   routine to get PMT data from an ascii file and
//   to create a histogram (spectrum or decay curve)
//   location of PMT data determined by 
//   "psdpref" defined in initialization

   char namebit[50], namepmt[50], yesorno[10], nameext[5];   
   
//   determine whether spectrum or decay curve
   printf("Enter \"s\" for a spectrum, \"d\" for a decay curve: ");
   scanf("%s",yesorno);
   if(yesorno[0]=='s')strcpy(nameext,"sp");
   else strcpy(nameext,"dc");

   if(yesorno[0]=='d'){
   printf("Is this a complex dataset? (y/n): ");
   scanf("%s",yesorno);
   }
//   determine name for raw data files
   printf("Enter desired PMT dataset name (w/out extension): ");
   scanf("%s",namebit);
   sprintf(namepmt,"%s%s.%s",psdpref,namebit,nameext);
   if(yesorno[0]=='y')sprintf(namepmt,"%s%s.%s",psdpref,namebit,"pl");
//   open raw data file in read mode
   FILE *fp = fopen(namepmt,"r");
   if (fp <= 0) {
     printf("Sorry, cannot open file\n");
     break;
   }
   Float_t lam,pos,sig,om,fc,time;
   //Float_t lam,pos,sig,om,fc,time,err;
   Int_t ncols, nlines,ndat;
                                                                                    // pmt2hist.c             
//   create ntuple
     TNtuple *pmtntuple = new TNtuple("pmtntuple","data from ascii file","lam:pos:sig:om:fc:time");
   //TNtuple *pmtntuple = new TNtuple("pmtntuple","data from ascii file","lam:pos:sig:om:fc:time:err");
 
//   read raw data and fill ntuple 
   nlines=0;
   while (fp) {
      //   get a line
      //ncols = fscanf(fp,"%f %f %f %f %f %f %f",&lam, &pos, &sig, &om, &fc, &time, &err);
      ncols = fscanf(fp,"%f %f %f %f %f %f",&lam, &pos, &sig, &om, &fc, &time);
      if (ncols < 0) break;
      nlines++;
      if(nlines<5)printf("lam=%8.3f, pos=%8.3f, sig=%8.0f\n",lam,pos,sig);

      if (pos>100) {
         printf("\nThese look wrong. Do you wish to abort(y/n)? ");
         scanf("%s",yesorno);
         if (yesorno[0]=='y') break;
      } 

      //   collect events and fill ntuple
      //pmtntuple->Fill(lam,pos,sig,om,fc,time,err);
      pmtntuple->Fill(lam,pos,sig,om,fc,time);
   }

   //   close file
   fclose(fp);

   char namebit[50], nameroot[50], hmtitle[60], yesorno[10], nm[30]; 
   char hpname[30],cpname[30],hplabel[50],fid[5];
     
//Declaration of leaves types
   Float_t         lam,lam1;
   Float_t         pos,pos0,pos1;
   Float_t         sig;
   Float_t         om;
   Float_t         fc;
   Float_t         time;
//   Float_t         err;
//Set branch addresses
   pmtntuple->SetBranchAddress("lam",&lam);
   pmtntuple->SetBranchAddress("pos",&pos);
   pmtntuple->SetBranchAddress("sig",&sig);
   pmtntuple->SetBranchAddress("om",&om);
   pmtntuple->SetBranchAddress("fc",&fc);
   pmtntuple->SetBranchAddress("time",&time);
   //pmtntuple->SetBranchAddress("err",&err);
                                                                                    // pmt2hist.c             
// set histogram parameters
   Axis_t startx,stopx,stepx;
   Int_t nbinsx;

   if(nameext[0]=='s'){

//   reset wavelength offset if necessary
     printf("Wavelength offset (monochromator wavelength - known wavelength): %.3f\n",mono_offset);
     printf("Would you like to change this? (y/n): ");
     scanf("%s",yesorno);
     if(yesorno[0]=='y'){
      printf("Enter new offset: ");
      scanf("%f",&mono_offset);
     }

     printf("Wavelength offset (monochromator wavelength - known wavelength): %.3f\n",mono_offset);

//   find min and max wavelength, rounded to angstrom
//   (min rounded down, max rounded up)
     printf("getting minima and maxima...\n");
     pmtntuple.GetEvent(1);
     lam1=lam-mono_offset;
     pmtntuple.GetEvent(0);
     stepx=lam1-(lam-mono_offset);
     startx=(lam-mono_offset)-stepx/2.0;
     stopx = pmtntuple.GetMaximum("lam")-mono_offset+stepx/2.0;
   
     printf("wavelength min:%.2f max:%.2f bin size:%.2f\n",startx,stopx,stepx);

     printf("Would you like to change histogram wavelength parameters? (y/n): ");
     scanf("%s",yesorno);
     if(yesorno[0]=='y'){
       printf("Enter new minimum wavelength: ");
       scanf("%f",&startx);
       printf("Enter new maximum wavelength: ");
       scanf("%f",&stopx);
       printf("Enter wavelength resolution: ");
       scanf("%f",&stepx);
     }

     printf("position: %.2f\n",pos);
   }
   
   else{
//   find min and max position, rounded to angstrom
//   (min rounded down, max rounded up)
     printf("getting minima and maxima...\n");
     pmtntuple.GetEvent(1);
     pos1=pos;
     pmtntuple.GetEvent(0);
     stepx=pos1-pos;
     startx=pos-stepx/2.0;
     stopx = pmtntuple.GetMaximum("pos")+stepx/2.0;
   
     printf("position min:%.2f max:%.2f bin size:%.2f\n",startx,stopx,stepx);

     printf("Would you like to change histogram position parameters? (y/n): ");
     scanf("%s",yesorno);
     if(yesorno[0]=='y'){
       printf("Enter new minimum position: ");
       scanf("%f",&startx);
       printf("Enter new maximum position: ");
       scanf("%f",&stopx);
       printf("Enter position resolution: ");
       scanf("%f",&stepx);
     }

     printf("wavelength: %.2f\n",lam-mono_offset);
   }
   
   nbinsx=(int)((stopx-startx)/stepx+.5);
   stopx=startx+int((stopx-startx)/stepx+.5)*stepx;   
// last line insures that bin sizes are preserved by insuring max differs from min by 
// an integral number of steps

// abort option
   printf("Enter \"a\" to abort, anything else to continue: ");
   scanf("%s",yesorno);
   if(yesorno[0]=='a')break;
  
//   name and title histogram and name canvas
   if(nameext[0]=='s'){
     sprintf(hpname,"%sp%d",namebit,int(pos*10+.5));
     sprintf(cpname,"%sc",hpname);
     sprintf(hplabel,"%s, Spectrum @ position = %.1f",namebit,pos);
   }

   else{
     sprintf(hpname,"%sw%d",namebit,lam-mono_offset);
     sprintf(cpname,"%sc",hpname);
     sprintf(hplabel,"%s, Decay @ wavelength = %.1f",namebit,lam-mono_offset);
   }
                                                                                    // pmt2hist.c
// define histogram for spectrum
   printf("creating 1D histogram \"%s\"...\n",hpname);

   hx++;
   
   histp[hx].hh = new TH1D(hpname,hplabel,nbinsx,startx,stopx);
// set name in structure and initialize number of fits
   strcpy(histp[hx].hhn,hpname); 
   histp[hx].hhnf=0;
	
   Int_t nentries = pmtntuple->GetEntries();

   Int_t nbytes = 0;
   for (Int_t i=0; i<nentries;i++) {
      nbytes += pmtntuple->GetEvent(i);
//      if(i==0)float om0=om;
      if(nameext[0]=='s')
//        if(om>0)
//         histp[hx].hh.SetBinContent(histp[hx].hh.GetBin(int((lam-mono_offset-startx)/stepx)+1),om0/om*sig);
//        else
         histp[hx].hh.SetBinContent(histp[hx].hh.GetBin(int((lam-mono_offset-startx)/stepx)+1),sig);
      else
//        if(om>0)
//         histp[hx].hh.SetBinContent(histp[hx].hh.GetBin(int((pos-startx)/stepx)+1),om0/om*sig);
//        else
         histp[hx].hh.SetBinContent(histp[hx].hh.GetBin(int((pos-startx)/stepx)+1),sig);
         //histp[hx].hh.SetBinError(histp[hx].hh.GetError((int(pos-startx)/stepx)+1),(float err));       //7-24-06
   }
   printf("\n");
   printf("finished\007\n");

// draw spectrum and set info

// values for canvas position defined at initialization and incremented for cascade effect
// canvas title is hist name, canvas name is hist name plus "c"
   cx+=20;cy+=30;if(cy>280){cx=360;cy=10;}
   histp[hx].hhc=new TCanvas(cpname,hpname,cx,cy,703,503);
   gPad->SetTicks(1,1);
   gPad->SetGridx();
   gPad->SetGridy();
   histp[hx].hh.Draw("e");

// create and draw limit lines
   histp[hx].hhl[0]=new TLine(histp[hx].hh.GetBinLowEdge(1),-500,histp[hx].hh.GetBinLowEdge(1),1.5*histp[hx].hh.GetMaximum());
   histp[hx].hhl[0].SetLineColor(46);
   histp[hx].hhl[0].Draw();
   histp[hx].hhl[1]=new TLine(histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),-500,histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),1.5*histp[hx].hh.GetMaximum());
   histp[hx].hhl[1].SetLineColor(38);
   histp[hx].hhl[1].Draw();

// end of pmt2hist.c                                                            end of pmt2hist.c
 break;//=======================================================================================

 case 4://=======================================================================================
//  raw2info.c
//  
//   macro to read data from header file (created in psd2ntup.c) or raw PMT file and
//   provide information on the run.
 
 Float_t lambdam[500], position[500], signals[500], optical[500], faraday[500], seconds[500];
  
//   interrogate user for data file name

 char namebit[50], namehead[50], st[10], yesorno[10], nameext[5], ispsd[5];   
 //printf("Is this a PSD dataset? (y/n): ");
 //scanf("%s",ispsd);
 //if(ispsd[0]=='y'){
 // printf("Enter desired PSD dataset name (w/out extension): ");
 // scanf("%s",namebit);
 // sprintf(namehead,"%s%shead.dat",namepref,namebit);
 // printf("\ninput file: %s\n\n",namehead);
 // FILE *fp = fopen(namehead,"r");
 // if (fp <= 0) printf("Sorry, cannot open file\n");
 //}
 //else{
  printf("Enter desired PMT dataset name (w/out extension): ");
  scanf("%s",namebit);
  strcpy(nameext,"sp");
  sprintf(namehead,"%s%s.%s",psdpref,namebit,nameext);
  FILE *fp = fopen(namehead,"r");
  if (fp <= 0){
   strcpy(nameext,"dc");
   sprintf(namehead,"%s%s.%s",psdpref,namebit,nameext);
   FILE *fp = fopen(namehead,"r");
  } 
  if (fp <= 0){
   strcpy(nameext,"pl");
   sprintf(namehead,"%s%s.%s",psdpref,namebit,nameext);
   FILE *fp = fopen(namehead,"r");
  } 
  if (fp <= 0) printf("Sorry, cannot open file\n");
  printf("\ninput file: %s\n\n",namehead);
 //}
 
 Float_t lam,pos,sig,om,fc,time;
 Int_t ncols;
 Int_t nlines = 0;
   
//   get rid of first line of header file 
 if(ispsd[0]=='y')fscanf(fp,"%s %s %s %s %s %s",st,st,st,st,st,st);
                                                                                     // monitor.c             
//   fill arrays with data
 Int_t i=0;
 while (fp) {
  ncols = fscanf(fp,"%f %f %f %f %f %f", &lambdam[i], &position[i], &signals[i], &optical[i], &faraday[i], &seconds[i]);
  if (ncols < 0) break;
  seconds[i]/=10000;
  i++;
  nlines++;
 }
 fclose(fp);

 printf(" found %d lines\n",nlines);

 float avgnow, minnow, maxnow, stdnow;
 printf("\n                   Minimum   Maximum   Step\n");

 minnow=1e20;maxnow=0;
 for(i=0;i<nlines;i++){
  if(lambdam[i]<minnow)minnow=lambdam[i];
  if(lambdam[i]>maxnow)maxnow=lambdam[i];
 }
 printf("Monochromator      %-10.2f%-10.2f%-10.2f\n",minnow,maxnow,lambdam[1]-lambdam[0]);

 minnow=1e20;maxnow=0;
 for(i=0;i<nlines;i++){
  if(position[i]<minnow)minnow=position[i];
  if(position[i]>maxnow)maxnow=position[i];
 } 
 printf("Foil Position      %-10.2f%-10.2f%-10.2f\n",minnow,maxnow,position[1]-position[0]);

 printf("\n                   Average   Minimum   Maximum   Std.Dev.\n");
 
 avgnow=0;minnow=1e20;maxnow=0;
 for(i=0;i<nlines;i++){
  avgnow+=seconds[i];
  if(seconds[i]<minnow)minnow=seconds[i];
  if(seconds[i]>maxnow)maxnow=seconds[i];
 }
 avgnow/=nlines;
 printf("Seconds per point  %-10.2f%-10.2f%-10.2f(Total:%.2f minutes)\n",avgnow,minnow,maxnow,avgnow*nlines/60);
  
 avgnow=0;minnow=1e20;maxnow=0;stdnow=0;
 for(i=0;i<nlines;i++){
  avgnow+=signals[i]/seconds[i];
  if(signals[i]/seconds[i]<minnow)minnow=signals[i]/seconds[i];
  if(signals[i]/seconds[i]>maxnow)maxnow=signals[i]/seconds[i];
 }
 avgnow/=nlines;
 for(i=0;i<nlines;i++){
  stdnow+=pow((signals[i]/seconds[i]-avgnow),2);
 }
 if(nlines>1)stdnow=sqrt(stdnow/(nlines-1));
 else stdnow=0;
 printf("Signals/sec        %-10.3f%-10.3f%-10.3f%-10.3f\n",avgnow,minnow,maxnow,stdnow);
  
 avgnow=0;minnow=1e20;maxnow=0;stdnow=0;
 for(i=0;i<nlines;i++){
  avgnow+=optical[i]/seconds[i];
  if(optical[i]/seconds[i]<minnow)minnow=optical[i]/seconds[i];
  if(optical[i]/seconds[i]>maxnow)maxnow=optical[i]/seconds[i];
 }
 avgnow/=nlines;
 for(i=0;i<nlines;i++){
  stdnow+=pow((optical[i]/seconds[i]-avgnow),2);
 }
 if(nlines>1)stdnow=sqrt(stdnow/(nlines-1));
 else stdnow=0;
 printf("OM/sec             %-10.3f%-10.3f%-10.3f%-10.3f\n",avgnow,minnow,maxnow,stdnow);
  
 avgnow=0;minnow=1e20;maxnow=0;stdnow=0;
 for(i=0;i<nlines;i++){
  avgnow+=faraday[i]/seconds[i];
  if(faraday[i]/seconds[i]<minnow)minnow=faraday[i]/seconds[i];
  if(faraday[i]/seconds[i]>maxnow)maxnow=faraday[i]/seconds[i];
 }
 avgnow/=nlines;
 for(i=0;i<nlines;i++){
  stdnow+=pow((faraday[i]/seconds[i]-avgnow),2);
 }
 if(nlines>1)stdnow=sqrt(stdnow/(nlines-1));
 else stdnow=0;
 printf("FC/sec             %-10.3f%-10.3f%-10.3f%-10.3f\n",avgnow,minnow,maxnow,stdnow);

 avgnow=0;minnow=1e20;maxnow=0;stdnow=0;
 for(i=0;i<nlines;i++){
  avgnow+=faraday[i];
  if(faraday[i]<minnow)minnow=faraday[i];
  if(faraday[i]>maxnow)maxnow=faraday[i];
 }
 avgnow/=nlines;
 for(i=0;i<nlines;i++){
  stdnow+=pow((faraday[i]-avgnow),2);
 }
 if(nlines>1)stdnow=sqrt(stdnow/(nlines-1));
 else stdnow=0;
 printf("FC/step            %-10.3f%-10.3f%-10.3f%-10.3f\n",avgnow,minnow,maxnow,stdnow);


 printf("\n");
//                                                                              end of raw2info.c
 break;//=======================================================================================

 case 17://=======================================================================================
//  histnorm.c                                                                         histnorm.c
//  normalization and correction options for a histogram

if(hx==0){
  printf("sorry, no spectra or decay curves exist\n");
  break;
}

char yesorno[10],dname[30],nm[30],hname[30],hnormname[30],nameext[5],namebit[50],namehead[50],
 normopt[3],hplabel[50], ispsd[5];
int hindex,hfd=0,iscomplex=0;
Float_t lam,pos,sig,om,fc,time;
   
// find histogram
printf("Enter name of histogram to modify: ");
scanf("%s",hname);
for(int l=1;l<70;l++)if(strcmp(histp[l].hhn,hname)==0){hindex=l;hfd=1;}
if(hfd==0){printf("Sorry, can't find that histogram\n");break;}

hx++;
normx++;

//   open raw data file
printf("I'll locate the dataset containing norm. data...\n");
//printf("Is this a PSD dataset? (y/n): ");
// scanf("%s",ispsd);
if(ispsd[0]=='y'){
 printf("Enter desired PSD dataset name (w/out extension): ");
 scanf("%s",namebit);
 sprintf(namehead,"%s%shead.dat",namepref,namebit);
}
else{
 printf("Enter desired PMT dataset name (w/out extension): ");
 scanf("%s",namebit);
 strcpy(nameext,"sp");
 sprintf(namehead,"%s%s.%s",psdpref,namebit,nameext);
 FILE *fp = fopen(namehead,"r");
 if (fp <= 0){
  strcpy(nameext,"dc");
  sprintf(namehead,"%s%s.%s",psdpref,namebit,nameext);
  FILE *fp = fopen(namehead,"r");
 } 
 if (fp <= 0){
  strcpy(nameext,"pl");
  sprintf(namehead,"%s%s.%s",psdpref,namebit,nameext);
  FILE *fp = fopen(namehead,"r");
  iscomplex=1;
 } 
 if (fp <= 0) printf("Sorry, cannot open file\n");
}
printf("\ninput file: %s\n\n",namehead);
    
//   create copy of histo
hnorm[normx]=(*histp[hindex].hh);
sprintf(hnormname,"%sn%d",hname,normx);
hnorm[normx].SetName(hnormname);
histp[hx].hh = (TH1D*)gROOT.FindObject(hnormname);
// set name in structure and initialize number of fits
strcpy(histp[hx].hhn,hnormname); 
histp[hx].hhnf=0;
strcpy(hplabel,histp[hindex].hh.GetTitle());

//  manually set errors for new histo (if this is not done errors will be erased)
Int_t nx=histp[hindex].hh.GetNbinsX();
for (Int_t i=1; i<=nx;i++) {
 float benow=histp[hindex].hh.GetBinError(i);
 histp[hx].hh.SetBinError(i,benow);
}


float sigdc=0,sigdcerr=0,dwell=0;

//  SUBTRACT DARK COUNT FROM SIGNAL
FILE *fp = fopen(namehead,"r");
if (fp <= 0) printf("Sorry, cannot open file\n");

printf("Would you like to subtract a dark count from the signal? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y'){
 printf("Enter value of dark count in signals/second: ");
 scanf("%f",&sigdc);
 printf("Enter error in dark count in signals/second: ");
 scanf("%f",&sigdcerr);
 //printf("Enter dwell time: ");
// scanf("%f",&dwell);
 // get rid of first line of header file 
 if(ispsd[0]=='y')fscanf(fp,"%s %s %s %s %s %s",st,st,st,st,st,st);
 for (Int_t i=1; i<=nx;i++) {
  fscanf(fp,"%f %f %f %f %f %f", &lam, &pos, &sig, &om, &fc, &time);
  time/=10000;
  float bcnow=histp[hx].hh.GetBinContent(i);
  float benow=histp[hx].hh.GetBinError(i);
   histp[hx].hh.SetBinContent(i,bcnow-sigdc*time);

  //if(bcnow>sigdc)histp[hx].hh.SetBinContent(i,bcnow-sigdc*dwell);
  //else histp[hx].hh.SetBinContent(i,0);
  histp[hx].hh.SetBinError(i,sqrt(pow(benow,2)+pow(sigdcerr*dwell,2)));
 }
 strcat(hplabel,", -dc");
 fclose(fp);
}


int frisset=0;

//  SUBTRACT BEAM-DEPENDENT BACKGROUND FROM SIGNAL
FILE *fp = fopen(namehead,"r");
if (fp <= 0) printf("Sorry, cannot open file\n");

float beambkg,beambkgerr,fcbkg,fcbkgerr,foilratio,foilratioerr,bdbkg,bdbkgerr;
printf("\nWould you like to subtract a beam-dependent background \nfrom the signal? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y'){
 printf("Enter beam-on signals in signals/second: ");
 scanf("%f",&beambkg);
 printf("Enter error in beam-on signals: ");
 scanf("%f",&beambkgerr);
 printf("Enter corresponding faraday cup signals in signals/second: ");
 scanf("%f",&fcbkg);
 printf("Enter error in corresponding faraday cup signals: ");
 scanf("%f",&fcbkgerr);
 printf("Enter foil-out/foil-in faraday cup ratio: ");
 scanf("%f",&foilratio);
 printf("Enter error in foil-out/foil-in faraday cup ratio: ");
 scanf("%f",&foilratioerr);
 frisset=1;
 // get rid of first line of header file 
 if(ispsd[0]=='y')fscanf(fp,"%s %s %s %s %s %s",st,st,st,st,st,st);
 for (Int_t i=1; i<=nx;i++) {
  fscanf(fp,"%f %f %f %f %f %f", &lam, &pos, &sig, &om, &fc, &time);
  time/=10000;
  bdbkg=((beambkg-sigdc)/fcbkg)*fc*foilratio;
 // bdbkg=((beambkg-sigdc)/fcbkg)*fc*foilratio*time;
 // if(bdbkg<0)bdbkg=0;
  if(bdbkg=0)bdbkgerr=0;
  else bdbkgerr=bdbkg*sqrt(pow(1/fc,2)+pow(foilratioerr/foilratio,2)+pow(fcbkgerr/fcbkg,2)
   +(pow(beambkgerr,2)+pow(sigdcerr,2))/pow(beambkg-sigdc,2));
  float bcnow=histp[hx].hh.GetBinContent(i);
  float benow=histp[hx].hh.GetBinError(i);
  
//  if(bcnow>bdbkg)histp[hx].hh.SetBinContent(i,bcnow-bdbkg);
//  else histp[hx].hh.SetBinContent(i,0);
  histp[hx].hh.SetBinContent(i,bcnow-bdbkg);
  
  histp[hx].hh.SetBinError(i,sqrt(pow(benow,2)+pow(bdbkgerr,2)));
 }
 strcat(hplabel,", -bdb");
 fclose(fp);
}


float omdc=0,omdcerr=0;

//  SUBTRACT DARK COUNT FROM OPTICAL MONITOR
printf("\nWould you like to subtract a dark count from the optical monitor? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y'){
 printf("Enter value of dark count in signals/second: ");
 scanf("%f",&omdc);
 printf("Enter error in dark count in signals/second: ");
 scanf("%f",&omdcerr);
}


float ombeambkg=0,ombeambkgerr=0,omfcbkg=1,omfcbkgerr=0;

//  SUBTRACT BEAM-DEPENDENT BACKGROUND FROM OPTICAL MONITOR
printf("\nWould you like to subtract a beam-dependent background \nfrom the optical monitor? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y'){
 printf("Enter beam-on OM signals in signals/second: ");
 scanf("%f",&ombeambkg);
 printf("Enter error in beam-on OM signals: ");
 scanf("%f",&ombeambkgerr);
 printf("Enter corresponding faraday cup signals in signals/second: ");
 scanf("%f",&omfcbkg);
 printf("Enter error in corresponding faraday cup signals: ");
 scanf("%f",&omfcbkgerr);
 if(frisset==0){
  printf("Enter foil-out/foil-in faraday cup ratio: ");
  scanf("%f",&foilratio);
  printf("Enter error in foil-out/foil-in faraday cup ratio: ");
  scanf("%f",&foilratioerr);
 }
}


//  NORMALIZE
FILE *fp = fopen(namehead,"r");
if (fp <= 0) printf("Sorry, cannot open file\n");

printf("\nWould you like to normalize the signal? (y/n): ");
scanf("%s",yesorno);
if(yesorno[0]=='y'){
 //   choose normalization channel
 printf("  o: Normalize to OPTICAL MONITOR\n");
 printf("  f: Normalize to FARADAY CUP\n");
 printf("Enter option: ");
 scanf("%s",normopt);

 // get normalization data and normalize
 // get rid of first line of header file 
 //if(ispsd[0]=='y')fscanf(fp,"%s %s %s %s %s %s",st,st,st,st,st,st);

 if(normopt[0]=='o' || normopt[0]=='O'){
  for (Int_t i=1; i<=nx;i++) {
   fscanf(fp,"%f %f %f %f %f %f", &lam, &pos, &sig, &om, &fc, &time);
   time/=10000;
   bdbkg=((ombeambkg-omdc)/omfcbkg)*fc*foilratio;
 //  if(bdbkg<0)bdbkg=0;
   if(ombeambkg==0)bdbkgerr=0;
   else bdbkgerr=bdbkg*sqrt(pow(1/fc,2)+pow(foilratioerr/foilratio,2)+pow(omfcbkgerr/omfcbkg,2)
    +(pow(ombeambkgerr,2)+pow(omdcerr,2))/pow(ombeambkg-omdc,2));
   if(i==1) float om0=om-omdc*time-bdbkg;
   if(om<=0){printf("Can't normalize; normalization channel is zero or negative.\n");break;}
   float bcnow=histp[hx].hh.GetBinContent(i);
   float benow=histp[hx].hh.GetBinError(i);
   histp[hx].hh.SetBinContent(i,om0/(om-omdc*time-bdbkg)*bcnow);
   
   float bcnorm = om0/(om-omdc*time-bdbkg)*bcnow;
   
   histp[hx].hh.SetBinError(i,om0/(om-omdc*time-bdbkg)*sqrt(pow(bcnow/(om-omdc*time-bdbkg),2)
    *(om+pow(omdcerr*time,2)+pow(bdbkgerr,2))+pow(benow,2)));
    
    float benorm = om0/(om-omdc*time-bdbkg)*sqrt(pow(bcnow/(om-omdc*time-bdbkg),2)
    *(om+pow(omdcerr*time,2)+pow(bdbkgerr,2))+pow(benow,2));
  //////////////////////////////////////////////////
   FILE *ofp;
   char ofilename[]="C:/Rob Learning Stuff/Root Outputs/normdat.txt";
   char normdat[80];

   ofp = fopen(ofilename,"a");
   //printf("%d\t%f\t%f\n",i,bcnow,benow);
   //fprintf(ofp,"%d\t%f\t%f\n",i,bcnow,benow);
   fprintf(ofp,"%d\t%f\t%f\n",i,bcnorm,benorm); 
   fclose(ofp);
   ///////////////////////////////////////////////////////

  }
  printf("om normalization value: %f\n",om0);
 }
 else{
  for (Int_t i=1; i<=nx;i++) {
   fscanf(fp,"%f %f %f %f %f %f", &lam, &pos, &sig, &om, &fc, &time);
   time/=10000;
   if(i==1) float fc0=fc;
   if(fc0==0){printf("Can't normalize; normalization channel is zero.\n");break;}
   float bcnow=histp[hx].hh.GetBinContent(i);
   float benow=histp[hx].hh.GetBinError(i);
   histp[hx].hh.SetBinContent(i,fc0/fc*bcnow);
   histp[hx].hh.SetBinError(i,fc0/fc*sqrt(pow(bcnow/fc,2)+pow(benow,2)));
  }
  printf("fc normalization value: %f\n",fc0);

 }
 if(normopt[0]=='o' || normopt[0]=='O')
  strcat(hplabel,", OM norm");
 else
  strcat(hplabel,", FC norm");
 fclose(fp);
}   


histp[hx].hh.SetTitle(hplabel);

sprintf(cpname,"%sc",hnormname);

// values for canvas position defined at initialization and incremented for cascade effect
// canvas title is hist name, canvas name is hist name plus "c"
cx+=20;cy+=30;if(cy>280){cx=360;cy=10;}
histp[hx].hhc=new TCanvas(cpname,hnormname,cx,cy,703,503);
gPad->SetTicks(1,1);
   gPad->SetGridx();
   gPad->SetGridy();
histp[hx].hh.Draw("e");

// create and draw limit lines
histp[hx].hhl[0]=new TLine(histp[hx].hh.GetBinLowEdge(1),-500,histp[hx].hh.GetBinLowEdge(1),1.5*histp[hx].hh.GetMaximum());
histp[hx].hhl[0].SetLineColor(46);
histp[hx].hhl[0].Draw();
histp[hx].hhl[1]=new TLine(histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),-500,histp[hx].hh.GetBinLowEdge(histp[hx].hh.GetNbinsX()),1.5*histp[hx].hh.GetMaximum());
histp[hx].hhl[1].SetLineColor(38);
histp[hx].hhl[1].Draw();

fclose(fp);
//                                                                              end of histnorm.c
 break;//=======================================================================================

 case 20://=======================================================================================
 break;//=======================================================================================



}
}
