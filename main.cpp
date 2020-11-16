//main.cpp : Defines the entry point for the console application
#include <stdio.h>
#include <string>
#include "Globals.h"
#include "Process.h"
#include "communicationManager.h"
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main() {

	char inputCmd;
	startTime_MS = getTimeNow();
	while (true) {

		cout << "Time now " << getTimeNow() << endl;
		if(vectorManager.getProcessesNumber() > 0)
			cout << "coordinator " << vectorManager.getCoordinatorID() << " status " << ((vectorManager.getProcess(vectorManager.getCoordinatorID()).active)? "ALIVE":"TERMINATED") << endl;
		commManager.startCommunication();

		cout << "Would you like to excute any action? (y/n): ";
		cin >> inputCmd;
		if (tolower(inputCmd) == 'y') {
			while (true) {
				cout << "\nPlease choose the action that you'd like to perform:\n";
				cout << "c: to create a new process and add it to the network.\nt: to terminate a certain process and restart the election process.\na: to bring a process back to life.\nx:to exit without performing any condition.\n";
				cin >> inputCmd;
				inputCmd = tolower(inputCmd);
				if (inputCmd == 'c') {
					Process newProcess;
					cout << "Number of processes created so far: " << vectorManager.getProcessesNumber() << endl;
					newProcess.startElection();
					cout << endl;
					break;
				}
				else if (inputCmd == 't') {
					int procID;
					cout << "Available active processes are:\n";
					for (int i = 0; i < vectorManager.getProcessesNumber(); i++) {
						if (vectorManager.getProcess(i).active)
							cout << "Process " << i << endl;
					}
					while (true) {
						cout << "Enter the ID of the process that you want to terminate: " << endl;
						cin >> procID;
						if (procID < vectorManager.getProcessesNumber()) {
							Process curProcess = vectorManager.getProcess(procID);
							if (curProcess.active) {
								curProcess.updateProcessStatus(false);
								break;
							}
							else
								cout << "The process ID that you've entered, belongs to a terminated process.\n";
						}
						else
							cout << "The process ID that you've entered does not exist.\n";
					}
					cout << endl;
					break;
				}
				else if (inputCmd == 'a') {
					cout << "Available terminated processes are:\n";
					for (int i = 0; i < vectorManager.getProcessesNumber(); i++) {
						if (!vectorManager.getProcess(i).active)
							cout << "Process " << i << endl;
					}

					int procID;
					while (true) {
						cout << "Enter the ID of the process that you want to terminate: " << endl;
						cin >> procID;
						if (procID < vectorManager.getProcessesNumber()) {
							Process curProcess = vectorManager.getProcess(procID);
							if (!curProcess.active) {
								curProcess.updateProcessStatus(true);
								break;
							}
							else
								cout << "The process ID that you've entered, belongs to an active process.\n";
						}
						else {
							cout << "The process ID that you've entered does not exist.\n";
						}
					}
					cout << endl;
					break;
				}
				else if (inputCmd == 'x')
					break;
				else
					cout << "Please enter a valid command, (c/t/a/x)(C/T/A/X).\n";
			}
		}
		else if (tolower(inputCmd) != 'n')
			cout << "Please enter a valid answer, (y/n)(Y/N).\n";
	}
}