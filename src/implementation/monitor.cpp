// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include <array>
#include <string>
#include <GLFW/glfw3.h>

module pragma.platform;

import :monitor;

using namespace pragma::platform;

Monitor::Monitor(GLFWmonitor *monitor) : m_monitor(monitor) {}

Monitor::~Monitor() {}

std::string Monitor::GetName() const { return glfwGetMonitorName(m_monitor); }
const GLFWmonitor *Monitor::GetGLFWMonitor() const { return m_monitor; }

Vector2i Monitor::GetPhysicalSize() const
{
	int w = 0;
	int h = 0;
	glfwGetMonitorPhysicalSize(m_monitor, &w, &h);
	return Vector2i(w, h);
}

Vector2i Monitor::GetPos() const
{
	int x = 0;
	int y = 0;
	glfwGetMonitorPos(m_monitor, &x, &y);
	return Vector2i(x, y);
}

std::vector<Vector3i> Monitor::GetGammaRamp() const
{
	auto *gamma = glfwGetGammaRamp(m_monitor);
	std::vector<Vector3i> r(gamma->size);
	for(auto i = decltype(gamma->size) {0}; i < gamma->size; ++i) {
		r[i][0] = gamma->red[i];
		r[i][1] = gamma->green[i];
		r[i][2] = gamma->blue[i];
	}
	return r;
}

void Monitor::SetGammaRamp(const std::vector<Vector3i> gammaRamp) const
{
	GLFWgammaramp gamma {};
	gamma.size = static_cast<uint32_t>(gammaRamp.size());
	std::array<std::vector<std::remove_reference<decltype(*gamma.red)>::type>, 3> values;
	for(auto it = values.begin(); it != values.end(); ++it)
		it->reserve(gamma.size);
	for(auto it = gammaRamp.begin(); it != gammaRamp.end(); ++it) {
		values[0].push_back((*it)[0]);
		values[1].push_back((*it)[1]);
		values[2].push_back((*it)[2]);
	}
	gamma.red = values[0].data();
	gamma.green = values[1].data();
	gamma.blue = values[2].data();
	glfwSetGammaRamp(m_monitor, &gamma);
}

void Monitor::SetGamma(float gamma) const { glfwSetGamma(m_monitor, gamma); }

Monitor::VideoMode Monitor::GetVideoMode() const { return *glfwGetVideoMode(m_monitor); }

std::vector<Monitor::VideoMode> Monitor::GetSupportedVideoModes() const
{
	std::vector<Monitor::VideoMode> modes;
	int count = 0;
	auto *videoModes = glfwGetVideoModes(m_monitor, &count);
	modes.reserve(count);
	for(auto i = decltype(count) {0}; i < count; ++i) {
		auto &mode = videoModes[i];
		modes.push_back(mode);
	}
	return modes;
}
