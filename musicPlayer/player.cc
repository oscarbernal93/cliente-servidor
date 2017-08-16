#include <chrono>
#include <thread>
#include <SFML/Audio.hpp>
//#include <SFML/Audio/SoundBufferRecorder.hpp>

using namespace std::chrono;
using namespace std::this_thread;

int main(int argc, char *argv[]){
   sf::SoundBuffer buffer;
      if (!buffer.loadFromFile("wololo.ogg")){
        return -1;
	printf("error");
     }
      
     sf::Sound sound;
     sound.setBuffer(buffer);
     sound.play();
     while(true);
     return 0;
}
