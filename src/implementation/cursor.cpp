// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include <sharedutils/def_handle.h>
#include <mathutil/uvec.h>
#include <GLFW/glfw3.h>

module pragma.platform;

using namespace pragma::platform;

DEFINE_BASE_HANDLE(, Cursor, Cursor);

Cursor::Cursor(GLFWcursor *cursor) : m_handle(new PtrCursor(this)), m_cursor(cursor) {}

Cursor::~Cursor() { glfwDestroyCursor(m_cursor); }

CursorHandle Cursor::GetHandle() { return m_handle; }

void Cursor::Remove() { delete this; }

const GLFWcursor *Cursor::GetGLFWCursor() const { return m_cursor; }

////////////////////////

std::unique_ptr<Cursor> Cursor::Create(uint32_t width, uint32_t height, unsigned char *data, const Vector2i &hotSpot)
{
	GLFWimage image {};
	image.width = width;
	image.height = height;
	image.pixels = data;

	auto *cursor = glfwCreateCursor(&image, hotSpot.x, hotSpot.y);
	return std::unique_ptr<Cursor>(new Cursor(cursor));
}

std::unique_ptr<Cursor> Cursor::Create(Shape shape)
{
	auto *cursor = glfwCreateStandardCursor(static_cast<int>(shape));
	return std::unique_ptr<Cursor>(new Cursor(cursor));
}
