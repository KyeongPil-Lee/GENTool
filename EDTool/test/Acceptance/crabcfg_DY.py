from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
# config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_DYAcceptanceProducer.py'

config.Data.inputDataset = ''

config.Data.inputDBS = 'global'
# config.Data.splitting = 'Automatic' # -- a few problems in getting job status using API
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
config.Data.publication = False

config.Site.storageSite = 'T2_BE_IIHE'
# config.Site.storageSite = 'T3_KR_KNU' # -- as T2 BE server is shut down this week (13 Nov. 2023) ...

# version = 'v5_1'
version = 'v5_2'
config.General.workArea = 'CRABDir_%s' % version
config.Data.outLFNDirBase = '/store/user/kplee/DYAccPlot_%s' % version

# -- dasgoclient --query "/DYJetsToMuMu_M-*powhegMiNNLO*/RunIISummer20UL16MiniAODAPVv2*/MINIAODSIM"
list_sample_16pre_mm = [ 
'/DYJetsToMuMu_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11_ext1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-50_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM',
'/DYJetsToMuMu_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-200to400_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-400to500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-500to700_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-700to800_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-800to1000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-1000to1500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-1500to2000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-2000toInf_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToMuMu_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
]

# -- dasgoclient --query "/DYJetsToEE_M-*powhegMiNNLO*/RunIISummer20UL16MiniAODAPVv2*/MINIAODSIM"
list_sample_16pre_ee = [ 
'/DYJetsToEE_M-1000to1500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11_ext1-v2/MINIAODSIM',
'/DYJetsToEE_M-1500to2000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-2000toInf_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-200to400_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-400to500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-500to700_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM',
'/DYJetsToEE_M-700to800_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-800to1000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
'/DYJetsToEE_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM',
]

# -- dasgoclient --query "/DYJetsToMuMu_M-*powhegMiNNLO*/RunIISummer20UL16MiniAODv2*/MINIAODSIM"
list_sample_16post_mm = [
'/DYJetsToMuMu_M-1000to1500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17_ext1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-1500to2000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-2000toInf_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-200to400_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-400to500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-500to700_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-50_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v1/MINIAODSIM',
'/DYJetsToMuMu_M-700to800_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-800to1000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToMuMu_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
]

# -- dasgoclient --query "/DYJetsToEE_M-*powhegMiNNLO*/RunIISummer20UL16MiniAODv2*/MINIAODSIM"
list_sample_16post_ee = [
'/DYJetsToEE_M-1000to1500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17_ext1-v2/MINIAODSIM',
'/DYJetsToEE_M-1500to2000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-2000toInf_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-200to400_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-400to500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-500to700_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v1/MINIAODSIM',
'/DYJetsToEE_M-700to800_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-800to1000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
'/DYJetsToEE_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v2/MINIAODSIM',
]

# -- dasgoclient --query "/DYJetsToMuMu_M-*powhegMiNNLO*/RunIISummer20UL17MiniAODv2*/MINIAODSIM"
list_sample_17_mm = [
'/DYJetsToMuMu_M-1000to1500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9_ext1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-1500to2000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-2000toInf_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-200to400_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-400to500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-500to700_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-700to800_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-800to1000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToMuMu_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9_ext1-v2/MINIAODSIM',
'/DYJetsToMuMu_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v3/MINIAODSIM',
]

# -- dasgoclient --query "/DYJetsToEE_M-*powhegMiNNLO*/RunIISummer20UL17MiniAODv2*/MINIAODSIM"
list_sample_17_ee = [
'/DYJetsToEE_M-1000to1500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9_ext1-v2/MINIAODSIM',
'/DYJetsToEE_M-1500to2000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-2000toInf_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-200to400_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-400to500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-500to700_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-700to800_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-800to1000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM',
'/DYJetsToEE_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9_ext1-v2/MINIAODSIM',
]

# -- dasgoclient --query "/DYJetsToMuMu_M-*powhegMiNNLO*/RunIISummer20UL18MiniAODv2*/MINIAODSIM"
list_sample_18_mm = [
'/DYJetsToMuMu_M-1000to1500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1_ext1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-1500to2000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-2000toInf_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-200to400_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-400to500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-500to700_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-700to800_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-800to1000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToMuMu_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v3/MINIAODSIM',
]

# -- dasgoclient --query "/DYJetsToEE_M-*powhegMiNNLO*/RunIISummer20UL18MiniAODv2*/MINIAODSIM"
list_sample_18_ee = [
'/DYJetsToEE_M-1000to1500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1_ext1-v2/MINIAODSIM',
'/DYJetsToEE_M-1500to2000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-2000toInf_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-200to400_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-400to500_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-500to700_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-700to800_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-800to1000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
'/DYJetsToEE_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM',
]

dic_type_samples = {
    "16pre_mm" : list_sample_16pre_mm,
    "16post_mm" : list_sample_16post_mm,
    "17_mm" : list_sample_17_mm,
    "18_mm" : list_sample_18_mm,
    "16pre_ee" : list_sample_16pre_ee,
    "16post_ee" : list_sample_16post_ee,
    "17_ee" : list_sample_17_ee,
    "18_ee" : list_sample_18_ee,
}

dic_type_globalTag = {
    "16pre_mm" : "106X_mcRun2_asymptotic_preVFP_v11",
    "16post_mm" : "106X_mcRun2_asymptotic_v17",
    "17_mm" : "106X_mc2017_realistic_v9",
    "18_mm" : "106X_upgrade2018_realistic_v16_L1v1",

    "16pre_ee" : "106X_mcRun2_asymptotic_preVFP_v11",
    "16post_ee" : "106X_mcRun2_asymptotic_v17",
    "17_ee" : "106X_mc2017_realistic_v9",
    "18_ee" : "106X_upgrade2018_realistic_v16_L1v1",
}

def Get_MassRange(datasetName):
  massRange = datasetName
  massRange = massRange.split("/")[1] # -- e.g. DYJetsToEE_M-800to1000_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos
  massRange = massRange.split("-")[1] # -- e.g. 800to1000_H2ErratumFix_TuneCP5_13TeV
  massRange = massRange.split("_")[0] # -- e.g. 800to1000

  cut_m100 = "false"
  if massRange == "50": cut_m100 = "true"

  if massRange == "10to50" and "_ext" in datasetName:
      massRange = massRange + "_ext"

  if massRange == "50" and "ZptWeighted" in datasetName:
      massRange = massRange + "_ZptWeighted"
      if "_ext" in datasetName:
          massRange = massRange + "_ext"

  # -- special case (no mass info in the dataset name)
  # -- ignore the above logic
  if datasetName.split("/")[1] == "DYJetsToMuMu_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos":
    massRange = "50_H2ErratumFix"

  return massRange


# 'MultiCRAB' part
if __name__ == '__main__':
    
    from multiprocessing import Process
    from CRABAPI.RawCommand import crabCommand
    from CRABClient.ClientExceptions import ClientException
    # from httplib import HTTPException

    def submit(config):
        try:
            crabCommand('submit', config = config)
        # except HTTPException as hte:
        #     print("Failed submitting task: %s" % (hte.headers))
        except ClientException as cle:
            print("Failed submitting task: %s" % (cle))

    for sampleType in dic_type_samples.keys():
        theGlobalTag = dic_type_globalTag[sampleType]
        channel = ""
        if "_ee" in sampleType: channel = "ee"
        if "_mm" in sampleType: channel = "mm"

        list_sample = dic_type_samples[sampleType]
        for datasetName in list_sample:

            massRange = Get_MassRange(datasetName)

            era = sampleType.split("_")[0]
            theRequestName = "%s_m%s_%s" % (channel, massRange, era)

            cut_m100 = "false"
            if "m50_" in theRequestName: cut_m100 = "true"

            list_param = []
            list_param.append( "globalTag=%s" % theGlobalTag )
            list_param.append( "channel=%s" % channel )
            list_param.append( "cutAtM100=%s" % cut_m100 )
            # -- adjust PDF weights (remove unphysically large weights)
            list_param.append( "adjustPDFWeight=true" )
            list_param.append( "requestName=%s" % theRequestName )

            # print("%s" % datasetName)
            # print("--> requestName = %s" % theRequestName)
            # print("--> global tag  = %s" % theGlobalTag)
            # print("--> channel     = %s" % channel)
            # print("--> cut_m100    = %s" % cut_m100)
            # print("--> list_param: ", list_param)
            # print("\n")

            config.General.requestName = theRequestName
            config.Data.inputDataset = datasetName
            config.JobType.pyCfgParams = list_param
            # crabCommand('submit', config = config)

            # -- recommended way from CRAB3 twiki when you change pyCfgParams: https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3FAQ#Multiple_submission_fails_with_a
            p = Process(target=submit, args=(config,))
            p.start()
            p.join()

        #     break
        # break



    # config.General.requestName = "DYEE_M50"
    # config.Data.inputDataset = '/DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    # config.JobType.pyCfgParams = ["globalTag=106X_mcRun2_asymptotic_preVFP_v11", "leptonType=electron"]

    # p = Process(target=submit, args=(config,))
    # p.start()
    # p.join()



