#pragma once

namespace ui {
	class button : public base_control {
	public:
		button ( const std::string &name, const std::function< void ( ) > &fn ) {
			this->m_type = control_type_t::CONTROL_BUTTON;
			this->m_name = name;
			this->m_callback = fn;
		}

		std::function< void ( ) > m_callback = nullptr;

		bool m_click = false;
		bool m_held = false;

		void paint ( ) override;
		void think ( ) override;
	};
}