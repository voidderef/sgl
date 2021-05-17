#!/usr/bin/env bash

set -eu

#
# This script goes through the defined external repositories and updates the local checkout.
# It is configrable via the */import.conf
#
# Usage: ./import.sh
#

export RESET_COLORS="\033[0m"
export FG_GREEN="\033[0;32m"
export FG_LIGHT_RED="\033[1;31m"
export FG_YELLOW="\033[0;33m"

echo_success()
{
    printf "${FG_GREEN}\xE2\x9C\x94 ${RESET_COLORS}$*\n"
}
echo_warn()
{
    printf "${FG_YELLOW}$*${RESET_COLORS}\n"
}
echo_error()
{
    printf "${FG_LIGHT_RED}$*${RESET_COLORS}\n"
}

get_checksum()
{
    dir="${1?Missing argument for checksum directory}"

    if [[ ! -d "${dir}" ]]; then
        exit 0
    fi

    # Get the md5 sum. On Mac it is md5, on linux it is md5sum
    find "${dir}" -type f -print0  | xargs -0 shasum -a1 | shasum -a1 | awk '{print $1}'
}

update_files()
{
    # Checkout the external repository and copy the files defined in the import.conf
    repo="${1?Missing argument for repository}"
    desired_commit="${2?Missing argument for commit}"

    # Load import configuration
    source "${repo}/import.conf"

    if [ "$type" = "git" ]; then
        git clone "${repository_uri}" "${repo}/checkout"
        git -C "${repo}/checkout" checkout "${commit_id}"
    elif [ "$type" = "hg" ]; then
        hg clone "${repository_uri}" "${repo}/checkout"
        cd "${repo}/checkout"
        hg update -r "${commit_id}"
        cd ../..
    else
        echo "ERROR: Unknown repository type $type"
        exit 1
    fi
}

import_repository()
{
    repo="${1?Missing argument for repository}"

    # Get desired commit_id
    unset commit_id
    source "${repo}/import.conf"
    conf_commit="${commit_id}"

    echo_warn "${repo}: Updating to ${conf_commit}..."
    update_files "${repo}" "${conf_commit}"

    # Commit the updated files if something has changed
    num_changed_files="$(git diff --name-only HEAD "${repo}" | wc -l)"
    if (( num_changed_files != 0 )); then
        echo_success "${repo}: Imported files were updated successfully. You can now push to remote."
    else
        echo_warn "${repo}: Files updated only locally, no changes to commit. Note: manual changes in ${repo} will be overwritten!"
        echo_success "${repo}: Files are up-to-date"
    fi
}

if [ ! "$1" ]; then
    echo "Usage: $0 <repository folder name>"
    exit 1
fi


# Change to script directory
old_pwd="$(pwd)"
echo "${old_pwd}"
cd "$(dirname $0)"
echo "$(pwd)"

repo="$1"

import_repository "${repo}"

cd "${old_pwd}"