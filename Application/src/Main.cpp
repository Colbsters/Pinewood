#include <Pinewood/Pinewood.h>
#include <PWMath/PWMath.h>

#include <thread>

using namespace Pinewood::Operators;

int main()
{
	Pinewood::Window window;
	Pinewood::HLContext context;
	Pinewood::HLRenderInterface renderInterface;
	Pinewood::HLBuffer vertexBuffer;

	PWMath::Vector2F32 vertices[3]{
		{  0.0,  0.5 },
		{ -0.5, -0.5 },
		{  0.5, -0.5 },
	};

	window.Create({
		.title = "Pinewood Application Window",
		.x = 100,
		.y = 100,
		.width = 1280,
		.height = 720,
		.flags = Pinewood::WindowCreateFlags::DefaultStyle | Pinewood::WindowCreateFlags::Show | Pinewood::WindowCreateFlags::Async
		});

	context.Create({
		.window = window,
		.swapInterval = 1
		});

	renderInterface.Create({
		.context = context
		});

	renderInterface.SetClearColor({ 0.2f, 0.3f, 0.5f, 1.0f });

	vertexBuffer.Create({
		.context = context,
		.usage = Pinewood::HLBufferUsage::Mutable,
		.size = sizeof(vertices),
		.data = vertices
		});

	// No need to call update, it's done automatically on a separate thread
	while (window.IsRunning())
	{
		context.SwapBuffers();

		renderInterface.ClearTarget(Pinewood::ClearTargetFlags::Color);

		// Render here
	}

	return 0;
}
