// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "includes.hpp"
#include <vector>
#include <memory>

export module pragma.platform:cursor;

import pragma.math;
import pragma.util;

export {
	namespace pragma::platform {
		enum class CursorMode : uint32_t { Normal = GLFW_CURSOR_NORMAL, Hidden = GLFW_CURSOR_HIDDEN, Disabled = GLFW_CURSOR_DISABLED };

		class Cursor;
		using CursorHandle = util::THandle<Cursor>;

		class DLLGLFW Cursor {
		  public:
			enum class DLLGLFW Shape : uint32_t { Default = 0, Hidden = 1, Arrow = GLFW_ARROW_CURSOR, IBeam = GLFW_IBEAM_CURSOR, Crosshair = GLFW_CROSSHAIR_CURSOR, Hand = GLFW_HAND_CURSOR, HResize = GLFW_HRESIZE_CURSOR, VResize = GLFW_VRESIZE_CURSOR };
		  private:
			Cursor(GLFWcursor *cursor);
			GLFWcursor *m_cursor;
			CursorHandle m_handle;
		  public:
			static std::unique_ptr<Cursor> Create(uint32_t width, uint32_t height, unsigned char *data, const Vector2i &hotSpot = Vector2i(0, 0));
			static Cursor &GetStandardCursor(Shape shape);
			~Cursor();
			CursorHandle GetHandle();
			void Remove();
			const GLFWcursor *GetGLFWCursor() const;
		};
		using namespace umath::scoped_enum::arithmetic;
	};

	namespace umath::scoped_enum::arithmetic {
		template<>
		struct enable_arithmetic_operators<pragma::platform::CursorMode> : std::true_type {};
	}
}
