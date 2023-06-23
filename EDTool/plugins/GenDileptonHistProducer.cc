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

/*

// -- HistSet: easily control many histograms
HistSet *histSet = new HistSet("type"); // -- or put ""
histSet->Register("pt",  10000, 0,   10000); // -- it will make "h_pt" histogram with given bin info
histSet->Register("eta", 200,   -10, 10); // -- it will make "h_eta" histogram with given bin info
...
// -- end of register -- //

// -- fill part
for(auto& mu : vec_muon ) {
  ...
  histSet-->Fill( "pt", mu.pt, eventWeight );
  histSet-->Fill( "eta", mu.pt, eventWeight );
  ...
}

// -- save part
histSet->Write();

*/

class HistSet { 
public:
  HistSet(edm::Service<TFileService>& fs, TString type): 
  fs_(fs) { 
    type_ = type; 
  }

  void Register(TString var, Int_t nBin, Double_t min, Double_t max) {

    TString histName = "h_"+var+"_"+type_;
    if( type_ == "" )
      histName = "h_"+var;

    // TH1D* h_temp = new TH1D(histName, "", nBin, min, max);
    TH1D* h_temp = fs_->make<TH1D>(histName, "", nBin, min, max);

    map_var_hist_.insert( std::make_pair(var, h_temp) );
  }

  void Fill(TString var, Double_t value, Double_t weight = 1.0 ) {

    auto it_pair = map_var_hist_.find(var);
    if( it_pair == map_var_hist_.end() )
      std::cout << "no histogram corresponding to " << var << std::endl;
    else
      it_pair->second->Fill( value, weight );
  }

  TH1D* Get(TString var) {
    auto it_pair = map_var_hist_.find(var);
    if( it_pair == map_var_hist_.end() )
      std::cout << "no histogram corresponding to " << var << std::endl;
    else
      return it_pair->second;

    return nullptr;
  }

  void Write() {
    for(auto pair : map_var_hist_ )
      pair.second->Write();
  }

private:
  edm::Service<TFileService>& fs_;
  TString type_;
  map<TString, TH1D*> map_var_hist_;
};

class HistContainer {
public:
  HistContainer(edm::Service<TFileService>& fs, TString type) {
    type_ = type;
    Init(fs);
  }

  void Set_Acc(double ptCut_lead, double etaCut_lead, double ptCut_sub, double etaCut_sub) {
    setAcc_ = true;
    ptCut_lead_ = ptCut_lead;
    etaCut_lead_ = etaCut_lead;
    ptCut_sub_ = ptCut_sub;
    etaCut_sub_ = etaCut_sub;
  }

  void Fill( TLorentzVector vecP_lep1, TLorentzVector vecP_lep2, double weight) {

    // fill weight regardless of passing or failing acceptance
    histSet_->Get("weight")->Fill("sumWeight", weight);
    if( weight > 0 ) histSet_->Get("weight")->Fill("sumWeight_pos", weight);
    if( weight < 0 ) histSet_->Get("weight")->Fill("sumWeight_neg", weight);

    TLorentzVector vecP_lead, vecP_sub;
    if( vecP_lep1.Pt() > vecP_lep2.Pt() ) {
      vecP_lead = vecP_lep1;
      vecP_sub  = vecP_lep2;
    }
    else {
      vecP_lead = vecP_lep2;
      vecP_sub  = vecP_lep1;
    }

    // -- check acceptance
    if( setAcc_ && !PassAcceptance(vecP_lead, vecP_sub) ) return;

    Fill_Lepton("lead", vecP_lead, weight);
    Fill_Lepton("sub", vecP_sub, weight);

    TLorentzVector vecP_diLep = vecP_lep1 + vecP_lep2;

    histSet_->Fill( "diLep_mass", vecP_diLep.M(), weight);
    histSet_->Fill( "diLep_pt", vecP_diLep.Pt(), weight);
    histSet_->Fill( "diLep_rap", vecP_diLep.Rapidity(), weight);
  }

private:
  TString type_ = "";
  HistSet* histSet_;

  bool setAcc_ = false;
  double ptCut_lead_ = 0;
  double etaCut_lead_ = 0;
  double ptCut_sub_ = 0;
  double etaCut_sub_ = 0;

  void Init(edm::Service<TFileService>& fs) {
    histSet_ = new HistSet(fs, type_);

    histSet_->Register("weight", 3, 0, 3);
    histSet_->Get("weight")->GetXaxis()->SetBinLabel(1, "sumWeight");
    histSet_->Get("weight")->GetXaxis()->SetBinLabel(2, "sumWeight_pos");
    histSet_->Get("weight")->GetXaxis()->SetBinLabel(3, "sumWeight_neg");

    histSet_->Register("lep_pt",  10000, 0, 10000);
    histSet_->Register("lep_eta", 2000, -10, 10);
    histSet_->Register("lep_phi", 80, -4, 4);

    histSet_->Register("lep_lead_pt",  10000, 0, 10000);
    histSet_->Register("lep_lead_eta", 2000, -10, 10);
    histSet_->Register("lep_lead_phi", 80, -4, 4);

    histSet_->Register("lep_sub_pt",  10000, 0, 10000);
    histSet_->Register("lep_sub_eta", 2000, -10, 10);
    histSet_->Register("lep_sub_phi", 80, -4, 4);

    histSet_->Register("diLep_mass", 10000, 0, 10000);
    histSet_->Register("diLep_pt",   10000, 0, 10000);
    histSet_->Register("diLep_rap",  2000, -10, 10);
  }

  void Fill_Lepton(TString lepType, TLorentzVector vecP, double weight) {
    histSet_->Fill( "lep_pt", vecP.Pt(), weight);
    histSet_->Fill( "lep_eta", vecP.Eta(), weight);
    histSet_->Fill( "lep_phi", vecP.Phi(), weight);

    histSet_->Fill( "lep_"+lepType+"_pt", vecP.Pt(), weight);
    histSet_->Fill( "lep_"+lepType+"_eta", vecP.Eta(), weight);
    histSet_->Fill( "lep_"+lepType+"_phi", vecP.Phi(), weight);
  }

  bool PassAcceptance(TLorentzVector vecP_lead, TLorentzVector vecP_sub) {
    // if acceptance condition is not set: should include all events
    if( !setAcc_ ) return true;

    bool flag = false;

    if( vecP_lead.Pt() > ptCut_lead_ && std::abs(vecP_lead.Eta()) < etaCut_lead_ &&
        vecP_sub.Pt()  > ptCut_sub_  && std::abs(vecP_sub.Eta())  < etaCut_sub_ )
        flag = true;

    return flag;
  }
};

class GenDileptonHistProducer : public edm::EDAnalyzer {
  public:
    explicit GenDileptonHistProducer(const edm::ParameterSet&);
    void analyze(const edm::Event&, const edm::EventSetup&);
  private:
    edm::EDGetTokenT<LHEEventProduct>                t_LHEEvent_;
    edm::EDGetTokenT<GenEventInfoProduct>            t_genEventInfo_;
    edm::EDGetTokenT<std::vector<reco::GenParticle>> t_genParticles_;

    HistContainer* hist_HP_mm_; // -- isHardProcess
    HistContainer* hist_HPFS_mm_; // -- fromHardProcessFinalState
    HistContainer* hist_LHE_mm_; // -- LHE

    HistContainer* hist_HP_ee_; // -- isHardProcess
    HistContainer* hist_HPFS_ee_; // -- fromHardProcessFinalState
    HistContainer* hist_LHE_ee_; // -- LHE

    // -- within acceptance
    HistContainer* hist_HPFS_mm_acc_;
    HistContainer* hist_HPFS_ee_acc_;

    // -- within acceptance, another scenario
    HistContainer* hist_HPFS_mm_acc_v2_;
    HistContainer* hist_HPFS_ee_acc_v2_;

    // int genID_lepton_;
    // std::string genFlag_lepton_;

    void Fill_GENLepton(std::vector<reco::GenParticle> vec_GenParticle, HistContainer* hist, double weight, std::string genFlag, int pdgID);
    void Fill_LHELepton(const lhef::HEPEUP& lheEvent, HistContainer* hist, double weight, int pdgID);
};

GenDileptonHistProducer::GenDileptonHistProducer(const edm::ParameterSet& iConfig):
t_LHEEvent_(   mayConsume< LHEEventProduct >                (iConfig.getUntrackedParameter<edm::InputTag>("LHEEvent")) ),
t_genEventInfo_( consumes< GenEventInfoProduct >            (iConfig.getUntrackedParameter<edm::InputTag>("GenEventInfo")) ),
t_genParticles_( consumes< std::vector<reco::GenParticle> > (iConfig.getUntrackedParameter<edm::InputTag>("GenParticles")))
{
  edm::Service<TFileService> fs;

  hist_HP_mm_ = new HistContainer(fs, "HP_mm");
  hist_HPFS_mm_ = new HistContainer(fs, "HPFS_mm");
  hist_LHE_mm_ = new HistContainer(fs, "LHE_mm");

  hist_HP_ee_ = new HistContainer(fs, "HP_ee");
  hist_HPFS_ee_ = new HistContainer(fs, "HPFS_ee");
  hist_LHE_ee_ = new HistContainer(fs, "LHE_ee");

  hist_HPFS_mm_acc_ = new HistContainer(fs, "HPFS_mm_acc");
  hist_HPFS_ee_acc_ = new HistContainer(fs, "HPFS_ee_acc");
  hist_HPFS_mm_acc_->Set_Acc(3.0, 2.4, 3.0, 2.4); // UPDATE: set via python arguments
  hist_HPFS_ee_acc_->Set_Acc(3.0, 2.4, 3.0, 2.4);

  hist_HPFS_mm_acc_v2_ = new HistContainer(fs, "HPFS_mm_acc_v2");
  hist_HPFS_ee_acc_v2_ = new HistContainer(fs, "HPFS_ee_acc_v2");
  hist_HPFS_mm_acc_v2_->Set_Acc(20.0, 2.4, 15.0, 2.4);
  hist_HPFS_ee_acc_v2_->Set_Acc(28.0, 2.4, 20.0, 2.4);

  // genID_lepton_   = iConfig.getUntrackedParameter<int>("genID_lepton");  
  // genFlag_lepton_ = iConfig.getUntrackedParameter<std::string>("genFlag_lepton");
}

void GenDileptonHistProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iEventSetup) {

  // -- GEN weights
  edm::Handle < GenEventInfoProduct > h_genEventInfo;
  iEvent.getByToken(t_genEventInfo_, h_genEventInfo);

  double weight = h_genEventInfo->weight();

  // -- GEN particles
  edm::Handle < std::vector<reco::GenParticle> > h_genParticles;
  iEvent.getByToken(t_genParticles_, h_genParticles);

  Fill_GENLepton(*h_genParticles, hist_HP_ee_, weight, "isHardProcess", 11);
  Fill_GENLepton(*h_genParticles, hist_HP_mm_, weight, "isHardProcess", 13);

  Fill_GENLepton(*h_genParticles, hist_HPFS_ee_, weight, "fromHardProcessFinalState", 11);
  Fill_GENLepton(*h_genParticles, hist_HPFS_mm_, weight, "fromHardProcessFinalState", 13);

  Fill_GENLepton(*h_genParticles, hist_HPFS_ee_acc_, weight, "fromHardProcessFinalState", 11);
  Fill_GENLepton(*h_genParticles, hist_HPFS_mm_acc_, weight, "fromHardProcessFinalState", 13);

  Fill_GENLepton(*h_genParticles, hist_HPFS_ee_acc_v2_, weight, "fromHardProcessFinalState", 11);
  Fill_GENLepton(*h_genParticles, hist_HPFS_mm_acc_v2_, weight, "fromHardProcessFinalState", 13);

  // -- LHE events
  edm::Handle < LHEEventProduct > h_LHEEvent;
  if( iEvent.getByToken(t_LHEEvent_, h_LHEEvent) ) { // -- run only when LHE information is available (some samples don't have LHE)
    double weight_LHE = h_LHEEvent->originalXWGTUP();

    const lhef::HEPEUP& lheEvent = h_LHEEvent->hepeup();
    Fill_LHELepton(lheEvent, hist_LHE_ee_, weight_LHE, 11);
    Fill_LHELepton(lheEvent, hist_LHE_mm_, weight_LHE, 13);
  } // -- end of isLHE

}

void GenDileptonHistProducer::Fill_GENLepton(std::vector<reco::GenParticle> vec_GenParticle, HistContainer* hist, double weight, std::string genFlag, int pdgID) {
  vector<TLorentzVector> vec_vecP_lep;

  for(size_t i_gen=0; i_gen<vec_GenParticle.size(); ++i_gen) {
    reco::GenParticle genLepton = vec_GenParticle[i_gen];

    if( abs(genLepton.pdgId()) != pdgID ) continue;

    bool passGenFlag = false;
    if( genFlag == "isHardProcess" )             passGenFlag = genLepton.isHardProcess();
    if( genFlag == "fromHardProcessFinalState" ) passGenFlag = genLepton.fromHardProcessFinalState();
    if( genFlag == "" )                          passGenFlag = true; // take any particles

    if( !passGenFlag ) continue;

    TLorentzVector vecP_lep;
    vecP_lep.SetPxPyPzE(genLepton.px(), genLepton.py(), genLepton.pz(), genLepton.energy());
    vec_vecP_lep.push_back(vecP_lep);
  }

  auto nLep = vec_vecP_lep.size();
  // nLep != 0: can happen if the sample is 3-flavor sample
  if( nLep != 0 && nLep != 2 ) {
    cout << "nLep = " << nLep << " is not equal to 0 or 2! ... do not fill the histograms" << endl;
    return;
  }
  if( nLep == 0 ) return;

  hist->Fill(vec_vecP_lep[0], vec_vecP_lep[1], weight);
}

void GenDileptonHistProducer::Fill_LHELepton(const lhef::HEPEUP& lheEvent, HistContainer* hist, double weight, int pdgID) {
  vector<TLorentzVector> vec_vecP_lep;

  std::vector<lhef::HEPEUP::FiveVector> lheParticles = lheEvent.PUP;
  for( size_t i_par = 0; i_par < lheParticles.size(); ++i_par ) {
    Int_t id = lheEvent.IDUP[i_par];

    if( abs(id) != pdgID ) continue;

    Double_t px     = lheParticles[i_par][0];
    Double_t py     = lheParticles[i_par][1];
    Double_t pz     = lheParticles[i_par][2];
    Double_t energy = lheParticles[i_par][3];
    // Double_t M = lheParticles[i_par][4];   
    // Int_t status = lheEvent.ISTUP[i_par];

    TLorentzVector vecP_lep;
    vecP_lep.SetPxPyPzE(px, py, pz, energy);

    vec_vecP_lep.push_back( vecP_lep );
  }

  auto nLep = vec_vecP_lep.size();
  // nLep != 0: can happen if the sample is 3-flavor sample
  if( nLep != 0 && nLep != 2 ) {
    cout << "nLep = " << nLep << " is not equal to 0 or 2! ... do not fill the histograms" << endl;
    return;
  }
  if( nLep == 0 ) return;

  hist->Fill(vec_vecP_lep[0], vec_vecP_lep[1], weight);
}

DEFINE_FWK_MODULE(GenDileptonHistProducer);