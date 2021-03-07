#include <YSI_Coding\y_hooks>

enum PLAYER_DATA {
    ID,
    Name[MAX_PLAYER_NAME],
    Password[129],
    DateReg[11],
    TimeReg[9],
    IP[16]
};
new playerData[MAX_PLAYERS][PLAYER_DATA];

hook OnPlayerConnect(playerid) {
	static const empty_player[PLAYER_DATA];
	playerData[playerid] = empty_player;
    return 1;
}