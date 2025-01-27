// This file is part of the Acts project.
//
// Copyright (C) 2021 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>

#include "CommonUtils.h"
#include "TreeReader_new.h"
#include "SetStyleATLAS.hpp"


std::string tag1 = "reco/pipijpsi/CKF.estimated.maxSeedsPerSpM6.sigmaScattering300.PreditedDriftSign.chi2Cut30.maxPropSteps330.NoStepAjustError.allseeds.NoNoise";
std::string tag2 = "reco/pipijpsi/CKF.estimated.maxSeedsPerSpM6.sigmaScattering300.PreditedDriftSign.chi2Cut30.maxPropSteps330.NoStepAjustError.allseeds";


void draw_perf_pipijpsi(
    const std::vector<std::string>& seedingPerfFileNames =
        {
          "./" + tag1 + "/pipijpsiBKG_seeding_hists.root",	
          "./" + tag2 + "/pipijpsiBKG_seeding_hists.root",	
	},
    const std::vector<std::string>& ckfPerfFileNames =
        {
          "./" + tag1 + "/pipijpsiBKG_ckf.root",	
          "./" + tag2 + "/pipijpsiBKG_ckf.root",	
	},
    const std::vector<std::string>& ckfTrackFileNames =
        {
          "./" + tag1 + "/pipijpsiBKG_tracksummary_ckf.root",	
          "./" + tag2 + "/pipijpsiBKG_tracksummary_ckf.root",	
	},
   const std::vector<std::string>& legends_plus =
        {
          "W/O backgrounds",
          "W/ backgrounds",
	},
    const std::vector<std::string>& legends_minus =
        {
          "W/O backgrounds",
          "W/ backgrounds",
	},
    const std::vector<std::string>& legends =
        {
          "W/O backgrounds",
          "W/ backgrounds",
	},
	std::vector<int> colors_plus={
          854,
          796,
	},
	std::vector<int> colors_minus={
          854,
          796,
	},
	std::vector<int> colors={
          854,
          796,
	},
	std::vector<int> markers_plus ={20,24},
	std::vector<int> markers_minus ={20,24},
	std::vector<int> markers ={20,24},
	int absPdgId=211) {

  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.17);
  gStyle->SetTitleSize(0.05, "xy");
  gStyle->SetLabelSize(0.05, "xy");
  gStyle->SetTitleOffset(1.2, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetNdivisions(505, "y");
  gStyle->SetPaintTextFormat(".3f");

  std::vector<TH1F*> track_purity;

  std::vector<TEfficiency*> seeding_eff_2d;
  std::vector<TProfile2D*> seeding_nDuplicated_2d;
  std::vector<TEfficiency*> ckf_eff_2d;
  std::vector<TEfficiency*> ckf_fakerate_2d;
  std::vector<TEfficiency*> ckf_duplirate_2d;
  std::vector<TProfile2D*> ckf_nMeasurements_2d;
  std::vector<TProfile2D*> ckf_nMajorityHits_2d;
  std::vector<TProfile2D*> ckf_nDuplicated_2d;
  std::vector<TProfile2D*> ckf_trackPurity_2d;

  std::vector<TLegend*> legs;
  for (int i = 0; i < 6; ++i) {
    TLegend* legend = new TLegend(0.6, 0.75, 0.9, 0.9);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetTextFont(42);
    legs.push_back(legend);
  }
  
  int nTrackFiles = seedingPerfFileNames.size();
  for (int i = 0; i < nTrackFiles; ++i) {
    std::string effName= "trackeff_" + std::to_string(absPdgId) +"_vs_pT_costheta";
    std::string fakeName= "fakerate_" + std::to_string(absPdgId) +"_vs_pT_costheta";
    std::string dupliName= "duplicationRate_" + std::to_string(absPdgId) +"_vs_pT_costheta";
    std::string nHitsName= "nMeasurements_" + std::to_string(absPdgId) +"_vs_pT_costheta";
    std::string nMajorityHitsName= "nMajorityHits_" + std::to_string(absPdgId) +"_vs_pT_costheta";
    std::string nDuplicatedName= "nDuplicated_" + std::to_string(absPdgId) +"_vs_pT_costheta";
    std::string puName= "trackPurity_" + std::to_string(absPdgId) +"_vs_pT_costheta";
    
    TFile* seedfile = TFile::Open(seedingPerfFileNames[i].c_str(), "READ"); 
    seeding_eff_2d.push_back((TEfficiency*)seedfile->Get(effName.c_str())); 
    seeding_nDuplicated_2d.push_back((TProfile2D*)seedfile->Get(nDuplicatedName.c_str())); 
    
    TFile* ckffile = TFile::Open(ckfPerfFileNames[i].c_str(), "READ"); 
    ckf_eff_2d.push_back((TEfficiency*)ckffile->Get(effName.c_str())); 
    ckf_fakerate_2d.push_back((TEfficiency*)ckffile->Get(fakeName.c_str())); 
    ckf_duplirate_2d.push_back((TEfficiency*)ckffile->Get(dupliName.c_str())); 
    ckf_nMeasurements_2d.push_back((TProfile2D*)ckffile->Get(nHitsName.c_str())); 
    ckf_nMajorityHits_2d.push_back((TProfile2D*)ckffile->Get(nMajorityHitsName.c_str())); 
    ckf_nDuplicated_2d.push_back((TProfile2D*)ckffile->Get(nDuplicatedName.c_str())); 
    ckf_trackPurity_2d.push_back((TProfile2D*)ckffile->Get(puName.c_str())); 


    seeding_nDuplicated_2d[i]->GetXaxis()->SetTitle("Truth p_{T} [GeV/c]"); 
    seeding_nDuplicated_2d[i]->GetYaxis()->SetTitle("Truth cos#theta"); 
    seeding_nDuplicated_2d[i]->GetZaxis()->SetTitle("nDuplicated");
    seeding_nDuplicated_2d[i]->GetZaxis()->SetRangeUser(0, 3);
    ckf_nDuplicated_2d[i]->GetXaxis()->SetTitle("Truth p_{T} [GeV/c]"); 
    ckf_nDuplicated_2d[i]->GetYaxis()->SetTitle("Truth cos#theta"); 
    ckf_nDuplicated_2d[i]->GetZaxis()->SetTitle("nDuplicated");
    ckf_nDuplicated_2d[i]->GetZaxis()->SetRangeUser(0, 3);

  }

  for (int i = 0; i < nTrackFiles; ++i) {
    TFile* trackfile = TFile::Open(ckfTrackFileNames[i].c_str(), "READ"); 
    TTree* tracktree = (TTree*)trackfile->Get("tracksummary");
    TH1F* htrackPurity = new TH1F(Form("trackPurity_%i", i), "", 80, 0.2, 1);
    tracktree->Draw(Form("nMajorityHits/nMeasurements>>trackPurity_%i",i));
    htrackPurity->GetXaxis()->SetTitle("nMajorityParticleHits/nMeasurements");
    htrackPurity->GetYaxis()->SetTitle("Entries");
    setHistStyle(htrackPurity, colors[i]);
    
    track_purity.push_back(htrackPurity);
    legs[0]->AddEntry(track_purity[i], legends[i].c_str(),"APL"); 
  }

   std::string myLabel_plus;
   std::string myLabel_minus;
   std::string myLabel;
  if(absPdgId == 211){
   myLabel_plus = "#pi^{+}, #psi(3686)#rightarrow #pi^{+}#pi^{-}J/#psi(#rightarrow #mu^{+}#mu^{-})";
   myLabel_minus = "#pi^{-}, #psi(3686)#rightarrow #pi^{+}#pi^{-}J/#psi(#rightarrow #mu^{+}#mu^{-})";
   myLabel = "#pi, #psi(3686)#rightarrow #pi^{+}#pi^{-}J/#psi(#rightarrow #mu^{+}#mu^{-})";
  } else {
   myLabel_plus = "#mu^{+}, #psi(3686)#rightarrow #pi^{+}#pi^{-}J/#psi(#rightarrow #mu^{+}#mu^{-})";
   myLabel_minus = "#mu^{-}, #psi(3686)#rightarrow #pi^{+}#pi^{-}J/#psi(#rightarrow #mu^{+}#mu^{-})";
   myLabel = "#mu, #psi(3686)#rightarrow #pi^{+}#pi^{-}J/#psi(#rightarrow #mu^{+}#mu^{-})";
  }

  // Now draw the plots
  std::vector<TCanvas*> cs1d;
  for(int i=0; i < 1; ++i){
     cs1d.push_back(new TCanvas(Form("c1d_%i", i), "", 600, 500)); 
  }
  std::vector<TCanvas*> cs2d;
  for(int i=0; i < 18; ++i){
     cs2d.push_back(new TCanvas(Form("c2d_%i", i), "", 600, 500)); 
  }

  //1d plots
  cs1d[0] -> cd();
  for (int i = 0; i < nTrackFiles; ++i) {
    gPad->SetRightMargin(0.15);
    if(i==0){
      track_purity[i]->Draw();
      track_purity[i]->GetYaxis()->SetRangeUser(0, 50000);
    } else {
      track_purity[i]->Draw("same");
    }
  }
  myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
  //myText(0.55, 0.8, 1, 0.04, legends[i].c_str());
  legs[0]->Draw(); 


  //2d plots 
  for (int i = 0; i < nTrackFiles; ++i) {
    cs2d[0+i*9]->cd();
    gPad->SetRightMargin(0.15);
    gStyle->SetPaintTextFormat(".3f");
    seeding_eff_2d[i]->Draw("TEXT colz");
    myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
    myText(0.55, 0.8, 1, 0.04, legends[i].c_str());
 
    cs2d[1+i*9]->cd();
    gPad->SetRightMargin(0.15);
    gStyle->SetPaintTextFormat(".3f");
    seeding_nDuplicated_2d[i]->Draw("TEXT colz");
    myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
    myText(0.55, 0.8, 1, 0.04, legends[i].c_str());
  
    cs2d[2+i*9]->cd();
    gPad->SetRightMargin(0.15);
    gStyle->SetPaintTextFormat(".3f");
    ckf_eff_2d[i]->Draw("TEXT colz");
    myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
    myText(0.55, 0.8, 1, 0.04, legends[i].c_str());
  
    cs2d[3+i*9]->cd();
    gPad->SetRightMargin(0.15);
    gStyle->SetPaintTextFormat(".3f");
    ckf_fakerate_2d[i]->Draw("TEXT colz");
    myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
    myText(0.55, 0.8, 1, 0.04, legends[i].c_str());
  
    cs2d[4+i*9]->cd();
    gPad->SetRightMargin(0.15);
    gStyle->SetPaintTextFormat(".3f");
    ckf_duplirate_2d[i]->Draw("TEXT colz");
    myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
    myText(0.55, 0.8, 1, 0.04, legends[i].c_str());
    
    cs2d[5+i*9]->cd();
    gPad->SetRightMargin(0.15);
    //gStyle->SetPaintTextFormat(".1f");
    ckf_nMeasurements_2d[i]->Draw("TEXT colz");
    myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
    myText(0.55, 0.8, 1, 0.04, legends[i].c_str());
    
    cs2d[6+i*9]->cd();
    //gStyle->SetPaintTextFormat(".1f");
    gPad->SetRightMargin(0.15);
    ckf_nMajorityHits_2d[i]->Draw("TEXT colz");
    ckf_nMajorityHits_2d[i]->GetZaxis()->SetTitle("nMajorityHits");
    myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
    myText(0.55, 0.8, 1, 0.04, legends[i].c_str());
  
    cs2d[7+i*9]->cd();
    //gStyle->SetPaintTextFormat(".1f");
    gPad->SetRightMargin(0.15);
    ckf_nDuplicated_2d[i]->Draw("TEXT colz");
    myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
    myText(0.55, 0.8, 1, 0.04, legends[i].c_str());


    cs2d[8+i*9]->cd();
    gPad->SetRightMargin(0.15);
   // gStyle->SetPaintTextFormat(".2f");
    ckf_trackPurity_2d[i]->Draw("TEXT colz");
    myText(0.4, 0.85, 1, 0.04, myLabel.c_str());
    myText(0.55, 0.8, 1, 0.04, legends[i].c_str());
  }


  std::string particle = "";
  if(absPdgId==211){
	  particle = "pi";
  }
  if(absPdgId==13){
	  particle = "mu";
  }
 
  cs1d[0]->SaveAs(Form("STCF_pipijpsi_ckf_truthMatchProb.0.8_%s_trackPurity_1d.pdf", particle.c_str())); 
  for (int i = 0; i < nTrackFiles; ++i) {
    cs2d[0+9*i]->SaveAs(Form("STCF_pipijpsi_seeding_%s_eff_2d_%i.pdf", particle.c_str(), i));
    cs2d[1+9*i]->SaveAs(Form("STCF_pipijpsi_seeding_%s_nDuplicated_2d_%i.pdf", particle.c_str(), i));
    cs2d[2+9*i]->SaveAs(Form("STCF_pipijpsi_ckf_truthMatchProb.0.8_%s_eff_2d_%i.pdf", particle.c_str(), i));
    cs2d[3+9*i]->SaveAs(Form("STCF_pipijpsi_ckf_truthMatchProb.0.8_%s_fakerate_2d_%i.pdf", particle.c_str(), i));
    cs2d[4+9*i]->SaveAs(Form("STCF_pipijpsi_ckf_truthMatchProb.0.8_%s_duplirate_2d_%i.pdf", particle.c_str(), i));
    cs2d[5+9*i]->SaveAs(Form("STCF_pipijpsi_ckf_truthMatchProb.0.8_%s_nMeasurements_2d_%i.pdf", particle.c_str(), i));
    cs2d[6+9*i]->SaveAs(Form("STCF_pipijpsi_ckf_truthMatchProb.0.8_%s_nMajorityHits_2d_%i.pdf", particle.c_str(), i));
    cs2d[7+9*i]->SaveAs(Form("STCF_pipijpsi_ckf_truthMatchProb.0.8_%s_nDuplicated_2d_%i.pdf", particle.c_str(), i));
    cs2d[8+9*i]->SaveAs(Form("STCF_pipijpsi_ckf_truthMatchProb.0.8_%s_trackPurity_2d_%i.pdf", particle.c_str(), i));
  }
}
