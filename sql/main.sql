#数据库
CREATE DATABASE db_student;

USE db_student;

#学生表信息
CREATE TABLE tb_student
(
    studentId            int(9)        NOT NULL PRIMARY KEY,
    studentName          varchar(100)  NOT NULL ,
    studentClass         varchar(100)  NOT NULL,
    studentEvent         varchar(100)  NOT NULL ,
    startDate            varchar(30)   NOT NULL,
    stuedntEventSpecific varchar(1024) null
);

#密码表信息
CREATE TABLE tb_password
(
    username varchar(10) NOT NULL PRIMARY KEY ,
    _password varchar(50) NOT NULL
);