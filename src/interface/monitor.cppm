/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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
