#ifndef _HERMES_HH_
  #define _HERMES_HH_

#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <regex>
#include <string>
#include <iterator>
#include <vector>

#define CHUNK_SIZE 65536

using namespace std;
using namespace zmqpp;

class FileSplitter {
  protected:
    int pos;
    string filename;
    int endpos;

  public:
    FileSplitter(string filename);
    int getNumberOfParts();
    void nextChunkToMesage(message& msg);    
    bool isOver();
    int getSize();
};

int string_to_number ( const string &Text );
long long int string_to_big_number ( const string &Text );
string number_to_string ( int number );
string big_number_to_string ( long long int number );
void clean_message(message& m);
vector<char> readFileToBytes(const string& fileName) ;
void fileToMessage(const string& fileName, message& msg);
void messageToFile(const message& msg, const string& fileName);
void messageToPartialFile(const message& msg, const string& fileName);
void create_message(const string& cmd, const string& filename, message& msg);
void seek_n_destroy(string& victim, string& part);
vector<string> explode(string &big_string,string token);


#endif