#ifndef SCHEDULER_H
#define SCHEDULER_H
/*************/
/*
 * File: Scheduler.h
 * Author: Andres Rebeil
 * NetID: andres07
 * Date: November 21st, 2017
 *
 * Description: This class implements all the functionality for the scheduling algorithms.
 */
 /**************************************************************************************************/
#include <vector>
#include <queue>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <cstdio>
#include <algorithm>
#include "node.h"
#include "net.h"
#include "block.h"

/**************************************************************************************************/
using namespace std;

class Scheduler { 
	
	public:
		//Default Constructor
		Scheduler();

		//Perform all the steps of Force Directed Scheduling on current block's nodes
		void forceDirectedScheduling(Block* block);

		
};
 
#endif //SCHEDULER_H
