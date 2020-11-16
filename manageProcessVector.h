#pragma once
#include <vector>
#include "Process.h"

class manageProcessVector {
private:
	// Vector holding all active processes
	static vector<Process> createdProcesses;

	// Coordinator's process number, it works as a flag for whether or not
	static int coordinatorID;

public:
	// Adds a new process to the processes vector.
	static void addNewProcess(Process new_process) {
		createdProcesses.push_back(new_process);
	}
	// Returns a process object for the requested ID.
	static Process getProcess(int id) {
		return createdProcesses[id];
	}
	// Returns the total number of created processes in the system.
	static int getProcessesNumber() {
		return createdProcesses.size();
	}
	// Updates the process status, active or terminated in the main processes vector.
	static void updateProcessStatus(int id, bool status) {
		createdProcesses[id].active = status;
	}
	// Updates the election status, continue or stop in the main processes vector.
	static void updateProcessElectionStatus(int id, bool status) {
		createdProcesses[id].stopElection = status;
	}
	// Updates the new timestamp of coordinator communication.
	static void updateProcessTimeStamp(int id, long long timeStamp) {
		createdProcesses[id].lastComm_timeStamp = timeStamp;
	}
	// returns the ID of the coordinator ID.
	static int getCoordinatorID() {
		return coordinatorID;
	}
	// Updates the global variable holding the coordinator process ID.
	static void updateCoordinatorID(int newID) {
		coordinatorID = newID;
	}
};