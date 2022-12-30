#pragma once

namespace ui {
	class tab : public base_control {
	public:
		tab ( const std::string &name ) {
			this->m_type = control_type_t::CONTROL_TAB;
			this->m_name = name;
		}

		std::deque< std::shared_ptr< ui::group > > childs;

		__forceinline std::shared_ptr< ui::group > add_child ( const std::string &name, const rect_t &area ) {
			auto child = std::make_shared< ui::group > ( name, area );

			childs.push_back ( child );

			return child;
		}

		bool m_visible;

		void paint ( ) override;
		void think ( ) override;
	};
}