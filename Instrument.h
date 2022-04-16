
#ifndef INSTRUMENT
#define INSTRUMENT

#include <map>
#include <string>
#include <fstream>
#include <mutex>
#include <ctime>
#include <random>
#include <thread>
#include <memory>




using namespace std;


class Instrument {
private:

    // static so that stats collected in one thread
    static map<string, int> keyToCounter;
    static map<string, int> previousCounterValue;
    static string file;
    static mutex m;
    static unique_ptr<thread> statsThread;

    
public:
    Instrument(){
        m.lock();
        file = "stats.out";
        Instrument* ptr = this;
        // if it hasn't been started start it
        if(!statsThread) {
            thread* tmp = new thread(&Instrument::writeStats);
            statsThread.reset(tmp);
        }
        m.unlock();
    }

    ~Instrument(){
        statsThread->~thread();

    }

    static void writeStats(){
        while (true){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            dumpStats();
        }
    }

    static void dumpStats(){
        m.lock();
        ofstream openedFile;
        openedFile.open(file, std::ios_base::app);
        openedFile << "StatsDump:" << std::time(0) << "\n";
        for(std::map<string,int>::iterator it = keyToCounter.begin(); it != keyToCounter.end(); it++){
           // openedFile << "Value:" << it->first << "=" << it->second << "\n";
           openedFile << "Rate Per Second: " << it->first << "=" << it->second - previousCounterValue[it->first] << "\n";
           previousCounterValue[it->first]  = it->second;
        }
        openedFile.close();
        m.unlock();
    }        

    void IncrementKey(string key){
       m.lock();
       keyToCounter[key]++;
       m.unlock();
    }
};

map<string, int>     Instrument::keyToCounter;
map<string, int>     Instrument::previousCounterValue;
string               Instrument::file;
mutex                Instrument::m;
unique_ptr<thread>   Instrument::statsThread;





#endif


