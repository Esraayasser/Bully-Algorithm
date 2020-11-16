#pragma once
#pragma warning(disable:4996) 

#include <WinSock2.h>
#include <iostream>
#include <string> 
#include <queue>;
#include <map>

using namespace std;

class CommunicationManager
{
public:
	CommunicationManager(void);
	virtual ~CommunicationManager(void);
	// Starts the communication sequence and 
	//		checks for the coordinators responce time and handles any faliure.
	void startCommunication();
	// Here the coordinator starts actually sending the messages.
	bool coordinatorCommunication();
	// Adds a new process to the communication queue.
	void addProcessToCommunicationQueue(int processID);
	// Whena new coordinator is found,
	//		the global variable holding it's id, is updates here.
	void updateCoordinator(int newID);
	// Excutes the array creation, spliting and distributing to the processes.
	void coordinateTaskDistribution();

private:
	// a priority queue holding the processes 
	//		that the coordinator must communicate with in order, 
	//			stored as {timeStamp, processID}
	queue<int> communicationQueue;
};

