#pragma once

class c_global_vars {
public:
	float m_realtime; // 0x0000
	int	m_framecount; // 0x0004
	float m_abs_frametime; // 0x0008
	float m_abs_framestarttime; // 0x000C
	float m_curtime; // 0x0010
	float m_frametime; // 0x0014
	int	m_max_clients; // 0x0018
	int	m_tickcount; // 0x001C
	float m_interval_per_tick; // 0x0020
	float m_interpolation_amount; // 0x0024
	int	m_simticks_this_frame; // 0x0028
	int	m_network_protocol; // 0x002C
};