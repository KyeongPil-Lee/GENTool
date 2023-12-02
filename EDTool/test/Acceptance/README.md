# Purpose

Calculate the acceptance of the DY acceptance as a function of dilepton mass, including the systematic variations

## Setup (first time)

```shell
# -- lxplus
cd /afs/cern.ch/user/k/kplee/work/private/Analysis/DYAcceptance
cmsrel CMSSW_10_6_27 # -- where the nanoAODv9 was run
cd CMSSW_10_6_27/src
git clone git@github.com:KyeongPil-Lee/GENTool.git -b accCorr_DYFullRun2
scram b
```

## Setup working area

```shell
# -- lxplus
# -- open two termiinals: one for scram, one for running
cd /afs/cern.ch/user/k/kplee/work/private/Analysis/DYAcceptance/CMSSW_10_6_27/src/GENTool/EDTool/test/Acceptance
cmsenv
voms-proxy-init --voms cms
```

## Running interactively

```shell
# -- m100-200 sample (UL18, mm)
# -- add 'InvOverflow=true' for the investigation
cmsRun run_DYAcceptanceProducer.py \
channel=mm \
globalTag=106X_upgrade2018_realistic_v16_L1v1 \
exampleFile=/store/mc/RunIISummer20UL18MiniAODv2/DYJetsToMuMu_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/2560000/7BCE6EC9-BA6F-F540-A331-EEB3C4AF915F.root >&run_DYAcceptanceProducer.log&
tail -f run_DYAcceptanceProducer.log

# -- m50 sample (UL18, mm)
cmsRun run_DYAcceptanceProducer.py \
channel=mm \
globalTag=106X_upgrade2018_realistic_v16_L1v1 \
cutAtM100=true \
exampleFile=/store/mc/RunIISummer20UL18MiniAODv2/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/270000/C10AF425-1891-5849-A619-CE2D859DC4FF.root >&run_DYAcceptanceProducer.log&
tail -f run_DYAcceptanceProducer.log
```

## CRAB

### Submission

```bash
python crabcfg_DY.py
```

### check the status

```
python CRAB_Status.py -d CRABDir_v3
```

