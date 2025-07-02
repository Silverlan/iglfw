// SPDX-FileCopyrightText: © 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#ifndef __GLFW_DEFINITIONS_H__
#define __GLFW_DEFINITIONS_H__

#ifdef DLLGLFW_EX
#ifdef __linux__
#define DLLGLFW __attribute__((visibility("default")))
#else
#define DLLGLFW __declspec(dllexport)
#endif
#else
#ifdef __linux__
#define DLLGLFW
#else
#define DLLGLFW __declspec(dllimport)
#endif
#endif

#endif
