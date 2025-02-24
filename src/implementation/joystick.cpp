/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

module;

#include <cstring>
#include <cassert>
#include <memory>
#include <string>
#include <algorithm>
#include <mathutil/uvec.h>
#include <GLFW/glfw3.h>

module pragma.platform;

using namespace pragma::platform;

std::shared_ptr<Joystick> Joystick::Create(int32_t joystickId) { return std::shared_ptr<Joystick>(new Joystick(joystickId)); }
Joystick::Joystick(int32_t joystickId) : m_joystickId(joystickId) {}

void Joystick::InitializeAxes(int32_t count)
{
	if(m_axes.size() == count)
		return;
	m_axes.resize(count);
	std::fill(m_axes.begin(), m_axes.end(), 0.f);
	m_oldAxes = m_axes;
}

void Joystick::InitializeButtonStates(int32_t count)
{
	if(m_buttonStates.size() == count)
		return;
	m_buttonStates.resize(count);
	std::fill(m_buttonStates.begin(), m_buttonStates.end(), KeyState::Release);
	m_oldButtonStates = m_buttonStates;
}

int32_t Joystick::GetJoystickId() const { return m_joystickId; }

std::string Joystick::GetName() const { return glfwGetJoystickName(m_joystickId); }

const std::vector<float> &Joystick::GetAxes() const { return m_axes; }
const std::vector<KeyState> &Joystick::GetButtons() const { return m_buttonStates; }
void Joystick::SetButtonCallback(const std::function<void(uint32_t, KeyState, KeyState)> &callback) { m_buttonCallback = callback; }
void Joystick::SetAxisCallback(const std::function<void(uint32_t, float, float)> &callback) { m_axisCallback = callback; }

void Joystick::Poll()
{
	// Update axes
	m_oldAxes = m_axes;
	int count = 0;
	const float *values = glfwGetJoystickAxes(GetJoystickId(), &count);
	if(values != nullptr) {
		InitializeAxes(count);
		memcpy(m_axes.data(), values, sizeof(values[0]) * count);
		auto threshold = pragma::platform::get_joystick_axis_threshold();
		for(auto &axis : m_axes) {
			if(umath::abs(axis) < threshold)
				axis = 0.f;
		}
	}
	else
		std::fill(m_axes.begin(), m_axes.end(), 0.f);
	assert(m_oldAxes.size() == m_axes.size());
	if(m_axisCallback != nullptr) {
		for(auto i = decltype(m_oldAxes.size()) {0}; i < m_oldAxes.size(); ++i) {
			auto oldAxis = m_oldAxes.at(i);
			auto newAxis = m_axes.at(i);
			if(umath::abs(newAxis) > 0.f || umath::abs(oldAxis) > 0.f) // oldAxis > 0.f && newAxis == 0.f means the axis has been "unpressed"
				m_axisCallback(i, oldAxis, newAxis);
		}
	}

	// Update buttons
	m_oldButtonStates = m_buttonStates;
	count = 0;
	auto *states = glfwGetJoystickButtons(GetJoystickId(), &count);
	if(states != nullptr) {
		InitializeButtonStates(count);
		for(auto i = decltype(count) {0}; i < count; ++i)
			m_buttonStates.at(i) = static_cast<KeyState>(states[i]);
	}
	else
		std::fill(m_buttonStates.begin(), m_buttonStates.end(), KeyState::Release);
	assert(m_oldButtonStates.size() == m_buttonStates.size());
	if(m_buttonCallback != nullptr) {
		for(auto i = decltype(m_oldButtonStates.size()) {0}; i < m_oldButtonStates.size(); ++i) {
			auto oldState = m_oldButtonStates.at(i);
			auto newState = m_buttonStates.at(i);
			if(oldState == newState)
				continue;
			m_buttonCallback(i, oldState, newState);
		}
	}
}
