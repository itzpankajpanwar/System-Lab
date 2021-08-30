#include<bits/stdc++.h>
#include<iostream>
#include<thread>
#include <mutex> 
#include <unistd.h>

using namespace std;
int rem;

ofstream fi;


thread philosopher[5];   // creating 5 thread , each thread will represent a philospher 
mutex forks[5];          // creating 5 mutex ,  each mutex  will represent a philospher 

mutex outputwindow;      // mutex for deciding which thread will get output console , as output console is a common resource 
                        // cout is a stream and threads will jumble together as they actually can
                         // all output at the same time. So we'll just lock to access a shared resource 

void display_pick_and_drop(int i,int pd,int d)
{   
    outputwindow.lock();
    if(pd==0)
    {
        
        if(d==0)
            {cout<<"\n Philosopher "<<i+1<<" picked left fork \n"; fi<<"\n Philosopher "<<i+1<<" picked left fork \n";}
        else
           { cout<<"\n Philosopher "<<i+1<<" picked right fork \n"; fi<<"\n Philosopher "<<i+1<<" picked right fork \n";}
    }
    else
    {
        if(d==0)
            {cout<<"\n Philosopher "<<i+1<<" put down left fork \n"; fi<<"\n Philosopher "<<i+1<<" put down left fork \n";}
        else
            {cout<<"\n Philosopher "<<i+1<<" put down right fork \n"; fi<<"\n Philosopher "<<i+1<<" put down right fork \n";}
    }
    outputwindow.unlock();
}

 
void thread_function(int i)   // thread function which will be called on creation of a thread 
{
    outputwindow.lock();
    cout<<"\n Philosopher "<<i+1<<" is thinking \n";
    fi<<"\n Philosopher "<<i+1<<" is thinking \n";
    outputwindow.unlock();

    if(i!=4)                        // condition to avoid deadlock  
        {                           //all philosophers ( except last philospher ) will pick left and then right fork 
                                    // last one will pick right and then left fork         
            forks[(i)%5].lock();
            display_pick_and_drop(i,0,0);
             
            forks[(i+1)%5].lock();
            display_pick_and_drop((i),0,1);
        }
    else 
        {  
            forks[(i+1)%5].lock();
            display_pick_and_drop(i,0,1);

            forks[(i)%5].lock();
            display_pick_and_drop(i,0,0);
        }   

    outputwindow.lock();
    cout<<"\n Philosopher "<<i+1<<" started eating  \n";
    fi<<"\n Philosopher "<<i+1<<" started eating  \n";
    outputwindow.unlock();

    usleep(1200);           

    forks[(i)%5].unlock();          // releasig locks aquired by current philosopher as his work is done 
    display_pick_and_drop(i,1,0);

    forks[(i+1)%5].unlock();  // // releasig locks aquired by current philosopher as his work is done
    display_pick_and_drop(i,1,1);

    outputwindow.lock();
    cout<<"\n Philosopher "<<i+1<<" finished eating \t remaing philosopher to eat : "<<--rem<<"\n"; 
    fi<<"\n Philosopher "<<i+1<<" finished eating \t remaing philosopher to eat : "<<rem<<"\n";    
    outputwindow.unlock();

    return;
}

int main()
{      

    fi.open("output.txt");
    cout<<"\n\n                             DINIG PHILOSOPHERS PROBLEM IMPLEMENTATION \n";
    fi<<"\n\n                             DINIG PHILOSOPHERS PROBLEM IMPLEMENTATION \n";

    cout<<"\n No Of philosophers : 5 ( already specified in question ) \n" ;
    fi<<"\n No Of philosophers : 5 ( already specified in question ) \n" ;
    int r=0;
    while(r++<5)
        {   
            rem=5;
            cout<<"                            ROUND "<<r<<" \n";
            fi<<"                            ROUND "<<r<<" \n";
            rem =5;
            cout<<"\n-------------------------------------------------------------------------------------------------\n";
            fi<<"\n-------------------------------------------------------------------------------------------------\n";
            for(int i=0;i<5;i++)            // creaitng philosopher threads 
                philosopher[i] = thread(thread_function, i);  
            
            for (int i=0;i<5;i++)   // Join will block our main thread, and so the program won't exit until  everyone every philosopher finish eating .
                philosopher[i].join();
            cout<<"\n-------------------------------------------------------------------------------------------------\n";
            fi<<"\n-------------------------------------------------------------------------------------------------\n";
        }
    
    return 0;
}
