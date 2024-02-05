#include "algorithm"

#include "TString.h"
#include "TH1D.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
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

  // -- when you fill the histogram with a tag
  // -- if no histogram was registered with the tag, it will automatically add the new histogram with the given tag
  void Fill(TString baseVar, TString tag, Double_t value, Double_t weight = 1.0 ) {

    TString var_wTag = TString::Format("%s_%s", baseVar.Data(), tag.Data());

    auto it_pair = map_var_hist_.find(var_wTag);
    if( it_pair == map_var_hist_.end() ) {
      // -- if no histogram is available with the tag, make it:
      cout << "no "+baseVar+" histogram is registered for the tag = " + tag + " ... make it" << endl;
      TH1D* h_baseVar = Get(baseVar);

      TH1D* h_var_wTag = fs_->make<TH1D>(*h_baseVar); // -- copy the format (binning)

      TString histName = h_baseVar->GetName();
      histName.ReplaceAll(baseVar, var_wTag);
      h_var_wTag->SetName(histName);

      h_var_wTag->Fill( value, weight );

      map_var_hist_.insert( std::make_pair(var_wTag, h_var_wTag) );
    }
    else
      it_pair->second->Fill( value, weight );
  }

  TH1D* Get(TString var) {
    auto it_pair = map_var_hist_.find(var);
    if( it_pair == map_var_hist_.end() )
      std::cout << "[HistSet::Get] no histogram corresponding to " << var << std::endl;
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
  HistContainer(edm::Service<TFileService>& fs, TString type): type_(type) {
    Init(fs);
  }

  void Set_Acc(double ptCut_lead, double etaCut_lead, double ptCut_sub, double etaCut_sub) {
    setAcc_ = true;
    ptCut_lead_ = ptCut_lead;
    etaCut_lead_ = etaCut_lead;
    ptCut_sub_ = ptCut_sub;
    etaCut_sub_ = etaCut_sub;
  }

  void Fill(const TLorentzVector& vecP_lep1, 
            const TLorentzVector& vecP_lep2, 
            const double& weight, 
            const std::map<TString, double>& map_systWRatio) {

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

    for(const auto& pair : map_systWRatio ) {
      double weight_systVar = weight*pair.second;
      histSet_->Fill( "diLep_mass", pair.first, vecP_diLep.M(), weight_systVar);
    }
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

class DYAcceptanceProducer : public edm::EDAnalyzer {
  public:
    explicit DYAcceptanceProducer(const edm::ParameterSet&);
    void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
    void endRun(const Run& iRun, const EventSetup& iSetup);

  private:
    edm::EDGetTokenT<LHERunInfoProduct>              t_LHERunInfo_;    
    edm::EDGetTokenT<LHEEventProduct>                t_LHEEvent_;
    edm::EDGetTokenT<GenEventInfoProduct>            t_genEventInfo_;
    edm::EDGetTokenT<std::vector<reco::GenParticle>> t_genParticles_;
    edm::EDGetTokenT<reco::GenJetCollection>         t_dressedLepton_;
    
    std::string channel_;
    int thePDGID_ = 0;

    bool doCut_at_m100_ = false;
    bool doCheckOverflow_ = false;

    bool adjustPDFWeight_ = false;
    double PDFWeightRangeSigma_ = 5.0; // -- allow the weight within 5sigma by default
    vector<double> vec_PDFWeight_mean_;
    vector<double> vec_PDFWeight_sigma_;
    vector<double> vec_PDFWeight_lowerLimit_;
    vector<double> vec_PDFWeight_upperLimit_;

    void Save_WeightInfo(std::map<TString, double>& map_systWRatio, const int& id_weight, double& ratio);
    void Fill_WeightInfo_Range(std::map<TString, double>& map_systWRatio, 
                               const int& id_weight, double& ratio,
                               TString baseTag, int id_start, int id_end, int id_cv = -1);
    void Fill_WeightInfo_Single(std::map<TString, double>& map_systWRatio, 
                                const int& id_weight, double& ratio,
                                TString baseTag, int theID);

    double Get_DileptonMass_IsHardProcess(const edm::Event& iEvent);

    void Print_OverflowEvent(const edm::Event& iEvent, const vector<TLorentzVector>& vec_vecP_dLep);
    vector<TLorentzVector> Get_GenLeptons(const edm::Event& iEvent, const TString& genFlag);

    vector<TLorentzVector> Find_TrueDYPairLepton(const edm::Event& iEvent, const vector<TLorentzVector>& vec_vecP_dLep);
    TLorentzVector Matched_SmallestDR(const TLorentzVector& vecP_HP, const vector<TLorentzVector>& vec_vecP_dLep);

    vector<TLorentzVector> Get_LHELeptons(const edm::Event& iEvent);
    double Get_DileptonMass_LHE(const edm::Event& iEvent);

    void InitVector_WeightRange();
    void Adjust_PDFWeight(int iSet, double& ratio);

    HistContainer* hist_; // -- full phase space
    HistContainer* hist_acc_; // -- fiducial phase space
};

DYAcceptanceProducer::DYAcceptanceProducer(const edm::ParameterSet& iConfig):
t_LHERunInfo_ ( mayConsume< LHERunInfoProduct,edm::InRun >   (iConfig.getUntrackedParameter<edm::InputTag>("LHERunInfoProduct")) ),
t_LHEEvent_   ( mayConsume< LHEEventProduct >                (iConfig.getUntrackedParameter<edm::InputTag>("LHEEvent")) ),
t_genEventInfo_( consumes< GenEventInfoProduct >            (iConfig.getUntrackedParameter<edm::InputTag>("GenEventInfo")) ),
t_genParticles_( consumes< std::vector<reco::GenParticle> > (iConfig.getUntrackedParameter<edm::InputTag>("GenParticles"))),
t_dressedLepton_( consumes< reco::GenJetCollection >        (iConfig.getUntrackedParameter<edm::InputTag>("DressedLepton"))) {
  
  channel_ = iConfig.getUntrackedParameter<std::string>("Channel");
  if( channel_ == "ee" )      thePDGID_ = 11;
  else if( channel_ == "mm" ) thePDGID_ = 13;
  else
    throw std::invalid_argument("Channel  = " + channel_ + " is not supported!");

  edm::Service<TFileService> fs;

  hist_    = new HistContainer(fs, "");
  hist_acc_ = new HistContainer(fs, "acc");

  double ptCut_lead  = iConfig.getUntrackedParameter<double>("PtCut_lead");
  double ptCut_sub   = iConfig.getUntrackedParameter<double>("PtCut_sub");
  double etaCut_lead = iConfig.getUntrackedParameter<double>("EtaCut_lead");
  double etaCut_sub  = iConfig.getUntrackedParameter<double>("EtaCut_sub");
  hist_acc_->Set_Acc(ptCut_lead, etaCut_lead, ptCut_sub, etaCut_sub);

  doCut_at_m100_ = iConfig.getUntrackedParameter<bool>("Cut_At_M100");
  doCheckOverflow_ = iConfig.getUntrackedParameter<bool>("Investigate_Overflow");
  adjustPDFWeight_ = iConfig.getUntrackedParameter<bool>("AdjustPDFWeight");
  PDFWeightRangeSigma_ = iConfig.getUntrackedParameter<double>("PDFWeightRangeSigma");

  if( adjustPDFWeight_ ) {
    vec_PDFWeight_mean_ = iConfig.getUntrackedParameter< vector<double> >("PDFWeight_mean");
    vec_PDFWeight_sigma_ = iConfig.getUntrackedParameter< vector<double> >("PDFWeight_sigma");
    InitVector_WeightRange();
  }
  else {
    vec_PDFWeight_mean_.clear();
    vec_PDFWeight_sigma_.clear();
    vec_PDFWeight_lowerLimit_.clear();
    vec_PDFWeight_upperLimit_.clear();
  }
}

void DYAcceptanceProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iEventSetup) {

  // -- does not fill the event if it has m(ll) > 100.0 GeV
  // -- to combine it with the high mass samples
  if( doCut_at_m100_ ) {

    // double mll_HP = Get_DileptonMass_IsHardProcess(iEvent);
    // double mll_LHE = Get_DileptonMass_LHE(iEvent);
    // vector<TLorentzVector> vec_vecP_HPLep = Get_GenLeptons(iEvent, "isHardProcess"); 
    // vector<TLorentzVector> vec_vecP_LHELep = Get_LHELeptons(iEvent);
    // printf("[isHardProcess]\n");
    // for(const auto& vecP_lep : vec_vecP_HPLep) {
    //   printf("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", vecP_lep.Pt(), vecP_lep.Eta(), vecP_lep.Phi());
    // }
    // printf("--> dilepton mass = %lf\n\n", mll_HP);

    // printf("[LHE]\n");
    // for(const auto& vecP_lep : vec_vecP_LHELep) {
    //   printf("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", vecP_lep.Pt(), vecP_lep.Eta(), vecP_lep.Phi());
    // }
    // printf("--> dilepton mass = %lf\n", mll_LHE);
    // printf("-----------------------\n");

    // double diLep_mass = Get_DileptonMass_IsHardProcess(iEvent);
    double diLep_mass = Get_DileptonMass_LHE(iEvent);
    if( diLep_mass > 100.0 ) return;
  }

  // -- GEN weights
  edm::Handle < GenEventInfoProduct > h_genEventInfo;
  iEvent.getByToken(t_genEventInfo_, h_genEventInfo);

  double weight = h_genEventInfo->weight();
  weight = (weight > 0) ? 1.0 : -1.0; // -- take sign-only (to avoid too large weight in a few events)

  // -- LHE event (return if it is not available)
  edm::Handle < LHEEventProduct > h_LHEEvent;
  bool hasLHE = iEvent.getByToken(t_LHEEvent_, h_LHEEvent);
  if( !hasLHE ) return;

  // -- get weights for the systematic variations
  double LHEWeight_cv = h_LHEEvent->originalXWGTUP();
  int nSystWeight = (int)h_LHEEvent->weights().size();
  std::map<TString, double> map_systWRatio;

  for(int i_weight=0; i_weight<nSystWeight; ++i_weight) {
    double systWeight_ith = h_LHEEvent->weights()[i_weight].wgt;
    double ratio = systWeight_ith / LHEWeight_cv;
    TString id_weight = h_LHEEvent->weights()[i_weight].id;

    // -- Atoi: convert TString to int
    Save_WeightInfo(map_systWRatio, id_weight.Atoi(), ratio);
    // vec_systWeightRatio.push_back(ratio);
    // std::cout << i_weight << "th weight = " << weight 
    //           << "(ID = " << h_LHEEvent->weights()[i_weight].id <<"), ratio w.r.t. original: " << ratio << endl;
  }

  // -- collect dressed leptons
  edm::Handle<reco::GenJetCollection> h_dressedLepton;
  iEvent.getByToken(t_dressedLepton_, h_dressedLepton);

  vector<TLorentzVector> vec_vecP_lep;
  for(size_t i_lep=0; i_lep<(*h_dressedLepton).size(); ++i_lep) {
    reco::GenJet dLep = (*h_dressedLepton)[i_lep];

    if( abs(dLep.pdgId()) != thePDGID_ ) continue;

    TLorentzVector vecP_lep;
    vecP_lep.SetPtEtaPhiM(dLep.pt(), dLep.eta(), dLep.phi(), dLep.mass());
    vec_vecP_lep.push_back(vecP_lep);
  }

  auto nLep = vec_vecP_lep.size();
  if( nLep != 2 ) {
    cout << "nLep = " << nLep << " is not equal to 2! ..." << endl;
    cout << "all lepton kinematics:" << endl;
    for(const auto& vecP_lep : vec_vecP_lep) {
      printf("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", vecP_lep.Pt(), vecP_lep.Eta(), vecP_lep.Phi());
    }
    if( nLep > 2 ) {
      cout << "--> nLep > 2: find leptons from the true DY pairs (isHardProcess) by dR matching" << endl;
      vec_vecP_lep = Find_TrueDYPairLepton(iEvent, vec_vecP_lep);
    }
    else { // -- if nLep == 0 or == 1
      cout << "--> nLep < 2: does not fill the histogram" << endl;
      return;
    }
  }
  // // nLep != 0: can happen if the sample is 3-flavor sample
  // if( nLep != 0 && nLep != 2 ) {
  //   cout << "nLep = " << nLep << " is not equal to 0 or 2! ... do not fill the histograms" << endl;
  //   return;
  // }
  // if( nLep == 0 ) return;

  // -- fill the histograms
  hist_->Fill(vec_vecP_lep[0], vec_vecP_lep[1], weight, map_systWRatio);
  hist_acc_->Fill(vec_vecP_lep[0], vec_vecP_lep[1], weight, map_systWRatio);

  // -- investigation: overflow event in m10-50
  if( doCheckOverflow_ )
    Print_OverflowEvent(iEvent, vec_vecP_lep);
}

void DYAcceptanceProducer::InitVector_WeightRange() {
  int nValue = (int)vec_PDFWeight_mean_.size();
  printf("[InitVector_WeightRange] # sets = %d\n", nValue);

  for(int i=0; i<nValue; ++i) {
    double mean  = vec_PDFWeight_mean_[i];
    double sigma = vec_PDFWeight_sigma_[i];

    double lowerLimit = mean - PDFWeightRangeSigma_ * sigma;
    double upperLimit = mean + PDFWeightRangeSigma_ * sigma;
    vec_PDFWeight_lowerLimit_.push_back( lowerLimit );
    vec_PDFWeight_upperLimit_.push_back( upperLimit );

    printf("--> [%d set] (mean, sigma, lowerLimit, upperLimit) = (%.4lf, %.4lf, %.4lf, %.4lf)\n", i+1, mean, sigma, lowerLimit, upperLimit);
  }
}

// -- matching with "isHardProcess" leptons using eta and phi --> use the matched dressed leptons for the mass
vector<TLorentzVector> DYAcceptanceProducer::Find_TrueDYPairLepton(const edm::Event& iEvent, const vector<TLorentzVector>& vec_vecP_dLep) {
  vector<TLorentzVector> vec_matchedDLep;

  vector<TLorentzVector> vec_vecP_HPLep = Get_GenLeptons(iEvent, "isHardProcess");

  for(const auto& vecP_HP : vec_vecP_HPLep) {
    TLorentzVector dLep_matched = Matched_SmallestDR(vecP_HP, vec_vecP_dLep);

    // printf("[isHardProcess lepton]   (pt, eta, phi) = (%.3lf, %.3lf, %.3lf)\n", vecP_HP.Pt(), vecP_HP.Eta(), vecP_HP.Phi());
    // printf("[Matched dressed lepton] (pt, eta, phi) = (%.3lf, %.3lf, %.3lf)\n", dLep_matched.Pt(), dLep_matched.Eta(), dLep_matched.Phi());
    // printf("\n");

    vec_matchedDLep.push_back( dLep_matched );
  }

  return vec_matchedDLep;
}

TLorentzVector DYAcceptanceProducer::Matched_SmallestDR(const TLorentzVector& vecP_HP, const vector<TLorentzVector>& vec_vecP_dLep) {
  double dR_smallest = 1e10;

  int i_selected = -1;
  int nDLep = (int)vec_vecP_dLep.size();
  for(int i=0; i<nDLep; ++i) {
    double dR = vecP_HP.DeltaR( vec_vecP_dLep[i] );
    if( dR < dR_smallest ) {
      dR_smallest = dR;
      i_selected = i;
    }
  }

  if( i_selected < 0 )
    throw std::runtime_error("[DYAcceptanceProducer::Matched_SmallestDR] no lepton is selected! check the details");

  return vec_vecP_dLep[i_selected];
}

void DYAcceptanceProducer::Print_OverflowEvent(const edm::Event& iEvent, const vector<TLorentzVector>& vec_vecP_dLep) {
  double diLep_mass_dressed = (vec_vecP_dLep[0]+vec_vecP_dLep[1]).M();
  // -- only overflow events with m > 50 GeV is our interest for this investigation
  if( diLep_mass_dressed < 50.0 ) return;

  printf("[Print_OverflowEvent]\n");
  printf("Dressed leptons:\n");
  for(const auto& vecP : vec_vecP_dLep) {
    printf("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", vecP.Pt(), vecP.Eta(), vecP.Phi());
  }
  printf("--> dilepton mass = %.1lf\n", diLep_mass_dressed);
  printf("\n");


  printf("isHardProcess leptons\n");
  vector<TLorentzVector> vec_vecP_HPLep = Get_GenLeptons(iEvent, "isHardProcess");
  for(const auto& vecP : vec_vecP_HPLep) {
    printf("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", vecP.Pt(), vecP.Eta(), vecP.Phi());
  }
  if( vec_vecP_HPLep.size() >= 2 )
    printf("--> dilepton mass = %.1lf\n", (vec_vecP_HPLep[0]+vec_vecP_HPLep[1]).M());
  else
    printf("--> less than 2 leptons!\n");
  printf("\n");


  printf("fromHardProcessFinalState leptons\n");
  vector<TLorentzVector> vec_vecP_FSLep = Get_GenLeptons(iEvent, "fromHardProcessFinalState");
  for(const auto& vecP : vec_vecP_FSLep) {
    printf("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", vecP.Pt(), vecP.Eta(), vecP.Phi());
  }
  if( vec_vecP_FSLep.size() >= 2 )
    printf("--> dilepton mass = %.1lf\n", (vec_vecP_FSLep[0]+vec_vecP_FSLep[1]).M());
  else
    printf("--> less than 2 leptons!\n");
  printf("\n");

  printf("--- Print_OverflowEvent: Done --- \n\n");
}

vector<TLorentzVector> DYAcceptanceProducer::Get_GenLeptons(const edm::Event& iEvent, const TString& genFlag) {
  edm::Handle < std::vector<reco::GenParticle> > h_genParticles;
  iEvent.getByToken(t_genParticles_, h_genParticles);

  vector<TLorentzVector> vec_vecP_lep;
  for(size_t i_gen=0; i_gen<(*h_genParticles).size(); ++i_gen) {
    reco::GenParticle genLepton = (*h_genParticles)[i_gen];

    if( abs(genLepton.pdgId()) != thePDGID_ ) continue;

    bool passGenFlag = false;
    if( genFlag == "isHardProcess" )             passGenFlag = genLepton.isHardProcess();
    if( genFlag == "fromHardProcessFinalState" ) passGenFlag = genLepton.fromHardProcessFinalState();
    if( genFlag == "" )                          passGenFlag = true; // take any particles

    if( !passGenFlag ) continue;

    TLorentzVector vecP_lep;
    vecP_lep.SetPxPyPzE(genLepton.px(), genLepton.py(), genLepton.pz(), genLepton.energy());
    vec_vecP_lep.push_back(vecP_lep);
  }

  return vec_vecP_lep;
}

double DYAcceptanceProducer::Get_DileptonMass_IsHardProcess(const edm::Event& iEvent) {

  vector<TLorentzVector> vec_vecP_lep = Get_GenLeptons(iEvent, "isHardProcess");

  auto nLep = vec_vecP_lep.size();
  // nLep != 0: can happen if the sample is 3-flavor sample
  // if( nLep != 0 && nLep != 2 ) {
  //   cout << "nLep = " << nLep << " is not equal to 0 or 2! ... do not fill the histograms" << endl;
  //   return;
  // }
  // if( nLep == 0 ) return;

  if( nLep != 2 ) {
    cout << "[Get_DileptonMass_IsHardProcess] nLep = " << nLep << " is not equal to 2! ..." << endl;
    cout << "all lepton kinematics:" << endl;
    for(const auto& vecP_lep : vec_vecP_lep) {
      printf("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", vecP_lep.Pt(), vecP_lep.Eta(), vecP_lep.Phi());
    }
    if( nLep > 2 )
      cout << "--> nLep > 2: use the two leading leptons" << endl;
    else { // -- if nLep == 0 or == 1
      cout << "--> nLep < 2: mass = 0" << endl;
      return 0;
    }
  }

  return (vec_vecP_lep[0]+vec_vecP_lep[1]).M();
}

void DYAcceptanceProducer::Save_WeightInfo(std::map<TString, double>& map_systWRatio, const int& id_weight, double& ratio) {
  TString tag = "";

  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "scaleVar", 1001, 1009, 1000);
  // -- id = 2000:          central value
  // -- id = 2001 ... 2100: 100 hessian sets
  // -- id = 2101:          alphaS, up
  // -- id = 2102:          alphaS, down
  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "PDFVar",  2000, 2102);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "PDFVar_as_0117", 2108);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "PDFVar_as_0119", 2109);

  // -- alternative PDFs
  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "NNPDF40", 2200, 2252);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "NNPDF40_as_0116", 2260);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "NNPDF40_as_0120", 2270);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "NNPDF40_pch", 2300);

  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "NNPDF30", 3000, 3100);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "NNPDF30_as_0117", 3103);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "NNPDF30_as_0119", 3104);

  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "PDF4LHC21", 4000, 4042);

  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "CT18", 5000, 5058);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18_as_0116", 5070);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18_as_0117", 5071);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18_as_0119", 5072);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18_as_0120", 5073);

  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "CT18Z", 5100, 5158);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18_as_0116", 5170);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18_as_0117", 5171);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18_as_0119", 5172);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18_as_0120", 5173);

  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "MSHT20", 6000, 6064);
  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "MSHT20_as", 6070, 6076);

  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "MMHT2014", 7000, 7050);
  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "MMHT2014_as", 7060, 7062);

  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "ABMP16", 8000, 8029);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "ABMP16als116_5_nlo", 8050);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "ABMP16als118_5_nlo", 8051);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "ABMP16als120_5_nlo", 8052);

  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "HERAPDF20", 13000, 13028);
  Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "HERAPDF20Var", 13050, 13063);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "HERAPDF20_as_0116", 13100);
  Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "HERAPDF20_as_0120", 13200);


  // Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "NNPDF30", 3000, 3100);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "NNPDF30_as_0115", 3102);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "NNPDF30_as_0117", 3103);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "NNPDF30_as_0119", 3104);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "NNPDF30_as_0121", 3105);

  // Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "CT18NNLO", 5100, 5158);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18NNLO_as_0116", 5160);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18NNLO_as_0120", 5170);

  // Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "CT18ZNNLO", 5200, 5258);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18ZNNLO_as_0116", 5260);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "CT18ZNNLO_as_0120", 5270);

  // Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "MMHT2014", 7000, 7050);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "MMHT2014_as_0120", 7060);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "MMHT2014_as_0117", 7061);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "MMHT2014_as_0119", 7062);

  // Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "ABMP16", 8000, 8029);

  // Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "HERAPDF20", 13000, 13028);
  // Fill_WeightInfo_Range(map_systWRatio, id_weight, ratio, "HERAPDF20Var", 13050, 13063);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "HERAPDF20_as_0116", 13106);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "HERAPDF20_as_0117", 13107);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "HERAPDF20_as_0118", 13108);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "HERAPDF20_as_0119", 13109);
  // Fill_WeightInfo_Single(map_systWRatio, id_weight, ratio, "HERAPDF20_as_0120", 13110);
}

void DYAcceptanceProducer::Fill_WeightInfo_Range(std::map<TString, double>& map_systWRatio, 
                                                 const int& id_weight, double& ratio,
                                                 TString baseTag, int id_start, int id_end, int id_cv) {
  bool isFound = false;
  int nVar = 0;

  if( id_start <= id_weight && id_weight <= id_end ) {
    isFound = true;
    if( id_cv < 0 ) id_cv = id_start; // -- if id_cv is not set --> cv = start
    nVar = id_weight - id_cv; // -- i.e. id = id_start --> nVar = 000    
  }

  if( isFound ) {
    TString tag = TString::Format("%s_%03d", baseTag.Data(), nVar);
    if( baseTag == "PDFVar" && adjustPDFWeight_ )
      Adjust_PDFWeight(nVar, ratio);

    map_systWRatio.insert( std::make_pair(tag, ratio) );
  }
}

void DYAcceptanceProducer::Adjust_PDFWeight(int iSet, double& ratio) {
  if( iSet == 0 ) return; // -- default set
  if( iSet == 101 ) return; // -- alphaS: no info
  if( iSet == 102 ) return; // -- alphaS: no info

  int index = iSet - 1; // -- e.g. 001 set --> 0-th element in vec_mean & vec_sigma

  double lowerLimit = vec_PDFWeight_lowerLimit_[index];
  double upperLimit = vec_PDFWeight_upperLimit_[index];

  bool isNormal = (lowerLimit < ratio && ratio < upperLimit);

  if( !isNormal ) {
    printf("[%d set] ratio = %lf", iSet, ratio);
    printf(" --> outside of 5-sigma range (%.4lf, %.4lf) (mean, sigma) = (%.4lf, %.4lf)\n", 
           lowerLimit, upperLimit, vec_PDFWeight_mean_[index], vec_PDFWeight_sigma_[index]);
  }


  if( !isNormal ) {
    ratio = vec_PDFWeight_mean_[index]; // -- change the value with the mean value
    printf("--> fixed value = %.4lf\n", ratio);
  }
}

void DYAcceptanceProducer::Fill_WeightInfo_Single(std::map<TString, double>& map_systWRatio, 
                                                 const int& id_weight, double& ratio,
                                                 TString baseTag, int theID) {
  if( id_weight == theID )
    map_systWRatio.insert( std::make_pair(baseTag, ratio) );
}

double DYAcceptanceProducer::Get_DileptonMass_LHE(const edm::Event& iEvent) {
  double mass = -1.0;

  vector<TLorentzVector> vec_vecP_lep = Get_LHELeptons(iEvent);
  int nLep = (int)vec_vecP_lep.size();

  if( nLep != 2 ) {
    cout << "[DYAcceptanceProducer::Get_DileptonMass_LHE] # LHE leptons are not 2!" << endl;
    if( nLep < 2 ) {
      throw std::invalid_argument( TString::Format("--> nLep = %d < 2 ... mass is set to -1.0 and return", nLep) );
      return mass;
    }
    else
      printf("--> nLep = %d > 2 ... select two leading leptons\n", nLep);
  }

  return (vec_vecP_lep[0]+vec_vecP_lep[1]).M();
}

vector<TLorentzVector> DYAcceptanceProducer::Get_LHELeptons(const edm::Event& iEvent) {
  vector<TLorentzVector> vec_vecP_lep;
  vec_vecP_lep.clear();

  edm::Handle < LHEEventProduct > h_LHEEvent;
  Bool_t isLHEAvailable = iEvent.getByToken(t_LHEEvent_, h_LHEEvent);
  if( !isLHEAvailable ) {
    cout << "LHE information is not found ... check sample or input tag" << endl;
    return vec_vecP_lep; // -- return empty vector
  }

  const lhef::HEPEUP& lheEvent = h_LHEEvent->hepeup();

  std::vector<lhef::HEPEUP::FiveVector> lheParticles = lheEvent.PUP;
  for( size_t i_par = 0; i_par < lheParticles.size(); ++i_par ) {
    Int_t id = lheEvent.IDUP[i_par];

    if( std::abs(id) != thePDGID_ ) continue;

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

  return vec_vecP_lep;
}

void DYAcceptanceProducer::endRun(const Run& iRun, const EventSetup& iSetup) {
  // -- LHE information
  // -- ref: https://twiki.cern.ch/twiki/bin/viewauth/CMS/LHEReaderCMSSW#Retrieving_the_weights
  edm::Handle<LHERunInfoProduct> h_LHERunInfo;
  bool hasLHE = iRun.getByToken(t_LHERunInfo_, h_LHERunInfo);
  if( !hasLHE ) {
    cout << "[DYAcceptanceProducer::endRun] This sample has no LHE information" << endl;
    return;
  }

  cout << "##### Information about LHE weights #####" << endl;
  LHERunInfoProduct info = *(h_LHERunInfo.product());
  typedef std::vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
  for (headers_const_iterator iter=info.headers_begin(); iter!=info.headers_end(); iter++) {
    std::cout << iter->tag() << std::endl;
    std::vector<std::string> lines = iter->lines();
    for (unsigned int iLine = 0; iLine<lines.size(); iLine++)
      std::cout << lines.at(iLine);
  }
  cout << "##### End of information about LHE weights #####" << endl;
}

DEFINE_FWK_MODULE(DYAcceptanceProducer);