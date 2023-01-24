#pragma once

struct view_setup_t {
	int m_x;					 
	int m_old_x;				 
	int m_y;					 
	int	m_old_y;				 
	int m_width;				 
	int m_old_width;			 
	int m_height;				 
	int m_old_height;			 
	bool m_ortho;				 
	float m_ortho_left;			 
	float m_ortho_top;			 
	float m_ortho_right;			 
	float m_ortho_bottom;			 
	bool m_custom_view_matrix;	 
	matrix3x4_t m_custom_matrix;		 
private:
	PAD ( 0x48 );						 
public:
	float m_fov;                   
	float m_viewmodel_fov;		 
	vec_t m_origin;
	vec_t m_angles;
	float m_znear;
	float m_zfar;
	float m_znear_viewmodel;
	float m_zfar_viewmodel;
	float m_aspect_ratio;
	float m_near_blur_depth;
	float m_near_focus_depth;
	float m_far_focus_depth;
	float m_far_blur_depth;
	float m_near_blur_radius;
	float m_far_blur_radius;
	int m_quality;
	int m_motionblur_mode;
	float m_shutter_time;
	vec_t m_shutter_open_pos;
	vec_t m_shutter_open_angles;
	vec_t m_shutter_close_pos;
	vec_t m_shutter_close_angles;
	float m_offcenter_top;
	float m_offcenter_bottom;
	float m_offcenter_left;
	float m_offcenter_right;
	int m_edge_blur;
};

class c_client_mode {
public:
	/* USELESS. */
};