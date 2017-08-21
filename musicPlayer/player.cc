#include "hermes.hh"
#include "apolo.hh"

void print_songs(vector<string> &mlist);
void add_song(vector<string> &mlist, list<string> &playlist);
void print_playlist(list<string> &playlist);

int main(int argc, char *argv[]){
	//get the file list
	vector<string> mlist = files_in("music");
	//vars
	list<string> playlist;
	char option=' ';
	//Display the options
	cout << "//U MUSIC PLAYER//" << endl << endl;
    cout << "Available options:" << endl;
    cout << " * (L)ist Songs" << endl;
    cout << " * (U)pload Song" << endl;
    cout << " * (A)dd Song to Playlist" << endl;
    cout << " * (P)rint the Playlist" << endl;
    cout << " * E(X)it" << endl;
    
    do
    {
      //Read the selected Option
      cout << "> ";
      cin >> option;

      switch(option){
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
        case 'X':
            return 0;
            break;
        default:
            cout << "Unrecognized option: " << option << endl;
        break;
      }
    } while (option!='X');
    
}

void print_songs(vector<string> &mlist){
	//update the file list and print it
	mlist = files_in("music");
	int counter = 0;
	for (auto i = mlist.begin(); i != mlist.end(); ++i){
		cout << counter << ": " << *i << endl;
		counter++;
	}
}

void add_song(vector<string> &mlist, list<string> &playlist){
	int song_num;
	cout << "Insert the number of the song" << endl << "> ";
	cin >> song_num;
	//add a song to play list
	playlist.push_back(mlist[song_num]);
}

void print_playlist(list<string> &playlist){
	//print the playlist
	int counter = (int)'a';
	for (auto i = playlist.begin(); i != playlist.end(); ++i){
		cout << (char)counter << ": " << *i << endl;
		counter++;
	}
}

