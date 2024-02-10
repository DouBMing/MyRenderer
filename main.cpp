#include <iostream>
#include "bitmap.h"
#include "camera.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main(int argc, char** argv)
{
	string modelPath = "Models/";
	string modelName;
	cout << "model name:" << endl;
	cin >> modelName;
	// Object object(modelPath + modelName + ".obj",
	// {2, 0, -1}, {0, 120, 15}, {0.02, 0.02, 0.02});
	Object object(modelPath + modelName + ".obj");

	Camera camera(1600, 900, {4.5, 0, -2.5}, {-2, -60, 0});
	camera.Render(object);
	return 0;
}