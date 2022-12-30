#pragma once

namespace ui {
	class keybind : public base_control {
	public:
		keybind ( const std::string &name, int *key, int *key_type ) {
			this->m_type = control_type_t::CONTROL_KEYBIND;
			this->m_name = name;
			this->m_key = key;
			this->m_key_type = key_type;

			m_key_names = {
				"Disabled",
				"Always On",
				"Hold",
				"Toggle"
			};
		}

		rect_t m_focus_area = { 0.f, 0.f, 0.f, 0.f };
		rect_t m_box_rect = { 0.f, 0.f, 0.f, 0.f };
		rect_t m_item_area = { 0.f, 0.f, 0.f, 0.f };

		int *m_key = nullptr;
		int *m_key_type = nullptr;

		bool m_type_set = false;
		bool m_set = false, m_old_set = false;

		std::deque< std::string > m_key_names { };

		void paint ( ) override;
		void think ( ) override;
	};
}