#!/bin/sh
# tools/hgss_repin.sh HASH [--secs N] [--worker ID]: re-pin HeartGold's submodule to HASH, regenerate only what changed,
# build, boot-test, commit (tools/hgss_repin.py says the steps and exit codes; docs/hgss.md "Re-pin")
cd "$(dirname "$0")/.." && exec nice -n 10 python3 tools/hgss_repin.py "$@"
