import FWCore.ParameterSet.Config as cms

process = cms.Process("GENTool")

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/k/kplee/work/private/Analysis/gridpack_MiNNLO/CMSSW_10_6_18/src/SMP-RunIISummer20UL16wmLHEGENAPV-00001.root'),
    secondaryFileNames = cms.untracked.vstring(),
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '106X_mcRun2_asymptotic_preVFP_v8'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

from GENTool.EDTool.GenDileptonHistProducer_cfi import *

process.genDileptonHistProducer = GenDileptonHistProducer.clone()
# process.genDileptonHistProducer.genID_lepton    = cms.untracked.int32(13)
# process.genDileptonHistProducer.genFlag_lepton  = cms.untracked.string("isHardProcess") # -- isHardProcess or fromHardProcessFinalState

process.mypath = cms.EndPath(process.genDileptonHistProducer)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("GENPlots.root"),
    closeFileFast = cms.untracked.bool(False),
)