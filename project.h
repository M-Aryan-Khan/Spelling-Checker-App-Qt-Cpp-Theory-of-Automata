#ifndef PROJECT_H
#define PROJECT_H
#include <QMovie>
#include <QMainWindow>

namespace Ui {
class project;
}

class project : public QMainWindow
{
    Q_OBJECT

public:
    explicit project(QWidget *parent = nullptr);
    ~project();

private slots:
    void on_submit_clicked();

private:
    Ui::project *ui;
    void animateText();
    void stopLoadingAnimation();
    QMovie *movie;
};

#endif // PROJECT_H
