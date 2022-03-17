#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <boost/format.hpp>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QKeyEvent>
#include <string>
#include <vector>
#include <QDebug>
#include <QTableWidgetItem>
#include <QString>
#include <QDateTime>
#include <QList>
#include <algorithm>
#include <QTimer>
#include <libxl.h>
#include <windows.h>
#include <shlobj.h>

#include "student.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool sqlInit();
    //键盘监听事件
    void keyPressEvent(QKeyEvent *event);

    void mainWindowInit();

    void setInformationNumber(int number);

    void tableSerchInit(StudentInformation studentInformation, QString value);

    void getDesktopPath(char *path);
private slots:

    void on_return_btn_clicked();

    void on_pushButton_clicked();

    void on_toSerchWindow_btn_clicked();

    void on_serch_btn_clicked();

    void on_delInformation_btn_clicked();

    void on_insert_btn_clicked();

    void on_return_btn_2_clicked();

    void on_pushButton_2_clicked();

    void on_change_btn_clicked();

    void on_export_btn_clicked();

    wchar_t* StrToWStr(const char* Input);

private:
    Ui::MainWindow *ui;
    //数据库信息
    QSqlDatabase db;
    QSqlQuery sql;

    //表格标签
    QTableWidgetItem *item_id;
    QTableWidgetItem *item_studentName;
    QTableWidgetItem *item_studentClass;
    QTableWidgetItem *item_studentEvent;
    QTableWidgetItem *item_startDate;
    QTableWidgetItem *item_stuedntEventSpecific;
};
#endif // MAINWINDOW_H
