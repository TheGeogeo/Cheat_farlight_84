#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <dwmapi.h>
#include <d3d9.h>
#include <string>
#include <fstream>

#include "offset.h"
#include "cfg.h"
#include "driver.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

template<class T>
class TArray
{
public:
	int Length() const
	{
		return m_nCount;
	}

	bool IsValid() const
	{
		if (m_nCount > m_nMax)
			return false;
		if (!m_Data)
			return false;
		return true;
	}

	uint64_t GetAddress() const
	{
		return m_Data;
	}

	T GetById(int i)
	{
		return read<T>(m_Data + i * 8);
	}

protected:
	uint64_t m_Data;
	uint32_t m_nCount;
	uint32_t m_nMax;
};

struct FString : private TArray<wchar_t>
{
	std::wstring ToWString() const
	{
		wchar_t* buffer = new wchar_t[m_nCount];
		read_array(m_Data, buffer, m_nCount);
		std::wstring ws(buffer);
		delete[] buffer;

		return ws;
	}

	std::string ToString() const
	{
		std::wstring ws = ToWString();
		std::string str(ws.begin(), ws.end());

		return str;
	}
};

std::string RandomString(int len)
{
	srand(time(NULL));
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string newstr;
	int pos;
	while (newstr.size() != len) {
		pos = ((rand() % (str.size() - 1)));
		newstr += str.substr(pos, 1);
	}
	return newstr;
}

void SaveConfigToFile(const char* filename) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open config file for writing!" << std::endl;
		return;
	}

	// Write boolean settings to the file
	file << "b_MenuShow=" << CFG.b_MenuShow << "\n";
	file << "b_Visual=" << CFG.b_Visual << "\n";
	file << "b_EspBox=" << CFG.b_EspBox << "\n";
	file << "b_EspSkeleton=" << CFG.b_EspSkeleton << "\n";
	file << "b_EspLine=" << CFG.b_EspLine << "\n";
	file << "b_EspDistance=" << CFG.b_EspDistance << "\n";
	file << "b_EspHealth=" << CFG.b_EspHealth << "\n";
	file << "b_EspShield=" << CFG.b_EspShield << "\n";
	file << "b_EspTeam=" << CFG.b_EspTeam << "\n";
	file << "b_EspItem=" << CFG.b_EspItem << "\n";
	file << "b_EspName=" << CFG.b_EspName << "\n";
	file << "b_Aimbot=" << CFG.b_Aimbot << "\n";
	file << "b_AimbotFOV=" << CFG.b_AimbotFOV << "\n";
	file << "b_AimbotSmooth=" << CFG.b_AimbotSmooth << "\n";
	file << "b_NoRecoil=" << CFG.b_NoRecoil << "\n";
	file << "b_NoSpread=" << CFG.b_NoSpread << "\n";
	file << "b_FastReload=" << CFG.b_FastReload << "\n";
	file << "b_FastScope=" << CFG.b_FastScope << "\n";

	//config
	file << "b_fxHotkey=" << CFG.b_fxHotkey << "\n";
	file << "b_numpadHotkey=" << CFG.b_numpadHotkey << "\n";

	// Write color settings
	file << "VisibleColor=" << CFG.fl_VisibleColor[0] << "," << CFG.fl_VisibleColor[1] << "," << CFG.fl_VisibleColor[2] << "\n";
	file << "FovColor=" << CFG.fl_FovColor[0] << "," << CFG.fl_FovColor[1] << "," << CFG.fl_FovColor[2] << "\n";

	// Write integer and float settings
	file << "cpuUsage=" << CFG.cpuUsage << "\n";
	file << "BoxType=" << CFG.BoxType << "\n";
	file << "LineType=" << CFG.LineType << "\n";
	file << "aimLocation=" << CFG.aimLocation << "\n";
	file << "selectedReload=" << CFG.selectedReload << "\n";
	file << "tab_index=" << CFG.tab_index << "\n";
	file << "AimKey1=" << CFG.AimKey[0] << "\n";
	file << "AimKey2=" << CFG.AimKey[1] << "\n";
	file << "Smoothing=" << CFG.Smoothing << "\n";
	file << "AimbotFOV=" << CFG.AimbotFOV << "\n";
	file << "max_distance=" << CFG.max_distance << "\n";

	file.close();
}

void ConsoleMsgLog(std::string msg) {
	std::cerr << msg << std::endl;
}

void LoadConfigFromFile(const char* filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open config file for reading!" << std::endl;
		return;
	}

	std::string line;
	while (getline(file, line)) {
		// Load boolean settings
		if (line.find("b_MenuShow=") != std::string::npos) {
			CFG.b_MenuShow = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_Visual=") != std::string::npos) {
			CFG.b_Visual = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_EspBox=") != std::string::npos) {
			CFG.b_EspBox = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_EspSkeleton=") != std::string::npos) {
			CFG.b_EspSkeleton = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_EspLine=") != std::string::npos) {
			CFG.b_EspLine = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_EspDistance=") != std::string::npos) {
			CFG.b_EspDistance = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_EspHealth=") != std::string::npos) {
			CFG.b_EspHealth = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_EspShield=") != std::string::npos) {
			CFG.b_EspShield = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_EspName=") != std::string::npos) {
			CFG.b_EspName = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_EspTeam=") != std::string::npos) {
			CFG.b_EspTeam = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_EspItem=") != std::string::npos) {
			CFG.b_EspItem = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_Aimbot=") != std::string::npos) {
			CFG.b_Aimbot = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_AimbotFOV=") != std::string::npos) {
			CFG.b_AimbotFOV = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_AimbotSmooth=") != std::string::npos) {
			CFG.b_AimbotSmooth = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_NoRecoil=") != std::string::npos) {
			CFG.b_NoRecoil = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_NoSpread=") != std::string::npos) {
			CFG.b_NoSpread = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_FastReload=") != std::string::npos) {
			CFG.b_FastReload = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_FastScope=") != std::string::npos) {
			CFG.b_FastScope = std::stoi(line.substr(line.find("=") + 1));
		}
		// config
		else if (line.find("b_fxHotkey=") != std::string::npos) {
			CFG.b_fxHotkey = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("b_numpadHotkey=") != std::string::npos) {
			CFG.b_numpadHotkey = std::stoi(line.substr(line.find("=") + 1));
		}
		// Load color settings
		else if (line.find("VisibleColor=") != std::string::npos) {
			std::string colors = line.substr(line.find("=") + 1);
			std::istringstream ss(colors);
			std::string token;
			int index = 0;
			while (std::getline(ss, token, ',')) {
				CFG.fl_VisibleColor[index++] = std::stof(token);
			}
		}
		else if (line.find("FovColor=") != std::string::npos) {
			std::string colors = line.substr(line.find("=") + 1);
			std::istringstream ss(colors);
			std::string token;
			int index = 0;
			while (std::getline(ss, token, ',')) {
				CFG.fl_FovColor[index++] = std::stof(token);
			}
		}
		// Load integer and float settings
		else if (line.find("cpuUsage=") != std::string::npos) {
			CFG.cpuUsage = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("BoxType=") != std::string::npos) {
			CFG.BoxType = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("LineType=") != std::string::npos) {
			CFG.LineType = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("aimLocation=") != std::string::npos) {
			CFG.aimLocation = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("selectedReload=") != std::string::npos) {
			CFG.selectedReload = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("tab_index=") != std::string::npos) {
			CFG.tab_index = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("AimKey1=") != std::string::npos) {
			CFG.AimKey[0] = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("AimKey2=") != std::string::npos) {
			CFG.AimKey[1] = std::stoi(line.substr(line.find("=") + 1));
		}
		else if (line.find("Smoothing=") != std::string::npos) {
			CFG.Smoothing = std::stof(line.substr(line.find("=") + 1));
		}
		else if (line.find("AimbotFOV=") != std::string::npos) {
			CFG.AimbotFOV = std::stof(line.substr(line.find("=") + 1));
		}
		else if (line.find("max_distance=") != std::string::npos) {
			CFG.max_distance = std::stof(line.substr(line.find("=") + 1));
		}
	}

	file.close();
}

void move_to(float x, float y)
{
	float center_x = GameVars.ScreenWidth / 2;
	float center_y = GameVars.ScreenHeight / 2;

	float smooth = CFG.Smoothing;

	float target_x = 0.f;
	float target_y = 0.f;

	if (x != 0.f)
	{
		if (x > center_x)
		{
			target_x = -(center_x - x);
			target_x /= smooth;
			if (target_x + center_x > center_x * 2.f) target_x = 0.f;
		}

		if (x < center_x)
		{
			target_x = x - center_x;
			target_x /= smooth;
			if (target_x + center_x < 0.f) target_x = 0.f;
		}
	}

	if (y != 0.f)
	{
		if (y > center_y)
		{
			target_y = -(center_y - y);
			target_y /= smooth;
			if (target_y + center_y > center_y * 2.f) target_y = 0.f;
		}

		if (y < center_y)
		{
			target_y = y - center_y;
			target_y /= smooth;
			if (target_y + center_y < 0.f) target_y = 0.f;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(target_x), static_cast<DWORD>(target_y), 0, 0);
}

D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray = read<DWORD_PTR>(mesh + GameOffset.offset_bone_array);
	if (!bonearray)
		bonearray = read<DWORD_PTR>(mesh + GameOffset.offset_bone_array + 0x10);
	return read<FTransform>(bonearray + (index * 0x30));
}

Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = read<FTransform>(mesh + GameOffset.offset_component_to_world);
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}
std::string GetNameFromFName(int key)
{
	auto chunkOffset = (UINT)((int)(key) >> 16);
	auto nameOffset = (USHORT)key;

	auto namePoolChunk = read<UINT64>(GameVars.dwProcess_Base + GameOffset.offset_g_names + ((chunkOffset + 2) * 8));
	auto entryOffset = namePoolChunk + (ULONG)(2 * nameOffset);
	auto nameEntry = read<INT16>(entryOffset);

	auto nameLength = nameEntry >> 6;
	char buff[1028];
	if ((DWORD)nameLength && nameLength > 0)
	{
		read_array(entryOffset + 2, buff, nameLength);
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	else return "";
}
D3DXMATRIX Matrix(Vector3 rot, Vector3 origin)
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}
struct FMinimalViewInfo
{
	Vector3 Location; // 0x00(0x0c)
	Vector3 Rotation; // 0x0c(0x0c)
	float FOV; // 0x18(0x04)
	float DesiredFOV; // 0x1c(0x04)
	float OrthoWidth; // 0x20(0x04)
	float OrthoNearClipPlane; // 0x24(0x04)
	float OrthoFarClipPlane; // 0x28(0x04)
	float AspectRatio; // 0x2c(0x04)
};

struct FCameraCacheEntry
{
	float Timestamp; // 0x00(0x04)
	char pad_4[0xc]; // 0x04(0x0c)
	FMinimalViewInfo POV; // 0x10(0x5e0)
};

D3DMATRIX to_matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radpitch = (rot.x * M_PI / 180);
	float radyaw = (rot.y * M_PI / 180);
	float radroll = (rot.z * M_PI / 180);
	float sp = sinf(radpitch);
	float cp = cosf(radpitch);
	float sy = sinf(radyaw);
	float cy = cosf(radyaw);
	float sr = sinf(radroll);
	float cr = cosf(radroll);
	D3DMATRIX matrix{};
	matrix.m[0][0] = cp * cy;
	matrix.m[0][1] = cp * sy;
	matrix.m[0][2] = sp;
	matrix.m[0][3] = 0.f;
	matrix.m[1][0] = sr * sp * cy - cr * sy;
	matrix.m[1][1] = sr * sp * sy + cr * cy;
	matrix.m[1][2] = -sr * cp;
	matrix.m[1][3] = 0.f;
	matrix.m[2][0] = -(cr * sp * cy + sr * sy);
	matrix.m[2][1] = cy * sr - cr * sp * sy;
	matrix.m[2][2] = cr * cp;
	matrix.m[2][3] = 0.f;
	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;
	return matrix;
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);
	auto camera_manager = read<uintptr_t>(GameVars.local_player_controller + GameOffset.offset_camera_manager); // APlayerController->PlayerCameraManager
	auto CameraCache = read<FCameraCacheEntry>(camera_manager + GameOffset.offset_camera_cache);

	D3DMATRIX tempMatrix = Matrix(CameraCache.POV.Rotation, Vector3(0, 0, 0));

	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - CameraCache.POV.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	float FovAngle = CameraCache.POV.FOV;

	float ScreenCenterX = GameVars.ScreenWidth / 2.0f;
	float ScreenCenterY = GameVars.ScreenHeight / 2.0f;
	//float ScreenCenterZ = Height / 2.0f;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	//Screenlocation.z = ScreenCenterZ - vTransformed.z * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}

void GetCpuUsageVisual()
{
	if (CFG.cpuUsage == 1)
	{
		Sleep(20);
	}
	else if (CFG.cpuUsage == 2)
	{
		Sleep(40);
	}
}

void GetCpuUsageAimbot()
{
	if (CFG.cpuUsage == 0)
	{
		Sleep(1);
	}
	if (CFG.cpuUsage == 1)
	{
		Sleep(20);
	}
	else if (CFG.cpuUsage == 2)
	{
		Sleep(40);
	}
}

int GetIndexByShieldId(int id)
{
	for (int i = 0; i < (sizeof(CFG.shieldId) / sizeof(CFG.shieldId[0])); i++)
	{
		if (CFG.shieldId[i] == id) return i;
	}
	return 0;
}

float GetMaxShieldByIndexArray(int id)
{
	switch (id)
	{
	case 0:
		return 1.0f;
	case 1:
		return 40.0f;
	case 2:
		return 60.0f;
	case 3:
		return 80.0f;
	case 4:
		return 100.0f;
	case 5:
		return 120.0f;
	default:
		break;
	}
	return 1.0f;
}

bool IsWeaponAR(int itemID)
{
	if (itemID == 1510201 || itemID == 1510207 || itemID == 1510208
		|| itemID == 1510219 || itemID == 1510210 || itemID == 1510216
		|| itemID == 1510222 || itemID == 1510223 || itemID == 1510201)
		return true;
	return false;
}

bool IsWeaponSMG(int itemID)
{
	if (itemID == 1510250 || itemID == 1510203 || itemID == 1510209
		|| itemID == 1510211 || itemID == 1510218)
		return true;
	return false;
}

bool IsWeaponSNP(int itemID)
{
	if (itemID == 1510204 || itemID == 1510206 || itemID == 1510212)
		return true;
	return false;
}

bool IsWeaponSpecial(int itemID)
{
	if (itemID == 1511521 || itemID == 1511522)
		return true;
	return false;
}

bool IsWeaponSHG(int itemID)
{
	if (itemID == 1510205 || itemID == 1510214)
		return true;
	return false;
}