#include "hermes.hh"
#include "apolo.hh"
#include <zmqpp/zmqpp.hpp>

#define SNAME "server"  //server's name
#define LOG_ENABLED true	//if true shows a log in console

using namespace zmqpp;
using namespace std;

int socket_message_manager(socket &skt);
int console_message_manager();

int main(int argc, char *argv[]){
    if ( not ( 2 == argc ) ) {
        cerr << "Please, write the number of the port to serve" << endl;
        return 1;
    }
    //Starting the server
    string port_num(argv[1]);
    //initialize the connection
    context my_context;
    socket server_socket(my_context, socket_type::xreply);
    string address_to_connect("tcp://*:");
    address_to_connect += port_num;
    server_socket.bind( address_to_connect );
    cout << "system> Server started correctly, listening on port " << port_num << endl;

    //Polling Stage
    poller deadpool;
    int console = fileno(stdin);
    //add to the poll te socket and the console input
    deadpool.add(server_socket, poller::poll_in);
    deadpool.add(console, poller::poll_in);
    while (true){
        if(deadpool.poll()){
            //call a function when the secket has messages to read
            if(deadpool.has_input(server_socket)){
                if(1 == socket_message_manager(server_socket)){
                    return 0;
                }
            }
            //call a function when the console has messages to read
            if(deadpool.has_input(console)){
                if(1 == console_message_manager()){
                    return 0;
                }
            }
        }
    }
    return 0;
}


int socket_message_manager(socket &skt)
{
	message minput;
	message delivery_message;
	skt.receive(minput);
	//se separa el comando del resto del mensaje
	string command;
	minput.get(command,1);
	cout << "Llego mensaje... " << command << endl;
	return 0;
}

int console_message_manager()
{
	string str_console;
    getline(cin,str_console);
	cout << "Deberia hacer algo..." << str_console << endl;
	return 0;
}
