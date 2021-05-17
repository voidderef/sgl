#!/usr/bin/env bash

###
# Execute SGL in an endless loop
#
# Bootstraps SGL in a loop to ensure it gets restarted once the launched application exits.
###

# Root path is path of this script
ROOT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

xorg_conf=""

if [ "$1" ]; then
    xorg_conf="$1"

    if [ ! -e "$xorg_conf" ]; then
        echo "ERROR: Could not find xorg conf: $xorg_conf"
        exit 1
    fi
fi

# Remove any old/previously executed temp tools
rm -rf "/tmp/sgl_exec.sh"

cd "$ROOT_PATH" || exit 1

# Run loader
if [ "$xorg_conf" ]; then
    xinit "$ROOT_PATH/sgl.sh" -- /usr/bin/X -config "$xorg_conf"
else
    "$ROOT_PATH/sgl.sh"
fi

# Execute temp script which starts the selected game
if [ -f "/tmp/sgl_exec.sh" ]; then
    "/tmp/sgl_exec.sh"

	  # Start game loader again
	  exec bash "${BASH_SOURCE[0]}" "$@"
fi
