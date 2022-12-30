#pragma once

namespace ui {
	struct vec2_t {
		float x, y;
	};

	enum control_type_t : int {
		CONTROL_WINDOW,
		CONTROL_TAB,
		CONTROL_TEXTBOX,
		CONTROL_SUBTAB,
		CONTROL_GROUP,
		CONTROL_CHECKBOX,
		CONTROL_COMBOBOX,
		CONTROL_COLORPICKER,
		CONTROL_KEYBIND,
		CONTROL_MULTICOMBOBOX,
		CONTROL_BUTTON,
		CONTROL_SLIDER
	};

	struct key_code_info_t {
		int vk;
		char regular;
		char shift;
	};

	class base_control {
	public:
		enum focus_type_t : int {
			FOCUS_NONE,
			FOCUS_CONTROL,
			FOCUS_CONTEXT
		};

		focus_type_t m_focus_type { focus_type_t::FOCUS_NONE };

		rect_t m_draw_area = { 0.0f, 0.0f, 0.0f, 0.0f };
		rect_t m_area = { 0.0f, 0.0f, 0.0f, 0.0f };
		float m_offset;

		bool m_hovered = false;
		int	m_page = 0;

		base_control *m_parent = nullptr;

		__forceinline bool blocked ( ) {
			if ( !inputsys.hovered ( { m_parent->m_area.x, m_parent->m_area.y + 18.f, m_parent->m_area.w, m_parent->m_area.h - 18.f } ) ) {
				this->m_hovered = false;
				return true;
			}

			return false;
		}

		control_type_t m_type;
		std::string m_name;
		float m_padding;

		virtual void paint ( ) = 0;
		virtual void think ( ) = 0;
	};

	inline base_control *focused = nullptr;
	inline color_t stored_color = color_t ( 0, 0, 0, 255 );
	inline double scroll_delta = 0.0f;

	__forceinline long wndproc ( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
		switch ( msg ) {
		case WM_MOUSEWHEEL:
			scroll_delta += GET_WHEEL_DELTA_WPARAM ( wparam ) > 0 ? -1.0 : 1.0;
			return true;
		}

		return true;
	}
}