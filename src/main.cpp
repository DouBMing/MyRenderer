#include <iostream>
#include "scene.h"

int main(int argc, char** argv)
{
	std::string scenePath = "Scene.txt";
	Scene scene(scenePath);
	scene.camera->Render();
	return 0;
}