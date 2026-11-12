# pokebw2 (vendored)

The C of Pokémon Black 2 and White 2 for the port: a copy of [pokebw2](https://github.com/fuddlesworth/pokebw2), a
matched decompilation of both games by its contributors, under its license (GPL-3.0, `LICENSE`), at commit
`921123bed5426a59de3e79a6912936be2ed7cd70`.

Patched for the port: `port/subprojects/pokebw2-*.patch` (each says what it fixes and why). Changed beyond them:

- Only what the port's build reads: `src`, `include`, the headers of eight libraries (`lib/*/include`, with
  `lib/poke_net/src` and `lib/spl/src`), each version's `config/<version>/arm9` `delinks.txt` and `symbols.txt`, and
  the generated headers the C includes (`generated/`, written by pokebw2's `tools/data/gen_constants.py`,
  `tools/text/text_ids.py` and `tools/text/pms_words.py`).
- The message-ID names carry no game text: `tools/msgid_neutral.py` renamed them to the bank and the message index.
- The tables that are the games' data are not here: each such array includes `asset/<name>.inc`, which the port's
  build writes from the player's own ROM (`config/b2_us/assets.tsv`, `config/w2_us/assets.tsv`, `tools/asset.py`).

`tools/pokebw2_fetch.sh --vendor` writes this tree again from pokebw2 (docs/black2-white2.md, "The vendored tree").
