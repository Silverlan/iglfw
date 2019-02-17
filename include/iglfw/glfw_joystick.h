/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __GLFW_JOYSTICK_H__
#define __GLFW_JOYSTICK_H__

#include "glfw_includes.h"
#include "glfw_keys.h"
#include <string>
#include <vector>
#include <mathutil/uvec.h>
#include <mathutil/color.h>
#include <functional>
#include <memory>

#pragma warning(push)
#pragma warning(disable : 4251)
namespace GLFW
{
	class DLLGLFW Joystick
	{
	public:
		static std::shared_ptr<Joystick> Create(int32_t joystickId);
		std::string GetName() const;
		int32_t GetJoystickId() const;
		const std::vector<float> &GetAxes() const;
		const std::vector<KeyState> &GetButtons() const;
		void Poll();
		void SetButtonCallback(const std::function<void(uint32_t,KeyState,KeyState)> &callback);
		void SetAxisCallback(const std::function<void(uint32_t,float,float)> &callback);
	private:
		Joystick(int32_t joystickId);
		int32_t m_joystickId = -1;

		std::vector<KeyState> m_oldButtonStates;
		std::vector<KeyState> m_buttonStates;

		std::vector<float> m_oldAxes;
		std::vector<float> m_axes;

		std::function<void(uint32_t,KeyState,KeyState)> m_buttonCallback = nullptr;
		std::function<void(uint32_t,float,float)> m_axisCallback = nullptr;
		void InitializeButtonStates(int32_t count);
		void InitializeAxes(int32_t count);
	};
};
#pragma warning(pop)

#endif
