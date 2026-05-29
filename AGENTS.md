# util — Agent Instructions

Canonical repository instructions for AI coding agents. Keep this file as the
source of truth and keep tool-specific files thin.

## 1. Project overview

This repository contains `mb.util`, a header-only C++ utility library (`mb::util`, include entrypoint `mb/ul/ul.hpp`) with:

- library headers in `include/mb/ul/**`
- tests and compile checks in `src/**`
- one example target in `examples/`
- project CMake helpers in `cmake/`
- CI workflow wrappers in `.github/workflows/` that call reusable workflows from the `devenv/` git submodule

`devenv/` is a submodule and has its own agent config (`devenv/AGENTS.md`,
`devenv/CLAUDE.md`). When changing files under `devenv/**`, apply those nested
rules as well.

## 2. Build commands

Verified in this repo:

- List configure presets: `cmake --list-presets`
- Configure: `cmake --preset <configure-preset>`
- Build: `cmake --build --preset <build-preset>`
- End-to-end (configure + build + test): `cmake --workflow --preset <workflow-preset>`

Common preset names from `CMakePresets.json`:

- `gcc-debug`, `gcc-release`
- `clang-debug`, `clang-release`
- `clang-libc++-debug`, `clang-libc++-release`
- `appleclang-debug`, `appleclang-release`, `appleclang-maxsan-release`
- `msvc-debug`, `msvc-release`

Local-only presets may exist in `CMakeUserPresets.json` (for example `appleclang-debug-noasan`).

## 3. Test commands

Verified command shape:

- `ctest --preset <test-preset>`
- `cmake --workflow --preset <workflow-preset>`

Typical local loop:

1. `cmake --preset <preset>`
2. `cmake --build --preset <preset>`
3. `ctest --preset <preset>`

## 4. Formatting and linting

- Full lint/format sweep: `pre-commit run --all-files` (verified command available locally)
- clang-tidy helper (from submodule): `python3 devenv/scripts/clang-tidy-review.py changed` or `full`
  (unverified in this session)
- cppcheck helper (from submodule): `./devenv/scripts/run-cppcheck.sh [preset] [build-dir]` (unverified in this session)
- Workflow linting: `actionlint -color` (unverified in this session; not installed in current shell)

Primary config files:

- `.clang-format`
- `.clang-tidy`
- `.pre-commit-config.yaml`
- `.markdownlint.yaml`
- `.codespellrc`
- `CppCheckSuppressions.txt`

## 5. Architecture and important directories

- `include/mb/ul/**`: public header-only library surface
- `src/**`: tests (`*.test.cpp`) and compile-check target
- `examples/`: sample executable(s)
- `cmake/**`: project CMake modules/tooling
- `.github/workflows/**`: CI entrypoints (mostly `workflow_call` consumers from `devenv`)
- `fetchcontent-lockfile.json`: pinned FetchContent dependencies
- `devenv/**`: external shared tooling submodule boundary

## 6. Coding conventions

- Follow `.clang-format` (LLVM-based, 4-space indent, 120-column limit).
- Follow `.clang-tidy` policy; warnings are treated as errors.
- Keep naming aligned with current codebase conventions (notably lower_snake_case for most functions/variables).
- Keep edits scoped; avoid drive-by refactors unrelated to the task.
- Do not change public CMake option names or exported target names (`mb.util`, `mb::util`) without explicit request.

## 7. Versioning and releases

`CMakeLists.txt` `project(... VERSION X.Y.Z)` is the **single source of truth** for semver.
Git release tags use the `vX.Y.Z` prefix (for example `v0.1.0`). FetchContent lockfile consumers
should pin `git_tag` to that tag, not a bare commit SHA.

Release flow:

1. Bump `VERSION` in root `CMakeLists.txt` (semver: patch for fixes, minor for compatible
   features, major for breaking changes).
2. Merge to `main`. CI workflow `release-tag.yml` runs `scripts/sync-release-tag.py
   --push-if-missing` and creates/pushes `vX.Y.Z` when the tag does not exist yet.
3. Locally (optional): `python3 scripts/sync-release-tag.py --push-if-missing` after a version
   bump, or `python3 scripts/sync-release-tag.py --check` to verify the tag exists.

Installed/config packages expose `MB_UTIL_VERSION` from the same `PROJECT_VERSION`.

## 9. Testing expectations

Before finalizing non-trivial code changes:

1. Run `pre-commit run --all-files` for touched files.
2. Run at least one relevant configure/build/test preset loop.
3. If CMake, toolchain, or preset files changed, run a full preset workflow (`cmake --workflow --preset ...`) where possible.
4. If workflow files changed, run `actionlint` when available.

If any command cannot be run locally, state that clearly in the final response.

## 10. Files and directories agents must not edit without explicit approval

- `devenv/**` (submodule content and submodule pointer), unless task explicitly targets it
- `fetchcontent-lockfile.json` (dependency pin changes are high-impact)
- `CMakeUserPresets.json` (developer-local settings)
- generated/build outputs: `build/**`, `cmake-build-*/**`, `**/_deps/**`, `.venv/**`, `__pycache__/**`
- vendored/third-party SDK content under `src/string/sdks/**`
- CI/secrets-related workflow wiring unless requested, especially `.github/workflows/pre-commit-update.yml`

## 11. Security and privacy constraints

- Never commit secrets or tokens (for example workflow secrets used by Codacy/pre-commit-update).
- Treat `pull_request_target` workflow changes as security-sensitive; do not loosen permissions casually.
- Do not add new external network/dependency execution paths in CI without explicit justification.
- Keep dependency provenance explicit when changing submodule refs or lockfile pins.

## 12. Review checklist before final response

- [ ] Changes are scoped to the user request.
- [ ] Formatting/linting run, or skipped with reason.
- [ ] Build/test commands run, or skipped with reason.
- [ ] No edits to restricted files without explicit approval.
- [ ] `devenv/**` boundary handled correctly (and nested rules respected when touched).
- [ ] Risks/assumptions are called out when verification is partial.
