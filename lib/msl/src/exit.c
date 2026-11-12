// MSL's exit as Black has it, in an object of its own before abort_exit_arm_eabi.c (built with MWCC 2.0, library.toml's
// file_compiler): it only keeps whether its status was a failure, which nothing reads. The name is pokebw2's.

static _Bool __exit_failed;

void exit(int status) {
    __exit_failed = status != 0;
}
