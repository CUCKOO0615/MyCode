
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵�����ļ�����
//*************************************************
#pragma once

#ifndef CUCKOO0615_UTILS_MACRO
#define CUCKOO0615_UTILS_MACRO

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef ERRMSG_LENGTH
#define ERRMSG_LENGTH 256
#endif

#endif

#include <string.h>

class FileUtils
{

private:
    char m_szErrMsg[ERRMSG_LENGTH];
    void SetErrMsg(const char* szErrMsg)   { strcpy(m_szErrMsg, szErrMsg); }
    void SetErrMsgEx(const char* szErrMsg) { strcat(m_szErrMsg, szErrMsg); }

public:
    FileUtils(void);
    ~FileUtils(void);

    /*
    ** ��ȡ�ļ��ֽڴ�С
    ** @param szFilePath: �ļ�·��
    ** @return: 0��ʾʧ�ܻ������ļ���С����0
    */
    __int64 GetFileSize(const char* szFilePath);

    //��ȡ������Ϣ
    const char* GetErrMsg() { return m_szErrMsg; }
};

