#include <iostream>
#include "scene.h"
#include "timer.h"

int main(int argc, char** argv)
{
	TimerManager timerManager;
	std::string scenePath = "Scene.txt";
	Scene scene(scenePath);
	scene.camera->Render();
	timerManager.Print();
	system("pause");
	return 0;
}