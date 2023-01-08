#pragma once

struct notify_text_t {
	std::string m_message;
	color_t m_color;
	float m_time;
	int m_alpha;
};

struct notify_t {
	void print ( const std::string &msg );
	void add ( const std::string &msg );
	void paint ( );

	std::deque < notify_text_t > m_notify_text;
	color_t m_accent = { 207, 201, 245 };
};

extern notify_t notify;