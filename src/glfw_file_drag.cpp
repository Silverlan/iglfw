/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifdef _WIN32
#include "file_drop_target.hpp"
#include "iglfw/glfw_window.h"

void GLFW::Window::InitFileDropHandler()
{
	m_fileDropTarget = std::make_unique<FileDropTarget>(*this);
	RegisterDragDrop(GetWin32Handle(), m_fileDropTarget.get());
}
void GLFW::Window::ReleaseFileDropHandler()
{
	RevokeDragDrop(GetWin32Handle());
	m_fileDropTarget->Release();
	m_fileDropTarget = {};
}
#endif
