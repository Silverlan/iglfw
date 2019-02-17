/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __GLFW_DEFINITIONS_H__
#define __GLFW_DEFINITIONS_H__

#ifdef DLLGLFW_EX
	#ifdef __linux__
		#define DLLGLFW __attribute__((visibility("default")))
	#else
 	  #define DLLGLFW  __declspec(dllexport)
	#endif
#else
	#ifdef __linux__
		#define DLLGLFW
	#else
		#define DLLGLFW  __declspec(dllimport)
	#endif
#endif

#endif