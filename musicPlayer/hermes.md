# Hermes
_Hermes was the emissary and messenger of the gods_

This library contains class and functions related with the manipulation of messages between the clients and the server.

**Class:** FileSplitter  
This class allows to transmit a message in parts.  
Functions:

* _FileSplitter():_ The contructor receive as parameter the filename to be  sended.  
* _getNumberOfParts():_ Returns the number of parts to be sended.
* _nextChunkToMesage(message& msg):_Put in the message the next part of the file.
* _isOver():_ Check if all the file's chunks was readed.
* _getSize():_ Returns the size of the file

**Function:** string_to_number & string_to_big_number  
Take a string that represents a number and return that number as integer or long integer.

**Function:** number_to_string & big_number_to_string  
Receives a number and return it in a string

**Function:** create_message  
Initialize a message with a specific format, first come a comand and next a username

**Function:** clean_message  
Take a message and destroy all the content

**Function:** readFileToBytes  
Read the file specified by the parameter filename, read and put their bytes in a vector of chars (each char take 4 bits)

**Function:** fileToMessage  
Take a file by filename and put it in a message by bytes as raw data.

**Function:** messageToFile  
Read a message and put it in a entire file.

**Function:** messageToPartialFile  
Read a message and put it (the message contains only a part) at the end of a file.

**Function:** seek_n_destroy  
Take the first string and delete of it the first occurrence of the second string

**Function:** explode  
Read a string (the first one) and divide it in parts, using the second string as delimiter.

