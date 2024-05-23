#pragma once
#include "Dialogue.h"

class DialogueBranch {
public:
	std::string BranchID;
	std::vector<Dialogue*> DialoguesInBranch;
	unsigned int Index = 0;
	bool AtEndOfIndex = false;
	bool Loop = true;
	DialogueBranch(std::string ID, std::vector<Dialogue*> Dialogues, bool WillLoop) {
		BranchID = ID;
		DialoguesInBranch = Dialogues;
		Loop = WillLoop;
	}
	Dialogue* Proceed() {
		if (Index + 1 == DialoguesInBranch.size()) {
			if (Loop) {
				unsigned int TempIndex = Index;
				Index = 0;
				return DialoguesInBranch[TempIndex];
			}
			else {
				AtEndOfIndex = true;
			}
			return DialoguesInBranch[Index];
		}
		return DialoguesInBranch[Index++];
	}
	void Reset() {
		Index = 0;
		AtEndOfIndex = false;
	}
};

extern std::vector<DialogueBranch> AllDialogueBranches;

void InitializeBranches();
Dialogue* ProceedBranch(std::string BranchID);
std::string GetNextInBranch(std::string BranchID);
void ResetBranch(std::string BranchID);