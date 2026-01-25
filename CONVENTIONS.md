# C Conventions

## Naming

- Prefix: short, no underscores, and match the symbol's case (e.g. namespaces,
  scopes).
- Private symbols: `__`, two underscores between prefix and symbol name.
- Public symbols: `_`, one underscore between prefix and symbol name.
- Files: `snake_case`.
- Folders: `snake_case`.
- Macros: `SCREAMING_SNAKE_CASE`.
- Constants: `SCREAMING_SNAKE_CASE`.
- Structs: `snake_case`.
- Struct members: `snake_case`.
- Unions: `snake_case`.
- Enums: `snake_case`.
- Enum members: `SCREAMING_SNAKE_CASE`.
- Functions: `snake_case`.
- Parameters: `snake_case`.
- Variables: `snake_case`.
- Typedefs: `snake_case`.

**Notes:**

- Do not use `__` or `_` prefix for variables, functions, macros and types [^1].
  They are used by the compiler and standard library. Here's what [the C
  standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) says
  (section 7.1.3) [^2]:

  > - All identifiers that begin with an underscore and either an uppercase
  >   letter or another underscore are always reserved for any use.
  > - All identifiers that begin with an underscore are always reserved for use
  >   as identifiers with file scope in both the ordinary and tag name spaces.

- Do not use `typedef` for structures and pointers [^3].

[^1]: [General rules | MaJerle/c-code-style: Recommended C style and coding
    rules](https://github.com/MaJerle/c-code-style?tab=readme-ov-file#general-rules)
[^2]: [use _ and __ in C programs](https://stackoverflow.com/a/25090719)
[^3]: [Typedefs | Linux kernel coding
    style](https://www.kernel.org/doc/html/latest/process/coding-style.html#typedefs)
