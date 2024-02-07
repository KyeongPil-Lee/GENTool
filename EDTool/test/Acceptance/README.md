# Purpose

Calculate the acceptance of the DY acceptance as a function of dilepton mass, including the systematic variations

## Setup (first time)

* CMSSW_10_6_27: not available under `el9_X` architecture -> make an error in CRAB if the job is submitted
* Need to use Singularity, especially for CentOS7 environment: `cmssw-el7` command
  * https://cms-sw.github.io/singularity.html
  * Submit CRAB under Singularity
  * Related post: https://cms-talk.web.cern.ch/t/crab-jobs-failing-with-error-code-8001-cmsrun-on-lxplus-works-fine/33698/4

```shell
# -- lxplus
export SCRAM_ARCH=slc7_amd64_gcc700
cd /afs/cern.ch/user/k/kplee/work/private/Analysis/DYAcceptance

cmssw-el7 # -- use CentOS7 env. (for this CMSSW & ARCH)

cmsrel CMSSW_10_6_27 # -- where the nanoAODv9 was run
cd CMSSW_10_6_27/src
git clone git@github.com:KyeongPil-Lee/GENTool.git -b accCorr_DYFullRun2
scram b

```

## Setup working area

```shell
# -- lxplus
# -- open two terminals: one for scram, one for running
cd /afs/cern.ch/user/k/kplee/work/private/Analysis/DYAcceptance/CMSSW_10_6_27/src/GENTool/EDTool/test/Acceptance

cmssw-el7 # -- use CentOS7 env. (for this CMSSW & ARCH)

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
exampleFile=/store/mc/RunIISummer20UL18MiniAODv2/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/270000/C10AF425-1891-5849-A619-CE2D859DC4FF.root \
adjustPDFWeight=true \
requestName=mm_m50_18 >&run_DYAcceptanceProducer.log&
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

