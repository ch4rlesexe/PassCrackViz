// src/UIManager.cpp
#include "UIManager.h"
#include "DictionaryCracker.h"
#include "BruteForceCracker.h"

#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

UIManager::UIManager(QWidget *parent)
    : QMainWindow(parent),
      dictCracker(nullptr),
      bfCracker(nullptr)
{
    // ----------------------------
    // 1) Apply a “dark” palette to the entire QApplication
    // ----------------------------
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,           QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText,       QColor(230, 230, 230));
    darkPalette.setColor(QPalette::Base,             QColor(42, 42, 42));
    darkPalette.setColor(QPalette::AlternateBase,    QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase,      QColor(230, 230, 230));
    darkPalette.setColor(QPalette::ToolTipText,      QColor(230, 230, 230));
    darkPalette.setColor(QPalette::Text,             QColor(230, 230, 230));
    darkPalette.setColor(QPalette::Button,           QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText,       QColor(230, 230, 230));
    darkPalette.setColor(QPalette::BrightText,       QColor(255, 0, 0));
    darkPalette.setColor(QPalette::Link,             QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight,        QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText,  QColor(20, 20, 20));

    qApp->setPalette(darkPalette);
    // Optionally, ensure the built‐in Fusion style is used for better consistency:
    qApp->setStyle("Fusion");

    // ----------------------------
    // 2) Build the rest of the UI
    // ----------------------------
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // === Input Group: target MD5 hash + wordlist path ===
    QGroupBox *inputGroup = new QGroupBox("Hash & Wordlist", central);
    inputGroup->setStyleSheet("QGroupBox { font-weight: bold; }");
    QFormLayout *inputLayout = new QFormLayout(inputGroup);

    // 1) Target MD5 hash
    QLineEdit *lineEditHash = new QLineEdit(inputGroup);
    lineEditHash->setObjectName("lineEditHash");
    lineEditHash->setPlaceholderText("e.g. 0d107d09f5bbe40cade3de5c71e9e9b7");
    inputLayout->addRow("Target MD5 Hash:", lineEditHash);

    // 2) Wordlist file + Browse button
    QHBoxLayout *wordlistLayout = new QHBoxLayout();
    QLineEdit *lineEditWordlist = new QLineEdit(inputGroup);
    lineEditWordlist->setObjectName("lineEditWordlist");
    lineEditWordlist->setPlaceholderText("Select a wordlist file…");
    QPushButton *buttonBrowse = new QPushButton("Browse", inputGroup);
    wordlistLayout->addWidget(lineEditWordlist);
    wordlistLayout->addWidget(buttonBrowse);
    inputLayout->addRow("Wordlist File:", wordlistLayout);

    mainLayout->addWidget(inputGroup);

    // === Dictionary Attack Button ===
    QPushButton *buttonStartDictionary = new QPushButton("Start Dictionary Attack", central);
    buttonStartDictionary->setObjectName("buttonStartDictionary");
    mainLayout->addWidget(buttonStartDictionary);

    // === Brute-Force Group: set max length + start button ===
    QGroupBox *bfGroup = new QGroupBox("Brute-Force Attack", central);
    bfGroup->setStyleSheet("QGroupBox { font-weight: bold; }");
    QHBoxLayout *bfLayout = new QHBoxLayout(bfGroup);
    QLabel *labelMaxLen = new QLabel("Max Length:", bfGroup);
    QSpinBox *spinBoxMaxLen = new QSpinBox(bfGroup);
    spinBoxMaxLen->setRange(1, 6);
    spinBoxMaxLen->setValue(4);
    spinBoxMaxLen->setObjectName("spinBoxMaxLen");
    QPushButton *buttonStartBruteForce = new QPushButton("Start Brute-Force Attack", bfGroup);
    buttonStartBruteForce->setObjectName("buttonStartBruteForce");
    bfLayout->addWidget(labelMaxLen);
    bfLayout->addWidget(spinBoxMaxLen);
    bfLayout->addWidget(buttonStartBruteForce);
    mainLayout->addWidget(bfGroup);

    // === Stop Button ===
    QPushButton *buttonStop = new QPushButton("Stop Cracking", central);
    buttonStop->setObjectName("buttonStop");
    buttonStop->setEnabled(false);
    mainLayout->addWidget(buttonStop);

    // === Progress Bar (indeterminate) ===
    QProgressBar *progressBar = new QProgressBar(central);
    progressBar->setObjectName("progressBar");
    progressBar->setRange(0, 0); // Indeterminate mode
    progressBar->setTextVisible(false);
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);

    // === Log Display ===
    QTextEdit *textEditLog = new QTextEdit(central);
    textEditLog->setObjectName("textEditLog");
    textEditLog->setReadOnly(true);
    // Dark‐mode tweak: use a slightly lighter background for readability
    textEditLog->setStyleSheet(
        "QTextEdit { background-color: #2D2D2D; color: #E6E6E6; }"
    );
    mainLayout->addWidget(textEditLog);

    // === Connect signals/slots ===
    connect(buttonBrowse, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(
            this,
            "Select Wordlist",
            QString(),
            "Text Files (*.txt);;All Files (*)"
        );
        if (!fileName.isEmpty()) {
            lineEditWordlist->setText(fileName);
        }
    });

    connect(buttonStartDictionary, &QPushButton::clicked,
            this, &UIManager::on_buttonStartDictionary_clicked);
    connect(buttonStartBruteForce, &QPushButton::clicked,
            this, &UIManager::on_buttonStartBruteForce_clicked);
    connect(buttonStop, &QPushButton::clicked,
            this, &UIManager::on_buttonStop_clicked);

    setWindowTitle("PassCrackViz");
    resize(650, 450);
}

UIManager::~UIManager() {
    if (dictCracker && dictCracker->isRunning()) {
        dictCracker->stop();
        dictCracker->wait();
    }
    if (bfCracker && bfCracker->isRunning()) {
        bfCracker->stop();
        bfCracker->wait();
    }
}

void UIManager::resetUI() {
    QPushButton *buttonStop = findChild<QPushButton *>("buttonStop");
    buttonStop->setEnabled(false);
    QProgressBar *progressBar = findChild<QProgressBar *>("progressBar");
    progressBar->setVisible(false);
}

void UIManager::on_buttonStartDictionary_clicked() {
    QLineEdit *lineEditHash = findChild<QLineEdit *>("lineEditHash");
    QLineEdit *lineEditWordlist = findChild<QLineEdit *>("lineEditWordlist");
    QTextEdit *textEditLog = findChild<QTextEdit *>("textEditLog");
    QPushButton *buttonStop = findChild<QPushButton *>("buttonStop");
    QProgressBar *progressBar = findChild<QProgressBar *>("progressBar");

    QString hash = lineEditHash->text().trimmed().toLower();
    QString wordlist = lineEditWordlist->text().trimmed();
    if (hash.isEmpty() || wordlist.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both hash and wordlist file.");
        return;
    }

    textEditLog->clear();
    dictCracker = new DictionaryCracker(hash, wordlist);
    connect(dictCracker, &DictionaryCracker::progressUpdated,
            this, &UIManager::updateProgress);
    connect(dictCracker, &DictionaryCracker::finishedCracking,
            this, &UIManager::crackingFinished);

    buttonStop->setEnabled(true);
    progressBar->setVisible(true);
    dictCracker->start();
}

void UIManager::on_buttonStartBruteForce_clicked() {
    QLineEdit *lineEditHash = findChild<QLineEdit *>("lineEditHash");
    QSpinBox *spinBoxMaxLen = findChild<QSpinBox *>("spinBoxMaxLen");
    QTextEdit *textEditLog = findChild<QTextEdit *>("textEditLog");
    QPushButton *buttonStop = findChild<QPushButton *>("buttonStop");
    QProgressBar *progressBar = findChild<QProgressBar *>("progressBar");

    QString hash = lineEditHash->text().trimmed().toLower();
    int maxLen = spinBoxMaxLen->value();
    if (hash.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter the target hash.");
        return;
    }

    textEditLog->clear();
    bfCracker = new BruteForceCracker(hash, maxLen);
    connect(bfCracker, &BruteForceCracker::progressUpdated,
            this, &UIManager::updateProgress);
    connect(bfCracker, &BruteForceCracker::finishedCracking,
            this, &UIManager::crackingFinished);

    buttonStop->setEnabled(true);
    progressBar->setVisible(true);
    bfCracker->start();
}

void UIManager::on_buttonStop_clicked() {
    QPushButton *buttonStop = findChild<QPushButton *>("buttonStop");
    buttonStop->setEnabled(false);

    if (dictCracker && dictCracker->isRunning()) {
        dictCracker->stop();
    }
    if (bfCracker && bfCracker->isRunning()) {
        bfCracker->stop();
    }
}

void UIManager::updateProgress(const QString &currentGuess, int attempts) {
    QTextEdit *textEditLog = findChild<QTextEdit *>("textEditLog");
    textEditLog->append(QString("<font color=\"#A9F\" >Attempt #%1:</font> %2")
                            .arg(attempts)
                            .arg(currentGuess));
}

void UIManager::crackingFinished(const QString &found, bool success) {
    QPushButton *buttonStop = findChild<QPushButton *>("buttonStop");
    buttonStop->setEnabled(false);
    QProgressBar *progressBar = findChild<QProgressBar *>("progressBar");
    progressBar->setVisible(false);

    QTextEdit *textEditLog = findChild<QTextEdit *>("textEditLog");
    if (success) {
        textEditLog->append(
            QString("<font color=\"#6C6\">✅ Success! Password found: %1</font>")
                .arg(found)
        );
    } else {
        textEditLog->append("<font color=\"#C66\">❌ Finished. Password not found.</font>");
    }
}

