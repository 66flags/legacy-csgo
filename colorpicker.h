#pragma once

namespace ui {
	class colorpicker : public base_control {
	public:
		colorpicker ( const std::string &name, color_t *value, int sub_offset = 0, bool alpha_bar = true ) {
			this->m_type = control_type_t::CONTROL_COLORPICKER;
			this->m_name = name;
			this->m_value = value;
			this->m_alpha_bar = alpha_bar;
			this->m_sub_offset = sub_offset;

			m_value->to_hsv ( m_new_hsv [ 0 ], m_new_hsv [ 1 ], m_new_hsv [ 2 ] );

			m_action_names = {
				"Copy",
				"Paste"
			};
		}

		enum color_type_t {
			NONE = -1,
			BOX,
			OPACITY,
			HUE
		};

		rect_t m_context_area;
		rect_t m_focus_area;
		rect_t m_item_area;
		rect_t m_drop_area;
		vec2_t m_opacity_mouse;
		vec2_t m_hue_mouse;
		rect_t m_box_rect;
		rect_t m_box_picker_rect;
		rect_t m_hue_picker_rect;
		rect_t m_opacity_rect;

		int m_sub_offset = 0;
		bool m_alpha_bar = true;
		bool m_drag = false;

		std::deque < std::string > m_action_names;
		std::array < float, 3 > m_new_hsv;

		color_type_t m_area_type = NONE;
		color_t *m_value = nullptr;

		void paint ( ) override;
		void think ( ) override;
	};
}