#pragma once

namespace ui {
	class textbox : public base_control {
	public:
		textbox ( const std::string &name, int size, std::string *data ) {
			this->m_type = control_type_t::CONTROL_TEXTBOX;
			this->m_name = name;
			this->m_data = data;
			this->m_size = size;
			this->m_offset = 10.f;
		}

		std::string *m_data = nullptr;
		int m_size = 0;
		bool m_set = false;

		void paint ( ) override;
		void think ( ) override;
	};
}