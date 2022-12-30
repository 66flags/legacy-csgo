#pragma once

struct notify_text_t {
	std::string m_message;
	color_t m_color;
	float m_time;
	int m_alpha;
};

struct notify_t {
	void add ( const std::string &msg, color_t col );
	void paint ( );

	std::deque < notify_text_t > m_notify_text;
};

extern notify_t notify;