#include <bits/stdc++.h>
#include<thread>
#include<mutex>
#include<unistd.h>

using namespace std;

thread philosopher[5];
std::mutex forks[5];

void thread_function(int n)
{
	cout<<"Philosopher "<<n+1<<"	is Thinking!!!";
    if(n%5<4)
    {
    	forks[(n+5)%5].lock();
    	forks[(n+6)%5].lock();
	}
	else
	{
    	forks[(n+6)%5].lock();
    	forks[(n+5)%5].lock();
	}

	cout<<"Philosopher "<<n+1<<"	is Eating!!!";
	cout<<"Philosopher "<<n+1<<"	finished eating!!!";

	forks[(n+6)%5].lock();
    forks[(n+5)%5].lock();

}

int main()
{
    for(int i=0;i<5;i++)
    {
    philosopher[i]=thread(thread_function,i);
    }
    for(int i=0;i<5;i++)
    {
    philosopher[i].join();
    }

    return 0;
}
