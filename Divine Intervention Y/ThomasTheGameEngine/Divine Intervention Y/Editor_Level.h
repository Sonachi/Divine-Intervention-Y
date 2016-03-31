#pragma once

#include <Level.h>
#include "Layer.h"
#include <Rigidbody.h>
#include <Ray.h>
#include "Sun.h"
#include <States.h>

class EditorLevel : public Level{
	GameObject* currentSelection, skybox;
public:

	EditorLevel();
	EditorLevel(std::string filename_); //For loading from an alraedy saved level
	~EditorLevel();

	//Inherited fields
	void LoadContent() override;
	void LevelUpdate(float timeStep_) override;

	//Specific to Editor
	Ray screenCaster;
	std::vector<Layer *> layers;

	//Creator interaction
	void swapLayerOrder(int, int);
	void changeActiveLayer(int);
	void addLayer();
	void deleteLayer();

	void newCourse();
	void saveCourse();
	void loadCourse(std::string fileName_);
	void testCourse();
	void quitEditor();

	void addObjectToLayer(GameObject* obstacle_);

	//Content common across all courses
	Sun dayStar;
	std::string starName;
	std::string saveFileName;
};