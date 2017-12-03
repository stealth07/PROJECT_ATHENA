/*
* File: Scheduler.cpp
* Author: Andres Rebeil
* NetID: andres07
* Date: November 21st, 2017
*
* Description: This file .cpp file contains the implementation of the Scheduler class
*/
/**************************************************************************************************/
#include "scheduler.h"
/**************************************************************************************************/

//Default Constructor
Scheduler::Scheduler()
{
	
}

/**************************************************************************************************/
void Scheduler::forceDirectedScheduling(Block* block){
    while(fdsNotDone){
        updateTimeFrame(block);
        updateDistributions(block);
        updateSelfForce(block);
        updatePredecessorForces(block);
        updateSucessorForces(block);
        scheduleNode(block);
        block->printSchedulingInfo();
    };
}
bool Scheduler::updateTimeFrame(Block* block){
    asapSchedule(block);
    determineAlapSchedule(block);
    for(unsigned int i = 0; i< block->nodeVector.size(); i++){
        block->nodeVector.at(i)->updateFrameParams();        
        if(block->nodeVector.at(i)->asapTime==block->nodeVector.at(i)->alapTime){
            block->nodeVector.at(i)->scheduleTime = block->nodeVector.at(i)->asapTime;
        }
    }
    return true;
}
bool Scheduler::updateDistributions(Block* block){
    //clear distribution and update by new probabilities
    aluDistribution.clear();
    multDistribution.clear();
    divModDistribution.clear();
    aluDistribution.push_back(0.0);
    multDistribution.push_back(0.0);
    divModDistribution.push_back(0.0);

    for(int i = 1; i<=block->timeConstraint; i++){
        float aluTemp = 0.0;
        float multTemp = 0.0;
        float divModTemp = 0.0;
        for(unsigned int j = 0; j< block->nodeVector.size(); j++){
            if(block->nodeVector.at(j)->op=="MUL"){
                multTemp+=block->nodeVector.at(j)->forceData.probabilities.at(i);
            }
            else if (block->nodeVector.at(j)->op=="DIV"){
                divModTemp+=block->nodeVector.at(j)->forceData.probabilities.at(i);
            }
            else if (block->nodeVector.at(j)->op=="MOD"){
                divModTemp+=block->nodeVector.at(j)->forceData.probabilities.at(i);
            }
            else{
                aluTemp+=block->nodeVector.at(j)->forceData.probabilities.at(i);
            }
        }
        aluDistribution.push_back(aluTemp);
        multDistribution.push_back(multTemp);
        divModDistribution.push_back(divModTemp);
    }
//#if 0
	cout << endl << "All distribution: \n";
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "aluDG: ";
	for (int i = 1; i <= block->timeConstraint; i++) {
		cout << "	"<< aluDistribution.at(i);
	}
    cout<<endl;
    cout << "--------------------------------------------------------------------------" << endl;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "mulDG: ";
	for (int i = 1; i <= block->timeConstraint; i++) {
		cout << "	"<< multDistribution.at(i);
	}
    cout<<endl;
    cout << "--------------------------------------------------------------------------" << endl;
//#endif
    return true;
}

bool Scheduler::updateSelfForce(Block* block){
    for(unsigned int i = 0; i< block->nodeVector.size(); i++){
        block->nodeVector.at(i)->updateSelfForces(aluDistribution,multDistribution,divModDistribution);
//#if 0
        cout << "Node "<< i << " : ";
        for (int j = 1; j <= block->timeConstraint; j++) {
		    cout << "\t"<< block->nodeVector.at(i)->forceData.selfForces.at(j);
	    }
        cout<<endl;
//#endif
    } 
   fdsNotDone = false; // temperally stop here 
    return true;}
bool Scheduler::updatePredecessorForces(Block* block){return true;}
bool Scheduler::updateSucessorForces(Block* block){return true;}
bool Scheduler::scheduleNode(Block* block){return true;}
/**************************************************************************************************/
bool Scheduler::determineAlapSchedule(Block * block){

	if (block->timeConstraint > 0) {
		//Reset all traverse Parameters
		block->resetAll();
		block->findLastNodes();

		bool ready = false;
		Node* currentNode;
		Node* parentNode;
		Node* childNode;
		int earliestAlapTime = 0;
		int maxExecuteTime = 0;
		vector<Node*> predecessors;
		int remainingTime = block->timeConstraint;

		//Schedule the last nodes in graph first
		for (unsigned int i = 0; i < block->nodeVector.size(); i++) {
			currentNode = block->nodeVector.at(i);
			remainingTime = block->timeConstraint;

			if (currentNode->last && !currentNode->scheduled) {
				
				currentNode->alapTime = remainingTime - (currentNode->executionTime - 1);
				currentNode->marked = true;
				currentNode->scheduled = true;

				//Add all current nodes predecessors to queue
				for (unsigned int j = 0; j < currentNode->parentNodes.size(); j++) {
					parentNode = currentNode->parentNodes.at(j);
					
					//Check that parentNode is within current block
					if (std::find(block->nodeVector.begin(), block->nodeVector.end(),
						parentNode) != block->nodeVector.end()) {

						if (!parentNode->marked) {
							predecessors.push_back(parentNode);
							parentNode->marked = true;
						}
					}
				}
			}
		}
		//Schedule All the predecessors
		while (!predecessors.empty()) {
			for (unsigned int i = 0; i < predecessors.size(); i++) {
				ready = true;
				currentNode = predecessors.at(i);
				for (unsigned int i = 0; i < currentNode->childNodes.size(); i++) {
					childNode = currentNode->childNodes.at(i);
					//Ensure child is in current block
					if (std::find(block->nodeVector.begin(), block->nodeVector.end(),
						childNode) != block->nodeVector.end()) {
						if (!childNode->scheduled) {
							ready = false;
							break;
						}
					}
				}
				//Schedule current node if ready, all child nodes scheduled
				if (ready) {
					earliestAlapTime = block->timeConstraint;
					maxExecuteTime = 0;
					for (unsigned int k = 0; k < currentNode->childNodes.size(); k++) {
						childNode = currentNode->childNodes.at(k);
						if (std::find(block->nodeVector.begin(), block->nodeVector.end(),
							childNode) != block->nodeVector.end()) {
							if (childNode->executionTime >= maxExecuteTime) {
								maxExecuteTime = childNode->executionTime;
							}
							if ((childNode->scheduled)
								&& (childNode->alapTime <= earliestAlapTime)) {
								earliestAlapTime = childNode->alapTime;
							}
						}
					}
					remainingTime = (earliestAlapTime - currentNode->executionTime);
					//currentNode->alapTime = remainingTime - (currentNode->executionTime - 1);
					currentNode->alapTime = remainingTime;
					currentNode->scheduled = true;
				}
				//Add all current nodes predecessors to queue
				for (unsigned int j = 0; j < currentNode->parentNodes.size(); j++) {

					parentNode = currentNode->parentNodes.at(j);
					//Check that parentNode is within current block
					if (std::find(block->nodeVector.begin(), block->nodeVector.end(),
						parentNode) != block->nodeVector.end()) {

						if (!parentNode->marked) {
							predecessors.push_back(parentNode);
							parentNode->marked = true;
						}
					}
				}
				//If currentNode is scheduled remove it from predecessor queue
				if (currentNode->scheduled) {
					predecessors.erase(predecessors.begin() + i);
				}
			}
		}
		//Check that all scheduled Alap times are valid
		for (unsigned int i = 0; i < block->nodeVector.size(); i++) {
			currentNode = block->nodeVector.at(i);
			if (currentNode->alapTime > block->timeConstraint || currentNode->alapTime < 0)
				return false;
		}
		block->scheduled = true;
		return true;
	}
	//Scheduling failed, constraint not met
	block->scheduled = false;
	return  true;
}
/**************************************************************************************************/
bool Scheduler::asapSchedule(Block * block){
	if (block->timeConstraint <= 0) {
	    //Scheduling failed, constraint not met
	    block->scheduled = false;
        return false;
	}
	//Reset all traverse Parameters
	block->resetAll();
    
    bool scheduleNotDone = true;
    // Initialize
    for (unsigned int i = 0; i< block->nodeVector.size();i++){
        block->nodeVector[i]->output->available = false;
        block->nodeVector[i]->output->nAvailable = false;
    }

    //Test schedule time!
    //block->nodeVector.at(4)->scheduleTime = 0;

    int exec_cycle = 1;
    while(scheduleNotDone){
        vector<Node*> newScheduled;
        //executing phase
        for(unsigned int i = 0; i< block->nodeVector.size();i++){
            bool nodeAvailable = true;
            // if the node has been schedule, wait the schedule time

            nodeAvailable &= exec_cycle >= block->nodeVector.at(i)->scheduleTime;
            for(unsigned int j = 0; j< block->nodeVector.at(i)->inputs.size();j++){
                if(block->nodeVector.at(i)->inputs.at(j)){
                    nodeAvailable &= block->nodeVector.at(i)->inputs.at(j)->available;
                }
            }
            if(nodeAvailable&&
               //node available but not scheduled yet
               (block->nodeVector.at(i)->visited==false)){
                // mark node visited, push to the vector, and mark asapTime 
				block->nodeVector.at(i)->nVisited = true;
				newScheduled.push_back(block->nodeVector.at(i));
				block->nodeVector.at(i)->asapTime = exec_cycle;
                
                if(block->nodeVector.at(i)->op=="MUL"){
                    block->nodeVector.at(i)->nAsapCount++;
                }
                else if(block->nodeVector.at(i)->op=="DIV"){
                    block->nodeVector.at(i)->nAsapCount++;
                }
                else if(block->nodeVector.at(i)->op=="MOD"){
                    block->nodeVector.at(i)->nAsapCount++;
                }
                else {
                    block->nodeVector.at(i)->nMarked=true;
                    block->nodeVector.at(i)->output->nAvailable=true;
                }
            }
            else if((block->nodeVector.at(i)->visited==true)&&
                    //node scheduled but not finished yet
                    (block->nodeVector.at(i)->marked==false)){
                //complete at current cycle
                if((block->nodeVector.at(i)->op=="MUL")&&
                    (block->nodeVector.at(i)->asapCount==2)){
                    block->nodeVector.at(i)->nMarked=true;
                    block->nodeVector.at(i)->output->nAvailable=true;
                }
                else if((block->nodeVector.at(i)->op=="DIV")&&
                    (block->nodeVector.at(i)->asapCount==3)){
                    block->nodeVector.at(i)->nMarked=true;
                    block->nodeVector.at(i)->output->nAvailable=true;
                } 
                else if((block->nodeVector.at(i)->op=="MOD")&&
                    (block->nodeVector.at(i)->asapCount==3)){
                    block->nodeVector.at(i)->nMarked=true;
                    block->nodeVector.at(i)->output->nAvailable=true;
                }
                //need more cycles
                else if((block->nodeVector.at(i)->op=="MUL")&&
                    (block->nodeVector.at(i)->asapCount<2)){
                    block->nodeVector.at(i)->nAsapCount++;
                }
                else if((block->nodeVector.at(i)->op=="DIV")&&
                    (block->nodeVector.at(i)->asapCount<3)){
                    block->nodeVector.at(i)->nAsapCount++;
                }
                else if((block->nodeVector.at(i)->op=="MOD")&&
                    (block->nodeVector.at(i)->asapCount<3)){
                    block->nodeVector.at(i)->nAsapCount++;
                }
            }
        }
        //increment cycle
        exec_cycle++;
        
        //check phase
        bool done = true;
        for(unsigned int i = 0; i< block->nodeVector.size();i++){
            done &= block->nodeVector.at(i)->marked;
        }
        scheduleNotDone = !done;
        if(!done){
            //one cycle end, if there are new scheduled nodes, push to array
            asap.push_back(newScheduled);
        }

        // Then update "n"flag to flag
        //
        for(unsigned int i = 0; i< block->nodeVector.size();i++){
           block->nodeVector.at(i)->marked = block->nodeVector.at(i)->nMarked;
           block->nodeVector.at(i)->visited = block->nodeVector.at(i)->nVisited;
           block->nodeVector.at(i)->asapCount = block->nodeVector.at(i)->nAsapCount;
           block->nodeVector.at(i)->output->available = block->nodeVector.at(i)->output->nAvailable;
		   
		}

    }
return true;
}

