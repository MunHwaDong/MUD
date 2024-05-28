#ifndef __Server__
#define __Server__

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <thread>
#include <string>
#include <array>
#include <cstring>
#include <ws2tcpip.h>
#include "JobClass.h"

//채팅서버랑 게임 서버랑 포트를 다르게 줘야할듯. -> 서로 다른 프로세스이니.
#define PORT_NUMBER 9999

using namespace std;

struct SocketInfo
{

	SocketInfo()
	{
	}

	SocketInfo(SOCKET sock, HANDLE handle, string ipaddr)
	{
		this->sock = sock;
		this->ev = handle;
		this->ipAddr = ipaddr;
	}

	SOCKET sock;
	HANDLE ev;
	string nickName;
	string ipAddr;

};

class Server
{

public:
	//Constructer
	Server();

	void CloseServer();
	int InitServer();
	int AddClient(int idx);
	void RemoveClient(int idx);
	string GetClientIP(int idx);

protected:
	Job jobEntity;
	SocketInfo serverSocket;
	array<SocketInfo, 11> clientsSockets;
	const int maxClientNum = 10;
	int socketCount;

};

#endif
