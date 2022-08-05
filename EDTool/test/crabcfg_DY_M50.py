from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_GenDileptonHistProducer_forCRAB.py'

config.Data.inputDataset = ''

config.Data.inputDBS = 'global'
config.Data.splitting = 'Automatic'
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 5
config.Data.publication = False

config.Site.storageSite = 'T2_BE_IIHE'

version = 'v1'
config.Data.outLFNDirBase = '/store/user/kplee/GENPlot_DY_%s' % version

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from multiprocessing import Process
    from CRABAPI.RawCommand import crabCommand
    from CRABClient.ClientExceptions import ClientException
    from httplib import HTTPException

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
        except ClientException as cle:
            print "Failed submitting task: %s" % (cle)

    config.General.requestName = "DYMuMu_M50"
    config.Data.inputDataset = '/DYJetsToMuMu_M-50_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    config.JobType.pyCfgParams = ["globalTag=106X_mcRun2_asymptotic_preVFP_v11", "leptonType=muon"]
    # crabCommand('submit', config = config)

    # -- recommended way from CRAB3 twiki when you change pyCfgParams: https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3FAQ#Multiple_submission_fails_with_a
    p = Process(target=submit, args=(config,))
    p.start()
    p.join()

    config.General.requestName = "DYEE_M50"
    config.Data.inputDataset = '/DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    config.JobType.pyCfgParams = ["globalTag=106X_mcRun2_asymptotic_preVFP_v11", "leptonType=electron"]

    p = Process(target=submit, args=(config,))
    p.start()
    p.join()



