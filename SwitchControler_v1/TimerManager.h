/*
 * TimerManager.h
 *
 *  Created on: 10 thg 2, 2017
 *      Author: MyPC
 */

#ifndef TIMERMANAGER_H_
#define TIMERMANAGER_H_

#include "GlobalConfigs.h"
#include "SysTime.h"
#include "Singleton.h"

class TimerManager : public Singleton<TimerManager>{
	public:
		TimerManager();
		virtual ~TimerManager();

};

#endif /* TIMERMANAGER_H_ */
