#define CINTERFACE

#include <Windows.h>
#include <vector>
#include "INIReader.h"
#include "MinHook.h"
using namespace std;

#define IsKeyPressed(CODE) ((GetKeyState(CODE) & 0x8000) > 0)

void Add_Loh_Message(const char * msg)
{
	FILE * f;
	fopen_s(&f, "logcast.txt", "a+");
	if (f)
	{
		fprintf_s(f, "%s\n", msg);
		fclose(f);
	}
}

struct KeyActionStruct
{
	int VK;
	int btnID;
	int altbtnID;
	BOOL IsSkill;
	BOOL IsShift;
	BOOL IsCtrl;
	BOOL IsAlt;
	BOOL IsRightClick;
};

int GetAltBtnID(int btnID)
{
	switch (btnID)
	{
	case 2:
		return 0;
	case 5:
		return 3;
	case 8:
		return 6;
	case 11:
		return 9;
	case 4:
		return 1;
	case 7:
		return 4;
	}

	return -1;
}

vector<KeyActionStruct> KeyActionList;

// Добавляет новый хоткей в список
int __stdcall AddKeyButtonAction(int KeyCode, int btnID, BOOL IsSkill)
{
	if (!KeyCode)
	{
		return 0;
	}

	KeyActionStruct tmpstr;
	tmpstr.VK = KeyCode & 0xFF;
	tmpstr.btnID = btnID;
	tmpstr.IsSkill = IsSkill;
	if (IsSkill)
		tmpstr.altbtnID = (GetAltBtnID(btnID));
	else
		tmpstr.altbtnID = 0;

	tmpstr.IsAlt = KeyCode & 0x10000;
	tmpstr.IsCtrl = KeyCode & 0x20000;
	tmpstr.IsShift = KeyCode & 0x40000;
	tmpstr.IsRightClick = KeyCode & 0x80000;

	char addedhotkeys[ 100 ];
	sprintf_s( addedhotkeys, "KeyCode:%X btnID:%X(ALT:%X) IsSkill:%s", KeyCode, btnID, GetAltBtnID( btnID ), IsSkill ? "Yes" : "No" );
	Add_Loh_Message(addedhotkeys);
	if (KeyCode & 0xF0000)
	{
		for (KeyActionStruct & curstr : KeyActionList)
		{
			if (curstr.btnID == tmpstr.btnID)
			{
				if (((!curstr.IsAlt && !curstr.IsCtrl && !curstr.IsShift) &&
					(!tmpstr.IsAlt && !tmpstr.IsCtrl && !tmpstr.IsShift))
					|| (curstr.IsAlt && tmpstr.IsAlt)
					|| (curstr.IsCtrl && tmpstr.IsAlt)
					|| (curstr.IsShift && tmpstr.IsAlt)
					)
				{
					/*	if ( SetInfoObjDebugVal )
					{
					PrintText( "Replaced hotkey" );
					}*/
					curstr = tmpstr;
					return 0;
				}
			}
		}
	}
	//if ( SetInfoObjDebugVal )
	//{
	//	PrintText( "added new hotkey" );
	//}
	KeyActionList.push_back(tmpstr);

	return 0;
}


// | 0 | 3 | 6 | 9  |
// | 1 | 4 | 7 | 10 | 
// | 2 | 5 | 8 | 11 |


// | 0 | 1
// | 2 | 3
// | 4 | 5



int GameDll = 0;

BOOL IsGame()
{
	int War3CurrentUIManager = *(int*)(GameDll + 0xACE66C);
	if (War3CurrentUIManager)
	{
		BOOL retval = *(int*)War3CurrentUIManager == GameDll + 0x93631C;
		return retval;
	}

	return FALSE;
}
int pW3XGlobalClass = 0;

// Возвращает адрес pW3XGlobalClass
int GetGlobalClassAddr()
{
	return *(int*)(GameDll + 0xAB4F80);
}

#define flagsOffset 0x138
#define sizeOfCommandButtonObj 0x1c0

// Получает адрес SimpleКнопки по ID (панель способностей)
int __stdcall GetSkillPanelButton(int idx)
{
	int pclass = GetGlobalClassAddr();
	if (pclass > 0)
	{
		int pGamePlayerPanelSkills = *(int*)(pclass + 0x3c8);
		if (pGamePlayerPanelSkills > 0)
		{
			int topLeftCommandButton = *(int*)(pGamePlayerPanelSkills + 0x154);
			if (topLeftCommandButton > 0)
			{
				topLeftCommandButton = **(int**)(topLeftCommandButton + 0x8);
				if (topLeftCommandButton > 0)
					return topLeftCommandButton + sizeOfCommandButtonObj * idx;
			}
		}
	}
	return 0;
}

// Получает адрес SimpleКнопки по ID (панель предметов)
int __stdcall GetItemPanelButton(int idx)// by Karaulov
{
#ifdef DOTA_HELPER_LOG
	AddNewLineToDotaHelperLog(__func__);
#endif
	int pclass = GetGlobalClassAddr();
	if (pclass > 0)
	{
		int pGamePlayerPanelItems = *(int*)(pclass + 0x3c4);
		if (pGamePlayerPanelItems > 0)
		{
			int topLeftCommandButton = *(int*)(pGamePlayerPanelItems + 0x148);
			if (topLeftCommandButton > 0)
			{
				topLeftCommandButton = *(int*)(topLeftCommandButton + 0x130);
				if (topLeftCommandButton > 0)
				{
					topLeftCommandButton = *(int*)(topLeftCommandButton + 0x4);
					if (topLeftCommandButton > 0)
					{
						return topLeftCommandButton + sizeOfCommandButtonObj * idx;
					}
				}
			}
		}
	}
	return 0;
}


// Возвращает >0 если чат открыт
BOOL IsChat()
{
	return*(BOOL*)(GameDll + 0xAD15F0);//1.27a 0xBDAA14
}

typedef int(__fastcall * c_SimpleButtonClickEvent)(int pButton, int unused, int ClickEventType);
c_SimpleButtonClickEvent SimpleButtonClickEvent;

// Нажать на кнопку скилов
void PressSkillPanelButton(int idx, BOOL RightClick)
{

	int button = GetSkillPanelButton(idx);
	if (button > 0 && *(int*)button > 0)
	{
		UINT oldflag = *(UINT *)(button + flagsOffset);
		if (!(oldflag & 2))
			*(UINT *)(button + flagsOffset) = oldflag | 2;
		SimpleButtonClickEvent(button, 0, RightClick ? 4 : 1);
		*(UINT *)(button + flagsOffset) = oldflag;
	}
}

// Нажать на кнопку предметов
void PressItemPanelButton(int idx, BOOL RightClick)
{
	int button = GetItemPanelButton(idx);
	if (button > 0 && *(int*)button > 0)
	{
		UINT oldflag = *(UINT *)(button + flagsOffset);
		if (!(oldflag & 2))
			*(UINT *)(button + flagsOffset) = oldflag | 2;
		SimpleButtonClickEvent(button, 0, RightClick ? 4 : 1);
		*(UINT *)(button + flagsOffset) = oldflag;
	}
}

BOOL IsNULLButtonFound(int pButton)
{
	if (pButton > 0 && *(int*)(pButton) > 0)
	{
		if (*(int*)(pButton + 0x190) != 0 && *(int*)(*(int*)(pButton + 0x190) + 4) == 0)
			return TRUE;
	}
	return FALSE;
}

typedef LRESULT(__fastcall *  WarcraftRealWNDProc)(HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam);
WarcraftRealWNDProc WarcraftRealWNDProc_ptr = NULL;
WarcraftRealWNDProc WarcraftRealWNDProc_org = NULL;

LPARAM lpShiftScanKeyDOWN = (LPARAM)(0x00000001 | (LPARAM)(MapVirtualKey(VK_SHIFT, 0) << 16));


HWND Warcraft3Window = 0;

void JustClickMouse()
{
	BOOL ButtonDown = FALSE;
	if (IsKeyPressed(VK_LBUTTON))
	{
		ButtonDown = TRUE;
		SendMessage(Warcraft3Window, WM_LBUTTONUP, 0, 0);
	}

	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &Input, sizeof(INPUT));

	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &Input, sizeof(INPUT));

}


int GetGlobalPlayerData()
{
	return  *(int *)(0xAB65F4 + GameDll);//1.27a 0xBE4238
}

// Получает адрес игрока по номеру слота
int GetPlayerByNumber(int number)
{
	int arg1 = GetGlobalPlayerData();
	int result = 0;
	if (arg1 > NULL)
	{
		result = (int)arg1 + (number * 4) + 0x58;
		if (result)
		{
			result = *(int*)result;
		}
		else
		{
			return 0;
		}
	}
	return result;
}

// Получить ID игрока
int GetLocalPlayerId()
{
	int gldata = GetGlobalPlayerData();
	if (gldata > 0)
	{
		short retval = *(short *)(gldata + 0x28);
		return retval;
	}
	return 0;
}
// Получает количество выделенных юнитов
int GetSelectedUnitCountBigger(int slot)
{
	int plr = GetPlayerByNumber(slot);
	if (plr > 0)
	{
		int PlayerData1 = *(int*)(plr + 0x34);
		if (PlayerData1 > 0)
		{
			int unitcount = *(int *)(PlayerData1 + 0x10);
			int unitcount2 = *(int *)(PlayerData1 + 0x1D4);

			if (unitcount > unitcount2)
				return unitcount;
			else
				return unitcount2;
		}
	}

	return NULL;
}
// Получает выделенного игроком юнита
int GetSelectedUnit(int slot)
{
	int plr = GetPlayerByNumber(slot);
	if (plr > 0)
	{
		int PlayerData1 = *(int*)(plr + 0x34);
		if (PlayerData1 > 0)
		{
			return *(int *)(PlayerData1 + 0x1e0);
		}
	}

	return NULL;
}

// Получает номер игрока-владельца юнита
int __stdcall GetUnitOwnerSlot(int unitaddr)
{
	if (unitaddr > 0)
		return *(int*)(unitaddr + 88);
	return 15;
}

void PressMouseSuperSpeed()
{

	JustClickMouse();
}

BOOL AllOk3 = FALSE;

LRESULT __fastcall BeforeWarcraftWNDProc(HWND hWnd, unsigned int _Msg, WPARAM _wParam, LPARAM lParam)
{
	unsigned int Msg = _Msg;
	WPARAM wParam = _wParam;

	if (Warcraft3Window == 0)
	{
		Warcraft3Window = hWnd;
		Add_Loh_Message("Все норм 2");
	}

	if (IsGame() && !IsChat())
	{
		if (!AllOk3)
		{
			Add_Loh_Message("Все норм 3");
			AllOk3 = TRUE;
		}

		// SHIFT+NUMPAD TRICK
		if ((Msg == WM_KEYDOWN || Msg == WM_KEYUP) && (
			wParam == 0xC ||
			wParam == 0x23 ||
			wParam == 0x24 ||
			wParam == 0x25 ||
			wParam == 0x26 ||
			wParam == 0x28
			))
		{
			int  scanCode = (int)((lParam >> 24) & 0x1);


			if (scanCode != 1)
			{
				switch (wParam)
				{
				case 0x23:
					wParam = VK_NUMPAD1;
					break;
				case 0x28:
					wParam = VK_NUMPAD2;
					break;
				case 0x25:
					wParam = VK_NUMPAD4;
					break;
				case 0xC:
					wParam = VK_NUMPAD5;
					break;
				case 0x24:
					wParam = VK_NUMPAD7;
					break;
				case 0x26:
					wParam = VK_NUMPAD8;
					break;
				default:
					break;
				}
				if (wParam != _wParam)
				{
					if (!IsKeyPressed(VK_SHIFT))
					{
						BOOL NumLock = (((unsigned short)GetKeyState(VK_NUMLOCK)) & 0xffff) != 0;
						if (NumLock)
							WarcraftRealWNDProc_ptr(hWnd, WM_KEYDOWN, VK_SHIFT, lpShiftScanKeyDOWN);
					}
				}
			}
		}


		BOOL NeedSkipThisKey = FALSE;


		if (Msg == WM_KEYDOWN || Msg == WM_XBUTTONDOWN || Msg == WM_MBUTTONDOWN ||
			Msg == WM_SYSKEYDOWN)
		{

			if (_Msg == WM_XBUTTONDOWN)
			{
				Msg = WM_KEYDOWN;
				wParam = _wParam & MK_XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
			}

			if (_Msg == WM_MBUTTONDOWN)
			{
				Msg = WM_KEYDOWN;
				wParam = VK_MBUTTON;
			}

			//	//CONSOLE_Print( "[Debug] : found " + to_string( KeyActionList.size( ) ) + " hotkeys..." );

			BOOL itempressed = FALSE;
			for (KeyActionStruct keyAction : KeyActionList)
			{
				if (keyAction.VK == (int)wParam)
				{
					char LogTmp[ 100 ];
					sprintf_s( LogTmp, 100, "Press %X key for %i", keyAction.VK, keyAction.btnID );
					Add_Loh_Message(LogTmp);

					itempressed = !keyAction.IsSkill;

					if ((!keyAction.IsAlt && !keyAction.IsCtrl && !keyAction.IsShift)
						|| (keyAction.IsAlt && IsKeyPressed(VK_MENU))
						|| (keyAction.IsCtrl && IsKeyPressed(VK_CONTROL))
						|| (keyAction.IsShift && IsKeyPressed(VK_SHIFT))
						)
					{
						NeedSkipThisKey = TRUE;
						if ((keyAction.IsAlt && IsKeyPressed(VK_MENU))
							|| (keyAction.IsCtrl && IsKeyPressed(VK_CONTROL))
							|| (keyAction.IsShift && IsKeyPressed(VK_SHIFT))
							)
						{
							if (Msg == WM_SYSKEYDOWN)
								Msg = WM_KEYDOWN;
						}
						else
						{
							if (IsKeyPressed(VK_MENU)
								|| IsKeyPressed(VK_CONTROL))
							{
								NeedSkipThisKey = FALSE;
							}
						}


						int selectedunitcout = 0;
						int selectedunit = GetSelectedUnit(GetLocalPlayerId());
						if (selectedunit > 0/* && (selectedunitcout = GetSelectedUnitCountBigger(GetLocalPlayerId())) > 0*/)
						{
							int unitowner = GetUnitOwnerSlot(selectedunit);
						
							//if (GetUnitOwnerSlot(selectedunit) != 15)
						//	{
								if (IsNULLButtonFound(GetSkillPanelButton(11)))
								{
									if (keyAction.altbtnID >= 0)
									{
										if (!(lParam & 0x40000000))
										{
											if (keyAction.IsSkill)
												PressSkillPanelButton(keyAction.altbtnID, keyAction.IsRightClick);
											else
												PressItemPanelButton(keyAction.btnID, keyAction.IsRightClick);
											
											PressMouseSuperSpeed();
											break;

										}

									}
								}
								else
								{
									if (!(lParam & 0x40000000))
									{
										if (keyAction.IsSkill)
											PressSkillPanelButton(keyAction.btnID, keyAction.IsRightClick);
										else
											PressItemPanelButton(keyAction.btnID, keyAction.IsRightClick);
										
										PressMouseSuperSpeed();
										break;

									}

								}
							//}
						}

					}
				}
			}



			if (_Msg == WM_XBUTTONDOWN
				|| _Msg == WM_MBUTTONDOWN)
			{
				Msg = _Msg;
				wParam = _wParam;
			}

		}


		if (NeedSkipThisKey)
			return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return WarcraftRealWNDProc_ptr(hWnd, Msg, wParam, lParam);
}

HRESULT __stdcall DllMain(HMODULE _hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		MH_Initialize();


		GameDll = (int)GetModuleHandle("Game.dll");
		INIReader QuickCastConfig(".\\conf.ini");

		SimpleButtonClickEvent = (c_SimpleButtonClickEvent)(GameDll + 0x603440);//1.27a  0x0BB560
		WarcraftRealWNDProc_org = (WarcraftRealWNDProc)(GameDll + 0x6C6AA0);//1.27a  0x153710 

		MH_CreateHook(WarcraftRealWNDProc_org, &BeforeWarcraftWNDProc, reinterpret_cast< void** >(&WarcraftRealWNDProc_ptr));
		MH_EnableHook(WarcraftRealWNDProc_org);

		
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "Q", 0), 2, TRUE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "W", 0), 5, TRUE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "E", 0), 8, TRUE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "R", 0), 11, TRUE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "T", 0), 4, TRUE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "Y", 0), 7, TRUE);

		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "Num1", 0), 4, FALSE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "Num2", 0), 5, FALSE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "Num4", 0), 2, FALSE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "Num5", 0), 3, FALSE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "Num7", 0), 0, FALSE);
		AddKeyButtonAction(QuickCastConfig.GetInteger("QUICKCAST", "Num8", 0), 1, FALSE);

		Add_Loh_Message("Все норм");
	}
	return 1;
}


