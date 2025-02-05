#include "PotiEngine/InputManager.h"
#include <stdexcept>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <iostream>


namespace PotiEngine
{
	InputManager::InputManager()
	{
		if (instance != nullptr)
			throw std::runtime_error("only one ResourceManager can be created");

		instance = this;

	}

	InputManager::~InputManager()
	{
		instance = nullptr;
	}


	void InputManager::BindKeyPressed(SDL_Keycode input, std::string actionName)
	{
		if (!actionName.empty())
			keyBindMap[input] = std::move(actionName);
		else
			keyBindMap.erase(input);
	}

	void InputManager::BindMouseButtonPressed(MouseButton button, std::string action)
	{
		// Plutôt que de traduire depuis notre enum vers les defines de la SDL à chaque événement
		// on peut le faire une seule fois au binding (plus efficace)
		int mouseButton;
		switch (button)
		{
		case MouseButton::Left:   mouseButton = SDL_BUTTON_LEFT;   break;
		case MouseButton::Right:  mouseButton = SDL_BUTTON_RIGHT;  break;
		case MouseButton::Middle: mouseButton = SDL_BUTTON_MIDDLE; break;
		case MouseButton::X1:     mouseButton = SDL_BUTTON_X1;     break;
		case MouseButton::X2:     mouseButton = SDL_BUTTON_X2;     break;
		default:
			return; //< ne devrait pas arriver
		}

		if (!action.empty())
			mouseBindMap[mouseButton] = std::move(action);
		else
			mouseBindMap.erase(mouseButton);
	}

	

	
	void InputManager::BindAction(std::string actionName, std::function<void(bool /*active*/)> func)
	{
		actionBindMap[actionName].callback = std::move(func);
		
	}

	void InputManager::TriggerAction(const std::string& action)
	{
		ActionData& actionData = actionBindMap[action];
		if (actionData.inputCount++ == 0)
		{
			if (actionData.callback)
				actionData.callback(true);
		}
	}

	void InputManager::ReleaseAction(const std::string& action)
	{
		auto it = actionBindMap.find(action);
		if (it == actionBindMap.end())
			return;

		ActionData& actionData = it->second;
		if (--actionData.inputCount == 0)
		{
			if (actionData.callback)
				actionData.callback(false);
		}
	}

	void InputManager::HandleEvent(const SDL_Event& event)
	{
		switch (event.type)
		{

			case SDL_KEYDOWN:
			{
				if (event.key.repeat != 0)
					break;

				auto it = keyBindMap.find(event.key.keysym.sym);
				if (it != keyBindMap.end())
					TriggerAction(it->second);

				break;
			}

			case SDL_KEYUP:
			{
				auto it = keyBindMap.find(event.key.keysym.sym);
				if (it != keyBindMap.end())
					ReleaseAction(it->second);

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				auto it = mouseBindMap.find(event.button.button);
				if (it != mouseBindMap.end())
					TriggerAction(it->second);

				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				auto it = mouseBindMap.find(event.button.button);
				if (it != mouseBindMap.end())
					ReleaseAction(it->second);

				break;
			}
		}
	}

	bool InputManager::IsActive(const std::string& action) const
	{
		auto it = actionBindMap.find(action);
		if (it == actionBindMap.end())
			return false;

		const ActionData& actionData = it->second;
		return actionData.inputCount > 0;
	}


	InputManager& InputManager::Instance()
	{
		if (instance == nullptr)
			throw std::runtime_error("ResourceManager hasn't been instantied");

		return *instance;
	}

	

	InputManager* InputManager::instance = nullptr;

}
