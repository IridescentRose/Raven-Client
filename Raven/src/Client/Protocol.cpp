#include "Protocol.h"
#include <json/json.h>
#include "../Common/Translation.h"
#include "World/World.h"
#include <map>

namespace Minecraft::Protocol {
	int spawn_object_handler(PacketIn* p) { std::cout << "WARNING SPAWN_OBJECT TRIGGERED" << std::endl; return 0; }
	int spawn_experience_orb_handler(PacketIn* p) { std::cout << "WARNING SPAWN_EXPERIENCE_ORB TRIGGERED" << std::endl; return 0; }
	int spawn_global_entity_handler(PacketIn* p) { std::cout << "WARNING SPAWN_GLOBAL_ENTITY TRIGGERED" << std::endl; return 0; }
	int spawn_mob_handler(PacketIn* p) { std::cout << "WARNING SPAWN_MOB TRIGGERED" << std::endl; return 0; }
	int spawn_painting_handler(PacketIn* p) { std::cout << "WARNING SPAWN_PAINTING TRIGGERED" << std::endl; return 0; }
	int spawn_player_handler(PacketIn* p) { std::cout << "WARNING SPAWN_PLAYER TRIGGERED" << std::endl; return 0; }
	int animation_handler(PacketIn* p) { std::cout << "WARNING ANIMATION TRIGGERED" << std::endl; return 0; }
	int statistics_handler(PacketIn* p) { std::cout << "WARNING STATISTICS TRIGGERED" << std::endl; return 0; }
	int block_break_animation_handler(PacketIn* p) { std::cout << "WARNING BLOCK_BREAK_ANIMATION TRIGGERED" << std::endl; return 0; }
	int update_block_entity_handler(PacketIn* p) { std::cout << "WARNING UPDATE_BLOCK_ENTITY TRIGGERED" << std::endl; return 0; }
	int block_action_handler(PacketIn* p) { std::cout << "WARNING BLOCK_ACTION TRIGGERED" << std::endl; return 0; }
	int block_change_handler(PacketIn* p) { std::cout << "WARNING BLOCK_CHANGE TRIGGERED" << std::endl; return 0; }
	int boss_bar_handler(PacketIn* p) { std::cout << "WARNING BOSS_BAR TRIGGERED" << std::endl; return 0; }
	
	int server_difficulty_handler(PacketIn* p) { 
		
		uint8_t difficulty;
		p->buffer->ReadBEUInt8(difficulty);

		std::cout << "DIFFICULTY: " << difficulty << std::endl;

		return 0; 
	}
	
	int chat_message_handler(PacketIn* p) { std::cout << "WARNING CHAT_MESSAGE TRIGGERED" << std::endl; return 0; }
	int multi_block_change_handler(PacketIn* p) { std::cout << "WARNING MULTI_BLOCK_CHANGE TRIGGERED" << std::endl; return 0; }
	int tab_complete_handler(PacketIn * p) { std::cout << "WARNING TAB-COMPLETE TRIGGERED" << std::endl; return 0; }
	int declare_commands_handler(PacketIn* p) { std::cout << "WARNING DECLARE_COMMANDS TRIGGERED" << std::endl; return 0; }
	int confirm_transaction_handler(PacketIn* p) { std::cout << "WARNING CONFIRM_TRANSACTION TRIGGERED" << std::endl; return 0; }
	int close_window_handler(PacketIn* p) { std::cout << "WARNING CLOSE_WINDOW TRIGGERED" << std::endl; return 0; }
	int open_window_handler(PacketIn* p) { std::cout << "WARNING OPEN_WINDOW TRIGGERED" << std::endl; return 0; }
	int window_items_handler(PacketIn* p) { std::cout << "WARNING WINDOW_ITEMS TRIGGERED" << std::endl; return 0; }
	int window_property_handler(PacketIn* p) { std::cout << "WARNING WINDOW_PROPERTY TRIGGERED" << std::endl; return 0; }
	int set_slot_handler(PacketIn* p) { std::cout << "WARNING SET_SLOT TRIGGERED" << std::endl; return 0; }
	int set_cooldown_handler(PacketIn* p) { std::cout << "WARNING SET_COOLDOWN TRIGGERED" << std::endl; return 0; }
	
	int plugin_message_handler(PacketIn* p) { 
		//TODO: FINISH PLUGINS
		//std::cout << "WARNING PLUGIN_MESSAGE TRIGGERED" << std::endl; 

		std::string channel;
		p->buffer->ReadVarUTF8String(channel);
		
		if(channel == "minecraft:brand"){
			PacketsOut::send_plugin_message(channel, "raven-recrafted");
		}else{
			std::cout << "UNKNOWN IDENTIFIER: " << channel << std::endl;
		}


		return 0; 
	}
	
	int named_sound_effect_handler(PacketIn* p) { std::cout << "WARNING NAMED_SOUND_EFFECT TRIGGERED" << std::endl; return 0; }
	
	int disconnect_handler(PacketIn* p) { 
		
		std::string jsonresponse = "";
		p->buffer->ReadVarUTF8String(jsonresponse);

		Json::Value root;

		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();
		std::string errors;

		bool parsingSuccessful = reader->parse(jsonresponse.c_str(), jsonresponse.c_str() + jsonresponse.size(),
			&root,
			&errors);

		if(!parsingSuccessful){
			std::cout << "Failed to parse the JSON, errors:" << std::endl;
			std::cout << errors << std::endl;
		}

		if(!root["translate"].isNull()){
			std::stringstream str;
			str << root["with"][0].asString() << std::endl;

			std::cout << str.str() << std::endl;
			throw std::runtime_error(str.str());
		}


		return 0; 
	}
	
	int entity_status_handler(PacketIn* p) { std::cout << "WARNING ENTITY_STATUS TRIGGERED" << std::endl; return 0; }
	int nbt_query_response_handler(PacketIn* p) { std::cout << "WARNING NBT_QUERY_RESPONSE TRIGGERED" << std::endl; return 0; }
	int explosion_handler(PacketIn* p) { std::cout << "WARNING EXPLOSION TRIGGERED" << std::endl; return 0; }
	int unload_chunk_handler(PacketIn* p) { std::cout << "WARNING UNLOAD_CHUNK TRIGGERED" << std::endl; return 0; }
	int change_game_state_handler(PacketIn* p) { std::cout << "WARNING CHANGE_GAME_STATE TRIGGERED" << std::endl; return 0; }
	
	int keep_alive_handler(PacketIn* p) { 

		uint64_t longData;
		p->buffer->ReadBEUInt64(longData);

		PacketsOut::send_keep_alive(longData);

		return 0; 
	}

	void readChunkColumn(Internal::Chunks::ChunkColumn* chunk, bool full, uint32_t mask, ByteBuffer* buf) {
		
		for (int y = 0; y < 16; y++) {
			if ((mask & (1 << y)) != 0) {
				uint8_t bitsPerBlock;
				buf->ReadBEUInt8(bitsPerBlock);

				uint32_t paletteLen;
				buf->ReadVarInt32(paletteLen);

				std::map<int, uint16_t> palette;

				if (paletteLen == 0) {
					//Direct palette
				}
				else {
					//Map
					for (int i = 0; i < paletteLen; i++) {
						uint32_t id;
						buf->ReadVarInt32(id);
						palette.emplace(i, id);
					}
				}

				uint32_t individualValueMask = (uint32_t)((1 << bitsPerBlock) - 1);

				uint32_t dataArrayLen;
				buf->ReadVarInt32(dataArrayLen);

				uint64_t* dataArray = (uint64_t*)malloc(dataArrayLen);
				
				for (int i = 0; i < dataArrayLen / 8; i++) {
					uint64_t longboi;
					buf->ReadBEUInt64(longboi);

					dataArray[i] = longboi;
				}

				Internal::ChunkSection* sect = new Internal::ChunkSection(y);

				for (int blockY = 0; blockY < 16; blockY++) {
					for (int blockZ = 0; blockZ < 16; blockZ++) {
						for (int blockX = 0; blockX < 16; blockX++) {
							int blockNumber = (((blockY * 16) + blockZ) * 16) + blockX;
							int startLong = (blockNumber * bitsPerBlock) / 64;
							int startOffset = (blockNumber * bitsPerBlock) % 64;
							int endLong = ((blockNumber + 1) * bitsPerBlock - 1) / 64;

							uint16_t data;
							if (startLong == endLong) {
								data = (uint16_t)(dataArray[startLong] >> startOffset);
							}
							else {
								int endOffset = 64 - startOffset;
								data = (uint16_t)(dataArray[startLong] >> startOffset | dataArray[endLong] << endOffset);
							}
							data &= individualValueMask;

							uint16_t id;
							if (paletteLen == 0) {
								id = data;
							}
							else {
								id = palette[data];
							}

							//Add into the array
							sect->blocks[(((blockY * 16) + blockZ) * 16) + blockX] = id;
						}
					}
				}


				for (int i = 0; i < 2048; i++) {
					uint8_t b;
					buf->ReadBEUInt8(b);
					sect->blk_light[i] = b;
				}

				for (int i = 0; i < 2048; i++) {
					uint8_t b;
					buf->ReadBEUInt8(b);
					sect->sky_light[i] = b;
				}

				chunk->addSection(sect);
			}
		}

		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				uint8_t biomeData;
				buf->ReadBEUInt8(biomeData);
				chunk->biomes[x][z] = biomeData;
			}
		}
	}
	
	int chunk_data_handler(PacketIn* p) { 
		int32_t x, z;
		p->buffer->ReadBEInt32(x);
		p->buffer->ReadBEInt32(z);

		bool newChunk = false;
		p->buffer->ReadBool(newChunk);
		
		uint32_t mask = 0;
		p->buffer->ReadVarInt32(mask);

		uint32_t size = 0;
		p->buffer->ReadVarInt32(size);

		Internal::Chunks::ChunkColumn* chk;

		if (newChunk) {
			chk = new Internal::Chunks::ChunkColumn(x, z);
		}
		else {
			chk = Internal::g_World->chunkMap[mc::Vector3i(x, z, 0)];
		}

		//Read
		readChunkColumn(chk, newChunk, mask, p->buffer);



		uint32_t blockEntities = 0;
		p->buffer->ReadVarInt32(blockEntities);

		
		for (int i = 0; i < (int)blockEntities; i++) {
			std::cout << "UNSUPPORTED BLOCK ENTITIES!" << std::endl;
		}

		return 0; 
	}
	
	int effect_handler(PacketIn* p) { std::cout << "WARNING EFFECT TRIGGERED" << std::endl; return 0; }
	int particle_handler(PacketIn* p) { std::cout << "WARNING PARTICLE TRIGGERED" << std::endl; return 0; }
	
	int join_game_handler(PacketIn* p) { 
		int eid = 0;
		uint8_t gm = 0;
		int dimension = 0;
		uint8_t difficulty = 0;
		uint8_t max_players = 0;
		std::string levelType = "";
		bool debugInfo;

		p->buffer->ReadBEInt32(eid);
		p->buffer->ReadBEUInt8(gm);
		p->buffer->ReadBEInt32(dimension);
		p->buffer->ReadBEUInt8(difficulty);
		p->buffer->ReadBEUInt8(max_players);
		p->buffer->ReadVarUTF8String(levelType);
		p->buffer->ReadBool(debugInfo);

		std::cout << "EID: " << eid << std::endl;
		std::cout << "GM: " << (int)gm << std::endl;
		std::cout << "DIM: " << dimension << std::endl;
		std::cout << "DIF: " << (int)difficulty << std::endl;
		std::cout << "MAX: " << (int)max_players << std::endl;
		std::cout << "LEV: " << levelType << std::endl;
		std::cout << "DEB: " << debugInfo << std::endl;

		//Default settings
		PacketsOut::send_client_settings("en_us", 2, 0, false, 0b01111111, 1);
		
		return 0; 
	}
	
	int map_data_handler(PacketIn* p) { std::cout << "WARNING MAP_DATA TRIGGERED" << std::endl; return 0; }
	
	int entity_handler(PacketIn* p) { 
		//std::cout << "WARNING ENTITY TRIGGERED" << std::endl; 
		return 0; 
	}
	
	int entity_relative_move_handler(PacketIn* p) { 
		//std::cout << "WARNING ENTITY_RELATIVE_MOVE TRIGGERED" << std::endl; 
		return 0; 
	}
	
	int entity_look_and_relative_move_handler(PacketIn* p) { 
		//std::cout << "WARNING ENTITY_LOOK_AND_RELATIVE_MOVE TRIGGERED" << std::endl; 
		return 0; 
	}

	int entity_look_handler(PacketIn* p) { 
		//std::cout << "WARNING ENTITY_LOOK TRIGGERED" << std::endl; 
		return 0; 
	}
	
	int entity_head_look_handler(PacketIn* p) {
		//std::cout << "WARNING ENTITY_HEAD_LOOK TRIGGERED" << std::endl; 
		return 0;
	}

	int entity_metadata_handler(PacketIn* p) { 
		//std::cout << "WARNING ENTITY_METADATA TRIGGERED" << std::endl; 
		return 0; 
	}
	
	int entity_teleport_handler(PacketIn* p) { 
		//std::cout << "WARNING ENTITY_TELEPORT TRIGGERED" << std::endl; 
		return 0; 
	}
	
	int entity_properties_handler(PacketIn* p) { 
		//std::cout << "WARNING ENTITY_PROPERTIES TRIGGERED" << std::endl; 
		return 0; 
	}
	
	int entity_effect_handler(PacketIn* p) { 
		//std::cout << "WARNING ENTITY_EFFECT TRIGGERED" << std::endl; 
		return 0; 
	}

	int vehicle_move_handler(PacketIn* p) { std::cout << "WARNING VEHICLE_MOVE TRIGGERED" << std::endl; return 0; }
	int open_sign_editor_handler(PacketIn* p) { std::cout << "WARNING OPEN_SIGN_EDITOR TRIGGERED" << std::endl; return 0; }
	int craft_recipe_response_handler(PacketIn* p) { std::cout << "WARNING CRAFT_RECIPE_RESPONSE TRIGGERED" << std::endl; return 0; }
	
	int player_abilities_handler(PacketIn* p) { 

		uint8_t flags;
		float flySpeed;
		float fovMod;

		p->buffer->ReadBEUInt8(flags);
		p->buffer->ReadBEFloat(flySpeed);
		p->buffer->ReadBEFloat(fovMod);

		std::cout << "FLAGS: " << std::hex << flags << std::dec << std::endl;
		std::cout << "FLY SPEED: " << flySpeed << std::endl;
		std::cout << "FOV MOD: " << fovMod << std::endl;
		
		return 0; 
	}
	
	int combat_event_handler(PacketIn* p) { std::cout << "WARNING COMBAT_EVENT TRIGGERED" << std::endl; return 0; }
	int player_info_handler(PacketIn* p) { std::cout << "WARNING PLAYER_INFO TRIGGERED" << std::endl; return 0; }
	int face_player_handler(PacketIn* p) { std::cout << "WARNING FACE_PLAYER TRIGGERED" << std::endl; return 0; }
	
	int player_position_and_look_handler(PacketIn* p) { 

		Internal::g_World->is_loading = false;

		double x, y, z;
		float yaw, pitch;
		uint8_t flags = 0;

		uint32_t id;

		p->buffer->ReadBEDouble(x);
		p->buffer->ReadBEDouble(y);
		p->buffer->ReadBEDouble(z);
		p->buffer->ReadBEFloat(yaw);
		p->buffer->ReadBEFloat(pitch);
		p->buffer->ReadBEUInt8(flags);
		p->buffer->ReadVarInt32(id);

		if ((flags & 0x01) == 0x01) {
			Internal::g_World->player->x += x;
		}
		else {
			Internal::g_World->player->x = x;
		}

		if ((flags & 0x02) == 0x02) {
			Internal::g_World->player->y += y;
		}
		else {
			Internal::g_World->player->y = y;
		}

		if ((flags & 0x04) == 0x04) {
			Internal::g_World->player->z += z;
		}
		else {
			Internal::g_World->player->z = z;
		}

		if ((flags & 0x08) == 0x08) {
			Internal::g_World->player->yaw += yaw;
		}
		else {
			Internal::g_World->player->yaw = yaw;
		}

		if ((flags & 0x10) == 0x10) {
			Internal::g_World->player->pitch += pitch;
		}
		else {
			Internal::g_World->player->pitch = pitch;
		}

		PacketsOut::send_teleport_confirm(id);

		return 0; 
	}
	
	int use_bed_handler(PacketIn* p) { std::cout << "WARNING USE_BED TRIGGERED" << std::endl; return 0; }
	int unlock_recipes_handler(PacketIn* p) { std::cout << "WARNING UNLOCK_RECIPES TRIGGERED" << std::endl; return 0; }
	int destroy_entities_handler(PacketIn* p) { std::cout << "WARNING DESTROY_ENTITIES TRIGGERED" << std::endl; return 0; }
	int remove_entity_effect_handler(PacketIn* p) { std::cout << "WARNING REMOVE_ENTITY_EFFECT TRIGGERED" << std::endl; return 0; }
	int resource_pack_send_handler(PacketIn* p) { std::cout << "WARNING RESOURCE_PACK_SEND TRIGGERED" << std::endl; return 0; }
	int respawn_handler(PacketIn* p) { std::cout << "WARNING RESPAWN TRIGGERED" << std::endl; return 0; }
	
	
	int select_advancement_tab_handler(PacketIn* p) { std::cout << "WARNING SELECT_ADVANCEMENT_TAB TRIGGERED" << std::endl; return 0; }
	int world_border_handler(PacketIn* p) { std::cout << "WARNING WORLD_BORDER TRIGGERED" << std::endl; return 0; }
	int camera_handler(PacketIn* p) { std::cout << "WARNING CAMERA TRIGGERED" << std::endl; return 0; }
	
	int held_item_change_handler(PacketIn* p) { 

		uint8_t slotID;

		p->buffer->ReadBEUInt8(slotID);

		std::cout << "SLOT SET: " << (int)slotID << std::endl;
		
		return 0; 
	}
	
	int display_scoreboard_handler(PacketIn* p) { std::cout << "WARNING DISPLAY_SCOREBOARD TRIGGERED" << std::endl; return 0; }
	int attach_entity_handler(PacketIn* p) { std::cout << "WARNING ATTACH_ENTITY TRIGGERED" << std::endl; return 0; }
	int entity_velocity_handler(PacketIn* p) { std::cout << "WARNING ENTITY_VELOCITY TRIGGERED" << std::endl; return 0; }
	int entity_equipment_handler(PacketIn* p) { std::cout << "WARNING ENTITY_EQUIPMENT TRIGGERED" << std::endl; return 0; }
	int set_experience_handler(PacketIn* p) { std::cout << "WARNING SET_EXPERIENCE TRIGGERED" << std::endl; return 0; }
	int update_health_handler(PacketIn* p) { std::cout << "WARNING UPDATE_HEALTH TRIGGERED" << std::endl; return 0; }
	int scoreboard_objective_handler(PacketIn* p) { std::cout << "WARNING SCOREBOARD_OBJECTIVE TRIGGERED" << std::endl; return 0; }
	int set_passengers_handler(PacketIn* p) { std::cout << "WARNING SET_PASSENGERS TRIGGERED" << std::endl; return 0; }
	int teams_handler(PacketIn* p) { std::cout << "WARNING TEAMS TRIGGERED" << std::endl; return 0; }
	int update_score_handler(PacketIn* p) { std::cout << "WARNING UPDATE_SCORE TRIGGERED" << std::endl; return 0; }
	int spawn_position_handler(PacketIn* p) { std::cout << "WARNING SPAWN_POSITION TRIGGERED" << std::endl; return 0; }
	
	int time_update_handler(PacketIn* p) { 
		//std::cout << "WARNING TIME_UPDATE TRIGGERED" << std::endl;
		p->buffer->ReadBEUInt64(Internal::g_World->timeData.age);
		p->buffer->ReadBEUInt64(Internal::g_World->timeData.timeOfDay);
		return 0; 
	}
	
	int title_handler(PacketIn* p) { std::cout << "WARNING TITLE TRIGGERED" << std::endl; return 0; }
	int stop_sound_handler(PacketIn* p) { std::cout << "WARNING STOP_SOUND TRIGGERED" << std::endl; return 0; }
	int sound_effect_handler(PacketIn* p) { std::cout << "WARNING SOUND_EFFECT TRIGGERED" << std::endl; return 0; }
	int player_list_header_and_footer_handler(PacketIn* p) { std::cout << "WARNING PLAYER_LIST_HEADER_AND_FOOTER TRIGGERED" << std::endl; return 0; }
	int collect_item_handler(PacketIn* p) { std::cout << "WARNING COLLECT_ITEM TRIGGERED" << std::endl; return 0; }
	
	

	int advancements_handler(PacketIn* p) { std::cout << "WARNING ADVANCEMENTS TRIGGERED" << std::endl; return 0; }
	int declare_recipes_handler(PacketIn* p) { std::cout << "WARNING DECLARE_RECIPES TRIGGERED" << std::endl; return 0; }
	int tags_handler(PacketIn* p) { std::cout << "WARNING TAGS TRIGGERED" << std::endl; return 0; }


	void PacketsOut::send_plugin_message(std::string identifier, std::string data)
	{
		PacketOut* p = new PacketOut(64);
		p->ID = 0x0A;
		p->buffer->WriteVarUTF8String(identifier);
		p->buffer->WriteVarUTF8String(data);

		g_NetworkDriver.AddPacket(p);
		g_NetworkDriver.SendPackets(p);
	}
	void PacketsOut::send_client_settings(std::string locale, uint8_t distance, uint8_t c_mode, bool colors, uint8_t bitmask, uint8_t hand)
	{
		PacketOut* p = new PacketOut(64);
		p->ID = 0x04;
		p->buffer->WriteVarUTF8String(locale);
		p->buffer->WriteBEUInt8(distance);
		p->buffer->WriteBEUInt8(c_mode);
		p->buffer->WriteBool(colors);
		p->buffer->WriteBEUInt8(bitmask);
		p->buffer->WriteBEUInt8(hand);

		g_NetworkDriver.AddPacket(p);
		g_NetworkDriver.SendPackets(p);
	}
	void PacketsOut::send_keep_alive(uint64_t longData)
	{
		PacketOut* p = new PacketOut(12);
		p->ID = 0x0E;
		p->buffer->WriteBEUInt64(longData);

		g_NetworkDriver.AddPacket(p);
		g_NetworkDriver.SendPackets(p);
	}
	void PacketsOut::send_teleport_confirm(uint32_t id)
	{
		PacketOut* p = new PacketOut(12);
		p->ID = 0x00;
		p->buffer->WriteVarInt32(id);

		g_NetworkDriver.AddPacket(p);
		g_NetworkDriver.SendPackets(p);
	}
	void PacketsOut::send_player()
	{
		PacketOut* p = new PacketOut(8);
		p->ID = 0x0F;
		p->buffer->WriteBool(Internal::g_World->player->onGround);

		g_NetworkDriver.AddPacket(p);
		g_NetworkDriver.SendPackets(p);
	}
	void PacketsOut::send_player_position()
	{
		PacketOut* p = new PacketOut(64);
		p->ID = 0x10;
		p->buffer->WriteBEDouble(Internal::g_World->player->x);
		p->buffer->WriteBEDouble(Internal::g_World->player->y);
		p->buffer->WriteBEDouble(Internal::g_World->player->z);
		p->buffer->WriteBool(Internal::g_World->player->onGround);

		g_NetworkDriver.AddPacket(p);
		g_NetworkDriver.SendPackets(p);
	}
	void PacketsOut::send_player_position_look()
	{
		PacketOut* p = new PacketOut(64);
		p->ID = 0x11;
		p->buffer->WriteBEDouble(Internal::g_World->player->x);
		p->buffer->WriteBEDouble(Internal::g_World->player->y);
		p->buffer->WriteBEDouble(Internal::g_World->player->z);
		p->buffer->WriteBEFloat(Internal::g_World->player->yaw);
		p->buffer->WriteBEFloat(Internal::g_World->player->pitch);
		p->buffer->WriteBool(Internal::g_World->player->onGround);

		g_NetworkDriver.AddPacket(p);
		g_NetworkDriver.SendPackets(p);
	}
	void PacketsOut::send_player_look()
	{
		PacketOut* p = new PacketOut(64);
		p->ID = 0x12;
		p->buffer->WriteBEFloat(Internal::g_World->player->yaw);
		p->buffer->WriteBEFloat(Internal::g_World->player->pitch);
		p->buffer->WriteBool(Internal::g_World->player->onGround);

		g_NetworkDriver.AddPacket(p);
		g_NetworkDriver.SendPackets(p);
	}
}

