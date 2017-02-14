/*
 * TaskManager.h
 *
 *  Created on: 13 thg 2, 2017
 *      Author: MyPC
 */

#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "SysTime.h"

#include "GlobalConfigs.h"
#include "Singleton.h"

#define MAX_TASKS	50;

typedef enum TimerMode_t { Repeat, Once} TMode;
typedef struct __attribute__((packed)){
	char name[16];
	TMode mode;
	void (*func)();
	bool (*conditionFunc)();
	unsigned long timer;
	unsigned long nextTime;
	bool isRunning;
} TTask;

class TaskManager : public Singleton<TaskManager> {
	public:
		TaskManager();
		virtual ~TaskManager();
		virtual void addTask(String name, void (*func)(), bool (*conditionFunc)(), TMode mode, unsigned long timer = 0);
		virtual void addTask(String name, void (*func)(), bool (*conditionFunc)(), unsigned long timer = 0);
		virtual void addTask(String name, void (*func)(), TMode mode, unsigned long timer = 0);
		virtual void addTask(String name, void (*func)(), unsigned long timer = 0);
		virtual void loop();
		virtual void startTasks();
		virtual void pauseTasks();
		virtual void resumeTasks();
		virtual void stopTasks();
		virtual void restartTasks();
	protected:
		void insertTask(TTask &);
	private:
		int _totalTasks, _runTask;
		bool _isTasksRunning;
		TTask *_tasks;
};

#endif /* TASKMANAGER_H_ */
