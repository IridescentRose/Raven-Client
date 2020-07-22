#include "Client.h"
#include "../Config.h"
#include <iostream>
#include "Protocol.h"
namespace Minecraft {
	using namespace Protocol;
	Client::Client()
	{
	}

	Client::~Client()
	{
	}

	void send_handshake(int status){
		Network::PacketOut* packet = new Network::PacketOut(64);
		packet->ID = 0;
		packet->buffer->WriteVarInt32(404); //1.13.2 protocol
		packet->buffer->WriteVarUTF8String(g_Config.ip);
		packet->buffer->WriteBEUInt16(g_Config.port);
		packet->buffer->WriteBEUInt8(status); //Status ping

		//Send handshake
		Network::g_NetworkDriver.AddPacket(packet);
		Network::g_NetworkDriver.SendPackets(false);
	}

	void Client::testPing(){
		send_handshake(1);
		//Assuming this works, we need to send our status request and also send a ping packet.
		Network::PacketOut* packet2 = new Network::PacketOut(1);
		packet2->ID = 0;
		//This is blank
		Network::g_NetworkDriver.AddPacket(packet2);
		Network::g_NetworkDriver.SendPackets(false);


		//Wait a good 10 ticks
		Platform::delayForMS(500);
		int timeout = 0;
		Network::PacketIn* pIn = csock->Recv(false);
		while(pIn == NULL){
			pIn = csock->Recv(false);
			Platform::delayForMS(50);
			timeout++;

			if(timeout == 100){
				throw std::runtime_error("COULDN'T CONNECT!");
			}
		}

		//So we must have received a valid client status to get here.
		std::string serverData = "";
		pIn->buffer->ReadVarUTF8String(serverData);
		std::cout << serverData << std::endl;

		delete pIn;
		pIn = NULL;

		//Ping
		Network::PacketOut* packet3 = new Network::PacketOut(9);
		packet3->ID = 0x01;
		packet3->buffer->WriteBEUInt64(0xCAFEBABE);
		Network::g_NetworkDriver.AddPacket(packet3);
		Network::g_NetworkDriver.SendPackets(false);
		timeout = 0;
		pIn = csock->Recv(false);
		while (pIn == NULL) {
			pIn = csock->Recv(false);
			Platform::delayForMS(50);
			timeout++;

			if (timeout == 100) {
				throw std::runtime_error("COULDN'T GET PING!");
			}
		}

		uint64_t pingData = 0;
		pIn->buffer->ReadBEUInt64(pingData);

		std::cout << "PONG: " << std::hex << pingData << std::endl;

		csock->Close();
		//We're good

	}

	std::string UUID = "";

	void Client::login(){

		send_handshake(2);
		Network::PacketOut* p = new Network::PacketOut(32);

		p->ID = 0;
		p->buffer->WriteVarUTF8String(g_Config.username);

		Network::g_NetworkDriver.AddPacket(p);
		Network::g_NetworkDriver.SendPackets(false);

		bool needRecheck = false;

		do {
			needRecheck = false;
			int timeout = 0;
			Network::PacketIn* pIn = csock->Recv(false);
			while (pIn == NULL) {
				pIn = csock->Recv(false);
				Platform::delayForMS(50);
				timeout++;

				if (timeout == 100) {
					throw std::runtime_error("COULDN'T GET COMPRESSION / LOGIN!");
				}
			}

			switch (pIn->ID) {
			case 0x00:				
				throw std::runtime_error("DISCONNECTED!");
				break;

			case 0x01:
				throw std::runtime_error("NO ENCRYPTION SUPPORT!");
				break;

			case 0x02:
				//Login success, no compress
				pIn->buffer->ReadVarUTF8String(UUID);
				std::cout << UUID << std::endl;
				return;
				break;

			case 0x03:
				//Compression enable
				uint32_t threshold = -1;
				pIn->buffer->ReadVarInt32(threshold);

				std::cout << "THRESHOLD: " << std::dec << threshold << std::endl;

				if(threshold != -1){
					csock->threshold = threshold;
					csock->compression = true;
				}

				needRecheck = true;
				break;
			}

		} while (needRecheck);

	}

	void Client::init()
	{
		Network::g_NetworkDriver.Init();

		//Connect
		csock = new Network::ClientSocket();
		csock->Connect(g_Config.port, g_Config.ip.c_str());

		Network::g_NetworkDriver.setSocket(csock);

		//At this point we're being invoked - not neutral - hence we should connect.
		//We should test ping for inquiry

		testPing();

		csock->Connect(g_Config.port, g_Config.ip.c_str());
		csock->SetBlock(false);
		login();
		
		//Set up client connection state.
		
		Network::g_NetworkDriver.AddPacketHandler(SPAWN_OBJECT, spawn_object_handler);
		Network::g_NetworkDriver.AddPacketHandler(SPAWN_EXPERIENCE_ORB, spawn_experience_orb_handler);
		Network::g_NetworkDriver.AddPacketHandler(SPAWN_GLOBAL_ENTITY, spawn_global_entity_handler);
		Network::g_NetworkDriver.AddPacketHandler(SPAWN_MOB, spawn_mob_handler);
		Network::g_NetworkDriver.AddPacketHandler(SPAWN_PAINTING, spawn_painting_handler);
		Network::g_NetworkDriver.AddPacketHandler(SPAWN_PLAYER, spawn_player_handler);
		Network::g_NetworkDriver.AddPacketHandler(ANIMATION, animation_handler);
		Network::g_NetworkDriver.AddPacketHandler(STATISTICS, statistics_handler);
		Network::g_NetworkDriver.AddPacketHandler(BLOCK_BREAK_ANIMATION, block_break_animation_handler);
		Network::g_NetworkDriver.AddPacketHandler(UPDATE_BLOCK_ENTITY, update_block_entity_handler);
		Network::g_NetworkDriver.AddPacketHandler(BLOCK_ACTION, block_action_handler);
		Network::g_NetworkDriver.AddPacketHandler(BLOCK_CHANGE, block_change_handler);
		Network::g_NetworkDriver.AddPacketHandler(BOSS_BAR, boss_bar_handler);
		Network::g_NetworkDriver.AddPacketHandler(SERVER_DIFFICULTY, server_difficulty_handler);
		Network::g_NetworkDriver.AddPacketHandler(CHAT_MESSAGE, chat_message_handler);
		Network::g_NetworkDriver.AddPacketHandler(MULTI_BLOCK_CHANGE, multi_block_change_handler);
		Network::g_NetworkDriver.AddPacketHandler(TAB_COMPLETE, tab_complete_handler);
		Network::g_NetworkDriver.AddPacketHandler(DECLARE_COMMANDS, declare_commands_handler);
		Network::g_NetworkDriver.AddPacketHandler(CONFIRM_TRANSACTION, confirm_transaction_handler);
		Network::g_NetworkDriver.AddPacketHandler(CLOSE_WINDOW, close_window_handler);
		Network::g_NetworkDriver.AddPacketHandler(OPEN_WINDOW, open_window_handler);
		Network::g_NetworkDriver.AddPacketHandler(WINDOW_ITEMS, window_items_handler);
		Network::g_NetworkDriver.AddPacketHandler(WINDOW_PROPERTY, window_property_handler);
		Network::g_NetworkDriver.AddPacketHandler(SET_SLOT, set_slot_handler);
		Network::g_NetworkDriver.AddPacketHandler(SET_COOLDOWN, set_cooldown_handler);
		Network::g_NetworkDriver.AddPacketHandler(PLUGIN_MESSAGE, plugin_message_handler);
		Network::g_NetworkDriver.AddPacketHandler(NAMED_SOUND_EFFECT, named_sound_effect_handler);
		Network::g_NetworkDriver.AddPacketHandler(DISCONNECT, disconnect_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_STATUS, entity_status_handler);
		Network::g_NetworkDriver.AddPacketHandler(NBT_QUERY_RESPONSE, nbt_query_response_handler);
		Network::g_NetworkDriver.AddPacketHandler(EXPLOSION, explosion_handler);
		Network::g_NetworkDriver.AddPacketHandler(UNLOAD_CHUNK, unload_chunk_handler);
		Network::g_NetworkDriver.AddPacketHandler(CHANGE_GAME_STATE, change_game_state_handler);
		Network::g_NetworkDriver.AddPacketHandler(KEEP_ALIVE, keep_alive_handler);
		Network::g_NetworkDriver.AddPacketHandler(CHUNK_DATA, chunk_data_handler);
		Network::g_NetworkDriver.AddPacketHandler(EFFECT, effect_handler);
		Network::g_NetworkDriver.AddPacketHandler(PARTICLE, particle_handler);
		Network::g_NetworkDriver.AddPacketHandler(JOIN_GAME, join_game_handler);
		Network::g_NetworkDriver.AddPacketHandler(MAP_DATA, map_data_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY, entity_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_RELATIVE_MOVE, entity_relative_move_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_LOOK_AND_RELATIVE_MOVE, entity_look_and_relative_move_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_LOOK, entity_look_handler);
		Network::g_NetworkDriver.AddPacketHandler(VEHICLE_MOVE, vehicle_move_handler);
		Network::g_NetworkDriver.AddPacketHandler(OPEN_SIGN_EDITOR, open_sign_editor_handler);
		Network::g_NetworkDriver.AddPacketHandler(CRAFT_RECIPE_RESPONSE, craft_recipe_response_handler);
		Network::g_NetworkDriver.AddPacketHandler(PLAYER_ABILITIES, player_abilities_handler);
		Network::g_NetworkDriver.AddPacketHandler(COMBAT_EVENT, combat_event_handler);
		Network::g_NetworkDriver.AddPacketHandler(PLAYER_INFO, player_info_handler);
		Network::g_NetworkDriver.AddPacketHandler(FACE_PLAYER, face_player_handler);
		Network::g_NetworkDriver.AddPacketHandler(PLAYER_POSITION_AND_LOOK, player_position_and_look_handler);
		Network::g_NetworkDriver.AddPacketHandler(USE_BED, use_bed_handler);
		Network::g_NetworkDriver.AddPacketHandler(UNLOCK_RECIPES, unlock_recipes_handler);
		Network::g_NetworkDriver.AddPacketHandler(DESTROY_ENTITIES, destroy_entities_handler);
		Network::g_NetworkDriver.AddPacketHandler(REMOVE_ENTITY_EFFECT, remove_entity_effect_handler);
		Network::g_NetworkDriver.AddPacketHandler(RESOURCE_PACK_SEND, resource_pack_send_handler);
		Network::g_NetworkDriver.AddPacketHandler(RESPAWN, respawn_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_HEAD_LOOK, entity_head_look_handler);
		Network::g_NetworkDriver.AddPacketHandler(SELECT_ADVANCEMENT_TAB, select_advancement_tab_handler);
		Network::g_NetworkDriver.AddPacketHandler(WORLD_BORDER, world_border_handler);
		Network::g_NetworkDriver.AddPacketHandler(CAMERA, camera_handler);
		Network::g_NetworkDriver.AddPacketHandler(HELD_ITEM_CHANGE, held_item_change_handler);
		Network::g_NetworkDriver.AddPacketHandler(DISPLAY_SCOREBOARD, display_scoreboard_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_METADATA, entity_metadata_handler);
		Network::g_NetworkDriver.AddPacketHandler(ATTACH_ENTITY, attach_entity_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_VELOCITY, entity_velocity_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_EQUIPMENT, entity_equipment_handler);
		Network::g_NetworkDriver.AddPacketHandler(SET_EXPERIENCE, set_experience_handler);
		Network::g_NetworkDriver.AddPacketHandler(UPDATE_HEALTH, update_health_handler);
		Network::g_NetworkDriver.AddPacketHandler(SCOREBOARD_OBJECTIVE, scoreboard_objective_handler);
		Network::g_NetworkDriver.AddPacketHandler(SET_PASSENGERS, set_passengers_handler);
		Network::g_NetworkDriver.AddPacketHandler(TEAMS, teams_handler);
		Network::g_NetworkDriver.AddPacketHandler(UPDATE_SCORE, update_score_handler);
		Network::g_NetworkDriver.AddPacketHandler(SPAWN_POSITION, spawn_position_handler);
		Network::g_NetworkDriver.AddPacketHandler(TIME_UPDATE, time_update_handler);
		Network::g_NetworkDriver.AddPacketHandler(TITLE, title_handler);
		Network::g_NetworkDriver.AddPacketHandler(STOP_SOUND, stop_sound_handler);
		Network::g_NetworkDriver.AddPacketHandler(SOUND_EFFECT, sound_effect_handler);
		Network::g_NetworkDriver.AddPacketHandler(PLAYER_LIST_HEADER_AND_FOOTER, player_list_header_and_footer_handler);
		Network::g_NetworkDriver.AddPacketHandler(COLLECT_ITEM, collect_item_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_TELEPORT, entity_teleport_handler);
		Network::g_NetworkDriver.AddPacketHandler(ADVANCEMENTS, advancements_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_PROPERTIES, entity_properties_handler);
		Network::g_NetworkDriver.AddPacketHandler(ENTITY_EFFECT, entity_effect_handler);
		Network::g_NetworkDriver.AddPacketHandler(DECLARE_RECIPES, declare_recipes_handler);
		Network::g_NetworkDriver.AddPacketHandler(TAGS, tags_handler);
	}

	void Client::cleanup()
	{
		Network::g_NetworkDriver.Cleanup();
	}

	void Client::update()
	{
		int counts = 0;
		while(counts < 50){
			g_NetworkDriver.ReceivePacket(false);
			counts++;
		}
		g_NetworkDriver.HandlePackets();

	}

	void Client::draw()
	{

	}
}
