//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#include "StackingAction.hh"
#include "StackingMessenger.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ios.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
 #include <vector> 
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
G4bool StackingAction::fActive = false;
StackingAction::StackingAction()
 :  stage(0)
{ 
  messenger = new StackingMessenger(this); 
  
}

StackingAction::~StackingAction()
{  delete messenger; }

G4ClassificationOfNewTrack 
StackingAction::ClassifyNewTrack(const G4Track * aTrack)
{

G4ClassificationOfNewTrack classification = fUrgent;


  if (fActive==true) {
   if (GAMMATHEC(aTrack)==true)
   {
    classification = fKill;

    }
 }
  return classification;
}


    


void StackingAction::NewStage()
{
  stage++;
  // Other stage change : just re-classify
    stackManager->ReClassify();
 
}
    
void StackingAction::PrepareNewEvent()
{ 
  stage = 0; 
  
}

G4bool StackingAction::GAMMATHEC(const G4Track * aTrack)
{
  G4Navigator* gNavigator = G4TransportationManager::GetTransportationManager() ->GetNavigatorForTracking();
  G4ThreeVector pos = aTrack->GetPosition();
  G4ThreeVector *ptr = NULL;
  G4VPhysicalVolume *thepVolume;
thepVolume = gNavigator->LocateGlobalPointAndSetup(pos,ptr,false);
G4String theVolume= thepVolume->GetName();
G4ThreeVector direction = aTrack->GetMomentumDirection();
G4double theta  = std::acos(std::abs(direction.z()));
             
G4String  particleName= aTrack-> GetDefinition()->GetParticleName();
//ifand particleName=="gamma" (aTrack->GetParentID() >= 2){return true;};
//if (aTrack->GetKineticEnergy() <= 0.2 *MeV){return true;};
if ( aTrack->GetMomentumDirection().z() < 0){return true;};
if ( theta > 22.*deg and theVolume=="pTarget" ){return true;};



  return false;
}
void
StackingAction::SetIsActive(G4bool active) 
{
  fActive = active;
}

G4bool StackingAction::GetIsActive() 
{
  return fActive;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


