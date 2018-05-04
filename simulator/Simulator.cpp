// Compile the code with the following shell command:
// g++ -std=c++14 ActivationQueueDemo.cpp -o ActivationQueueDemo
// Mac-users will need Xcode command line tools to compile this. Additionally Xcode project can be used if someone prefers.
// This code does not do anything yet, but just is an empty skeleton.
// Example code by Karoliina Salminen

#include <queue>
#include <iostream>
#include <tuple>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <map>
#include <thread>
#include <unistd.h>

unsigned long timesteps=0;

class Point3 {
public:
    Point3(): x(0), y(0), z(0) { }
    Point3(int xx, int yy, int zz): x(xx), y(yy), z(zz) { }
    Point3(std::tuple<int, int, int> p) { x = std::get<0>(p); y = std::get<1>(p); z = std::get<2>(p); }
    bool operator ==(const Point3& other){
        if(x==other.x && y == other.y && z == other.z) return true;
        return false;
    }
    std::tuple<int,int,int> asTuple(){ 
        return std::make_tuple(x,y,z); 
    }
    int * getData(){
        int *dataPointer = &x;
        return dataPointer;
    }

    void *cachedReference = NULL;
    int x = 0;
    int y = 0;
    int z = 0;
};

class DataPackage {
public:
    DataPackage() { }
    DataPackage(Point3 toAddress, float ip, float dly, void *pointer):
        p(toAddress), inputParameter(ip), delay(dly), directPointer(pointer), empty(false) { }

    DataPackage(Point3 toAddress, float ip, float dly):
        p(toAddress), inputParameter(ip), delay(dly), directPointer(NULL), empty(false) {}

    bool decrement(){ 
        delay--;
        if(delay<1){
            return false;
        } else {
            return true;
        }
    }

     bool noDelayLeft(){
        if(delay==0){
            return true;
        }
        return false;
    }
    // This is sort of reference counting
    void step(){
        delay--;
    }

    bool isEmpty(){
        return empty;
    }

    bool empty=true;
    Point3 p;
    float inputParameter = 0.0f;
    float delay = 0.0f;
    void *directPointer = NULL;
};

template<typename T, typename Container=std::deque<T> >
class iterable_queue : public std::queue<T,Container>
{
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    void erase(typename Container::iterator it){this->c.erase(it); }
    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
    const_iterator begin() const { return this->c.begin(); }
    const_iterator end() const { return this->c.end(); }
};


class ActivationQueue {
public:
    ActivationQueue(){}

    void push(DataPackage& pkg) {
        q.push(pkg);
    }

    DataPackage pop() {
        DataPackage p;
        p.empty=true;
        bool found=false;
        if (!q.empty()) {
            p = q.front();
            if(p.noDelayLeft() == true){
                // Conductance delay consumed to end, pop it
                q.pop();
                found=true;

                // Case p is returned as is
            } else {
                // Find another item from the queue that could be potentially popped
                
                // Iterate through the queue
                for(auto it=q.begin();it!=q.end();it++){
                    // Check if no delay is left, this means the item can be popped
                    if(it->noDelayLeft() == true){
                        p = *it;
                        p.empty=false;
                        // The item can not be popped because it is in the middle, so it is erased with the erase
                        q.erase(it);
                        found = true;
                        // One thread picks only one item at a time
                        break;
                    }

                }
                

            }
        }
        if(found==true){
            printf("pop found\n");
            p.empty=false;
        } else {
            p.empty=true;
        }
        return p;
    }

    // Step function steps the queue one timestep forwards, this decrements the conductance delays on the items on the queue
    void step()
    {
        for(auto it=q.begin();it!=q.end();it++){
            it->step();
        }
        timesteps++;
    }

    iterable_queue<DataPackage> q;

};


class SimpleQueue {
public:
    void push(std::pair<Point3, float> address) {
        q.push(address);
    }

    std::pair<Point3, float> pop() {
        std::pair<Point3, float> p;

        if (!q.empty()) {
            p = q.front();
            q.pop();
        }

        return p;
    }

    std::queue<std::pair<Point3, float>> q;

};

// Global declaration of the queue

ActivationQueue activationQueue;
#define MAX_THREAD 8
// Global declaration of the thread pool
std::vector<std::thread> threadPool;

bool readyQueue[MAX_THREAD]={false};


void execThreadFunc(int id) {
    int threadid = id;
    readyQueue[id]=false;


    while (1) {
        while(readyQueue[id]!=false){
            //usleep(0);
        }
        //std::cout << "Thread running " << id << std::endl;
        printf(".");
        DataPackage package = activationQueue.pop();
        printf(",");
        readyQueue[id]=true;
        printf("_");
        usleep(0);
    }
}

void syncThreadFunc() {
    unsigned long totalEvents =0;
    while(1){
        short ready=0;
        for(int i=0;i<MAX_THREAD;i++){
            if(readyQueue[i]==true) ready++;
        }
        if(ready==MAX_THREAD){
            printf("%lu\n",timesteps);
            // Do timestep advance
            activationQueue.step();
            // Clear ready queue
            for(int i=0;i<MAX_THREAD;i++){
                readyQueue[i]=false;
            }
            totalEvents++;
        } else {
            //printf("Waiting for threads to finish. %d/%d finished.\n",ready,MAX_THREAD);
            //printf(" ");
            
            printf("Sync count =%lu, %d/%d finished.\n",totalEvents,ready,MAX_THREAD);
        }
        usleep(0);
    }
}


// Indexes neurons according to the drawing
class NeuronIndex {
public:

};

// Maps between which neurons which synapse belongs to
class SynapseIndex {

};

// NeuronStorage decouples the locations and storage.
class NeuronStorage {
public:
    // Use of void * avoids the cyclic dependency that would be otherwise introduced
    std::vector<void *> neurons;

};

// Global instantiation of the storage array of neurons (flat storage container that does not hold the structure of the neurons, to allow neurons to have non-fixed locations)
NeuronStorage ns;

class Synapse {
public:
    void *pre = NULL;
    void *post = NULL;
    float weight = 0.5f;
    float conductanceDelay = 1.0f;
};

class Neuron {
public:

    void appendMyselfToStorage() {
        void *p = (void*) this;
        ns.neurons.push_back(p);
    }
    std::vector<void *> synapses; // Pointer array to synapse storage


};

class ConnectomeStorage {
public:
    std::vector<void *> synapses;
};

int main() {
    std::cout << "Hello.... Real time Neurosimulator v 0.1.\n";
    //std::pair<Point3,float> activation = std::make_pair<Point3,float>(Point3(-25,196,248),0.4);
    //activationQueue.push(activation);
    //std::pair<Point3,float> poppedActivation = activationQueue.pop();

    // Create the worker threads for the SNN
/*
    DataPackage package(Point3(-25,196,248),0.5,4); // delay 5 ms
    activationQueue.push(package);
{
    DataPackage popped = activationQueue.pop();
    if(popped.isEmpty()==true){
        printf("Empty as expected\n");
    } else {printf("Not empty\n");}
}
activationQueue.step();
{
    DataPackage popped = activationQueue.pop();
    if(popped.isEmpty()==true){
        printf("Empty as expected\n");
    }
}
activationQueue.step();
{
    DataPackage popped = activationQueue.pop();
    if(popped.isEmpty()==true){
        printf("Empty as expected\n");
    }
}
activationQueue.step();
{
    DataPackage popped = activationQueue.pop();
    if(popped.isEmpty()==true){
        printf("Empty as expected\n");
    }
}
activationQueue.step();
{
    DataPackage popped = activationQueue.pop();
    if(popped.isEmpty()==false){
        printf("Not empty as expected\n");
    }
}*/

    for (int i = 0; i < MAX_THREAD; i++) {
        threadPool.push_back(std::thread(execThreadFunc, i));
    }

    std::thread syncThread(syncThreadFunc);


    for (int i = 0; i < threadPool.size(); i++) {
        threadPool[i].join();
    }
    syncThread.join();

    


    return 0;
}
