/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "impl_glfw_joystick_handler.h"
#include <algorithm>

using namespace GLFW;

static std::shared_ptr<JoystickHandler> s_joystickHandler = nullptr;
JoystickHandler &JoystickHandler::GetInstance()
{
	if(s_joystickHandler == nullptr)
		s_joystickHandler = std::shared_ptr<JoystickHandler>(new JoystickHandler());
	return *s_joystickHandler;
}
void JoystickHandler::Release() { s_joystickHandler = nullptr; }

JoystickHandler::JoystickHandler()
{
	m_joysticks.reserve(GLFW_JOYSTICK_LAST - GLFW_JOYSTICK_1 + 1);
	for(auto i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i) {
		auto r = glfwJoystickPresent(i);
		if(r == GLFW_FALSE)
			return;
		m_joysticks.push_back(Joystick::Create(i));
		if(m_joystickStateCallback != nullptr)
			m_joystickStateCallback(*m_joysticks.back(), JoystickState::Connected);
		auto &joystick = m_joysticks.back();
		auto *ptrJoystick = joystick.get();
		joystick->SetButtonCallback([this, ptrJoystick](uint32_t button, GLFW::KeyState oldState, GLFW::KeyState newState) {
			if(m_joystickButtonCallback == nullptr)
				return;
			m_joystickButtonCallback(*ptrJoystick, button, oldState, newState);
		});
		joystick->SetAxisCallback([this, ptrJoystick](uint32_t axis, float oldVal, float newVal) {
			if(m_joystickAxisCallback == nullptr)
				return;
			m_joystickAxisCallback(*ptrJoystick, axis, oldVal, newVal);
		});
	}

	glfwSetJoystickCallback([](int joystickId, int eventId) {
		auto *handler = s_joystickHandler.get();
		switch(eventId) {
		case GLFW_CONNECTED:
			handler->m_joysticks.push_back(Joystick::Create(joystickId));
			if(handler->m_joystickStateCallback != nullptr)
				handler->m_joystickStateCallback(*handler->m_joysticks.back(), JoystickState::Connected);
			break;
		default:
			auto it = std::find_if(handler->m_joysticks.begin(), handler->m_joysticks.end(), [joystickId](const std::shared_ptr<Joystick> &joystick) { return (joystick->GetJoystickId() == joystickId) ? true : false; });
			if(it == handler->m_joysticks.end())
				return;
			if(handler->m_joystickStateCallback != nullptr)
				handler->m_joystickStateCallback(*(*it), JoystickState::Disconnected);
			break;
		}
	});
}

JoystickHandler::~JoystickHandler() { glfwSetJoystickCallback(nullptr); }

void JoystickHandler::SetJoystickButtonCallback(const std::function<void(const Joystick &, uint32_t, KeyState, KeyState)> &callback) { m_joystickButtonCallback = callback; }
void JoystickHandler::SetJoystickAxisCallback(const std::function<void(const Joystick &, uint32_t, float, float)> &callback) { m_joystickAxisCallback = callback; }
void JoystickHandler::SetJoystickStateCallback(const std::function<void(const Joystick &, JoystickState)> &callback)
{
	m_joystickStateCallback = callback;
	if(callback == nullptr)
		return;
	for(auto &joystick : m_joysticks)
		callback(*joystick, JoystickState::Connected);
}
const std::vector<std::shared_ptr<Joystick>> &JoystickHandler::GetJoysticks() const { return m_joysticks; }

void JoystickHandler::Poll()
{
	for(auto &joystick : m_joysticks)
		joystick->Poll();
}
