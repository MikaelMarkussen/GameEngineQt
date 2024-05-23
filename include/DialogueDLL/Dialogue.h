#pragma once

#include <string>
#include <vector>

// Each dialogue consists of the text one would see in a single text-box.

class Dialogue
{
public:
	// The ID-string used to locate each individual piece of dialogue.
	std::string Identifier = "ERROR: NO DIALOGUE IDENTIFIER APPLIED";
	// The actual text in the dialogue. If there are no variables used in the dialogue, this only uses a single string.
	std::vector<std::string> Words;
	// Informs the program of what order to utilize each type of variable, and when to use them compared to using a fixed string from Words.
	// 0: Non-variable string
	// 1: Int
	// 2: Float
	// 3: String
	// For example, { 0, 1, 0, 1, 3} will tell the program to print the first item in Words (the vector of non-variable strings), then the first Int variable, then the second item in Words, the second Int variable and finally the first string variable.
	std::vector<int> VariableOrder;
	// These vectors contain the different variables used in dialogues, such as an NPC telling the player they have a certain number of enemies left or using character names the player has written.
	std::vector<int*> IntVector;
	std::vector<float*> FloatVector;
	std::vector<std::string*> StringVector;

	Dialogue() {}
	Dialogue(std::string ID, std::string WordsInput) {
		Identifier = ID;
		Words.push_back(WordsInput);
	}
	Dialogue(std::string ID, std::vector<std::string> WordsInput, std::vector<int> OrderOfVariables) {
		Identifier = ID;
		Words = WordsInput;
		VariableOrder = OrderOfVariables;
	}
};

// This vector contains all the dialogue blocks in the entire program.
extern std::vector<Dialogue> AllDialogue;

// Creates all the dialogue options. Run once at the start of the program.
void InitializeDialogue(std::vector<std::string> DialogueFileNames);

// Returns a specific piece of dialogue based on the ID-text given.
std::string GetDialogue(std::string ID);

// Retrieves a specific dialogue-object itself.
Dialogue* RetrieveDialogue(std::string ID);

void AddArray(std::string DialogueID, std::vector<int*> Ints);
void AddArray(std::string DialogueID, std::vector<float*> Floats);
void AddArray(std::string DialogueID, std::vector<std::string*> Strings);
void AddArray(std::string DialogueID, std::vector<int*> Ints, std::vector<float*> Floats);
void AddArray(std::string DialogueID, std::vector<int*> Ints, std::vector<std::string*> Strings);
void AddArray(std::string DialogueID, std::vector<float*> Floats, std::vector<std::string*> Strings);
void AddArray(std::string DialogueID, std::vector<int*> Ints, std::vector<float*> Floats, std::vector<std::string*> Strings);