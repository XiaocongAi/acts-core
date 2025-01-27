// This file is part of the Acts project.
//
// Copyright (C) 2019 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ActsExamples/Validation/TrackSummaryPlotTool.hpp"

#include "Acts/Utilities/Helpers.hpp"

ActsExamples::TrackSummaryPlotTool::TrackSummaryPlotTool(
    const ActsExamples::TrackSummaryPlotTool::Config& cfg,
    Acts::Logging::Level lvl)
    : m_cfg(cfg),
      m_logger(Acts::getDefaultLogger("TrackSummaryPlotTool", lvl)) {}

void ActsExamples::TrackSummaryPlotTool::book(
    TrackSummaryPlotTool::TrackSummaryPlotCache& trackSummaryPlotCache) const {
  PlotHelpers::Binning bEta = m_cfg.varBinning.at("Eta");
  PlotHelpers::Binning bCostheta = m_cfg.varBinning.at("Costheta");
  PlotHelpers::Binning bPt = m_cfg.varBinning.at("Pt");
  PlotHelpers::Binning bNum = m_cfg.varBinning.at("Num");
  PlotHelpers::Binning bPu = m_cfg.varBinning.at("Purity");
  ACTS_DEBUG("Initialize the histograms for track info plots");
  // number of track states versus eta
  trackSummaryPlotCache.nStates_vs_eta = PlotHelpers::bookProf(
      "nStates_vs_eta", "Number of total states vs. #eta", bEta, bNum);
  // number of measurements versus eta
  trackSummaryPlotCache.nMeasurements_vs_eta = PlotHelpers::bookProf(
      "nMeasurements_vs_eta", "Number of measurements vs. #eta", bEta, bNum);
  // number of holes versus eta
  trackSummaryPlotCache.nHoles_vs_eta = PlotHelpers::bookProf(
      "nHoles_vs_eta", "Number of holes vs. #eta", bEta, bNum);
  // number of outliers versus eta
  trackSummaryPlotCache.nOutliers_vs_eta = PlotHelpers::bookProf(
      "nOutliers_vs_eta", "Number of outliers vs. #eta", bEta, bNum);
  // number of Shared Hits versus eta
  trackSummaryPlotCache.nSharedHits_vs_eta = PlotHelpers::bookProf(
      "nSharedHits_vs_eta", "Number of Shared Hits vs. #eta", bEta, bNum);
  // number of track states versus pt
  trackSummaryPlotCache.nStates_vs_pt = PlotHelpers::bookProf(
      "nStates_vs_pT", "Number of total states vs. pT", bPt, bNum);
  // number of measurements versus pt
  trackSummaryPlotCache.nMeasurements_vs_pt = PlotHelpers::bookProf(
      "nMeasurements_vs_pT", "Number of measurements vs. pT", bPt, bNum);
  // number of holes versus pt
  trackSummaryPlotCache.nHoles_vs_pt = PlotHelpers::bookProf(
      "nHoles_vs_pT", "Number of holes vs. pT", bPt, bNum);
  // number of outliers versus pt
  trackSummaryPlotCache.nOutliers_vs_pt = PlotHelpers::bookProf(
      "nOutliers_vs_pT", "Number of outliers vs. pT", bPt, bNum);
  // number of Shared Hits versus pt
  trackSummaryPlotCache.nSharedHits_vs_pt = PlotHelpers::bookProf(
      "nSharedHits_vs_pT", "Number of Shared Hits vs. pT", bPt, bNum);

  for (const auto& [pdg, ptBin] : m_cfg.ptBinning) {
    trackSummaryPlotCache.nMeasurements_vs_pT_costheta[pdg] =
        PlotHelpers::bookProf2D(
            Form("nMeasurements_%i_vs_pT_costheta", pdg),
            Form("nMeasurements of particle with absolute pdg = %i", pdg),
            ptBin, bCostheta, bNum);
    trackSummaryPlotCache.nMajorityHits_vs_pT_costheta[pdg] =
        PlotHelpers::bookProf2D(
            Form("nMajorityHits_%i_vs_pT_costheta", pdg),
            Form("nMajorityHits of particle with absolute pdg = %i", pdg),
            ptBin, bCostheta, bNum);
    trackSummaryPlotCache.trackPurity_vs_pT_costheta[pdg] =
        PlotHelpers::bookProf2D(
            Form("trackPurity_%i_vs_pT_costheta", pdg),
            Form("Track purity of particle with absolute pdg = %i", pdg), ptBin,
            bCostheta, bPu);
  }
}

void ActsExamples::TrackSummaryPlotTool::clear(
    TrackSummaryPlotCache& trackSummaryPlotCache) const {
  delete trackSummaryPlotCache.nStates_vs_eta;
  delete trackSummaryPlotCache.nMeasurements_vs_eta;
  delete trackSummaryPlotCache.nOutliers_vs_eta;
  delete trackSummaryPlotCache.nHoles_vs_eta;
  delete trackSummaryPlotCache.nSharedHits_vs_eta;
  delete trackSummaryPlotCache.nStates_vs_pt;
  delete trackSummaryPlotCache.nMeasurements_vs_pt;
  delete trackSummaryPlotCache.nOutliers_vs_pt;
  delete trackSummaryPlotCache.nHoles_vs_pt;
  delete trackSummaryPlotCache.nSharedHits_vs_pt;
  for (auto& [pdg, dist] : trackSummaryPlotCache.nMeasurements_vs_pT_costheta) {
    delete dist;
  }
  for (auto& [pdg, dist] : trackSummaryPlotCache.nMajorityHits_vs_pT_costheta) {
    delete dist;
  }
  for (auto& [pdg, dist] : trackSummaryPlotCache.trackPurity_vs_pT_costheta) {
    delete dist;
  }
}

void ActsExamples::TrackSummaryPlotTool::write(
    const TrackSummaryPlotTool::TrackSummaryPlotCache& trackSummaryPlotCache)
    const {
  ACTS_DEBUG("Write the plots to output file.");
  trackSummaryPlotCache.nStates_vs_eta->Write();
  trackSummaryPlotCache.nMeasurements_vs_eta->Write();
  trackSummaryPlotCache.nOutliers_vs_eta->Write();
  trackSummaryPlotCache.nHoles_vs_eta->Write();
  trackSummaryPlotCache.nSharedHits_vs_eta->Write();
  trackSummaryPlotCache.nStates_vs_pt->Write();
  trackSummaryPlotCache.nMeasurements_vs_pt->Write();
  trackSummaryPlotCache.nOutliers_vs_pt->Write();
  trackSummaryPlotCache.nHoles_vs_pt->Write();
  trackSummaryPlotCache.nSharedHits_vs_pt->Write();
  for (const auto& [pdg, dist] :
       trackSummaryPlotCache.nMeasurements_vs_pT_costheta) {
    dist->Write();
  }
  for (const auto& [pdg, dist] :
       trackSummaryPlotCache.nMajorityHits_vs_pT_costheta) {
    dist->Write();
  }
  for (const auto& [pdg, dist] :
       trackSummaryPlotCache.trackPurity_vs_pT_costheta) {
    dist->Write();
  }
}

void ActsExamples::TrackSummaryPlotTool::fill(
    TrackSummaryPlotTool::TrackSummaryPlotCache& trackSummaryPlotCache,
    const Acts::BoundTrackParameters& fittedParameters,
    const Acts::PdgParticle& majorityParticlePdg, size_t nStates,
    size_t nMeasurements, size_t nMajorityHits, size_t nOutliers, size_t nHoles,
    size_t nSharedHits) const {
  using Acts::VectorHelpers::eta;
  using Acts::VectorHelpers::perp;
  using Acts::VectorHelpers::theta;
  const auto& momentum = fittedParameters.momentum();
  const double fit_eta = eta(momentum);
  const double fit_theta = theta(momentum);
  const double fit_pT = perp(momentum);

  PlotHelpers::fillProf(trackSummaryPlotCache.nStates_vs_eta, fit_eta, nStates);
  PlotHelpers::fillProf(trackSummaryPlotCache.nMeasurements_vs_eta, fit_eta,
                        nMeasurements);
  PlotHelpers::fillProf(trackSummaryPlotCache.nOutliers_vs_eta, fit_eta,
                        nOutliers);
  PlotHelpers::fillProf(trackSummaryPlotCache.nHoles_vs_eta, fit_eta, nHoles);
  PlotHelpers::fillProf(trackSummaryPlotCache.nSharedHits_vs_eta, fit_eta,
                        nSharedHits);

  PlotHelpers::fillProf(trackSummaryPlotCache.nStates_vs_pt, fit_pT, nStates);
  PlotHelpers::fillProf(trackSummaryPlotCache.nMeasurements_vs_pt, fit_pT,
                        nMeasurements);
  PlotHelpers::fillProf(trackSummaryPlotCache.nOutliers_vs_pt, fit_pT,
                        nOutliers);
  PlotHelpers::fillProf(trackSummaryPlotCache.nHoles_vs_pt, fit_pT, nHoles);
  PlotHelpers::fillProf(trackSummaryPlotCache.nSharedHits_vs_pt, fit_pT,
                        nSharedHits);
  int pdg = std::abs(majorityParticlePdg);
  if (trackSummaryPlotCache.nMeasurements_vs_pT_costheta.count(pdg) > 0) {
    PlotHelpers::fillProf2D(
        trackSummaryPlotCache.nMeasurements_vs_pT_costheta[pdg], fit_pT,
        std::cos(fit_theta), nMeasurements);
  }
  if (trackSummaryPlotCache.nMajorityHits_vs_pT_costheta.count(pdg) > 0) {
    PlotHelpers::fillProf2D(
        trackSummaryPlotCache.nMajorityHits_vs_pT_costheta[pdg], fit_pT,
        std::cos(fit_theta), nMajorityHits);
    PlotHelpers::fillProf2D(
        trackSummaryPlotCache.trackPurity_vs_pT_costheta[pdg], fit_pT,
        std::cos(fit_theta), nMajorityHits * 1.0 / nMeasurements);
  }
}
