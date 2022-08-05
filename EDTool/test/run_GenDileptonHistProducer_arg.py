import os
def GetList_NtuplePath(baseNtupleDir):
    list_rootFile = []

    # list_file = os.listdir(baseNtupleDir)
    # for fileName in list_file:
    #     if ".root" in fileName:
    #         ntuplePath = "%s/%s" % (baseNtupleDir, fileName)
    #         list_rootFile.append( ntuplePath )

    # -- search for all subdirectories
    for (path, list_dirName, list_fileName) in os.walk(baseNtupleDir):
        # print "fileName = %s" % list_fileName
        for fileName in list_fileName:
            if ".root" in fileName:
                ntuplePath = "%s/%s" % (path, fileName)
                list_rootFile.append( ntuplePath )

    if len(list_rootFile) == 0:
        print "[GetList_NtuplePath] no root file under %s ... exit" % baseNtupleDir
        sys.exit()

    list_rootFile.sort()

    return list_rootFile

#########################################
import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.register('sampleType',
                  "none", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "Sample type (e.g. DYMM_m10to50)")

options.register('basePath',
                  "/pnfs/iihe/cms/store/user/kplee/DY_MiNNLO_GEN_v1/CRAB_PrivateMC", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "SE base directory with the crab jobs")

options.parseArguments()

print "basePath = %s" % (options.basePath)
print "sampleType = %s" % (options.sampleType)

crabDir = "crab_GEN_MiNNLO_Powheg_%s" % options.sampleType
dirPath = "%s/%s" % (options.basePath, crabDir)
list_ntuplePath = GetList_NtuplePath(dirPath) # -- get all list


process = cms.Process("GENTool")

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(),
    secondaryFileNames = cms.untracked.vstring(),
)

for ntuplePath in list_ntuplePath:
    if "inLHE" in ntuplePath:
        continue
    process.source.fileNames.append( "file:%s" % ntuplePath )

print "process.source.fileNames = ", process.source.fileNames

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '106X_mcRun2_asymptotic_v13'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

from GENTool.EDTool.GenDileptonHistProducer_cfi import *

process.genDileptonHistProducer = GenDileptonHistProducer.clone()
# process.genDileptonHistProducer.genID_lepton    = cms.untracked.int32(13)
# process.genDileptonHistProducer.genFlag_lepton  = cms.untracked.string("isHardProcess") # -- isHardProcess or fromHardProcessFinalState

process.mypath = cms.EndPath(process.genDileptonHistProducer)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("GENPlots_%s.root" % options.sampleType),
    closeFileFast = cms.untracked.bool(False),
)