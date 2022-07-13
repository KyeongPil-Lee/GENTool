#include "TH1D.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

using namespace std;
using namespace edm;

class GenDileptonHistProducer : public edm::EDAnalyzer {
  public:
    explicit GenDileptonHistProducer(const edm::ParameterSet&);
    void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    edm::EDGetTokenT<GenEventInfoProduct>            t_genEventInfo_;
    edm::EDGetTokenT<std::vector<reco::GenParticle>> t_genParticles_;

    TH1D* h_gen_lep_pt_;
    TH1D* h_gen_lep_eta_;
    TH1D* h_gen_lep_phi_;

    TH1D* h_gen_lep_lead_pt_;
    TH1D* h_gen_lep_lead_eta_;
    TH1D* h_gen_lep_lead_phi_;

    TH1D* h_gen_lep_sub_pt_;
    TH1D* h_gen_lep_sub_eta_;
    TH1D* h_gen_lep_sub_phi_;

    TH1D* h_gen_diLep_mass_;
    TH1D* h_gen_diLep_pt_;
    TH1D* h_gen_diLep_rap_;

    int genID_lepton_;
    std::string genFlag_lepton_;
};

GenDileptonHistProducer::GenDileptonHistProducer(const edm::ParameterSet& iConfig):
t_genEventInfo_( consumes< GenEventInfoProduct >            (iConfig.getUntrackedParameter<edm::InputTag>("GenEventInfo")) ),
t_genParticles_( consumes< std::vector<reco::GenParticle> > (iConfig.getUntrackedParameter<edm::InputTag>("GenParticles")))
{
  edm::Service<TFileService> fs;
  h_gen_lep_pt_  = fs->make<TH1D>("h_gen_lep_pt_",  "", 10000, 0, 10000);
  h_gen_lep_eta_ = fs->make<TH1D>("h_gen_lep_eta_", "", 2000, -10, 10);
  h_gen_lep_phi_ = fs->make<TH1D>("h_gen_lep_phi_", "", 80, -4, 4);

  h_gen_lep_lead_pt_  = fs->make<TH1D>("h_gen_lep_lead_pt_",  "", 10000, 0, 10000);
  h_gen_lep_lead_eta_ = fs->make<TH1D>("h_gen_lep_lead_eta_", "", 2000, -10, 10);
  h_gen_lep_lead_phi_ = fs->make<TH1D>("h_gen_lep_lead_phi_", "", 80, -4, 4);

  h_gen_lep_sub_pt_  = fs->make<TH1D>("h_gen_lep_sub_pt_",  "", 10000, 0, 10000);
  h_gen_lep_sub_eta_ = fs->make<TH1D>("h_gen_lep_sub_eta_", "", 2000, -10, 10);
  h_gen_lep_sub_phi_ = fs->make<TH1D>("h_gen_lep_sub_phi_", "", 80, -4, 4);

  h_gen_diLep_mass_ = fs->make<TH1D>("h_gen_diLep_mass_", "", 10000, 0, 10000);
  h_gen_diLep_pt_   = fs->make<TH1D>("h_gen_diLep_pt_",   "", 10000, 0, 10000);
  h_gen_diLep_rap_  = fs->make<TH1D>("h_gen_diLep_rap_",  "", 2000, -10, 10);

  genID_lepton_   = iConfig.getUntrackedParameter<int>("genID_lepton");  
  genFlag_lepton_ = iConfig.getUntrackedParameter<std::string>("genFlag_lepton");
}

void GenDileptonHistProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iEventSetup) {

  // -- GEN weights
  edm::Handle < GenEventInfoProduct > h_genEventInfo;
  iEvent.getByToken(t_genEventInfo_, h_genEventInfo);

  double weight = h_genEventInfo->weight();

  // -- GEN particles
  edm::Handle < std::vector<reco::GenParticle> > h_genParticles;
  iEvent.getByToken(t_genParticles_, h_genParticles);

  vector<TLorentzVector> vec_vecP_lep;

  for(size_t i_gen=0; i_gen<h_genParticles->size(); ++i_gen) {
    reco::GenParticle genLepton = (*h_genParticles)[i_gen];

    if( abs(genLepton.pdgId()) != genID_lepton_ ) continue;

    bool genFlag = false;
    if( genFlag_lepton_ == "isHardProcess" )             genFlag = genLepton.isHardProcess();
    if( genFlag_lepton_ == "fromHardProcessFinalState" ) genFlag = genLepton.fromHardProcessFinalState();

    if( !genFlag ) continue;

    TLorentzVector vecP_lep;
    vecP_lep.SetPxPyPzE(genLepton.px(), genLepton.py(), genLepton.pz(), genLepton.energy());
    vec_vecP_lep.push_back(vecP_lep);
  }

  auto nLep = vec_vecP_lep.size();
  if( nLep != 2 ) {
    cout << "nLep = " << nLep << " is not equal to 2! ... do not fill the histograms" << endl;
    return;
  }

  TLorentzVector vecP_lead;
  TLorentzVector vecP_sub;
  TLorentzVector vecP_diLep;


  if( vec_vecP_lep[0].Pt() > vec_vecP_lep[1].Pt() ) {
    vecP_lead = vec_vecP_lep[0];
    vecP_sub  = vec_vecP_lep[1];
  }
  else {
    vecP_lead = vec_vecP_lep[1];
    vecP_sub  = vec_vecP_lep[0];
  }

  h_gen_lep_pt_->Fill(vecP_lead.Pt(), weight);
  h_gen_lep_eta_->Fill(vecP_lead.Eta(), weight);
  h_gen_lep_phi_->Fill(vecP_lead.Phi(), weight);
  h_gen_lep_pt_->Fill(vecP_sub.Pt(), weight);
  h_gen_lep_eta_->Fill(vecP_sub.Eta(), weight);
  h_gen_lep_phi_->Fill(vecP_sub.Phi(), weight);

  h_gen_lep_lead_pt_->Fill(vecP_lead.Pt(), weight);
  h_gen_lep_lead_eta_->Fill(vecP_lead.Eta(), weight);
  h_gen_lep_lead_phi_->Fill(vecP_lead.Phi(), weight);

  h_gen_lep_sub_pt_->Fill(vecP_sub.Pt(), weight);
  h_gen_lep_sub_eta_->Fill(vecP_sub.Eta(), weight);
  h_gen_lep_sub_phi_->Fill(vecP_sub.Phi(), weight);

  vecP_diLep = vecP_lead + vecP_sub;

  h_gen_diLep_mass_->Fill(vecP_diLep.M(), weight);
  h_gen_diLep_pt_->Fill(vecP_diLep.Pt(), weight);
  h_gen_diLep_rap_->Fill(vecP_diLep.Rapidity(), weight);
}

DEFINE_FWK_MODULE(GenDileptonHistProducer);