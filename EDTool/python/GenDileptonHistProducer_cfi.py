import FWCore.ParameterSet.Config as cms

GenDileptonHistProducer = cms.EDAnalyzer('GenDileptonHistProducer',
    LHEEvent     = cms.untracked.InputTag("externalLHEProducer"),
    GenEventInfo = cms.untracked.InputTag("generator"),
    GenParticles = cms.untracked.InputTag("genParticles"),

    # genID_lepton   = cms.untracked.int32(13),
    # genFlag_lepton = cms.untracked.string("isHardProcess"), # -- isHardProcess or fromHardProcessFinalState
)