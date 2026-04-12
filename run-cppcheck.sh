#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

preset="${1:-clang-release}"
build_dir="${2:-${repo_root}/build/${preset}}"
compile_commands="${build_dir}/compile_commands.json"
cppcheck_build_dir="${build_dir}/cppcheck"
suppression_file="${repo_root}/CppCheckSuppressions.txt"
cmake --preset "${preset}" -B "${build_dir}" -DMB_UL_CPPCHECK=ON -DMB_UL_CPPCHECK_AUTO_INSTALL=ON

if [[ ! -f "${compile_commands}" ]]; then
    echo "Expected compile database at '${compile_commands}' but it was not generated." >&2
    exit 1
fi

cppcheck_bin="$(python3 - "${build_dir}/CMakeCache.txt" <<'PY'
from pathlib import Path
import sys

cache_path = Path(sys.argv[1])
if not cache_path.is_file():
    raise SystemExit(0)

for line in cache_path.read_text().splitlines():
    if line.startswith("MB_UL_CPPCHECK_EXECUTABLE:FILEPATH="):
        print(line.split("=", 1)[1])
        break
PY
)"

if [[ -z "${cppcheck_bin}" || ! -f "${cppcheck_bin}" ]]; then
    echo "cppcheck is required but no executable was detected by CMake." >&2
    exit 1
fi

mkdir -p "${cppcheck_build_dir}"

"${cppcheck_bin}" \
    --project="${compile_commands}" \
    --cppcheck-build-dir="${cppcheck_build_dir}" \
    --enable=warning,style,performance,portability \
    --check-level=exhaustive \
    --error-exitcode=1 \
    --inline-suppr \
    --inconclusive \
    --library=googletest \
    --suppress=missingIncludeSystem \
    --suppressions-list="${suppression_file}" \
    --template=gcc \
    -i"${build_dir}/_deps"
