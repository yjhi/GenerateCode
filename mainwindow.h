#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<Qmenu>
#include"models.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void slotRightContext(const QPoint &pos);
private:
    Ui::MainWindow *ui;
    void initUI();



    QMenu *rightMenu;
    int selRow;

    void DoMakeCode();
    QString strCodeTmpl(QList<DataItem> &datas);
    QString strMenuTmpl(QList<DataItem> &datas);
    QString strZhTmpl(QList<DataItem> &datas);
    void AddRow();
    void DelRow(int t=0);
    void ReadFile();
    void SaveFile();
    void DelAllRow();
    void closeEvent(QCloseEvent *event);
    void showMsg(QString msg);
};

#endif // MAINWINDOW_H
