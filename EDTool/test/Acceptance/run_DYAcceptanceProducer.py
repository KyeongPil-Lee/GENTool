import FWCore.ParameterSet.Config as cms

# -- m50 sample, mm
# exampleFile = '/store/mc/RunIISummer20UL18MiniAODv2/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/270000/C10AF425-1891-5849-A619-CE2D859DC4FF.root'
# nEvent = 1000

# -- m10to50 sample, ee
# exampleFile = '/store/mc/RunIISummer20UL18MiniAODv2/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/2560000/015AB5F3-7566-9B4C-BC3F-00C5181B27FB.root'
# nEvent = 10000
# exampleFile = '/store/mc/RunIISummer20UL18MiniAODv2/DYJetsToEE_M-10to50_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/2550000/7E3FB340-1701-F048-BDA1-82ED411CC300.root'
# nEvent = 10000

# -- m100-200 sample, UL18, mm
# exampleFile = '/store/mc/RunIISummer20UL18MiniAODv2/DYJetsToMuMu_M-100to200_H2ErratumFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/2560000/7BCE6EC9-BA6F-F540-A331-EEB3C4AF915F.root'
# nEvent = 10

# -- m50 sample, UL18, mm (more recent one, with tag "ZptWeighted")
# exampleFile = '/store/mc/RunIISummer20UL18MiniAODv2/DYJetsToMuMu_M-50_TuneCP5_ZptWeighted_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/50000/E8C8F28C-7A9D-3649-A3F7-33F98E47FFCD.root'
nEvent = 1000

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.register('globalTag',
                  "106X_upgrade2018_realistic_v16_L1v1", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "global tag")

options.register('channel',
                  "mm", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "channel (ee or mm)")

options.register('cutAtM100',
                  "false", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "Apply a cut at m=100 GeV? (for m-50 sample)")

options.register('InvOverflow',
                  "false", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "Perform an investigation on the overflow events")

options.register('exampleFile',
                  "", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "example file")

# options.register('leptonType',
#                   "none", # default value
#                   VarParsing.multiplicity.singleton, # singleton or list
#                   VarParsing.varType.string,         # string, int, or float
#                   "lepton type (electron or muon)")

options.parseArguments()

print("\n#######################")
print(" ---Input arguments ---")
print("global tag:  %s" % options.globalTag)
print("channel:     %s" % options.channel)
print("cutAtM100: %s" % options.cutAtM100)
print("InvOverflow: %s" % options.InvOverflow)
print("exampleFile: %s" % options.exampleFile)
print("#######################\n")

process = cms.Process("GENTool")

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(""),
    secondaryFileNames = cms.untracked.vstring(),
)

if options.exampleFile != "":
    process.source.fileNames = cms.untracked.vstring(options.exampleFile)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(nEvent))

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

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
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
cutAtM100_bool = False
if options.cutAtM100 == "true" or options.cutAtM100 == "True": 
    cutAtM100_bool = True

InvOverflow_bool = False
if options.InvOverflow == "true" or options.InvOverflow == "True": 
    InvOverflow_bool = True
process.DYAcceptanceProducer = cms.EDAnalyzer('DYAcceptanceProducer',
    LHERunInfoProduct = cms.untracked.InputTag("externalLHEProducer"),
    LHEEvent     = cms.untracked.InputTag("externalLHEProducer"),
    GenEventInfo = cms.untracked.InputTag("generator"),
    GenParticles = cms.untracked.InputTag("prunedGenParticles"), # -- or genParticles if tier >= AOD
    DressedLepton = cms.untracked.InputTag("particleLevel:leptons"),
    Channel = cms.untracked.string(options.channel),
    PtCut_lead  = cms.untracked.double(20.0),
    PtCut_sub   = cms.untracked.double(15.0),
    EtaCut_lead = cms.untracked.double(2.4),
    EtaCut_sub  = cms.untracked.double(2.4),
    Cut_At_M100 = cms.untracked.bool(cutAtM100_bool), # -- for m-50 sample (drop events above m=100 GeV to combine with high mass samples)
    Investigate_Overflow = cms.untracked.bool(InvOverflow_bool),
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
