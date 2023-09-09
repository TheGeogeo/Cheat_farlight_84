#include "function.h"
#include "overlay.h"
#include "driver.h"
#include "time.h"
#include "vector.h"

namespace OverlayWindow
{
	WNDCLASSEX WindowClass;
	HWND Hwnd;
	LPCSTR Name;
}

void PrintPtr(std::string text, uintptr_t ptr) {
	std::cout << text << ptr << std::endl;
}

enum bones
{
	Root = 0,
	Bip001 = 1,
	pelvis = 2,
	spine_01 = 3,
	spine_02 = 4,
	spine_03 = 5,
	clavicle_l = 6,
	upperarm_l = 7,
	lowerarm_l = 8,
	hand_l = 9,
	thumb_01_l = 10,
	thumb_02_l = 11,
	thumb_03_l = 12,
	index_01_l = 13,
	index_02_l = 14,
	index_03_l = 15,
	middle_01_l = 16,
	middle_02_l = 17,
	middle_03_l = 18,
	ring_01_l = 19,
	ring_02_l = 20,
	ring_03_l = 21,
	pinky_01_l = 22,
	pinky_02_l = 23,
	pinky_03_l = 24,
	clavicle_r = 25,
	upperarm_r = 26,
	lowerarm_r = 27,
	hand_r = 28,
	thumb_01_r = 29,
	thumb_02_r = 30,
	thumb_03_r = 31,
	index_01_r = 32,
	index_02_r = 33,
	index_03_r = 34,
	middle_01_r = 35,
	middle_02_r = 36,
	middle_03_r = 37,
	ring_01_r = 38,
	ring_02_r = 39,
	ring_03_r = 40,
	pinky_01_r = 41,
	pinky_02_r = 42,
	pinky_03_r = 43,
	RightHandWeaponAttach = 44,
	neck_01 = 45,
	head = 46,
	BackpackAttach = 47,
	thigh_l = 48,
	calf_l = 49,
	foot_l = 50,
	ball_l = 51,
	thigh_r = 52,
	calf_r = 53,
	foot_r = 54,
	ball_r = 55,
	VB_spine_03_RightHandWeaponAttach = 56,
	VB_VB_spine_03_RightHandWeaponAttach_hand_r = 57,
	VB_VB_VB_spine_03_RightHandWeaponAttach_hand_r_lowerarm_r = 58,
};

struct FVector
{
	float X; // 0x00[0x04]
	float Y; // 0x04[0x04]
	float Z; // 0x08[0x04]
	FVector() : X(0.0f), Y(0.0f), Z(0.0f) { }
};

namespace DirectX9Interface
{
	IDirect3D9Ex* Direct3D9 = NULL;
	IDirect3DDevice9Ex* pDevice = NULL;
	D3DPRESENT_PARAMETERS pParams = { NULL };
	MARGINS Margin = { -1 };
	MSG Message = { NULL };
}
typedef struct _EntityList
{
	uintptr_t actor_pawn;
	uintptr_t actor_mesh;
	uintptr_t actor_state;
	int actor_id;
}EntityList;
std::vector<EntityList> entityList;
std::vector<EntityList> itemEntityList;

auto CallHacks() -> VOID
{
	while (true)
	{
		auto CachedCurrentWeapon = read<uint64_t>(GameVars.local_player_pawn + GameOffset.offset_CachedCurrentWeapon); // struct ASolarPlayerWeapon* CachedCurrentWeapon; // 0x1a18(0x08)
		auto RecoilComponent = read<uint64_t>(CachedCurrentWeapon + GameOffset.offset_RecoilComponent); // struct UWeaponRecoilComponent* RecoilComponent; // 0x598(0x08)
		auto USingleWeaponConfig = read<uint64_t>(CachedCurrentWeapon + GameOffset.offset_USingleWeaponConfig); // 	struct USingleWeaponConfig* ; //
		auto UWeaponShootConfig = read<uint64_t>(USingleWeaponConfig + GameOffset.offset_UWeaponShootConfig); // 	struct UWeaponShootConfig
		auto WeaponConfig = read<uint64_t>(RecoilComponent + GameOffset.offset_WeaponConfig); //possibly WeaponFireModeType;   WeaponAbilityTag; or 	struct FGameplayTag WeaponAbilityTag; // 0xd8(0x08)
		auto primaryammo = read<uint64_t>(USingleWeaponConfig + GameOffset.offset_primaryammo); // struct UAmmoConfig* PrimaryAmmo; // 0x2d8(0x08)
		auto secondaryammo = read<uint64_t>(USingleWeaponConfig + GameOffset.offset_secondaryammo); // struct UAmmoConfig* PrimaryAmmo; // 0x2d8(0x08)
		auto secondaryupdateammo = read<uint64_t>(USingleWeaponConfig + GameOffset.offset_secondaryupdateammo); // struct UAmmoConfig* PrimaryAmmo; // 0x2d8(0x08)
		auto FAmmonRecoilScope = read<uint64_t>(primaryammo + GameOffset.offset_FAmmonRecoilScope); // struct UAmmoConfig* PrimaryAmmo; // 0x2d8(0x08)
		auto FAmmonRecoilRollStruct = read<uint64_t>(primaryammo + GameOffset.offset_FAmmonRecoilRollStruct); // struct UAmmoConfig* PrimaryAmmo; // 0x2d8(0x08)
		auto FAmmonRecoilScope2 = read<uint64_t>(secondaryammo + GameOffset.offset_FAmmonRecoilScope2); // struct UAmmoConfig* PrimaryAmmo; // 0x2d8(0x08)
		auto FAmmonRecoilRollStruct2 = read<uint64_t>(secondaryammo + GameOffset.offset_FAmmonRecoilRollStruct2); // struct UAmmoConfig* PrimaryAmmo; // 0x2d8(0x08)
		auto FAmmonRecoilScope3 = read<uint64_t>(secondaryupdateammo + GameOffset.offset_FAmmonRecoilScope3); // struct UAmmoConfig* PrimaryAmmo; // 0x2d8(0x08)
		auto FAmmonRecoilRollStruct3 = read<uint64_t>(secondaryupdateammo + GameOffset.offset_FAmmonRecoilRollStruct3); // struct UAmmoConfig* PrimaryAmmo; // 0x2d8(0x08) // HorizontalCurve
		if (CFG.b_NoRecoil)
		{
			write<bool>(primaryammo + 0x03E0 + 0x0, false); //FAmmonRecoilScope EnableScopeVibration
			write<bool>(primaryammo + 0x03E0 + 0x1, false); //FAmmonRecoilScope EnableCrossHairVibration
			write<bool>(primaryammo + 0x03E0 + 0x2, false); //FAmmonRecoilScope EnableScopeRoll
			write<float>(primaryammo + 0x498, 0.0f); // float ADSRecoilCOP; // 0x0498   (0x0004)
			write<float>(primaryammo + 0x03E0, 0.0f);  // float  ADSSpreadCOP;   // 0x0224   (0x0004)

			write<float>(primaryammo + 0x0224, 0.0f);  // float  ADSSpreadCOP;   // 0x0224   (0x0004)

			write<bool>(secondaryammo + 0x03E0 + 0x0, false); // EnableScopeVibration
			write<bool>(secondaryammo + 0x03E0 + 0x1, false); // EnableCrossHairVibration
			write<bool>(secondaryammo + 0x03E0 + 0x2, false); // EnableScopeRoll
			write<bool>(secondaryammo + 0x0224, false); // float  ADSSpreadCOP;   // 0x0224   (0x0004)
			write<float>(secondaryammo + 0x0498, 0.0f); //float ADSRecoilCOP; // 0x0498   (0x0004)
			write<bool>(RecoilComponent + 0x00C0, false); //UWeaponRecoilComponent
			write<bool>(RecoilComponent + 0x00C1, false); //UWeaponRecoilComponent
			write<bool>(RecoilComponent + 0x00C2, false); //UWeaponRecoilComponent
			write<bool>(UWeaponShootConfig + 0x0031, false); //bEnableNewRecoil;  // 0x0033   (0x0001)
		}
		if (CFG.b_NoSpread)
		{
			write<float>(WeaponConfig + 0x00E4, -9.9999f); //float FastestFireInterval; // 0xf8(0x04)
			write<float>(WeaponConfig + 0x00EC, -9.9999f); //float BaseFireInterval; // 0xf8(0x04)
			write<float>(WeaponConfig + 0x00F0, -9.9999f); //float FireIntervalRevertPreTime; // 0xf8(0x04)
			write<float>(WeaponConfig + 0x00F4, -9.9999f); //float FireIntervalReavertSpeed; // 0xf8(0x04)
			write<float>(WeaponConfig + 0x00F8, -9.9999f); //float BoltActionTime; // 0xf8(0x04)

			write<float>(USingleWeaponConfig + 0x0108, -9.9999f); //float MaxSpread; // 0xf8(0x04)
			write<float>(USingleWeaponConfig + 0x010C, -9.9999f); //float MinSpread; // 0xf8(0x04)
			write<float>(USingleWeaponConfig + 0x0110, -9.9999f); //float HipFireBaseSpread; // 0xf8(0x04)
			write<float>(USingleWeaponConfig + 0x0114, -9.9999f); //float ShoulderFireBaseSpread; // 0xf8(0x04)
			write<float>(USingleWeaponConfig + 0x0118, -9.9999f); //float ADSBaseSpread; // 0xf8(0x04)
			write<float>(USingleWeaponConfig + 0x011C, -9.9999f); //float VhADSBaseSpread; // 0xf8(0x04)

			write<bool>(UWeaponShootConfig + 0x0030, false); //bEnableNewSpread;  // 0x0033   (0x0001)
			write<bool>(UWeaponShootConfig + 0x0032, false); //bEnableNewWeaponAnim;  // 0x0033   (0x0001)
			write<bool>(UWeaponShootConfig + 0x0033, false); //bEnableNewCameraShake;  // 0x0033   (0x0001)
			write<float>(UWeaponShootConfig + 0x0098, -9.9999f); //float BaseSpread; // 0xf8(0x04)
		}
		if (CFG.b_FastReload)
		{
			write<float>(primaryammo + 0x0118, 1.650f);// ChargeTime
		}
		if (CFG.b_FastScope)
		{
			write<float>(USingleWeaponConfig + 0x0494, -100.0f);// ChargeTime
			write<float>(USingleWeaponConfig + 0x0498, -100.0f);// ChargeTime
		}
	}
}

auto CallAimbot() -> VOID
{
	while (true)
	{
		auto EntityList_Copy = entityList;

		bool isAimbotActive = CFG.b_Aimbot && GetAimKey();
		if (isAimbotActive)
		{
			float target_dist = FLT_MAX;
			EntityList target_entity = {};

			for (int index = 0; index < EntityList_Copy.size(); ++index)
			{
				auto Entity = EntityList_Copy[index];
				auto Health = read<float>(Entity.actor_state + GameOffset.offset_Hp);
				auto MaxHealth = read<float>(Entity.actor_state + GameOffset.offset_HpMax);
				auto procentage = Health * 100 / MaxHealth;

				auto PlayerName = read<FString>(Entity.actor_state + GameOffset.offset_player_name);
				auto BotAI = read<BYTE>(Entity.actor_state + GameOffset.offset_bot);
				auto SolarTeamIfo = read<DWORD_PTR>(Entity.actor_state + GameOffset.offset_ASolarTeamInfotTeam);
				auto TeamId = read<BYTE>(SolarTeamIfo + GameOffset.offset_ASolarTeamInfotTeamID);
				auto MyTeamInfo = read<DWORD_PTR>(GameVars.local_player_state + GameOffset.offset_ASolarTeamInfotTeam);
				auto MyTeamID = read<BYTE>(MyTeamInfo + GameOffset.offset_ASolarTeamInfotTeamID);

				if (!Entity.actor_mesh || procentage == 0)
					continue;

				if (SolarTeamIfo == MyTeamInfo)
					continue;

				Vector3 aimingLocationPos = Vector3();
				Vector3 head_pos = GetBoneWithRotation(Entity.actor_mesh, bones::head);
				Vector3 neck_pos = GetBoneWithRotation(Entity.actor_mesh, bones::neck_01);
				Vector3 body_pos = GetBoneWithRotation(Entity.actor_mesh, bones::spine_03);
				Vector3 pelvis_pos = GetBoneWithRotation(Entity.actor_mesh, bones::pelvis);
				Vector3 thighr_pos = GetBoneWithRotation(Entity.actor_mesh, bones::thigh_r);
				Vector3 thighl_pos = GetBoneWithRotation(Entity.actor_mesh, bones::thigh_l);
				Vector3 calfr_pos = GetBoneWithRotation(Entity.actor_mesh, bones::calf_r);
				Vector3 calfl_pos = GetBoneWithRotation(Entity.actor_mesh, bones::calf_l);
				Vector3 upperarmr_pos = GetBoneWithRotation(Entity.actor_mesh, bones::upperarm_r);
				Vector3 upperarml_pos = GetBoneWithRotation(Entity.actor_mesh, bones::upperarm_l);
				Vector3 lowerarmr_pos = GetBoneWithRotation(Entity.actor_mesh, bones::lowerarm_r);
				Vector3 lowerarml_pos = GetBoneWithRotation(Entity.actor_mesh, bones::lowerarm_l);
				Vector3 handr_pos = GetBoneWithRotation(Entity.actor_mesh, bones::hand_r);
				Vector3 handl_pos = GetBoneWithRotation(Entity.actor_mesh, bones::hand_l);
				Vector3 footr_pos = GetBoneWithRotation(Entity.actor_mesh, bones::foot_r);
				Vector3 footl_pos = GetBoneWithRotation(Entity.actor_mesh, bones::foot_l);

				switch (CFG.aimLocation)
				{
				case 0: // Head
					aimingLocationPos = ProjectWorldToScreen(Vector3(head_pos.x, head_pos.y, head_pos.z));
					break;

				case 1: // Neck
					aimingLocationPos = ProjectWorldToScreen(Vector3(neck_pos.x, neck_pos.y, neck_pos.z));
					break;

				case 2: // Body
					aimingLocationPos = ProjectWorldToScreen(Vector3(body_pos.x, body_pos.y, body_pos.z));
					break;

				case 3: // Pelvis
					aimingLocationPos = ProjectWorldToScreen(Vector3(pelvis_pos.x, pelvis_pos.y, pelvis_pos.z));
					break;

				case 4: // thigh r
					aimingLocationPos = ProjectWorldToScreen(Vector3(thighr_pos.x, thighr_pos.y, thighr_pos.z));
					break;

				case 5: // thigh l
					aimingLocationPos = ProjectWorldToScreen(Vector3(thighl_pos.x, thighl_pos.y, thighl_pos.z));
					break;

				case 6: // calf r
					aimingLocationPos = ProjectWorldToScreen(Vector3(calfr_pos.x, calfr_pos.y, calfr_pos.z));
					break;

				case 7: // calf l
					aimingLocationPos = ProjectWorldToScreen(Vector3(calfl_pos.x, calfl_pos.y, calfl_pos.z));
					break;

				case 8: // upperarm r
					aimingLocationPos = ProjectWorldToScreen(Vector3(upperarmr_pos.x, upperarmr_pos.y, upperarmr_pos.z));
					break;

				case 9: // upperarm l
					aimingLocationPos = ProjectWorldToScreen(Vector3(upperarml_pos.x, upperarml_pos.y, upperarml_pos.z));
					break;

				case 10: // Lowerarm r
					aimingLocationPos = ProjectWorldToScreen(Vector3(lowerarmr_pos.x, lowerarmr_pos.y, lowerarmr_pos.z));
					break;

				case 11: // Lowerarm l
					aimingLocationPos = ProjectWorldToScreen(Vector3(lowerarml_pos.x, lowerarml_pos.y, lowerarml_pos.z));
					break;

				case 12: // hand r
					aimingLocationPos = ProjectWorldToScreen(Vector3(handr_pos.x, handr_pos.y, handr_pos.z));
					break;

				case 13: // hand l
					aimingLocationPos = ProjectWorldToScreen(Vector3(handl_pos.x, handl_pos.y, handl_pos.z));
					break;

				case 14: // Foot r
					aimingLocationPos = ProjectWorldToScreen(Vector3(footr_pos.x, footr_pos.y, footr_pos.z));
					break;

				case 15: // Foot l
					aimingLocationPos = ProjectWorldToScreen(Vector3(footl_pos.x, footl_pos.y, footl_pos.z));
					break;

				default:
					aimingLocationPos = ProjectWorldToScreen(Vector3(neck_pos.x, neck_pos.y, neck_pos.z));
					break;
				}

				float x = aimingLocationPos.x - GameVars.ScreenWidth / 2.0f;
				float y = aimingLocationPos.y - GameVars.ScreenHeight / 2.0f;
				float crosshair_dist = sqrtf((x * x) + (y * y));

				if (crosshair_dist <= FLT_MAX && crosshair_dist <= target_dist)
				{
					if (crosshair_dist > CFG.AimbotFOV) // FOV
						continue;

					target_dist = crosshair_dist;
					target_entity = Entity;
				}
			}

			if (target_entity.actor_mesh != 0 || target_entity.actor_pawn != 0 || target_entity.actor_id != 0)
			{
				if (target_entity.actor_pawn == GameVars.local_player_pawn)
					continue;

				Vector3 aimingLocationPos = Vector3();
				Vector3 head_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::head);
				Vector3 neck_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::neck_01);
				Vector3 body_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::spine_03);
				Vector3 pelvis_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::pelvis);
				Vector3 thighr_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::thigh_r);
				Vector3 thighl_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::thigh_l);
				Vector3 calfr_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::calf_r);
				Vector3 calfl_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::calf_l);
				Vector3 upperarmr_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::upperarm_r);
				Vector3 upperarml_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::upperarm_l);
				Vector3 lowerarmr_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::lowerarm_r);
				Vector3 lowerarml_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::lowerarm_l);
				Vector3 handr_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::hand_r);
				Vector3 handl_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::hand_l);
				Vector3 footr_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::foot_r);
				Vector3 footl_pos = GetBoneWithRotation(target_entity.actor_mesh, bones::foot_l);

				switch (CFG.aimLocation)
				{
				case 0: // Head
					aimingLocationPos = ProjectWorldToScreen(Vector3(head_pos.x, head_pos.y, head_pos.z));
					break;

				case 1: // Neck
					aimingLocationPos = ProjectWorldToScreen(Vector3(neck_pos.x, neck_pos.y, neck_pos.z));
					break;

				case 2: // Body
					aimingLocationPos = ProjectWorldToScreen(Vector3(body_pos.x, body_pos.y, body_pos.z));
					break;

				case 3: // Pelvis
					aimingLocationPos = ProjectWorldToScreen(Vector3(pelvis_pos.x, pelvis_pos.y, pelvis_pos.z));
					break;

				case 4: // thigh r
					aimingLocationPos = ProjectWorldToScreen(Vector3(thighr_pos.x, thighr_pos.y, thighr_pos.z));
					break;

				case 5: // thigh l
					aimingLocationPos = ProjectWorldToScreen(Vector3(thighl_pos.x, thighl_pos.y, thighl_pos.z));
					break;

				case 6: // calf r
					aimingLocationPos = ProjectWorldToScreen(Vector3(calfr_pos.x, calfr_pos.y, calfr_pos.z));
					break;

				case 7: // calf l
					aimingLocationPos = ProjectWorldToScreen(Vector3(calfl_pos.x, calfl_pos.y, calfl_pos.z));
					break;

				case 8: // upperarm r
					aimingLocationPos = ProjectWorldToScreen(Vector3(upperarmr_pos.x, upperarmr_pos.y, upperarmr_pos.z));
					break;

				case 9: // upperarm l
					aimingLocationPos = ProjectWorldToScreen(Vector3(upperarml_pos.x, upperarml_pos.y, upperarml_pos.z));
					break;

				case 10: // Lowerarm r
					aimingLocationPos = ProjectWorldToScreen(Vector3(lowerarmr_pos.x, lowerarmr_pos.y, lowerarmr_pos.z));
					break;

				case 11: // Lowerarm l
					aimingLocationPos = ProjectWorldToScreen(Vector3(lowerarml_pos.x, lowerarml_pos.y, lowerarml_pos.z));
					break;

				case 12: // hand r
					aimingLocationPos = ProjectWorldToScreen(Vector3(handr_pos.x, handr_pos.y, handr_pos.z));
					break;

				case 13: // hand l
					aimingLocationPos = ProjectWorldToScreen(Vector3(handl_pos.x, handl_pos.y, handl_pos.z));
					break;

				case 14: // Foot r
					aimingLocationPos = ProjectWorldToScreen(Vector3(footr_pos.x, footr_pos.y, footr_pos.z));
					break;

				case 15: // Foot l
					aimingLocationPos = ProjectWorldToScreen(Vector3(footl_pos.x, footl_pos.y, footl_pos.z));
					break;

				default:
					aimingLocationPos = ProjectWorldToScreen(Vector3(neck_pos.x, neck_pos.y, neck_pos.z));
					break;
				}

				move_to(aimingLocationPos.x, aimingLocationPos.y);
			}
		}
		GetCpuUsageAimbot();
	}
}
auto GameCache() -> VOID
{
	while (true)
	{
		std::vector<EntityList> tmpList;
		std::vector<EntityList> tmpItemList;

		GameVars.u_world = read<DWORD_PTR>(GameVars.dwProcess_Base + GameOffset.offset_u_world);
		GameVars.game_instance = read<DWORD_PTR>(GameVars.u_world + GameOffset.offset_game_instance);
		GameVars.local_player_array = read<DWORD_PTR>(GameVars.game_instance + GameOffset.offset_local_players_array);
		GameVars.local_player = read<DWORD_PTR>(GameVars.local_player_array);
		GameVars.local_player_controller = read<DWORD_PTR>(GameVars.local_player + GameOffset.offset_player_controller);
		GameVars.local_player_pawn = read<DWORD_PTR>(GameVars.local_player_controller + GameOffset.offset_apawn);
		GameVars.local_player_root = read<DWORD_PTR>(GameVars.local_player_pawn + GameOffset.offset_root_component);
		GameVars.local_player_state = read<DWORD_PTR>(GameVars.local_player_pawn + GameOffset.offset_player_state);
		GameVars.ranged_weapon_component = read<DWORD_PTR>(GameVars.local_player_pawn + GameOffset.offset_ranged_weapon_component);
		GameVars.equipped_weapon_type = read<DWORD_PTR>(GameVars.ranged_weapon_component + GameOffset.offset_equipped_weapon_type);
		GameVars.persistent_level = read<DWORD_PTR>(GameVars.u_world + GameOffset.offset_persistent_level);
		GameVars.actors = read<DWORD_PTR>(GameVars.persistent_level + GameOffset.offset_actor_array);
		GameVars.actor_count = read<int>(GameVars.persistent_level + GameOffset.offset_actor_count);
		/*
		PrintPtr("uworld ", GameVars.u_world);
		PrintPtr("game instance ", GameVars.game_instance);
		PrintPtr("L Player Array ", GameVars.local_player_array);
		PrintPtr("L Player ", GameVars.local_player);
		PrintPtr("L Player Controller ", GameVars.local_player_controller);
		PrintPtr("L Player Pawn ", GameVars.local_player_pawn);
		PrintPtr("L Player Root ", GameVars.local_player_root);
		PrintPtr("L Player State ", GameVars.local_player_state);
		PrintPtr("P Level ", GameVars.persistent_level);
		PrintPtr("Actors ", GameVars.actors);
		PrintPtr("Actor Count ", GameVars.actor_count);
		*/
		for (int index = 0; index < GameVars.actor_count; ++index)
		{
			auto actor_pawn = read<uintptr_t>(GameVars.actors + index * 0x8);
			if (actor_pawn == 0x00)
				continue;

			auto actor_id = read<int>(actor_pawn + GameOffset.offset_actor_id);
			auto actor_mesh = read<uintptr_t>(actor_pawn + GameOffset.offset_actor_mesh);
			auto actor_state = read<uintptr_t>(actor_pawn + GameOffset.offset_player_state);
			string name = GetNameFromFName(actor_id);

			//printf("\n: %s", name.c_str());

			if (name.find("SolarItemActor") != std::string::npos || name.find("SolarGroundPreviewActor") != std::string::npos)
			{
				if (actor_pawn != NULL || actor_id != NULL || actor_state != NULL || actor_mesh != NULL)
				{
					EntityList Entity{ };
					Entity.actor_pawn = actor_pawn;
					Entity.actor_id = actor_id;
					Entity.actor_state = actor_state;
					Entity.actor_mesh = actor_mesh;
					tmpItemList.push_back(Entity);
				}
			}

			if (name.find("BP_Character") != std::string::npos)
			{
				if (actor_pawn != NULL || actor_id != NULL || actor_state != NULL || actor_mesh != NULL)
				{
					EntityList Entity{ };
					Entity.actor_pawn = actor_pawn;
					Entity.actor_id = actor_id;
					Entity.actor_state = actor_state;
					Entity.actor_mesh = actor_mesh;
					tmpList.push_back(Entity);
				}
			}
		}
		entityList = tmpList;
		itemEntityList = tmpItemList;
	}
	Sleep(40);
}
auto RenderVisual() -> VOID
{
	auto EntityList_Copy = entityList;
	auto itemEntityList_Copy = itemEntityList;

	if (CFG.b_Visual)
	{
		for (int index = 0; index < itemEntityList_Copy.size(); ++index)
		{
			auto Entity = itemEntityList_Copy[index];

			auto local_pos = read<Vector3>(GameVars.local_player_root + GameOffset.offset_relative_location);
			auto pawn_root = read<DWORD_PTR>(Entity.actor_pawn + GameOffset.offset_root_component);
			auto pawn_location = read<Vector3>(pawn_root + GameOffset.offset_relative_location);
			auto entity_distance = local_pos.Distance(pawn_location);

			if (entity_distance < CFG.max_distance)
			{
				if (CFG.b_EspItem)
				{
					auto itemID = read<uint32_t>((uintptr_t)(Entity.actor_pawn) + GameOffset.offset_ItemData + GameOffset.offset_ItemID);
					auto itemName = read<FString>((uintptr_t)(Entity.actor_pawn) + GameOffset.offset_ItemData + GameOffset.offset_ItemName);
					auto itemQuality = read<uint32_t>((uintptr_t)(Entity.actor_pawn) + GameOffset.offset_ItemData + GameOffset.offset_ItemQuality);
					string itemNameStr = itemName.ToString();

					auto itemPosScreen = ProjectWorldToScreen(Vector3(pawn_location.x, pawn_location.y, pawn_location.z));

					ImColor colorQuality = ImColor(255, 255, 255);
					switch (itemQuality)
					{
					case 1:
						colorQuality = ImColor(255, 255, 255);
						break;
					case 2:
						colorQuality = ImColor(0, 168, 6);
						break;
					case 3:
						colorQuality = ImColor(32, 41, 201);
						break;
					case 4:
						colorQuality = ImColor(201, 0, 212);
						break;
					case 5:
						colorQuality = ImColor(222, 122, 0);
						break;
					case 6:
						colorQuality = ImColor(200, 0, 0);
						break;
					default:
						break;
					}

					string strToDrawItem = "";

					if (IsWeaponAR(itemID))
					{
						strToDrawItem += "[AR] " + itemNameStr;
					}
					else if (IsWeaponSMG(itemID))
					{
						strToDrawItem += "[SMG] " + itemNameStr;
					}
					else if (IsWeaponSNP(itemID))
					{
						strToDrawItem += "[SNP] " + itemNameStr;
					}
					else if (IsWeaponSpecial(itemID))
					{
						strToDrawItem += "[SP] " + itemNameStr;
					}
					else if (IsWeaponSHG(itemID))
					{
						strToDrawItem += "[SHG] " + itemNameStr;
					}
					else
					{
						strToDrawItem += itemNameStr;
					}

					DrawOutlinedText(Verdana, strToDrawItem, ImVec2(itemPosScreen.x, itemPosScreen.y), 16.0f, colorQuality, true);
				}
			}
		}
	}

	if (CFG.b_Visual)
	{
		for (int index = 0; index < EntityList_Copy.size(); ++index)
		{
			auto Entity = EntityList_Copy[index];

			if (Entity.actor_pawn == GameVars.local_player_pawn)
				continue;

			if (!Entity.actor_mesh || !Entity.actor_state || !Entity.actor_pawn)
				continue;

			float Health = read<float>(Entity.actor_state + GameOffset.offset_Hp);
			float MaxHealth = read<float>(Entity.actor_state + GameOffset.offset_HpMax);
			if (MaxHealth == 0) {  // Guard against division by zero.
				MaxHealth = 1.0f;
			}
			float shield = read<float>(Entity.actor_state + GameOffset.offset_ShieldInfo + GameOffset.offset_Shield);
			float maxShield = 120.0f;
			float procentageHp = Health * 100 / MaxHealth;
			float procentageShield = shield * 100 / maxShield;

			auto PlayerName = read<FString>(Entity.actor_state + GameOffset.offset_player_name);
			auto BotAI = read<BYTE>(Entity.actor_state + GameOffset.offset_bot);
			auto SolarTeamIfo = read<DWORD_PTR>(Entity.actor_state + GameOffset.offset_ASolarTeamInfotTeam);
			auto TeamId = read<BYTE>(SolarTeamIfo + GameOffset.offset_ASolarTeamInfotTeamID);
			auto MyTeamInfo = read<DWORD_PTR>(GameVars.local_player_state + GameOffset.offset_ASolarTeamInfotTeam);
			auto MyTeamID = read<BYTE>(MyTeamInfo + GameOffset.offset_ASolarTeamInfotTeamID);

			if (procentageHp == 0)
				continue;

			if (SolarTeamIfo == MyTeamInfo && !CFG.b_EspTeam)
				continue;

			auto local_pos = read<Vector3>(GameVars.local_player_root + GameOffset.offset_relative_location);
			auto head_pos = GetBoneWithRotation(Entity.actor_mesh, bones::head);
			auto bone_pos = GetBoneWithRotation(Entity.actor_mesh, 0);
			auto BottomBox = ProjectWorldToScreen(bone_pos);
			auto TopBox = ProjectWorldToScreen(Vector3(head_pos.x, head_pos.y, head_pos.z + 15));

			if (BottomBox.y == TopBox.y) {  // Guard against undefined behavior.
				BottomBox.y += 1.0f;
			}

			float CornerHeight = abs(TopBox.y - BottomBox.y);
			float CornerWidth = CornerHeight * 0.65f;

			float width = CornerWidth / 10;
			if (width < 2.f) width = 2.f;
			if (width > 3) width = 3.f;

			auto entity_distance = local_pos.Distance(bone_pos);

			if (entity_distance < CFG.max_distance)
			{
				if (CFG.b_EspBox)
				{
					if (CFG.BoxType == 0)
					{
						DrawBox(TopBox.x - (CornerWidth / 2), TopBox.y, CornerWidth, CornerHeight, CFG.VisibleColor);
					}
					else if (CFG.BoxType == 1)
					{
						DrawCorneredBox(TopBox.x - (CornerWidth / 2), TopBox.y, CornerWidth, CornerHeight, CFG.VisibleColor, 1.5);
					}
				}
				if (CFG.b_EspLine)
				{
					if (CFG.LineType == 0)
					{
						DrawLine(ImVec2(static_cast<float>(GameVars.ScreenWidth / 2), static_cast<float>(GameVars.ScreenHeight)), ImVec2(BottomBox.x, BottomBox.y), CFG.VisibleColor, 1.5f); //LINE FROM BOTTOM SCREEN
					}
					if (CFG.LineType == 1)
					{
						DrawLine(ImVec2(static_cast<float>(GameVars.ScreenWidth / 2), 0.f), ImVec2(BottomBox.x, BottomBox.y), CFG.VisibleColor, 1.5f); //LINE FROM TOP SCREEN
					}
					if (CFG.LineType == 2)
					{
						DrawLine(ImVec2(static_cast<float>(GameVars.ScreenWidth / 2), static_cast<float>(GameVars.ScreenHeight / 2)), ImVec2(BottomBox.x, BottomBox.y), CFG.VisibleColor, 1.5f); //LINE FROM CROSSHAIR
					}
				}
				if (CFG.b_EspDistance)
				{
					char dist[64];
					sprintf_s(dist, "Distance :%.fm", entity_distance);
					DrawOutlinedText(Verdana, dist, ImVec2(BottomBox.x, BottomBox.y), 14.0f, ImColor(255, 255, 255), true);
				}
				if (CFG.b_EspName)
				{
					if (BotAI == 2) {
						DrawOutlinedText(Verdana, "BOT", ImVec2(TopBox.x, TopBox.y - 20), 16.0f, ImColor(255, 255, 255), true);
					}
					else
					{
						DrawOutlinedText(Verdana, PlayerName.ToString(), ImVec2(TopBox.x, TopBox.y - 20), 16.0f, ImColor(255, 255, 255), true);
					}
				}
				if (CFG.b_EspHealth)
				{
					HealthBar(TopBox.x - (CornerWidth / 2) - 8, TopBox.y, width, BottomBox.y - TopBox.y, procentageHp, true);
				}
				if (CFG.b_EspShield)
				{
					ShieldBar(TopBox.x - (CornerWidth / 2) - 12, TopBox.y, width, BottomBox.y - TopBox.y, procentageShield, true);
				}
				if (CFG.b_EspSkeleton)
				{
					Vector3 vHeadOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::head));
					Vector3 vNeckOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::neck_01));
					Vector3 vSpine1Out = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::spine_01));
					Vector3 vSpine2Out = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::spine_02));
					Vector3 vSpine3Out = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::spine_03));
					Vector3 vPelvisOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::pelvis));
					Vector3 vRightThighOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::thigh_r));
					Vector3 vLeftThighOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::thigh_l));
					Vector3 vRightCalfOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::calf_r));
					Vector3 vLeftCalfOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::calf_l));
					Vector3 vRightFootOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::foot_r));
					Vector3 vLeftFootOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::foot_l));
					Vector3 vUpperArmRightOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::upperarm_r));
					Vector3 vUpperArmLeftOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::upperarm_r));
					Vector3 vLowerArmRightOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::lowerarm_r));
					Vector3 vLowerArmLeftOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::lowerarm_r));
					Vector3 vHandRightOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::hand_r));
					Vector3 vHandLeftOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, bones::hand_l));
					Vector3 vFootOutOut = ProjectWorldToScreen(GetBoneWithRotation(Entity.actor_mesh, 0));

					//DrawCircleFilled(vHeadOut.x, vHeadOut.y, 5.f, CFG.ESP_Color, 1000);

					DrawLine(ImVec2(vHeadOut.x, vHeadOut.y), ImVec2(vNeckOut.x, vNeckOut.y), CFG.VisibleColor, 1.5f);
					DrawLine(ImVec2(vNeckOut.x, vNeckOut.y), ImVec2(vSpine1Out.x, vSpine1Out.y), CFG.VisibleColor, 1.5f);
					DrawLine(ImVec2(vSpine1Out.x, vSpine1Out.y), ImVec2(vSpine2Out.x, vSpine2Out.y), CFG.VisibleColor, 1.5f);
					DrawLine(ImVec2(vSpine2Out.x, vSpine2Out.y), ImVec2(vSpine3Out.x, vSpine3Out.y), CFG.VisibleColor, 1.5f);

					DrawLine(ImVec2(vSpine1Out.x, vSpine1Out.y), ImVec2(vUpperArmRightOut.x, vUpperArmRightOut.y), CFG.VisibleColor, 1.5f);
					DrawLine(ImVec2(vSpine1Out.x, vSpine1Out.y), ImVec2(vUpperArmLeftOut.x, vUpperArmLeftOut.y), CFG.VisibleColor, 1.5f);

					DrawLine(ImVec2(vSpine1Out.x, vSpine1Out.y), ImVec2(vPelvisOut.x, vPelvisOut.y), CFG.VisibleColor, 1.5f);

					DrawLine(ImVec2(vPelvisOut.x, vPelvisOut.y), ImVec2(vRightThighOut.x, vRightThighOut.y), CFG.VisibleColor, 1.5f);
					DrawLine(ImVec2(vPelvisOut.x, vPelvisOut.y), ImVec2(vLeftThighOut.x, vLeftThighOut.y), CFG.VisibleColor, 1.5f);

					DrawLine(ImVec2(vRightThighOut.x, vRightThighOut.y), ImVec2(vRightCalfOut.x, vRightCalfOut.y), CFG.VisibleColor, 1.5f);
					DrawLine(ImVec2(vLeftThighOut.x, vLeftThighOut.y), ImVec2(vLeftCalfOut.x, vLeftCalfOut.y), CFG.VisibleColor, 1.5f);

					DrawLine(ImVec2(vRightCalfOut.x, vRightCalfOut.y), ImVec2(vRightFootOut.x, vRightFootOut.y), CFG.VisibleColor, 1.5f);
					DrawLine(ImVec2(vLeftCalfOut.x, vLeftCalfOut.y), ImVec2(vLeftFootOut.x, vLeftFootOut.y), CFG.VisibleColor, 1.5f);

					DrawLine(ImVec2(vUpperArmRightOut.x, vUpperArmRightOut.y), ImVec2(vLowerArmRightOut.x, vLowerArmRightOut.y), CFG.VisibleColor, 1.5f);
					DrawLine(ImVec2(vUpperArmLeftOut.x, vUpperArmLeftOut.y), ImVec2(vLowerArmLeftOut.x, vLowerArmLeftOut.y), CFG.VisibleColor, 1.5f);

					DrawLine(ImVec2(vLowerArmLeftOut.x, vLowerArmLeftOut.y), ImVec2(vHandLeftOut.x, vHandLeftOut.y), CFG.VisibleColor, 1.5f);
					DrawLine(ImVec2(vLowerArmRightOut.x, vLowerArmRightOut.y), ImVec2(vHandRightOut.x, vHandRightOut.y), CFG.VisibleColor, 1.5f);
				}
			}
		}
	}

	if (CFG.b_Aimbot)
	{
		if (CFG.b_AimbotFOV)
		{
			DrawCircle(GameVars.ScreenWidth / 2, GameVars.ScreenHeight / 2, CFG.AimbotFOV, CFG.FovColor, 0);
		}
	}

	GetCpuUsageVisual();
}

void InputHandler() {
	for (int i = 0; i < 5; i++) ImGui::GetIO().MouseDown[i] = false;
	int button = -1;
	if (GetAsyncKeyState(VK_LBUTTON)) button = 0;
	if (button != -1) ImGui::GetIO().MouseDown[button] = true;
}
void Render()
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		CFG.b_MenuShow = !CFG.b_MenuShow;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	RenderVisual();
	ImGui::GetIO().MouseDrawCursor = CFG.b_MenuShow;

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

	if (CFG.b_MenuShow)
	{
		InputHandler();
		ImGui::SetNextWindowSize(ImVec2(675, 530));
		ImGui::PushFont(DefaultFont);
		ImGui::Begin("Farlight 84", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		TabButton("Visuals", &CFG.tab_index, 0, true);
		TabButton("Aimbot", &CFG.tab_index, 1, true);
		TabButton("Misc", &CFG.tab_index, 2, true);
		TabButton("Config", &CFG.tab_index, 3, true);
		TabButton("Credit", &CFG.tab_index, 4, true);

		ImGui::Separator();

		if (CFG.tab_index == 0)
		{
			ImGui::Checkbox("Enable Hooking (required)", &CFG.b_Visual);
			if (CFG.b_Visual)
			{
				ImGui::Checkbox("Box ESP", &CFG.b_EspBox);
				ImGui::Checkbox("Skeleton ESP", &CFG.b_EspSkeleton);
				ImGui::Checkbox("Snaplines", &CFG.b_EspLine);
				ImGui::Checkbox("Name ESP", &CFG.b_EspName);
				ImGui::Checkbox("Distance ESP", &CFG.b_EspDistance);
				ImGui::Checkbox("Health ESP", &CFG.b_EspHealth);
				ImGui::Checkbox("Armor ESP", &CFG.b_EspShield);
				ImGui::Checkbox("Team ESP", &CFG.b_EspTeam);
				ImGui::Checkbox("Item ESP", &CFG.b_EspItem);
				ImGui::SliderFloat("Max Distance", &CFG.max_distance, 1.0f, 1000.0f);
				if (ImGui::ColorEdit3("Visible Color", CFG.fl_VisibleColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.VisibleColor = ImColor(CFG.fl_VisibleColor[0], CFG.fl_VisibleColor[1], CFG.fl_VisibleColor[2]);
				}
				if (CFG.b_EspBox)
				{
					ImGui::Combo("Box Type", &CFG.BoxType, CFG.BoxTypes, 2);
				}
				if (CFG.b_EspLine)
				{
					ImGui::Combo("Snapline Type", &CFG.LineType, CFG.LineTypes, 3);
				}
			}
		}
		else if (CFG.tab_index == 1)
		{
			ImGui::Checkbox("Memory Aimbot", &CFG.b_Aimbot);
			ImGui::Checkbox("Show Field of View", &CFG.b_AimbotFOV);
			if (CFG.b_AimbotFOV)
			{
				ImGui::SliderFloat("Field of View Size", &CFG.AimbotFOV, 1.0f, 500.f);
				if (ImGui::ColorEdit3("FOV Color", CFG.fl_FovColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.FovColor = ImColor(CFG.fl_FovColor[0], CFG.fl_FovColor[1], CFG.fl_FovColor[2]);
				}
			}
			ImGui::Combo("Aim Key 1", &CFG.AimKey[0], keyItems, IM_ARRAYSIZE(keyItems));
			ImGui::Combo("Aim Key 2", &CFG.AimKey[1], keyItems, IM_ARRAYSIZE(keyItems));

			ImGui::SliderFloat("Smoothing", &CFG.Smoothing, 1.0f, 10.0f);
			ImGui::Combo("Aim location", &CFG.aimLocation, CFG.aimLocations, IM_ARRAYSIZE(CFG.aimLocations));
		}
		else if (CFG.tab_index == 2)
		{
			ImGui::Checkbox("No Recoil", &CFG.b_NoRecoil);

			ImGui::Checkbox("No Spread", &CFG.b_NoSpread);

			ImGui::Checkbox("Fast Reload", &CFG.b_FastReload);

			ImGui::Checkbox("Fast Scope", &CFG.b_FastScope);

			ImGui::Combo("CPU usage", &CFG.cpuUsage, CFG.cpuUsages, IM_ARRAYSIZE(CFG.cpuUsages));
		}
		else if (CFG.tab_index == 3)
		{
			if (ImGui::Button("Save Config")) {
				SaveConfigToFile("config.txt");
			}
			if (ImGui::Button("Load Config")) {
				LoadConfigFromFile("config.txt");
			}
		}
		else if (CFG.tab_index == 4)
		{
			ImGui::Text("All the base code was made by Flags. <3");
			ImGui::Text("Was updated & config (save/load) by Bunti.");
			ImGui::Text("Now it's by TheGeogeo.");
			ImGui::Text("");
			ImGui::Text("This cheat is free if you paid it click below...");
			if (ImGui::Button("HERE"))
			{
				ShellExecute(0, 0, "https://www.youtube.com/watch?v=dQw4w9WgXcQ", 0, 0, SW_SHOW);
				CFG.rick_rolled = true;
			}
			if (CFG.rick_rolled)
			{
				ImGui::Text("Don't be stupid next time and check first on any forum :)");
			}
		}
		ImGui::PopFont();
		ImGui::End();
	}
	ImGui::EndFrame();

	DirectX9Interface::pDevice->SetRenderState(D3DRS_ZENABLE, false);
	DirectX9Interface::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DirectX9Interface::pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

	DirectX9Interface::pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (DirectX9Interface::pDevice->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		DirectX9Interface::pDevice->EndScene();
	}

	HRESULT result = DirectX9Interface::pDevice->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && DirectX9Interface::pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

void MainLoop() {
	static RECT OldRect;
	ZeroMemory(&DirectX9Interface::Message, sizeof(MSG));

	while (DirectX9Interface::Message.message != WM_QUIT) {
		if (PeekMessage(&DirectX9Interface::Message, OverlayWindow::Hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&DirectX9Interface::Message);
			DispatchMessage(&DirectX9Interface::Message);
		}
		HWND ForegroundWindow = GetForegroundWindow();
		if (ForegroundWindow == GameVars.gameHWND) {
			HWND TempProcessHwnd = GetWindow(ForegroundWindow, GW_HWNDPREV);
			SetWindowPos(OverlayWindow::Hwnd, TempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT TempRect;
		POINT TempPoint;
		ZeroMemory(&TempRect, sizeof(RECT));
		ZeroMemory(&TempPoint, sizeof(POINT));

		GetClientRect(GameVars.gameHWND, &TempRect);
		ClientToScreen(GameVars.gameHWND, &TempPoint);

		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameVars.gameHWND;

		POINT TempPoint2;
		GetCursorPos(&TempPoint2);
		io.MousePos.x = TempPoint2.x - TempPoint.x;
		io.MousePos.y = TempPoint2.y - TempPoint.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else {
			io.MouseDown[0] = false;
		}

		if (TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom) {
			OldRect = TempRect;
			GameVars.ScreenWidth = TempRect.right;
			GameVars.ScreenHeight = TempRect.bottom;
			DirectX9Interface::pParams.BackBufferWidth = GameVars.ScreenWidth;
			DirectX9Interface::pParams.BackBufferHeight = GameVars.ScreenHeight;
			SetWindowPos(OverlayWindow::Hwnd, (HWND)0, TempPoint.x, TempPoint.y, GameVars.ScreenWidth, GameVars.ScreenHeight, SWP_NOREDRAW);
			DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
		}
		Render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (DirectX9Interface::pDevice != NULL) {
		DirectX9Interface::pDevice->EndScene();
		DirectX9Interface::pDevice->Release();
	}
	if (DirectX9Interface::Direct3D9 != NULL) {
		DirectX9Interface::Direct3D9->Release();
	}
	DestroyWindow(OverlayWindow::Hwnd);
	UnregisterClass(OverlayWindow::WindowClass.lpszClassName, OverlayWindow::WindowClass.hInstance);
}

bool DirectXInit() {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9Interface::Direct3D9))) {
		return false;
	}

	D3DPRESENT_PARAMETERS Params = { 0 };
	Params.Windowed = TRUE;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.hDeviceWindow = OverlayWindow::Hwnd;
	Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	Params.BackBufferWidth = GameVars.ScreenWidth;
	Params.BackBufferHeight = GameVars.ScreenHeight;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if (FAILED(DirectX9Interface::Direct3D9->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, OverlayWindow::Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &DirectX9Interface::pDevice))) {
		DirectX9Interface::Direct3D9->Release();
		return false;
	}

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplWin32_Init(OverlayWindow::Hwnd);
	ImGui_ImplDX9_Init(DirectX9Interface::pDevice);
	DirectX9Interface::Direct3D9->Release();
	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message) {
	case WM_DESTROY:
		if (DirectX9Interface::pDevice != NULL) {
			DirectX9Interface::pDevice->EndScene();
			DirectX9Interface::pDevice->Release();
		}
		if (DirectX9Interface::Direct3D9 != NULL) {
			DirectX9Interface::Direct3D9->Release();
		}
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (DirectX9Interface::pDevice != NULL && wParam != SIZE_MINIMIZED) {
			ImGui_ImplDX9_InvalidateDeviceObjects();
			DirectX9Interface::pParams.BackBufferWidth = LOWORD(lParam);
			DirectX9Interface::pParams.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void SetupWindow() {
	OverlayWindow::WindowClass = {
		sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, OverlayWindow::Name, LoadIcon(nullptr, IDI_APPLICATION)
	};

	RegisterClassEx(&OverlayWindow::WindowClass);
	if (GameVars.gameHWND) {
		static RECT TempRect = { NULL };
		static POINT TempPoint;
		GetClientRect(GameVars.gameHWND, &TempRect);
		ClientToScreen(GameVars.gameHWND, &TempPoint);
		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		GameVars.ScreenWidth = TempRect.right;
		GameVars.ScreenHeight = TempRect.bottom;
	}

	OverlayWindow::Hwnd = CreateWindowEx(NULL, OverlayWindow::Name, OverlayWindow::Name, WS_POPUP | WS_VISIBLE, GameVars.ScreenLeft, GameVars.ScreenTop, GameVars.ScreenWidth, GameVars.ScreenHeight, NULL, NULL, 0, NULL);
	DwmExtendFrameIntoClientArea(OverlayWindow::Hwnd, &DirectX9Interface::Margin);
	SetWindowLong(OverlayWindow::Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	ShowWindow(OverlayWindow::Hwnd, SW_SHOW);
	UpdateWindow(OverlayWindow::Hwnd);
}

bool CreateConsole = true;

int main()
{
	system("kdmapper.exe driver.sys");
	driver::find_driver();
	ProcId = driver::find_process(GameVars.dwProcessName);
	BaseId = driver::find_image();
	GameVars.dwProcessId = ProcId;
	GameVars.dwProcess_Base = BaseId;
	printf("Driver: %d\n", driver_handle);
	PrintPtr("ProcId: ", GameVars.dwProcessId);
	PrintPtr("BaseId: ", GameVars.dwProcess_Base);

	HWND tWnd = FindWindowA("UnrealWindow", nullptr);
	if (tWnd)
	{
		GameVars.gameHWND = tWnd;
		RECT clientRect;
		GetClientRect(GameVars.gameHWND, &clientRect);
		POINT screenCoords = { clientRect.left, clientRect.top };
		ClientToScreen(GameVars.gameHWND, &screenCoords);
		printf("HWND: %d\n", GameVars.gameHWND);
	}

	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(GameCache), nullptr, NULL, nullptr);
	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(CallAimbot), nullptr, NULL, nullptr);
	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(CallHacks), nullptr, NULL, nullptr);

	if (CreateConsole == false)
		ShowWindow(GetConsoleWindow(), SW_HIDE);

	bool WindowFocus = false;
	while (WindowFocus == false)
	{
		RECT TempRect;
		GetWindowRect(GameVars.gameHWND, &TempRect);
		GameVars.ScreenWidth = TempRect.right - TempRect.left;
		GameVars.ScreenHeight = TempRect.bottom - TempRect.top;
		GameVars.ScreenLeft = TempRect.left;
		GameVars.ScreenRight = TempRect.right;
		GameVars.ScreenTop = TempRect.top;
		GameVars.ScreenBottom = TempRect.bottom;
		WindowFocus = true;
	}

	OverlayWindow::Name = RandomString(32).c_str();
	printf("OverlayWindow::Name: %s\n", OverlayWindow::Name);
	SetupWindow();
	DirectXInit();

	ImGuiIO& io = ImGui::GetIO();
	DefaultFont = io.Fonts->AddFontDefault();
	Verdana = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 16.0f);
	io.Fonts->Build();

	while (TRUE)
	{
		MainLoop();
	}
}