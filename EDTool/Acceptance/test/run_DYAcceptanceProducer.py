import FWCore.ParameterSet.Config as cms

exampleFile = '/store/mc/RunIISummer20UL18MiniAODv2/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/270000/C10AF425-1891-5849-A619-CE2D859DC4FF.root'

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.register('globalTag',
                  "106X_upgrade2018_realistic_v16_L1v1", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "global tag")

# options.register('leptonType',
#                   "none", # default value
#                   VarParsing.multiplicity.singleton, # singleton or list
#                   VarParsing.varType.string,         # string, int, or float
#                   "lepton type (electron or muon)")

options.parseArguments()

process = cms.Process("GENTool")

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(exampleFile),
    secondaryFileNames = cms.untracked.vstring(),
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = options.globalTag

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

# -- Follow the procedure used in nanoAOD
# ---- 1) Merge prunedGenParticles & packedGenParticles
# ---- 2) Convert to HepMC format
# ---- 3) Produce dressed leptons with Rivet convention
# -- https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_27/PhysicsTools/NanoAOD/python/particlelevel_cff.py
process.mergedGenParticles = cms.EDProducer("MergedGenParticleProducer",
    inputPruned = cms.InputTag("prunedGenParticles"),
    inputPacked = cms.InputTag("packedGenParticles"),
)

process.genParticles2HepMC = cms.EDProducer("GenParticles2HepMCConverter",
    genParticles = cms.InputTag("mergedGenParticles"),
    genEventInfo = cms.InputTag("generator"),
    signalParticlePdgIds = cms.vint32(),
)

process.particleLevel = cms.EDProducer("ParticleLevelProducer",
    src = cms.InputTag("genParticles2HepMC:unsmeared"),
    
    usePromptFinalStates = cms.bool(True), # for leptons, photons, neutrinos
    excludePromptLeptonsFromJetClustering = cms.bool(False),
    excludeNeutrinosFromJetClustering = cms.bool(True),
    
    particleMinPt  = cms.double(0.),
    particleMaxEta = cms.double(999.0), # HF range. Maximum 6.0 on MiniAOD; no cut on particles
    
    lepConeSize = cms.double(0.1), # for photon dressing
    lepMinPt    = cms.double(0.0), # no cut on the leptons!
    lepMaxEta   = cms.double(999.0), # no cut on the leptons!
    
    jetConeSize = cms.double(0.4),
    jetMinPt    = cms.double(10.),
    jetMaxEta   = cms.double(999.),
    
    fatJetConeSize = cms.double(0.8),
    fatJetMinPt    = cms.double(170.),
    fatJetMaxEta   = cms.double(999.),

    phoIsoConeSize = cms.double(0.4),
    phoMaxRelIso = cms.double(0.5),
    phoMinPt = cms.double(10),
    phoMaxEta = cms.double(2.5),
)

# -- plot producer
process.DYAcceptanceProducer = cms.EDAnalyzer('DYAcceptanceProducer',
    LHERunInfoProduct = cms.untracked.InputTag("externalLHEProducer"),
    LHEEvent     = cms.untracked.InputTag("externalLHEProducer"),
    GenEventInfo = cms.untracked.InputTag("generator"),
    GenParticles = cms.untracked.InputTag("genParticles"),
    DressedLepton = cms.untracked.InputTag("particleLevel:leptons"),
)

process.mypath = cms.EndPath(
    process.mergedGenParticles +
    process.genParticles2HepMC + 
    process.particleLevel +
    process.DYAcceptanceProducer )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("DYAccPlots.root"),
    closeFileFast = cms.untracked.bool(False),
)

# from GENTool.EDTool.GenDileptonHistProducer_cfi import *

# process.genDileptonHistProducer = GenDileptonHistProducer.clone()
# process.genDileptonHistProducer.GenParticles = cms.untracked.InputTag("prunedGenParticles") # -- miniAOD

# if options.leptonType == "muon":
#     process.genDileptonHistProducer.genID_lepton = cms.untracked.int32(13)
# elif options.leptonType == "electron":
#     process.genDileptonHistProducer.genID_lepton = cms.untracked.int32(11)

# process.genDileptonHistProducer.genFlag_lepton  = cms.untracked.string("isHardProcess") # -- isHardProcess or fromHardProcessFinalState

# process.mypath = cms.EndPath(process.genDileptonHistProducer)
