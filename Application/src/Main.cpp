#include <Pinewood/Pinewood.h>
#include <PWMath/PWMath.h>

#include <thread>
#include <mutex>

using namespace Pinewood::Operators;

struct Vertex
{
	PWMath::Vector2F32 position;
	PWMath::Vector2F32 uv;
};

Vertex vertices[4]{
	{ {  0.5,  0.5 }, {  2.0,  2.0 } },
	{ {  0.5, -0.5 }, {  2.0, -1.0 } },
	{ { -0.5, -0.5 }, { -1.0, -1.0 } },
	{ { -0.5,  0.5 }, { -1.0,  2.0 } }
};

uint32_t indices[6]{
	0, 1, 2,
	2, 3, 0
};

Pinewood::HLLayoutElement layoutElements[]{
	{ 0,					Pinewood::HLLayoutElementType::Vector2F32, /* index */ 0, /* binding */ 0, /* divisor (per-vertex) */ 0 },
	{ offsetof(Vertex, uv),	Pinewood::HLLayoutElementType::Vector3F32, /* index */ 1, /* binding */ 0, /* divisor (per-vertex) */ 0 }
};

Pinewood::HLLayoutBinding layoutBindings[]{
	{ 0, sizeof(Vertex) }
};

const char* vertexShaderSource = R"(
#version 450

layout(location = 0) in vec4 i_position;
layout(location = 1) in vec4 i_color;

layout(std140, location = 0) uniform transform
{
	mat4 u_mvp;
};

out vec4 v_color;

void main()
{
	gl_Position = u_mvp * i_position;
	v_color = i_color;
}
)";

const char* pixelShaderSource = R"(
#version 450

layout(location = 0) out vec4 o_color;

layout(location = 1) uniform sampler2D i_sampler;

in vec4 v_color;

void main()
{
	o_color = texture(i_sampler, v_color.rg);//vec4(pow(v_color.rgb, vec3(1/2.2)), v_color.a); 
}
)";

const char* postVertexSource = R"(
#version 450

layout(location = 0) in vec4 i_position;
out vec2 v_uv;

void main()
{
	gl_Position = i_position;
	v_uv = 0.5 * (i_position.xy + 1.0);
}
)";

const char* postPixelSource = R"(
#version 450

layout(location = 0) out vec4 o_color;

layout(location = 1) uniform sampler2D i_color;

in vec2 v_uv;

void main()
{
	vec3 color = texture(i_color, v_uv).rgb;
	color = 1.0 - color;
	o_color = vec4(color, 1.0);
}
)";

constexpr uint8_t textureData[] = {
	0xff, 0x00, 0x00,
	0xff, 0xff, 0x00,
	0x00, 0xff, 0x00,
	0x00, 0x00, 0x00,

	0x00, 0xff, 0xff,
	0x00, 0x00, 0xff,
	0xff, 0x00, 0xff,
	0xff, 0xff, 0xff,
};

std::mutex contextMutex;
Pinewood::Window window;
Pinewood::HLContext context;
Pinewood::HLRenderInterface renderInterface;
Pinewood::HLBuffer vertexBuffer, indexBuffer, uniformBuffer;
Pinewood::HLLayout vertexLayout;
Pinewood::HLVertexBinding vertexBinding;
Pinewood::HLShaderModule vertexShader, pixelShader;
Pinewood::HLShaderProgram shaderProgram;
Pinewood::HLTexture2D texture;

Pinewood::HLBuffer screenVertexBuffer;
Pinewood::HLLayout screenVertexLayout;
Pinewood::HLVertexBinding screenVertexBinding;
Pinewood::HLShaderModule postVertex, postPixel;
Pinewood::HLShaderProgram postProgram;
Pinewood::HLTexture2D colorAttachment;
Pinewood::HLFramebuffer framebuffer;

Pinewood::KeyboardInput keyboard;
Pinewood::MouseInput mouse;

void WindowResizeHandler(const Pinewood::Window&, const Pinewood::WindowEvent& e, void*)
{
	auto& event = static_cast<const Pinewood::WindowResizeEvent&>(e);
	std::lock_guard<std::mutex> lock{ contextMutex };

	context.MakeCurrent();
	context.ResizeSwapChain(event.width, event.height);

	// Recreate framebuffer
	colorAttachment.Create({
		.context = context,
		.width = event.width,
		.height = event.height,
		.sampleFilter = Pinewood::HLTextureFilter::Nearest,
		.format = Pinewood::HLImageFormat::R8G8B8_UNorm,
		.data = nullptr
		});

	{
		Pinewood::HLTexture2D textures[]{ colorAttachment };
		Pinewood::HLFramebufferAttachment attachments[]{ Pinewood::HLFramebufferAttachment::Color0 };
		framebuffer.Create({
			.context = context,
			.textures = std::span{ textures },
			.attachments = std::span{ attachments }
			});
	}

	context.MakeObsolete();
}

int main()
{
	window.Create({
		.title = "Pinewood Application Window",
		.x = 100,
		.y = 100,
		.width = 1280,
		.height = 720,
		.flags = Pinewood::WindowCreateFlags::DefaultStyle | Pinewood::WindowCreateFlags::Show | Pinewood::WindowCreateFlags::Async
		});

	window.AddEventHandler({
		.function = WindowResizeHandler,
		.event = Pinewood::WindowEventCode::WindowResize
		});

	keyboard.Create(window);
	mouse.Create(window);

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

	indexBuffer.Create({
		.context = context,
		.usage = Pinewood::HLBufferUsage::Mutable,
		.size = sizeof(indices),
		.data = indices
		});

	uniformBuffer.Create({
		.context = context,
		.usage = Pinewood::HLBufferUsage::Mutable,
		.size = sizeof(PWMath::Matrix4x4F32)
		});

	vertexLayout.Create({
		.context = context,
		.elements = layoutElements,
		.bindings = layoutBindings
		});

	vertexBinding.Create({
		.context = context,
		.vertexBuffers = std::span{ &vertexBuffer, 1 },
		.indexBuffer = indexBuffer,
		.vertexLayout = vertexLayout
		});

	vertexShader.Create({
		.context = context,
		.type = Pinewood::HLShaderModuleType::Vertex,
		.shaderSource = vertexShaderSource
		});

	pixelShader.Create({
		.context = context,
		.type = Pinewood::HLShaderModuleType::Pixel,
		.shaderSource = pixelShaderSource
		});

	{
		Pinewood::HLShaderModule shaderModules[]{ vertexShader, pixelShader };

		shaderProgram.Create({
			.context = context,
			.shaderModules = shaderModules
			});
	}

	postVertex.Create({
		.context = context,
		.type = Pinewood::HLShaderModuleType::Vertex,
		.shaderSource = postVertexSource
		});

	postPixel.Create({
		.context = context,
		.type = Pinewood::HLShaderModuleType::Pixel,
		.shaderSource = postPixelSource
		});

	{
		Pinewood::HLShaderModule shaderModules[]{ postVertex, postPixel };

		postProgram.Create({
			.context = context,
			.shaderModules = shaderModules
			});
	}

	texture.Create({
		.context = context,
		.width = 4,
		.height = 2,
		.sampleFilter = Pinewood::HLTextureFilter::Nearest,
		.format = Pinewood::HLImageFormat::R8G8B8_UNorm,
		.data = (void*)textureData
		});
	

	{
		PWMath::Vector2F32 screenVertices[] = {
			PWMath::Vector2F32{-1.0f, 3.0f },
			PWMath::Vector2F32{ 3.0f,-1.0f },
			PWMath::Vector2F32{-1.0f,-1.0f }
		};

		Pinewood::HLLayoutElement screenLayoutElements[]{
			{ 0,	Pinewood::HLLayoutElementType::Vector2F32, /* index */ 0, /* binding */ 0, /* divisor (per-vertex) */ 0 },
		};

		Pinewood::HLLayoutBinding screenLayoutBindings[]{
			{ 0, sizeof(PWMath::Vector2F32) }
		};

		screenVertexBuffer.Create({
			.context = context,
			.usage = Pinewood::HLBufferUsage::Immutable,
			.size = sizeof(screenVertices),
			.data = screenVertices
			});

		screenVertexLayout.Create({
			.context = context,
			.elements = screenLayoutElements,
			.bindings = screenLayoutBindings
			});

		Pinewood::HLBuffer screenVertexBuffers[]{ screenVertexBuffer };
		screenVertexBinding.Create({
			.context = context,
			.vertexBuffers = screenVertexBuffers,
			.vertexLayout = screenVertexLayout
			});
	}

	// Create framebuffer
	colorAttachment.Create({
		.context = context,
		.width = 1280,
		.height = 720,
		.sampleFilter = Pinewood::HLTextureFilter::Nearest,
		.format = Pinewood::HLImageFormat::R8G8B8_UNorm,
		.data = nullptr
		});

	{
		Pinewood::HLTexture2D textures[]{ colorAttachment };
		Pinewood::HLFramebufferAttachment attachments[]{ Pinewood::HLFramebufferAttachment::Color0 };
		framebuffer.Create({
			.context = context,
			.textures = std::span{ textures },
			.attachments = std::span{ attachments }
			});
	}

	PWMath::Vector3F32 position{ 0.0f };
	uint32_t qwerty = 0;
	// No need to call update, it's done automatically on a separate thread
	while (window.IsRunning())
	{
		qwerty++;
		if (keyboard.IsKeyPressed(Pinewood::KeyCode::W)) position.y += 0.05f;
		if (keyboard.IsKeyPressed(Pinewood::KeyCode::S)) position.y -= 0.05f;
		if (keyboard.IsKeyPressed(Pinewood::KeyCode::A)) position.x -= 0.05f;
		if (keyboard.IsKeyPressed(Pinewood::KeyCode::D)) position.x += 0.05f;

		std::lock_guard<std::mutex> lock{ contextMutex };

		context.MakeCurrent();

		context.SwapBuffers();

		auto matrix = PWMath::Translate(PWMath::Matrix4x4F32{ 1 }, position);
		matrix = PWMath::Scale(matrix, PWMath::Vector3F32{ std::expf(mouse.GetScrollDelta()) });

		void* uniformMapping;
		uniformBuffer.Map(uniformMapping, Pinewood::HLBufferAccess::Write);
		std::memcpy(uniformMapping, &matrix, sizeof(matrix));
		uniformBuffer.Unmap();

		// Render here
		renderInterface.SetFramebuffer(framebuffer);

		renderInterface.ClearTarget(Pinewood::ClearTargetFlags::Color);

		renderInterface.BindShaderProgram(shaderProgram);

		renderInterface.BindVertexBinding(vertexBinding);

		renderInterface.SetConstantBuffer(0, uniformBuffer);

		renderInterface.SetTexture2D(1, 0, texture);

		renderInterface.DrawIndexed(6);



		renderInterface.ResetFramebuffer();
		renderInterface.ClearTarget(Pinewood::ClearTargetFlags::Color);

		renderInterface.ClearTarget(Pinewood::ClearTargetFlags::Color | Pinewood::ClearTargetFlags::Depth | Pinewood::ClearTargetFlags::Stencil);

		renderInterface.BindShaderProgram(postProgram);

		renderInterface.BindVertexBinding(screenVertexBinding);

		renderInterface.SetTexture2D(1, 0, colorAttachment);

		renderInterface.Draw(0, 3);

		context.MakeObsolete();
	}

	return 0;
}
