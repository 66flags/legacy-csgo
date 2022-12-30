#pragma once

namespace ui {
	class group : public base_control {
	public:
		group ( const std::string &name, const rect_t &area ) {
			this->m_type = control_type_t::CONTROL_GROUP;
			this->m_name = name;
			this->m_draw_area = area;
			this->m_offset = { 0.f, 0.f };
		}

		std::deque < std::shared_ptr< ui::base_control > > objects { };

		__forceinline std::shared_ptr< ui::colorpicker > add_colorpicker ( const std::string &name, color_t *value, int sub_offset = 0, bool alpha_bar = true ) {
			auto object = std::make_shared< ui::colorpicker > ( name, value, sub_offset, alpha_bar );

			objects.emplace_back ( object );

			return object;
		}

		__forceinline std::shared_ptr< ui::button > add_button ( const std::string &name, const std::function< void ( ) > &fn ) {
			auto object = std::make_shared< ui::button > ( name, fn );

			objects.emplace_back ( object );

			return object;
		}

		__forceinline std::shared_ptr< ui::keybind > add_keybind ( const std::string &name, int *key, int *key_type ) {
			auto object = std::make_shared< ui::keybind > ( name, key, key_type );

			objects.emplace_back ( object );

			return object;
		}

		__forceinline std::shared_ptr< ui::slider > add_slider ( const std::string &name, float *value, float min, float max ) {
			auto object = std::make_shared< ui::slider > ( name, value, min, max );

			objects.emplace_back ( object );

			return object;
		}

		__forceinline std::shared_ptr< ui::textbox > add_textbox ( const std::string &name, int size, std::string *data ) {
			auto object = std::make_shared< ui::textbox > ( name, size, data );

			objects.emplace_back ( object );

			return object;
		}

		__forceinline std::shared_ptr< ui::multi_combobox > add_multi_combobox ( const std::string &name, std::deque< bool * > value, const std::deque< std::string > &items ) {
			auto object = std::make_shared< ui::multi_combobox > ( name, value, items );

			objects.emplace_back ( object );

			return object;
		}

		__forceinline std::shared_ptr< ui::checkbox > add_checkbox ( const std::string &name, bool *value ) {
			auto object = std::make_shared< ui::checkbox > ( name, value );

			objects.emplace_back ( object );

			return object;
		}

		__forceinline std::shared_ptr< ui::combobox > add_combobox ( const std::string &name, int *value, const std::deque< std::string > &items ) {
			auto object = std::make_shared< ui::combobox > ( name, value, items );

			objects.emplace_back ( object );

			return object;
		}

		vec2_t m_offset = { 0.f, 0.f };

		bool m_should_scroll = false;
		int	m_first_pad = 0;
		int	m_last_pad = 0;
		float m_scroll_offset = 0.f;

		void paint ( ) override;
		void think ( ) override;
	};
}