#pragma once

#define MAX_BUFF_SIZE 4000
#define MAX_PACKET_SIZE 256

#define BOARD_WIDTH 300
#define BOARD_HEIGHT 300

#define VIEW_RADIUS 49

#define NUM_OF_USER 50000
#define NUM_OF_NPC 800

#define MY_SERVER_PORT 4000

#define MAX_STR_SIZE 50

#define ID_STR_LENGTH 10

#define PT_RECV 0
#define PT_SEND 1
#define PT_NPC_MOVE 2
#define PT_NPC_ATTACK 3
#define PT_USER_HEAL 4

#define CS_LOGIN 1
#define CS_LOGOUT 2
#define CS_MOVE 3
#define CS_ATTACK 4
#define CS_CHAT 5

#define SC_LOGIN_OK 1
#define SC_LOGIN_FAIL 2
#define SC_POSITION_INFO 3
#define SC_CHAT 4
#define SC_STAT_CHANGE 5
#define SC_REMOVE_OBJECT 6
#define SC_ADD_OBJECT 7

#define OT_NONE 0
#define OT_USER 1
#define OT_MONSTER1 2
#define OT_MONSTER2 3
#define OT_MONSTER3 4
#define OT_MONSTER4 5

#pragma pack (push, 1)

struct cs_login
{
	unsigned char size;
	unsigned char type;
	wchar_t id_str[ID_STR_LENGTH];
};

struct cs_logout
{
	unsigned char size;
	unsigned char type;
};

struct cs_move
{
	unsigned char size;
	unsigned char type;
	unsigned char dir;
};

struct cs_attack
{
	unsigned char size;
	unsigned char type;
};

struct cs_chat
{
	unsigned char size;
	unsigned char type;
	wchar_t chat_str[MAX_STR_SIZE];
};

struct sc_login_ok
{
	unsigned char size;
	unsigned char type;
	int myKey;
	int level;
	int x;
	int y;
	int exp;
	int hp;
};

struct sc_login_fail
{
	unsigned char size;
	unsigned char type;
};

struct sc_position_info
{
	unsigned char size;
	unsigned char type;
	int targetKey;
	int objectType;
	int x;
	int y;
};

struct sc_chat
{
	unsigned char size;
	unsigned char type;
	int speakerKey;
	wchar_t chat_str[MAX_STR_SIZE];
};

struct sc_stat_change
{
	unsigned char size;
	unsigned char type;
	unsigned char id;
	unsigned char objectType;
	unsigned short hp;
	unsigned char level;
	unsigned long exp;
};

struct sc_remove_object
{
	unsigned char size;
	unsigned char type;
	int targetKey;
	int objectType;
};

struct sc_add_object
{
	unsigned char size;
	unsigned char type;
	int targetKey;
	unsigned char objectType;
	unsigned short hp;
};

#pragma pack (pop)