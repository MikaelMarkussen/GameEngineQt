#ifndef DIALOGUECONTROLLER_H
#define DIALOGUECONTROLLER_H
#include <DialogueDLL/Dialogue.h>
#include <DialogueDLL/DialogueBranch.h>

class DialogueController
{
public:
    DialogueController();
    static DialogueController* getInstance();
    void setMainWindow(class MainWindow *mainWindowIn);
    class MainWindow* mMainWindow{nullptr};

    void AdvanceDialogueA();
    void AdvanceDialogueB();
};

#endif // DIALOGUECONTROLLER_H
