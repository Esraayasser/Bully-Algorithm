#pragma once
#pragma warning(disable:4996) 

#include <WinSock2.h>
#include <iostream>
#include <string> 

using namespace std;

class UDPInterProcessCommunication
{
public:
	UDPInterProcessCommunication(void);
	virtual ~UDPInterProcessCommunication(void);
	string recieveMessage();
	void sendMessage(string message, int proc_id);
	void setTimeout(unsigned int timeout);
private:
	bool initializeWinsock();
	void initializeRecieveSocket();
	void initializeSendSocket();

	WSADATA wsaData;
	SOCKET reciveSocket;
	SOCKET sendSocket;
};

