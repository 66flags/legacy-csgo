#pragma once

namespace ui {
	class window : public base_control {
	public:
		window ( const std::string &name, const rect_t &area ) {
			this->m_type = control_type_t::CONTROL_WINDOW;
			this->m_name = name;
			this->m_area = area;
		}

		std::deque < std::shared_ptr< ui::tab > > tabs { };

		__forceinline std::shared_ptr< ui::tab > add_tab ( const std::string &name ) {
			auto tab = std::make_shared< ui::tab > ( name );

			tabs.emplace_back ( tab );

			return tab;
		}

		vec2_t m_delta = { 0.0f, 0.0f };
		bool m_drag = false;

		void paint ( ) override;
		void think ( ) override;
	};
}