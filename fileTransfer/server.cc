#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace zmqpp;

// formating the code: clang-format -i server.cc

/*
It is capable of
recieving incomming connections of a client
recieving files and save them to disk
send stored files
send a list of files stored into disk
remove files with a command
*/

void listf(socket &s, const string &files);
void uploadf(socket &s,string filename, string &files);
void downloadf(socket &s,string filename);
void erasef(socket &s, string filename, string &files);

void seek_n_destroy(string& victim, string& part);

void clean_message(message& m);
void messageToFile(const message& msg, const string& fileName);
vector<char> readFileToBytes(const string& fileName) ;
void fileToMesage(const string& fileName, message& msg);

int main(int argc, char *argv[])
{
  // initialize the context (blackbox)
  context bbox;
  // generate a reply socket
  socket s(bbox, socket_type::reply);
  // bind to the socket
  s.bind("tcp://*:4242");
  
  //string with a list of the uploaded filenames
  string files;

  //the eternal loop
  while (true)
  {
    // receive the message
    message req;
    cout << "Receiving message... ";
    s.receive(req);

    //declaration of the variables
    string option;
    string fname;
    //saving in option the received message
    req >> option;
    //print the received option
    cout << option << endl;
    //do the correspondient action to the option
    if (option == "list")
    {
      //this list the uploaded files
      const string chain(files);
      listf(s,chain);
    }else if (option == "upload")
    {
      //receive a new file, and save it
      req >> fname;
      uploadf(s,fname,files);
    }else if (option == "download")
    {
      //send a file to the client
      req >> fname;
      downloadf(s,fname);
    }else if (option == "erase")
    {
      //delete a file
      //without confirmation
      req >> fname;
      erasef(s,fname,files);
    }else{
      //the default option
      //in the case of an error in the option
      //TODO
    }
    cout << "Finished." << endl;
  }
}

//the name of the function indiques the request of the client

void listf(socket &s, const string &files)
{
  //create a message an put in it the files list
  message m;
  m << files;
  //send to the client the files list
  s.send(m);
  //report the action
  cout << "A client asked for list" << endl;
}

void uploadf(socket &s,string filename, string &files)
{
  //create a message and start a "conversation" with the client
  message m;
  s.send("Ready to recieve");
  //receive the file
  s.receive(m);
  //in m comes the file from the user, modify the filename (prefix)
  //filename = "uploaded_" + filename;
  //and put the message in a file
  messageToFile(m,filename);
  //wipe the message
  clean_message(m);

  //report the course of the actions
  cout << "Image " << filename << " saved" << endl;
  //say to the client that all is fine...
  s.send("Saved");
  //after saved the file add it to the 
  files += "\n" + filename;
}

void downloadf(socket &s,string filename)
{
 //declaration of varibles
  streampos size;
  string strMsg;

  //check if file exist
  ifstream infile(filename);
  if (!infile.good())
  {
    //if it doen't exist, give the bad news to the client
    s.send("bad");
    return;
  }//get out

  //if All Izz Well report it!
  s.send("good");
  cout << "Client downloading "<< filename << "... ";

  //receive the ACK (Acknowledgement) from the client
  message m;
  string ack;
  s.receive(m);
  m >> ack;
  //report the news from the client   
  cout << ack << "... ";
  //wipe the message
  clean_message(m);
  //put the file in the message, and send it
  fileToMesage(filename,m);
  s.send(m);
  //report to the console that all is ou-key
  cout << "ok" << endl;
}

void erasef(socket &s, string filename , string &files)
{
  //report what the hell are the client trying to do 
  cout << "Client erasing "<< filename << "... ";
  //try to delete a file and send the result to the client
  if( remove( filename.c_str() ) != 0 ){
    //report the bad news
    s.send("Error deleting file" );
    cout << "Error" << endl;
  }else{
    // proclaim the good news
    s.send("File successfully deleted" );
    cout << "Ok" << endl;
    //if ok then delete the file from the list
    filename = "\n" + filename;
    seek_n_destroy(files,filename);
  }
  return ;
}

//other functions (A.K.A. auxiliary functions)

void seek_n_destroy(string& victim, string& part)
{
  //kill 'em all
  //get the size
  size_t n = part.length();
  for (size_t i = victim.find(part);
    i != string::npos;
    i = victim.find(part))
    {
      //search and erase the part from the victim
      victim.erase(i, n);
    }
}

//wipe all the garbage from the message
void clean_message(message& m)
{
  while(m.parts() > 0){
    m.pop_back();
  }
}

//file manager functions
void messageToFile(const message& msg, const string& fileName)
{
  //put a message in a file
  const void *data;
  msg.get(&data, 0);
  size_t size = msg.size(0);

  ofstream ofs(fileName, ios::binary);
  ofs.write((char*)data, size);
}

vector<char> readFileToBytes(const string& fileName)
{
  //read binarily a file
  ifstream ifs(fileName, ios::binary | ios::ate);
  ifstream::pos_type pos = ifs.tellg();

  vector<char> result(pos);

  ifs.seekg(0, ios::beg);
  ifs.read(result.data(), pos);

  return result;
}

void fileToMesage(const string& fileName, message& msg)
{
  //put a file in a message
  vector<char> bytes = readFileToBytes(fileName);
  msg.add_raw(bytes.data(), bytes.size());
}
