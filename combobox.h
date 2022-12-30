#pragma once

namespace ui {
	class combobox : public base_control {
	public:
		combobox ( const std::string &name, int *value, const std::deque< std::string > &items ) {
			this->m_type = control_type_t::CONTROL_COMBOBOX;
			this->m_name = name;
			this->m_value = value;
			this->m_items = items;
			this->m_offset = 10.f;
		}

		int *m_value = nullptr;
		std::deque< std::string > m_items;
		rect_t m_focus_area;
		rect_t m_dropdown_area;

		void paint ( ) override;
		void think ( ) override;
	};
}