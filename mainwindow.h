#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QDir>

#include <QPushButton>
#include <QSpinBox>
#include <QDialog>
#include <QFile>
#include <QGroupBox>

#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QProcess>
#include <QRadioButton>
#include <QTimer>

#include <QSizePolicy>
#include <QSettings>
#include <QString>
#include <QtSvg>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString arg = "", QWidget *parent = nullptr);
    ~MainWindow();
private:
    QPushButton * btn;
    QTextEdit * text;
    QAction * createAction(QString text, QString code, int offset = 0);
    QPushButton * createButton(QString text, QString code, int offset = 0);
    void updateSvg();
    void save();
    void open();
    void openArg(QString file);
};
#endif // MAINWINDOW_H
