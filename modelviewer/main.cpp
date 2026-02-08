#include "model_viewer.h"

int main(int argc, char** argv)
{
	ModelViewer modelViewer;

	if (modelViewer.init() != 0)
	{
		std::cerr << "Failed to initialize application" << std::endl;
		return 1;
	}

	// Run the main loop
	unsigned result = modelViewer.run();

	// Shutdown is called automatically in destructor
	std::cout << "Application finished with code: " << result << std::endl;

	return result;
}