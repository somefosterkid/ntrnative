#!/bin/bash
# Prints the path of the maintainer's base ROM: the main checkout's orig/baserom_b_us.nds (a worktree has no orig/).
ROOT=$(cd "$(dirname "$0")/.." && pwd)
echo "$(git -C "$ROOT" worktree list --porcelain | sed -n 's/^worktree //p;q')/orig/baserom_b_us.nds"
