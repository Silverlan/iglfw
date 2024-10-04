/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __GLFW_FILE_DROP_TARGET_HPP__
#define __GLFW_FILE_DROP_TARGET_HPP__

#ifdef _WIN32

#include "iglfw/glfw_window.h"
#include <shlobj.h>

namespace GLFW {
	class FileDropTarget : public IDropTarget {
	  public:
		FileDropTarget(GLFW::Window &window) : refCount {1}, m_window {window} {}

		HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject) override
		{
			if(riid == IID_IUnknown || riid == IID_IDropTarget) {
				*ppvObject = static_cast<IDropTarget *>(this);
				AddRef();
				return S_OK;
			}
			*ppvObject = nullptr;
			return E_NOINTERFACE;
		}

		ULONG __stdcall AddRef() override { return InterlockedIncrement(&refCount); }

		ULONG __stdcall Release() override
		{
			ULONG count = InterlockedDecrement(&refCount);
			return count;
		}

		HRESULT __stdcall DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override
		{
			FORMATETC fmt = {CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
			if(pDataObj->QueryGetData(&fmt) == S_OK) {
				*pdwEffect = DROPEFFECT_COPY; // Allow copy if files are dragged
			}
			else {
				*pdwEffect = DROPEFFECT_NONE;
			}
			m_window.DragEnterCallback();
			return S_OK;
		}

		HRESULT __stdcall DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override
		{
			*pdwEffect = DROPEFFECT_COPY; // Show "copy" effect when hovering
			return S_OK;
		}

		HRESULT __stdcall DragLeave() override
		{
			m_window.DragExitCallback();
			return S_OK;
		}

		HRESULT __stdcall Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override
		{
			FORMATETC fmt = {CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
			STGMEDIUM stg = {0};

			std::vector<std::string> droppedFiles;
			if(SUCCEEDED(pDataObj->GetData(&fmt, &stg))) {
				HDROP hDrop = static_cast<HDROP>(GlobalLock(stg.hGlobal));
				if(hDrop != nullptr) {
					UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);

					// Process each file
					droppedFiles.reserve(fileCount);
					for(UINT i = 0; i < fileCount; i++) {
						char fileName[MAX_PATH];
						if(DragQueryFile(hDrop, i, fileName, MAX_PATH))
							droppedFiles.push_back(fileName);
					}
					GlobalUnlock(stg.hGlobal);
				}
				ReleaseStgMedium(&stg);
			}
			if(!droppedFiles.empty()) {
				std::vector<const char *> droppedFilesC;
				droppedFilesC.reserve(droppedFiles.size());
				for(auto &f : droppedFiles)
					droppedFilesC.push_back(f.c_str());
				m_window.DropCallback(droppedFilesC.size(), droppedFilesC.data());
			}

			*pdwEffect = DROPEFFECT_COPY;
			return S_OK;
		}
	  private:
		LONG refCount;
		GLFW::Window &m_window;
	};
};
#endif

#endif
