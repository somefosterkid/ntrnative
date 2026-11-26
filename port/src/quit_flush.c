// What libntr's SIM_Quit (sim_main.cpp) closes for the port before _Exit: the sound capture and the acore wav, which an atexit
// handler closed while exit() still ran (SIM_Quit skips atexit on purpose: the other threads are still alive then).
extern void acapture_close(void);
extern void ab_wav_close(void);

void bwport_quit_flush(void) {
    acapture_close();
    ab_wav_close();
}
