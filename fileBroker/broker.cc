#include "hermes.cc"
#include "atenea.hh"

// formating the code: clang-format -i server.cc

/*
It is capable of
recieving incomming connections of a client
and send to the apropiate server
*/
int main(int argc, char *argv[])
{
  vector<Server> servers;
  // initialize the context (blackbox)
  context sbox;
  context cbox;
  // generate a reply socket
  socket socket_clients(cbox, socket_type::reply);
  socket socket_servers(sbox, socket_type::pull);
  // bind to the socket
  socket_clients.bind("tcp://*:4242");
  socket_servers.bind("tcp://*:4243");
  
  cout << "Remember the port to the clients is 4242" << endl;
  cout << "Remember the port to the servers is 4243" << endl;
  //string with a list of the uploaded filenames
  string files;

  //definition of a poller
  poller mein_poll;
  int console = fileno(stdin);
  //added to the poller the stdin and the sockets
  mein_poll.add(socket_clients, poller::poll_in);
  mein_poll.add(socket_servers, poller::poll_in);
  mein_poll.add(console, poller::poll_in);
  //the endless history (loop)
  while (true){
      if(mein_poll.poll()){
          //read a message from the sockets
          if(mein_poll.has_input(socket_clients)){
            string username, cmd,filename,size;
            message mc;//hammer!
            socket_clients.receive(mc);
            //the message sintax is:
            //username, command, filename, size
            mc >> username;
            mc >> cmd;
            if (cmd == "list"){
              string filelist;
              filelist = files_of_user(username,servers);
              socket_clients.send(filelist);
            }
            if(cmd == "upload"){
              mc >> filename;
              mc >> size;
              string file;
              file = username + "_uploaded_" + filename;
              //check if the file exists
              if(not ("null" == locate_file(file,servers))){
                //existe!
                socket_clients.send("repeated");
              }else{
                cout << "User " << username << " is trying to upload " << size << " bytes of the file " << filename << endl;
                //select the best server
                string server_ip = selectServer(file, size,servers);
                if (server_ip == "null")
                {
                  socket_clients.send("no ok :(");
                }else{
                  clean_message(mc);
                  mc << "ok" << server_ip << file;
                  socket_clients.send(mc);
                  //update the list of servers
                  update_upload(server_ip, file, size,servers);
                }
              }
            }
            if (cmd == "download"){
              mc >> filename;
              string file;
              file = username + "_uploaded_" + filename;
              string location_ip = locate_file(file,servers);
              if("null" == location_ip){
                //no exists!
                socket_clients.send("notexists");
              }else{
                //file exists
                clean_message(mc);
                mc << "ok" << location_ip << file;
                socket_clients.send(mc);
              }
            }
            if (cmd == "erase"){
              mc >> filename;
              string file;
              file = username + "_uploaded_" + filename;
              //check if the file exists
              string location_ip = locate_file(file,servers);
              if("null" == location_ip){
                //no exists!
                socket_clients.send("notexists");
              }else{
                //file exists
                clean_message(mc);
                mc << "ok" << location_ip << file;
                socket_clients.send(mc);
                //delete from the database
                delete_file(file,servers);
              }
            }

          }
          if(mein_poll.has_input(socket_servers)){
            string server_ip, cmd,filename,max_size;
            message ms;//hammer!
            socket_servers.receive(ms);
            //the message sintax is:
            //username, command, ip address
            ms >> server_ip;
            ms >> cmd;
            if (cmd == "connect"){
              ms >> max_size;
              long long int msize = string_to_big_number(max_size);
              cout << "the server " << server_ip << " is now online"<< endl;
              Server new_server(server_ip,msize);
              servers.push_back(new_server);
            }
            if (cmd == "busy"){
              cout << "the server " << server_ip  << " is now in a busy state"<< endl;
            }
            if (cmd == "ready"){
              cout << "the server " << server_ip  << " is now ready"<< endl;
            }
            if (cmd == "disconnect"){
              cout << "the server " << server_ip  << " is now offline"<< endl;
            }
          }
          //if the stdin has messages
          if(mein_poll.has_input(console)){
              //do something
            }
        }
    }
}

string selectServer(string file, string size, vector<Server> &servers){
  float lowest_charge = 100; //percent of charge
  long long int nsize = string_to_big_number(size);
  string selected_ip("null");
  for (vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
  {
    Server &serv_i = *it;
    if (serv_i.isAvailable() and nsize <= serv_i.availableSpace()){
      //take every available server with enough space to save the file
      //and select te ip (only if the charge is lower)
      float charge = serv_i.getCharge();
      if (charge < lowest_charge){
        selected_ip = serv_i.getAddress();
        lowest_charge = charge;
      }
    }
  }
  //after look all the servers return the selected ip
  return selected_ip;
}

void update_upload(string server_ip, string file, string size,vector<Server> &servers){
  for (vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
  {
    Server &serv_i = *it;
    if (serv_i.getAddress() == server_ip)
    {
      //find the correct server, and update the info
      long long int nsize = string_to_big_number(size);
      serv_i.addFile(file,nsize);
    }
  }
}

string locate_file(string filename,vector<Server> &servers){
  string my_ip("null");
  for (vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
  {
    Server &serv_i = *it;
    if (serv_i.fileExists(filename)){
      my_ip = serv_i.getAddress();
    }
  }
  return my_ip;
}

string files_of_user(string username,vector<Server> &servers){
  string files("");
  for (vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
  {
    Server &serv_i = *it;
    files = files + serv_i.getFilesOfUser(username);
  }
  return files;
}

void delete_file(string fname,vector<Server> &servers){
  for (vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
  {
    Server &serv_i = *it;
    serv_i.deleteFile(fname);
  }
}


//-------------------------
// Class functions
//-------------------------


Server::Server(string ip, long long int max_size) :ip(ip),max_size(max_size) {
  status = "ready";
  available_space = max_size;
}

string Server::getAddress(){
  return ip;
}

bool Server::isAvailable(){
  return ("ready" == status);
}

long long int Server::availableSpace(){
  return available_space;
}

float Server::getCharge(){
  float charge;
    charge = 100 - (available_space*100/max_size);
  return charge;
}

void Server::addFile(string filename, long long int size ){
  file_list.push_back(filename);
  available_space -= size;
}

bool Server::fileExists(string filename){
  bool exist(false);
  for (string fname : file_list)
  {
    exist = exist or (filename == fname);
  }
  return exist;
}

string Server::getFilesOfUser(string username){
  string files("");
  for (string fname : file_list)
  {
    vector<string> parts;
    parts = explode(fname,"_");
    string uname = parts[0];
    string partial_name = parts[1] +"."+ parts[2];
    if(uname == username){
      //the file belongs to the user
      files = files + "\n" + partial_name;
    }
  }
  return files;
}

void Server::deleteFile(string fname){
  vector<string>::iterator to_delete;
  for (vector<string>::iterator it = file_list.begin(); it != file_list.end(); ++it)
  {
    string &file_i = *it;
    if (file_i == fname){
    to_delete= it;
    }
  }
  file_list.erase(to_delete);
}