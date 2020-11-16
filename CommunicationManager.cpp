#include "communicationManager.h"
#include "Globals.h"

//int CommunicationManager::curID = 0;
//map<int, bool> CommunicationManager::inQueue;

CommunicationManager::CommunicationManager(void)
{
}

CommunicationManager::~CommunicationManager(void)
{
}

void CommunicationManager::startCommunication()
{
	int numOfCureatedProcesses = vectorManager.getProcessesNumber();
	for(int id = 0; id < numOfCureatedProcesses; id++){
		addProcessToCommunicationQueue(id);
	}

	if (!vectorManager.getProcessesNumber())
		return;
	
	bool communicationSucceeded = true;

	while (!communicationQueue.empty())
	{
		int curID = communicationQueue.front();
		Process curProcess = vectorManager.getProcess(curID);
		long long waitTime = getTimeNow() - curProcess.lastComm_timeStamp;
		//cout << "process " << curID << " inqueue " << inQueue[curID] << " timestamp " << curProcess.lastComm_timeStamp << " wait time " << waitTime << endl;
		if (waitTime >= TIMEOUT_MS) {
			cout << "Timout " << TIMEOUT_MS << " has been reached and process " << curID << ", did not hear from the coordinator.\n";
			cout << "Initiating elections..\n";
			curProcess.startElection();
			break;
		}
		else if(communicationSucceeded)
			communicationSucceeded = coordinatorCommunication();
	}
}

bool CommunicationManager::coordinatorCommunication()
{
	if (!communicationQueue.empty()) {
		// Get the process that should be communicated with now.
		int curID = communicationQueue.front();
		Process curProcess = vectorManager.getProcess(curID);
		bool active = vectorManager.getProcess(vectorManager.getCoordinatorID()).active;
		if (active) {
			//send message
			if (curID != vectorManager.getCoordinatorID())
				curProcess.recieveMessage("COORDINATOR_IS_COMMUNICATING", vectorManager.getCoordinatorID());

			communicationQueue.pop();
			return true;
		}
		else {
			cout << "Coordinator, process " << vectorManager.getCoordinatorID() << " is not active, communication failed\n";
			return false;
		}
	}
}

void CommunicationManager::addProcessToCommunicationQueue(int processID)
{
	Process curProcess = vectorManager.getProcess(processID);
	if (processID != vectorManager.getCoordinatorID() && curProcess.active) {
		long long tim = getTimeNow();
		vectorManager.updateProcessTimeStamp(processID, tim);
		//cout << "process " << processID << " is pushed into queue with time stamp " << -1 * curProcess.lastComm_timeStamp << " " << -1 * tim << endl;
		communicationQueue.push(processID);
	}
}

void CommunicationManager::updateCoordinator(int newID)
{
	// Update the coordinator process ID.
	vectorManager.updateCoordinatorID(newID);
	// Empty the communication queue.
	while (!communicationQueue.empty()){
		communicationQueue.pop();
	}

	// Send I_AM_COORDINATOR to all active processes
	for (int i = 0; i < vectorManager.getProcessesNumber(); i++) {
		Process curProcess = vectorManager.getProcess(i);
		// Send election message to the process.
		if (curProcess.active && newID != i)
			curProcess.recieveMessage("I_AM_COORDINATOR", vectorManager.getCoordinatorID());
	}

	// Start task distribution
	coordinateTaskDistribution();
}

void CommunicationManager::coordinateTaskDistribution()
{
	vector<int> largeArray;
	cout << "Creating the large array.\nArray values:";
	for (int i = 0; i < BUFFER_LEN; i++) {
		largeArray.push_back(rand()%10000 + 1);
		cout << " " << largeArray[i];
	}
	cout << endl << endl;
	int numOfProcesses = vectorManager.getProcessesNumber();
	int chunckSize = BUFFER_LEN / numOfProcesses;
	int chunck_st = 0;
	int chunck_en;
	int ans = 20000;
	for (int id = 0; id < numOfProcesses; id++) {
		Process curProcess = vectorManager.getProcess(id);
		chunck_en = chunck_st + chunckSize;
		if (id == numOfProcesses - 1)
			chunck_en = BUFFER_LEN;
		cout << "Sending the chunck of array with indeces [ " << chunck_st
			<< ", " << chunck_en << " ] " << "to process " << id << "." << endl;
		int proc_ans = curProcess.performTask(largeArray, chunck_st, chunck_en);
		cout << "The process returned " << proc_ans << " as it's answer\n";
		ans = min(ans, proc_ans);
		chunck_st = chunck_en;
	}
	cout << "The minimum value in the array is, " << ans << endl;
}


