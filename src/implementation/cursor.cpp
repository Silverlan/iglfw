// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include <GLFW/glfw3.h>

module pragma.platform;

using namespace pragma::platform;

Cursor::Cursor(GLFWcursor *cursor) : m_handle(util::create_handle<Cursor>(this)), m_cursor(cursor) {}

Cursor::~Cursor()
{
	m_handle.Invalidate();
	glfwDestroyCursor(m_cursor);
}

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

Cursor &Cursor::GetStandardCursor(Shape shape)
{
	static std::unordered_map<pragma::platform::Cursor::Shape, std::unique_ptr<pragma::platform::Cursor>> g_standardCursors;
	auto it = g_standardCursors.find(shape);
	if(it == g_standardCursors.end()) {
		auto *glfwCursor = glfwCreateStandardCursor(static_cast<int>(shape));
		std::unique_ptr<Cursor> cursor {new Cursor(glfwCursor)};
		it = g_standardCursors.insert(std::make_pair(shape, std::move(cursor))).first;
	}
	return *it->second;
}
