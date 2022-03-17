#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

struct Student
{
    int Id;//id 年-月-日-序号 例：220306001 2022年3月6日第1个
    QString studentName;//姓名
    QString studentClass;//班级
    QString studentEvent;//事件
    QString startDate;//日期
    QString stuedntEventSpecific;//事件详情
};

enum StudentInformation
{
    Id,
    StudentName,
    StudentClass,
    StudentEvent,
    StudentDate,
    StartDate,
    StudentEventSpecific
};

#endif // STUDENT_H
