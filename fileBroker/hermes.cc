#include "hermes.hh"

FileSplitter::FileSplitter(string filename) : filename(filename)
{
  ifstream ifs(filename, ios::binary | ios::ate);
  endpos = ifs.tellg();
  ifs.seekg(0, ios::beg);
  pos = ifs.tellg();
}

int FileSplitter::getNumberOfParts()
{
  return endpos / CHUNK_SIZE + (endpos % CHUNK_SIZE > 0);
}

int FileSplitter::getSize()
{
  return (int) endpos;
}

void FileSplitter::nextChunkToMesage(message& msg)
{
  ifstream ifs(filename, ios::binary);
      //validate that has bytes to read
  if (!isOver())
  {
        //the chunk size is half Mib = 2^16 bytes
        //verify if isn't the last part
    if( pos <= (endpos - CHUNK_SIZE) )
    {
      vector<char> bytes(CHUNK_SIZE);
      ifs.seekg(pos);
      ifs.read(bytes.data(), CHUNK_SIZE);
      ifs.seekg(pos + CHUNK_SIZE);
      pos = pos + CHUNK_SIZE;
      msg.add_raw(bytes.data(), bytes.size());
    }else{
      vector<char> bytes(endpos-pos);
      ifs.seekg(pos);
          //if is the las part
      ifs.read(bytes.data(), endpos-pos);
      pos = endpos;
      msg.add_raw(bytes.data(), bytes.size());
    }
  }
}   

bool FileSplitter::isOver()
{
  return pos >= endpos;
}

int string_to_number ( const string &Text )
  {
     istringstream ss(Text);
     int result;
     return ss >> result ? result : 0;
  }

long long int string_to_big_number ( const string &Text )
  {
     istringstream ss(Text);
     long long int result;
     return ss >> result ? result : 0;
  }

string number_to_string ( int number )
{
   ostringstream ss;
   ss << number;
   return ss.str();
}

string big_number_to_string ( long long int number )
{
   ostringstream ss;
   ss << number;
   return ss.str();
}

void messageToPartialFile(const message& msg, const string& fileName)
{
  const void *data;
  msg.get(&data, 0);
  size_t size = msg.size(0);

  ofstream ofs(fileName, ios::binary | ios::ate | ios::app);
  ofs.seekp(0,ios::end);
  ofs.write((char*)data, size);
}

void create_message(const string& cmd, const string& filename, message& msg)
{
  msg << cmd << filename;
}


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

void fileToMessage(const string& fileName, message& msg)
{
  //put a file in a message
  vector<char> bytes = readFileToBytes(fileName);
  msg.add_raw(bytes.data(), bytes.size());
}

vector<string> explode(string &big_string, string token) {
  vector<string> individual_strings;
  regex rex("(.*)_uploaded_(.*)\\.(....?)");
  smatch m;
  regex_match(big_string,m,rex);
  //0 position is the whole string
  for (unsigned int i = 1; i < m.size(); ++i)
  {
    ssub_match sm = m[i];
    string little_string = sm.str();
    individual_strings.push_back(little_string);
  }
  return individual_strings;
}