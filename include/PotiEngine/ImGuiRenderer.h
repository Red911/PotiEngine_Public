#pragma once

#include "PotiEngine/Export.h"

union SDL_Event;
struct ImGuiContext;

namespace PotiEngine {

	class Renderer;
	class Window;

	class POTI_ENGINE_API ImGuiRenderer
	{
		public:
			ImGuiRenderer(Window& window, Renderer& renderer);
			ImGuiRenderer(const ImGuiRenderer&) = delete;
			ImGuiRenderer(ImGuiRenderer&&) = delete;
			~ImGuiRenderer();

			ImGuiContext* GetContext();

			void Button();

			void Render();

			void NewFrame();

			void ProcessEvent(SDL_Event& event);

	

			ImGuiRenderer& operator=(const ImGuiRenderer&) = delete;
			ImGuiRenderer& operator=(ImGuiRenderer&&) = delete;

		private:
			ImGuiContext* m_context;
	};
}

