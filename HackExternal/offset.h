#pragma once

#ifndef BLOODHUNT_H

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <dwmapi.h>
#include  <d3d9.h>
#include  <d3dx9.h>

#include "singleton.h"
#include "vector.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

inline namespace BloodHunt
{
	class Variables : public Singleton<Variables>
	{
	public:
		const char* dwProcessName = "SolarlandClient-Win64-Shipping.exe";
		DWORD dwProcessId = NULL;
		uint64_t dwProcess_Base = NULL;
		HWND gameHWND = NULL;

		int actor_count = NULL;
		int ScreenHeight = NULL;
		int ScreenWidth = NULL;
		int ScreenLeft = NULL;
		int ScreenRight = NULL;
		int ScreenTop = NULL;
		int ScreenBottom = NULL;

		float ScreenCenterX = ScreenWidth / 2;
		float ScreenCenterY = ScreenHeight / 2;

		DWORD_PTR game_instance = NULL;
		DWORD_PTR u_world = NULL;
		DWORD_PTR local_player_pawn = NULL;
		DWORD_PTR local_player_array = NULL;
		DWORD_PTR local_player = NULL;
		DWORD_PTR local_player_root = NULL;
		DWORD_PTR local_player_controller = NULL;
		DWORD_PTR local_player_state = NULL;
		DWORD_PTR persistent_level = NULL;
		DWORD_PTR actors = NULL;
		DWORD_PTR ranged_weapon_component = NULL;
		DWORD_PTR equipped_weapon_type = NULL;
	};
#define GameVars BloodHunt::Variables::Get()

	class Offsets : public Singleton<Offsets>
	{
	public:

		DWORD offset_u_world = 0x67B5080;//.text:00007FF6523B2E02	sub_7FF6523B26B0	mov     rbx, cs : 7FF658411CE0h // 0x7026B0 or // 0x23B26B0 // 676 1CE0
		DWORD offset_g_names = 0x6645580;//.text:00007FF6540BE386	sub_7FF6540BE2E0	db  48h; H		  7FF651CB0000 // 0x240E2E0 7FF6582A2240
		DWORD offset_g_obj = 0x665DDF0;//.48 8B 05 ?? ?? ?? ?? 48 8B 14 C8 49 63 C0 48 8D 0C 40 48 39 34 CA .text:00007FF6525547D8	sub_7FF652554750	mov     rax, cs:7FF6582BAA90h
		//Camera
		DWORD offset_camera_manager = 0x2d8; // APlayerController->PlayerCameraManager
		DWORD offset_camera_cache = 0x1bf0; //APlayerCameraManager->CameraCachePrivate0x2230
		DWORD offset_component_to_world = 0x250; // bComponentToWorldUpdated  0x14c 0x1c0 0x14c 0x4701c0 truct FTransform K2_GetComponentToWorld(); // Function Engine.SceneComponent.K2_GetComponentToWorld      0F 28 81 ? ? ? ? 48 8B C2 0F 28 89 ? ? ? ? 0F 29 02
		DWORD offset_relative_location = 0x11c/* + 0x144*/; //USceneComponent->RelativeLocation + 0x144
		DWORD offset_relative_Rotation = 0x128; //USceneComponent->RelativeLocation + 0x144
		DWORD offset_ComponentVelocity = 0x140; //USceneComponent->RelativeLocation + 0x144
		//Engine
		DWORD offset_persistent_level = 0x30; //UWorld->PersistentLevel
		DWORD offset_game_instance = 0x220; //UWolrd->OwningGameInstance
		DWORD offset_Uscenecomponent = 0x230; //UWolrd->OwningGameInstance
		DWORD offset_local_players_array = 0x38; //UGameInstance->LocalPlayers
		DWORD offset_player_controller = 0x30; //UPlayer->PlayerController
		DWORD offset_apawn = 0x2c0;  //APlayerController->AcknowledgedPawn
		DWORD offset_root_component = 0x138; //AActor->RootComponent
		DWORD offset_objectid = 0x18;
		DWORD offset_actor_array = 0x98; //UNetConnection->OwningActor
		DWORD offset_actor_count = 0xa0; //UNetConnection->MaxPacket
		DWORD offset_actor_id = 0x18;
		DWORD offset_player_state = 0x248; //apawjn->PlayerState
		DWORD offset_actor_mesh = 0x288; //ACharacter->Mesh
		DWORD offset_actor_StaticMesh = 0x588; //ACharacter->Mesh
		DWORD offset_bone_array = 0x598;// // 488// 0x4B0
		DWORD offset_CachedCurrentWeapon = 0x1a30; // struct ASolarPlayerWeapon* CachedCurrentWeapon; // 0x1a18(0x08)
		DWORD offset_RecoilComponent = 0x598; // // struct UWeaponRecoilComponent* RecoilComponent; // 0x598(0x08)
		DWORD offset_USingleWeaponConfig = 0x0570; // // 	struct USingleWeaponConfig* ; //
		DWORD offset_UWeaponShootConfig = 0x0120; // struct UWeaponShootConfig
		DWORD offset_WeaponConfig = 0x00D0; // possibly WeaponFireModeType;   WeaponAbilityTag; or 	struct FGameplayTag WeaponAbilityTag; // 0xd8(0x08)
		DWORD offset_primaryammo = 0x02D8; // // struct UAmmoConfig* PrimaryAmmo;
		DWORD offset_secondaryammo = 0x02E8; // UAmmoConfig* secondaryammo;
		DWORD offset_secondaryupdateammo = 0x02F8; // UAmmoConfig* secondaryupdateammo;
		DWORD offset_CurrentClipAmmo = 0x618; // int32_t  CurrentClipAmmo;  // 0x0618   (0x0004)
		DWORD offset_bulletSpeedInitSpeed = 0x00D0; // // Class /Script/Solarland.AmmoConfig float InitSpeed;  // 0x00D0   (0x0004)
		DWORD offset_bulletSpeedProjectileMaxGravity = 0x0514; // // Class /Script/Solarland.AmmoConfig float  ProjectileMaxGravity; // 0x0514   (0x0004)
		DWORD offset_FAmmonRecoilScope = 0x3e0; // UAmmoConfig
		DWORD offset_FAmmonRecoilRollStruct = 0x03A0; // UAmmoConfig
		DWORD offset_FAmmonRecoilScope2 = 0x03E0; // UAmmoConfig
		DWORD offset_FAmmonRecoilRollStruct2 = 0x03A0; //UAmmoConfig
		DWORD offset_FAmmonRecoilScope3 = 0x03E0; //UAmmoConfig
		DWORD offset_FAmmonRecoilRollStruct3 = 0x03A0; // UAmmoConfig
		DWORD offset_WeaponTextType = 0x868; //FString  WeaponTextType;  // 0x0868   (0x0010)
		DWORD offset_Hp = 0x580; // //ASolarPlayerState  float CurrentHealth; // 0x580(0x04)
		DWORD offset_HpMax = 0x584; // ASolarPlayerState float MaxHealth; // 0x584(0x04)
		DWORD offset_ShieldInfo = 0x588; // APlayerState char bIsABot : 1; // 0x232(0x01)
		DWORD offset_ShieldLevel = 0x0; // APlayerState char bIsABot : 1; // 0x232(0x01) + offset_ShieldInfo
		DWORD offset_Shield = 0x4; // APlayerState char bIsABot : 1; // 0x232(0x01) + offset_ShieldInfo
		DWORD offset_ShieldMax = 0x10f4; // APlayerState char bIsABot : 1; // 0x232(0x01)
		DWORD offset_player_name = 0x310; // APlayerState 	PlayerNamePrivate 	0x300 	FString
		DWORD offset_ESCMPlayerTypeisBOT = 0x0ABA; // APlayerState char bIsABot : 1; // 0x232(0x01)
		DWORD offset_ASolarTeamInfotTeam = 0x0A88; // APlayerState char bIsABot : 1; // 0x232(0x01)
		DWORD offset_ASolarTeamInfotTeamID = 0x0240; // APlayerState char bIsABot : 1; // 0x232(0x01)
		DWORD offset_KillNum = 0x728; // APlayerState char bIsABot : 1; // 0x232(0x01)
		DWORD offset_bot = 0x0ABA;
		DWORD offset_Level = 0x458;
		DWORD offset_Spects = 0x310; // APlayerState 	PlayerNamePrivate 	0x300 	FString
		DWORD offset_Ping = 0x0; // AServerStatReplicator -> NumRelevantDeletedActors
		DWORD offset_KDA = 0x0; // AServerStatReplicator -> NumReplicatedActors
		DWORD offset_USolarCharacterCamComponent = 0x08B0; // //class /Script/Solarland.SolarPlayerController USolarCharacterCameraComponent* FollowCamera; // 0x08B0   (0x0008)
		DWORD offset_USolarCharacterCamComponent2 = 0x09C8; //  Class /Script/Solarland.SolarCharacterCameraComponent USolarCharacterCameraDataAsset* CharacterCameraConfig; // 0x09C8   (0x0008)
		DWORD offset_CurrentCharacterStateInGame = 0x05C9; // Class Solarland.SolarPlayerState char CurrentCharacterStateInGame; // 0x05C9(0x01)
		DWORD offset_last_submit_time = 0x288; // AServerStatReplicator -> NumRelevantDeletedActors
		DWORD offset_last_render_time = 0x28C;
		DWORD offset_ranged_weapon_component = 0x4c8;
		DWORD offset_equipped_weapon_type = 0x36d; //old: 0x170
		DWORD offset_BodyType = 0x7a4;
		DWORD offset_solaranimstate = 0x7a0;
		DWORD offset_gamejobmanager = 0x228;
		DWORD offset_ItemData = 0x268;
		DWORD offset_ItemID = 0x0c; //int
		DWORD offset_ItemName = 0x18; //FString
		DWORD offset_ItemQuality = 0x50; //int
	};

#define GameOffset BloodHunt::Offsets::Get()
}
#endif  !BLOODHUNT_H
