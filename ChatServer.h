#ifndef __ChatServer__
#define __ChatServer__

#include "Server.h"

class ChatServer : public Server
{

public:
	void RecvAndBroadcast(int index);
	int NotifyClient(char* message);
	int ReadClient(int idx);
	void ChatService();

private:

};

#endif