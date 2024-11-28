#include "project.h"
#include "ui_project.h"
#include <QTimer>
#include <QMovie>
#include <QString>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <sstream>

using namespace std;

project::project(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::project)
{
    ui->setupUi(this);
    ui->description->setText("");
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &project::animateText);
    timer->start(80);
    ui->loadingLabel->setVisible(false);
    movie = new QMovie("C:/Users/Lenovo/Documents/TOA/load.gif");
    ui->loadingLabel->setMovie(movie);
    movie->stop();
    ui->sentence_2->setVisible(false);
}

project::~project()
{
    delete ui;
}

string findW(const vector<string>& dw, const string& w) {
    for (const auto& word : dw) {
        if (word == w)
            return word;
    }
    return "";
}

int levenshteinDistance(const string& word1, const string& word2) {
    int m = word1.length();
    int n = word2.length();

    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            if (i == 0) {
                dp[i][j] = j;
            }
            else if (j == 0) {
                dp[i][j] = i;
            }
            else if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = 1 + min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
            }
        }
    }

    return dp[m][n];
}

string findClosestMatch(const vector<string>& dw, const string& w) {
    string closestMatch = "";
    int minDistance = INT_MAX;

    for (const auto& word : dw) {
        int distance = levenshteinDistance(word, w);
        if (distance < minDistance) {
            minDistance = distance;
            closestMatch = word;
        }
    }

    return closestMatch;
}

void project::animateText()
{
    static QString text = "Welcome to Correctify, your magical companion for flawless spelling every time!";
    static int currentIndex = 0;

    if (currentIndex < text.length()) {
        QString currentText = ui->description->text();
        currentText += text.at(currentIndex);
        ui->description->setText(currentText);
        currentIndex++;
    }
}

vector<string> token(const string& input) {
    istringstream iss(input);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void project::on_submit_clicked()
{
    ui->sentence_2->clear();
    ui->submit->setText("");
    ui->loadingLabel->setVisible(true);
    movie->start();
    QTimer::singleShot(1000, this, [this]() {
        stopLoadingAnimation();
        ui->sentence_2->setVisible(true);
        QString str = ui->sentence->text();
        string input = str.toStdString();
        ifstream dictionaryFile("C:/Users/Lenovo/Documents/TOA/dictionary.txt");
        vector<string> dictionaryWords;
        int total = 0;

        if (dictionaryFile.is_open()) {
            string word;
            while (getline(dictionaryFile, word)) {
                dictionaryWords.push_back(word);
            }
            dictionaryFile.close();
            total = dictionaryWords.size();
        }
        else {
            cerr << "Error opening dictionary file!\n";
            return;
        }

        regex wordRegex(R"([a-z]+)");
        regex excWord(R"([A-Za-z][a-z]*!)");
        regex hyphenRegex(R"([a-z]+[-][a-z]+)");
        regex aposRegex(R"([a-z]+['’](s|re|ll|d|t))");

        vector<string> text = token(input);
        for(int i=0;i<text.size();i++){
            string word = text[i];
            if(regex_match(word, wordRegex)){
                string matchW = findW(dictionaryWords, word);
                if (matchW.empty()) {
                    string suggestion = findClosestMatch(dictionaryWords, word);
                    ui->sentence_2->insert(QString::fromStdString(suggestion) + " ");
                }
                else{
                    ui->sentence_2->insert(QString::fromStdString(word) + " ");
                }
            }
            else if(regex_match(word, excWord)){
                string matchW = findW(dictionaryWords, word);
                if (matchW.empty()) {
                    string suggestion = findClosestMatch(dictionaryWords, word);
                    ui->sentence_2->insert(QString::fromStdString(suggestion) + " ");
                }
                else{
                    ui->sentence_2->insert(QString::fromStdString(word) + " ");
                }
            }
            else if(regex_match(word, hyphenRegex)){
                string matchW = findW(dictionaryWords, word);
                if (matchW.empty()) {
                    string suggestion = findClosestMatch(dictionaryWords, word);
                    ui->sentence_2->insert(QString::fromStdString(suggestion) + " ");
                }
                else{
                    ui->sentence_2->insert(QString::fromStdString(word) + " ");
                }
            }
            else if(regex_match(word, aposRegex)){
                string matchW = findW(dictionaryWords, word);
                if (matchW.empty()) {
                    string suggestion = findClosestMatch(dictionaryWords, word);
                    ui->sentence_2->insert(QString::fromStdString(suggestion) + " ");
                }
                else{
                    ui->sentence_2->insert(QString::fromStdString(word) + " ");
                }
            }
            else{
                ui->sentence_2->setText(QString::number(i+1) + " number word is invalid.");
                return;
            }
        }
    });
}

void project::stopLoadingAnimation()
{
    movie->stop();
    ui->submit->setText("Submit");
    ui->loadingLabel->setVisible(false);
}
