#include "PotiEngine/ImgUIRenderer.h"
#include <PotiEngine/Renderer.h>
#include "PotiEngine/Window.h"
#include "PotiEngine/Vector2.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <iostream>

namespace PotiEngine
{
	ImGuiRenderer::ImGuiRenderer(Window& window, Renderer& renderer)
	{
		// Setup imgui
		IMGUI_CHECKVERSION();
		m_context = ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGui_ImplSDL2_InitForSDLRenderer(window.GetHandle(), renderer.GetHandle());
		ImGui_ImplSDLRenderer2_Init(renderer.GetHandle());
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		// Cleanup
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext(m_context);
	}

	ImGuiContext* ImGuiRenderer::GetContext()
	{
		return m_context;
	}

	void ImGuiRenderer::Button()
	{
		if (ImGui::Button("Pause", ImVec2(1,1)))
		{
			std::cout << "feur" << std::endl;
		}
	}

	void ImGuiRenderer::Render()
	{
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiRenderer::NewFrame()
	{
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::ProcessEvent(SDL_Event& event)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

}