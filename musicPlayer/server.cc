#include "hermes.hh"
#include "apolo.hh"
#include <zmqpp/zmqpp.hpp>

#define SNAME "μMus"  //server's name
#define LOG_ENABLED true	//if true shows a log in console

using namespace zmqpp;
using namespace std;

int socket_message_manager(vector<string> &mlist, list<string> &playlist,socket &skt);
string get_songs(vector<string> &mlist);
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
    cout << SNAME << "> Server started correctly, listening on port " << port_num << endl;
    
    //get the file list
	vector<string> mlist = files_in("music");
	//vars
	list<string> playlist;

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
                if(1 == socket_message_manager(mlist,playlist,server_socket)){
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


int socket_message_manager(vector<string> &mlist, list<string> &playlist,socket &skt)
{
	message minput;
	message delivery_message;
	string command;
	string uname;
	string ident;
	string slist;
	//get the message by parts 
	skt.receive(minput);
	minput.get(ident,0);
	minput.get(command,1);
	minput.get(uname,2);
	char option = command.at(0); 
	switch(option)
		  {
			case 'L':
				if (LOG_ENABLED) cout << uname << "> List songs" << endl;
				slist = get_songs(mlist);
				delivery_message << ident << "L" << slist;
				skt.send(delivery_message);
				break;
			case 'U':
				cout << "upload song..." << endl;
				//upload_song();
				break;
			case 'A':
				cout << "add song..." << endl;
				//add_song(mlist,playlist);
				break;
			case 'P':
				cout << "print playlist..." << endl;
				//print_playlist(playlist);
				break;
			case 'D':
				cout << "skip song..." << endl;
				//playlist.pop_front();
				break;
			case 'H':
				cout << SNAME << "> User " << uname << " connected." << endl;
				//Answer the Handshake
				delivery_message << ident << "W" << SNAME;
                skt.send(delivery_message);
				break;
			case 'X':
				return 0;
				break;
			default:
				cout << "Unrecognized option: " << option << endl;
				cout << "Use (H)elp to see the available options" << endl;
			break;
		  }
	return 0;
}

int console_message_manager()
{
	string str_console;
    getline(cin,str_console);
	cout << "Deberia hacer algo..." << str_console << endl;
	return 0;
}

string get_songs(vector<string> &mlist)
{
	string result = "";
	//update the file list and print it
	mlist = files_in("music");
	if ( 0 < mlist.size())
	{
		result  += "Available Songs:\n";
		int counter = 0;
		for (auto i = mlist.begin(); i != mlist.end(); ++i){
			result += number_to_string(counter) + ": " + *i + "\n";
			counter++;
		}
	}
	else
	{
		result = "There are no songs available.\n";
	}
	return result;
}
