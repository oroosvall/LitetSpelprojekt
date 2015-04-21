#include "Audio.h"
#include <iostream>

using namespace std;

Audio::Audio()
{
	currTrack = -1;
	musicTracks[0] = "../Audio/witcher_dusk.wav";
	musicTracks[1] = "../Audio/witcher_omnious.wav";
}

Audio::~Audio()
{
}

bool Audio::LoadMusic(int fileId)
{
	if (currTrack != fileId)
	{
		//clean previous track data
		shutdown();

		currTrack = fileId;
		//Loading of the WAVE file
		FILE *fp = NULL;
		fp = fopen(musicTracks[fileId], "rb");
		if (!fp) return endWithError("Failed to open file");

		WaveHeader waveFileHeader;

		//Check that the WAVE file is OK
		fread(waveFileHeader.type, sizeof(char), 4, fp);                                              //Reads the first bytes in the file
		if (waveFileHeader.type[0] != 'R' || waveFileHeader.type[1] != 'I'
			|| waveFileHeader.type[2] != 'F' || waveFileHeader.type[3] != 'F')            //Should be "RIFF"
			return endWithError("No RIFF");                                            //Not RIFF

		fread(&waveFileHeader.size, sizeof(unsigned long), 1, fp);                                           //Continue to read the file
		fread(waveFileHeader.type, sizeof(char), 4, fp);                                             //Continue to read the file
		if (waveFileHeader.type[0] != 'W' || waveFileHeader.type[1] != 'A'
			|| waveFileHeader.type[2] != 'V' || waveFileHeader.type[3] != 'E')           //This part should be "WAVE"
			return endWithError("not WAVE");                                            //Not WAVE

		fread(waveFileHeader.type, sizeof(char), 4, fp);                                              //Continue to read the file
		if (waveFileHeader.type[0] != 'f' || waveFileHeader.type[1] != 'm'
			|| waveFileHeader.type[2] != 't' || waveFileHeader.type[3] != ' ')           //This part should be "fmt "
			return endWithError("not fmt ");                                            //Not fmt 

		//Now we know that the file is a acceptable WAVE file
		//Info about the WAVE data is now read and stored
		fread(&waveFileHeader.chunkSize, sizeof(unsigned long), 1, fp);
		fread(&waveFileHeader.formatType, sizeof(short), 1, fp);
		fread(&waveFileHeader.channels, sizeof(short), 1, fp);
		fread(&waveFileHeader.sampleRate, sizeof(unsigned long), 1, fp);
		fread(&waveFileHeader.avgBytesPerSec, sizeof(unsigned long), 1, fp);
		fread(&waveFileHeader.bytesPerSample, sizeof(short), 1, fp);
		fread(&waveFileHeader.bitsPerSample, sizeof(short), 1, fp);

		fread(waveFileHeader.type, sizeof(char), 4, fp);
		if (waveFileHeader.type[0] != 'd' || waveFileHeader.type[1] != 'a'
			|| waveFileHeader.type[2] != 't' || waveFileHeader.type[3] != 'a')           //This part should be "data"
			return endWithError("Missing DATA");                                        //not data

		fread(&waveFileHeader.dataSize, sizeof(unsigned long), 1, fp);                                        //The size of the sound data is read

		//Display the info about the WAVE file
		cout << "Chunk Size: " << waveFileHeader.chunkSize << "\n";
		cout << "Format Type: " << waveFileHeader.formatType << "\n";
		cout << "Channels: " << waveFileHeader.channels << "\n";
		cout << "Sample Rate: " << waveFileHeader.sampleRate << "\n";
		cout << "Average Bytes Per Second: " << waveFileHeader.avgBytesPerSec << "\n";
		cout << "Bytes Per Sample: " << waveFileHeader.bytesPerSample << "\n";
		cout << "Bits Per Sample: " << waveFileHeader.bitsPerSample << "\n";
		cout << "Data Size: " << waveFileHeader.dataSize << "\n";

		unsigned char* buf = new unsigned char[waveFileHeader.dataSize];                            //Allocate memory for the sound data
		cout << fread(buf, sizeof(char), waveFileHeader.dataSize, fp) << " bytes loaded\n";           //Read the sound data and display the 
		//number of bytes loaded.
		//Should be the same as the Data Size if OK


		//Now OpenAL needs to be initialized
		device = alcOpenDevice(NULL);                                               //Open the device
		if (!device) return endWithError("no sound device");                         //Error during device oening
		context = alcCreateContext(device, NULL);                                   //Give the device a context
		alcMakeContextCurrent(context);                                             //Make the context the current
		if (!context) return endWithError("no sound context");                       //Error during context handeling

		//Stores the sound data
		ALuint frequency = waveFileHeader.sampleRate;;                                               //The Sample Rate of the WAVE file
		ALenum format = 0;                                                            //The audio format (bits per sample, number of channels)
		ALint state;

		alGenBuffers(1, &buffer);                                                    //Generate one OpenAL Buffer and link to "buffer"
		alGenSources(1, &source);                                                   //Generate one OpenAL Source and link to "source"
		if (alGetError() != AL_NO_ERROR) return endWithError("Error GenSource");     //Error during buffer/source generation

		//Figure out the format of the WAVE file
		if (waveFileHeader.bitsPerSample == 8)
		{
			if (waveFileHeader.channels == 1)
				format = AL_FORMAT_MONO8;
			else if (waveFileHeader.channels == 2)
				format = AL_FORMAT_STEREO8;
		}
		else if (waveFileHeader.bitsPerSample == 16)
		{
			if (waveFileHeader.channels == 1)
				format = AL_FORMAT_MONO16;
			else if (waveFileHeader.channels == 2)
				format = AL_FORMAT_STEREO16;
		}
		if (!format) return endWithError("Wrong BitPerSample");                      //Not valid format

		alBufferData(buffer, format, buf, waveFileHeader.dataSize, frequency);                    //Store the sound data in the OpenAL Buffer
		if (alGetError() != AL_NO_ERROR)
			return endWithError("Error loading ALBuffer");                              //Error during buffer loading

		//Sound setting variables
		ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };                                    //Position of the source sound
		ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };                                    //Velocity of the source sound
		ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };                                  //Position of the listener
		ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };                                  //Velocity of the listener
		ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };                 //Orientation of the listener
		//First direction vector, then vector pointing up) 
		//Listener                                                                               
		alListenerfv(AL_POSITION, ListenerPos);                                  //Set position of the listener
		alListenerfv(AL_VELOCITY, ListenerVel);                                  //Set velocity of the listener
		alListenerfv(AL_ORIENTATION, ListenerOri);                                  //Set orientation of the listener

		//Source
		alSourcei(source, AL_BUFFER, buffer);                                 //Link the buffer to the source
		alSourcef(source, AL_PITCH, 1.0f);                                 //Set the pitch of the source
		alSourcef(source, AL_GAIN, 1.0f);                                 //Set the gain of the source
		alSourcefv(source, AL_POSITION, SourcePos);                                 //Set the position of the source
		alSourcefv(source, AL_VELOCITY, SourceVel);                                 //Set the velocity of the source
		alSourcei(source, AL_LOOPING, AL_TRUE);                                 //Set if source is looping sound

		//PLAY 
		alSourcePlay(source);

		//Clean-up
		fclose(fp);
		delete[] buf;
	}

	return EXIT_SUCCESS;
}

void Audio::playSound()
{
	// test code
	alSourcePlay(source);
}

void Audio::shutdown()
{
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcDestroyContext(context);
	alcCloseDevice(device);

}

int Audio::endWithError(char* msg, int error)
{
	//Display error message in console
	cout << msg << "\n";
	system("PAUSE");
	return error;
}