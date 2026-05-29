#!/usr/bin/env python3
"""Sync git release tag v{VERSION} with project(VERSION) in CMakeLists.txt.

CMakeLists.txt `project(... VERSION X.Y.Z)` is the single source of truth.
FetchContent lockfile consumers should pin `git_tag` to `vX.Y.Z`.
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

VERSION_LINE_RE = re.compile(r"^\s*VERSION\s+(\d+\.\d+\.\d+)\s*$", re.IGNORECASE)


def eprint(*args: object) -> None:
    print(*args, file=sys.stderr)


def run(
    command: list[str],
    *,
    check: bool = True,
    capture_output: bool = False,
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        check=check,
        text=True,
        capture_output=capture_output,
    )


def repo_root() -> Path:
    return Path(__file__).resolve().parent.parent


def read_project_version(cmake_lists: Path) -> str:
    lines = cmake_lists.read_text(encoding="utf-8").splitlines()
    in_project = False
    paren_depth = 0

    for line in lines:
        stripped = line.strip()
        if not in_project:
            if re.match(r"project\s*\(", stripped, re.IGNORECASE):
                in_project = True
                paren_depth = stripped.count("(") - stripped.count(")")
                version_match = VERSION_LINE_RE.match(line)
                if version_match:
                    return version_match.group(1)
            continue

        version_match = VERSION_LINE_RE.match(line)
        if version_match:
            return version_match.group(1)

        paren_depth += line.count("(") - line.count(")")
        if paren_depth <= 0:
            break

    raise SystemExit(f"Could not find project(VERSION ...) in {cmake_lists}")


def git_tag_name(version: str) -> str:
    return f"v{version}"


def resolve_tag_commit(tag: str) -> str | None:
    result = run(
        ["git", "rev-list", "-n", "1", tag],
        check=False,
        capture_output=True,
    )
    if result.returncode != 0:
        return None
    return result.stdout.strip()


def create_annotated_tag(tag: str, version: str, commit: str) -> None:
    message = f"Release {tag}\n\nTagged from CMakeLists.txt project(VERSION {version})."
    run(["git", "tag", "-a", tag, commit, "-m", message])


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--cmake-lists",
        type=Path,
        default=repo_root() / "CMakeLists.txt",
        help="Path to CMakeLists.txt containing project(VERSION ...).",
    )
    parser.add_argument(
        "--commit",
        default="HEAD",
        help="Commit to tag (default: HEAD).",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Verify tag v{VERSION} exists; do not create or push.",
    )
    parser.add_argument(
        "--push-if-missing",
        action="store_true",
        help="Create tag at --commit when missing, then push it.",
    )
    parser.add_argument(
        "--push",
        action="store_true",
        help="Push tag v{VERSION} to origin after creating it locally.",
    )
    args = parser.parse_args()

    version = read_project_version(args.cmake_lists)
    tag = git_tag_name(version)
    commit = run(["git", "rev-parse", args.commit], capture_output=True).stdout.strip()
    tagged_commit = resolve_tag_commit(tag)

    eprint(f"project VERSION {version} -> git tag {tag}")

    if tagged_commit:
        if tagged_commit == commit:
            eprint(f"ok: {tag} already points to {commit}")
            return 0
        if args.push_if_missing or args.check:
            eprint(
                f"ok: {tag} exists at {tagged_commit} "
                f"(HEAD is {commit}; bump VERSION for the next release)"
            )
            return 0
        eprint(
            f"error: {tag} already points to {tagged_commit}, "
            f"but requested commit is {commit}. Bump VERSION in CMakeLists.txt."
        )
        return 1

    if args.check:
        eprint(f"error: missing git tag {tag} for VERSION {version}")
        return 1

    create_annotated_tag(tag, version, commit)
    eprint(f"created {tag} at {commit}")

    if args.push or args.push_if_missing:
        run(["git", "push", "origin", tag])
        eprint(f"pushed {tag} to origin")

    return 0


if __name__ == "__main__":
    sys.exit(main())
