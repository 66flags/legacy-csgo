#include "includes.h"

static color_t hue_colors [ 7 ] = {
	{ 255.f, 0.f, 0.f },
	{ 255.f, 255.f, 0.f },
	{ 0.f, 255.f, 0.f },
	{ 0.f, 255.f, 255.f },
	{ 0.f, 0.f, 255.f },
	{ 255.f, 0.f, 255.f },
	{ 255.f, 0.f, 0.f }
};

void ui::colorpicker::paint ( ) {
	auto checker_draw = [ ] ( rect_t region ) {
		const auto square_side_len = region.h * 0.5f;
		auto alpha_clr_flip = false;
		const auto remaining_len = std::fmodf ( region.w, square_side_len );

		for ( auto i = 0.0f; i < region.w; i += square_side_len ) {
			const auto calc_len = i > region.w - square_side_len ? remaining_len : square_side_len;

			draw_list::rect_filled ( region.x + i, region.y, calc_len, calc_len, alpha_clr_flip ? color_t { 170, 170, 170, 255 } : color_t { 20, 20, 20, 255 } );
			draw_list::rect_filled ( region.x + i, region.y + calc_len, calc_len, calc_len, alpha_clr_flip ? color_t { 20, 20, 20, 255 } : color_t { 170, 170, 170, 255 } );

			alpha_clr_flip = !alpha_clr_flip;
		}
	};

	checker_draw ( this->m_item_area );
	draw_list::gradient ( this->m_item_area.x, this->m_item_area.y, this->m_item_area.w, this->m_item_area.h, *this->m_value, { m_value->r * 0.75f, m_value->g * 0.75f, m_value->b * 0.75f, static_cast< float >( m_value->a ) }, true );
	draw_list::rect_outlined ( this->m_item_area.x, this->m_item_area.y, this->m_item_area.w, this->m_item_area.h, { 0, 0, 0 } );

	if ( focused != this )
		return;

	if ( this->m_focus_type == focus_type_t::FOCUS_CONTROL ) {
		color_t picker_gradient { 255, 255, 255, 255 };

		picker_gradient.from_hsv ( this->m_new_hsv [ 0 ], 1.0f, 1.0f ); {
			draw_list::rect_filled ( this->m_box_rect.x, this->m_box_rect.y, this->m_box_rect.w, this->m_box_rect.h, { 18, 18, 18, 255 } );
			draw_list::rect_outlined ( this->m_box_rect.x, this->m_box_rect.y, this->m_box_rect.w, this->m_box_rect.h, { 12, 12, 12, 255 } );
			draw_list::multi_gradient ( this->m_box_picker_rect.x, this->m_box_picker_rect.y, this->m_box_picker_rect.w, this->m_box_picker_rect.h, { 255, 255, 255, 255 }, picker_gradient, picker_gradient, { 255, 255, 255, 255 } );
			draw_list::multi_gradient ( this->m_box_picker_rect.x, this->m_box_picker_rect.y, this->m_box_picker_rect.w, this->m_box_picker_rect.h, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 255 }, { 0, 0, 0, 255 } );
			draw_list::rect_outlined ( this->m_box_picker_rect.x, this->m_box_picker_rect.y, this->m_box_picker_rect.w, this->m_box_picker_rect.h, { 12, 12, 12, 255 } );

			for ( int i = 0; i < 6; i++ )
				draw_list::multi_gradient ( this->m_hue_picker_rect.x, this->m_hue_picker_rect.y + ( i * ( this->m_hue_picker_rect.h / 6 ) ), this->m_hue_picker_rect.w, this->m_hue_picker_rect.h / 6, hue_colors [ i ], hue_colors [ i ], hue_colors [ i + 1 ], hue_colors [ i + 1 ] );

			draw_list::rect_outlined ( this->m_hue_picker_rect.x, this->m_hue_picker_rect.y, this->m_hue_picker_rect.w, this->m_hue_picker_rect.h, { 12, 12, 12, 255 } );

			if ( this->m_alpha_bar ) {
				checker_draw ( this->m_opacity_rect );
				draw_list::gradient ( this->m_opacity_rect.x, this->m_opacity_rect.y, this->m_opacity_rect.w, this->m_opacity_rect.h, { static_cast< float >( m_value->r ), static_cast< float >( m_value->g ), static_cast< float >( m_value->b ), 0.f }, *this->m_value, false );
				draw_list::rect_outlined ( this->m_opacity_rect.x, this->m_opacity_rect.y, this->m_opacity_rect.w, this->m_opacity_rect.h, { 12, 12, 12, 255 } );
			}
		}

		vec2_t box_indicator_pos = {
			std::clamp< float > ( static_cast< int >( this->m_box_picker_rect.x + ( std::clamp< float > ( m_new_hsv [ 1 ], 0.f, 1.f ) * static_cast< float >( this->m_box_picker_rect.w ) ) ), this->m_box_picker_rect.x, this->m_box_picker_rect.x + this->m_box_picker_rect.w - 4 ),
			std::clamp< float > ( static_cast< int >( this->m_box_picker_rect.y + ( std::clamp< float > ( 1.0f - m_new_hsv [ 2 ], 0.f, 1.f ) * static_cast< float >( this->m_box_picker_rect.h ) ) ), this->m_box_picker_rect.y, this->m_box_picker_rect.y + this->m_box_picker_rect.h - 4 )
		};

		draw_list::rect_filled ( box_indicator_pos.x, box_indicator_pos.y, 4, 4, { 255, 255, 255, 50 } );
		draw_list::rect_outlined ( box_indicator_pos.x, box_indicator_pos.y, 4, 4, { 12, 12, 12, 255 } );

		vec2_t hue_indicator_pos = {
			this->m_hue_picker_rect.x,
			std::clamp< float > ( static_cast< int >( this->m_hue_picker_rect.y + ( std::clamp< float > ( m_new_hsv [ 0 ], 0.f, 1.f ) * static_cast< float >( this->m_hue_picker_rect.h ) ) ), this->m_hue_picker_rect.y, this->m_hue_picker_rect.y + this->m_hue_picker_rect.h - 3 )
		};

		draw_list::rect_filled ( hue_indicator_pos.x, hue_indicator_pos.y, this->m_hue_picker_rect.w, 3, { 255, 255, 255, 50 } );
		draw_list::rect_outlined ( hue_indicator_pos.x, hue_indicator_pos.y, this->m_hue_picker_rect.w, 3, { 12, 12, 12, 255 } );

		vec2_t opacity_indicator_pos = {
			std::clamp< float > ( this->m_opacity_rect.x, this->m_opacity_rect.x + ( std::clamp< float > ( m_value->a / 255.f, 0.f, 1.f ) * static_cast< float >( this->m_opacity_rect.w ) - 3 ), this->m_opacity_rect.x + this->m_opacity_rect.w ),
			this->m_opacity_rect.y
		};

		if ( this->m_alpha_bar ) {
			draw_list::rect_filled ( opacity_indicator_pos.x, opacity_indicator_pos.y, 3, m_opacity_rect.h, { 255, 255, 255, 50 } );
			draw_list::rect_outlined ( opacity_indicator_pos.x, opacity_indicator_pos.y, 3, m_opacity_rect.h, { 12, 12, 12, 255 } );
		}
	}

	else if ( this->m_focus_type == focus_type_t::FOCUS_CONTEXT ) {
		draw_list::rect_filled ( this->m_context_area.x, this->m_context_area.y, this->m_context_area.w, this->m_context_area.h, { 18, 18, 18 } );

		for ( int i = 0; i < this->m_action_names.size ( ); i++ ) {
			auto item_dim = draw_list::menu.size ( this->m_action_names [ i ] );

			bool hovered = inputsys.hovered ( { this->m_context_area.x, this->m_context_area.y + ( i * 15.f ), m_context_area.w, 15.f } );

			draw_list::rect_filled ( this->m_context_area.x, this->m_context_area.y + ( i * 15.f ), this->m_context_area.w, 15.f, { 18, 18, 18 } );
			draw_list::menu.string ( this->m_context_area.x + this->m_context_area.w - item_dim.m_width - 3.f, this->m_context_area.y + ( i * 15.f ) + 2.5f, hovered ? color_t { 91, 164, 255 } : color_t { 150, 150, 150 }, this->m_action_names [ i ], draw_list::ALIGN_LEFT, false, true );
		}

		draw_list::rect_outlined ( this->m_context_area.x, this->m_context_area.y, this->m_context_area.w, this->m_context_area.h, { 0, 0, 0, 255 } );
	}
}

void ui::colorpicker::think ( ) {
	auto dim = draw_list::menu.size ( m_name );

	this->m_item_area = { this->m_area.x + 163.f - m_sub_offset, this->m_area.y + 1, this->m_area.w, this->m_area.h };

	if ( !this->blocked ( ) ) {
		this->m_hovered = inputsys.hovered ( this->m_item_area );

		if ( this->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) ) {
			this->m_focus_type = focus_type_t::FOCUS_CONTROL;
			m_value->to_hsv ( m_new_hsv [ 0 ], m_new_hsv [ 1 ], m_new_hsv [ 2 ] );
			focused = this;
		}

		if ( this->m_hovered && inputsys.key_pressed ( VK_RBUTTON ) ) {
			this->m_focus_type = focus_type_t::FOCUS_CONTEXT;
			focused = this;
		}
	}

	this->m_box_rect = { this->m_item_area.x, this->m_item_area.y + this->m_item_area.h + 2.f, 174.f, this->m_alpha_bar ? 172.f : 157.f };
	this->m_box_picker_rect = { this->m_item_area.x + 4, this->m_item_area.y + this->m_item_area.h + 5.f, 150.f, 150.f };
	this->m_hue_picker_rect = { this->m_item_area.x + 157.f, this->m_item_area.y + this->m_item_area.h + 5.f, 14.f, 150.f };

	if ( focused != this )
		return;

	if ( this->m_focus_type == focus_type_t::FOCUS_CONTROL ) {
		this->m_opacity_rect = { this->m_box_picker_rect.x, this->m_box_picker_rect.y + this->m_box_picker_rect.h + 4.f, 150.f, 10.f };
		this->m_opacity_mouse = { this->m_opacity_rect.x + int ( std::roundf ( this->m_value->a / 255.f * this->m_opacity_rect.w ) ), this->m_opacity_rect.y / 2 };

		{
			if ( inputsys.key_pressed ( VK_LBUTTON ) ) {
				if ( inputsys.hovered ( this->m_box_picker_rect ) )
					this->m_area_type = color_type_t::BOX;

				else if ( inputsys.hovered ( this->m_hue_picker_rect ) )
					this->m_area_type = color_type_t::HUE;

				if ( this->m_alpha_bar ) {
					if ( inputsys.hovered ( this->m_opacity_rect ) )
						this->m_area_type = color_type_t::OPACITY;
				}
			}
		}

		{
			if ( this->m_area_type != color_type_t::NONE && inputsys.key_down ( VK_LBUTTON ) ) {
				this->m_drag = true;

				switch ( this->m_area_type ) {
				case color_type_t::HUE: {
					this->m_hue_mouse = { this->m_hue_picker_rect.h / 2, std::clamp< float > ( inputsys.m_pos.y, this->m_hue_picker_rect.y, this->m_hue_picker_rect.y + this->m_hue_picker_rect.h ) };
					this->m_new_hsv [ 0 ] = std::clamp ( float ( this->m_hue_mouse.y - this->m_hue_picker_rect.y ) / float ( this->m_hue_picker_rect.h - 1 ), 0.0f, 1.0f );
				} break;
				case color_type_t::OPACITY: {
					if ( this->m_alpha_bar )
						this->m_opacity_mouse = { std::clamp< float > ( inputsys.m_pos.x, this->m_opacity_rect.x, this->m_opacity_rect.x + this->m_opacity_rect.w ), this->m_opacity_rect.h / 2 };
				} break;
				case color_type_t::BOX: {
					point_t mouse_delta = { inputsys.m_pos.x - this->m_box_picker_rect.x, inputsys.m_pos.y - this->m_box_picker_rect.y };

					this->m_new_hsv [ 1 ] = std::clamp ( ( float ) mouse_delta.x / ( ( float ) m_box_picker_rect.w ), 0.f, 1.f );
					this->m_new_hsv [ 2 ] = 1.0f - std::clamp ( ( float ) mouse_delta.y / ( ( float ) m_box_picker_rect.h ), 0.f, 1.f );
				} break;
				}
			}
		}

		{
			if ( this->m_area_type != color_type_t::NONE ) {
				color_t new_col;

				new_col.from_hsv ( this->m_new_hsv [ 0 ], std::clamp ( this->m_new_hsv [ 1 ], 0.f, 1.f ), std::clamp ( this->m_new_hsv [ 2 ], 0.f, 1.f ) );
				new_col.a = this->m_alpha_bar ? static_cast< float >( static_cast< float >( this->m_opacity_mouse.x - this->m_opacity_rect.x ) / this->m_opacity_rect.w * 255.f ) : 255.f;

				*this->m_value = new_col;
			}
		}

		{
			if ( inputsys.key_released ( VK_LBUTTON ) && this->m_drag ) {
				this->m_drag = false;
				this->m_area_type = color_type_t::NONE;
			}

			if ( !inputsys.hovered ( this->m_box_rect ) && !inputsys.hovered ( this->m_item_area ) && inputsys.key_pressed ( VK_LBUTTON ) ) {
				focused = nullptr;
				this->m_focus_type = focus_type_t::FOCUS_NONE;
			}
		}
	}

	else if ( this->m_focus_type == focus_type_t::FOCUS_CONTEXT ) {
		this->m_context_area = { this->m_item_area.x + this->m_item_area.w + 3.f, this->m_item_area.y, 70.f, this->m_action_names.size ( ) * 15.f };

		for ( int i = 0; i < this->m_action_names.size ( ); i++ ) {
			if ( inputsys.hovered ( { this->m_context_area.x, this->m_context_area.y + ( i * 15.f ), m_context_area.w, 15.f } ) && inputsys.key_pressed ( VK_LBUTTON ) ) {
				switch ( i ) {
				case 0: {
					stored_color = *this->m_value;
				} break;
				case 1: {
					*this->m_value = stored_color;
					m_value->to_hsv ( m_new_hsv [ 0 ], m_new_hsv [ 1 ], m_new_hsv [ 2 ] );
				} break;
				}

				focused = nullptr;

				break;
			}
		}

		if ( !inputsys.hovered ( this->m_context_area ) && !inputsys.hovered ( this->m_item_area ) && inputsys.key_pressed ( VK_LBUTTON ) ) {
			focused = nullptr;
			this->m_focus_type = focus_type_t::FOCUS_NONE;
		}
	}
}