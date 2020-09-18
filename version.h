#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <direct.h>
#include <unistd.h>
#include <io.h>
#include <windows.h>
#include "md5.h"
#include "version.h"
#define MAXFILENUM 100
#define MAXSIZEVERSION 100
using namespace std;

struct Version   //ֻ�������ϸ��汾�仯���ļ����綪ʧ�Ļ�Ķ��Ļ����ӵģ��汾1�������а汾1���ļ��ұ�־λΪ2
{
    string allFilesPath[MAXFILENUM]; //�ļ�·��
    string allFilesName[MAXFILENUM];//�ļ���
    string logInformation;      //log ��Ϣ
    int fileState[MAXFILENUM]; //��־λ��1��ʾ���ǰ�汾�иĶ���2��ʾ�������ӵģ�3��ʾ�Ǵ˰汾�����ǰ�汾ȱʧ��
    int fileNum;               //���ļ�����
    int versionNum;            //�汾��
    int BeforeVersionNum;      //�ϸ��汾�İ汾��
};

string getinttostring ( const int n );
int* inilialize(Version version[], string path, string svnPath);
int getstringtoint  ( string  str );
void getFile( string path, string allFilesPath[], string allFilesName[]);
void ergodicExistFileInfo(Version version[], int nowVersionNum, string outputFilesName[], string outputFilesPath[]);
void stateChange(string path, Version version[], int nowVersionNum, string outFilePath[], string outFileName[], char outFileState[]);
void func1Create(string svnPath);
void func2Status(string outFileName[], char outFileState[], string sCommand);
void func3Add(string outFilePath[], string outFileName[], char outFileState[], string sCommand);
void func4Delete(Version version[], int nowVersionNum, string outFilePath[], string outFileName[], char outFileState[], string sCommand);
void func5Update(int iJumpVersion, Version version[], string path);
int func6Commit(Version version[], int nowVersionNum, int newestVersionNum, string svnPath, string outFilePath[], string outFileName[], char outFileState[], string log);
void func7Revert(int iJumpVersion, Version version[], string path, string outFileName[], char outFilestate[]);
void func8Log(Version version[], int nowVersionNum, int logNum);
void func9Attribute(Version version[], int nowVersionNum, int attributeNum);
void func10Quit(Version version[], int nowVersionNum, int newestVersionNum, string svnPath, string path);

#endif // VERSION_H_INCLUDED
