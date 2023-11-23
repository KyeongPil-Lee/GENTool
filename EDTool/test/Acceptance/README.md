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
cmsRun run_DYAcceptanceProducer.py cutAtM100=true >&run_DYAcceptanceProducer.log&
tail -f run_DYAcceptanceProducer.log

# -- investigation version
cmsRun run_DYAcceptanceProducer.py cutAtM100=true InvOverflow=true >&run_DYAcceptanceProducer.log&
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

