/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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
