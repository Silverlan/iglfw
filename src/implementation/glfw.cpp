// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#ifdef _WIN32
#include <oleidl.h>
#endif
#include <string>
#include <memory>
#include <functional>

#include <GLFW/glfw3.h>

module pragma.platform;

import :joystick_handler;

static bool g_initialized = false;
static bool g_headless = false;
static pragma::platform::JoystickHandler *s_joystickHandler = nullptr;

bool pragma::platform::initialize(std::string &outErr, bool headless)
{
	if(g_initialized)
		return g_initialized;
#ifdef _WIN32
	// On Windows we can just use the Win32 platform for headless rendering
	headless = false;
#endif
	g_headless = headless;
	if(headless)
		glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);
	auto res = glfwInit();
	if(res != GLFW_TRUE) {
		const char *description;
		int code = glfwGetError(&description);
		if(description)
			outErr = std::string {description} + " (" + std::to_string(code) + ")!";
		else
			outErr = "error " + std::to_string(code) + "!";
		return false;
	}

	g_initialized = true;
#ifdef _WIN32
	OleInitialize(nullptr);
#endif
	return g_initialized;
}

void pragma::platform::terminate()
{
	if(g_initialized == false)
		return;
	set_joysticks_enabled(false);
	glfwTerminate();
#ifdef _WIN32
	OleUninitialize();
#endif
}

pragma::platform::Platform pragma::platform::get_platform()
{
	auto platform = glfwGetPlatform();
	switch(platform) {
	case GLFW_PLATFORM_WIN32:
		return Platform::Win32;
	case GLFW_PLATFORM_COCOA:
		return Platform::Cocoa;
	case GLFW_PLATFORM_WAYLAND:
		return Platform::Wayland;
	case GLFW_PLATFORM_X11:
		return Platform::X11;
	case GLFW_PLATFORM_NULL:
		return Platform::Windowless;
	default:
		break;
	}
	return Platform::Unknown;
}

void pragma::platform::set_swap_interval(int interval) { glfwSwapInterval(interval); }

bool pragma::platform::is_initialized() { return g_initialized; }
bool pragma::platform::is_headless() { return g_headless; }

void pragma::platform::get_version(int *major, int *minor, int *rev) { glfwGetVersion(major, minor, rev); }

std::string pragma::platform::get_version_string() { return glfwGetVersionString(); }

void pragma::platform::poll_events()
{
	glfwPollEvents();
	for(auto *window : pragma::platform::Window::GetWindows())
		window->Poll();
}
void pragma::platform::poll_joystick_events()
{
	if(s_joystickHandler != nullptr)
		s_joystickHandler->Poll();
}
void pragma::platform::wait_events() { glfwWaitEvents(); }
void pragma::platform::post_empty_events() { glfwPostEmptyEvent(); }
double pragma::platform::get_time() { return glfwGetTime(); }
void pragma::platform::set_time(double t) { glfwSetTime(t); }

static std::function<void(pragma::platform::Monitor, bool)> monitor_callback = nullptr;
void pragma::platform::set_monitor_callback(const std::function<void(Monitor, bool)> &callback)
{
	monitor_callback = callback;
	glfwSetMonitorCallback([](GLFWmonitor *monitor, int ev) { monitor_callback(Monitor(monitor), (ev == GLFW_CONNECTED) ? true : false); });
}
void pragma::platform::set_joystick_state_callback(const std::function<void(const Joystick &, bool)> &callback)
{
	if(s_joystickHandler == nullptr)
		return;
	if(callback == nullptr) {
		s_joystickHandler->SetJoystickStateCallback(nullptr);
		return;
	}
	s_joystickHandler->SetJoystickStateCallback([callback](const pragma::platform::Joystick &joystick, pragma::platform::JoystickHandler::JoystickState state) { callback(joystick, (state == pragma::platform::JoystickHandler::JoystickState::Connected) ? true : false); });
}
void pragma::platform::set_joystick_button_callback(const std::function<void(const Joystick &, uint32_t, KeyState, KeyState)> &callback)
{
	if(s_joystickHandler == nullptr)
		return;
	s_joystickHandler->SetJoystickButtonCallback(callback);
}
void pragma::platform::set_joystick_axis_callback(const std::function<void(const Joystick &, uint32_t, float, float)> &callback)
{
	if(s_joystickHandler == nullptr)
		return;
	s_joystickHandler->SetJoystickAxisCallback(callback);
}

void pragma::platform::set_joysticks_enabled(bool b)
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
void pragma::platform::set_joystick_axis_threshold(float threshold) { s_axisThreshold = threshold; }
float pragma::platform::get_joystick_axis_threshold() { return s_axisThreshold; }

const std::vector<std::shared_ptr<pragma::platform::Joystick>> &pragma::platform::get_joysticks()
{
	if(s_joystickHandler == nullptr) {
		static std::vector<std::shared_ptr<pragma::platform::Joystick>> r {};
		return r;
	}
	return s_joystickHandler->GetJoysticks();
}

std::string pragma::platform::get_joystick_name(uint32_t joystickId)
{
	auto &joysticks = get_joysticks();
	if(joystickId >= joysticks.size())
		return "";
	return joysticks.at(joystickId)->GetName();
}
const std::vector<float> &pragma::platform::get_joystick_axes(uint32_t joystickId)
{
	auto &joysticks = get_joysticks();
	if(joystickId >= joysticks.size()) {
		static std::vector<float> r {};
		return r;
	}
	return joysticks.at(joystickId)->GetAxes();
}
const std::vector<pragma::platform::KeyState> &pragma::platform::get_joystick_buttons(uint32_t joystickId)
{
	auto &joysticks = get_joysticks();
	if(joystickId >= joysticks.size()) {
		static std::vector<pragma::platform::KeyState> r {};
		return r;
	}
	return joysticks.at(joystickId)->GetButtons();
}

pragma::platform::Monitor pragma::platform::get_primary_monitor() { return pragma::platform::Monitor(glfwGetPrimaryMonitor()); }
std::vector<pragma::platform::Monitor> pragma::platform::get_monitors()
{
	std::vector<pragma::platform::Monitor> r;
	int count = 0;
	auto *monitors = glfwGetMonitors(&count);
	r.reserve(count);
	for(auto i = decltype(count) {0}; i < count; ++i)
		r.push_back(Monitor(monitors[i]));
	return r;
}
