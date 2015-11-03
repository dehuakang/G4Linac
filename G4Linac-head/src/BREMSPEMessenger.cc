//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
//
#include "BREMSPEMessenger.hh"
#include "physics.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"

BREMSPEMessenger::BREMSPEMessenger(physics * list)
  :list(list) 
{  
 listDir = new G4UIdirectory("/BREMSPE/");

 // nSplit command
 nSplitCmd = new G4UIcmdWithAnInteger("/BREMSPE/nSplit",this);  
 nSplitCmd->SetGuidance("Splitting parameter - Combien des photons doivent etre produit");
 nSplitCmd->SetParameterName("nSplit",false);
 nSplitCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

 activeCmd = new G4UIcmdWithABool("/BREMSPE/active",this);  
 activeCmd->SetGuidance("Activater ou  deactivater brem splitting");
 activeCmd->SetParameterName("active",false);
 activeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

BREMSPEMessenger::~BREMSPEMessenger()
{
  delete nSplitCmd;
  delete activeCmd;
  delete listDir;
}

void BREMSPEMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
 if (command == nSplitCmd ) { 
   list->SetNSplit(nSplitCmd->GetNewIntValue(newValue));
 } else if (command == activeCmd) {
   list->SetSplittingActive(activeCmd->GetNewBoolValue(newValue));
 }
 else {
   G4cerr << "***** Command is not found !!! " << newValue << G4endl;
 }

}

