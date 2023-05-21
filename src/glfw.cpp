/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "iglfw/glfw.h"
#include "impl_glfw_joystick_handler.h"
#include "iglfw/glfw_window.h"

#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "mathutil.lib")

static bool bIsInitialized = false;
static GLFW::JoystickHandler *s_joystickHandler = nullptr;
bool GLFW::initialize()
{
	auto r = (glfwInit() != GLFW_FALSE) ? true : false;
	bIsInitialized = r;
	return r;
}

void GLFW::terminate()
{
	if(bIsInitialized == false)
		return;
	set_joysticks_enabled(false);
	glfwTerminate();
}

bool GLFW::is_initialized() { return bIsInitialized; }

void GLFW::get_version(int *major, int *minor, int *rev) { glfwGetVersion(major, minor, rev); }

std::string GLFW::get_version_string() { return glfwGetVersionString(); }

void GLFW::poll_events()
{
	glfwPollEvents();
	for(auto *window : GLFW::Window::GetWindows())
		window->Poll();
}
void GLFW::poll_joystick_events()
{
	if(s_joystickHandler != nullptr)
		s_joystickHandler->Poll();
}
void GLFW::wait_events() { glfwWaitEvents(); }
void GLFW::post_empty_events() { glfwPostEmptyEvent(); }
double GLFW::get_time() { return glfwGetTime(); }
void GLFW::set_time(double t) { glfwSetTime(t); }

static std::function<void(GLFW::Monitor, bool)> monitor_callback = nullptr;
void GLFW::set_monitor_callback(const std::function<void(Monitor, bool)> &callback)
{
	monitor_callback = callback;
	glfwSetMonitorCallback([](GLFWmonitor *monitor, int ev) { monitor_callback(Monitor(monitor), (ev == GLFW_CONNECTED) ? true : false); });
}
void GLFW::set_joystick_state_callback(const std::function<void(const Joystick &, bool)> &callback)
{
	if(s_joystickHandler == nullptr)
		return;
	if(callback == nullptr) {
		s_joystickHandler->SetJoystickStateCallback(nullptr);
		return;
	}
	s_joystickHandler->SetJoystickStateCallback([callback](const GLFW::Joystick &joystick, GLFW::JoystickHandler::JoystickState state) { callback(joystick, (state == GLFW::JoystickHandler::JoystickState::Connected) ? true : false); });
}
void GLFW::set_joystick_button_callback(const std::function<void(const Joystick &, uint32_t, KeyState, KeyState)> &callback)
{
	if(s_joystickHandler == nullptr)
		return;
	s_joystickHandler->SetJoystickButtonCallback(callback);
}
void GLFW::set_joystick_axis_callback(const std::function<void(const Joystick &, uint32_t, float, float)> &callback)
{
	if(s_joystickHandler == nullptr)
		return;
	s_joystickHandler->SetJoystickAxisCallback(callback);
}

void GLFW::set_joysticks_enabled(bool b)
{
	if(is_initialized() == false)
		return;
	if(b == false) {
		if(s_joystickHandler != nullptr) {
			s_joystickHandler->Release();
			s_joystickHandler = nullptr;
		}
		return;
	}
	if(s_joystickHandler != nullptr)
		return;
	s_joystickHandler = &JoystickHandler::GetInstance();
}

static auto s_axisThreshold = 0.f;
void GLFW::set_joystick_axis_threshold(float threshold) { s_axisThreshold = threshold; }
float GLFW::get_joystick_axis_threshold() { return s_axisThreshold; }

const std::vector<std::shared_ptr<GLFW::Joystick>> &GLFW::get_joysticks()
{
	if(s_joystickHandler == nullptr) {
		static std::vector<std::shared_ptr<GLFW::Joystick>> r {};
		return r;
	}
	return s_joystickHandler->GetJoysticks();
}

std::string GLFW::get_joystick_name(uint32_t joystickId)
{
	auto &joysticks = get_joysticks();
	if(joystickId >= joysticks.size())
		return "";
	return joysticks.at(joystickId)->GetName();
}
const std::vector<float> &GLFW::get_joystick_axes(uint32_t joystickId)
{
	auto &joysticks = get_joysticks();
	if(joystickId >= joysticks.size()) {
		static std::vector<float> r {};
		return r;
	}
	return joysticks.at(joystickId)->GetAxes();
}
const std::vector<GLFW::KeyState> &GLFW::get_joystick_buttons(uint32_t joystickId)
{
	auto &joysticks = get_joysticks();
	if(joystickId >= joysticks.size()) {
		static std::vector<GLFW::KeyState> r {};
		return r;
	}
	return joysticks.at(joystickId)->GetButtons();
}

GLFW::Monitor GLFW::get_primary_monitor() { return GLFW::Monitor(glfwGetPrimaryMonitor()); }
std::vector<GLFW::Monitor> GLFW::get_monitors()
{
	std::vector<GLFW::Monitor> r;
	int count = 0;
	auto *monitors = glfwGetMonitors(&count);
	r.reserve(count);
	for(auto i = decltype(count) {0}; i < count; ++i)
		r.push_back(Monitor(monitors[i]));
	return r;
}
