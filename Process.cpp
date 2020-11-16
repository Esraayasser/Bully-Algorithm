#include "Process.h"
#include "Globals.h"

int Process::processesCount = 0;

Process::Process(void)
{
	processID = getProcessID();
	this->active = true;
	this->stopElection = false;
	this->lastComm_timeStamp = 0;
	vectorManager.addNewProcess(*this);
	printf("Process with ID: %d was created.\n", processID);
}

void Process::startElection()
{
	if (enterElectionMode()) {
		// Assign this process as the new coordinator
		cout << "Coordinator found: " << processID << endl;
		commManager.updateCoordinator(this->processID);
		return;
	}
}

bool Process::enterElectionMode()
{
	cout << "process " << processID << " is in election mode.\n";
	int curID = 0;
	vectorManager.updateProcessElectionStatus(this->processID, false);
	long long ElectionStartTime = getTimeNow();
	while (true) {
		long long waitTime = getTimeNow() - ElectionStartTime;
		//cout << "process " << processID << " it's wait time " << waitTime << " election status " << vectorManager.getProcess(this->processID).stopElection << endl;
		this->stopElection = vectorManager.getProcess(this->processID).stopElection;

		if (this->stopElection)
			break;
		if (waitTime >= TIMEOUT_MS) {
			cout << "Process " << this->processID << ", did not hear from any of the higher processes.\n";
			cout << "Process " << this->processID << ", entering coordinator mode.\n";
			break;
		}

		if (curID < vectorManager.getProcessesNumber() && curID != this->processID) {
			//cout << "-> calls process " << curID << endl;
			Process curProcess = vectorManager.getProcess(curID);
			// Send election message to the process.
			if(curProcess.active)
				curProcess.recieveMessage("START_ELECTION", this->processID);
		}
		curID++;
	}

	return !this->stopElection;
}

void Process::updateProcessStatus(bool status)
{
	this->active = status;
	vectorManager.updateProcessStatus(this->processID, status);

	cout << "Process number " << this->processID << " was " << ((status)?"brought back to life.":"termainated") << endl;

	if (status) {
		cout << "Initiating elections to validate the current coordinator..\n";
		this->startElection();
	}
}

void Process::recieveMessage(string message, int processID)
{
	cout << "Message: " << message << " was sent from process: " << processID << " to process " << this->processID << endl;
	if (message == "START_ELECTION") {
		if (processID > this->processID) {
			cout << "Process " << this->processID << ", recieved a START_ELECTION message from a higher process." << endl;
			cout << "Process " << this->processID << ", is exiting election mode.\n";
			vectorManager.updateProcessElectionStatus(this->processID, true);
		}
		else
			this->startElection();
	}
}

int Process::performTask(vector<int> chunck, int chunck_st, int chunck_end)
{
	int ans = 20000;
	for (int i = chunck_st; i < chunck_end; i++)
		ans = min(chunck[i], ans);
	return ans;
}

int Process::getProcessID()
{
	return processesCount++;
}


