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
config.Data.outLFNDirBase = '/store/user/kplee/GENPlot_GGtoLL_%s' % version

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

    config.General.requestName = "GGtoMuMu_m50"
    config.Data.inputDataset = '/GGToLL_TuneCP5_13TeV-pythia8/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM'
    config.JobType.pyCfgParams = ["globalTag=106X_upgrade2018_realistic_v16_L1v1", "leptonType=muon"]
    # crabCommand('submit', config = config)

    # -- recommended way from CRAB3 twiki when you change pyCfgParams: https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3FAQ#Multiple_submission_fails_with_a
    p = Process(target=submit, args=(config,))
    p.start()
    p.join()

    config.General.requestName = "GGtoEE_m50"
    config.Data.inputDataset = '/GGToLL_TuneCP5_13TeV-pythia8/RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2/MINIAODSIM'
    config.JobType.pyCfgParams = ["globalTag=106X_upgrade2018_realistic_v16_L1v1", "leptonType=electron"]

    p = Process(target=submit, args=(config,))
    p.start()
    p.join()



