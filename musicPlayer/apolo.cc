#include "apolo.hh"

/*
 * files_in
 * Take the name of a directory and returns a vector with the names of 
 * the files in that directory, uses glob()*/
vector<string> files_in(string dir_name){
	glob_t glob_result;
	vector<string> file_list;
	string name;
	name += dir_name+"/*";
	glob(name.c_str(),GLOB_TILDE,NULL,&glob_result);
	for(unsigned int i=0; i<glob_result.gl_pathc; ++i){
	  file_list.push_back(glob_result.gl_pathv[i]);
	}
	return file_list;
}

/*sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("music/Lumme-Badloop.ogg")){
		return -1;
		printf("error");
	}

	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();
	while(true);
	return 0;*/
