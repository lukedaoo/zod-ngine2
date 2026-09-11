# Coding Convention

- **Functions/methods**: `snake_case` — e.g. `read_file()`, `zero_memory()`
- **Enums, constants, `#define` values**: `ALL_UPPER_CASE` — e.g. `MAX_STRING_CHARS`, `ZERO_MEMORY`
- **`const`/`constexpr`**: use whenever a value doesn't change after init — prefer `constexpr` over `#define` for typed constants
- **File names**: `snake_case` — e.g. `sys_assert.h`, `zarray.h`
- **Types/classes**: `PascalCase`, except the `z` project prefix stays lowercase — e.g. `zArray`, `zArray2D`, `TestRunner`
- **Aggregate init**: use designated initializers (`{.field = value, ...}`), not positional — self-documenting, and immune to breaking silently if the struct's field order ever changes
