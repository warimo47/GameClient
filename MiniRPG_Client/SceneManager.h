#pragma once

#include "../../../NeoplePortfolio/NeoplePortfolio/protocol.h"
#include <atlimage.h>
#include <iostream>
#include <fstream>

struct Client
{
	unsigned char objectType;
	int posX;
	int posY;
	int level;
	int exp;
	int hp;
};

class SceneManager
{
	// ��� Ÿ��
	CImage m_iTile;

	// �÷��̾�, �ٸ� ����
	CImage m_iPlayer;
	CImage m_iUser;

	// ����
	CImage m_iMonster1;
	CImage m_iMonster2;
	CImage m_iMonster3;
	CImage m_iMonster4;

	// ê�� Ȱ��ȭ ������
	CImage m_iChatIcon;

	// Ŭ���̾�Ʈ �迭
	Client m_cClients[NUM_OF_USER];

	// NPC �迭
	Client m_cNPCs[NUM_OF_NPC];

	// �� ID
	int m_Myid;

	// �� ����
	short m_map[300][300];

public:
	// ä�� �Է� ����
	bool m_bInputChat;

	// �� ID ���ڿ�
	wchar_t m_id_str[ID_STR_LENGTH];

	// �� ê�� ���� �� ���ڿ�
	wchar_t m_myChatStr[MAX_STR_SIZE];

	// ê�� â ���ڿ�
	wchar_t m_ChatArray[5][MAX_STR_SIZE];

	// ȭ�� ũ��, Ÿ�� ũ��
	int m_nWin_Width, m_nWin_Height;
	int m_nTile_Width, m_nTile_Height;

public:
	SceneManager();
	~SceneManager();

	void ImageAlphaSetting(CImage*);

	void Render(HDC);

	void LoginOkey(sc_login_ok*);
	void LoginFail(sc_login_fail*);
	void PosUpdate(sc_position_info*);
	void Chat(sc_chat*);
	void StatChange(sc_stat_change*);
	void RemoveObject(sc_remove_object*);
	void AddObject(sc_add_object*);
	void ProcessKeyDown(wchar_t tChar);
	void PressBackspace();
};