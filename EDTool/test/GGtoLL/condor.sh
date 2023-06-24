#!/bin/bash

cd /user/kplee/Analysis/CMSSW/CMSSW_10_6_30_patch1/src/GENTool/EDTool/test/GGtoLL
cmsenv

# python run_GenDileptonHistProducer_GGtoLL_lowMass_arg.py ptMin=$1
cmsRun run_GenDileptonHistProducer_GGtoLL_lowMass_arg.py ptMin=$1

echo "done"
