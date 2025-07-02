// SPDX-FileCopyrightText: © 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include <memory>
#ifdef _WIN32
#include <Ole2.h>
#endif

module pragma.platform;

#ifdef _WIN32

import :file_drop_target;

void pragma::platform::Window::InitFileDropHandler()
{
	m_fileDropTarget = std::make_unique<FileDropTarget>(*this);
	RegisterDragDrop(GetWin32Handle(), m_fileDropTarget.get());
}
void pragma::platform::Window::ReleaseFileDropHandler()
{
	RevokeDragDrop(GetWin32Handle());
	m_fileDropTarget->Release();
	m_fileDropTarget = {};
}
#endif
