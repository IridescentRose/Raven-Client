#include "Client.h"
#include <json/json.h>
#include <Network/NetworkDriver.h>
#include "Config.h"
#include "Internal/InternalClient.h"
#include <psprtc.h>
#include "Protocol/1-12-2.h"
using namespace Minecraft::Client::Protocol;
namespace Minecraft::Client {
	Client::Client()
	{
		running = false;
		thr = NULL;
	}
	Client::~Client()
	{
	}
	void Client::run()
	{
		running = true;
		utilityPrint("Starting Client...", LOGGER_LEVEL_INFO);
		if (!Network::g_NetworkDriver.Init()) {
			throw std::runtime_error("Fatal: Could not connect to Network! Check Stardust Core Logs!");
		}

#ifdef RAVEN_CLIENT_DEBUG
		pspDebugScreenInit();
		pspDebugScreenClear();
		pspDebugScreenSetXY(0, 0);
#endif

		if (!Network::g_NetworkDriver.Connect(g_Config.port, g_Config.ip.c_str(), false)) {
			throw std::runtime_error("Fatal: Could not connect to server!");
		}

		Internal::g_InternalClient = new Internal::InternalClient();
		Internal::g_InternalClient->start();

		connState = CONNECTION_STATE_HANDSHAKE;
		//We need to send a handshake - get status
		getStatus();
		sceKernelDelayThread(1000 * 1000);
		//Then reconnect and handshake - login
		login();
	}
	bool Client::isRunning()
	{
		return running;
	}
	void Client::update()
	{
		Internal::g_InternalClient->update();
	}
	void Client::draw()
	{
		Internal::g_InternalClient->draw();
	}


	//This is 1.12.2 protocol! Technically, we just need to swap protocol version to upgrade :)
	void Client::getStatus()
	{
		utilityPrint("Asking Server for Status!", LOGGER_LEVEL_INFO);

		Network::PacketOut* p = new Network::PacketOut();
		p->ID = 0x00;

		Network::encodeVarInt(340, p->bytes);
		Network::encodeStringNonNull(g_Config.ip, *p);
		Network::encodeShort(g_Config.port, *p);
		Network::encodeVarInt(CONNECTION_STATE_STATUS, p->bytes); //Status

		Network::g_NetworkDriver.AddPacket(p);
		Network::g_NetworkDriver.SendPackets(false);


		Network::PacketOut* p2 = new Network::PacketOut();
		p2->ID = 0x00;
		Network::g_NetworkDriver.AddPacket(p2);
		Network::g_NetworkDriver.SendPackets(false);

		//Wait for response
		Network::g_NetworkDriver.ReceivePacket(false);

		Network::PacketOut* p3 = new Network::PacketOut();
		p3->ID = 0x01;

		uint64_t time = 0;
		sceRtcGetCurrentTick(&time);
		Network::encodeLong(time, *p3);

		Network::g_NetworkDriver.AddPacket(p3);
		Network::g_NetworkDriver.SendPackets(false);

		//Wait for response
		Network::g_NetworkDriver.ReceivePacket(false);

		//We can safely assume we got both packets back!

		Network::PacketIn* p4 = Network::g_NetworkDriver.unhandledPackets.front();

		std::string jsonString = Network::decodeStringLE(*p4);

		Json::Value v;
		Json::Reader r;

		if (!r.parse(jsonString.c_str(), v)) {
			throw std::runtime_error("INVALID JSON!\n" + r.getFormattedErrorMessages());
		}

		//First check is protocol version!
		int prot = v["version"]["protocol"].asInt();
		if (prot != 340) {
			utilityPrint("Server is not a valid 1.12.2 server!", LOGGER_LEVEL_WARN);
		}

		//Display Server listing
		utilityPrint("Server #1 Status\n", LOGGER_LEVEL_INFO);
		utilityPrint("MOTD: " + v["description"]["text"].asString(), LOGGER_LEVEL_INFO);
		utilityPrint("Players: " + v["players"]["online"].asString() + "/" + v["players"]["max"].asString(), LOGGER_LEVEL_INFO);
		utilityPrint("Version: " + v["version"]["name"].asString() + "\n", LOGGER_LEVEL_INFO);

		//We should have 2 packets. We don't care about the pong.
		delete Network::g_NetworkDriver.unhandledPackets.front();
		Network::g_NetworkDriver.unhandledPackets.pop();
		delete Network::g_NetworkDriver.unhandledPackets.front();
		Network::g_NetworkDriver.unhandledPackets.pop();
	}

	void Client::login()
	{
		if (!Network::g_NetworkDriver.Connect(g_Config.port, g_Config.ip.c_str(), false)) {
			throw std::runtime_error("Fatal: Could not connect to server!");
		}

		utilityPrint("Logging into server!", LOGGER_LEVEL_INFO);

		Network::PacketOut* p = new Network::PacketOut();
		p->ID = 0x00;

		Network::encodeVarInt(340, p->bytes);
		Network::encodeStringNonNull(g_Config.ip, *p);
		Network::encodeShort(g_Config.port, *p);
		Network::encodeVarInt(CONNECTION_STATE_LOGIN, p->bytes); //Status

		Network::g_NetworkDriver.AddPacket(p);
		Network::g_NetworkDriver.SendPackets(false);

		Network::PacketOut* p2 = new Network::PacketOut();
		p2->ID = 0x00;

		Network::encodeStringNonNull(g_Config.username, *p2);
		Network::g_NetworkDriver.AddPacket(p2);
		Network::g_NetworkDriver.SendPackets(false);

		Network::g_NetworkDriver.ReceivePacket(false);

		Network::PacketIn* p3 = Network::g_NetworkDriver.unhandledPackets.front();

		//These do indeed work - are irrelevant for now
		std::string uuid = Network::decodeStringNonNullLE(*p3);
		std::string user = Network::decodeStringNonNullLE(*p3);

		delete Network::g_NetworkDriver.unhandledPackets.front();
		Network::g_NetworkDriver.unhandledPackets.pop();

		Network::g_NetworkDriver.ClearPacketHandlers();

		Network::g_NetworkDriver.AddPacketHandler(Packets::SPAWN_OBJECT, spawn_object_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SPAWN_EXPERIENCE_ORB, spawn_experience_orb_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SPAWN_GLOBAL_ENTITY, spawn_global_entity_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SPAWN_MOB, spawn_mob_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SPAWN_PAINTING, spawn_painting_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SPAWN_PLAYER, spawn_player_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ANIMATION, animation_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::STATISTICS, statistics_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::BLOCK_BREAK_ANIMATION, block_break_animation_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::UPDATE_BLOCK_ENTITY, update_block_entity_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::BLOCK_ACTION, block_action_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::BLOCK_CHANGE, block_change_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::BOSS_BAR, boss_bar_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SERVER_DIFFICULTY, server_difficulty_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::TAB_COMPLETE, tab_complete_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::CHAT_MESSAGE, chat_message_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::MULTI_BLOCK_CHANGE, multi_block_change_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::CONFIRM_TRANSACTION, confirm_transaction_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::CLOSE_WINDOW, close_window_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::OPEN_WINDOW, open_window_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::WINDOW_ITEMS, window_items_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::WINDOW_PROPERTY, window_property_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SET_SLOT, set_slot_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SET_COOLDOWN, set_cooldown_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::PLUGIN_MESSAGE, plugin_message_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::NAMED_SOUND_EFFECT, named_sound_effect_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::DISCONNECT, disconnect_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_STATUS, entity_status_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::EXPLOSION, explosion_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::UNLOAD_CHUNK, unload_chunk_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::CHANGE_GAME_STATE, change_game_state_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::KEEP_ALIVE, keep_alive_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::CHUNK_DATA, chunk_data_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::EFFECT, effect_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::PARTICLE, particle_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::JOIN_GAME, join_game_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::MAP, map_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY, entity_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_RELATIVE_MOVE, entity_relative_move_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_LOOK_AND_RELATIVE_MOVE, entity_look_and_relative_move_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_LOOK, entity_look_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::VEHICLE_MOVE, vehicle_move_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::OPEN_SIGN_EDITOR, open_sign_editor_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::CRAFT_RECIPE_RESPONSE, craft_recipe_response_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::PLAYER_ABILITIES, player_abilities_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::COMBAT_EVENT, combat_event_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::PLAYER_LIST_ITEM, player_list_item_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::PLAYER_POSITION_AND_LOOK, player_position_and_look_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::USE_BED, use_bed_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::UNLOCK_RECIPES, unlock_recipes_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::DESTROY_ENTITIES, destroy_entities_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::REMOVE_ENTITY_EFFECT, remove_entity_effect_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::RESOURCE_PACK_SEND, resource_pack_send_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::RESPAWN, respawn_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_HEAD_LOOK, entity_head_look_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SELECT_ADVANCEMENT_TAB, select_advancement_tab_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::WORLD_BORDER, world_border_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::CAMERA, camera_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::HELD_ITEM_CHANGE, held_item_change_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::DISPLAY_SCOREBOARD, display_scoreboard_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_METADATA, entity_metadata_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ATTACH_ENTITY, attach_entity_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_VELOCITY, entity_velocity_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_EQUIPMENT, entity_equipment_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SET_EXPERIENCE, set_experience_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::UPDATE_HEALTH, update_health_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SCOREBOARD_OBJECTIVE, scoreboard_objective_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SET_PASSENGERS, set_passengers_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::TEAMS, teams_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::UPDATE_SCORE, update_score_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SPAWN_POSITION, spawn_position_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::TIME_UPDATE, time_update_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::TITLE, title_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::SOUND_EFFECT, sound_effect_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::PLAYER_LIST_HEADER_AND_FOOTER, player_list_header_and_footer_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::COLLECT_ITEM, collect_item_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_TELEPORT, entity_teleport_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ADVANCEMENTS, advancements_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_PROPERTIES, entity_properties_handler);
		Network::g_NetworkDriver.AddPacketHandler(Packets::ENTITY_EFFECT, entity_effect_handler);

		//Let's make a thread too
		if (thr != NULL) {
			thr->Kill();
			delete thr;
		}

		g_NetworkDriver.m_Socket.SetBlock(false);
		thr = new Thread(thread_network);
		thr->Start(0);
	}

	int Client::thread_network(unsigned int i, void* a)
	{
		g_NetworkDriver.ClearPacketQueue();
		while (g_NetworkDriver.m_Socket.isAlive()) {
			g_NetworkDriver.SendPackets(false);
			
			int packCount = 0;
			while (packCount < 50) {
				g_NetworkDriver.ReceivePacket(false);
				packCount++;
			}
			g_NetworkDriver.HandlePackets();

			sceKernelDelayThread(1000 * 50);
		}
		return 0;
	}

	Client* g_Client = NULL;
}