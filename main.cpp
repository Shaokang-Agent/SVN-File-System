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


int main()
{
    Version version[MAXSIZEVERSION]; //����汾��Ϣ�Ľṹ������
    string sCommand;//ָ��
    string outFilePath[MAXFILENUM]; //�����ļ�������ϸ��汾��ʧ��G�����ļ���·���� ��������е��򱣴浱ǰ·��������Ƕ�ʧ���򱣴�_SVN_�ļ����ϸ��汾��·��
    string outFileName[MAXFILENUM]; //�����ļ�������ϸ��汾��ʧ��G�����ļ�������
    char outFileState[MAXFILENUM]; //�����ļ�������ϸ��汾��ʧ��G�����ļ���״̬λ��־����ʼ��Ϊ0,add������Ϊ+,delete�����Ϊ-,��ʧ��ΪG���޸ĵ�ΪM,
                                    //�ϸ��汾�������Ҳ��ǣ�+����Ϊ�����ϸ��汾�����Ҳ��ǣ�+��-��G��M����ΪQ
    string svnPath = "D:\\Test\\_svn_";
    string path = "D:\\Test";
    int nowVersionNum = *inilialize(version, path, svnPath);
    int newestVersionNum = *(inilialize(version, path, svnPath) + 1);

    while (1)
    {
        cout << "svn>";
        getline(cin,sCommand);

        if( sCommand == "create") //"create"
        {
            func1Create(svnPath);
        }

        if(access(svnPath.c_str(), F_OK) == 0)
        {
            stateChange(path, version, nowVersionNum, outFilePath, outFileName, outFileState);
            func2Status(outFileName, outFileState, sCommand); //"status"
            func3Add(outFilePath, outFileName, outFileState, sCommand);//"add ..."
            func4Delete(version, nowVersionNum, outFilePath, outFileName, outFileState, sCommand);//"delete ..."


            if( sCommand == "update") //"update"
            {
                if (newestVersionNum != 0)
                {
                    int iChange = 0;
                    for(int iOutFileCount = 0; outFileName[iOutFileCount] != ""; ++iOutFileCount)
                    {
                        if(outFileState[iOutFileCount] == 'M' || outFileState[iOutFileCount] == 'G' || outFileState[iOutFileCount] == '+' || outFileState[iOutFileCount] == '-')
                        {
                            iChange = 1;
                        }
                    }
                    if(iChange == 1)
                    {
                        cout << "there changes" <<endl;
                        cout << endl;
                    }
                    if(iChange == 0)
                    {
                        func5Update(newestVersionNum, version, path);
                        nowVersionNum = newestVersionNum;
                        for(int iClearCount = 0; outFilePath[iClearCount] != "" ; ++iClearCount)
                        {
                            outFilePath[iClearCount] = "";
                            outFileName[iClearCount] = "";
                            outFileState[iClearCount] = '0';
                        }
                    }

                }
            }

            if( sCommand != "update"  && sCommand.substr(0,6) == "update") //"update ..."
            {
                if(getstringtoint(sCommand.substr(7)) > 0 && getstringtoint(sCommand.substr(7)) <= newestVersionNum)
                {
                    int iChange = 0;
                    for(int iOutFileCount = 0; outFileName[iOutFileCount] != ""; ++iOutFileCount)
                    {
                        if(outFileState[iOutFileCount] == 'M' || outFileState[iOutFileCount] == 'G' || outFileState[iOutFileCount] == '+' || outFileState[iOutFileCount] == '-')
                        {
                            iChange = 1;
                        }
                    }
                    if(iChange == 1)
                    {
                        cout << "there changes" <<endl;
                        cout << endl;
                    }
                    if(iChange == 0)
                    {
                        func5Update(getstringtoint(sCommand.substr(7)), version, path);
                        nowVersionNum = getstringtoint(sCommand.substr(7));
                        for(int iClearCount = 0; outFilePath[iClearCount] != "" ; ++iClearCount)
                        {
                            outFilePath[iClearCount] = "";
                            outFileName[iClearCount] = "";
                            outFileState[iClearCount] = '0';
                        }
                    }
                }
            }

            if( sCommand == "commit") //"commit"
            {
                int iReurnValue = func6Commit(version, nowVersionNum, newestVersionNum, svnPath, outFilePath, outFileName, outFileState, "") ;
                if( iReurnValue > 0)
                {
                    nowVersionNum = iReurnValue;
                    newestVersionNum = newestVersionNum + 1;
                }
            }

            if( sCommand != "commit" && sCommand.substr(0,7) == "commit ") //"commit ..."
            {
                if(sCommand.find('"') == 7)
                {
                    if(sCommand.find('"',8) > 0)
                    {
                        sCommand = sCommand.erase(sCommand.find('"',8), 1);
                    }
                    int iReurnValue = func6Commit(version, nowVersionNum, newestVersionNum, svnPath, outFilePath, outFileName, outFileState, sCommand.substr(8));
                    if( iReurnValue > 0)
                    {
                        nowVersionNum = iReurnValue;
                        newestVersionNum = newestVersionNum + 1;
                    }
                }
            }

            if( sCommand == "revert") //"revert"
            {
                func7Revert(nowVersionNum, version, path, outFileName, outFileState);
                for(int iClearCount = 0; outFilePath[iClearCount] != "" ; ++iClearCount)
                {
                    outFilePath[iClearCount] = "";
                    outFileName[iClearCount] = "";
                    outFileState[iClearCount] = '0';
                }
            }

            if( sCommand == "log")//"log ..."
            {
                if(nowVersionNum != 0)
                {
                    func8Log(version, nowVersionNum, 0);
                }
            }
            if( sCommand != "log" && sCommand.substr(0,4) == "log ") //"log ..."
            {
                if( getstringtoint(sCommand.substr(4)) > 0 && getstringtoint(sCommand.substr(4)) <= newestVersionNum )
                {
                    func8Log(version, nowVersionNum, getstringtoint(sCommand.substr(4)));
                }
            }

            if( sCommand == "attribute") //"attribute ..."
            {
                if(nowVersionNum != 0)
                {
                    func9Attribute(version, nowVersionNum, nowVersionNum);
                }
            }
            if( sCommand != "attribute" && sCommand.substr(0,10) == "attribute ") //"attribute ..."
            {
                if( getstringtoint(sCommand.substr(10)) > 0 && getstringtoint(sCommand.substr(10)) <= newestVersionNum )
                {
                    func9Attribute(version, nowVersionNum, getstringtoint(sCommand.substr(10)));
                }
            }

            if( sCommand == "quit") //"quit"
            {
                func10Quit(version, nowVersionNum, newestVersionNum, svnPath, path);
                break;
            }
        }


    }
    return 0;
}

