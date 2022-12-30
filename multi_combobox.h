#pragma once

namespace ui {
	class multi_combobox : public base_control {
	public:
		multi_combobox ( const std::string &name, std::deque< bool * > value, const std::deque< std::string > &items ) {
			this->m_type = control_type_t::CONTROL_MULTICOMBOBOX;
			this->m_name = name;
			this->m_value = value;
			this->m_items = items;
			this->m_offset = 10.f;
		}

		std::deque< bool * > m_value;
		std::deque< std::string > m_items;

		rect_t m_focus_area;
		rect_t m_dropdown_area;

		void paint ( ) override;
		void think ( ) override;
	};
}