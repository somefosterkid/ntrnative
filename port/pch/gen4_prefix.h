/* The PCH header of the Gen 4 and Black 2 / White 2 targets (port/meson.build "The Gen 4 and Black 2"): empty. What goes into their
 * .pch is the target's own `-include` list (port_plat.h, port_hgss.h, port_b2w2.h ... : each pulls in what its game needs; Black's
 * port_prefix.h is not theirs), which clang compiles before this header. */
