#include "1-12-2.h"
#include "../Internal/InternalClient.h"

namespace Minecraft::Client::Protocol {
	int spawn_object_handler(PacketIn* p) { utilityPrint("SPAWN_OBJECT", LOGGER_LEVEL_WARN); return 0; }
	int spawn_experience_orb_handler(PacketIn* p) { utilityPrint("SPAWN_EXPERIENCE_ORB", LOGGER_LEVEL_WARN); return 0; }
	int spawn_global_entity_handler(PacketIn* p) { utilityPrint("SPAWN_GLOBAL_ENTITY", LOGGER_LEVEL_WARN); return 0; }
	int spawn_mob_handler(PacketIn* p) { utilityPrint("SPAWN_MOB", LOGGER_LEVEL_WARN); return 0; }
	int spawn_painting_handler(PacketIn* p) { utilityPrint("SPAWN_PAINTING", LOGGER_LEVEL_WARN); return 0; }
	int spawn_player_handler(PacketIn* p) { utilityPrint("SPAWN_PLAYER", LOGGER_LEVEL_WARN); return 0; }
	int animation_handler(PacketIn* p) { utilityPrint("ANIMATION", LOGGER_LEVEL_WARN); return 0; }
	int statistics_handler(PacketIn* p) { utilityPrint("STATISTICS", LOGGER_LEVEL_WARN); return 0; }
	int block_break_animation_handler(PacketIn* p) { utilityPrint("BLOCK_BREAK_ANIMATION", LOGGER_LEVEL_WARN); return 0; }
	int update_block_entity_handler(PacketIn* p) { utilityPrint("UPDATE_BLOCK_ENTITY", LOGGER_LEVEL_WARN); return 0; }
	int block_action_handler(PacketIn* p) { utilityPrint("BLOCK_ACTION", LOGGER_LEVEL_WARN); return 0; }
	int block_change_handler(PacketIn* p) { utilityPrint("BLOCK_CHANGE", LOGGER_LEVEL_WARN); return 0; }
	int boss_bar_handler(PacketIn* p) { utilityPrint("BOSS_BAR", LOGGER_LEVEL_WARN); return 0; }
	
	int server_difficulty_handler(PacketIn* p) { 
		utilityPrint("SERVER_DIFFICULTY", LOGGER_LEVEL_TRACE);
		Internal::g_ClientInfo.difficulty = decodeByte(*p);
		utilityPrint("DIFFICULTY: " + std::to_string((int)Internal::g_ClientInfo.difficulty), LOGGER_LEVEL_TRACE);
		return 0; 
	}
	
	int tab_complete_handler(PacketIn* p) { utilityPrint("TAB_COMPLETE", LOGGER_LEVEL_WARN); return 0; }
	int chat_message_handler(PacketIn* p) { utilityPrint("CHAT_MESSAGE", LOGGER_LEVEL_WARN); return 0; }
	int multi_block_change_handler(PacketIn* p) { utilityPrint("MULTI_BLOCK_CHANGE", LOGGER_LEVEL_WARN); return 0; }
	int confirm_transaction_handler(PacketIn* p) { utilityPrint("CONFIRM_TRANSACTION", LOGGER_LEVEL_WARN); return 0; }
	int close_window_handler(PacketIn* p) { utilityPrint("CLOSE_WINDOW", LOGGER_LEVEL_WARN); return 0; }
	int open_window_handler(PacketIn* p) { utilityPrint("OPEN_WINDOW", LOGGER_LEVEL_WARN); return 0; }
	int window_items_handler(PacketIn* p) { utilityPrint("WINDOW_ITEMS", LOGGER_LEVEL_WARN); return 0; }
	int window_property_handler(PacketIn* p) { utilityPrint("WINDOW_PROPERTY", LOGGER_LEVEL_WARN); return 0; }
	int set_slot_handler(PacketIn* p) { utilityPrint("SET_SLOT", LOGGER_LEVEL_WARN); return 0; }
	int set_cooldown_handler(PacketIn* p) { utilityPrint("SET_COOLDOWN", LOGGER_LEVEL_WARN); return 0; }

	int plugin_message_handler(PacketIn* p) { 
		utilityPrint("PLUGIN_MESSAGE", LOGGER_LEVEL_TRACE);
		//TODO: Actually support plugins in the future
		std::string str = decodeStringNonNullLE(*p);

		if (str == "MC|Brand") {
			//Hey, it's a Brand plugin! We gotta respond.
			std::string serv_brand = decodeStringNonNullLE(*p);
			
			utilityPrint("Server Brand: " + serv_brand, LOGGER_LEVEL_TRACE);

			PacketOut* brandResp = new PacketOut();
			brandResp->ID = 0x09;
			encodeStringLE("MC|Brand", *brandResp);
			encodeStringLE("PSP-Raven", *brandResp);

			g_NetworkDriver.AddPacket(brandResp);
			g_NetworkDriver.SendPackets(false);
		}

		//This is super useless
		utilityPrint(str, LOGGER_LEVEL_TRACE);

		return 0; 
	
	}
	int named_sound_effect_handler(PacketIn* p) { utilityPrint("NAMED_SOUND_EFFECT", LOGGER_LEVEL_WARN); return 0; }
	int disconnect_handler(PacketIn* p) { utilityPrint("DISCONNECT", LOGGER_LEVEL_WARN); return 0; }
	
	int entity_status_handler(PacketIn* p) { 
		//TODO: SHOULD CREATE EVENT
		utilityPrint("ENTITY_STATUS", LOGGER_LEVEL_TRACE);

		int id = decodeInt(*p);
		uint8_t action = decodeByte(*p);

		utilityPrint("EID: " + std::to_string((int)id), LOGGER_LEVEL_TRACE);
		utilityPrint("ACT: " + std::to_string((int)action), LOGGER_LEVEL_TRACE);


		return 0; 
	}
	
	int explosion_handler(PacketIn* p) { utilityPrint("EXPLOSION", LOGGER_LEVEL_WARN); return 0; }
	int unload_chunk_handler(PacketIn* p) { utilityPrint("UNLOAD_CHUNK", LOGGER_LEVEL_WARN); return 0; }
	int change_game_state_handler(PacketIn* p) { utilityPrint("CHANGE_GAME_STATE", LOGGER_LEVEL_WARN); return 0; }
	int keep_alive_handler(PacketIn* p) { utilityPrint("KEEP_ALIVE", LOGGER_LEVEL_WARN); return 0; }
	int chunk_data_handler(PacketIn* p) { utilityPrint("CHUNK_DATA", LOGGER_LEVEL_WARN); return 0; }
	int effect_handler(PacketIn* p) { utilityPrint("EFFECT", LOGGER_LEVEL_WARN); return 0; }
	int particle_handler(PacketIn* p) { utilityPrint("PARTICLE", LOGGER_LEVEL_WARN); return 0; }
	
	int join_game_handler(PacketIn* p) {
		utilityPrint("JOIN_GAME", LOGGER_LEVEL_TRACE); 

		Internal::g_ClientInfo.eid = decodeInt(*p);
		Internal::g_ClientInfo.gamemode = decodeByte(*p);
		Internal::g_ClientInfo.dimension = decodeInt(*p);
		Internal::g_ClientInfo.difficulty = decodeByte(*p);

		//USELESS PARAM!
		Internal::g_ClientInfo.maxPlayers = decodeByte(*p);

		Internal::g_ClientInfo.levelType = decodeStringNonNullLE(*p);

		Internal::g_ClientInfo.reducedInfo = decodeBool(*p);

		utilityPrint("ID: " + std::to_string(Internal::g_ClientInfo.eid), LOGGER_LEVEL_TRACE);
		utilityPrint("GAMEMODE: " + std::to_string((int)Internal::g_ClientInfo.gamemode), LOGGER_LEVEL_TRACE);
		utilityPrint("DIMENSION: " + std::to_string(Internal::g_ClientInfo.dimension), LOGGER_LEVEL_TRACE);
		utilityPrint("DIFFICULTY: " + std::to_string((int)Internal::g_ClientInfo.difficulty), LOGGER_LEVEL_TRACE);
		utilityPrint("LEVEL: " + std::to_string(Internal::g_ClientInfo.levelType), LOGGER_LEVEL_TRACE);

		//We should respond with a client settings
		//TODO: USE OPTIONS!
		PacketOut* client_settings = new PacketOut();
		client_settings->ID = 0x04;
		encodeStringNonNull("en_us", *client_settings);
		encodeByte(2, *client_settings); //Render Distance!
		encodeByte(0, *client_settings); //CHAT MODE 0 default
		encodeBool(false, *client_settings); //Colors is off
		encodeByte(0b01111111, *client_settings); //Display all parts of skin
		encodeByte(1, *client_settings); //Default hand (1 = Right)

		g_NetworkDriver.AddPacket(client_settings);
		g_NetworkDriver.SendPackets(false);

		return 0; 
	}
	
	int map_handler(PacketIn* p) { utilityPrint("MAP", LOGGER_LEVEL_WARN); return 0; }
	int entity_handler(PacketIn* p) { utilityPrint("ENTITY", LOGGER_LEVEL_WARN); return 0; }
	int entity_relative_move_handler(PacketIn* p) { utilityPrint("ENTITY_RELATIVE_MOVE", LOGGER_LEVEL_WARN); return 0; }
	int entity_look_and_relative_move_handler(PacketIn* p) { utilityPrint("ENTITY_LOOK_AND_RELATIVE_MOVE", LOGGER_LEVEL_WARN); return 0; }
	int entity_look_handler(PacketIn* p) { utilityPrint("ENTITY_LOOK", LOGGER_LEVEL_WARN); return 0; }
	int vehicle_move_handler(PacketIn* p) { utilityPrint("VEHICLE_MOVE", LOGGER_LEVEL_WARN); return 0; }
	int open_sign_editor_handler(PacketIn* p) { utilityPrint("OPEN_SIGN_EDITOR", LOGGER_LEVEL_WARN); return 0; }
	int craft_recipe_response_handler(PacketIn* p) { utilityPrint("CRAFT_RECIPE_RESPONSE", LOGGER_LEVEL_WARN); return 0; }
	
	int player_abilities_handler(PacketIn* p) { 
		utilityPrint("PLAYER_ABILITIES", LOGGER_LEVEL_TRACE);

		uint8_t mask = decodeByte(*p);
		Internal::g_ClientInfo.invulnerable = mask & 0x01;
		Internal::g_ClientInfo.flying = mask & 0x02;
		Internal::g_ClientInfo.allowed_fly = mask & 0x04;
		Internal::g_ClientInfo.creative = mask & 0x08;

		Internal::g_ClientInfo.flySpeed = decodeFloat(*p);
		Internal::g_ClientInfo.fovMod = decodeFloat(*p);

		utilityPrint("FLY_SPEED: " + std::to_string(Internal::g_ClientInfo.flySpeed), LOGGER_LEVEL_TRACE);
		utilityPrint("FOV_MOD: " + std::to_string(Internal::g_ClientInfo.fovMod), LOGGER_LEVEL_TRACE);

		return 0; 
	}

	int combat_event_handler(PacketIn* p) { utilityPrint("COMBAT_EVENT", LOGGER_LEVEL_WARN); return 0; }
	int player_list_item_handler(PacketIn* p) { utilityPrint("PLAYER_LIST_ITEM", LOGGER_LEVEL_WARN); sceKernelDelayThread(1000 * 5 * 1000); sceKernelExitGame(); return 0; }
	int player_position_and_look_handler(PacketIn* p) { utilityPrint("PLAYER_POSITION_AND_LOOK", LOGGER_LEVEL_WARN); return 0; }
	int use_bed_handler(PacketIn* p) { utilityPrint("USE_BED", LOGGER_LEVEL_WARN); return 0; }
	
	int unlock_recipes_handler(PacketIn* p) {
		utilityPrint("UNLOCK_RECIPES", LOGGER_LEVEL_WARN);
		utilityPrint("NOT FULLY IMPLEMENTED!!!", LOGGER_LEVEL_WARN);

		uint8_t action = decodeByte(*p);
		if (p == 0) {
			utilityPrint("RECIPE INIT", LOGGER_LEVEL_TRACE);
		}

		return 0;
	}
	
	int destroy_entities_handler(PacketIn* p) { utilityPrint("DESTROY_ENTITIES", LOGGER_LEVEL_WARN); return 0; }
	int remove_entity_effect_handler(PacketIn* p) { utilityPrint("REMOVE_ENTITY_EFFECT", LOGGER_LEVEL_WARN); return 0; }
	int resource_pack_send_handler(PacketIn* p) { utilityPrint("RESOURCE_PACK_SEND", LOGGER_LEVEL_WARN); return 0; }
	int respawn_handler(PacketIn* p) { utilityPrint("RESPAWN", LOGGER_LEVEL_WARN); return 0; }
	int entity_head_look_handler(PacketIn* p) { utilityPrint("ENTITY_HEAD_LOOK", LOGGER_LEVEL_WARN); return 0; }
	int select_advancement_tab_handler(PacketIn* p) { utilityPrint("SELECT_ADVANCEMENT_TAB", LOGGER_LEVEL_WARN); return 0; }
	int world_border_handler(PacketIn* p) { utilityPrint("WORLD_BORDER", LOGGER_LEVEL_WARN); return 0; }
	int camera_handler(PacketIn* p) { utilityPrint("CAMERA", LOGGER_LEVEL_WARN); return 0; }

	int held_item_change_handler(PacketIn* p) {
		//TODO: SHOULD CREATE EVENT
		utilityPrint("HELD_ITEM_CHANGE", LOGGER_LEVEL_TRACE);

		uint8_t slot = decodeByte(*p);

		utilityPrint("Slot: " + std::to_string((int)slot), LOGGER_LEVEL_TRACE);
		Internal::g_ClientInfo.hotbarSlot = slot;

		return 0; 
	}

	int display_scoreboard_handler(PacketIn* p) { utilityPrint("DISPLAY_SCOREBOARD", LOGGER_LEVEL_WARN); return 0; }
	int entity_metadata_handler(PacketIn* p) { utilityPrint("ENTITY_METADATA", LOGGER_LEVEL_WARN); return 0; }
	int attach_entity_handler(PacketIn* p) { utilityPrint("ATTACH_ENTITY", LOGGER_LEVEL_WARN); return 0; }
	int entity_velocity_handler(PacketIn* p) { utilityPrint("ENTITY_VELOCITY", LOGGER_LEVEL_WARN); return 0; }
	int entity_equipment_handler(PacketIn* p) { utilityPrint("ENTITY_EQUIPMENT", LOGGER_LEVEL_WARN); return 0; }
	int set_experience_handler(PacketIn* p) { utilityPrint("SET_EXPERIENCE", LOGGER_LEVEL_WARN); return 0; }
	int update_health_handler(PacketIn* p) { utilityPrint("UPDATE_HEALTH", LOGGER_LEVEL_WARN); return 0; }
	int scoreboard_objective_handler(PacketIn* p) { utilityPrint("SCOREBOARD_OBJECTIVE", LOGGER_LEVEL_WARN); return 0; }
	int set_passengers_handler(PacketIn* p) { utilityPrint("SET_PASSENGERS", LOGGER_LEVEL_WARN); return 0; }
	int teams_handler(PacketIn* p) { utilityPrint("TEAMS", LOGGER_LEVEL_WARN); return 0; }
	int update_score_handler(PacketIn* p) { utilityPrint("UPDATE_SCORE", LOGGER_LEVEL_WARN); return 0; }
	int spawn_position_handler(PacketIn* p) { utilityPrint("SPAWN_POSITION", LOGGER_LEVEL_WARN); return 0; }
	int time_update_handler(PacketIn* p) { utilityPrint("TIME_UPDATE", LOGGER_LEVEL_WARN); return 0; }
	int title_handler(PacketIn* p) { utilityPrint("TITLE", LOGGER_LEVEL_WARN); return 0; }
	int sound_effect_handler(PacketIn* p) { utilityPrint("SOUND_EFFECT", LOGGER_LEVEL_WARN); return 0; }
	int player_list_header_and_footer_handler(PacketIn* p) { utilityPrint("PLAYER_LIST_HEADER_AND_FOOTER", LOGGER_LEVEL_WARN); return 0; }
	int collect_item_handler(PacketIn* p) { utilityPrint("COLLECT_ITEM", LOGGER_LEVEL_WARN); return 0; }
	int entity_teleport_handler(PacketIn* p) { utilityPrint("ENTITY_TELEPORT", LOGGER_LEVEL_WARN); return 0; }
	int advancements_handler(PacketIn* p) { utilityPrint("ADVANCEMENTS", LOGGER_LEVEL_WARN); return 0; }
	int entity_properties_handler(PacketIn* p) { utilityPrint("ENTITY_PROPERTIES", LOGGER_LEVEL_WARN); return 0; }
	int entity_effect_handler(PacketIn* p) { utilityPrint("ENTITY_EFFECT", LOGGER_LEVEL_WARN); return 0; }
}