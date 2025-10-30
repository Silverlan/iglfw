// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#ifdef _WIN32

#include <shlobj.h>

#endif

export module pragma.platform:file_drop_target;

import :window;

#ifdef _WIN32

export namespace pragma::platform {
	class FileDropTarget : public IDropTarget {
	  public:
		FileDropTarget(pragma::platform::Window &window) : refCount {1}, m_window {window} {}

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
						wchar_t fileName[MAX_PATH];

						if(DragQueryFileW(hDrop, i, fileName, MAX_PATH))
							droppedFiles.push_back(ustring::wstring_to_string(fileName));
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
		pragma::platform::Window &m_window;
	};
};
#endif
