# zipper 

`zipper` creates a basic zip file

## notes

### struct padding

Be aware of struct padding (will pad to 4-byte values by default with an int in the struct)
This behavior can be modified by instructing the compiler to pad to 1 byte, e.g.

```c
// To force compiler to use 1 byte padding
#pragma pack(1)
```

### DOS dates and times

Dates and times are stored in MSDOS format.

- [DOS date and time format](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-dosdatetimetofiletime)

## crc32

zip files use a cyclic redundancy check (CRC) for error-detection and corruption.
This project uses crc32 from the zlib library.

- [crc](https://en.wikipedia.org/wiki/Cyclic_redundancy_check)
- [zlib](https://zlib.net/)

## Useful links

- [APPNOTE.TXT](https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT)
- [ZIP file format - wikipedia](https://en.wikipedia.org/wiki/ZIP_(file_format))
- [binary files in C](https://www.delftstack.com/howto/c/read-binary-file-in-c/)


