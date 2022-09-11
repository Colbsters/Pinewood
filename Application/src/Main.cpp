#include <Pinewood/Pinewood.h>
#include <PWMath/PWMath.h>

#include <thread>

struct Vertex
{
	PWMath::Vector2F32 position;
	PWMath::Vector3F32 color;
};

Vertex vertices[4]{
	{ {  0.5,  0.5 }, { 1.0, 1.0, 0.0 } },
	{ {  0.5, -0.5 }, { 0.0, 1.0, 0.0 } },
	{ { -0.5, -0.5 }, { 0.0, 0.0, 1.0 } },
	{ { -0.5,  0.5 }, { 1.0, 0.0, 0.0 } }
};

uint32_t indices[6]{
	0, 1, 2,
	2, 3, 0
};

Pinewood::HLLayoutElement layoutElements[]{
	{ 0,						Pinewood::HLLayoutElementType::Vector2F32, /* index */ 0, /* binding */ 0, /* divisor (per-vertex) */ 0 },
	{ offsetof(Vertex, color),	Pinewood::HLLayoutElementType::Vector3F32, /* index */ 1, /* binding */ 0, /* divisor (per-vertex) */ 0 }
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

in vec4 v_color;

void main()
{
	o_color = vec4(pow(v_color.rgb, vec3(1/2.2)), v_color.a); 
}
)";

using namespace Pinewood::Operators;

int main()
{
	Pinewood::Window window;
	Pinewood::HLContext context;
	Pinewood::HLRenderInterface renderInterface;
	Pinewood::HLBuffer vertexBuffer, indexBuffer, uniformBuffer;
	Pinewood::HLLayout vertexLayout;
	Pinewood::HLVertexBinding vertexBinding;
	Pinewood::HLShaderModule vertexShader, pixelShader;
	Pinewood::HLShaderProgram shaderProgram;

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

	float x = 0.0f;
	// No need to call update, it's done automatically on a separate thread
	while (window.IsRunning())
	{
		context.SwapBuffers();

		renderInterface.ClearTarget(Pinewood::ClearTargetFlags::Color);

		x += 0.005f;
		if (x > 1.5) x = -1.5;
		auto matrix = PWMath::Translate(PWMath::Matrix4x4F32{ 1 }, { x, x, 0 });

		void* uniformMapping;
		uniformBuffer.Map(uniformMapping, Pinewood::HLBufferAccess::Write);
		std::memcpy(uniformMapping, &matrix, sizeof(matrix));
		uniformBuffer.Unmap();

		// Render here
		renderInterface.BindShaderProgram(shaderProgram);

		renderInterface.BindVertexBinding(vertexBinding);

		renderInterface.SetConstantBuffer(0, uniformBuffer);

		renderInterface.DrawIndexed(6);
	}

	return 0;
}
