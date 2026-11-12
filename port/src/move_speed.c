// The player's walk, run and Surf speeds (the maintainers' notes, "Walk, run and Surf speed"): 1 = the game's,
// 2 or 4 = that many times faster. Only the player controller's own steps change (port/extern/src/ov021/
// field_player_grid.c); NPCs, scripts and the bike keep the game's commands. Frames a tile at 60 Hz: walk 16, 8, 4;
// run and Surf 8, 4, 2 (the 4x run and Surf are 1 game frame a tile, faster than the bike); uphill walk 24, 12, 6,
// uphill run 12, 6, 4; the turn in place before a walk 8, 4, 4. On rails the run and Surf stop at 4 (their 2x).
// The settings WalkSpeed, RunSpeed and SurfSpeed (sim_config.ini, BWPORT_WALKSPEED / RUNSPEED / SURFSPEED, cfgscript
// walkspeed= / runspeed= / surfspeed=, SIM_UISettingGet "walkspeed" ...) hold 1, 2 or 4; each step reads them, so a
// change applies at once.
extern int SIM_UISettingGet(const char *key);

int PortMoveSpeed_Walk(void) {
    return SIM_UISettingGet("walkspeed");
}

int PortMoveSpeed_Run(void) {
    return SIM_UISettingGet("runspeed");
}

int PortMoveSpeed_Surf(void) {
    return SIM_UISettingGet("surfspeed");
}
