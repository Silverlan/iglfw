// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "includes.hpp"

export module pragma.platform:joystick;

import :keys;

#pragma warning(push)
#pragma warning(disable : 4251)
export namespace pragma::platform {
	class DLLGLFW Joystick {
	  public:
		static std::shared_ptr<Joystick> Create(int32_t joystickId);
		std::string GetName() const;
		int32_t GetJoystickId() const;
		const std::vector<float> &GetAxes() const;
		const std::vector<KeyState> &GetButtons() const;
		void Poll();
		void SetButtonCallback(const std::function<void(uint32_t, KeyState, KeyState)> &callback);
		void SetAxisCallback(const std::function<void(uint32_t, float, float)> &callback);
	  private:
		Joystick(int32_t joystickId);
		int32_t m_joystickId = -1;

		std::vector<KeyState> m_oldButtonStates;
		std::vector<KeyState> m_buttonStates;

		std::vector<float> m_oldAxes;
		std::vector<float> m_axes;

		std::function<void(uint32_t, KeyState, KeyState)> m_buttonCallback = nullptr;
		std::function<void(uint32_t, float, float)> m_axisCallback = nullptr;
		void InitializeButtonStates(int32_t count);
		void InitializeAxes(int32_t count);
	};
};
#pragma warning(pop)
