#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //主窗口初始化
    this->setWindowTitle(boost::str(boost::format("学生违纪信息管理-%1%") % "登录").c_str());
    //tab切换焦点
    this->setTabOrder(ui->username_line, ui->password_line);
    //限制只可以输入数字
    ui->ID_line->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    //数据库初始化
    if (!sqlInit())
    {
        ui->information_lab->setText("网络连接失败！请检查网络状况！");
        //登录按钮不可操作
        ui->login_btn->setEnabled(false);
    }
    connect(ui->login_btn, &QPushButton::clicked, this, [=] {
        db.open();
        if (ui->username_line->text().isEmpty() || ui->password_line->text().isEmpty())
        {
            ui->information_lab->setText("请输入账号和密码");
        }
        else
        {
            sql = QSqlQuery(db);//查询绑定数据库
            sql.exec("select * from tb_password;");
            while (sql.next())
            {
                if (ui->username_line->text() == sql.value(0) && ui->password_line->text() == sql.value(1))
                {
                    this->setWindowTitle(boost::str(boost::format("学生违纪信息管理-%1%") % ui->username_line->text().toStdString()).c_str());
                    ui->stackedWidget->setCurrentIndex(2);
                    break;
                }
            }
            ui->information_lab->setText("账号或密码错误！");
        }
    });

}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

/**
 * 数据库初始化信息
 * @brief MainWindow::sqlInit
 * @return
 */
bool MainWindow::sqlInit()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    db.setHostName("101.42.254.26");
    db.setUserName("myh");
    db.setPassword("myh112233");
    db.setDatabaseName("db_myh");
    db.setPort(3306);
    return db.open();
}

/**
 * 键盘监听事件
 * @brief MainWindow::keyPressEvent
 * @param event
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )  //判断是否是回车键按下
    {
        emit ui->login_btn->click();     //发送出登录按钮的点击信号
    }
}

/**
 * 表格初始化
 * @brief MainWindow::mainWindowInit
 */
void MainWindow::mainWindowInit()
{
    db.open();
    ui->tableWidget->setColumnCount(6);//设置列数
    ui->tableWidget->setAlternatingRowColors(true);//设置行自动变色
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //按行选中
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);//设置第6列自动扩展列宽

    //学生信息
    Student student;
    std::vector<Student> students;
    //数据库查询
    sql.exec("select * from tb_student;");
    while(sql.next())
    {
        student.Id = sql.value(0).toInt();
        student.studentName = sql.value(1).toString();
        student.studentClass = sql.value(2).toString();
        student.studentEvent = sql.value(3).toString();
        student.startDate = sql.value(4).toString();
        student.stuedntEventSpecific = sql.value(5).toString();
        students.push_back(student);
    }
    ui->tableWidget->setRowCount(students.size());//设置行数
    //初始化表格标签
    int rowNumber = 0;
    for (std::vector<Student>::iterator it = students.begin(); it != students.end(); it++)
    {
        item_id = new QTableWidgetItem(QString::number(it->Id));
        item_studentName = new QTableWidgetItem(it->studentName);
        item_studentClass = new QTableWidgetItem(it->studentClass);
        item_studentEvent = new QTableWidgetItem(it->studentEvent);
        item_startDate = new QTableWidgetItem(it->startDate);
        item_stuedntEventSpecific = new QTableWidgetItem(it->stuedntEventSpecific);

        ui->tableWidget->setItem(rowNumber, 0, item_id);
        ui->tableWidget->setItem(rowNumber, 1, item_studentName);
        ui->tableWidget->setItem(rowNumber, 2, item_studentClass);
        ui->tableWidget->setItem(rowNumber, 3, item_studentEvent);
        ui->tableWidget->setItem(rowNumber, 4, item_startDate);
        ui->tableWidget->setItem(rowNumber, 5, item_stuedntEventSpecific);
        rowNumber++;
    }
    setInformationNumber(ui->tableWidget->rowCount());
};

/**
 * 返回按钮
 * @brief MainWindow::on_return_btn_clicked
 */
void MainWindow::on_return_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    this->setWindowTitle(boost::str(boost::format("学生违纪信息管理-%1%") % ui->username_line->text().toStdString()).c_str());
}

/**
 * 退出登录
 * @brief MainWindow::on_pushButton_clicked
 */
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->information_lab->setText("");
    this->setWindowTitle(boost::str(boost::format("学生违纪信息管理-%1%") % "登录").c_str());
    db.close();
}

/**
 * 跳转查询界面
 * @brief MainWindow::on_toSerchWindow_btn_clicked
 */
void MainWindow::on_toSerchWindow_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    this->setWindowTitle(boost::str(boost::format("学生违纪信息管理-%1%-%2%") % ui->username_line->text().toStdString() % "查询").c_str());
    mainWindowInit();//查询初始化
    ui->serchInformation_line->setPlaceholderText("搜索内容...");
}

/**
 * 设置信息数量
 * @brief MainWindow::setInformationNumber
 * @param number
 */
void MainWindow::setInformationNumber(int number)
{
    ui->lineNumber_lab->setText(boost::str(boost::format("共有%1%条记录...") % number).c_str());
}

/**
 * 按条件查询按钮
 * @brief MainWindow::on_serch_btn_clicked
 */
void MainWindow::on_serch_btn_clicked()
{
    if (!ui->serchInformation_line->text().isEmpty())
    {
        switch (ui->serchItem->currentIndex())
        {
        case 0:
            tableSerchInit(StudentInformation::Id, ui->serchInformation_line->text());
            break;
        case 1://名字
            tableSerchInit(StudentInformation::StudentName, ui->serchInformation_line->text());
            break;
        case 2://时间
            tableSerchInit(StudentInformation::StartDate, ui->serchInformation_line->text());
            break;
        case 3://班级
            tableSerchInit(StudentInformation::StudentClass, ui->serchInformation_line->text());
            break;
        }
    }
    {
        ui->serchInformation_line->setPlaceholderText("查询不能为空！");
    }
}

/**
 * 表格查询初始化
 * @brief MainWindow::tableSerchInit
 */
void MainWindow::tableSerchInit(StudentInformation studentInformation, QString value)
{
    db.open();
    ui->tableWidget->clearContents();
    //初始化表格标签
    int rowNumber = 0;
    ui->tableWidget->setColumnCount(6);//设置列数
    ui->tableWidget->setAlternatingRowColors(true);//设置行自动变色
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //按行选中
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);//设置第6列自动扩展列宽

    switch (studentInformation)
    {
    case StudentInformation::Id:
        sql.exec(boost::str(boost::format("select * from tb_student where %1% = %2%;") % "studentId" % value.toStdString()).c_str());
        break;
    case StudentInformation::StudentClass:
        sql.exec(boost::str(boost::format("select * from tb_student where %1% = '%2%';") % "studentClass" % value.toStdString()).c_str());
        break;
    case StudentInformation::StudentName:
        sql.exec(boost::str(boost::format("select * from tb_student where %1% = '%2%';") % "studentName" % value.toStdString()).c_str());
        break;
    case StudentInformation::StartDate:
        sql.exec(boost::str(boost::format("select * from tb_student where %1% = '%2%';") % "startDate" % value.toStdString()).c_str());
        break;
    }

    //学生信息
    Student student;
    std::vector<Student> students;
    while(sql.next())
    {
        student.Id = sql.value(0).toInt();
        student.studentName = sql.value(1).toString();
        student.studentClass = sql.value(2).toString();
        student.studentEvent = sql.value(3).toString();
        student.startDate = sql.value(4).toString();
        student.stuedntEventSpecific = sql.value(5).toString();
        students.push_back(student);
    }
    ui->tableWidget->setRowCount(students.size());//设置行数
    for (std::vector<Student>::iterator it = students.begin(); it != students.end(); it++)
    {
        item_id = new QTableWidgetItem(QString::number(it->Id));
        item_studentName = new QTableWidgetItem(it->studentName);
        item_studentClass = new QTableWidgetItem(it->studentClass);
        item_studentEvent = new QTableWidgetItem(it->studentEvent);
        item_startDate = new QTableWidgetItem(it->startDate);
        item_stuedntEventSpecific = new QTableWidgetItem(it->stuedntEventSpecific);

        ui->tableWidget->setItem(rowNumber, 0, item_id);
        ui->tableWidget->setItem(rowNumber, 1, item_studentName);
        ui->tableWidget->setItem(rowNumber, 2, item_studentClass);
        ui->tableWidget->setItem(rowNumber, 3, item_studentEvent);
        ui->tableWidget->setItem(rowNumber, 4, item_startDate);
        ui->tableWidget->setItem(rowNumber, 5, item_stuedntEventSpecific);
        rowNumber++;
    }
    setInformationNumber(ui->tableWidget->rowCount());
}


/**
 * 删除选中按钮信息
 * @brief MainWindow::on_delInformation_btn_clicked
 */
void MainWindow::on_delInformation_btn_clicked()
{
    db.open();
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    if (items.length() == 0)
    {
        ui->lineNumber_lab->setText("请选中需要删除的项");
    }
    else
    {
        QList<QTableWidgetItem*> _items;
        int size = items.length() / 6;
        //清洗数据
        for (int i = 0; i < size; i++)
        {
            _items.push_back(items.at(i * 6));
        }
        int newSize = _items.length();
        //执行删除
        for(int i = 0; i < newSize; i++)
        {
            int row = ui->tableWidget->row(_items.at(i));
            sql.exec(boost::str(boost::format("delete from tb_student where studentId = %1%;") % ui->tableWidget->item(row, 0)->text().toStdString()).c_str());
        }
        mainWindowInit();//查询初始化
    }
}

/**
 * 信息录入
 * @brief MainWindow::on_insert_btn_clicked
 */
void MainWindow::on_insert_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    this->setWindowTitle(boost::str(boost::format("学生违纪信息管理-%1%-%2%") % ui->username_line->text().toStdString() % "录入与修改").c_str());
    //清空数据
    ui->tip_lab->setText("");
    ui->studentClass_line->clear();
    ui->studentName_line->clear();
    ui->studentEvent_line->clear();
    ui->stuedntEventSpecific_line->clear();
    ui->ID_line->clear();
    ui->changeValue_line->clear();
}

//返回主界面
void MainWindow::on_return_btn_2_clicked()
{
    on_return_btn_clicked();
}

/**
 * 录入按钮
 * @brief MainWindow::on_pushButton_2_clicked
 */
void MainWindow::on_pushButton_2_clicked()
{
    Student student;
    if (ui->studentName_line->text().isEmpty() || ui->studentClass_line->text().isEmpty() || ui->studentEvent_line->text().isEmpty())
    {
        ui->tip_lab->setText("*项为必填项！");
    }
    else
    {
        db.open();
        ui->tip_lab->setText("");
        student.studentName = ui->studentName_line->text();
        student.studentClass = ui->studentClass_line->text();
        student.studentEvent = ui->studentEvent_line->text();
        student.stuedntEventSpecific = ui->stuedntEventSpecific_line->toPlainText();

        QDate todayDate = ui->studentDate_date->selectedDate();
        int year = todayDate.toString("yyyy").toInt();
        int mon = todayDate.toString("MM").toInt();
        int day = todayDate.toString("dd").toInt();
        //处理后的日期
        QString newDate = todayDate.toString(Qt::ISODate);

        int insertStudentID = (year - 2000) * 10000000 + mon * 100000 + day * 1000;

        //获取最新日期编号
        sql.exec("select studentId from tb_student;");
        std::vector<int> Ids;
        while(sql.next())
        {
            Ids.push_back(sql.value(0).toInt());
        }
        sort(Ids.begin(), Ids.end());//默认升序排列
        while (Ids.size())
        {
            if (Ids.back() / 1000 == insertStudentID / 1000)
            {
                insertStudentID = Ids.back() + 1;
                break;
            }
            else if (Ids.back() / 1000 > insertStudentID / 1000)
            {
                Ids.pop_back();
            }
            else if (Ids.back() / 1000 < insertStudentID / 1000)
            {
                insertStudentID = insertStudentID + 1;
                break;
            }
        }

        if (Ids.size() == 0)
        {
            insertStudentID = insertStudentID + 1;
        }

        QString sqlText = boost::str(boost::format("insert into tb_student(studentId, studentName, studentClass, studentEvent, startDate, stuedntEventSpecific) VALUES(%1%, '%2%', '%3%', '%4%', '%5%', '%6%');")
                                     % insertStudentID
                                     % student.studentName.toStdString()
                                     % student.studentClass.toStdString()
                                     % student.studentEvent.toStdString()
                                     % newDate.toStdString()
                                     % student.stuedntEventSpecific.toStdString()).c_str();
        if (sql.exec(sqlText))
        {
            ui->tip_lab->setText("录入成功！");
            QTimer::singleShot(2000,ui->tip_lab, [=]{ ui->tip_lab->setText(""); });//定时清理 1s
        }
        else
        {
            ui->tip_lab->setText("网络错误！,请重新启动程序！");
        }
    }
}

/**
 * 修改信息
 * @brief MainWindow::on_change_btn_clicked
 */
void MainWindow::on_change_btn_clicked()
{
    db.open();
    sql.exec("select studentId from tb_student;");
    std::vector<int> studentIds;
    while (sql.next())
    {
        studentIds.push_back(sql.value(0).toInt());
    }
    QString sqlStr;
    switch (ui->change_Com->currentIndex())
    {
    case 0:
        sqlStr = boost::str(boost::format("update tb_student set %1% = '%2%' where studentId = %3%;")
                            % "studentName"
                            % ui->changeValue_line->toPlainText().toStdString()
                            % ui->ID_line->text().toInt()).c_str();
        break;
    case 1:
        sqlStr = boost::str(boost::format("update tb_student set %1% = '%2%' where studentId = %3%;")
                            % "studentClass"
                            % ui->changeValue_line->toPlainText().toStdString()
                            % ui->ID_line->text().toInt()).c_str();
        break;
    case 2:
        sqlStr = boost::str(boost::format("update tb_student set %1% = '%2%' where studentId = %3%;")
                            % "studentEvent"
                            % ui->changeValue_line->toPlainText().toStdString()
                            % ui->ID_line->text().toInt()).c_str();
        break;
    case 3:
        sqlStr = boost::str(boost::format("update tb_student set %1% = '%2%' where studentId = %3%;")
                            % "stuedntEventSpecific"
                            % ui->changeValue_line->toPlainText().toStdString()
                            % ui->ID_line->text().toInt()).c_str();
        break;
    }
    bool isEmpty = true;//是否为空
    if (sql.exec(sqlStr))
    {
        for (std::vector<int>::iterator it = studentIds.begin(); it != studentIds.end(); it++)
        {
            if (*it == ui->ID_line->text().toInt())
            {
                ui->changeTip_lab->setText("修改成功！");
                QTimer::singleShot(2000, ui->changeTip_lab, [=]{ ui->changeTip_lab->setText(""); });
                isEmpty = false;
                break;
            }
        }
        if (isEmpty)
        {
            ui->changeTip_lab->setText("未找到可修改的对象！");
            QTimer::singleShot(2000, ui->changeTip_lab, [=]{ ui->changeTip_lab->setText(""); });
        }
    }
    else
    {
        ui->changeTip_lab->setText("网络错误！,请重新启动程序！");
    }
}


/**
 * 导出Excel
 * @brief MainWindow::on_export_btn_clicked
 */
void MainWindow::on_export_btn_clicked()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    std::vector<Student> students;
    Student student;
    if (items.length() == 0)
    {
        //执行当前全部
        int allrow = ui->tableWidget->rowCount();
        int allcol = ui->tableWidget->columnCount();
        for (int i = 0; i < allrow; i++)
        {
            int _col = 0;
            student.Id = ui->tableWidget->item(i, _col++)->text().toInt();
            student.studentName = ui->tableWidget->item(i, _col++)->text();
            student.studentClass = ui->tableWidget->item(i, _col++)->text();
            student.studentEvent = ui->tableWidget->item(i, _col++)->text();
            student.startDate = ui->tableWidget->item(i, _col++)->text();
            student.stuedntEventSpecific =ui->tableWidget->item(i, _col++)->text();
            students.push_back(student);
        }
//        sql.exec("select * from tb_student;");
//        while(sql.next())
//        {
//            qDebug() << "执行全部";
//            student.Id = sql.value(0).toInt();
//            student.studentName = sql.value(1).toString();
//            student.studentClass = sql.value(2).toString();
//            student.studentEvent = sql.value(3).toString();
//            student.startDate = sql.value(4).toString();
//            student.stuedntEventSpecific = sql.value(5).toString();
//            students.push_back(student);
//        }
    }
        else
        {
            qDebug() << "执行部分";
            QList<QTableWidgetItem*> _items;
            int size = items.length() / 6;
            //清洗数据
            for (int i = 0; i < size; i++)
            {
                _items.push_back(items.at(i * 6));
            }
            int newSize = _items.length();
            //执行查询
            for(int i = 0; i < newSize; i++)
            {
                int row = ui->tableWidget->row(_items.at(i));
                sql.exec(boost::str(boost::format("select * from tb_student where studentId = %1%;") % ui->tableWidget->item(row, 0)->text().toStdString()).c_str());
                qDebug() << boost::str(boost::format("select * from tb_student where studentId = %1%;") % ui->tableWidget->item(row, 0)->text().toStdString()).c_str();
                while (sql.next())
                {
                    student.Id = sql.value(0).toInt();
                    student.studentName = sql.value(1).toString();
                    student.studentClass = sql.value(2).toString();
                    student.studentEvent = sql.value(3).toString();
                    student.startDate = sql.value(4).toString();
                    student.stuedntEventSpecific = sql.value(5).toString();
                    students.push_back(student);
                }
            }
        }
    qDebug() << students.size();
    libxl::Book *book = xlCreateXMLBook();
    book->setKey(L"myh", L"windows-2c2b220001ccef0a66bf646dabr7s1he");
    if (book != NULL)
    {
        libxl::Sheet *sheet = book->addSheet(L"sheet1");
        if (sheet)
        {
            int row = 1;//设置行
            int col = 0;//设置列
            sheet->writeStr(0, 0, L"事件编号");
            sheet->writeStr(0, 1, L"学生姓名");
            sheet->writeStr(0, 2, L"学生班级");
            sheet->writeStr(0, 3, L"事件");
            sheet->writeStr(0, 4, L"日期");
            sheet->writeStr(0, 5, L"事件详情");

            for (std::vector<Student>::iterator it = students.begin(); it != students.end(); it++)
            {
                col = 0;
                sheet->writeNum(row, col++, it->Id);
                sheet->writeStr(row, col++, reinterpret_cast<const wchar_t *>(it->studentName.utf16()));
                sheet->writeStr(row, col++, reinterpret_cast<const wchar_t *>(it->studentClass.utf16()));
                sheet->writeStr(row, col++, reinterpret_cast<const wchar_t *>(it->studentEvent.utf16()));
                sheet->writeStr(row, col++, reinterpret_cast<const wchar_t *>(it->startDate.utf16()));
                sheet->writeStr(row, col++, reinterpret_cast<const wchar_t *>(it->stuedntEventSpecific.utf16()));
                row++;
            }
            char path[1024] = {0};//文件存储路径
            getDesktopPath(path);
            strcat(path, "\\students.xlsx");
            qDebug() << path;
            if (book->save(StrToWStr(path)))//保存到example.xls
            {
                ui->lineNumber_lab->setText("保存到桌面成功！");
                QTimer::singleShot(2000, ui->changeTip_lab, [=]{ ui->lineNumber_lab->setText(""); });
                book->release();//释放对象
            }
            else
            {
                ui->lineNumber_lab->setText("保存到桌面失败！");
                QTimer::singleShot(2000, ui->changeTip_lab, [=]{ ui->lineNumber_lab->setText(""); });
                book->release();//释放对象
            }
        }
    }

}

/**
 * 获取桌面路径
 * @brief MainWindow::getDesktopPath
 * @param path
 */
void MainWindow::getDesktopPath(char *path)
{
    LPITEMIDLIST pidl;
    LPMALLOC pShellMalloc;
    if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
    {
        if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl))) {
            SHGetPathFromIDListA(pidl, path);
            pShellMalloc->Free(pidl);
        }
        pShellMalloc->Release();
    }
}

wchar_t* MainWindow::StrToWStr(const char* Input)
{
        // Str的长度大于或等于转换后的WStr的长度
        auto MaxLen = strlen(Input);
        wchar_t* Output = new wchar_t[MaxLen + 1]{};
        std::mbstowcs(Output, Input, MaxLen);
        return Output;
}

