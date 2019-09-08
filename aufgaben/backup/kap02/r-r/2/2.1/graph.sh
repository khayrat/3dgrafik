#!/bin/bash
set -Eeou pipefail

usage() {
	cat << EOF
	$self <fun> [fun-file]
EOF
	exit 1
}

init() {
	dir=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
	self=$(basename $0)

	if [[ $# < 1 ]]; then usage; fi

	f=$1 && shift

	f_file=${1:-exercises_4_1.txt}
}

init "$@"
./perl/gen_fun.sh -d $f -f $dir/$f_file -t $dir/application.cpp.in
