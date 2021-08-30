#include<bits/stdc++.h>

using namespace std;

int main(int argc, char const *argv[])
{   
    if(argc !=3 )  //as we need 3 arguments passed to program so if argument count is not equal to 3 then error message
    {
        cout<<"\n incorrect format of paramter passed to the program ";
        cout<<"correct format is : <./a.out> <IP_ADDR> <PORT> \n ";
        exit(0);
    }


    int sockfd , connectionError;
    struct sockaddr_in serveraddr , clientaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	   // creating a connection end point (Socket)

	if(sockfd < 0)
        {
            printf("Unable to create socket\n");
            exit(0);
        }
    else
		printf("Socket creation successful\n");

	memset(&serveraddr, '0', sizeof(serveraddr));

	/*Setting up server address variables*/
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	int PORT=atoi(argv[2]);
	serveraddr.sin_port = htons(PORT);

	connectionError = connect(sockfd , (struct sockaddr*)&serveraddr , sizeof(serveraddr));	// connecting socket to server

	if(conError == -1)
        {
            printf("unable to connect to server");
            exit(0);
        }
    else 
        printf("connection to server established\n");


    int choice;
    string sent_message,server_reply;
    char decision=y;

    //take input of IP/domain from users
    while(decision==y)
    {

        cout<<"\n ----------------------------------------------";
        cout<<"\n1.find doamin name from IP address ";
        cout<<"\n2. find IP address from domain name ";
        cout<<"\n ----------------------------------------------";
        cout<<"\n\n Enter your choice : ";
        cin>>choice;

        if(choice==1){
            cout<<"\n enter IP address : ";
            cin>>sent_message;
        }
        else{
            cout<<"\nenter domain name : ";
            cin>>sent_message;
        }

        write(sockfd, sent_message, sizeof(msg));		//send this IP/domain to proxy server				
	    
        read(sockfd, server_reply, sizeof(msg2));	   //receving back from server 
        
        // dispaly result 
        cout<<"reply from server is : ";
        cout<<"server reply ";

        cout<<"\n Do you want to continue (y/n) ";
        cin>>decision;
    }

    // close connection 
    close(sockfd);
    cout<<"connection terminated successfully ";
    return 0;
}

