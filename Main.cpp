#include <iostream>
#include <string>
#include <vector>
#include "Server.h"
#include "ChatServer.h"
//#include "GameServer.h"

#define MAX_NOTI_MESSAGE 244

using namespace std;

int main(int argc, char* argv[])
{
    int portNumber = 9999;
    char message[MAXBYTE] = "";
    
    ChatServer chatServer;

    Server& server = chatServer;

    if (argv[1] != NULL)
        portNumber = atoi(argv[1]);

    thread mthread = thread(&ChatServer::ChatService, &chatServer);

    //게임 서버
    //thread gThread = thread(&GameServer::

    while (true)
    {
        gets_s(message, MAXBYTE);

        if (strcmp(message, "/x") == 0)
            break;
        else
        {
            char notifiForm[MAXBYTE] = "** 서버 공지 : ";
            strcat_s(notifiForm, message);
            chatServer.NotifyClient(notifiForm);

        }
    }

    chatServer.CloseServer();
    mthread.detach();

    WSACleanup();

	cout << argc << " " << argv[0] << endl;

	return 0;

}