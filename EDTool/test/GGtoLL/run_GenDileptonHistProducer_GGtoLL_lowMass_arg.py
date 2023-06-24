import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.register('ptMin',
                  "1p0", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "pTHatMinDiverge value")

options.parseArguments()

inputFile = "input/input_pTHatMinDiverge_%s.txt" % options.ptMin
outputFile = "GENPlots_ptMin_%s.root" % options.ptMin
print("ptMin = %s" % options.ptMin)
print("--> inputFile  = %s" % inputFile)
print("--> outputFile = %s" % outputFile)

process = cms.Process("GENTool")

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(''),
    secondaryFileNames = cms.untracked.vstring(),
)

process.source.fileNames = cms.untracked.vstring()
f_input = open(inputFile)
for line in f_input.readlines():
    line = line.split('\n')[0]
    line = "file:" + line
    process.source.fileNames.append(line)

# print("input file list:")
# print process.source.fileNames

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
# process.GlobalTag.globaltag = '106X_mcRun2_asymptotic_v13'
# process.GlobalTag.globaltag = '106X_mcRun2_asymptotic_preVFP_v11'
process.GlobalTag.globaltag = '106X_upgrade2018_realistic_v16_L1v1'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

from GENTool.EDTool.GenDileptonHistProducer_cfi import *

process.genDileptonHistProducer = GenDileptonHistProducer.clone()
process.genDileptonHistProducer.genID_lepton    = cms.untracked.int32(13)
# process.genDileptonHistProducer.genFlag_lepton  = cms.untracked.string("isHardProcess") # -- isHardProcess or fromHardProcessFinalState
# process.genDileptonHistProducer.GenParticles = cms.untracked.InputTag("prunedGenParticles") # -- miniAOD
# process.genDileptonHistProducer.genFlag_lepton = cms.untracked.string("")

process.mypath = cms.EndPath(process.genDileptonHistProducer)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outputFile),
    closeFileFast = cms.untracked.bool(False),
)