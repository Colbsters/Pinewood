#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Window.h>
#include <PWMath/Vector2.h>

#include <bitset>
#include <mutex>

namespace Pinewood
{
	class KeyboardInput
	{
	public:
		KeyboardInput() = default;
		KeyboardInput(const KeyboardInput&) = default;
		inline ~KeyboardInput()
		{
			Destroy();
		}

		inline Result Create(const Window& window)
		{
			Result result;

			// Make sure no other window is going to call our event handlers (ex: we were already initialized)
			result = Destroy();
			if (IsError(result) && (result != Result::NotInitialized)) // If it's not initialized then it's ok
				return result;

			m_window = window;

			result = m_window.AddEventHandler({
				.function = KeyDownHandler,
				.userPointer = this,
				.event = WindowEventCode::KeyDown
				});
			if (IsError(result))
				return result;

			result = m_window.AddEventHandler({
				.function = KeyUpHandler,
				.userPointer = this,
				.event = WindowEventCode::KeyUp
				});
			if (IsError(result))
				return result;

			return Result::Success;
		}

		inline Result Destroy()
		{
			if (IsInitialized())
			{
				Result result;

				result = m_window.RemoveEventHandler({
					.function = KeyDownHandler,
					.userPointer = this,
					.event = WindowEventCode::KeyDown
					});
				if (IsError(result))
					return result;

				result = m_window.RemoveEventHandler({
					.function = KeyUpHandler,
					.userPointer = this,
					.event = WindowEventCode::KeyUp
					});
				if (IsError(result))
					return result;

				return Result::Success;
			}
			else
				return Result::NotInitialized;
		}

		inline bool IsKeyPressed(KeyCode key)
		{
			std::lock_guard guard{ m_mutex };
			return m_keys[static_cast<uint32_t>(key) % 256];
		}

		inline const Window& GetWindow() { return m_window; }

		inline bool IsInitialized() { return m_window.IsInitialized(); }

	private:
		static inline void KeyDownHandler(const Window&, const WindowEvent& e, void* ptr)
		{
			const WindowKeyEvent& event = static_cast<const WindowKeyEvent&>(e);
			auto* keyboard = reinterpret_cast<KeyboardInput*>(ptr);
			std::lock_guard guard{ keyboard->m_mutex };

			keyboard->m_keys[static_cast<uint32_t>(event.key) % 256] = true;
		}

		static inline void KeyUpHandler(const Window&, const WindowEvent& e, void* ptr)
		{
			const WindowKeyEvent& event = static_cast<const WindowKeyEvent&>(e);
			auto* keyboard = reinterpret_cast<KeyboardInput*>(ptr);
			std::lock_guard guard{ keyboard->m_mutex };

			keyboard->m_keys[static_cast<uint32_t>(event.key) % 256] = false;
		}

		std::mutex m_mutex;
		Window m_window;
		std::bitset<256> m_keys;
	};

	class MouseInput
	{
	public:
		MouseInput() = default;
		MouseInput(const MouseInput&) = default;
		inline ~MouseInput()
		{
			Destroy();
		}

		inline Result Create(const Window& window)
		{
			Result result;

			// Make sure no other window is going to call our event handlers (ex: we were already initialized)
			result = Destroy();
			if (IsError(result) && (result != Result::NotInitialized)) // If it's not initialized then it's ok
				return result;

			m_window = window;

			result = m_window.AddEventHandler({
				.function = ButtonDownHandler,
				.userPointer = this,
				.event = WindowEventCode::MouseButtonDown
				});
			if (IsError(result))
				return result;

			result = m_window.AddEventHandler({
				.function = ButtonUpHandler,
				.userPointer = this,
				.event = WindowEventCode::MouseButtonUp
				});
			if (IsError(result))
				return result;

			result = m_window.AddEventHandler({
				.function = MoveHandler,
				.userPointer = this,
				.event = WindowEventCode::MouseMove
				});
			if (IsError(result))
				return result;

			result = m_window.AddEventHandler({
				.function = ScrollHandler,
				.userPointer = this,
				.event = WindowEventCode::MouseScroll
				});
			if (IsError(result))
				return result;

			return Result::Success;
		}

		inline Result Destroy()
		{
			if (IsInitialized())
			{
				Result result;

				result = m_window.RemoveEventHandler({
					.function = ButtonDownHandler,
					.userPointer = this,
					.event = WindowEventCode::MouseButtonDown
					});
				if (IsError(result))
					return result;

				result = m_window.RemoveEventHandler({
					.function = ButtonUpHandler,
					.userPointer = this,
					.event = WindowEventCode::MouseButtonUp
					});
				if (IsError(result))
					return result;

				result = m_window.RemoveEventHandler({
					.function = MoveHandler,
					.userPointer = this,
					.event = WindowEventCode::MouseMove
					});
				if (IsError(result))
					return result;

				result = m_window.RemoveEventHandler({
					.function = ScrollHandler,
					.userPointer = this,
					.event = WindowEventCode::MouseScroll
					});
				if (IsError(result))
					return result;

				return Result::Success;
			}
			else
				return Result::NotInitialized;
		}

		inline MouseButton GetButtons() { std::lock_guard guard{ m_mutex }; return m_buttons; }
		inline int16_t GetX() { std::lock_guard guard{ m_mutex };  return m_x; }
		inline int16_t GetY() { std::lock_guard guard{ m_mutex }; return m_y; }
		inline PWMath::Vector2I16 GetPosition() { std::lock_guard guard{ m_mutex }; return { m_x, m_y }; }
		inline float GetScrollDelta() { std::lock_guard guard{ m_mutex }; return m_scrollDelta; }

		inline const Window& GetWindow() { return m_window; }

		inline bool IsInitialized() { return m_window.IsInitialized(); }

		inline void ResetScrollDelta() { std::lock_guard guard{ m_mutex }; m_scrollDelta = 0; }

	private:
		static inline void ButtonDownHandler(const Window&, const WindowEvent& e, void* ptr)
		{
			const WindowMouseEvent& event = static_cast<const WindowMouseEvent&>(e);
			auto* mouse = reinterpret_cast<MouseInput*>(ptr);
			std::lock_guard guard{ mouse->m_mutex };

			// Mark this mouse button as pressed
			mouse->m_buttons |= (event.buttons & MouseButton::AllButtons);

			// Update the keyboard keys
			mouse->m_buttons = (mouse->m_buttons & MouseButton::AllButtons) | (event.buttons & MouseButton::AllKeys);

			// Update position
			mouse->m_x = event.x;
			mouse->m_y = event.y;
		}

		static inline void ButtonUpHandler(const Window&, const WindowEvent& e, void* ptr)
		{
			const WindowMouseEvent& event = static_cast<const WindowMouseEvent&>(e);
			auto* mouse = reinterpret_cast<MouseInput*>(ptr);
			std::lock_guard guard{ mouse->m_mutex };

			// Mark this mouse button as not pressed
			mouse->m_buttons &= ~(event.buttons & MouseButton::AllButtons);

			// Update the keyboard keys
			mouse->m_buttons = (mouse->m_buttons & MouseButton::AllButtons) | (event.buttons & MouseButton::AllKeys);
			
			// Update position
			mouse->m_x = event.x;
			mouse->m_y = event.y;
		}

		static inline void MoveHandler(const Window&, const WindowEvent& e, void* ptr)
		{
			const WindowMouseEvent& event = static_cast<const WindowMouseEvent&>(e);
			auto* mouse = reinterpret_cast<MouseInput*>(ptr);
			std::lock_guard guard{ mouse->m_mutex };

			// Update the buttons and keys
			mouse->m_buttons = mouse->m_buttons;

			// Update position
			mouse->m_x = event.x;
			mouse->m_y = event.y;
		}

		static inline void ScrollHandler(const Window&, const WindowEvent& e, void* ptr)
		{
			const WindowMouseScrollEvent& event = static_cast<const WindowMouseScrollEvent&>(e);
			auto* mouse = reinterpret_cast<MouseInput*>(ptr);
			std::lock_guard guard{ mouse->m_mutex };

			// Update the buttons and keys
			mouse->m_buttons = mouse->m_buttons;

			// Update position
			mouse->m_x = event.x;
			mouse->m_y = event.y;

			// Add the scroll delta
			mouse->m_scrollDelta += event.scroll;
		}

		std::mutex m_mutex;

		Window m_window;
		MouseButton m_buttons = MouseButton::Null;
		int16_t m_x = 0, m_y = 0;
		float m_scrollDelta = 0;
	};
}
