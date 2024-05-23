InitializeDialogue(std::vector<std::string> "Filenames of Dialogue text-files") must be run once at the start of the program.

This DLL uses txt-files to contain all the dialogue and relevant information.

Example:

Identifier
Dialogue
%sINTs
Dialogue
%END

Would give "Dialogue 8 Dialogue"

%END - Signals the end of the current text.
%ENDn - Ends the dialogue without going to the next row.
%INT - An int-variable goes here. An AddArray-function must be called with the correct variables in order.
%sINT - An int-variable with a space before
%INTs - An int-variable with a space after.
%sINTs - An int-variable with a space before and after.
Repeat for %FLOAT, %STRING.

These dialogues are retrieved for use in-game through GetDialogue(std::string of the dialogue Identifier), and Branches are played through GetNextInBranch(std::string of the dialogue Identifier) and ResetBranch(std::string of the dialogue Identifier)

All of these functions are run plainly in the code, and don't need to be assigned to an object or anything.

Variables must be assigned in the code itself using the AddArray-functions for each individual dialogue that involves variables.

By default, there are two text-files containing the dialogue: BasicDialogueTextMaster.txt and VariableDialogueTextMaster.txt. More can be added, but their filenames must be added to the Intilalize-function that runs once at the start of the program.

Dialogue Branches are clumps of dialogues that can be played one after the other. They are assigned in BranchAssignmentMaster.txt

Branch example 1:
BranchID
%LOOP
DialogueID1
DialogueID2
%END

When retrieving dialogue from BranchID thrice, you will first recieve DialogueID1, DialogueID2 the second time and DialogueID1 the third time.

Branch example 2:
BranchID
%NOLOOP
DialogueID1
DialogueID2
%END

When retrieving dialogue from BranchID thrice, you will first recieve DialogueID1, DialogueID2 the second time and DialogueID2 the third time.