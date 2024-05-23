#include "dialoguecontroller.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPlainTextEdit>
#include <string>

DialogueController::DialogueController(){}

DialogueController *DialogueController::getInstance()
{
    static DialogueController *mInstance = new DialogueController();
    return mInstance;
}

void DialogueController::setMainWindow(MainWindow *mainWindowIn)
{
    mMainWindow = mainWindowIn;
}

void DialogueController::AdvanceDialogueA()
{
    std::string input = "Testing.";
    //std::string input = GetNextInBranch("TestingBranch");
    QTextCharFormat textformat;
    textformat = mMainWindow->ui->outputLog->currentCharFormat();
    textformat.setForeground(QBrush((Qt::black)));
    mMainWindow->ui->dialogueTextOutput->setCurrentCharFormat(textformat);
    mMainWindow->ui->dialogueTextOutput->appendPlainText(input.c_str());
}

void DialogueController::AdvanceDialogueB()
{
    std::string input = "Another test.";
    //std::string input = GetNextInBranch("SecondTester");
    QTextCharFormat textformat;
    textformat = mMainWindow->ui->outputLog->currentCharFormat();
    textformat.setForeground(QBrush((Qt::black)));
    mMainWindow->ui->dialogueTextOutput->setCurrentCharFormat(textformat);
    mMainWindow->ui->dialogueTextOutput->appendPlainText(input.c_str());
}

