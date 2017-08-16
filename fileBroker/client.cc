#include "hermes.cc"

/*
It is capable of
loading a binary file
conecting to a server
recieving files and save them to disk
send stored files
remove files with a command
list the files stored in the server
*/

void listf(socket &socket_broker, string username);
void uploadf(socket &socket_broker, string username);
void downloadf(socket &socket_broker, string username, string own_ip,string own_port);
void erasef(socket &socket_broker, string username);

int main(int argc, char const *argv[]) {

    if ( not ( 4 == argc ) ) {
      cerr << "Wrong number of arguments, remember to provide your username, ip & port" << endl;
      return 1;
    }
    //take the username
    string username(argv[1]);
    string own_ip(argv[2]);
    string own_port(argv[3]);
    cout << "Welcome " <<  username <<endl;
    string broker_ip;
    cout << "Please specify the BROKER \"ip:port\"" << endl;
    cin >> broker_ip;
    broker_ip = "tcp://" + broker_ip;
    own_ip = "tcp://" + own_ip + ":" + own_port;

    //Created a context (blackbox)
    context brkr;
    //BROKER CONNECTION
    //Created the socket and the conection
    socket socket_broker(brkr, socket_type::request);
    socket_broker.connect(broker_ip);

    char option=' ';
    cout << "Available options:" << endl;
    cout << " * (L)ist" << endl;
    cout << " * (U)pload" << endl;
    cout << " * (D)ownload" << endl;
    cout << " * (E)rase" << endl;
    cout << " * E(X)it" << endl;
    do
    {
      //display of the options
      cout << "> ";
      cin >> option;

      switch(option){
        case 'L':
            listf(socket_broker,username);
            break;
        case 'U':
            uploadf(socket_broker,username);
            break;
        case 'D':
            downloadf(socket_broker,username,own_ip, own_port);
            break;
        case 'E':
            erasef(socket_broker,username);
            break;
        case 'X':
            return 0;
            break;
        default:
            cout << "Unrecognized option: " << option << endl;
        break;
      }
    } while (option!='X');

  cout << "Finished." << endl;
  return 0;
}

void listf(socket &socket_broker, string username){
    //ask for list
    message m;
    string cmd("list");
    m << username << cmd;
    socket_broker.send(m);
    //wait for answer
    message ans;
    string chain;
    socket_broker.receive(ans);
    ans >> chain;
    cout << "Files in the server: "<< chain << endl;

}

void uploadf(socket &socket_broker, string username){
  //declaration
  string filename,new_filename;

  //get the name of the file
  cout << "write the filename: ";
  cin >> filename;
  //check if file exist
  ifstream infile(filename);
  if (!infile.good())
  {
    cout << "Bad filename!"<< endl;
    return;
  }//get out if bad filename

  //the privated client-server comand
  string cmd = "upload";
  string strMsg;
  message m;
  //create the object for manage the file
  //jack the ripper
  FileSplitter chop(filename);
  int size = chop.getSize();
  //send a message to the boker
  //whit the info of the file
  string ssize;
  ssize = number_to_string(size);
  m << username << cmd << filename << ssize;
  socket_broker.send(m);
  //wipe and receive;
  clean_message(m);
  socket_broker.receive(m);
  //the broker send:
  //status, the server ip and port
  string status;
  string servip;
  m >> status;
  if(status != "ok"){
    //something went wrong
    if(status == "repeated"){
      cout << "The file already exists"<< endl;
    }else{
      cout << "The broker answer with bad news"<< endl;
    }
    return;
  }
    m >> servip;
    m >> new_filename;
    //establish new connection in an existent context
    cout << servip << endl;
    context srvr;
    socket socket_server(srvr, socket_type::push);
    socket_server.connect(servip);
    clean_message(m);
    //calculate the numeber of parts
    int number_of_parts = chop.getNumberOfParts();
    string tmp;
    tmp = number_to_string(number_of_parts);
    //send the message
    m << cmd << new_filename << ssize << tmp ;
    socket_server.send(m);
    clean_message(m);
    //start to send the parts of the file
    while(!chop.isOver()){
      chop.nextChunkToMesage(m);
      socket_server.send(m);
      clean_message(m);
    }
    return;  ///--------------------------
}

void downloadf(socket &socket_broker, string username, string own_ip, string own_port){
  //first ask for the file
  //if file exists wait for answer
  //if file doesn't exist then exit
  string filename;
  string answer;
  string response;
  string file;
  string servip;

  //get the name of the file
  cout << "write the filename: ";
  cin >> filename;

  //the privated client-server comand
  string cmd = "download";

  message m;
  m << username << cmd << filename;
  socket_broker.send(m);
  clean_message(m);
  socket_broker.receive(m);
  m >> answer;
  if(answer == "ok"){
    m >> servip;
    m >> file;
    //push a message to the server
    context srvr;
    socket socket_server(srvr, socket_type::push);
    socket_server.connect(servip);
    clean_message(m);
    m << cmd << file << own_ip;
    socket_server.send(m);
    //change of context
    socket_server.disconnect(servip);
    //the long name indicates the formidable use
    //of the sword in this function (a.k.a. machete)
    string new_sword_ip("tcp://*:");
    new_sword_ip = new_sword_ip + own_port;
    socket socket_amakakeru_ryu_no_hirameki(srvr, socket_type::pull);
    socket_amakakeru_ryu_no_hirameki.bind(new_sword_ip);
    //new PULL context
          clean_message(m);
          string size, parts;
          string fname = username + "_downloaded_" + filename;
          socket_amakakeru_ryu_no_hirameki.receive(m);
          m >> size;
          m >> parts;
          int nparts = string_to_number(parts);
          clean_message(m);
          //start to receive the file
          remove( fname.c_str() );
          for (int i = 0; i < nparts; ++i)
          {
            socket_amakakeru_ryu_no_hirameki.receive(m);
            messageToPartialFile(m,fname);
            clean_message(m);
          }
          response = "File downloaded successfully!";
    // end of the PULL context
    //whit the end of the class the temp_sockets are destroyed
  }else if( answer == "notexists"){
    //do nothing
    response = "The file doesn't exists";
  }
  cout << response << endl;

  //----------------------------

  clean_message(m);

  if (answer == "bad")
  {
    cout << "Bad filename!"<< endl;
    return;
  }else if(answer == "good"){
    //recive the file
    socket_broker.send("ready to receive");
    //receive the number of parts
    clean_message(m);
    socket_broker.receive(m);
    int number_of_parts = 0;
    string cosas;
    cosas = m.get(0);
    number_of_parts = string_to_number(cosas);
    clean_message(m);
    socket_broker.send("everything is well");
    //define the prefix
    filename = "new_" + filename;
    remove( filename.c_str() );
    for (int i = 0; i < number_of_parts; ++i)
    {
      socket_broker.receive(m);
      socket_broker.send("ACK");
      messageToPartialFile(m,filename);
      clean_message(m);
    }
    socket_broker.receive(m);
    cout << "Finished" << endl;
  }
}

void erasef(socket &socket_broker, string username){
   //ask for the file deletion
  string filename,servip;

  //get the name of the file
  cout << "write the filename: ";
  cin >> filename;

  //the privated client-server comand
  string cmd = "erase";
  string answer;
  string file;
  string response;

  message m;
  m << username << cmd << filename;
  socket_broker.send(m);

  cout << "Attempting to delete the file...";
  socket_broker.receive(m);
  m >> answer;
  if(answer == "ok"){
    m >> servip;
    m >> file;
    //push a message to the server
    context srvr;
    socket socket_server(srvr, socket_type::push);
    socket_server.connect(servip);
    clean_message(m);
    m << cmd << file;
    socket_server.send(m);
    response = "File deleted successfully";
  }else if( answer == "notexists"){
    //do nothing
    response = "The file doesn't exists";
  }
  cout << response << endl;
}
