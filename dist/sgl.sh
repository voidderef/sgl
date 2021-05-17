#!/usr/bin/env bash

###
# Execute SGL using this script
#
# Bootstraps SGL with local modules and settings lua
###

# Root path is path of this script
ROOT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

__GL_SYNC_TO_VBLANK=1 LD_LIBRARY_PATH="${ROOT_PATH}/modules" "${ROOT_PATH}/sgl" "data/settings.lua"
