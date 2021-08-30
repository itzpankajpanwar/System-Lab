#include<bits/stdc++.h>

vector< pair<string,string> > proxy_cache;   //creating proxy cache to stroe temporary data at proxy server side

using namespace std;


int check_type_of_reqest(string s)
{
    int type=1;   
    for(auto &x: s)
    {
        if(x=='@')  // it means that if string contiands @ then it will surely be domain name
        { type = 2;
          break;
        }
    }
    return type;
}

stirng fetch_result_from_dns_server()
{   
    strign res;

    return res;
}

int main(int argc, char* argv[]){

    //establish all connection 


    //after receving request from client 

    vector< pair<string,string> > proxy_cache;   //creating proxy cache to stroe temporary data at proxy server side

    int request_type = check_type_of_reqest(arg);  // for identification of tye of service 

    if(request_type==1)   // i.e client require ip address to domain name conversion 
    for(auto & x : proxy_cache)
    {
        if(x.first == arg1 )
            return x.second;  // return back to client 
    }

    else   //i.e client require domain name to ip conversion 
    {
        for(auto & x : proxy_cache)
        {
            if(x.second == arg1 )
                return x.first;  // return back to client 
        }
    }

    // if here that means that required request is not present in proxy server cache 

    // call DNS server 
    
    string res = fetch_result_from_dns_server();


    //close all connections

    return 0;
}