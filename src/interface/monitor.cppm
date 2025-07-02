// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "includes.hpp"
#include <string>
#include <vector>
#include <mathutil/uvec.h>
#include <mathutil/color.h>

export module pragma.platform:monitor;

export namespace pragma::platform {
	class DLLGLFW Monitor {
	  public:
		using VideoMode = GLFWvidmode;
	  private:
		GLFWmonitor *m_monitor;
	  public:
		Monitor(GLFWmonitor *monitor);
		~Monitor();
		const GLFWmonitor *GetGLFWMonitor() const;
		std::string GetName() const;
		Vector2i GetPhysicalSize() const;
		Vector2i GetPos() const;
		std::vector<Vector3i> GetGammaRamp() const;
		void SetGammaRamp(const std::vector<Vector3i> gammaRamp) const;
		void SetGamma(float gamma) const;
		VideoMode GetVideoMode() const;
		std::vector<VideoMode> GetSupportedVideoModes() const;
	};
};
