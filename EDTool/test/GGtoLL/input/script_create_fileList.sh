#!/bin/bash

python create_fileList.py \
-d /pnfs/iihe/cms/store/user/kplee/GGToLL_GEN_v2/CRAB_PrivateMC/crab_GGtoLL_LowMass_m5to50 \
-o input_pTHatMinDiverge_1p0.txt

python create_fileList.py \
-d /pnfs/iihe/cms/store/user/kplee/GGToLL_GEN_v3/CRAB_PrivateMC/crab_GGtoLL_LowMass_m5to50_pTHatMinDiverge_1p5 \
-o input_pTHatMinDiverge_1p5.txt

python create_fileList.py \
-d /pnfs/iihe/cms/store/user/kplee/GGToLL_GEN_v3/CRAB_PrivateMC/crab_GGtoLL_LowMass_m5to50_pTHatMinDiverge_2p0 \
-o input_pTHatMinDiverge_2p0.txt

python create_fileList.py \
-d /pnfs/iihe/cms/store/user/kplee/GGToLL_GEN_v3/CRAB_PrivateMC/crab_GGtoLL_LowMass_m5to50_pTHatMinDiverge_2p5 \
-o input_pTHatMinDiverge_2p5.txt

python create_fileList.py \
-d /pnfs/iihe/cms/store/user/kplee/GGToLL_GEN_v3/CRAB_PrivateMC/crab_GGtoLL_LowMass_m5to50_pTHatMinDiverge_3p0 \
-o input_pTHatMinDiverge_3p0.txt

python create_fileList.py \
-d /pnfs/iihe/cms/store/user/kplee/GGToLL_GEN_v3/CRAB_PrivateMC/crab_GGtoLL_LowMass_m5to50_pTHatMinDiverge_3p5 \
-o input_pTHatMinDiverge_3p5.txt

python create_fileList.py \
-d /pnfs/iihe/cms/store/user/kplee/GGToLL_GEN_v3/CRAB_PrivateMC/crab_GGtoLL_LowMass_m5to50_pTHatMinDiverge_4p0 \
-o input_pTHatMinDiverge_4p0.txt

python create_fileList.py \
-d /pnfs/iihe/cms/store/user/kplee/GGToLL_GEN_v3/CRAB_PrivateMC/crab_GGtoLL_LowMass_m5to50_pTHatMinDiverge_4p5 \
-o input_pTHatMinDiverge_4p5.txt

python create_fileList.py \
-d /pnfs/iihe/cms/store/user/kplee/GGToLL_GEN_v1/CRAB_PrivateMC/crab_GGtoLL_LowMass_m5to50 \
-o input_pTHatMinDiverge_5p0.txt

echo "done"