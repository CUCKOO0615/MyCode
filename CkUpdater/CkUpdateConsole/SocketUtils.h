//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��14��
//��˵����Socket����
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

#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#ifndef ERRMSGTABLE_LENGTH
#define ERRMSGTABLE_LENGTH 36
#endif

typedef struct
{
	long lRetVal;
	const char* szErrMsg;
}ERRMSG_TABLE;
static const ERRMSG_TABLE ErrMsgTable[ERRMSGTABLE_LENGTH] =
{
	//socket
	{ WSANOTINITIALISED, "WSANOTINITIALISED" },
	{ WSAENETDOWN, "WSAENETDOWN" },
	{ WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT" },
	{ WSAEINPROGRESS, "WSAEINPROGRESS" },
	{ WSAEMFILE, "WSAEMFILE" },
	{ WSAENOBUFS, "WSAENOBUFS" },
	{ WSAEPROTONOSUPPORT, "WSAEPROTONOSUPPORT" },
	{ WSAEPROTOTYPE, "WSAEPROTOTYPE" },
	{ WSAESOCKTNOSUPPORT, "WSAESOCKTNOSUPPORT" },

	//connect
	{ WSANOTINITIALISED, "WSANOTINITIALISED" },
	{ WSAENETDOWN, "WSAENETDOWN" },
	{ WSAEADDRINUSE, "WSAEADDRINUSE" },
	{ WSAEINTR, "WSAEINTR" },
	{ WSAEINPROGRESS, "WSAEINPROGRESS" },
	{ WSAEADDRNOTAVAIL, "WSAEADDRNOTAVAIL" },
	{ WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT" },
	{ WSAECONNREFUSED, "WSAECONNREFUSED" },
	{ WSAEDESTADDRREQ, "WSAEDESTADDRREQ" },
	{ WSAEFAULT, "WSAEFAULT" },
	{ WSAEINVAL, "WSAEINVAL" },
	{ WSAEISCONN, "WSAEISCONN" },
	{ WSAEMFILE, "WSAEMFILE" },
	{ WSAENETUNREACH, "WSAENETUNREACH" },
	{ WSAENOBUFS, "WSAENOBUFS" },
	{ WSAENOTSOCK, "WSAENOTSOCK" },
	{ WSAETIMEDOUT, "WSAETIMEDOUT" },
	{ WSAEWOULDBLOCK, "WSAEWOULDBLOCK" },

	//bind
	{ WSANOTINITIALISED, "WSANOTINITIALISED" },
	{ WSAENETDOWN, "WSAENETDOWN" },
	{ WSAEADDRINUSE, "WSAEADDRINUSE" },
	{ WSAEFAULT, "WSAEFAULT" },
	{ WSAEINPROGRESS, "WSAEINPROGRESS" },
	{ WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT" },
	{ WSAEINVAL, "WSAEINVAL" },
	{ WSAENOBUFS, "WSAENOBUFS" },
	{ WSAENOTSOCK, "WSAENOTSOCK" }
};


class SocketUtils
{
	static bool m_bIsInited;
public:
	static const char* QueryErrMsg(size_t nErrCode);

	/*
	** ��ʼ��WinSocket, ��ʼ��WSA��Դ
	** @Param nErrCode:
	** @Ret : �����ɹ�����true,ʧ�ܷ���false
	** @ErrCode: WSASYSNOTREADY/WSAVERNOTSUPPORTED/WSAEINPROGRESS/WSAEPROCLIM/WSAEFAULT
	*/
	static bool InitSocketUtils(int& nErrCode);

	/*
	** �ͷ�WSA��Դ��
	** ���ú��������ʹ��socket,�������µ���InitSocketUtils()���г�ʼ��
	** @Ret : �����ɹ�����true,ʧ�ܷ���false
	*/
	static bool ReleaseSocketUtils();

	/*
	** ����һ��TCP�ͻ�������
	** @Param nErrCode: ��WSAGetLastError()���صĴ�����
	** @param szServerAddr�� ������IP��ַ
	** @param usPort�� �������˿ں�
	** @Ret : ���ӳɹ���������Ч��SOCKET�����򣬷���INVALID_SOCKET
	*/
	static SOCKET CreateClientSocket_TCP(int& nErrCode, const char* szServerAddr = "127.0.0.1", u_short usPort = 10086);

	/*
	** ����һ��TCP���������
	** @Param nErrCode: ��WSAGetLastError()���صĴ�����
	** @param usPort�� Ҫ�󶨵Ķ˿�
	** @return�� �����ɹ���������Ч��SOCKET�����򣬷���INVALID_SOCKET
	*/
	static SOCKET CreateServerSocket_TCP(int& nErrCode, u_short usPort = 10086);

	/*
	** ��ָ���Ķ˿ںŷ�Χ�ڳ��Խ���һ��TCP���������
	** @Param nErrCode: ��WSAGetLastError()���صĴ�����
	** @Param usPortMin: ��С�˿ں�
	** @Param usPortMax: ���˿ں�
	** @Param usPort: �����ɹ�ʱ���ص�ǰ���ڼ����Ķ˿ں�
	** @Ret: �����ɹ���������Ч��SOCKET�����򣬷���INVALID_SOCKET
	*/
	static SOCKET CreateServerSocket_TCP_Ex(int& nErrCode, u_short usPortMin, u_short usPortMax, u_short & usPort);

	/*
	** ��ָ����socket�Ͽ�ʼ����
	** @Param nErrCode: ��WSAGetLastError()���صĴ�����
	** @Param skSrvSock: 
	** @Param nBackLog: 
	** @Ret: �����ɹ�����true,ʧ�ܷ���false
	*/
	static bool ListenAt(int& nErrCode, SOCKET skSrvSock, int nBackLog);

	/*
	** ��ָ����Socket���ӷ���ָ�����ֽ���
	** @Param s: ָ����Socket����
	** @Param pBuffer: ���ͻ�����,��С��С��nSpecLength
	** @Param nSpecLength: ָ�����ֽ���
	** @Param nErrCode: ��WSAGetLastError()���صĴ�����
	** @Ret : socket���ӷ���SOCKET_ERRORʱ����false
	*/
	static bool SendToSocket(SOCKET s, char* pBuffer, int nSpecLength, int& nErrCode);

	/*
	** ��ָ����Socket���ӽ���ָ���ֽ���,
	** �����Socket�ж�ȡ���ֽ����޷��ﵽָ��,����������,
	** ֱ�����յ�ָ�����ֽ�����, ��������.
	** @Param s: ָ����Socket����
	** @Param pBuffer: ���ջ�����, ��С��С��nSpecLength
	** @Param nSpecLength: ָ�����ֽ���
	** @Param nErrCode: ��WSAGetLastError()���صĴ�����
	** @Ret : socket���ӷ���SOCKET_ERRORʱ����false
	*/
	static bool RecvFromSocket(SOCKET s, char* pBuffer, int nSpecLength, int& nErrCode);

	/*
	** ��ȡָ��socket��IP��Ϣ
	** @Param s: ָ����socket���
	** @Param addr: SOCKADDR_IN�ṹ��
	** @Param nErrCode: ��WSAGetLastError()���صĴ�����
	** @Ret : �����ɹ�����true,ʧ�ܷ���false
	*/
	static bool GetAddressBySocket(SOCKET s, SOCKADDR_IN & addr, int& nErrCode);

};


