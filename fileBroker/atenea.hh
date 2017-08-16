#ifndef _ATENEA_HH_
  #define _ATENEA_HH_

#include <unistd.h>
#include <climits>

class Server {
  protected:
    string ip;
    long long int max_size; //bytes
    string status; //online, offline, busy, ready
    long long int available_space; //bytes
    vector<string> file_list;

  public:
    Server(string ip, long long int max_size);
    bool isAvailable();
    long long int availableSpace();
    void changeStatus(string new_status);
    string getAddress();
    float getCharge();
    void addFile(string filename, long long int size );
    bool fileExists(string filename);
    string getFilesOfUser(string username);
    void deleteFile(string fname);


};

string selectServer(string file, string size,vector<Server> &servers);
void update_upload(string server_id, string file, string size,vector<Server> &servers);
string locate_file(string filename,vector<Server> &servers);
string files_of_user(string user,vector<Server> &servers);
void delete_file(string fname,vector<Server> &servers);

#endif