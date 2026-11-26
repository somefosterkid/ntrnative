// "Save first" (port/src/save_first.c, docs/save-first.md): the calls between the exe's request and the games' frame hooks
// (Gen 4: port/game/save_gen4.c; Gen 5: port/extern/src/system/game_event.c and port/extern_b2w2's), and the UI's.
#ifndef PORT_SAVE_FIRST_H
#define PORT_SAVE_FIRST_H

// The game's hook, each frame it runs: whether the game's own menu would offer Save now (the button's state)
void PortSaveFirst_Can(int can);
// The game's hook, when it is free to save now: 1 = a save is asked for, and it is the hook's to do from here
int PortSaveFirst_Take(void);
// The game's hook, at the save's end: saved = the game's own result (1 written); why says what failed
void PortSaveFirst_Done(int saved, const char *why);

// The UI: whether the game could save now (the hook said so in the last few V-blanks)
int bwport_save_first_possible(void);
// The UI: the game's save, then the leave (then 0 = Restart to title, 1 = Games). 0 when the game can't save here (nothing
// happens), 1 when it is on its way
int bwport_save_first(int then);
// The UI: 0 nothing asked, 1 waiting for the game to be free, 4 writing, 2 saved (the leave is pressed), 3 not saved (nothing
// left: the game goes on)
int bwport_save_first_state(void);
// The UI: drops a request the game has not begun (a save being written ends, and the leave follows)
void bwport_save_first_cancel(void);

#endif
