#include "hermes.hh"
#include "apolo.hh"

int console_message_manager(vector<string> &mlist, list<string> &playlist);
int socket_message_manager();
void print_songs(vector<string> &mlist);
void add_song(vector<string> &mlist, list<string> &playlist);
void print_playlist(list<string> &playlist);

int main(int argc, char *argv[])
{
	//read the args
	if ( not ( 3 == argc ) )
	{
    cerr << "Wrong number of arguments, remember to provide the server's ip with port and your username" << endl;
    return 1;
	}
	string connection_address(argv[1]);
    string user_name(argv[2]);
    //start the conection using the provided ip:port
    context my_context;
    socket canopus_socket(my_context, socket_type::xrequest);
    string ip_to_connect("tcp://");
    ip_to_connect += connection_address;
    canopus_socket.connect(ip_to_connect);
    //handshake
    message greetings("HS");
    greetings << user_name;
    canopus_socket.send(greetings);
	//get the file list
	vector<string> mlist = files_in("music");
	//vars
	list<string> playlist;
	//Display the options
	cout << "//U MUSIC PLAYER//" << endl << endl;
    cout << "Available options:" << endl;
    cout << " * (L)ist Songs" << endl;
    cout << " * (U)pload Song" << endl;
    cout << " * (A)dd Song to Playlist" << endl;
    cout << " * (P)rint the Playlist" << endl;
    cout << " * E(X)it" << endl;
    
    //POLLING PHASE
    poller deadpool;
    int console = fileno(stdin);
    //add to the poll te socket and the console input
    deadpool.add(canopus_socket, poller::poll_in);
    deadpool.add(console, poller::poll_in);
    while (true){
        if(deadpool.poll()){
            //call a function when the secket has messages to read
            if(deadpool.has_input(canopus_socket)){
                if(1 == socket_message_manager()){
                    return 0;
                }
            }
            //call a function when the console has messages to read
            if(deadpool.has_input(console)){
                if(1 == console_message_manager(mlist,playlist)){
                    return 0;
                }
            }
        }
    }
}

int console_message_manager(vector<string> &mlist, list<string> &playlist)
{
	  //Get the commands from the console
      string str_console;
      getline(cin,str_console);
      if (1 == str_console.length())
      {
		  char option = str_console.at(0); 
		  switch(option)
		  {
			case 'L':
				print_songs(mlist);
				break;
			case 'U':
				//upload_song();
				break;
			case 'A':
				add_song(mlist,playlist);
				break;
			case 'P':
				print_playlist(playlist);
				break;
			case 'D':
				playlist.pop_front();
				break;
			case 'H':
				cout << "Available options:" << endl;
				cout << " * (L)ist Songs" << endl;
				cout << " * (U)pload Song" << endl;
				cout << " * (A)dd Song to Playlist" << endl;
				cout << " * (P)rint the Playlist" << endl;
				cout << " * (H)elp" << endl;
				cout << " * E(X)it" << endl;
				break;
			case 'X':
				return 1;
				break;
			default:
				cout << "Unrecognized option: " << option << endl;
				cout << "Use (H)elp to see the available options" << endl;
			break;
		  }
	  }
      return 0;
}

int socket_message_manager()
{
	cout << "Llego mensaje..." << endl;
	return 0;
}

void print_songs(vector<string> &mlist)
{
	//update the file list and print it
	mlist = files_in("music");
	if ( 0 < mlist.size())
	{
		cout << "Available Songs:" << endl;
		int counter = 0;
		for (auto i = mlist.begin(); i != mlist.end(); ++i){
			cout << counter << ": " << *i << endl;
			counter++;
		}
	}
	else
	{
		cout << "There are no songs available." << endl;
	}
}

void add_song(vector<string> &mlist, list<string> &playlist)
{
	int song_num;
	cout << "Insert the number of the song" << endl;
	cin >> song_num;
	if (song_num > mlist.size())
	{
		cout << "The song does not exists." << endl;
	}
	else
	{
		//add a song to play list
		playlist.push_back(mlist[song_num]);
		cout << "Song Added Successfully" << endl;
	}
}

void print_playlist(list<string> &playlist)
{
	//print the playlist
	int counter = (int)'a';
	for (auto i = playlist.begin(); i != playlist.end(); ++i){
		cout << (char)counter << ": " << *i << endl;
		counter++;
	}
}

