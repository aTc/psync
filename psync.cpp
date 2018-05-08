// psync
// commit filesystem caches to disk, and give an indication of the progress.
// shows the amount of data that hasn't been flushed yet.
//
// aTc <aTc@k-n-p.org>


#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std;

bool done=false;


void sync_progress()
{
    int togo=0;
    
    cout.precision(2);
    cout << std::fixed;

    while(!done)
    {
        ifstream infile("/proc/meminfo");
        string line;
        while (getline(infile,line))
        {

            if(line.find("Dirty:") != string::npos)
            {
                stringstream str(line);
                string s1,s2;
                str >> s1 >> togo >> s2;
                
                cout << "\r" << togo/1024.0 << " MB left   ";
                cout.flush();
                break;
            }   
        }
    infile.close();
    this_thread::sleep_for(chrono::seconds(1));
    }
   
}


int main()
{
    std::thread syncthread(sync_progress);

    sync();
    done=true;
    syncthread.join();
    cout << "\rSynced                    " << endl;
}
