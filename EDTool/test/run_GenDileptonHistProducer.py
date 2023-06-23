import FWCore.ParameterSet.Config as cms

process = cms.Process("GENTool")

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(''),
    secondaryFileNames = cms.untracked.vstring(),
)

# basePath = "/user/kplee/SE/DY_MiNNLO_GEN_v1/CRAB_PrivateMC/crab_DYMuMu_M50_MiNNLO_GEN/220715_155814/0000"
# process.source.fileNames = cms.untracked.vstring()
# # for i in range(1, 101):
# for i in range(1, 2):
#     filePath = "file:%s/SMP-RunIISummer20UL16wmLHEGEN-00496_%d.root" % (basePath, i);
#     process.source.fileNames.append( filePath )

# process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source.fileNames = cms.untracked.vstring(
    '/store/mc/RunIISummer20UL18MiniAODv2/GGToLL_TuneCP5_13TeV-pythia8/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/2560000/07DC50E2-5A3B-DA42-A5F4-990F33DE1DD1.root')
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
# process.GlobalTag.globaltag = '106X_mcRun2_asymptotic_v13'
# process.GlobalTag.globaltag = '106X_mcRun2_asymptotic_preVFP_v11'
process.GlobalTag.globaltag = '106X_upgrade2018_realistic_v16_L1v1'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

from GENTool.EDTool.GenDileptonHistProducer_cfi import *

process.genDileptonHistProducer = GenDileptonHistProducer.clone()
process.genDileptonHistProducer.GenParticles = cms.untracked.InputTag("prunedGenParticles") # -- miniAOD

process.mypath = cms.EndPath(process.genDileptonHistProducer)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("GENPlots.root"),
    closeFileFast = cms.untracked.bool(False),
)