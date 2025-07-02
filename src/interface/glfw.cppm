// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "includes.hpp"
#include <mathutil/umath.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

export module pragma.platform:core;

import :monitor;
import :joystick;

export namespace pragma::platform {
	enum class Platform : uint8_t {
		Win32 = 0,
		Cocoa,
		Wayland,
		X11,
		Unknown,

		Count,
	};
	DLLGLFW bool initialize();
	DLLGLFW void terminate();
	DLLGLFW Platform get_platform();
	DLLGLFW void get_version(int *major, int *minor, int *rev);
	DLLGLFW std::string get_version_string();
	DLLGLFW void poll_events();
	DLLGLFW void poll_joystick_events();
	DLLGLFW void wait_events();
	DLLGLFW void post_empty_events();
	DLLGLFW double get_time();
	DLLGLFW void set_time(double t);
	DLLGLFW void set_monitor_callback(const std::function<void(Monitor, bool)> &callback);
	DLLGLFW void set_joystick_state_callback(const std::function<void(const Joystick &, bool)> &callback);
	DLLGLFW void set_joystick_button_callback(const std::function<void(const Joystick &, uint32_t, KeyState, KeyState)> &callback);
	DLLGLFW void set_joystick_axis_callback(const std::function<void(const Joystick &, uint32_t, float, float)> &callback);
	DLLGLFW void set_joysticks_enabled(bool b);
	DLLGLFW void set_joystick_axis_threshold(float threshold);
	DLLGLFW float get_joystick_axis_threshold();
	DLLGLFW const std::vector<std::shared_ptr<Joystick>> &get_joysticks();
	DLLGLFW std::string get_joystick_name(uint32_t joystickId);
	DLLGLFW const std::vector<float> &get_joystick_axes(uint32_t joystickId);
	DLLGLFW const std::vector<KeyState> &get_joystick_buttons(uint32_t joystickId);
	DLLGLFW Monitor get_primary_monitor();
	DLLGLFW std::vector<Monitor> get_monitors();
	DLLGLFW bool is_initialized();
	DLLGLFW void set_swap_interval(int interval);
};
