# Contributing to meta_rm

Thank you for considering a contribution! This document describes the local
workflow, CI gates, and where to add new code.

## Local workflow

### Requirements

- CMake 3.20 or newer
- A C++20 compiler — tested with `g++` 13 and `clang++` 17
- Ninja (optional, used by CI)

### Build and test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

Run a specific test with Catch2 filters:

```bash
./build/tests/meta_rm_tests --list-tests
./build/tests/meta_rm_tests "[inherit]"
```

Tests can be skipped at configure time with `-DMETA_RM_BUILD_TESTS=OFF`.
Examples can be skipped with `-DMETA_RM_BUILD_EXAMPLES=OFF`.

### Run the same lints CI runs

```bash
bash scripts/check-file-sizes.sh
```

## Project layout

```
include/meta_rm/   # public header-only library
examples/          # minimal usage examples
tests/             # Catch2 test suite, registered with CTest
scripts/           # CI helper scripts (file-size enforcement, etc.)
.github/workflows/ # CI pipeline
```

## Adding a test

1. Create a new `.cpp` file under `tests/`.
2. Include `<catch2/catch_test_macros.hpp>` and `<meta_rm/meta_rm.hpp>`.
3. Prefer `STATIC_REQUIRE` for type-level assertions — failures surface at
   compile time, which keeps regressions loud.
4. Add the file to the `meta_rm_tests` target in `tests/CMakeLists.txt`.
5. Keep one concern per file; small focused files are easier to review.

## Adding an example

1. Drop a new `.cpp` file under `examples/` with a `main()` function.
2. Wire it up as an `add_executable` in the root `CMakeLists.txt`, linking
   `meta_rm`.
3. Examples are for illustration — production assertions belong in tests.

## CI gates

Every push and pull request runs the pipeline at
[`.github/workflows/ci.yml`](.github/workflows/ci.yml). The pipeline follows
the AI-driven development best practices from
[`link-assistant/hive-mind`](https://github.com/link-assistant/hive-mind/blob/main/docs/BEST-PRACTICES.md):

- **detect-changes** — skips build/test when only docs change and vice versa
- **merge-check** — simulates a fresh merge against the base branch on PRs so
  stale PR snapshots fail early
- **lint** — enforces per-file line limits (code ≤ 1500, docs ≤ 2500)
- **docs** — runs a broken-link check on all Markdown files
- **build-and-test** — matrix build with `g++` and `clang++` on `ubuntu-latest`
  in Debug, executes `ctest --output-on-failure`
- **ci** — aggregates results so branch protection can require a single check

The pipeline uses `concurrency:` to cancel superseded runs on feature branches.

## Style

- Match the style of the surrounding code. `meta_rm` is a small template
  library; prefer clear names over clever tricks.
- Keep public headers in `include/meta_rm/` self-contained and documented via
  `meta_info<Name, Description>`.
- Don't silently grow the CI matrix (Windows/macOS, sanitizers, coverage) —
  those are deferred to follow-up issues.

## Reporting issues

See the templates in the
[hive-mind best practices](https://github.com/link-assistant/hive-mind/blob/main/docs/BEST-PRACTICES.md#writing-good-issues)
for guidance on writing actionable bug reports and feature requests.
