from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''

config.JobType.pluginName = 'PrivateMC'
config.JobType.numCores = 1
# config.JobType.maxMemoryMB = 2500
# config.JobType.maxJobRuntimeMin = 2000

config.Data.inputDataset = ''

# config.Data.inputDBS = 'global'
config.Data.publication = False

config.Data.totalUnits = 10000
config.Data.splitting = 'EventBased' # -- PrivateMC: only EventBased splitting is allowed
config.Data.unitsPerJob = 100

config.Site.storageSite = 'T2_BE_IIHE'

# config.JobType.allowUndistributedCMSSW = True

config.JobType.psetName = 'ProduceGEN_new_runcmsgrid.py' # -- should be filled

version = "v1"
config.General.workArea = 'CRABDir_%s' % version
config.Data.outLFNDirBase = '/store/user/kplee/DY_MiNNLO_GEN_%s' % version


# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    config.General.requestName = 'DYMuMu_M50_MiNNLO_GEN'
    # config.JobType.inputFiles = ["gridpack_another_runcmsgrid.tgz"] # -- gridpack
    config.JobType.inputFiles = ["/afs/cern.ch/work/k/kplee/public/gridpack_MiNNLO/Test_M50/gridpack_another_runcmsgrid.tgz"] # -- gridpack

    crabCommand('submit', config = config)

