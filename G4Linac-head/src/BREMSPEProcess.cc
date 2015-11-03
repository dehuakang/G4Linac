
#include "BREMSPEProcess.hh"

#include "G4Track.hh"
#include "G4VParticleChange.hh"
#include <assert.h>
#include <vector>

// 
// BREMSPE is modifed version of Bremsstrahlung spltting technique developed by Tinslay (Jane Tinslay, March 2006), to see original technique visit the followning link:
 // J. Tinslay, T. Koi, Hands-on 5 Alternative Physics Lists, Bremsstrahlung Splitting, SLAC National Accelerator Laboratory, Menlo Park, CA, Stanford University, 2010.




// Initialise static data
G4int BREMSPEProcess::fNSplit = 1;
G4int BREMSPEProcess::fNSecondaries = 0;
G4bool BREMSPEProcess::fActive = false;

BREMSPEProcess::BREMSPEProcess() {}

BREMSPEProcess::~BREMSPEProcess() {}

G4VParticleChange* 
BREMSPEProcess::PostStepDoIt(const G4Track& track, const G4Step& step)
{
static int h=0,f=0;
  // Just do regular processing if brem splitting is not activated
  G4VParticleChange* particleChange(0);

  if (!fActive) {
    particleChange = pRegProcess->PostStepDoIt(track, step);
    assert (0 != particleChange);

    fNSecondaries += particleChange->GetNumberOfSecondaries();

    return particleChange;
  }
G4ThreeVector direction = track.GetMomentumDirection();
G4double theta  = std::acos(std::abs(direction.z()));
  if ((track.GetParentID() == 0 and theta < 5*deg))
{
  //    Do brem splitting

  assert (fNSplit > 0);

  G4int i(0);
  G4double weight = track.GetWeight()/fNSplit;
  
  // Secondary store
  std::vector<G4Track*> secondaries;
  secondaries.reserve(fNSplit);
    
  // Loop over PostStepDoIt method to generate multiple secondaries.
  for (i=0; i<fNSplit; i++) {    
    particleChange = pRegProcess->PostStepDoIt(track, step);

    assert (0 != particleChange);
    particleChange->SetVerboseLevel(0);

    G4int j(0);

    for (j=0; j<particleChange->GetNumberOfSecondaries(); j++) {
      secondaries.push_back(new G4Track(*(particleChange->GetSecondary(j))));
    }
  }	

  // Configure particleChange to handle multiple secondaries. Other 
  // data is unchanged
  particleChange->SetNumberOfSecondaries(secondaries.size());
  particleChange->SetSecondaryWeightByProcess(true);

  // Add all secondaries 
  std::vector<G4Track*>::iterator iter = secondaries.begin();
	
  while (iter != secondaries.end()) {
    G4Track* myTrack = *iter;
    myTrack->SetWeight(weight);
G4ThreeVector direction = myTrack->GetMomentumDirection();
G4ThreeVector position0=myTrack->GetPosition();
G4double theta  = std::acos(std::abs(direction.z()));
 //G4ThreeVector position1=position0.rotateZ(360*G4UniformRand());
//G4double theta  = std::acos(std::abs(direction.z()));
//  myTrack->SetPosition( position1);
//G4ThreeVector direction1=direction.rotateZ(360*G4UniformRand());
 //myTrack->SetMomentumDirection(direction1);
//myTrack->SetPosition(position1);
    // particleChange takes ownership
 //if ( theta < 3 *degree)
 particleChange->AddSecondary(myTrack);

    
    iter++;
  }

  fNSecondaries += secondaries.size();

  return particleChange;
  }

else {   particleChange = pRegProcess->PostStepDoIt(track, step);
    assert (0 != particleChange);

    fNSecondaries += particleChange->GetNumberOfSecondaries();

    return particleChange;}
}

void BREMSPEProcess::SetNSplit(G4int nSplit) 
{
  fNSplit = nSplit;
}

void BREMSPEProcess::SetIsActive(G4bool active) 
{
  fActive = active;
}

G4bool BREMSPEProcess::GetIsActive() 
{
  return fActive;
}

G4int BREMSPEProcess::GetNSplit() 
{
  return fNSplit;
}

G4int BREMSPEProcess::GetNSecondaries() 
{
  return fNSecondaries;
}
