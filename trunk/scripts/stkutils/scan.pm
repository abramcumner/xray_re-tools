#!perl -w -I \temp\1\bin
#
# v.0.1. Last Modified: 25 May 2011
#######################################################################
package stkutils::scan;
use strict;
use IO::File;

use constant section_to_class => {
	actor					=> 'se_actor',

	graph_point				=> 'cse_alife_graph_point',

	stalker					=> 'se_stalker',
	stalker_monolith			=> 'se_stalker',
	stalker_zombied				=> 'se_stalker',
	stalker_fresh_zombied		=> 'se_stalker',
	stalker_trader				=> 'se_stalker',
	stalker_sakharov			=> 'se_stalker',
	stalker_die_hard			=> 'se_stalker',
	stalker_strelok				=> 'se_stalker',
	m_stalker_e				=> 'se_stalker',
	m_stalker_demo				=> 'se_stalker',
	m_stalker_wolf				=> 'se_stalker',
	m_barman				=> 'se_stalker',
	m_osoznanie				=> 'se_stalker',
	m_bandit_commander		=> 'cse_alife_human_stalker',
	m_fraction_commander	=> 'cse_alife_human_stalker',
	m_fraction_soldier		=> 'cse_alife_human_stalker',
	m_fraction_sniper		=> 'cse_alife_human_stalker',
	m_fraction_specnaz		=> 'cse_alife_human_stalker',
	m_army_soldier			=> 'cse_alife_human_stalker',
	m_army_commander		=> 'cse_alife_human_stalker',
	m_army_sniper			=> 'cse_alife_human_stalker',
	m_army_specnaz			=> 'cse_alife_human_stalker',
	m_bandit_bandit			=> 'cse_alife_human_stalker',
	m_sniper				=> 'cse_alife_human_stalker',
	m_idol_e				=> 'cse_alife_object_idol',
	m_dummy					=> 'cse_alife_object_dummy',
	m_target_cs_cask 				=> 'cse_target_cs_cask',
	m_target_cs_base 				=> 'cse_target_cs_base',
	m_trader				=> 'cse_alife_trader',
	m_lesnik				=> 'cse_alife_trader',
	
	respawn					=> 'se_respawn',
	spawn_group				=> 'cse_alife_spawn_group',
	spawn_group_zone		=> 'cse_alife_spawn_group',
	
	detector_simple				=> 'cse_alife_item_detector',
	detector_elite				=> 'cse_alife_item_detector',

	zat_b12_documents_2			=> 'cse_alife_item_pda',
	zat_b39_joker_pda			=> 'cse_alife_item_pda',
	zat_a23_gauss_rifle_docs		=> 'cse_alife_item_pda',
	zat_b40_notebook			=> 'cse_alife_item_pda',
	jup_a9_evacuation_info			=> 'cse_alife_item_pda',
	jup_a9_meeting_info			=> 'cse_alife_item_pda',
	jup_a9_losses_info			=> 'cse_alife_item_pda',
	jup_a9_delivery_info			=> 'cse_alife_item_pda',
	jup_a9_way_info				=> 'cse_alife_item_pda',
	jup_a9_conservation_info		=> 'cse_alife_item_pda',
	jup_a9_power_info			=> 'cse_alife_item_pda',
	jup_b200_tech_materials_wire		=> 'cse_alife_item_pda',
	jup_b200_tech_materials_acetone		=> 'cse_alife_item_pda',
	jup_b200_tech_materials_capacitor	=> 'cse_alife_item_pda',
	jup_b200_tech_materials_textolite	=> 'cse_alife_item_pda',
	jup_b200_tech_materials_transistor	=> 'cse_alife_item_pda',

	helm_respirator_joker			=> 'cse_alife_item_helmet',
	helm_respirator				=> 'cse_alife_item_helmet',
	helm_tactic				=> 'cse_alife_item_helmet',
	helm_hardhat				=> 'cse_alife_item_helmet',

	m_bloodsucker_e				=> 'se_monster',
	bloodsucker_weak			=> 'se_monster',
	bloodsucker_normal			=> 'se_monster',
	bloodsucker_strong			=> 'se_monster',
	bloodsucker_arena			=> 'se_monster',
	bloodsucker_mil				=> 'se_monster',

	m_boar_e				=> 'se_monster',
	boar_weak				=> 'se_monster',
	boar_normal				=> 'se_monster',
	boar_strong				=> 'se_monster',

	m_burer_e				=> 'se_monster',
	burer_weak				=> 'se_monster',
	burer_normal				=> 'se_monster',
	burer_arena				=> 'se_monster',
	m_burer_normal				=> 'se_monster',
	m_burer_normal_black			=> 'se_monster',
	burer_indoor				=> 'se_monster',
	burer_outdoor				=> 'se_monster',

	m_cat_e					=> 'se_monster',
	cat_weak				=> 'se_monster',
	cat_normal				=> 'se_monster',
	cat_strong				=> 'se_monster',

	m_chimera_e				=> 'se_monster',
	m_chimera				=> 'se_monster',
	chimera_weak				=> 'se_monster',
	chimera_normal				=> 'se_monster',
	chimera_strong				=> 'se_monster',

	m_controller_e				=> 'se_monster',
	m_controller_normal			=> 'se_monster',
	m_controller_normal_fat			=> 'se_monster',
	m_controller_old			=> 'se_monster',
	m_controller_old_fat			=> 'se_monster',
	controller_tubeman			=> 'se_monster',

	m_crow					=> 'cse_alife_creature_crow',

	m_dog_e					=> 'se_monster',
	dog_weak				=> 'se_monster',
	dog_normal				=> 'se_monster',
	dog_strong				=> 'se_monster',

	m_flesh_e				=> 'se_monster',
	flesh_weak				=> 'se_monster',
	flesh_normal				=> 'se_monster',
	flesh_strong				=> 'se_monster',

	m_fracture_e				=> 'se_monster',
	fracture_weak				=> 'se_monster',
	fracture_normal				=> 'se_monster',
	fracture_strong				=> 'se_monster',

	m_gigant_e				=> 'se_monster',
	gigant_normal				=> 'se_monster',
	gigant_strong				=> 'se_monster',

	m_phantom				=> 'cse_alife_creature_phantom',

	m_poltergeist_e				=> 'se_monster',
	m_poltergeist_normal			=> 'se_monster',
	m_poltergeist_normal_tele		=> 'se_monster',
	m_poltergeist_tele_outdoor		=> 'se_monster',
	m_poltergeist_normal_flame		=> 'se_monster',
	m_poltergeist_strong_flame		=> 'se_monster',

	m_pseudodog_e				=> 'se_monster',
	pseudodog_weak				=> 'se_monster',
	pseudodog_normal			=> 'se_monster',
	pseudodog_strong			=> 'se_monster',
	pseudodog_arena				=> 'se_monster',
	psy_dog					=> 'se_monster',
	psy_dog_phantom				=> 'se_monster',	# cse_alife_psy_dog_phantom really
	psy_dog_radar				=> 'se_monster',

	m_rat_e					=> 'se_monster',
	rat_group					=> 'cse_alife_rat_group',

	m_snork_e				=> 'se_monster',
	snork_weak				=> 'se_monster',
	snork_normal				=> 'se_monster',
	snork_strong				=> 'se_monster',
	aes_snork				=> 'se_monster',
	snork_arena				=> 'se_monster',
	snork_indoor				=> 'se_monster',
	snork_outdoor				=> 'se_monster',
	snork_jumper				=> 'se_monster',

	m_tushkano_e				=> 'se_monster',
	m_tushkano_normal			=> 'se_monster',
	tushkano_normal				=> 'se_monster',

	m_zombie_e				=> 'se_monster',
	zombie_weak				=> 'se_monster',
	zombie_normal				=> 'se_monster',
	zombie_strong				=> 'se_monster',
	zombie_immortal				=> 'se_monster',

	breakable_object			=> 'cse_alife_object_breakable',

	search_light				=> 'cse_alife_object_projector',
	physic_object				=> 'cse_alife_object_physic',
	script_object				=> 'cse_alife_dynamic_object_visual',
	physic_destroyable_object		=> 'cse_alife_object_physic',
	physic_door				=> 'cse_alife_object_physic',
	jup_b219_gate				=> 'cse_alife_object_physic',
	door_lab_x8				=> 'cse_alife_object_physic',
	gaz_plita				=> 'cse_alife_object_physic',
	gaz_plita_small				=> 'cse_alife_object_physic',
	gaz_balon				=> 'cse_alife_object_physic',
	child_bench				=> 'cse_alife_object_physic',
	stul_child_01				=> 'cse_alife_object_physic',
	stul_school_01				=> 'cse_alife_object_physic',
	stul_school_01_br			=> 'cse_alife_object_physic',

	climable_object				=> 'cse_alife_object_climable',
	lights_hanging_lamp			=> 'cse_alife_object_hanging_lamp',
	lights_signal_light			=> 'cse_alife_object_hanging_lamp',

	shooting_target_1			=> 'cse_alife_object_physic',

	box_bottle_1				=> 'cse_alife_object_physic',
	molot					=> 'cse_alife_object_physic',
	keyga					=> 'cse_alife_object_physic',
	box_1a					=> 'cse_alife_object_physic',
	box_1c					=> 'cse_alife_object_physic',
	box_metall_01				=> 'cse_alife_object_physic',
	box_paper				=> 'cse_alife_object_physic',
	box_wood_01				=> 'cse_alife_object_physic',
	box_wood_02				=> 'cse_alife_object_physic',
	wood_fence_1				=> 'cse_alife_object_physic',
	wood_fence_4				=> 'cse_alife_object_physic',
	balon_02link				=> 'cse_alife_object_physic',
	axe					=> 'cse_alife_object_physic',
	saw					=> 'cse_alife_object_physic',
	hammer					=> 'cse_alife_object_physic',
	pick					=> 'cse_alife_object_physic',
	riffler					=> 'cse_alife_object_physic',
	lopata					=> 'cse_alife_object_physic',
	pri_a28_actor_hideout			=> 'cse_alife_object_physic',

	balon_01				=> 'cse_alife_object_physic',
	balon_02				=> 'cse_alife_object_physic',
	balon_02a				=> 'cse_alife_object_physic',
	bidon					=> 'cse_alife_object_physic',
	bochka_close_1				=> 'cse_alife_object_physic',
	bochka_close_2				=> 'cse_alife_object_physic',
	bochka_close_3				=> 'cse_alife_object_physic',
	bochka_close_4				=> 'cse_alife_object_physic',

	notebook				=> 'cse_alife_object_physic',
	priemnik_gorizont			=> 'cse_alife_object_physic',
	rupor					=> 'cse_alife_object_physic',
	transiver				=> 'cse_alife_object_physic',
	tv_1					=> 'cse_alife_object_physic',
	table_lamp_01				=> 'cse_alife_object_physic',
	komp_monitor				=> 'cse_alife_object_physic',
	komp_klava				=> 'cse_alife_object_physic',
	komp_block				=> 'cse_alife_object_physic',

	banka_kraski_1				=> 'cse_alife_object_physic',
	kanistra_01				=> 'cse_alife_object_physic',
	kanistra_02				=> 'cse_alife_object_physic',
	vedro_01				=> 'cse_alife_object_physic',
	tiski					=> 'cse_alife_object_physic',

	bottle_3l				=> 'cse_alife_object_physic',
	freezer					=> 'cse_alife_object_physic',
	bludo					=> 'cse_alife_object_physic',
	kastrula				=> 'cse_alife_object_physic',
	kastrula_up				=> 'cse_alife_object_physic',
	krujka					=> 'cse_alife_object_physic',
	lojka					=> 'cse_alife_object_physic',
	miska					=> 'cse_alife_object_physic',
	tarelka1				=> 'cse_alife_object_physic',
	tarelka2				=> 'cse_alife_object_physic',
	teapot_1				=> 'cse_alife_object_physic',

	kolyaska_01				=> 'cse_alife_object_physic',
	kolyaska_01_braked			=> 'cse_alife_object_physic',
	kolyaska_wheel_01_braked		=> 'cse_alife_object_physic',
	wheel_litter_01				=> 'cse_alife_object_physic',
	wheel_litter_01_braked			=> 'cse_alife_object_physic',
	med_stolik_01				=> 'cse_alife_object_physic',

	fire_vedro				=> 'cse_alife_object_physic',
	ognetushitel				=> 'cse_alife_object_physic',

	smart_cover				=> 'se_smart_cover',

	inventory_box				=> 'cse_alife_inventory_box',

	space_restrictor			=> 'cse_alife_space_restrictor',
	script_zone				=> 'cse_alife_space_restrictor',
	touch_zone				=> 'cse_alife_space_restrictor',
	anomal_zone				=> 'cse_alife_space_restrictor',

	level_changer				=> 'se_level_changer',

	zone_team_base				=> 'cse_alife_team_base_zone',

	smart_terrain				=> 'se_smart_terrain',

	anomaly_field				=> 'se_anomaly_field',

	zone_ameba				=> 'cse_alife_zone_visual',
	zone_ameba1				=> 'cse_alife_zone_visual',

	zone_mine_steam_weak			=> 'se_zone_anom',
	zone_mine_steam_average			=> 'se_zone_anom',

	zone_mine_static_strong			=> 'se_zone_anom',
	zone_mine_chemical_weak			=> 'se_zone_anom',
	zone_mine_chemical_average		=> 'se_zone_anom',
	zone_mine_chemical_strong		=> 'se_zone_anom',

	fireball_acidic_zone			=> 'se_zone_torrid',

	camp_zone				=> 'cse_alife_space_restrictor',

	zone_burning_fuzz			=> 'se_zone_visual',
	zone_burning_fuzz1			=> 'se_zone_visual',
	zone_burning_fuzz_weak			=> 'se_zone_visual',
	zone_burning_fuzz_average		=> 'se_zone_visual',
	zone_burning_fuzz_strong		=> 'se_zone_visual',
	zone_burning_fuzz_bottom_weak		=> 'se_zone_visual',
	zone_burning_fuzz_bottom_average	=> 'se_zone_visual',
	zone_burning_fuzz_bottom_strong		=> 'se_zone_visual',

	zone_buzz				=> 'se_zone_anom',
	zone_buzz_weak				=> 'se_zone_anom',
	zone_buzz_average			=> 'se_zone_anom',
	zone_buzz_strong			=> 'se_zone_anom',

	zone_gravi_zone				=> 'se_zone_anom',
	zone_gravi_zone_weak			=> 'se_zone_anom',
	zone_gravi_zone_weak_noart			=> 'se_zone_anom',
	zone_gravi_zone_average			=> 'se_zone_anom',
	zone_gravi_zone_strong			=> 'se_zone_anom',
	zone_gravi_zone_killing			=> 'se_zone_anom',

	zone_campfire_mp_nolight		=> 'cse_alife_anomalous_zone',
	zone_campfire_grill			=> 'cse_alife_anomalous_zone',
	zone_flame				=> 'cse_alife_anomalous_zone',
	zone_zhar				=> 'cse_alife_anomalous_zone',
	zone_emi				=> 'cse_alife_anomalous_zone',
	zone_flame_small			=> 'cse_alife_anomalous_zone',
	campfire				=> 'cse_alife_anomalous_zone',

	zone_mincer				=> 'se_zone_anom',
	zone_mincer_weak			=> 'se_zone_anom',
	zone_mincer_weak_noart			=> 'se_zone_anom',
	zone_mincer_average			=> 'se_zone_anom',
	zone_mincer_strong			=> 'se_zone_anom',

	zone_mine_field				=> 'cse_alife_anomalous_zone',
	zone_mine_field_strong			=> 'cse_alife_anomalous_zone',
	zone_mine_field_no_damage		=> 'cse_alife_anomalous_zone',
	zone_death				=> 'cse_alife_anomalous_zone',

	zone_monolith				=> 'cse_alife_anomalous_zone',

	zone_mosquito_bald			=> 'se_zone_anom',
	zone_mosquito_bald_weak			=> 'se_zone_anom',
	zone_mosquito_bald_weak_noart		=> 'se_zone_anom',
	zone_mosquito_bald_average		=> 'se_zone_anom',
	zone_mosquito_bald_strong		=> 'se_zone_anom',
	zone_mosquito_bald_strong_noart		=> 'se_zone_anom',

	zone_no_gravity				=> 'cse_alife_anomalous_zone',

	zone_acid_fog			=> 'cse_alife_anomalous_zone',
	zone_lava				=> 'cse_alife_anomalous_zone',
	zone_rusty_hair			=> 'cse_alife_anomalous_zone',
	zone_x					=> 'cse_alife_anomalous_zone',	
	zone_x1					=> 'cse_alife_anomalous_zone',	
	zone_x2					=> 'cse_alife_anomalous_zone',	
	zone_x3					=> 'cse_alife_anomalous_zone',		
	
	zone_radioactive			=> 'cse_alife_anomalous_zone',
	zone_radioactive_weak			=> 'cse_alife_anomalous_zone',
	zone_radioactive_average		=> 'cse_alife_anomalous_zone',
	zone_radioactive_strong			=> 'cse_alife_anomalous_zone',
	zone_radioactive_killing		=> 'cse_alife_anomalous_zone',

	zone_teleport				=> 'cse_alife_anomalous_zone',
	zone_teleport_out			=> 'cse_alife_anomalous_zone',
	zone_teleport_monolith			=> 'cse_alife_anomalous_zone',
	zone_isparenie			=> 'cse_alife_anomalous_zone',

	torrid_zone				=> 'cse_alife_torrid_zone',
	fireball_zone				=> 'se_zone_torrid',
	fireball_electric_zone			=> 'se_zone_torrid',
	zone_sarcofag				=> 'cse_alife_anomalous_zone',

	zone_witches_galantine			=> 'se_zone_anom',
	zone_witches_galantine_weak		=> 'se_zone_anom',
	zone_witches_galantine_average		=> 'se_zone_anom',
	zone_witches_galantine_strong		=> 'se_zone_anom',
	zone_witches_galantine_safe		=> 'se_zone_anom',

	zone_zharka_static			=> 'se_zone_anom',
	zone_zharka_static_weak			=> 'se_zone_anom',
	zone_zharka_static_average		=> 'se_zone_anom',
	zone_zharka_static_strong		=> 'se_zone_anom',

	zone_mine_acidic			=> 'se_zone_anom',
	zone_mine_acidic_weak			=> 'se_zone_anom',
	zone_mine_acidic_average		=> 'se_zone_anom',
	zone_mine_acidic_strong			=> 'se_zone_anom',

	zone_mine_electric			=> 'se_zone_anom',
	zone_mine_electric_weak			=> 'se_zone_anom',
	zone_mine_electric_average		=> 'se_zone_anom',
	zone_mine_electric_strong		=> 'se_zone_anom',

	zone_mine_thermal			=> 'se_zone_anom',
	zone_mine_thermal_weak			=> 'se_zone_anom',
	zone_mine_thermal_average		=> 'se_zone_anom',
	zone_mine_thermal_strong		=> 'se_zone_anom',
	zone_mine_thermal_firetube		=> 'se_zone_anom',

	zone_mine_gravitational_weak		=> 'se_zone_anom',
	zone_mine_gravitational_average		=> 'se_zone_anom',
	zone_mine_gravitational_strong		=> 'se_zone_anom',

	zone_field_acidic			=> 'se_zone_anom',
	zone_field_acidic_weak			=> 'se_zone_anom',
	zone_field_acidic_average		=> 'se_zone_anom',
	zone_field_acidic_strong		=> 'se_zone_anom',

	zone_field_psychic			=> 'se_zone_anom',
	zone_field_psychic_weak			=> 'se_zone_anom',
	zone_field_psychic_average		=> 'se_zone_anom',
	zone_field_psychic_strong		=> 'se_zone_anom',

	zone_field_radioactive			=> 'se_zone_anom',
	zone_field_radioactive_weak		=> 'se_zone_anom',
	zone_field_radioactive_average		=> 'se_zone_anom',
	zone_field_radioactive_strong		=> 'se_zone_anom',

	zone_field_thermal			=> 'se_zone_anom',
	zone_field_thermal_weak			=> 'se_zone_anom',
	zone_field_thermal_average		=> 'se_zone_anom',
	zone_field_thermal_strong		=> 'se_zone_anom',

	af_quest_b14_twisted			=> 'cse_alife_item_artefact',
	af_oasis_heart				=> 'cse_alife_item_artefact',

	af_ameba_mica				=> 'cse_alife_item_artefact',
	af_ameba_slime				=> 'cse_alife_item_artefact',
	af_ameba_slug				=> 'cse_alife_item_artefact',
	af_blood				=> 'cse_alife_item_artefact',
	af_cristall_flower			=> 'cse_alife_item_artefact',
	af_dummy_battery			=> 'cse_alife_item_artefact',
	af_dummy_dummy				=> 'cse_alife_item_artefact',
	af_dummy_glassbeads			=> 'cse_alife_item_artefact',
	af_dummy_pellicle			=> 'cse_alife_item_artefact',
	af_dummy_spring				=> 'cse_alife_item_artefact',
	af_electra_flash			=> 'cse_alife_item_artefact',
	af_electra_moonlight			=> 'cse_alife_item_artefact',
	af_electra_sparkler			=> 'cse_alife_item_artefact',
	af_fireball				=> 'cse_alife_item_artefact',
	af_fuzz_kolobok				=> 'cse_alife_item_artefact',
	af_gold_fish				=> 'cse_alife_item_artefact',
	af_gravi				=> 'cse_alife_item_artefact',
	af_medusa				=> 'cse_alife_item_artefact',
	af_mincer_meat				=> 'cse_alife_item_artefact',
	af_night_star				=> 'cse_alife_item_artefact',
	'af_rusty_sea-urchin'			=> 'cse_alife_item_artefact',
	af_rusty_kristall			=> 'cse_alife_item_artefact',
	af_rusty_thorn			=> 'cse_alife_item_artefact',
	af_vyvert				=> 'cse_alife_item_artefact',
	af_compass				=> 'cse_alife_item_artefact',
	bar_ameba_spore				=> 'cse_alife_item_artefact',
	af_soul				=> 'cse_alife_item_artefact',
	af_drops				=> 'cse_alife_item_artefact',
	af_mercury_ball			=> 'cse_alife_item_artefact',
	af_cristall				=> 'cse_alife_item_artefact',
	af_ameba_slime				=> 'cse_alife_item_artefact',
	af_magnet				=> 'cse_alife_item_artefact',

	flesh_group				=> 'cse_alife_flesh_group',

	drug_radioprotector			=> 'cse_alife_item',
	drug_booster				=> 'cse_alife_item',
	drug_coagulant				=> 'cse_alife_item',
	drug_antidot				=> 'cse_alife_item',
	drug_anabiotic				=> 'cse_alife_item',
	drug_psy_blockade			=> 'cse_alife_item',

	toolkit_1				=> 'cse_alife_item_pda',
	toolkit_2				=> 'cse_alife_item_pda',
	toolkit_3				=> 'cse_alife_item_pda',
	jup_b205_sokolov_note			=> 'cse_alife_item_pda',
	pri_a19_american_experiment_info	=> 'cse_alife_item_pda',
	pri_a19_lab_x7_info			=> 'cse_alife_item_pda',
	pri_a19_lab_x10_info			=> 'cse_alife_item_pda',
	pri_a19_lab_x16_info			=> 'cse_alife_item_pda',
	pri_a19_lab_x18_info			=> 'cse_alife_item_pda',
	lx8_service_instruction			=> 'cse_alife_item_pda',

	museum_svd				=> 'cse_alife_object_physic',
	museum_ak74u				=> 'cse_alife_object_physic',
	museum_groza				=> 'cse_alife_object_physic',

	taburet_wood_01				=> 'cse_alife_object_physic',
	taburet_village				=> 'cse_alife_object_physic',
	stul_wood_01				=> 'cse_alife_object_physic',
	ventilator_01				=> 'cse_alife_object_physic',
	ventilator_02				=> 'cse_alife_object_physic',

	bread					=> 'cse_alife_item',
	bread_a					=> 'cse_alife_item',
	kolbasa					=> 'cse_alife_item',
	vodka					=> 'cse_alife_item',
	vodka_a					=> 'cse_alife_item',
	medkit					=> 'cse_alife_item',
	conserva				=> 'cse_alife_item',
	energy_drink				=> 'cse_alife_item',
	antirad					=> 'cse_alife_item',
	bandage					=> 'cse_alife_item',
	medkit					=> 'cse_alife_item',
	medkit_army				=> 'cse_alife_item',
	medkit_scientic				=> 'cse_alife_item',
	guitar_a				=> 'cse_alife_item',
	harmonica_a				=> 'cse_alife_item',
	hand_radio				=> 'cse_alife_item',
	dynamite				=> 'cse_alife_item',
	device_torch				=> 'cse_alife_item_torch',
	good_psy_helmet				=> 'cse_alife_item',
	bad_psy_helmet				=> 'cse_alife_item',

	af_blood_tutorial			=> 'cse_alife_item_artefact',
	quest_case_01				=> 'cse_alife_item',
	quest_case_02				=> 'cse_alife_item',
	pri_decoder_documents			=> 'cse_alife_item',
	cit_killers_documents			=> 'cse_alife_item',
	document			=> 'cse_alife_item_document',
	dar_pass_document				=> 'cse_alife_item',
	bar_darklab_documents				=> 'cse_alife_item',
	bar_decoding_documents				=> 'cse_alife_item',
	agr_nii_security_shedule_file				=> 'cse_alife_item',
	agr_nii_security_plan_flash				=> 'cse_alife_item',
	dolg_arhive_documents				=> 'cse_alife_item',
	dar_document1				=> 'cse_alife_item',
	dar_document2				=> 'cse_alife_item',
	dar_document3				=> 'cse_alife_item',
	dar_document4				=> 'cse_alife_item',
	dar_document5				=> 'cse_alife_item',
	lab_x16_documents				=> 'cse_alife_item',
	gunslinger_flash			=> 'cse_alife_item',
	decoder					=> 'cse_alife_item',
	device_pda					=> 'cse_alife_item_pda',
	mil_svoboda_leader_pda					=> 'cse_alife_item',
	detector_simple					=> 'cse_alife_item_detector',
	detector_advances					=> 'cse_alife_item_detector',
	detector_elite					=> 'cse_alife_item_detector',
	binocular_a				=> 'cse_alife_item',
	device_atifact_merger				=> 'cse_alife_item',
	cit_deadcity_maps				=> 'cse_alife_item',
	borov_flash				=> 'cse_alife_item',
	crazy_flash				=> 'cse_alife_item',
	max_flash				=> 'cse_alife_item',
	ugrum_flash				=> 'cse_alife_item',
	agroprom_notebook				=> 'cse_alife_item',
	garbage_letter				=> 'cse_alife_item',
	bolt					=> 'cse_alife_item_bolt',

	novice_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_novice_m1			=> 'cse_alife_item_custom_outfit',
	bandit_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_bandit_m1			=> 'cse_alife_item_custom_outfit',
	outfit_bandit_m2			=> 'cse_alife_item_custom_outfit',
	outfit_bandit_anom			=> 'cse_alife_item_custom_outfit',
	dolg_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_dolg_m1				=> 'cse_alife_item_custom_outfit',
	dolg_scientific_outfit			=> 'cse_alife_item_custom_outfit',
	scientific_outfit			=> 'cse_alife_item_custom_outfit',
	exo_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_exo_m1				=> 'cse_alife_item_custom_outfit',
	killer_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_killer_m1			=> 'cse_alife_item_custom_outfit',
	outfit_killer_m2			=> 'cse_alife_item_custom_outfit',
	monolit_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_monolith_m1				=> 'cse_alife_item_custom_outfit',
	outfit_military_m1				=> 'cse_alife_item_custom_outfit',
	outfit_military_m2				=> 'cse_alife_item_custom_outfit',
	specops_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_specnaz_m1			=> 'cse_alife_item_custom_outfit',
	outfit_specops_m1			=> 'cse_alife_item_custom_outfit',
	military_outfit				=> 'cse_alife_item_custom_outfit',
	stalker_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_stalker_m1			=> 'cse_alife_item_custom_outfit',
	outfit_stalker_m2			=> 'cse_alife_item_custom_outfit',
	svoboda_heavy_outfit			=> 'cse_alife_item_custom_outfit',
	svoboda_light_outfit			=> 'cse_alife_item_custom_outfit',
	outfit_svoboda_m1			=> 'cse_alife_item_custom_outfit',
	protection_outfit			=> 'cse_alife_item_custom_outfit',
	ecolog_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_stalker_anom			=> 'cse_alife_item_custom_outfit',
	outfit_stalker_anom2			=> 'cse_alife_item_custom_outfit',
	outfit_bandit_anom			=> 'cse_alife_item_custom_outfit',
	scientific_outfit			=> 'cse_alife_item_custom_outfit',
	soldier_outfit			=> 'cse_alife_item_custom_outfit',
	military_stalker_outfit				=> 'cse_alife_item_custom_outfit',

	wpn_addon_scope				=> 'cse_alife_item',
	wpn_addon_scope_susat			=> 'cse_alife_item',
	wpn_addon_silencer			=> 'cse_alife_item',
	wpn_addon_grenade_launcher		=> 'cse_alife_item',
	wpn_addon_grenade_launcher_m203		=> 'cse_alife_item',
	scope_m1		=> 'cse_alife_item',
	susat_anom		=> 'cse_alife_item',
	torch_m1		=> 'cse_alife_item_torch',

	grenade_f1				=> 'cse_alife_item_grenade',
	grenade_rgd5				=> 'cse_alife_item_grenade',
	'grenade_gd-05'				=> 'cse_alife_item_grenade',
	ammo_pkm_100				=> 'cse_alife_item_ammo',

	'ammo_11.43x23_fmj'			=> 'cse_alife_item_ammo',
	'ammo_11.43x23_hydro'			=> 'cse_alife_item_ammo',
	ammo_12x70_buck				=> 'cse_alife_item_ammo',
	ammo_12x76_dart				=> 'cse_alife_item_ammo',
	ammo_12x76_zhekan			=> 'cse_alife_item_ammo',
	ammo_12x76_zhekan_heli			=> 'cse_alife_item_ammo',
	ammo_16x70_buck				=> 'cse_alife_item_ammo',
	ammo_16x70_kart				=> 'cse_alife_item_ammo',
	ammo_16x70_zhekan			=> 'cse_alife_item_ammo',
	'ammo_5.45x39_ap'			=> 'cse_alife_item_ammo',
	'ammo_5.45x39_fmj'			=> 'cse_alife_item_ammo',
	'ammo_5.56x45_ap'			=> 'cse_alife_item_ammo',
	'ammo_5.56x45_ss190'			=> 'cse_alife_item_ammo',
	'ammo_5.7x28_fmj'			=> 'cse_alife_item_ammo',
	'ammo_5.7x28_ap'			=> 'cse_alife_item_ammo',
	'ammo_7.62x54_7h1'			=> 'cse_alife_item_ammo',
	'ammo_7.62x54_7h14'			=> 'cse_alife_item_ammo',
	'ammo_7.62x54_ap'			=> 'cse_alife_item_ammo',
	'ammo_7.62x39_fmj'			=> 'cse_alife_item_ammo',
	'ammo_7.62x39_ap'			=> 'cse_alife_item_ammo',
	ammo_9x18_fmj				=> 'cse_alife_item_ammo',
	ammo_9x18_pmm				=> 'cse_alife_item_ammo',
	ammo_9x18_pbp				=> 'cse_alife_item_ammo',
	ammo_9x19_fmj				=> 'cse_alife_item_ammo',
	ammo_9x19_pbp				=> 'cse_alife_item_ammo',
	ammo_9x39_pab9				=> 'cse_alife_item_ammo',
	ammo_9x39_sp5				=> 'cse_alife_item_ammo',
	ammo_9x39_ap				=> 'cse_alife_item_ammo',
	'ammo_vog-25'				=> 'cse_alife_item_ammo',
	'ammo_vog-25p'				=> 'cse_alife_item_ammo',
	ammo_m209				=> 'cse_alife_item_ammo',
	'ammo_og-7b'				=> 'cse_alife_item_ammo',
	ammo_gauss				=> 'cse_alife_item_ammo',

	explosive_barrel			=> 'cse_alife_item_explosive',
	explosive_barrel_low			=> 'cse_alife_item_explosive',
	explosive_dinamit			=> 'cse_alife_item_explosive',
	explosive_fuelcan			=> 'cse_alife_item_explosive',
	explosive_mobiltank			=> 'cse_alife_item_explosive',
	explosive_tank				=> 'cse_alife_item_explosive',
	explosive_hide				=> 'cse_alife_item_explosive',
	explosive_grenade			=> 'cse_alife_item_explosive',
	explosive_gaz_balon			=> 'cse_alife_item_explosive',

	mounted_weapon				=> 'cse_alife_mounted_weapon',
	stationary_mgun				=> 'cse_alife_stationary_mgun',

	wpn_knife				=> 'cse_alife_item_weapon',
	wpn_binoc				=> 'cse_alife_item_weapon_magazined',
	wpn_rpg7				=> 'cse_alife_item_weapon_magazined',
	wpn_pm					=> 'cse_alife_item_weapon_magazined',
	wpn_pb					=> 'cse_alife_item_weapon_magazined',
	wpn_fort				=> 'cse_alife_item_weapon_magazined',
	wpn_fort_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_walther				=> 'cse_alife_item_weapon_magazined',
	wpn_walther_m1			=> 'cse_alife_item_weapon_magazined',
	wpn_beretta				=> 'cse_alife_item_weapon_magazined',
	wpn_hpsa				=> 'cse_alife_item_weapon_magazined',
	wpn_sig220				=> 'cse_alife_item_weapon_magazined',
	wpn_usp					=> 'cse_alife_item_weapon_magazined',
	wpn_desert_eagle		=> 'cse_alife_item_weapon_magazined',
	wpn_eagle_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_colt1911				=> 'cse_alife_item_weapon_magazined',
	wpn_colt_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_mp5					=> 'cse_alife_item_weapon_magazined',
	wpn_mp5_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_mp5_m2				=> 'cse_alife_item_weapon_magazined',
	wpn_val					=> 'cse_alife_item_weapon_magazined',
	wpn_val_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_vintorez			=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_vintorec			=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_svd					=> 'cse_alife_item_weapon_magazined',
	wpn_svd_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_svu					=> 'cse_alife_item_weapon_magazined',
	wpn_ak74u				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_ak74u_m1			=> 'cse_alife_item_weapon_magazined',
	wpn_pkm					=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_gauss				=> 'cse_alife_item_weapon_magazined',
	wpn_groza				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_groza1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_groza_m1			=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_l85					=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_l85_m1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_l85_m2				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_g36					=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_lr300				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_lr300_m1			=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_sig550				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_sig_m1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_sig_m2				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_ak74				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_ak74_m1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_abakan				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_abakan_m1			=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_abakan_m2			=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_fn2000				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_toz34				=> 'cse_alife_item_weapon_shotgun',
	wpn_spas12				=> 'cse_alife_item_weapon_shotgun',
	wpn_spas12_m1				=> 'cse_alife_item_weapon_shotgun',
	wpn_wincheaster1300			=> 'cse_alife_item_weapon_shotgun',
	wpn_winchester_m1			=> 'cse_alife_item_weapon_shotgun',
	wpn_bm16				=> 'cse_alife_item_weapon_shotgun',
	wpn_bm16_full				=> 'cse_alife_item_weapon_shotgun',
	'wpn_rg-6'				=> 'cse_alife_item_weapon_shotgun',
	wpn_rg6_m1				=> 'cse_alife_item_weapon_shotgun',
	wpn_protecta				=> 'cse_alife_item_weapon_shotgun',
	hunters_toz				=> 'cse_alife_item_weapon_shotgun',
	wpn_knife_m1				=> 'cse_alife_item_weapon',

	m_car					=> 'cse_alife_car',
	helicopter				=> 'cse_alife_helicopter',

	turret_mgun				=> 'se_turret_mgun',
		
	mar_csky_commander			=> 'se_stalker',
	mar_csky_trader				=> 'se_stalker',
	mar_csky_tech				=> 'se_stalker',
	mar_csky_scientist			=> 'se_stalker',
	mar_csky_barman				=> 'se_stalker',
	mar_csky_tutorial_man			=> 'se_stalker',
	mar_csky_tactic				=> 'se_stalker',
	
	esc_stalker_guard_east_bridge				=> 'se_stalker',
	esc_stalker_guard_west_bridge				=> 'se_stalker',
	esc_leader_stalkerbase				=> 'se_stalker',
	esc_trader_stalkerbase				=> 'se_stalker',
	esc_tech_stalkerbase				=> 'se_stalker',
	esc_wolf				=> 'se_stalker',
	esc_zak_stalkerbase				=> 'se_stalker',
	esc_driver				=> 'se_stalker',
	esc_military_secret_trader				=> 'se_stalker',
	esc_story_military				=> 'se_stalker',
	esc_zak_stalkerbase_2				=> 'se_stalker',
	esc_military_general				=> 'se_stalker',
	esc_wolf_brother				=> 'se_stalker',
	
	agr_duty_base_trader				=> 'se_stalker',
	agr_stalker_base_trader				=> 'se_stalker',
	agr_secret_trader				=> 'se_stalker',
	agr_holeman				=> 'se_stalker',
	agr_snork_hole_1				=> 'se_monster',
	agr_snork_hole_2				=> 'se_monster',
	agr_snork_hole_3				=> 'se_monster',
	agr_snork_hole_4				=> 'se_monster',
	agr_snork_hole_5				=> 'se_monster',
	agr_snork_hole_6				=> 'se_monster',
	agr_snork_hole_7				=> 'se_monster',
	agr_snork_hole_8				=> 'se_monster',
	agr_snork_hole_9				=> 'se_monster',
	agr_snork_jumper_1				=> 'se_monster',
	agr_snork_jumper_2				=> 'se_monster',
	agr_snork_jumper_3				=> 'se_monster',
	agr_stalker_base_mechanic				=> 'se_stalker',
	agr_stalker_base_leader				=> 'se_stalker',
	agr_stalker_zombied_1_default				=> 'se_stalker',
	agr_bloodsucker_home				=> 'se_monster',
	agr_scientists_bloodsucker				=> 'se_monster',
	agr_weaponmaster				=> 'se_stalker',
	agr_barman				=> 'se_stalker',
	agr_bar_stalker_1				=> 'se_stalker',
	agr_bar_stalker_2				=> 'se_stalker',
	agr_bar_stalker_3				=> 'se_stalker',
	agr_bar_stalker_4				=> 'se_stalker',
	agr_bar_stalker_5				=> 'se_stalker',
	agr_stalker_commander_1				=> 'se_stalker',
	agr_stalker_commander_2				=> 'se_stalker',
	agr_dog_01				=> 'se_monster',
	agr_dog_02				=> 'se_monster',
	agr_dog_03				=> 'se_monster',
	agr_dog_04				=> 'se_monster',
	agr_dog_05				=> 'se_monster',
	agr_dog_06				=> 'se_monster',
	agr_dog_07				=> 'se_monster',
	agr_dog_08				=> 'se_monster',
	agr_dog_09				=> 'se_monster',
	agr_dog_10				=> 'se_monster',
	agr_dolg_blockpost_commander				=> 'se_stalker',

	gar_digger_conc_camp_searcher_1		=> 'se_stalker',
	gar_digger_conc_camp_searcher_2		=> 'se_stalker',
	gar_digger_conc_camp_searcher_3		=> 'se_stalker',
	gar_digger_conc_camp_prisoner_1		=> 'se_stalker',
	gar_digger_conc_camp_prisoner_2		=> 'se_stalker',
	gar_digger_messenger_man				=> 'se_stalker',
	gar_bandit_barman				=> 'se_stalker',
	gar_bandit_fixer				=> 'se_stalker',
	gar_bandit_leader				=> 'se_stalker',
	gar_bandit_trader				=> 'se_stalker',
	gar_bandit_minigame				=> 'se_stalker',
	gar_bandit_senya				=> 'se_stalker',
	gar_bandit_ambush_1				=> 'se_stalker',
	gar_bandit_ambush_2				=> 'se_stalker',
	gar_bandit_robber_1				=> 'se_stalker',
	gar_bandit_robber_2				=> 'se_stalker',
	gar_digger_fighter_1				=> 'se_stalker',
	gar_digger_fighter_2				=> 'se_stalker',
	gar_digger_fighter_3				=> 'se_stalker',
	gar_digger_fighter_4				=> 'se_stalker',
	gar_digger_fighter_5				=> 'se_stalker',
	gar_digger_quester				=> 'se_stalker',
	gar_digger_traitor				=> 'se_stalker',
	gar_digger_smuggler				=> 'se_stalker',
	gar_bandit_digger_traitor				=> 'se_stalker',
	gar_dead_camp_snork				=> 'se_monster',
	
	default_duty				=> 'se_stalker',
	default_freedom				=> 'se_stalker',
	default_bandit				=> 'se_stalker',
	default_stalker				=> 'se_stalker',
	stalker_regular				=> 'se_stalker',
	sim_faction					=> 'se_sim_faction',
	sim_default_duty_0				=> 'se_stalker',
	sim_default_duty_1				=> 'se_stalker',
	sim_default_duty_2				=> 'se_stalker',
	sim_default_duty_3				=> 'se_stalker',
	sim_default_duty_4				=> 'se_stalker',
	sim_default_monolith_0				=> 'se_stalker',
	sim_default_monolith_1				=> 'se_stalker',
	sim_default_monolith_2				=> 'se_stalker',
	sim_default_monolith_3				=> 'se_stalker',
	sim_default_monolith_4				=> 'se_stalker',
	sim_default_freedom_0				=> 'se_stalker',
	sim_default_freedom_1				=> 'se_stalker',
	sim_default_freedom_2				=> 'se_stalker',
	sim_default_freedom_3				=> 'se_stalker',
	sim_default_freedom_4				=> 'se_stalker',
	sim_default_bandit_0				=> 'se_stalker',
	sim_default_bandit_1				=> 'se_stalker',
	sim_default_bandit_2				=> 'se_stalker',
	sim_default_bandit_3				=> 'se_stalker',
	sim_default_bandit_4				=> 'se_stalker',
	sim_default_renegade_0				=> 'se_stalker',
	sim_default_renegade_1				=> 'se_stalker',
	sim_default_renegade_2				=> 'se_stalker',
	sim_default_csky_0				=> 'se_stalker',
	sim_default_csky_1				=> 'se_stalker',
	sim_default_csky_2				=> 'se_stalker',
	sim_default_csky_3				=> 'se_stalker',
	sim_default_csky_4				=> 'se_stalker',
	sim_default_stalker_0				=> 'se_stalker',
	sim_default_stalker_1				=> 'se_stalker',
	sim_default_stalker_2				=> 'se_stalker',
	sim_default_stalker_3				=> 'se_stalker',
	sim_default_stalker_4				=> 'se_stalker',
	sim_default_military_0				=> 'se_stalker',
	sim_default_military_1				=> 'se_stalker',
	sim_default_military_2				=> 'se_stalker',
	sim_default_military_3				=> 'se_stalker',
	sim_default_military_3_sniper				=> 'se_stalker',
	sim_default_military_4				=> 'se_stalker',
	sim_default_killer_0				=> 'se_stalker',
	sim_default_killer_1				=> 'se_stalker',
	sim_default_killer_2				=> 'se_stalker',
	sim_default_killer_3				=> 'se_stalker',
	sim_default_killer_4				=> 'se_stalker',
	sim_default_digger_0				=> 'se_stalker',
	sim_default_digger_1				=> 'se_stalker',
	sim_default_digger_2				=> 'se_stalker',
	sim_default_digger_3				=> 'se_stalker',
	sim_default_digger_4				=> 'se_stalker',
	arena_first_battle_stalker_1				=> 'se_stalker',
	arena_first_battle_ally_stalker_1				=> 'se_stalker',
	arena_second_battle_stalker_1				=> 'se_stalker',
	arena_second_battle_stalker_1_boss				=> 'se_stalker',
	arena_third_battle_stalker_1				=> 'se_stalker',
	arena_third_battle_stalker_1_boss				=> 'se_stalker',
	arena_fourth_battle_stalker_1				=> 'se_stalker',
	arena_fourth_battle_stalker_2				=> 'se_stalker',
	arena_fourth_battle_stalker_3				=> 'se_stalker',
	arena_fourth_battle_stalker_4				=> 'se_stalker',
	arena_fourth_battle_stalker_5				=> 'se_stalker',
	arena_survival_spawn				=> 'se_stalker',
	arena_survival_spawn1				=> 'se_stalker',
	arena_survival_spawn2				=> 'se_stalker',
	pl_test_sect				=> 'se_stalker',
	pl_test_sect1				=> 'se_stalker',
	
	val_freedom_blockpost_guard_north_1				=> 'se_stalker',
	val_freedom_blockpost_guard_north_2				=> 'se_stalker',
	val_freedom_blockpost_guard_leader_north				=> 'se_stalker',
	val_freedom_blockpost_guard_south_1				=> 'se_stalker',
	val_freedom_blockpost_guard_south_2				=> 'se_stalker',
	val_freedom_blockpost_guard_leader_south				=> 'se_stalker',
	val_freedom_deadblockpost_guard_1				=> 'se_stalker',
	val_freedom_deadblockpost_guard_2				=> 'se_stalker',
	val_freedom_deadblockpost_guard_3				=> 'se_stalker',
	val_freedom_deadblockpost_guard_4				=> 'se_stalker',
	val_freedom_deadblockpost_guard_5				=> 'se_stalker',
	sim_default_freedom_sniper				=> 'se_stalker',
	val_killer_sniper_1				=> 'se_stalker',
	val_killer_sniper_2				=> 'se_stalker',
	val_killer_sniper_3				=> 'se_stalker',
	val_killer_sniper_4				=> 'se_stalker',
	val_killer_sniper_5				=> 'se_stalker',
	val_killer_sniper_6				=> 'se_stalker',
	val_killer_sniper_7				=> 'se_stalker',
	val_killer_sniper_8				=> 'se_stalker',
	val_killer_sniper_9				=> 'se_stalker',
	val_killer_sniper_10				=> 'se_stalker',
	val_killer_1				=> 'se_stalker',
	val_killer_2				=> 'se_stalker',
	val_killer_3				=> 'se_stalker',
	val_killer_4				=> 'se_stalker',
	val_killer_5				=> 'se_stalker',
	val_killer_6				=> 'se_stalker',
	val_killer_7				=> 'se_stalker',
	val_killer_8				=> 'se_stalker',
	val_killer_9				=> 'se_stalker',
	val_killer_10				=> 'se_stalker',
	val_freedom_attack_1				=> 'se_stalker',
	val_freedom_attack_2				=> 'se_stalker',
	val_freedom_attack_3				=> 'se_stalker',
	val_freedom_attack_4				=> 'se_stalker',
	val_freedom_attack_commander				=> 'se_stalker',
	val_freedom_attack_6				=> 'se_stalker',
	val_freedom_attack_7				=> 'se_stalker',
	val_freedom_attack_8				=> 'se_stalker',
	val_freedom_attack_9				=> 'se_stalker',
	val_freedom_attack_10				=> 'se_stalker',
	val_freedom_trader				=> 'se_stalker',
	val_freedom_barmen				=> 'se_stalker',
	val_bandit_spy_1				=> 'se_stalker',
	val_bandit_spy_2				=> 'se_stalker',
	val_bandit_spy_3				=> 'se_stalker',
	val_bandit_spy_4				=> 'se_stalker',
	
	device_pda_fang	=> 'cse_alife_item_pda',
	esc_trader_habar	=> 'cse_alife_item_pda',
	device_pda_military	=> 'cse_alife_item_pda',
	device_pda_old	=> 'cse_alife_item_pda',
	device_pda_garbage_traitor	=> 'cse_alife_item_pda',
	device_pda_comendant	=> 'cse_alife_item_pda',
	device_pda_freedom	=> 'cse_alife_item_pda',
	device_pda_bloodsucker	=> 'cse_alife_item_pda',
	device_pda_digger	=> 'cse_alife_item_pda',
	red_forest_pda_map	=> 'cse_alife_item_pda',
	mar_quest_scout_pda	=> 'cse_alife_item_pda',
	mil_device_pda_lost_squard	=> 'cse_alife_item_pda',
	esc_quest_magic_vodka	=> 'cse_alife_item_pda',
	esc_quest_spec_medkit	=> 'cse_alife_item_pda',
	esc_device_pda_driver	=> 'cse_alife_item_pda',
	esc_mechanic_flash_card_1	=> 'cse_alife_item_pda',
	esc_mechanic_flash_card_2	=> 'cse_alife_item_pda',
	esc_mechanic_flash_card_3	=> 'cse_alife_item_pda',
	esc_mechanic_flash_card_4	=> 'cse_alife_item_pda',
	gar_quest_vodka_2	=> 'cse_alife_item_pda',
	agr_quest_duty_secret_pda	=> 'cse_alife_item_pda',
	agr_quest_duty_case	=> 'cse_alife_item_pda',
	agr_pda_for_secret_trader	=> 'cse_alife_item_pda',
	agr_map_animals	=> 'cse_alife_item_pda',
	agr_mechanic_pda	=> 'cse_alife_item_pda',
	yan_quest_granade	=> 'cse_alife_item_pda',
	red_forest_pda_map_2	=> 'cse_alife_item_pda',
	red_quest_prototipe_device	=> 'cse_alife_item_pda',
	
	museum_ammo_12x70_buck				=> 'cse_alife_object_physic',
	museum_ammo_545x39_fmj				=> 'cse_alife_object_physic',	
	museum_ammo_762x54_7h14				=> 'cse_alife_object_physic',	
	museum_abakan				=> 'cse_alife_object_physic',
	museum_ak74				=> 'cse_alife_object_physic',
	museum_ak74u				=> 'cse_alife_object_physic',
	museum_groza				=> 'cse_alife_object_physic',
	museum_lr300				=> 'cse_alife_object_physic',
	museum_rg6				=> 'cse_alife_object_physic',
	museum_rpg7				=> 'cse_alife_object_physic',
	museum_sig550				=> 'cse_alife_object_physic',
	museum_spas12				=> 'cse_alife_object_physic',
	museum_svd				=> 'cse_alife_object_physic',
	museum_bm16				=> 'cse_alife_object_physic',
	museum_toz34				=> 'cse_alife_object_physic',
	museum_val				=> 'cse_alife_object_physic',
	museum_vintorez				=> 'cse_alife_object_physic',
	museum_winchester1300				=> 'cse_alife_object_physic',
	
	agru_door				=> 'cse_alife_object_physic',
	red_forest_bridge				=> 'cse_alife_object_physic',
	
	flesh_up_ac_mp5	=> 'cse_alife_item_pda',
	flesh_up_bd_mp5	=> 'cse_alife_item_pda',
	flesh_up_a_novice_outfit	=> 'cse_alife_item_pda',
	flesh_up_ac_wincheaster1300	=> 'cse_alife_item_pda',
	flesh_up_bd_wincheaster1300	=> 'cse_alife_item_pda',
	flesh_up_ac_ak74u	=> 'cse_alife_item_pda',
	flesh_up_fh_scientific_outfit	=> 'cse_alife_item_pda',
	flesh_up_aceg_scientific_outfit	=> 'cse_alife_item_pda',
	flesh_up_bdfh_scientific_outfit	=> 'cse_alife_item_pda',
	flesh_up_ab_svu	=> 'cse_alife_item_pda',
	flesh_up_cd_svu	=> 'cse_alife_item_pda',
	flesh_up_abcd_svu	=> 'cse_alife_item_pda',
	flesh_up_ab_pkm	=> 'cse_alife_item_pda',
	flesh_up_cd_pkm	=> 'cse_alife_item_pda',
	flesh_up_abcd_pkm	=> 'cse_alife_item_pda',
	flesh_up_ac_desert_eagle	=> 'cse_alife_item_pda',
	flesh_up_bd_desert_eagle	=> 'cse_alife_item_pda',
	flesh_up_ac_spas12	=> 'cse_alife_item_pda',
};
sub launch {
	print "scanning your gamedata folder...";
	my $stalker_path = $_[1];
	my $s_to_cl = IO::File->new('sections.ini', 'w');
	my %engine_hash = ();
	my $obj = {};
	$obj->{sections_hash} = ();
	$obj->{sections_list} = [];
	my %table_hash = ();
	if (defined $stalker_path) {
		scan_system($stalker_path, '\system.ltx', $obj);
		foreach my $section (@{$obj->{sections_list}}) {
			delete ($obj->{sections_hash}{$section}) if substr($section, 0, 3) eq 'mp_';
			next if substr($section, 0, 3) eq 'mp_';
			my $sect = $section;
			while (1) {
				if (defined $obj->{sections_hash}{$section}{class}) {
					my $value = $obj->{sections_hash}{$section}{class};
					$obj->{sections_hash}{$sect}{class} = $value if !exists($obj->{sections_hash}{$sect}{class});
					last;
				} elsif (defined $obj->{sections_hash}{$section}{parent})  {
					$section = $obj->{sections_hash}{$section}{parent};
					next;
				} else {
					delete($obj->{sections_hash}{$sect});
					last;
				}
			}		
		}
		my %rev_hash = ();
		foreach my $section (keys %{$obj->{sections_hash}}) {
			my $class = $obj->{sections_hash}{$section}{class};
			next if !(defined $class);
			next if defined $rev_hash{$class};
			if (defined section_to_class->{$section}) {
				$rev_hash{$class} = section_to_class->{$section};
			}
		}
		foreach my $section (keys %{$obj->{sections_hash}}) {
			my $class = $obj->{sections_hash}{$section}{class};
			next if !(defined $class);
			if (defined section_to_class->{$section}) {
				my $sclass = section_to_class->{$section};
				$table_hash{$section} = $sclass;
			} else {
				next if !(defined $rev_hash{$class});
				$table_hash{$section} = $rev_hash{$class};
			}
		}
		print $s_to_cl "[sections]\n";
		foreach my $section (sort {$table_hash{$a} cmp $table_hash{$b}} keys %table_hash) {
			print $s_to_cl "$section	=	$table_hash{$section}\n";
		}
	} else {
		die usage();
	}
	$s_to_cl->close();
	print "done!\n";
}
sub get_class {
	return section_to_class->{$_[1]};
}
sub scan_system {
	my ($stalker_path, $path, $obj) = @_;
	my $system = read_ini($stalker_path.$path);
	push @{$obj->{sections_list}}, @{$system->{sections_list}};
	foreach my $section (%{$system->{sections_hash}}) {
		$obj->{sections_hash}{$section}{class} = $system->{sections_hash}{$section}{class} if defined $system->{sections_hash}{$section}{class};
		$obj->{sections_hash}{$section}{parent} = $system->{sections_hash}{$section}{parent} if defined $system->{sections_hash}{$section}{parent};
	}
	my @folder = split /\\/, $_[1];
	splice @folder, $#folder;
	foreach my $path (@{$system->{sections_paths}}) {
		temp_funct($stalker_path, join('\\', @folder).'\\'.substr($path, 1, -1), $obj) if -e ($stalker_path.join('\\', @folder).'\\'.substr($path, 1, -1));
	}
}
sub temp_funct {
	scan_system(@_);
}
sub read_ini {
	my $fh = IO::File->new($_[0], 'r') or die "cannot open $_[0]\n";
	my $self = {};
	$self->{fh} = $fh;
	$self->{sections_paths} = [];
	$self->{sections_list} = [];
	$self->{sections_hash} = ();

	my $section;
	while (<$fh>) {
		$_ =~ qr/^\s*;/ and next;
		if (/^\[(.*)\]\s*:\s*(\w.*)?/) {
			$section = $1;
			die if defined $self->{sections_hash}->{$section};
			push @{$self->{sections_list}}, $section;
			my %tmp = ();
			$self->{sections_hash}{$section} = \%tmp;
			my $parent = $2;
			if ((defined $parent) && ($parent =~ /^([A-Za-z_0-9\.\-]+)/)) {
				$self->{sections_hash}{$section}{parent} = $1;
			}
			next;
		} elsif (/^\[(.*)\]\s*:*\s*(\w.*)?/) {
			$section = $1;
			die if defined $self->{sections_hash}->{$section};
			push @{$self->{sections_list}}, $section;
			my %tmp = ();
			$self->{sections_hash}{$section} = \%tmp;
			next;			
		}
		if (/^([^=]*?)\s*=\s*(.*?)\s*$/) {
			my ($name, $value) = ($1, $2);
			die "died on $name, $value\n" unless defined $section;
			if ($value =~ /^\W+(\w+)\W+/) {
				$value = $1;
			}
			if ($name =~ /^\W+(\w+)\W+/) {
				$self->{sections_hash}{$section}{$1} = $value;
			} else {
				$self->{sections_hash}{$section}{$name} = $value;
			}
		}
		if (/^(#include)\s*(.*)\s*$/) {
			push @{$self->{sections_paths}}, $2;
		}
	}
	$fh->close();
	return $self;
}
1;
#######################################################################