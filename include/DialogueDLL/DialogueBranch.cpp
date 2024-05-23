#include "pch.h"
#include "DialogueBranch.h"
#include <fstream>

std::vector<DialogueBranch> AllDialogueBranches;

void InitializeBranches()
{
	std::ifstream BranchMaster;
	BranchMaster.open("BranchAssignmentMaster.txt");
	std::string TempIDStorage;
	std::string TempLoopStorage;
	std::string TempStringStorage;
	std::vector<std::string> DialogueIDs;
	std::vector<Dialogue*> DialogueStorage;
	bool InnerLoop = true;
	bool OuterLoop = true;
	bool BranchLoop = true;
	while (OuterLoop) {
		if (std::getline(BranchMaster, TempIDStorage))
		{
			if (std::getline(BranchMaster, TempLoopStorage)) {
				if (TempLoopStorage == "%NOLOOP") {
					BranchLoop = false;
				}
				else {
					BranchLoop = true;
				}
			}
			while (InnerLoop) {
				if (std::getline(BranchMaster, TempStringStorage)) {
					if (TempStringStorage == "%END") {
						for (int k = 0; k < DialogueIDs.size(); k++) {
							DialogueStorage.push_back(RetrieveDialogue(DialogueIDs[k]));
						}
						AllDialogueBranches.push_back(DialogueBranch(TempIDStorage, DialogueStorage, BranchLoop));
						InnerLoop = false;
						TempIDStorage.clear();
						TempLoopStorage.clear();
						TempStringStorage.clear();
						DialogueIDs.clear();
						DialogueStorage.clear();
					}
					else {
						DialogueIDs.push_back(TempStringStorage);
					}
				}
				else
				{
					OuterLoop = false;
					break;
				}
			}
		}
		else {
			OuterLoop = false;
			break;
		}

		InnerLoop = true;
	}
	BranchMaster.close();
}

Dialogue* ProceedBranch(std::string BranchID)
{
	for (int i = 0; i < AllDialogueBranches.size(); i++) {
		if (BranchID == AllDialogueBranches[i].BranchID) {
			return AllDialogueBranches[i].Proceed();
		}
	}
}

std::string GetNextInBranch(std::string BranchID)
{
	for (int i = 0; i < AllDialogueBranches.size(); i++) {
		if (BranchID == AllDialogueBranches[i].BranchID) {
			return GetDialogue(AllDialogueBranches[i].Proceed()->Identifier);
		}
	}
}

void ResetBranch(std::string BranchID)
{
	for (int i = 0; i < AllDialogueBranches.size(); i++) {
		if (BranchID == AllDialogueBranches[i].BranchID) {
			AllDialogueBranches[i].Reset();
		}
	}
}