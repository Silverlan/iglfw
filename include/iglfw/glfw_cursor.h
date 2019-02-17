/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __GLFW_CURSOR_H__
#define __GLFW_CURSOR_H__

#include "glfw_includes.h"
#include <sharedutils/def_handle.h>
#include <vector>
#include <memory>
#include <mathutil/uvec.h>

namespace GLFW
{
	class Cursor;
	DECLARE_BASE_HANDLE(DLLGLFW,Cursor,Cursor);

	class DLLGLFW Cursor
	{
	public:
		enum class DLLGLFW Shape : uint32_t
		{
			Default = 0,
			Hidden = 1,
			Arrow = GLFW_ARROW_CURSOR,
			IBeam = GLFW_IBEAM_CURSOR,
			Crosshair = GLFW_CROSSHAIR_CURSOR,
			Hand = GLFW_HAND_CURSOR,
			HResize = GLFW_HRESIZE_CURSOR,
			VResize = GLFW_VRESIZE_CURSOR
		};
	private:
		Cursor(GLFWcursor *cursor);
		GLFWcursor *m_cursor;
		CursorHandle m_handle;
	public:
		static std::unique_ptr<Cursor> Create(uint32_t width,uint32_t height,unsigned char *data,const Vector2i &hotSpot=Vector2i(0,0));
		static std::unique_ptr<Cursor> Create(Shape shape);
		~Cursor();
		CursorHandle GetHandle();
		void Remove();
		const GLFWcursor *GetGLFWCursor() const;
	};
};

#endif
