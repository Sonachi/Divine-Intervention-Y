#include "Editor_Level.h"
#include "FocusCamera.h"
#include <ModelManager.h>
#include <AudioManager.h>

/*class EditorLevel : public Level{
	GameObject* currentSelection;
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
	};*/

void EditorLevel::LoadContent(){

	//Set the camera to focus on the sun to start, will change to layers after initial setup
	mainCamera->isFlagged = true;
	mainCamera = new FocusCamera(this, &dayStar, dayStar.scale * 2);
	currentCamera = mainCamera;

	//Set up the skybox
	Models->CreateSkybox("skybox", 10000.0f);
	Models->loadTexture("skybox1", "Images/skyboxUP.png");
	Models->loadTexture("layerGrid", "Images/grid.png");

	//Loading all assets for things that could be added.
	Models->loadTexture("planet1", "Images/aruba.tif");
	Models->loadTexture("planet2", "Images/planet_Rim.tif");
	Models->loadTexture("planet3", "Images/minersMoon.tif");
	Models->loadTexture("planet4", "Images/planetTex.png");
	Models->loadTexture("planet5", "Images/hoth.tif");

	Models->loadModel("sphere", "Models/planet.obj", true);
	Models->loadModel("sun", "Models/turboSphere.obj", true);

	Models->loadModel("warpGate", "Models/WarpGate.obj", true);
	Models->loadTexture("gateTexture", "Images/rosary.png");

	Models->loadTexture("meteorTex1", "Images/meteor_texture.tif");
	Models->loadTexture("meteorTex2", "Images/meteor_texture_2.tif");
	Models->loadTexture("meteorTex3", "Images/meteor_texture_3.tif");

	Models->loadTexture("sunTexture", "Images/sun.tif");

	Models->loadModel("meteor1", "Models/meteor_01.obj", true);
	Models->loadModel("meteor2", "Models/meteor_02.obj", true);
	Models->loadModel("meteor3", "Models/meteor_03.obj", true);


	//Load up and play the music for in-game
	Audio->loadMusic("gameTheme", "Sounds/Exotics.wav");
	Audio->getMusic("gameTheme")->Play();
}