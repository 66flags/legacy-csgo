#pragma once

struct fire_data_t {
    int m_penetrate_count;
    float m_trace_length;
    float m_remaining_length;
    float m_damage;
    float m_out_damage;
};

class penetration_t {
public:
    fire_data_t run ( const vec_t src, const vec_t end, player_t *ent, bool is_zeus = false, weapon_info_t *info_override = nullptr );
    bool simulate_fire_bullet ( const weapon_info_t *data, vec_t src, vec_t pos, trace_filter_t *filter, fire_data_t &fire_info, bool is_zeus = false, player_t *ent = nullptr );
    bool is_breakable_ent ( player_t *ent );
    bool trace_to_exit ( vec_t start, vec_t dir, vec_t &end, trace_t *trace_enter, trace_t *trace_exit, float step_size, float max_distance );
    bool handle_bullet_penetration ( player_t *ent, float &penetration, int &enter_material, bool &hit_grate, trace_t &tr, vec_t &direction, surfacedata_t *surface_data, float penetration_modifier, float damage_modifier, float penetration_power, int &penetration_count, vec_t &src, float distance, float current_distance, float &current_damage );
    float scale_dmg ( player_t *player, float damage, float armor_ratio, int hitgroup, bool is_zeus = false );
    void clip_trace_to_player ( player_t *player, const vec_t &vec_abs_start, const vec_t &vec_abs_end, unsigned int mask, trace_filter_t *filter, trace_t *tr );
    void clip_trace_to_players ( const vec_t &start, const vec_t &end, unsigned int mask, trace_filter_t *filter, trace_t *tr );
};

extern penetration_t penetration;