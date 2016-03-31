#pragma once

#include "AudioManager.h"
#include "dependencies\TinyXML\tinyxml2.h"

/*
	A singleton class the will store all the relevant
	properties of the game like screen size, master volume.
	This class will read and write all properties to an XML
	so they will be saved from one day to the next
*/
class GameProperties
{
public:
	~GameProperties();

	//Returns pointer to singleton properties
	//Creates one if none exists
	static GameProperties * getInstance(){
		if (!instance) instance = new GameProperties();
		return instance;
	}

	//A struct containing all relavant video properties
	struct VideoProperties{
		float drawDistance;
		int screenWidth;
		int screenHeight;
		bool isFullscreen;

		//Reads video property values from the xml and assigns
		void readValues();

		//Writes changed values to xml
		void writeValues();

		float aspectRatio();
	};

	//Struct containing audio properties
	struct AudioProperties{
		float masterVolume;
		float musicVolume;
		float soundVolume;

		void readValues();
		void writeValues();
	};

	//Called when you click the apply after changing some settings
	//Writes current values to the xml and saves it
	static void ApplyChanges(){
		vp->writeValues();

		//A series of checks to ensure the volume values are between 0 and 1
		//May not be neccesary?
		if (ap->masterVolume > 1)ap->masterVolume = 1;
		if (ap->masterVolume < 0)ap->masterVolume = 0;

		if (ap->musicVolume > 1)ap->musicVolume = 1;
		if (ap->musicVolume < 0)ap->musicVolume = 0;

		if (ap->soundVolume > 1)ap->soundVolume = 1;
		if (ap->soundVolume < 0)ap->soundVolume = 0;

		ap->writeValues();

		doc.SaveFile("properties.xml");

		AudioManager * am = AudioManager::getInstance();
		am->setSoundVolume(ap->soundVolume * ap->masterVolume);
		am->setMusicVolume(ap->musicVolume * ap->masterVolume);
	}

	//Returns the address to the video propery struct
	static VideoProperties * getVideoProperties(){
		return vp;
	}

	//Returns the address to audio property struct
	static AudioProperties * getAudioProperties(){
		return ap;
	}

private:
	GameProperties();

	//Single pointer to GameProperties object
	static GameProperties * instance;

	//Single pointers to Property structs
	static VideoProperties * vp;
	static AudioProperties * ap;

	//A reference to the xml document
	static tinyxml2::XMLDocument doc;
};

