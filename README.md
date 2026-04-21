# meta_rm

`meta_rm` is a compile-time **meta model of relations** for C++.

The project starts from a simple thesis:

- every meaningful entity is represented as a **type**;
- a ternary node `rm<S, R, O>` is the canonical construction form;
- `S` is the **subject**: the target / receiver / representation;
- `R` is the **relation**: the controller / metafunction that decides how `O` is applied to `S`;
- `O` is the **object**: the value-class / payload / attached type / branch object / iterable object.

The result of an `rm<S, R, O>` node is intentionally exposed as a **variadic template alias**:

```cpp
using Result = rm<S, R, O>::type<>;
```

That rule is important because the final meaning of the resulting type is still expected to evolve. It should be possible to pass additional heirs, bases, payload types, or other relation-specific arguments through `::type<...>`.

## Core idea

`meta_rm` is not just a collection of template tricks. It is intended to become a language kernel for building large **ternary trees** of types.

A simple example:

```cpp
rm<
  rm<
    rm<Subject, inherit_r, Mix1>,
    inherit_r,
    Mix2
  >,
  inherit_r,
  Mix3
>
```

This tree says that `Mix1`, `Mix2`, and `Mix3` are related to `Subject` through the `inherit_r` relation, and the full tree can be introspected structurally.

## Design constraints fixed from the start

### 1. Full ternary introspection

Inside every `rm<S, R, O>` node the three parts are repeated explicitly:

- `using S = S_;`
- `using R = R_;`
- `using O = O_;`

This makes it possible to read and analyze any ternary tree purely at the type level.

### 2. Primitive metadata

Primitive entities carry metadata:

- `name`
- `description`

This allows the whole tree to remain readable as a semantic structure, not just as syntax.

### 3. Relation-driven semantics

`rm` itself does not decide what a node means. The relation does.

Each relation implements a protocol like:

```cpp
template <typename Subject, typename Object, typename... Extra>
struct apply;
```

So one relation may:

- make the subject inherit the object;
- run a loop in the context of the subject over the object-as-array;
- implement an `if` that compares the subject and returns either `S`, `O`, or a type from inside `O`.

### 4. Large trees first, surface syntax later

The first goal is to stabilize the **kernel**:

- metadata,
- primitive entities,
- relation protocol,
- ternary node structure,
- nested tree construction,
- introspection.

Only after that should the project grow more expressive surface layers and domain-specific vocabularies.

## Current bootstrap contents

The initial package contains:

- `fixed_string` for compile-time string metadata;
- `meta_info<Name, Description>`;
- primitive entities:
  - `symbol<Meta>`
  - `literal<V, Meta>`
  - `ref<Target, Meta>`
  - `entity<Meta, Heirs...>`
- relation base:
  - `relation<Meta>`
- ternary node:
  - `rm<S, R, O>`
- helpers:
  - `resolve<T>`
  - `node_meta<T>`
  - `node_children<T>`
  - `is_rm_v<T>`
- example relations:
  - `inherit_r`
  - `if_r` with a very small bootstrap `branch<...>` protocol.

## What is intentionally still open

The project has not yet frozen the full meaning of:

- streaming;
- currying;
- loop relations;
- condition protocols richer than a literal `true/false`;
- general subject rebinding beyond the bootstrap `entity<Meta, Heirs...>` primitive;
- domain vocabularies, including the future PMM construction language.

This is deliberate. The current package is meant to preserve the main idea and provide a stable base for planning issues and incremental implementation.

## Build

Requirements:
- CMake 3.20 or newer
- A C++20 compiler (tested with `g++` 13 and `clang++` 17)

Configure and build:

```bash
cmake -S . -B build
cmake --build build
```

## Run tests

Tests use [Catch2 v3](https://github.com/catchorg/Catch2) and are registered
with CTest. Catch2 is fetched automatically by CMake during configuration,
so no extra setup is required.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

To run the test executable directly (for example, to use Catch2 filters):

```bash
./build/tests/meta_rm_tests --list-tests
./build/tests/meta_rm_tests "[inherit]"
```

Tests can be disabled with `-DMETA_RM_BUILD_TESTS=OFF` when configuring.

## Adding new tests

Drop a new `.cpp` file into `tests/` that includes
`<catch2/catch_test_macros.hpp>` and add it to the `meta_rm_tests` target in
`tests/CMakeLists.txt`. Each file should focus on one relation or one
feature of the kernel. Prefer `STATIC_REQUIRE` for type-level assertions so
failures surface at compile time.

## Continuous integration

GitHub Actions runs the build and the full `ctest` suite on every push and
pull request, using both `g++` and `clang++` on `ubuntu-latest`. See
[`.github/workflows/ci.yml`](.github/workflows/ci.yml).

The pipeline follows the AI-driven development best practices from
[`link-assistant/hive-mind`](https://github.com/link-assistant/hive-mind/blob/main/docs/BEST-PRACTICES.md):
a `detect-changes` gate skips irrelevant jobs, `merge-check` simulates a
fresh merge against the base branch, `lint` enforces per-file line limits,
`docs` checks Markdown for broken links, and `build-and-test` only runs
after fast checks succeed. See [`CONTRIBUTING.md`](CONTRIBUTING.md) for
details on the full workflow and how to reproduce each gate locally.

## First milestone intention

The first milestone is to keep `meta_rm` small but structurally correct:

- `rm` as the canonical ternary node,
- all primitives metadata-rich,
- all trees fully introspectable,
- relation semantics isolated behind `apply`,
- nested trees compiling cleanly.
