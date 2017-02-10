/*
 * TimerManager.cpp
 *
 *  Created on: 10 thg 2, 2017
 *      Author: MyPC
 */

#include "TimerManager.h"

bool true_condition() {
	return true;
}

TimerManager::TimerManager() {
	// TODO Auto-generated constructor stub
	NtpServerName = "pool.ntp.org";
	PacketBuffer = (byte*)malloc(NTP_PACKET_SIZE);
	LastSync = 0;
	ResyncPeriod = 0;

	_lastCheckedJobId = -1;
	_workTime = 0;
}

TimerManager::~TimerManager() {
	// TODO Auto-generated destructor stub
	if ( PacketBuffer ) free(packetBuffer);
}

void TimerManager::addJob(char* name, void (*func)(), bool (*conditionFunc)(), TMode mode, time_t time, unsigned long timer, int priority) {
	TJob job = {name, func, true_condition, mode, timer, time, priority, true};
	insertJob(job);
}

void TimerManager::insertJob(TJob& job) {
	_count++;
	TJob *jobs = new TJob[_count];
	int runJobs = new int[_count];
	if (_count > 1)
		for (long long i = 0; i < _count - 1; i++){
			jobs[i] = _jobs[i];
			runJobs[i] = _runJobs[i];
		}
	jobs[_count - 1] = job;
	runJobs[_count - 1] = _runJobs[_count - 1];
	delete _jobs;
	delete _runJobs;
	_jobs = jobs;
	_runJobs = runJobs;
	sortRunJobs();
}

void TimerManager::sortRunJobs(){
	int temp;
	for(int i = 0;i < _count - 1; i++)
		for(int j=i+1; j<= _count; j++)
			if(_runJobs[i] >= _runJobs[j]){
				temp = _runJobs[i];
				_runJobs[i] = _runJobs[j];
				_runJobs[j] = temp;
			}
}

void TimerManager::update() {
	for ( int i = 0;  i < _count; i++) {
		TJob job = _jobs[_runJobs[i]];
		unsigned long curr = millis();
		time_t curT = now();
		if(job.isRun){
			if((job.mode == Repeat)&&(curr >= job.atTime)){
				if(job.conditionFunc()){
					job.func();
					job.atTime = curr + job.timer;
				}
			}else if((job.mode == Timer) && hour(curT)==hour(job.atTimeT) && minute(curT)==minute(job.atTimeT)){
				if(job.conditionFunc()){
					job.func();
					job.atTimeT = curr + job.time;
				}
			}else if((job.mode == Once)&&(curr >= job.atTime)){
				if(job.conditionFunc()){
					job.func();
					job.atTime = curr + job.timer;
				}
			}
		}
	}
}

void TimerManager::increaseWorkTime(const int delta) {
	_workTime += delta;
}

bool TimerManager::autoSync(time_t ifSyncOK, time_t ifSyncFailed) {
	if ( now() > ResyncPeriod ) {
	if ( sync() ) {
	ResyncPeriod = now() + ifSyncOK;
	}
	else {
	ResyncPeriod = now() + ifSyncFailed;
	return false;
	}
	}
	return true;
}

void TimerManager::setNTPServer(const char* server) {
	if ( server==NULL || !server[0]) return;
	NtpServerName = server;
}

bool TimerManager::sync() {
}

bool TimerManager::sendNTPpacket(WiFiUDP& udp) {
}


void TimerManager::startFirstJob() {
	_lastEndTime = millis();
}

void TimerManager::restart() {
	_lastCheckedJobId = -1;
}
