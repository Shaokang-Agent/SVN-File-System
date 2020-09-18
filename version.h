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

struct Version   //只保存与上个版本变化的文件，如丢失的或改动的或增加的，版本1保存所有版本1的文件且标志位为2
{
    string allFilesPath[MAXFILENUM]; //文件路径
    string allFilesName[MAXFILENUM];//文件名
    string logInformation;      //log 信息
    int fileState[MAXFILENUM]; //标志位：1表示相对前版本有改动，2表示是新增加的，3表示是此版本相对于前版本缺失的
    int fileNum;               //总文件个数
    int versionNum;            //版本号
    int BeforeVersionNum;      //上个版本的版本号
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
