/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

module;

#include "interface/definitions.hpp"
#include <GLFW/glfw3.h>
#include <memory>

export module pragma.platform:joystick_handler;

import :joystick;

namespace pragma::platform {
	class JoystickHandler {
	  public:
		static JoystickHandler &GetInstance();
		static void Release();
		~JoystickHandler();

		enum class JoystickState : decltype(GLFW_CONNECTED) { Connected = GLFW_CONNECTED, Disconnected = GLFW_DISCONNECTED };

		void SetJoystickButtonCallback(const std::function<void(const Joystick &, uint32_t, KeyState, KeyState)> &callback);
		void SetJoystickAxisCallback(const std::function<void(const Joystick &, uint32_t, float, float)> &callback);
		void SetJoystickStateCallback(const std::function<void(const Joystick &, JoystickState)> &callback);
		const std::vector<std::shared_ptr<Joystick>> &GetJoysticks() const;
		void Poll();
	  private:
		JoystickHandler();
		std::vector<std::shared_ptr<Joystick>> m_joysticks;
		std::function<void(const Joystick &, uint32_t, KeyState, KeyState)> m_joystickButtonCallback = nullptr;
		std::function<void(const Joystick &, uint32_t, float, float)> m_joystickAxisCallback = nullptr;
		std::function<void(const Joystick &, JoystickState)> m_joystickStateCallback = nullptr;
	};
};
