#!/bin/bash


#2mu2pi

ckfPropSteps="330"
maxSeeds="10000"
ckfchi2max=30
#default is 2.74
cotThetaMax=3.0
#default is 0.1 
radLengthPerSeed=0.03
impactMax=10
sigmaScattering=300
deltaRMin=20
deltaRMax=80
maxSeedsPerSpM=6

tag1=CKF.estimated.maxSeedsPerSpM$maxSeedsPerSpM.sigmaScattering$sigmaScattering.PreditedDriftSign.chi2Cut30.maxPropSteps330.NoStepAjustError.allseeds
tag2=CKF.estimated.maxSeedsPerSpM$maxSeedsPerSpM.sigmaScattering$sigmaScattering.PreditedDriftSign.chi2Cut30.maxPropSteps330.NoStepAjustError.allseeds.NoNoise


matInputFile=/home/xiaocong/Software/Oscar/acts/build/bin/mat-STCF_tracker_alt5_manual_MDI.json
geoTGeoJsonFile=/home/xiaocong/Software/Oscar/acts/build/bin/tgeo_STCF_tracker_config_alt.json
geoTGeoRootFile=/home/xiaocong/Software/Oscar/acts/build/bin/STCF_tracker.root

inputDir=./
inputFile_particles=sim_pipijpsi5w.root
inputFile_hits=simBKG_Info5w.root
outputDir1=./reco/pipijpsi/$tag1
outputDir2=./reco/pipijpsi/$tag2


#no noise
run1="../build/bin/ActsExampleCKFTracksSTCF_BKG  --seed-maxSeedsPerSpM $maxSeedsPerSpM --reco-ignore-noise --seed-impact-max $impactMax --seed-sigma-scattering $sigmaScattering --seed-deltar-min $deltaRMin  --seed-deltar-max $deltaRMax --seed-cottheta-max $cotThetaMax --seed-rad-length-per-seed $radLengthPerSeed --seed-max-seeds $maxSeeds --ckf-prop-steps $ckfPropSteps  --ckf-selection-nmax 1 --ckf-selection-chi2max=${ckfchi2max} --ckf-initial-variance-inflation=1:1:1000:1000:1000:1 -j 1 --mat-input-type file --mat-input-file=${matInputFile} --geo-tgeo-filename=${geoTGeoRootFile}  --geo-tgeo-jsonconfig=${geoTGeoJsonFile}  --input-dir=${inputDir} --input-files=${inputFile_particles} --input-files=${inputFile_hits} --output-dir=${outputDir2} --bf-constant-tesla=0:0:1  --geo-selection-config-file=/home/xiaocong/Software/Oscar/acts/Examples/Algorithms/TrackFinding/share/geoSelection-STCFDetector.json"

run2="../build/bin/ActsExampleCKFTracksSTCF_BKG  --seed-maxSeedsPerSpM $maxSeedsPerSpM --seed-impact-max $impactMax --seed-sigma-scattering $sigmaScattering --seed-deltar-min $deltaRMin  --seed-deltar-max $deltaRMax --seed-cottheta-max $cotThetaMax --seed-rad-length-per-seed $radLengthPerSeed --seed-max-seeds $maxSeeds --ckf-prop-steps $ckfPropSteps  --ckf-selection-nmax 1 --ckf-selection-chi2max=${ckfchi2max} --ckf-initial-variance-inflation=1:1:1000:1000:1000:1 -j 1 --mat-input-type file --mat-input-file=${matInputFile} --geo-tgeo-filename=${geoTGeoRootFile}  --geo-tgeo-jsonconfig=${geoTGeoJsonFile}  --input-dir=${inputDir} --input-files=${inputFile_particles} --input-files=${inputFile_hits} --output-dir=${outputDir1} --bf-constant-tesla=0:0:1  --geo-selection-config-file=/home/xiaocong/Software/Oscar/acts/Examples/Algorithms/TrackFinding/share/geoSelection-STCFDetector.json"

eval $run1
eval $run2
#-l 0 -n 50  > log 
