#pragma once
#include "communicationManager.h"
#include <vector>

class Process
{
public:
	Process(void);
	// Starts the election process.
	void startElection();
	// The process starts sending election messages
	//				and searches for the appropriate coordinator.
	bool enterElectionMode();
	// For Terminating or bringing a process back to life.
	void updateProcessStatus(bool status);
	// works as the receiving center and sending method of the processes.
	void recieveMessage(string message, int processID);
	// perfoms the task defined with the chunk of array sent to it by the coordinator.
	int performTask(vector<int> chunck, int chunck_st, int chunck_end);

	// Defines the process's status, active or terminated.
	bool active;
	// To stop the ellection process for a certain process when an election
	//						message is sent to it by a higher ID holding process.
	bool stopElection;
	// Holds the time stamp in which the process received a message from the 
	//					coordinator and is waiting for another after a certain timeout.
	long long lastComm_timeStamp;

private:
	// The ID assigned to the process.
	int processID;
	// The totalcount of processes created.
	static int processesCount;

	// Retrieves the new ID for the newly created process and increments the process count.
	int getProcessID();
};

