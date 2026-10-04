# libvpx (vendored)

VP8 encoder/decoder for the History video storage (matches the original
Kickidler node's `video_sequence` / `video_frame` model). Used only by
KikiHost (it encodes captured frames to VP8 and decodes them back to JPEG
when a viewer requests a history frame; the viewer itself is unchanged).

- Version: 1.17.0, mingw-w64-x86_64 build (msys2 package
  `mingw-w64-x86_64-libvpx-1.17.0-1`), static `libvpx.a`.
- Toolchain: MinGW GCC 13 (C:/Qt/Tools/mingw1310_64), the same one that
  builds KikiHost. Verified encode+decode round-trip links and runs.
- Link with `-lpthread` (see CMakeLists.txt).
