#pragma once

class entity_t;

enum weapon_type_t : int {
	weapontype_knife = 0,
	weapontype_pistol,
	weapontype_submachinegun,
	weapontype_rifle,
	weapontype_shotgun,
	weapontype_sniper_rifle,
	weapontype_machinegun,
	weapontype_c4,
	weapontype_grenade,
	weapontype_equipment,
	weapontype_stackableitem,
	weapontype_unknown
};

class weapon_info_t {
public:
	PAD ( 4 ); //0x0000
	const char *m_weapon_name; //0x0004
	PAD ( 12 ); //0x0008
	int m_max_clip; //0x0014
	int m_max_clip_alt; //0x0018
	int m_default_clip; //0x001C
	int m_default_clip_alt; //0x0020
	PAD ( 100 ); //0x0024
	const char *m_unk1; //0x0088
	const char *m_unk2; //0x008C
	PAD ( 56 ); //0x0090
	int m_weapon_type; //0x00C8
	PAD ( 8 ); //0x00CC
	const char *m_weapon_name_alt; //0x00D4
	PAD ( 20 ); //0x00D8
	int m_damage; //0x00EC
	float m_armor_ratio; //0x00F0
	int m_bullets; //0x00F4
	float m_penetration; //0x00F8
	PAD ( 8 ); //0x00FC
	float m_range; //0x0104
	float m_range_modifier; //0x0108
	PAD ( 32 ); //0x010C
	float m_max_speed; //0x012C
	float m_max_speed_alt; //0x0130
	PAD ( 3844 ); //0x0134
}; //Size: 0x01A0

class weapon_t : public entity_t {
public:
	NETVAR ( short, item_definition_index, "DT_BaseAttributableItem", "m_iItemDefinitionIndex" );
	NETVAR ( float, recoil_index, "DT_WeaponCSBase", "m_flRecoilIndex" );
	NETVAR ( float, accuracy_penalty, "DT_WeaponCSBase", "m_fAccuracyPenalty" );
	
	weapon_info_t *data ( );
	c_econ_item_definition *econ_item_view ( weapon_t *weapon );
	c_econ_item_definition *static_data ( c_econ_item_definition *item );
	const wchar_t *name ( );
};