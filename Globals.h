#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include "manageProcessVector.h"
#include<ctime>
#include <sys/timeb.h>

using namespace std;

// port to connect all sockets on
#define PORT 8888 
// Time out
#define TIMEOUT_MS 200
// Constant size of reciving buffer
#define BUFFER_LEN 1024

static unsigned long long startTime_MS;

// Class object to access process vector
static manageProcessVector vectorManager;

// Class object for inter-process communication
static CommunicationManager commManager;

static long long getTimeNow() {
	//unsigned long long time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	clock_t time = clock();
	return time;
}
