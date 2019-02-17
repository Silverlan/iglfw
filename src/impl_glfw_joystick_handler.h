#ifndef __IMPL_GLFW_JOYSTICK_HANDLER_H__
#define __IMPL_GLFW_JOYSTICK_HANDLER_H__

#include "iglfw/glfw_definitions.h"
#include "iglfw/glfw_joystick.h"
#include <memory>

namespace GLFW
{
	class JoystickHandler
	{
	public:
		static JoystickHandler &GetInstance();
		static void Release();
		~JoystickHandler();

		enum class JoystickState : decltype(GLFW_CONNECTED)
		{
			Connected = GLFW_CONNECTED,
			Disconnected = GLFW_DISCONNECTED
		};

		void SetJoystickButtonCallback(const std::function<void(const Joystick&,uint32_t,KeyState,KeyState)> &callback);
		void SetJoystickAxisCallback(const std::function<void(const Joystick&,uint32_t,float,float)> &callback);
		void SetJoystickStateCallback(const std::function<void(const Joystick&,JoystickState)> &callback);
		const std::vector<std::shared_ptr<Joystick>> &GetJoysticks() const;
		void Poll();
	private:
		JoystickHandler();
		std::vector<std::shared_ptr<Joystick>> m_joysticks;
		std::function<void(const Joystick&,uint32_t,KeyState,KeyState)> m_joystickButtonCallback = nullptr;
		std::function<void(const Joystick&,uint32_t,float,float)> m_joystickAxisCallback = nullptr;
		std::function<void(const Joystick&,JoystickState)> m_joystickStateCallback = nullptr;
	};
};

#endif
