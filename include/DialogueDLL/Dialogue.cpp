#include "pch.h"
#include "Dialogue.h"
#include <fstream>
#include <Windows.h>
#include "DialogueBranch.h"

std::vector<Dialogue> AllDialogue;


void InitializeDialogue(std::vector<std::string> DialogueFileNames)
{
	// First creates all basic, static dialogue that doesn't contain variables. More categories can be added by developers as needed.
	for (int u = 0; u < DialogueFileNames.size(); u++) {
		std::ifstream DialogueMaster;
		DialogueMaster.open(DialogueFileNames[u]);
		if (DialogueMaster.is_open()) {
			std::string IDContainer;
			std::string TempContainer;
			std::vector<std::string> DialogueContainer;
			std::vector<int> IntContainer;
			std::vector<int> OrderContainer;
			// FullLoop is for the entire textfile.
			bool FullLoop = true;
			// InnerLoop is for each individual Dialogue.
			bool InnerLoop = true;
			while (FullLoop) {
				// Retrieves the Identifier for the dialogue.
				if (std::getline(DialogueMaster, IDContainer))
				{
					while (InnerLoop) {
						// Retrieves the next line.
						if (std::getline(DialogueMaster, TempContainer)) {
							// Checks the contents of the line. If it is one of the keywords, acts upon it. Otherwise inserts it as the next part of the dialogue.
							if (TempContainer == "%END") {
								// For %END, tells the string to go to the next line, ends the current Dialogue and moves on to the next one after.
								OrderContainer.push_back(0);
								DialogueContainer.push_back("\n");
								AllDialogue.push_back(Dialogue(IDContainer, DialogueContainer, OrderContainer));
								InnerLoop = false;
								IDContainer.clear();
								DialogueContainer.clear();
								OrderContainer.clear();
							}
							else if (TempContainer == "%ENDn") {
								// Same as %END but without going to the next line.
								AllDialogue.push_back(Dialogue(IDContainer, DialogueContainer, OrderContainer));
								InnerLoop = false;
								IDContainer.clear();
								DialogueContainer.clear();
								OrderContainer.clear();
							}
							else if (TempContainer == "%INT") {
								// Informs the code that an int is supposed to go here.
								OrderContainer.push_back(1);
							}
							else if (TempContainer == "%sINT") {
								// Int preceded by a space.
								DialogueContainer.push_back(" ");
								OrderContainer.push_back(0);
								OrderContainer.push_back(1);
							}
							else if (TempContainer == "%INTs") {
								// Int with a space after.
								OrderContainer.push_back(1);
								OrderContainer.push_back(0);
								DialogueContainer.push_back(" ");
							}
							else if (TempContainer == "%sINTs") {
								// Int with a space before and after.
								OrderContainer.push_back(0);
								DialogueContainer.push_back(" ");
								OrderContainer.push_back(1);
								OrderContainer.push_back(0);
								DialogueContainer.push_back(" ");
							}
							else if (TempContainer == "%FLOAT") {
								OrderContainer.push_back(2);
							}
							else if (TempContainer == "%sFLOAT") {
								DialogueContainer.push_back(" ");
								OrderContainer.push_back(0);
								OrderContainer.push_back(2);
							}
							else if (TempContainer == "%FLOATs") {
								OrderContainer.push_back(2);
								OrderContainer.push_back(0);
								DialogueContainer.push_back(" ");
							}
							else if (TempContainer == "%sFLOATs") {
								OrderContainer.push_back(0);
								DialogueContainer.push_back(" ");
								OrderContainer.push_back(2);
								OrderContainer.push_back(0);
								DialogueContainer.push_back(" ");
							}
							else if (TempContainer == "%STRING") {
								OrderContainer.push_back(3);
							}
							else if (TempContainer == "%sSTRING") {
								DialogueContainer.push_back(" ");
								OrderContainer.push_back(0);
								OrderContainer.push_back(3);
							}
							else if (TempContainer == "%STRINGs") {
								OrderContainer.push_back(3);
								OrderContainer.push_back(0);
								DialogueContainer.push_back(" ");
							}
							else if (TempContainer == "%sSTRINGs") {
								OrderContainer.push_back(0);
								DialogueContainer.push_back(" ");
								OrderContainer.push_back(3);
								OrderContainer.push_back(0);
								DialogueContainer.push_back(" ");
							}
							else {
								// Inserts the actual text from the file.
								DialogueContainer.push_back(TempContainer);
								OrderContainer.push_back(0);
							}
						}
						else
						{
							// If there is no text retrieved, it's the end of the file and the FullLoop ends.
							FullLoop = false;
						}
					}
				}
				else {
					FullLoop = false;
				}
				InnerLoop = true;
			}
			DialogueMaster.close();
		}
		else {
			OutputDebugStringA("ERROR: Dialogue-textfile not found.");
		}
	}
	InitializeBranches();
}

std::string GetDialogue(std::string ID)
{
	std::string CurrentDialogue;
	std::vector<std::string> CurrentVariableDialogue;
	bool IsBasic = true;
	bool FoundDialogue = false;
	int TempIndexBasic = 0;
	int TempIndexInt = 0;
	int TempIndexFloat = 0;
	int TempIndexString = 0;
	for (unsigned int i = 0; i < AllDialogue.size(); i++) {
		if (ID == (AllDialogue[i].Identifier)) {
			// If the dialogue-vector has more than one item, uses the more advanced setup.
			if (AllDialogue[i].VariableOrder.size() > 1) {
				for (int j = 0; j < AllDialogue[i].VariableOrder.size(); j++) {
					// Checks the order to figure out which type of variable to retrieve. Each particular variable has its own number in the order-vector, which must be set up here.
					if (AllDialogue[i].VariableOrder[j] == 0) {
						CurrentVariableDialogue.push_back(AllDialogue[i].Words[TempIndexBasic]);
						TempIndexBasic++;
					}
					else if (AllDialogue[i].VariableOrder[j] == 1) {
						CurrentVariableDialogue.push_back(std::to_string(*AllDialogue[i].IntVector[TempIndexInt]));
						TempIndexInt++;
					}
					else if (AllDialogue[i].VariableOrder[j] == 2) {
						CurrentVariableDialogue.push_back(std::to_string(*AllDialogue[i].FloatVector[TempIndexFloat]).substr(0, std::to_string(*AllDialogue[i].FloatVector[TempIndexFloat]).find(".") + 3));
						TempIndexFloat++;
					}
					else if (AllDialogue[i].VariableOrder[j] == 3) {
						CurrentVariableDialogue.push_back(*AllDialogue[i].StringVector[TempIndexString]);
						TempIndexString++;
					}
					else {
						OutputDebugStringA("ERROR: Incorrect order-digit.");
					}
					IsBasic = false;
				}
			}
			else {
				// If there are no variables in the dialogue, simply returns the first and only item in the vector.
				CurrentDialogue = AllDialogue[i].Words[0];
				IsBasic = true;
			}
			FoundDialogue = true;
			break;
		}
	}
	if (FoundDialogue) {
		if (IsBasic) {
			return std::string(CurrentDialogue);
		}
		else {
			for (int k = 0; k < CurrentVariableDialogue.size(); k++) {
				CurrentDialogue.append(CurrentVariableDialogue[k]);
			}
			return std::string(CurrentDialogue);
		}
	}
	else {
		return std::string("ERROR: Incorrect dialogue Identifier.");
	}
	//return std::string("Testing testing, does the library work as a library?");
}

Dialogue* RetrieveDialogue(std::string ID)
{
	for (int i = 0; i < AllDialogue.size(); i++) {
		if (ID == AllDialogue[i].Identifier) {
			return &AllDialogue[i];
		}
	}
	return nullptr;
}

void AddArray(std::string DialogueID, std::vector<int*> Ints)
{
	for (int i = 0; i < AllDialogue.size(); i++) {
		if (AllDialogue[i].Identifier == DialogueID) {
			AllDialogue[i].IntVector = Ints;
		}
	}
}

void AddArray(std::string DialogueID, std::vector<float*> Floats)
{
	for (int i = 0; i < AllDialogue.size(); i++) {
		if (AllDialogue[i].Identifier == DialogueID) {
			AllDialogue[i].FloatVector = Floats;
		}
	}
}

void AddArray(std::string DialogueID, std::vector<std::string*> Strings)
{
	for (int i = 0; i < AllDialogue.size(); i++) {
		if (AllDialogue[i].Identifier == DialogueID) {
			AllDialogue[i].StringVector = Strings;
		}
	}
}

void AddArray(std::string DialogueID, std::vector<int*> Ints, std::vector<float*> Floats)
{
	for (int i = 0; i < AllDialogue.size(); i++) {
		if (AllDialogue[i].Identifier == DialogueID) {
			AllDialogue[i].IntVector = Ints;
			AllDialogue[i].FloatVector = Floats;
		}
	}
}

void AddArray(std::string DialogueID, std::vector<int*> Ints, std::vector<std::string*> Strings)
{
	for (int i = 0; i < AllDialogue.size(); i++) {
		if (AllDialogue[i].Identifier == DialogueID) {
			AllDialogue[i].IntVector = Ints;
			AllDialogue[i].StringVector = Strings;
		}
	}
}

void AddArray(std::string DialogueID, std::vector<float*> Floats, std::vector<std::string*> Strings)
{
	for (int i = 0; i < AllDialogue.size(); i++) {
		if (AllDialogue[i].Identifier == DialogueID) {
			AllDialogue[i].FloatVector = Floats;
			AllDialogue[i].StringVector = Strings;
		}
	}
}

void AddArray(std::string DialogueID, std::vector<int*> Ints, std::vector<float*> Floats, std::vector<std::string*> Strings)
{
	for (int i = 0; i < AllDialogue.size(); i++) {
		if (AllDialogue[i].Identifier == DialogueID) {
			AllDialogue[i].IntVector = Ints;
			AllDialogue[i].FloatVector = Floats;
			AllDialogue[i].StringVector = Strings;
		}
	}
}
