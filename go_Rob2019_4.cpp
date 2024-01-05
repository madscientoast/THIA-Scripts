{
// monster macro containing all useful functionality

//  Matt Fritts
// Updated by: Robert Snuggs (last updated: 06-28-2019)
//  MCF version
//  (for RMS version namepref should be "d:/root/user/" and psdpref should be "f:/")
//  Initializing routine for Root session 

//  initialization is done just once, while macro is used repeatedly

int initdone;
if(initdone!=1){
   gROOT->Reset();
   int initdone=1;
   printf("initializing\n");

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


#include "Robtest_1.h"
//#include "hist_structs.cpp"
#include "monitor.C"
#include "raw2info.C"
#include "openhist.C"
#include "histzoom.C"

// default raw data filtering and correcting criteria above

   char psdpref[30],namepref[50],namesuff[30];
   strcpy(psdpref,"c:/Rob Learning Stuff/2019-05-15/");
   strcpy(namepref,"c:/Rob Learning Stuff/Root Outputs/");
   strcpy(namesuff,".root");

 //  namepref determines where macro will look to find ntuple files
 //  and where spectra and header files will be saved
 //  psdpref determines where macro will look for raw data

   gStyle.SetOptStat(11);
   gStyle->SetOptFit(1111);

 //  above sets info appearing when spectra are drawn

   Int_t hx=0,cx=-10,cy=-20;  
//Initializes Structs used throughout all our scripts//
   struct HP{
    TH1D *hh;
    char hhn[50];
    TCanvas *hhc;
    TPaveText hhf[50];
    int hhnf;
    TLine hhl[2];
   }histp[90];

 //  The structured array of variables histp[n] packages a spectrum or decay curve with
 //  its name, canvas, fit information, number of saved fits, and limit lines (these are 
 //  2 vertical lines which can be moved with the mouse to set limits). The integer hx
 //  keeps track of how many 1d histos have been created and sets the index of histp[n]
 
    struct HP2D{
    TH2F *mh;
    char mhn[30];
   }mhp[30];


   int sumx=0;TH1D hsum[70];

   Int_t mhx=0;
 //  struct HP2D{
 //   TH2F *mh;
 //   char mhn[30];
 //  }mhp[30];

   int comx=0;TH2F hcom[30];
   TH2F *hmm[30];

   int normx=0;TH1D hnorm[70];
   
}


                                                                                       //Below is the MENU//
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
printf(" 11: PMT data -> SPECTRUM or DECAY CURVE(s)                      11:pmt2hist.c\n");
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

 case 12://=======================================================================================
// openhist.c                                                                          openhist.c   
// load saved spectrum or decay curve into memory and draw it
  openhist(hx,cx,cy,namepref,namesuff);
//  end of openhist.c                                                           end of openhist.c   
 break;//=======================================================================================

 case 13://=======================================================================================
//  histzoom.c                                                                         histzoom.c
//  zooms in on a region of a spectrum or decay curve
histzoom(hx);
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


// histsum.c                                                                            histsum.c
// add two histograms with weighting factors
// histograms must have identical binning


if(hx==0){
  printf("sorry, no spectra or decay curves exist\n");
  break;
}

hx++;
sumx++;

char hpname[50], cpname[30],hplabel[50],dname[30],nm[30];
float posn1,posn2,wf1,
float bc,be = 0;
int l,,hfd, numSum, strNum, i,j;
char label_arr[50][50];
int hindex[50];
char str_array[50][50];

printf("Enter number of sums: ");
scanf("%d",&numSum);

strNum=numSum-1; //for the for loop
//name all your graphs
for(i=0; i<numSum;i++){
        if(i == 0){
             printf("Enter name of first histogram: ");
             scanf("%49s", str_array[i]);
             }
             
        if(i > 0 && i < strNum){
             
             printf("Enter name of the next histogram: ");
             scanf("%49s", str_array[i]);
             }
             
        if(i==strNum){
                      printf("Enter name of last histogram: ");
                      scanf("%49s", str_array[i]);
                      }   
}

printf("Enter weighting factor: "); //set for uniform fit (will add loop later for custom weights)
scanf("%f",&wf1);

//graph name management
for(i=0;i<numSum;i++){
                      sprintf(nm,"%.3f*",wf1);
                      strcpy(label_arr[i],nm);
                      strcpy(label_arr[i],str_array[i]);
                      strcat(label_arr[i]," + ");
                      strcat(hplabel, label_arr[i]);
                      hfd=0;
                      for(int l=1;l<70;l++)if(strcmp(histp[l].hhn,str_array[i])==0){hindex[i]=l;hfd=1;}  
                      if(hfd==0){printf("Sorry, can't find that histogram\n");break;}                 
}

                      
hsum[sumx]=(*histp[hindex[0]].hh);

printf("Enter identifier for new histogram: ");
scanf("%s",nm);
sprintf(hpname,"sum%s",nm);
//-----------------------------
FILE *fout;
  char nameout[80],*no;
  sprintf(nameout,"C:/Rob Learning Stuff/Root Outputs/sum2.txt");
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
Int_t nx;
int half_sum = numSum / 2;
if(half_sum%2 != 0){half_sum += 0.5;}
nx = histp[hindex[0]].hh.GetNbinsX();
float bc_temp[50], bc_temp2[50], be_temp[50], be_temp2[50];
for(i=0;i<half_sum;i++){
                        
if(numSum%2 == 0){
for(l=1;l<nx;l++){
                   float bc,be,bc2,be2;                                    

 bc = wf1*(histp[hindex[i]].hh.GetBinContent(l)) + wf1*(histp[hindex[i+1]].hh.GetBinContent(l));
 be = sqrt(pow(wf1*histp[hindex[i]].hh.GetBinError(l),2)+pow(wf1*histp[hindex[i+1]].hh.GetBinError(l),2));
  histp[hx].hh.SetBinContent(l,bc);
 histp[hx].hh.SetBinError(l,be);
 fprintf(fout,"%d\t%f\t%f\n",l,bc,be);


}}

else{
     for(l=1;l<nx;l++){
                        float bc,be;                                    

 bc = wf1*(histp[hindex[i]].hh.GetBinContent(l)) + wf1*(histp[hindex[i+1]].hh.GetBinContent(l));
 be = sqrt(pow(wf1*histp[hindex[i]].hh.GetBinError(l),2)+pow(wf1*histp[hindex[i+1]].hh.GetBinError(l),2));
  histp[hx].hh.SetBinContent(l,bc);
 histp[hx].hh.SetBinError(l,be);
 fprintf(fout,"%d\t%f\t%f\n",l,bc,be);

 bc2 = bc + wf1*(histp[hindex[i+2]].hh.GetBinContent(l));
 be2 = sqrt(be+pow(wf1*histp[hindex[i+2]].hh.GetBinError(l),2)); 
   histp[hx].hh.SetBinContent(l,bc2);
 histp[hx].hh.SetBinError(l,be2);
 fprintf(fout,"%d\t%f\t%f\n",l,bc2,be2);
 }
}

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

 
 case 3://=======================================================================================
//   monitor.c                                                                                   monitor.c
//   macro to read data from header file (created in psd2ntup.c) or raw PMT file and
//   graph OM,FC and the ratio OM/FC vs foil position.
 
monitor(psdpref);

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

 

 case 11://=======================================================================================
//  pmt2hist.c                                                                         pmt2hist.c
//  
//   routine to get PMT data from an ascii file and
//   to create a histogram (spectrum or decay curve)
//   location of PMT data determined by 
//   "psdpref" defined in initialization
// pnt2hist();



//pmt2hist();
  int data_count, name_startpmt, name_endpmt;
     char namebit3[50], namepmt2[50], yesorno2[10], nameext[5], dir_base[]="C:/Rob Learning Stuff/";   
     char* namebit2[50];
   
//   determine whether spectrum or decay curve
   printf("Enter \"s\" for a spectrum, \"d\" for a decay curve: ");
   scanf("%s",yesorno2);
   if(yesorno2[0]=='s')strcpy(nameext,"sp");
   else strcpy(nameext,"dc");
   
   if(yesorno2[0]=='d'){
   printf("Is this a complex dataset? (y/n): ");
   scanf("%s",yesorno2);
   }
   
   //   determine name for raw data files
   printf("Enter desired PMT dataset name (directory ex: 2001-08-02): ");
   scanf("%s",namebit3);
   namebit2 = strcat(dir_base, namebit3);
   printf("Directory: %s", namebit2);
   printf("Enter desired starting PMT dataset: ");
   scanf("%d",&name_startpmt);
   printf("Enter ending PMT: ");
   scanf("%d", &name_endpmt);
   printf("Enter number of runs? ");
   scanf("%d", &data_count);
   
   //loop to make all histograms for dataset
   for(Int_t i=0; i < data_count; i++)
   {

            
   char namepmt[50];
   char fileName[50];
  sprintf(namepmt,"%d", name_startpmt);
      if(yesorno2[0]=='y')sprintf(fileName,"%s/%s.%s",namebit2,namepmt,"pl");
      printf("Dir: %s", fileName);
   //   open raw data file in read mode
   FILE *fp = fopen(fileName,"r");
   
   if (fp <= 0) {
     printf("Sorry, cannot open file\n");
     break;
   }
   
    Float_t lam,pos,sig,om,fc,time;
     Int_t ncols, nlines,ndat;   
      //   create ntuple
    //  hx++;
     TNtuple *pmtntuple;
     pmtntuple = new TNtuple("pmtntuple","data from ascii file","lam:pos:sig:om:fc:time");
     
     //   read raw data and fill ntuple 
   nlines=0;
   while (fp) {
      //   get a line
      ncols = fscanf(fp,"%f %f %f %f %f %f",&lam, &pos, &sig, &om, &fc, &time);
      if (ncols < 0) break;
      nlines++;
      if(nlines<5)printf("lam=%8.3f, pos=%8.3f, sig=%8.0f\n",lam,pos,sig);

      if (pos>100) {
         printf("\nThese look wrong. Do you wish to abort(y/n)? ");
         scanf("%s",yesorno2);
         if (yesorno2[0]=='y') break;
         }
            //   collect events and fill ntuple
        
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
//Set branch addresses
   pmtntuple->SetBranchAddress("lam",&lam);
   pmtntuple->SetBranchAddress("pos",&pos);
   pmtntuple->SetBranchAddress("sig",&sig);
   pmtntuple->SetBranchAddress("om",&om);
   pmtntuple->SetBranchAddress("fc",&fc);
   pmtntuple->SetBranchAddress("time",&time);
                                                                                    // pmt2hist.c             
// set histogram parameters
   Axis_t startx,stopx,stepx;
   Int_t nbinsx;


   
   
 //  else{
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
  // }
   
   nbinsx=(int)((stopx-startx)/stepx+.5);
   stopx=startx+int((stopx-startx)/stepx+.5)*stepx;   
// last line insures that bin sizes are preserved by insuring max differs from min by 
// an integral number of steps

// abort option
   printf("Enter \"a\" to abort, anything else to continue: ");
   scanf("%s",yesorno);
   if(yesorno[0]=='a')break;
  
//   name and title histogram and name canvas
//   if(nameext[0]=='s'){
//     sprintf(hpname,"%sp%d",namepmt,int(pos*10+.5));
//     sprintf(cpname,"%sc",hpname);
//     sprintf(hplabel,"%s, Spectrum @ position = %.1f",namepmt,pos);
//   }

//   else{
     sprintf(hpname,"%sw%d",namepmt,lam-mono_offset);
     sprintf(cpname,"%sc",hpname);
     sprintf(hplabel,"%s, Decay @ wavelength = %.1f",namebit,lam-mono_offset);
//   }
                                                                                    // pmt2hist.c
// define histogram for spectrum
   printf("creating 1D histogram \"%s\"...\n",hpname);

hx++;   
 //  hx=1;
   histp[hx].hh = new TH1D(hpname,hplabel,nbinsx,startx,stopx);
// set name in structure and initialize number of fits
   strcpy(histp[hx].hhn,hpname); 
   histp[hx].hhnf=0;
	
   Int_t nentries = pmtntuple->GetEntries();

   Int_t nbytes = 0;
   for (Int_t j=0; j<nentries;j++) {
      nbytes += pmtntuple->GetEvent(j);
//      if(i==0)float om0=om;
    //  if(nameext[0]=='s'){
      //   histp[i].hh.SetBinContent(histp[i].hh.GetBin(int((lam-mono_offset-startx)/stepx)+1),sig);}
    //  else{
         histp[hx].hh.SetBinContent(histp[hx].hh.GetBin(int((pos-startx)/stepx)+1),sig);//}
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
   
   //free variables 

 name_startpmt += 1;

 }
    namebit3[0] = 0;
   namebit2[0]  = 0;
   
   //free(namebit);
// end of pmt2hist.c                                                            end of pmt2hist.c
 break;//=======================================================================================

 case 4://=======================================================================================
//  raw2info.c
//  
//   macro to read data from header file (created in psd2ntup.c) or raw PMT file and
//   provide information on the run.
 
raw2info(psdpref);
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
