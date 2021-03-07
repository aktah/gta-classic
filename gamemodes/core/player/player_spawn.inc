#include <YSI_Coding\y_hooks>

static
    const Float:SpawnPoints[][] = {
    {810.63520, -1340.0682, 13.5386, 37.33070}
};

hook OnPlayerLogin(playerid)
{
    defer ForceSpawnPlayer(playerid);
    return 1;
}

timer ForceSpawnPlayer[50](playerid) {
	// using SetSpawnInfo fixes the kick bug when
	// TogglePlayerSpectating is used to force spawn player but without using
	// SetSpawnInfo the server does not know where to spawn the player.
	SetSpawnInfo(playerid, 0, 264, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
	TogglePlayerSpectating(playerid, false);
}

hook OnPlayerSpawn(playerid) {
    SetPlayerPos(playerid, SpawnPoints[0][0], SpawnPoints[0][1], SpawnPoints[0][2]);
    SetPlayerFacingAngle(playerid, SpawnPoints[0][3]);
    printf(
    "\
        UID: %d | DATE: %s | TIME: %s | IP: %s\
    ", 
        Player_GetAccountID(playerid), 
        Player_GetDateReg(playerid), 
        Player_GetTimeReg(playerid), 
        Player_GetIPReg(playerid)
    );
    return 1;
}