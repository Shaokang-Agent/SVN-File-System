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
#include "version.h"
#define MAXFILENUM 100
#define MAXSIZEVERSION 100


int* inilialize(Version version[], string path, string svnPath)
{

    version[0].BeforeVersionNum = -1;
    version[0].versionNum = 0;

    int returnvalue[2];

    ifstream txtfInfo;
    txtfInfo.open((svnPath + "\\" + "information.txt").c_str(), ios::in);

    if(!txtfInfo.is_open())
    {
        returnvalue[0] = 0;
        returnvalue[1] = 0;
    }
    else
    {
        string txtReadInfo[4*MAXFILENUM];
        int iReadLineCount = 0;
        while(!txtfInfo.eof())
        {
            getline(txtfInfo, txtReadInfo[iReadLineCount]);
            ++iReadLineCount;
        }
        returnvalue[0] = getstringtoint(txtReadInfo[0]);
        returnvalue[1] = getstringtoint(txtReadInfo[1]);
        int iCopyCount = 2;
        for(int iVersionCount = 1 ; iVersionCount <= returnvalue[1]; ++iVersionCount)
        {
            version[iVersionCount].logInformation = txtReadInfo[iCopyCount++];
            version[iVersionCount].BeforeVersionNum = getstringtoint(txtReadInfo[iCopyCount++]);
            version[iVersionCount].versionNum = getstringtoint(txtReadInfo[iCopyCount++]);
            version[iVersionCount].fileNum = getstringtoint(txtReadInfo[iCopyCount++]);
            int iFileCount = 0;
            for(iFileCount = 0; iFileCount < version[iVersionCount].fileNum; ++iFileCount)
            {
                version[iVersionCount].allFilesName[iFileCount] = txtReadInfo[iCopyCount++];
                version[iVersionCount].allFilesPath[iFileCount] = txtReadInfo[iCopyCount++];
                version[iVersionCount].fileState[iFileCount] = getstringtoint(txtReadInfo[iCopyCount++]);
            }
        }
    }
    txtfInfo.close();

    return returnvalue;
}




 void getFile( string path, string allFilesPath[], string allFilesName[])
 {
      long lFile = 0;

      struct _finddata_t fileData;
      string sLookforfile;
      if((lFile = _findfirst(sLookforfile.assign(path).append("\\*").c_str(),&fileData)) !=  -1)
      {
         int i = 0;
         do
         {
             if(fileData.attrib !=  _A_SUBDIR && (string )fileData.name != "svn.exe")
             {
                 allFilesPath[i] = path+"\\"+fileData.name;
                 allFilesName[i] = fileData.name;
                 ++i;
             }
         }while(_findnext(lFile, &fileData)  == 0);
         _findclose(lFile);
      }

 }

void stateChange(string path, Version version[], int nowVersionNum, string outFilePath[], string outFileName[], char outFileState[])
{
    string nowFilePath[MAXFILENUM];
    string nowFileName[MAXFILENUM];
    string sameFileName[MAXFILENUM];

    string allFilesName[MAXFILENUM];
    string allFilesPath[MAXFILENUM];

    ergodicExistFileInfo(version, nowVersionNum, allFilesName, allFilesPath);
    getFile(path, nowFilePath, nowFileName);
    int outFileCount = 0;

    if(nowVersionNum == 0)
    {
        for(int iCountLastFile = 0; outFileName[iCountLastFile] != ""; ++iCountLastFile)
        {
            int isFound = 0;
            for(int iCountNowFile = 0; nowFileName[iCountNowFile] != ""; ++iCountNowFile)
            {
                if(outFileName[iCountLastFile] == nowFileName[iCountNowFile])
                {
                    isFound = 1;
                }
            }
            if(isFound == 0)
            {
                int iShiftnumber = iCountLastFile;
                for(iShiftnumber = iCountLastFile; outFileName[iShiftnumber] != ""; ++iShiftnumber)
                {
                    outFileName[iShiftnumber] = outFileName[iShiftnumber+1];
                    outFilePath[iShiftnumber] = outFilePath[iShiftnumber+1];
                    outFileState[iShiftnumber] = outFileState[iShiftnumber+1];
                    --iCountLastFile;
                }
            }
        }
    }

    for(int iFile1Count = 0; allFilesName[iFile1Count] != ""; ++iFile1Count)
    {
        for(int iFile2Count = 0; nowFileName[iFile2Count] != ""; ++iFile2Count)
        {
            if(allFilesName[iFile1Count] == nowFileName[iFile2Count])
            {
                sameFileName[outFileCount] = nowFileName[iFile2Count];
                outFileName[outFileCount] = nowFileName[iFile2Count];
                outFilePath[outFileCount] = nowFilePath[iFile2Count];

                if(outFileState[outFileCount] != '-')
                {
                    if( FileDigest(allFilesPath[iFile1Count]) != FileDigest(nowFilePath[iFile2Count]) )
                    {
                        outFileState[outFileCount] = 'M';
                    }
                    else
                    {
                        outFileState[outFileCount] = 'Q';
                    }
                }
                ++outFileCount;
            }
        }
    }

    for(int iFile1Count = 0; allFilesName[iFile1Count] != ""; ++iFile1Count)
    {
        int isExist = 0;
        for(int iFile2Count = 0; sameFileName[iFile2Count] != ""; ++iFile2Count)
        {
            if(allFilesName[iFile1Count] == sameFileName[iFile2Count])
             {
                 isExist = 1;
                 break;
             }
        }
        if(isExist == 0)
        {
             outFileName[outFileCount] = allFilesName[iFile1Count];
             outFilePath[outFileCount] = allFilesPath[iFile1Count];
             outFileState[outFileCount] = 'G';
             ++outFileCount;
        }
    }

    for(int iFile1Count = 0; nowFileName[iFile1Count] != ""; ++iFile1Count)
    {
        int isExist = 0;
        for(int iFile2Count = 0; sameFileName[iFile2Count] != ""; ++iFile2Count)
        {
            if(nowFileName[iFile1Count] == sameFileName[iFile2Count])
             {
                 isExist = 1;
                 break;
             }
        }
        if(isExist == 0)
        {
             if ( outFileState[outFileCount] != '+' && outFileState[outFileCount] != '-' )
             {
                outFileName[outFileCount] = nowFileName[iFile1Count];
                outFilePath[outFileCount] = nowFilePath[iFile1Count];
                outFileState[outFileCount] = '?';
                ++outFileCount;
             }

        }
    }


}

void ergodicExistFileInfo(Version version[], int nowVersionNum, string outputFilesName[], string outputFilesPath[])
{
    string FileName[MAXFILENUM];
    string FilePath[MAXFILENUM];

    if(version[nowVersionNum].BeforeVersionNum == -1)
    {
        int iOutputFileCount = 0;
        for(int iVersionCount = 0; version[nowVersionNum].allFilesName[iVersionCount] != ""; ++iVersionCount)
        {
            if(version[nowVersionNum].fileState[iVersionCount] == 1 || version[nowVersionNum].fileState[iVersionCount] == 2)
            {
                outputFilesName[iOutputFileCount] = version[nowVersionNum].allFilesName[iVersionCount];
                outputFilesPath[iOutputFileCount] = version[nowVersionNum].allFilesPath[iVersionCount];
                ++iOutputFileCount;
            }
        }
    }

    else
    {
        int iOutputFileCount = 0;
        for(int iVersionCount = 0; version[nowVersionNum].allFilesName[iVersionCount] != ""; ++iVersionCount)
        {
            if(version[nowVersionNum].fileState[iVersionCount] == 1 || version[nowVersionNum].fileState[iVersionCount] == 2)
            {
                outputFilesName[iOutputFileCount] = version[nowVersionNum].allFilesName[iVersionCount];
                outputFilesPath[iOutputFileCount] = version[nowVersionNum].allFilesPath[iVersionCount];
                ++iOutputFileCount;
            }
        }

        ergodicExistFileInfo( version, version[nowVersionNum].BeforeVersionNum, FileName, FilePath);
        for(int versionCount = 0; FileName[versionCount] != ""; ++versionCount)
        {
            int isFound = 0;
            for(int nowversionCount = 0; version[nowVersionNum].allFilesName[nowversionCount] != ""; ++nowversionCount)
            {
                if(FileName[versionCount] == version[nowVersionNum].allFilesName[nowversionCount])
                {
                    isFound = 1;
                }
            }
            if(isFound == 0)
            {
                outputFilesName[iOutputFileCount] = FileName[versionCount];
                outputFilesPath[iOutputFileCount] = FilePath[versionCount];
                ++iOutputFileCount;
            }
        }
    }
}

void func1Create(string svnPath)
{
    if(access(svnPath.c_str(), F_OK) == -1)
    {
        _mkdir(svnPath.c_str());
        _mkdir((svnPath+"\\"+"version0").c_str());
    }
}

void func2Status(string outFileName[], char outFileState[], string sCommand)
{
    if( sCommand == "status")
    {
        for(int iFileCount = 0; outFileName[iFileCount] != ""; ++iFileCount)
        {
            if(outFileState[iFileCount] == 'M')
            {
                cout << "M" << " " << outFileName[iFileCount] << endl;
            }
            if(outFileState[iFileCount] == 'G')
            {
                cout << "G" << " " << outFileName[iFileCount] << endl;
            }
            if(outFileState[iFileCount] == '+')
            {
                cout << "+" << " " << outFileName[iFileCount] << endl;
            }
            if(outFileState[iFileCount] == '-')
            {
                cout << "-" << " " << outFileName[iFileCount] << endl;
            }
            if(outFileState[iFileCount] == '?')
            {
                cout << "?" << " " << outFileName[iFileCount] << endl;
            }
        }

    }
    cout << endl;
}

void func3Add(string outFilePath[], string outFileName[], char outFileState[], string sCommand)
{
    for(int iFileCount = 0; outFilePath[iFileCount] != ""; iFileCount++)
    {
        string sAddCommand = "add "+ outFileName[iFileCount];
        if( string(sCommand) == string(sAddCommand) && ( outFileState[iFileCount] == '?' || outFileState[iFileCount] == '-'))
        {
            outFileState[iFileCount] = '+';
            break;
        }
    }
}

void func4Delete(Version version[], int nowVersionNum, string outFilePath[], string outFileName[], char outFileState[], string sCommand)
{
    string versionPath[MAXFILENUM];
    string versionName[MAXFILENUM];

    ergodicExistFileInfo(version, nowVersionNum, versionName, versionPath);

    for(int iFileCount = 0; outFilePath[iFileCount] != ""; iFileCount++)
    {
        string sDeleteCommand = "delete "+ outFileName[iFileCount];
        if( string(sCommand)== string(sDeleteCommand) && (outFileState[iFileCount] == '+' || outFileState[iFileCount] == 'M' || outFileState[iFileCount] == 'Q'))
        {
            int isFoundNowVersion = 0;
            for (int iNowVersionCount = 0; versionName[iNowVersionCount] != ""; ++iNowVersionCount)
            {
                if(outFileName[iFileCount] == versionName[iNowVersionCount])
                {
                    isFoundNowVersion = 1;
                }
            }
            if(isFoundNowVersion == 1)
            {
                outFileState[iFileCount] = '-' ;
            }
            else if(isFoundNowVersion == 0)
            {
                outFileState[iFileCount] = '?' ;
            }
            break;
        }
    }
}

void func5Update(int iJumpVersion, Version version[], string path)
{
    string allFilePath[MAXFILENUM];
    string allFileName[MAXFILENUM];

    string versionPath[MAXFILENUM];
    string versionName[MAXFILENUM];

    getFile(path, allFilePath, allFileName);
    ergodicExistFileInfo(version, iJumpVersion, versionName, versionPath);

    for(int iFileCount = 0; allFileName[iFileCount] != ""; ++iFileCount)
    {
        if(allFileName[iFileCount] == "_svn_" || allFileName[iFileCount] == "svn.exe")
        {
            continue;
        }
        remove(allFilePath[iFileCount].c_str());
    }

    for(int iFileCount = 0; versionPath[iFileCount] != ""; ++iFileCount)
    {
        CopyFile((LPCSTR)(versionPath[iFileCount].c_str()), (LPCSTR)((path + "\\" + versionName[iFileCount]).c_str()), FALSE);
    }
}

int func6Commit(Version version[], int nowVersionNum, int newestVersionNum, string svnPath, string outFilePath[], string outFileName[], char outFileState[], string log)
{
    int change = 0;
    string newpath = svnPath+"\\"+"version"+getinttostring(newestVersionNum+1);
    int iStoreCount = 0;
    for(int iFileCount = 0; outFileName[iFileCount] != ""; ++iFileCount)
    {
        if(outFileState[iFileCount] == 'M')
        {
            if(access(newpath.c_str(), F_OK) == -1)
            {
                 _mkdir(newpath.c_str());
            }
            change = 1;
            version[newestVersionNum+1].allFilesName[iStoreCount] = outFileName[iFileCount];
            version[newestVersionNum+1].allFilesPath[iStoreCount] = newpath+"\\"+outFileName[iFileCount];
            version[newestVersionNum+1].fileState[iStoreCount] = 1;
            CopyFile((LPCSTR)(outFilePath[iFileCount].c_str()), (LPCSTR)(version[newestVersionNum+1].allFilesPath[iStoreCount].c_str()), FALSE);
            ++iStoreCount;
        }

        if(outFileState[iFileCount] == 'G')
        {
            if(access(newpath.c_str(), F_OK) == -1)
            {
                 _mkdir(newpath.c_str());
            }
            change = 1;
            version[newestVersionNum+1].allFilesName[iStoreCount] = outFileName[iFileCount];
            version[newestVersionNum+1].allFilesPath[iStoreCount] = newpath+"\\"+outFileName[iFileCount];
            version[newestVersionNum+1].fileState[iStoreCount] = 3;
            CopyFile((LPCSTR)(outFilePath[iFileCount].c_str()), (LPCSTR)(version[newestVersionNum+1].allFilesPath[iStoreCount].c_str()), FALSE);
            ++iStoreCount;
        }

        if(outFileState[iFileCount] == '+')
        {
            if(access(newpath.c_str(), F_OK) == -1)
            {
                 _mkdir(newpath.c_str());
            }
            change = 1;
            version[newestVersionNum+1].allFilesName[iStoreCount] = outFileName[iFileCount];
            version[newestVersionNum+1].allFilesPath[iStoreCount] = newpath+"\\"+outFileName[iFileCount];
            version[newestVersionNum+1].fileState[iStoreCount] = 2;
            CopyFile((LPCSTR)(outFilePath[iFileCount].c_str()), (LPCSTR)(version[newestVersionNum+1].allFilesPath[iStoreCount].c_str()), FALSE);
            ++iStoreCount;
        }

        if(outFileState[iFileCount] == '-')
        {
            if(access(newpath.c_str(), F_OK) == -1)
            {
                 _mkdir(newpath.c_str());
            }
            change = 1;
            version[newestVersionNum+1].allFilesName[iStoreCount] = outFileName[iFileCount];
            version[newestVersionNum+1].allFilesPath[iStoreCount] = newpath+"\\"+outFileName[iFileCount];
            version[newestVersionNum+1].fileState[iStoreCount] = 3;
            CopyFile((LPCSTR)(outFilePath[iFileCount].c_str()), (LPCSTR)(version[newestVersionNum+1].allFilesPath[iStoreCount].c_str()), FALSE);
            remove(outFilePath[iFileCount].c_str());
            ++iStoreCount;
        }
    }

    if(change == 0)
    {
        cout << "no modification" <<endl;
        cout << endl;
        return -1;
    }
    else
    {
        for(int iClearCount = 0; outFilePath[iClearCount] != "" ; ++iClearCount)
        {
            outFilePath[iClearCount] = "";
            outFileName[iClearCount] = "";
            outFileState[iClearCount] = '0';
        }

        version[newestVersionNum+1].fileNum = iStoreCount;
        version[newestVersionNum+1].versionNum = newestVersionNum+1;
        version[newestVersionNum+1].BeforeVersionNum = nowVersionNum;
        version[newestVersionNum+1].logInformation = log;

        cout << "version: " << newestVersionNum+1 << endl;
        cout << endl;
        return newestVersionNum+1;
    }
}

void func7Revert(int iJumpVersion, Version version[], string path, string outFileName[], char outFilestate[])
{
    if(iJumpVersion != 0)
    {
        func5Update(iJumpVersion, version, path);
    }

    if(iJumpVersion == 0)
    {
        for(int iFileCount = 0; outFileName[iFileCount] != ""; ++iFileCount)
        {
            outFilestate[iFileCount] = '?';
        }
    }
}

void func8Log(Version version[], int nowVersionNum, int logNum)
{
    string outline1[MAXFILENUM];
    string outline2[MAXFILENUM];

    int outLineNumMAX = 0;

    if(logNum == 0)
    {
        outline1[outLineNumMAX] = "version: " + getinttostring(nowVersionNum) + "<-";
        outline2[outLineNumMAX] = "log: " + version[nowVersionNum].logInformation;
        ++outLineNumMAX;
        while(version[nowVersionNum].BeforeVersionNum != 0)
        {
            outline1[outLineNumMAX] = "version: " + getinttostring(version[nowVersionNum].BeforeVersionNum);
            outline2[outLineNumMAX] = "log: " + version[version[nowVersionNum].BeforeVersionNum].logInformation;
            outLineNumMAX++;
            nowVersionNum = version[nowVersionNum].BeforeVersionNum;
        }
        for(int iLineCount = outLineNumMAX-1; iLineCount >= 0; iLineCount--)
        {
            cout << outline1[iLineCount] << endl;
            cout << outline2[iLineCount] << endl;
            cout << endl;
        }
    }
    else
    {
        cout << "version: " + getinttostring(logNum) << endl;
        cout << "log: " + version[logNum].logInformation <<endl;
        cout << endl;
    }
}

void func9Attribute(Version version[], int nowVersionNum, int attributeNum)
{
    string allFileName[MAXFILENUM];
    string allFilePath[MAXFILENUM];

    if(attributeNum == nowVersionNum)
    {
        ergodicExistFileInfo(version, nowVersionNum, allFileName, allFilePath);
        cout << "version: " << nowVersionNum << "<-" << endl;
        cout << "file:"<< endl;
        for(int iFileCount = 0; allFileName[iFileCount] != ""; ++iFileCount)
        {
            cout << "    " << allFileName[iFileCount] << endl;
        }
    }
    else  if(attributeNum != nowVersionNum)
    {
        ergodicExistFileInfo(version, attributeNum, allFileName, allFilePath);
        cout << "version: " << attributeNum << endl;
        cout << "file:"<< endl;
        for(int iFileCount = 0; allFileName[iFileCount] != ""; ++iFileCount)
        {
            cout << "    " << allFileName[iFileCount] << endl;
        }
    }
    cout << "log: " << version[attributeNum].logInformation << endl;
    cout << endl;
}
void func10Quit(Version version[], int nowVersionNum, int newestVersionNum, string svnPath, string path)
{
    string filePath = svnPath + "\\" + "information.txt";
    remove(filePath.c_str());

    if(newestVersionNum != 0)
    {
        ofstream txtfile;
        txtfile.open(filePath.c_str(),ios::out);

        txtfile << nowVersionNum << endl;
        txtfile << newestVersionNum << endl;
        for(int iVersionCount = 1; iVersionCount <= newestVersionNum; ++iVersionCount)
        {
            txtfile << version[iVersionCount].logInformation << endl;
            txtfile << version[iVersionCount].BeforeVersionNum<< endl;
            txtfile << version[iVersionCount].versionNum << endl;
            txtfile << version[iVersionCount].fileNum << endl;
            for(int iFileCount = 0; version[iVersionCount].allFilesName[iFileCount] != ""; ++iFileCount)
            {
                txtfile << version[iVersionCount].allFilesName[iFileCount] << endl;
                txtfile << version[iVersionCount].allFilesPath[iFileCount] << endl;
                txtfile << version[iVersionCount].fileState[iFileCount] << endl;
            }
        }

        txtfile.close();
    }
}
string getinttostring ( const int n )
{
    std::stringstream newstr;
    newstr<<n;
    return newstr.str();
}

int getstringtoint  ( string  str )
{
    int i = atoi(str.c_str());;
    return i;
}
