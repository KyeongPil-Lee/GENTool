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

config.Data.totalUnits = 30000
config.Data.splitting = 'EventBased' # -- PrivateMC: only EventBased splitting is allowed
config.Data.unitsPerJob = 300

config.Site.storageSite = 'T2_BE_IIHE'

# config.JobType.allowUndistributedCMSSW = True

config.JobType.psetName = 'ProduceGEN_from_gridpack_forCRAB.py' # -- should be filled

version = "v1"
config.General.workArea = 'CRABDir_%s' % version
config.Data.outLFNDirBase = '/store/user/kplee/DY_MiNNLO_GEN_%s' % version


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

    list_massRange = ["m10to50", "m100to200", "m200to400", "m400to500", "m500to700", "m700to800", "m800to1000", "m1000to1500", "m1500to2000", "m2000to3000", "m2000toInf"]
    # list_channelType = ["DYMM", "DYEE"]
    list_channelType = ["DYEE"]

    for massRange in list_massRange:
        for channelType in list_channelType:
            theSampleType = "%s_%s" % (channelType, massRange) # -- e.g. DYMM_m10to50
            theRequestName = "GEN_MiNNLO_Powheg_%s" % theSampleType # -- e.g. GEN_MiNNLO_Powheg_DYMM_m10to50
            theGridpackName = "gridpack_MiNNLO_Powheg_%s.tgz" % theSampleType # -- e.g. gridpack_MiNNLO_Powheg_DYMM_m10to50.tgz

            config.General.requestName = theRequestName
            config.JobType.inputFiles = ["./gridpack/%s" % theGridpackName] # -- gridpack
            config.JobType.pyCfgParams = ["sampleType=%s" % theSampleType]

            # crabCommand('submit', config = config)
            # -- recommended way from CRAB3 twiki when you change pyCfgParams: https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3FAQ#Multiple_submission_fails_with_a
            p = Process(target=submit, args=(config,))
            p.start()
            p.join()

