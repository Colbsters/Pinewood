#include <Pinewood/Pinewood.h>

#include <thread>

using namespace Pinewood::operators;

int main()
{

	Pinewood::Window window;

	window.Create({
		.title = "Pinewood Application Window",
		.x = 100,
		.y = 100,
		.width = 1280,
		.height = 720,
		.flags = Pinewood::WindowCreateFlags::DefaultStyle | Pinewood::WindowCreateFlags::Show | Pinewood::WindowCreateFlags::Async
	});

	// No need to call update, it's done automatically on a separate thread
	while (window.IsRunning())
		std::this_thread::sleep_for(std::chrono::seconds{ 1 }); // I know it's a test, but I still don't want to hog the CPU

	return 0;
}
