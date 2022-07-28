#include <Pinewood/Pinewood.h>

#include <thread>

using namespace Pinewood::operators;

int main()
{
	auto window = std::make_shared<Pinewood::Window>();
	auto context = std::make_shared<Pinewood::HLContext>();

	window->Create({
		.title = "Pinewood Application Window",
		.x = 100,
		.y = 100,
		.width = 1280,
		.height = 720,
		.flags = Pinewood::WindowCreateFlags::DefaultStyle | Pinewood::WindowCreateFlags::Show | Pinewood::WindowCreateFlags::Async
	});

	context->Create({
		.window = window,
		.swapInterval = 1
	});

	// No need to call update, it's done automatically on a separate thread
	while (window->IsRunning())
	{
		context->SwapBuffers();
	}

	return 0;
}
