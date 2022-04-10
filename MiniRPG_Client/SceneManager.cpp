#include "SceneManager.h"

SceneManager::SceneManager()
{
	// 배경 타일 로드
	m_iTile.Load(L"Tile.png");
	
	// 플레이어, 다른 유저 이미지 로드
	m_iPlayer.Load(L"Player.png");
	m_iUser.Load(L"User.png");

	// 몬스터 이미지 로드
	m_iMonster1.Load(L"Monster1.png");
	m_iMonster2.Load(L"Monster2.png");
	m_iMonster3.Load(L"Monster3.png");
	m_iMonster4.Load(L"Monster4.png");

	// 챗팅 활성화 아이콘
	m_iChatIcon.Load(L"ChatIcon.png");

	// 택스처 알파 세팅
	ImageAlphaSetting(&m_iTile);
	ImageAlphaSetting(&m_iPlayer);
	ImageAlphaSetting(&m_iUser);
	ImageAlphaSetting(&m_iMonster1);
	ImageAlphaSetting(&m_iMonster2);
	ImageAlphaSetting(&m_iMonster3);
	ImageAlphaSetting(&m_iMonster4);
	ImageAlphaSetting(&m_iChatIcon);

	// 화면 크기, 타일 크기 설정
	m_nWin_Width = 960;
	m_nWin_Height = 900;

	m_nTile_Width = 32;
	m_nTile_Height = 32;
	
	// 클라이언트 배열 초기화
	for (int i = 0; i < NUM_OF_USER; ++i)
	{
		m_cClients[i].objectType = OT_NONE;
	}

	// NPC 배열 초기화
	for (int i = 0; i < NUM_OF_NPC; ++i)
	{
		m_cNPCs[i].objectType = OT_NONE;
	}

	// 내 아이디 초기화
	m_Myid = -1;

	// 맵 정보 로드
	std::ifstream in("MapData.bin", std::ios::binary);
	in.read(reinterpret_cast<char*>(&m_map), sizeof(m_map));
	in.close();

	// 채팅 입력 상태 초기화
	m_bInputChat = false;
}

SceneManager::~SceneManager()
{

}

void SceneManager::ImageAlphaSetting(CImage* Img)
{
	unsigned char * pCol = 0;

	long w = Img->GetWidth();
	long h = Img->GetHeight();

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			pCol = (unsigned char *)
				Img->GetPixelAddress(x, y);

			unsigned char alpha = pCol[3];

			if (alpha != 255)
			{
				pCol[0] = (pCol[0] * alpha + 128) >> 8;
				pCol[1] = (pCol[1] * alpha + 128) >> 8;
				pCol[2] = (pCol[2] * alpha + 128) >> 8;
			}
		}
	}
}

void SceneManager::Render(HDC hViewdc)
{
	int MyPosX = m_cClients[m_Myid].posX;
	int MyPosY = m_cClients[m_Myid].posY;

	//////////////////// Map ///////////////////////////////

	for (int y = 0; y < 21; ++y)
	{
		int yy = y + MyPosY - 10;

		for (int x = 0; x < 21; ++x)
		{
			int xx = x + MyPosX - 10;

			if (yy < 0) continue;
			if (yy > 299) continue;
			if (xx < 0) continue;
			if (xx > 299) continue;

			m_iTile.AlphaBlend(
				hViewdc,
				x * m_nTile_Width,
				y * m_nTile_Height,
				m_nTile_Width,
				m_nTile_Height,
				(m_map[yy][xx] % 21) * m_nTile_Width,
				(m_map[yy][xx] / 21) * m_nTile_Height,
				m_nTile_Width,
				m_nTile_Height
			);
		}
	}

	//////////////////// Player & NPC ///////////////////////

	m_iPlayer.AlphaBlend(hViewdc,
		320, 320, m_nTile_Width, m_nTile_Height,
		0, 0, m_nTile_Width, m_nTile_Height);

	TCHAR playerHp[10];
	wsprintf(playerHp, L"%d", m_cClients[m_Myid].hp);
	TextOut(hViewdc, 320, 320, playerHp, lstrlen(playerHp));

	for (int i = 0; i < NUM_OF_USER; ++i)
	{
		if (i == m_Myid) continue;

		if (m_cClients[i].objectType == OT_USER)
		{
			m_iUser.AlphaBlend(hViewdc,
				(m_cClients[i].posX - MyPosX + 10) * m_nTile_Width,
				(m_cClients[i].posY - MyPosY + 10) * m_nTile_Height,
				m_nTile_Width, m_nTile_Height,
				0, 0, m_nTile_Width, m_nTile_Height);
			TCHAR UserHp[10];
			wsprintf(UserHp, L"%d", m_cClients[i].hp);
			TextOut(hViewdc, (m_cClients[i].posX - MyPosX + 10) * m_nTile_Width,
				(m_cClients[i].posY - MyPosY + 10) * m_nTile_Height, UserHp, lstrlen(UserHp));
		}
		
	}

	for (int i = 0; i < NUM_OF_NPC; ++i)
	{
		if (m_cNPCs[i].objectType == OT_MONSTER1)
		{
			m_iMonster1.AlphaBlend(hViewdc,
				(m_cNPCs[i].posX - MyPosX + 10) * m_nTile_Width,
				(m_cNPCs[i].posY - MyPosY + 10) * m_nTile_Height,
				m_nTile_Width, m_nTile_Height,
				0, 0, m_nTile_Width, m_nTile_Height);
		}
		else if (m_cNPCs[i].objectType == OT_MONSTER2)
		{
			m_iMonster2.AlphaBlend(hViewdc,
				(m_cNPCs[i].posX - MyPosX + 10) * m_nTile_Width,
				(m_cNPCs[i].posY - MyPosY + 10) * m_nTile_Height,
				m_nTile_Width, m_nTile_Height,
				0, 0, m_nTile_Width, m_nTile_Height);
		}
		else if (m_cNPCs[i].objectType == OT_MONSTER3)
		{
			m_iMonster3.AlphaBlend(hViewdc,
				(m_cNPCs[i].posX - MyPosX + 10) * m_nTile_Width,
				(m_cNPCs[i].posY - MyPosY + 10) * m_nTile_Height,
				m_nTile_Width, m_nTile_Height,
				0, 0, m_nTile_Width, m_nTile_Height);
		}
		else if (m_cNPCs[i].objectType == OT_MONSTER4)
		{
			m_iMonster4.AlphaBlend(hViewdc,
				(m_cNPCs[i].posX - MyPosX + 10) * m_nTile_Width,
				(m_cNPCs[i].posY - MyPosY + 10) * m_nTile_Height,
				m_nTile_Width, m_nTile_Height,
				0, 0, m_nTile_Width, m_nTile_Height);
		}

		if (m_cNPCs[i].objectType != OT_NONE)
		{
			TCHAR monsterHp[10];
			wsprintf(monsterHp, L"%d", m_cNPCs[i].hp);
			TextOut(hViewdc, (m_cNPCs[i].posX - MyPosX + 10) * m_nTile_Width,
				(m_cNPCs[i].posY - MyPosY + 10) * m_nTile_Height, monsterHp, lstrlen(monsterHp));
		}
	}

	//////////////////// Status ///////////////////////////////

	TCHAR posText[60];

	wsprintf(posText, L"Position : [ %d | %d ]", MyPosX, MyPosY);
	TextOut(hViewdc, 700, 50, posText, lstrlen(posText));
	
	TCHAR nameText[20];

	wsprintf(nameText, L"ID : %s", m_id_str);
	TextOut(hViewdc, 700, 100, nameText, lstrlen(nameText));

	TCHAR levelText[20];

	wsprintf(levelText, L"Level : %d", m_cClients[m_Myid].level);
	TextOut(hViewdc, 700, 150, levelText, lstrlen(levelText));

	TCHAR HpText[20];

	wsprintf(HpText, L"HP : %d", m_cClients[m_Myid].hp);
	TextOut(hViewdc, 700, 200, HpText, lstrlen(HpText));

	TCHAR ExpText[20];

	wsprintf(ExpText, L"Exp : %d", m_cClients[m_Myid].exp);
	TextOut(hViewdc, 700, 250, ExpText, lstrlen(ExpText));

	//////////////////// Chat   ///////////////////////////////

	if (m_bInputChat)
	{
		m_iChatIcon.AlphaBlend(hViewdc, 0, 832,
			m_nTile_Width, m_nTile_Height, 0, 0,
			m_nTile_Width, m_nTile_Height);
		
		TextOut(hViewdc, 50, 835, m_myChatStr, lstrlen(m_myChatStr));
	}

	for (int i = 0; i < 5; ++i)
	{
		if (lstrlen(m_ChatArray[i]))
		{
			TextOut(hViewdc, 50, 675 + (i * 32), m_ChatArray[i], lstrlen(m_ChatArray[i]));
		}
	}
}

void SceneManager::LoginOkey(sc_login_ok* pBuf)
{
	m_Myid = pBuf->myKey;

	m_cClients[pBuf->myKey].objectType = OT_USER;
	m_cClients[pBuf->myKey].posX = pBuf->x;
	m_cClients[pBuf->myKey].posY = pBuf->y;
	m_cClients[pBuf->myKey].level = pBuf->level;
	m_cClients[pBuf->myKey].exp = pBuf->exp;
	m_cClients[pBuf->myKey].hp = pBuf->hp;
}

void SceneManager::LoginFail(sc_login_fail* pBuf)
{

}

void SceneManager::PosUpdate(sc_position_info* pBuf)
{
	if (pBuf->objectType == OT_USER)
	{
		m_cClients[pBuf->targetKey].posX = static_cast<int>(pBuf->x);
		m_cClients[pBuf->targetKey].posY = static_cast<int>(pBuf->y);
	}
	else
	{
		m_cNPCs[pBuf->targetKey].posX = static_cast<int>(pBuf->x);
		m_cNPCs[pBuf->targetKey].posY = static_cast<int>(pBuf->y);
	}
}

void SceneManager::Chat(sc_chat* pBuf)
{
	for (int i = 0; i < 4; ++i)
	{
		wcscpy_s(m_ChatArray[i], m_ChatArray[i + 1]);
	}

	wcscpy_s(m_ChatArray[4], pBuf->chat_str);
}

void SceneManager::StatChange(sc_stat_change* pBuf)
{
	if (pBuf->objectType == OT_USER && m_cClients[pBuf->targetKey].objectType == OT_USER)
	{
		m_cClients[pBuf->targetKey].hp = pBuf->hp;
		m_cClients[pBuf->targetKey].exp = pBuf->exp;
		m_cClients[pBuf->targetKey].level = pBuf->level;
		m_cClients[pBuf->targetKey].objectType = pBuf->objectType;
	}
	else if (m_cNPCs[pBuf->targetKey].objectType != OT_NONE)
	{
		m_cNPCs[pBuf->targetKey].hp = pBuf->hp;
		m_cNPCs[pBuf->targetKey].exp = pBuf->exp;
		m_cNPCs[pBuf->targetKey].level = pBuf->level;
		m_cNPCs[pBuf->targetKey].objectType = pBuf->objectType;
	}
}

void SceneManager::RemoveObject(sc_remove_object* pBuf)
{
	if (pBuf->objectType == OT_USER)
	{
		m_cClients[pBuf->targetKey].objectType = OT_NONE;
	}
	else
	{
		m_cNPCs[pBuf->targetKey].objectType = OT_NONE;
	}
}

void SceneManager::AddObject(sc_add_object* pBuf)
{
	if (pBuf->objectType == OT_USER)
	{
		m_cClients[pBuf->targetKey].objectType = pBuf->objectType;
	}
	else
	{
		m_cNPCs[pBuf->targetKey].objectType = pBuf->objectType;
		m_cNPCs[pBuf->targetKey].hp = pBuf->hp;
	}
}

void SceneManager::ProcessKeyDown(wchar_t tChar)
{
	int len = lstrlen(m_myChatStr);

	if (len < MAX_STR_SIZE)
	{
		m_myChatStr[len] = tChar;
		m_myChatStr[len + 1] = 0;
	}
}

void SceneManager::PressBackspace()
{
	int len = lstrlen(m_myChatStr);

	if (0 < len)
	{
		m_myChatStr[len - 1] = 0;
	}
}