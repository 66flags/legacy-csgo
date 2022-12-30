#pragma once

namespace ui {
	class checkbox : public base_control {
	public:
		checkbox ( const std::string &name, bool *value ) {
			this->m_type = control_type_t::CONTROL_CHECKBOX;
			this->m_name = name;
			this->m_value = value;
		}

		bool *m_value = nullptr;

		void paint ( ) override;
		void think ( ) override;
	};
}