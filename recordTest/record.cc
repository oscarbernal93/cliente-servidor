#include <chrono>
#include <thread>
#include <SFML/Audio.hpp>
//#include <SFML/Audio/SoundBufferRecorder.hpp>

using namespace std::chrono;
using namespace std::this_thread;

int main(int argc, char *argv[]){
   if (sf::SoundBufferRecorder::isAvailable())
	{
		// Record some audio data
		sf::SoundBufferRecorder recorder;
		recorder.start();
		sleep_for(seconds(5));
		recorder.stop();
		// Get the buffer containing the captured audio data
		const sf::SoundBuffer& buffer = recorder.getBuffer();
		// Save it to a file (for example...)
		buffer.saveToFile("my_record.ogg");
	}
	return 0;
}
