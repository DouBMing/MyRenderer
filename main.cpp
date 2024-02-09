#include <iostream>
#include "bitmap.h"
#include "camera.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

// const Color white(255, 255, 255);
// const Color black(0, 0, 0);
// const Color red(255, 0, 0);

int main(int argc, char** argv)
{
	string modelPath = "Models/";
	string modelName;
	cout << "model name:" << endl;
	cin >> modelName;
	Object object(modelPath + modelName + ".obj");
	object.transform.position = Vector3(0, 0, 0);

	Camera camera(1600, 900);
	camera.position = Vector3(0, 0, 3);
	camera.up = Vector3(0, 1, 0);
	camera.lookAt = Vector3(0, 0, -1);
	camera.Render(object);
	return 0;
}