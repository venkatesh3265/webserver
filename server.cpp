/*
What is Socket?
  It is a communicate endpoint.It can be used to send and receive messages.
  
  clientside- it like a simple program which has to get something from server
  serverside- it take a request from client and send a response for client 
  
  //server side
  1.we need to create a socket
  2.assign with ip and port number to bind them>
  3.listen- wait for the client to approach the server
  4.accept 
    client is connected 
  


*/

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
 
using namespace std;
 
int main()
{
    char *fun = "Hello world";
    /*Create a socket
    socket( domain , type, protocol )
    domain = Af_INET(internet domain)/Af_unix(unix domain);
    type = Sock_stream(tcp) / sock_dgram-udp
    protocol = we care about ip,so it will be represented as 0
     if something goes wrong it will return negative value
    */
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }
 
    //configure server site setting
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); //this is to represent byte ordring.here host to network short is reprented as htons
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);
    // Bind the ip address and port to a socket
 /* ****************************************** */ 
    bind(listening, (sockaddr*)&hint, sizeof(hint));
     // listening = the socket file descriptor returned by socket()
     // (sockaddr*)&hint = has the information about ip address    and port information.
     // sizeof(hint) = set to sizeof().
    /*
    here,
     Bind function assign an ip and port number to a socket.
     we have created a socket , we have ip and port info
 
    */
    
 
    // Tell  the socket is for listening
    listen(listening, SOMAXCONN);
    /* syntax= int listen() ;
    listening = the socket file descriptor returned by socket();
     SOMAXCONN = the number of request allowed in the queue.
    
    
    */
 
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
 
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
   /*
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on
 
    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
 
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port! " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }
    */
 
    // Close listening socket
    close(listening);
 
    // While loop: accept and send message back to client
    char buf[4096];
 
    while (true)
    {
        //memset(buf, 0, 4096);
 
        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }
 
        if (bytesReceived == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }
 
        cout << string(buf, 0, bytesReceived) << endl;
 
        // message back to client
        
        //send(clientSocket, buf, bytesReceived + 1, 0);
        send(clientSocket,fun,strlen(fun),0);
    }
 
    // Close the socket
    close(clientSocket);
 
    return 0;
}
