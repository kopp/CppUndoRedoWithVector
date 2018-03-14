# Undo Redo Framework

This is a sample implementation for a generic Undo-Redo-Framework using the
_Command_ pattern.
It is inspired by the one provided by
[Gerd Hirsch's Cpp-UndoRedoFramework](https://github.com/GerdHirsch/Cpp-UndoRedoFramework)
but should be more efficient when doing many undo/redo steps, because it does
not use two different `stack`s (one for undo and one for redo commands) and
pushes to the one what it popped from the first, but instead uses a `vector`
with all the known commands and a simple index (and flag) in the vector to the
next-to-redo command.


# Build

Build using `cmake` with the provided [`CMakeLists.txt`](CMakeLists.txt).

## Dependencies

- [Google Test](https://github.com/google/googletest)

