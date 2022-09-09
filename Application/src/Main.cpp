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
	Pinewood::HLLayout vertexLayout;
	Pinewood::HLVertexBinding vertexBinding;

	PWMath::Vector2F32 vertices[3]{
		{  0.0,  0.5 },
		{ -0.5, -0.5 },
		{  0.5, -0.5 },
	};

	Pinewood::HLLayoutElement layoutElements[]{
		{ 0, Pinewood::HLLayoutElementType::Vector2F32, /* index */ 0, /* binding */ 0, /* divisor (per-vertex) */ 0 }
	};

	Pinewood::HLLayoutBinding layoutBindings[]{
		{ 0, sizeof(PWMath::Vector2F32) }
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

	vertexLayout.Create({
		.context = context,
		.elements = layoutElements,
		.bindings = layoutBindings
		});

	vertexBinding.Create({
		.context = context,
		.vertexBuffers = std::span{ &vertexBuffer, 1 },
		.vertexLayout = vertexLayout
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
