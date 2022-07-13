import FWCore.ParameterSet.Config as cms

GenDileptonHistProducer = cms.EDAnalyzer('GenDileptonHistProducer',
    GenEventInfo = cms.untracked.InputTag("generator"),
    GenParticles = cms.untracked.InputTag("genParticles"),

    genID_lepton    = cms.untracked.int(13),
    # -- isHardProcess or fromHardProcessFinalState
    genFlag_lepton  = cms.untracked.string("isHardProcess"),
)