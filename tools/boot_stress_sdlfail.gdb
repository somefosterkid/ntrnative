# tools/boot_stress_sdlfail.gdb: run the Linux exe with SDL_Init forced to fail, watch the window title's heap chunk header.
#   gdb -q -nx -batch -x tools/boot_stress_sdlfail.gdb --args build/port/before/bwport      (cwd: a run dir)
# The title is SIM_RenderInit's std::string (SDL_CreateWindow's first argument, $rdi); malloc keeps the chunk's size word at
# title-8. A buffer overrun by the audio callback of the device-rate open (libntr before the SIM_SdlInit change) shows as a changed
# size word, and glibc says "free(): invalid size" when the title is freed.
set pagination off
set confirm off
set breakpoint pending on
handle SIGPIPE SIGUSR1 SIGUSR2 SIGFPE nostop noprint pass
set $forced = 0
break SDL_Init
commands
  silent
  set $forced = $forced + 1
  printf "sdlfail: SDL_Init forced to return -1\n"
  return (int)-1
  continue
end
break SDL_OpenAudio
commands
  silent
  printf "sdlfail: SDL_OpenAudio entered (audio driver name %s)\n", (char *)SDL_GetCurrentAudioDriver()
  continue
end
break SDL_CreateWindow
commands
  silent
  set $title = (long *)$rdi
  printf "sdlfail: title %p, size word %lx, next chunk word %lx\n", $title, $title[-1], *(long *)((char *)$title + ($title[-1] & ~7) - 8)
  continue
end
break SDL_GL_SwapWindow
ignore 4 40
commands
  silent
  printf "sdlfail: frame 40: title %p, size word %lx\n", $title, $title[-1]
  kill
  quit
end
run
