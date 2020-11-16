#include "UDPInterProcessCommunication.h"
#include "Globals.h"


UDPInterProcessCommunication::UDPInterProcessCommunication(void)
{
	initializeWinsock();
	initializeRecieveSocket();
	initializeSendSocket();
}

UDPInterProcessCommunication::~UDPInterProcessCommunication(void)
{
	closesocket(reciveSocket);
	closesocket(sendSocket);
	WSACleanup();
}

bool UDPInterProcessCommunication::initializeWinsock()
{
	// Intializing WinSock
	printf("Initializing WinSock..\n");
	// WSAStartup returns 0 if the intialization is successful.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("FAILED, ERROR: %d\n", WSAGetLastError());
		return false;
	}
	printf("Initialized..\n");
	return true;
}

void UDPInterProcessCommunication::initializeRecieveSocket()
{
	if ((reciveSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
		printf("Socket creation failed, ERROR: %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Recieve socket created successfully..\n");

	// Prepare the sockaddr_in address structure. 
	struct sockaddr_in recieverAddr;
	// Specifies the adress family the socket can communicate with, IPv4.
	recieverAddr.sin_family = AF_INET;
	// Assigns the predefined port to the socket address.
	recieverAddr.sin_port = htons(PORT);
	// The socket can recieve messages from any IP address hence, INADDR_ANY.
	recieverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// To allow the socket binding to the same local address.
	int broadcastValue = 1;
	if (setsockopt(reciveSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&broadcastValue, sizeof(broadcastValue)) == SOCKET_ERROR) {
		printf("Could not set socket option in the initialization of the recieving socket, ERROR:% d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Binding the socket
	if (bind(reciveSocket, (sockaddr*)&recieverAddr, sizeof recieverAddr) == SOCKET_ERROR) {
		printf("Failed to bind the recieve socket, ERROR: %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Binding the recieve socket is done successfully..\n");
}

void UDPInterProcessCommunication::initializeSendSocket()
{
	if ((sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
		printf("Socket creation failed, ERROR: %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// To allow the socket to send messages to all active neighbours on the network.
	int broadcastValue = 1;
	if (setsockopt(sendSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastValue, sizeof(broadcastValue)) == SOCKET_ERROR) {
		printf("Could not set socket option in the initialization of the sending socket, ERROR:% d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}


	printf("Send socket created successfully..\n");
}

void UDPInterProcessCommunication::sendMessage(string message, int proc_id)
{
	printf("Sending a message..\n");

	// Prepare the sockaddr_in address structure. 
	struct sockaddr_in recieverAddr;
	// Specifies the adress family the socket can communicate with, IPv4.
	recieverAddr.sin_family = AF_INET;
	// Assigns the predefined port to the socket address.
	recieverAddr.sin_port = htons(PORT);
	// The socket sends the message to all active neigbours, which is required in bully algorithm.
	recieverAddr.sin_addr.s_addr = INADDR_BROADCAST;

	if (sendto(sendSocket, message.c_str(), message.length(), 0, (sockaddr*)&recieverAddr, sizeof recieverAddr) == SOCKET_ERROR) {
		printf("Message sending failed, ERROR: %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Message sent successfully from process %d to Address: %s Port: %d,\n", proc_id, inet_ntoa(recieverAddr.sin_addr), ntohs(recieverAddr.sin_port));
}


string UDPInterProcessCommunication::recieveMessage()
{
	printf("Recieving a message..\n");
	char buffer[BUFFER_LEN];
	int messageLen;

	struct sockaddr_in senderAddr;
	int senderAddrSize = sizeof(senderAddr);

	memset(buffer, 0, sizeof buffer);

	messageLen = recvfrom(reciveSocket, buffer, BUFFER_LEN, 0, (sockaddr*)&senderAddr, &senderAddrSize);

	if (WSAGetLastError() == WSAETIMEDOUT) {
		printf("Message recieving experinced WSAETIMEDOUT\n");
		return string();
	}
	if (messageLen == SOCKET_ERROR) {
		printf("Message recieving failed, ERROR: %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Message: %s recieved from: %s, Port: %d\n", buffer, inet_ntoa(senderAddr.sin_addr), ntohs(senderAddr.sin_port));

	// Returning the same message to the sender
	return string(buffer);
}

void UDPInterProcessCommunication::setTimeout(unsigned int timeout)
{
	if (setsockopt(reciveSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof timeout) == SOCKET_ERROR) {
		printf("Couldn't set timeout option to the recieve socket, ERROR: %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}