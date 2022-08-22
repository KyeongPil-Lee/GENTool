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
    edm::EDGetTokenT<LHEEventProduct>                t_LHEEvent_;
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

    TH1D* h_LHE_weight_;
    TH1D* h_LHE_weight_signOnly_;
    TH1D* h_LHE_diLep_mass_;
    TH1D* h_LHE_diLep_mass_signOnly_;

    int genID_lepton_;
    std::string genFlag_lepton_;
};

GenDileptonHistProducer::GenDileptonHistProducer(const edm::ParameterSet& iConfig):
t_LHEEvent_(     consumes< LHEEventProduct >                (iConfig.getUntrackedParameter<edm::InputTag>("LHEEvent")) ),
t_genEventInfo_( consumes< GenEventInfoProduct >            (iConfig.getUntrackedParameter<edm::InputTag>("GenEventInfo")) ),
t_genParticles_( consumes< std::vector<reco::GenParticle> > (iConfig.getUntrackedParameter<edm::InputTag>("GenParticles")))
{
  edm::Service<TFileService> fs;
  h_gen_lep_pt_  = fs->make<TH1D>("h_gen_lep_pt",  "", 10000, 0, 10000);
  h_gen_lep_eta_ = fs->make<TH1D>("h_gen_lep_eta", "", 2000, -10, 10);
  h_gen_lep_phi_ = fs->make<TH1D>("h_gen_lep_phi", "", 80, -4, 4);

  h_gen_lep_lead_pt_  = fs->make<TH1D>("h_gen_lep_lead_pt",  "", 10000, 0, 10000);
  h_gen_lep_lead_eta_ = fs->make<TH1D>("h_gen_lep_lead_eta", "", 2000, -10, 10);
  h_gen_lep_lead_phi_ = fs->make<TH1D>("h_gen_lep_lead_phi", "", 80, -4, 4);

  h_gen_lep_sub_pt_  = fs->make<TH1D>("h_gen_lep_sub_pt",  "", 10000, 0, 10000);
  h_gen_lep_sub_eta_ = fs->make<TH1D>("h_gen_lep_sub_eta", "", 2000, -10, 10);
  h_gen_lep_sub_phi_ = fs->make<TH1D>("h_gen_lep_sub_phi", "", 80, -4, 4);

  h_gen_diLep_mass_ = fs->make<TH1D>("h_gen_diLep_mass", "", 10000, 0, 10000);
  h_gen_diLep_pt_   = fs->make<TH1D>("h_gen_diLep_pt",   "", 10000, 0, 10000);
  h_gen_diLep_rap_  = fs->make<TH1D>("h_gen_diLep_rap",  "", 2000, -10, 10);

  h_LHE_weight_ = fs->make<TH1D>("h_LHE_weight",  "", 1, 0, 1);
  h_LHE_weight_->GetXaxis()->SetBinLabel(1, "sumWeight");

  h_LHE_weight_signOnly_ = fs->make<TH1D>("h_LHE_weight_signOnly",  "", 1, 0, 1);
  h_LHE_weight_signOnly_->GetXaxis()->SetBinLabel(1, "sumWeight");

  h_LHE_diLep_mass_          = fs->make<TH1D>("h_LHE_diLep_mass", "", 10000, 0, 10000);
  h_LHE_diLep_mass_signOnly_ = fs->make<TH1D>("h_LHE_diLep_mass_signOnly", "", 10000, 0, 10000);

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

  // -- LHE events
  edm::Handle < LHEEventProduct > h_LHEEvent;
  iEvent.getByToken(t_LHEEvent_, h_LHEEvent);

  const lhef::HEPEUP& lheEvent = h_LHEEvent->hepeup();
  std::vector<lhef::HEPEUP::FiveVector> lheParticles = lheEvent.PUP;

  vector<TLorentzVector> vec_vecP_LHELep;
  for( size_t i_par = 0; i_par < lheParticles.size(); ++i_par ) {
    Int_t id = lheEvent.IDUP[i_par];

    if( abs(id) != genID_lepton_ ) continue;

    Double_t px     = lheParticles[i_par][0];
    Double_t py     = lheParticles[i_par][1];
    Double_t pz     = lheParticles[i_par][2];
    Double_t energy = lheParticles[i_par][3];
    // Double_t M = lheParticles[i_par][4];   
    // Int_t status = lheEvent.ISTUP[i_par];

    TLorentzVector vecP_LHELep;
    vecP_LHELep.SetPxPyPzE(px, py, pz, energy);

    vec_vecP_LHELep.push_back( vecP_LHELep );
  }

  // cout << "# LHE lepton = " << vec_vecP_LHELep.size() << endl;

  auto nLHELep = vec_vecP_LHELep.size();
  if( nLHELep != 2 ) {
    cout << "nLHELep = " << nLHELep << " is not equal to 2! ... do not fill the histograms" << endl;
    return;
  }

  TLorentzVector vecP_dilep_LHE = vec_vecP_LHELep[0] + vec_vecP_LHELep[1];

  double weight_LHE = h_LHEEvent->originalXWGTUP();
  h_LHE_weight_->Fill("sumWeight", weight_LHE);
  h_LHE_diLep_mass_->Fill( vecP_dilep_LHE.M(), weight_LHE );

  // -- only take the sign of the weight: to avoid a few events with too large weights which ruins the whole distribution
  double weight_LHE_sign = 0;
  if( weight_LHE != 0 )
    weight_LHE_sign = weight_LHE > 0 ? weight_LHE = 1.0 : weight_LHE = -1.0;

  h_LHE_weight_signOnly_->Fill( "sumWeight", weight_LHE_sign );
  h_LHE_diLep_mass_signOnly_->Fill( vecP_dilep_LHE.M(), weight_LHE_sign );
}

DEFINE_FWK_MODULE(GenDileptonHistProducer);