#include "GameProperties.h"
#include "AudioManager.h"
#include <iostream>
#include <string>

GameProperties * GameProperties::instance;
GameProperties::VideoProperties * GameProperties::vp;
GameProperties::AudioProperties * GameProperties::ap;
tinyxml2::XMLDocument GameProperties::doc;

//Initializes the video and audio property structs
//And reads the values from the xml assigning them
GameProperties::GameProperties()
{
	vp = new VideoProperties();
	ap = new AudioProperties();

	doc.LoadFile("properties.xml");

	vp->readValues();
	ap->readValues();

	//These are called here in the constructor to ensure all audio is
	//initialized to proper values.
	AudioManager * am = AudioManager::getInstance();
	am->setSoundVolume(ap->soundVolume * ap->masterVolume);
	am->setMusicVolume(ap->musicVolume * ap->masterVolume);
}


GameProperties::~GameProperties()
{
	delete vp;
	delete ap;
}

//Reads the values from the xml and assigns them to the audio properties
void GameProperties::AudioProperties::readValues()
{
	tinyxml2::XMLElement * element = doc.FirstChildElement("AudioProperty")->FirstChildElement("property");

	masterVolume = std::stoi(element->GetText());
	element = element->NextSiblingElement("property");
	musicVolume = std::stoi(element->GetText());
	element = element->NextSiblingElement("property");
	soundVolume = std::stoi(element->GetText());
}

//Writes current audio values to the xml
void GameProperties::AudioProperties::writeValues()
{
	tinyxml2::XMLElement * element = doc.FirstChildElement("AudioProperty")->FirstChildElement("property");

	element->SetText(masterVolume);
	element = element->NextSiblingElement("property");
	element->SetText(musicVolume);
	element = element->NextSiblingElement("property");
	element->SetText(soundVolume);
}

//Reads video properties from xml and assigns them
void GameProperties::VideoProperties::readValues()
{
	tinyxml2::XMLElement * element = doc.FirstChildElement("VideoProperty")->FirstChildElement("property");

	drawDistance = std::stoi(element->GetText());
	element = element->NextSiblingElement("property");
	screenWidth = std::stoi(element->GetText());
	element = element->NextSiblingElement("property");
	screenHeight = std::stoi(element->GetText());
	element = element->NextSiblingElement("property");
	isFullscreen = std::stoi(element->GetText());
}

//Writes current video values to xml
void GameProperties::VideoProperties::writeValues()
{
	tinyxml2::XMLElement * element = doc.FirstChildElement("VideoProperty")->FirstChildElement("property");

	element->SetText(drawDistance);
	element = element->NextSiblingElement("property");
	element->SetText(screenWidth);
	element = element->NextSiblingElement("property");
	element->SetText(screenHeight);
	element = element->NextSiblingElement("property");
	element->SetText(isFullscreen);
}

float GameProperties::VideoProperties::aspectRatio()
{
	return screenWidth / (float)screenHeight;
}