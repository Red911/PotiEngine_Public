#pragma once
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <PotiEngine/Export.h>
#include <functional>

namespace PotiEngine
{
	enum class MouseButton
	{
		Left,
		Right,
		Middle,
		X1,
		X2
	};
	class POTI_ENGINE_API InputManager
	{

	public:
		InputManager();
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;

		InputManager& operator=(const InputManager&) = delete;
		InputManager& operator=(InputManager&&) = delete;

		~InputManager();

		static InputManager& Instance();


	public :

		void BindKeyPressed(SDL_Keycode input, std::string actionName);

		// Appuyer sur le bouton "button" déclenchera "action"
		void BindMouseButtonPressed(MouseButton button, std::string action);

		
		void BindAction(std::string actionName, std::function<void(bool /*active*/)> func);

		void TriggerAction(const std::string& action);
		void ReleaseAction(const std::string& action);

		void HandleEvent(const SDL_Event& event);

		bool IsActive(const std::string& action) const;



	private:


		struct ActionData
		{
			std::function<void(bool /*active*/)> callback;
			unsigned int inputCount = 0;
		};

		static InputManager* instance;

		std::map<SDL_Keycode, std::string> keyBindMap;
		std::unordered_map<int /*mouseButton*/, std::string /*action*/> mouseBindMap;
		std::map<std::string, ActionData> actionBindMap;

		
	};
}



