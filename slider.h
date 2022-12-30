#pragma once

namespace ui {
	class slider : public base_control {
	public:
		slider ( const std::string &name, float *value, float min, float max ) {
			this->m_type = control_type_t::CONTROL_SLIDER;
			this->m_name = name;
			this->m_value = value;
			this->m_min = min;
			this->m_max = max;
			this->m_offset = 11.f;
		}

		float *m_value;
		float m_min;
		float m_max;
		bool m_drag = false;
		float m_fill;

		void paint ( ) override;
		void think ( ) override;
	};
}