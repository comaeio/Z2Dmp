/*++
    Copyright (c) Comae Technologies DMCC. All rights reserved.
--*/


#include "stdafx.h"
#include "Z2Dmp.h"


static CONST ULONG Crc32Table[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};


ULONG
ComputeCrc32(
    _In_ ULONG Crc32,
    _In_ CONST PUCHAR Buffer,
    _In_ ULONG BufferLength
    )
{
    ULONG i = 0;

    Crc32 = ~Crc32;

    for (i = 0; i < BufferLength; i++) {

        Crc32 = Crc32Table[(Crc32 ^ Buffer[i]) & 0xFF] ^ (Crc32 >> 8);
    }

    return ~Crc32;
}

BOOL
DecompressDumpFile(
    _In_ PCTSTR CompressedDumpFile,
    _In_ PCTSTR DecompressedDumpFile
    )
{
    NTSTATUS Status;
    HANDLE hCompressedDumpFile = INVALID_HANDLE_VALUE;
    HANDLE hDecompressedDumpFile = INVALID_HANDLE_VALUE;
    PUCHAR Buffer = nullptr;
    PUCHAR CompressedBuffer = nullptr;
    PUCHAR UncompressedBuffer = nullptr;
    ULONG FinalUncompressedSize = 0;
    ULONG Crc32 = 0;
    ULONG NumberOfBytesRead = 0;
    ULONG NumberOfBytesToWrite = 0;
    ULONG NumberOfBytesWritten = 0;
    LARGE_INTEGER Frequency = { 0 };
    LARGE_INTEGER StartTime = { 0 };
    LARGE_INTEGER EndTime = { 0 };
    LARGE_INTEGER ElapsedTime = { 0 };
    ZDMP_FILE_HEADER DmpFileHeader = {0};
    ZDMP_BLOCK_HEADER DmpBlockHeader = {0};
    LARGE_INTEGER FileSize = {0};
    LARGE_INTEGER TotalNumberOfBytesWritten = {0};

    hCompressedDumpFile = CreateFile(CompressedDumpFile,
                                     GENERIC_READ,
                                     FILE_SHARE_READ,
                                     nullptr,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL,
                                     nullptr);

    if (hCompressedDumpFile != INVALID_HANDLE_VALUE) {

        if (ReadFile(hCompressedDumpFile, &DmpFileHeader, sizeof(DmpFileHeader), &NumberOfBytesRead, nullptr)) {

            if (DmpFileHeader.Signature == ZDMP_FILE_SIGNATURE) {

                if (DmpFileHeader.Version == ZDMP_FILE_VERSION_10) {

                    hDecompressedDumpFile = CreateFile(DecompressedDumpFile,
                                                       GENERIC_WRITE,
                                                       0,
                                                       nullptr,
                                                       CREATE_NEW,
                                                       FILE_ATTRIBUTE_NORMAL,
                                                       nullptr);

                    if (hDecompressedDumpFile != INVALID_HANDLE_VALUE) {

                        UncompressedBuffer = (PUCHAR)calloc(DmpFileHeader.BlockSize, sizeof(UCHAR));

                        if (UncompressedBuffer) {

                            CompressedBuffer = (PUCHAR)calloc(DmpFileHeader.BlockSize, sizeof(UCHAR));

                            if (CompressedBuffer) {

                                FileSize.QuadPart = DmpFileHeader.FileSize.QuadPart / (1024 * 1024);

                                SetFilePointer(hCompressedDumpFile, PAGE_SIZE, nullptr, FILE_BEGIN);

                                QueryPerformanceFrequency(&Frequency);

                                QueryPerformanceCounter(&StartTime);

                                while (ReadFile(hCompressedDumpFile, &DmpBlockHeader, sizeof(DmpBlockHeader), &NumberOfBytesRead, NULL)) {

                                    if (!NumberOfBytesRead) {

                                        break;
                                    }

                                    if (DmpBlockHeader.Signature != ZDMP_BLOCK_SIGNATURE) {

                                        _tprintf(_T("\nError: Invalid block signature.\n"));

                                        break;
                                    }

                                    if (DmpBlockHeader.DataSize > DmpFileHeader.BlockSize) {

                                        _tprintf(_T("\nError: Invalid block size.\n"));

                                        break;
                                    }

                                    if (!ReadFile(hCompressedDumpFile, CompressedBuffer, DmpBlockHeader.DataSize, &NumberOfBytesRead, nullptr)) {

                                        _tprintf(_T("\nError: Could not read input file. Error code: 0x%08X\n"), GetLastError());

                                        break;
                                    }

                                    if (!NumberOfBytesRead) {

                                        break;
                                    }

                                    Crc32 = ComputeCrc32(0, CompressedBuffer, DmpBlockHeader.DataSize);

                                    if (Crc32 != DmpBlockHeader.Crc32) {

                                        _tprintf(_T("\nError: Invalid block CRC.\n"));

                                        break;
                                    }

                                    if (DmpBlockHeader.DataSize != DmpFileHeader.BlockSize) {

                                        Status = RtlDecompressBuffer(DmpFileHeader.CompressionFormat | COMPRESSION_ENGINE_STANDARD,
                                                                     UncompressedBuffer,
                                                                     DmpFileHeader.BlockSize,
                                                                     CompressedBuffer,
                                                                     DmpBlockHeader.DataSize,
                                                                     &FinalUncompressedSize);

                                        if (!NT_SUCCESS(Status)) {

                                            _tprintf(_T("\nError: Could not decompress block. Status 0x%08X.\n"), Status);

                                            break;
                                        }

                                        Buffer = UncompressedBuffer;
                                        NumberOfBytesToWrite = FinalUncompressedSize;
                                    }
                                    else {

                                        Buffer = CompressedBuffer;
                                        NumberOfBytesToWrite = DmpBlockHeader.DataSize;
                                    }

                                    if (!WriteFile(hDecompressedDumpFile, Buffer, NumberOfBytesToWrite, &NumberOfBytesWritten, nullptr)) {

                                        _tprintf(_T("\nError: Could not write to output file. Error code: 0x%08X\n"), GetLastError());

                                        break;
                                    }

                                    TotalNumberOfBytesWritten.QuadPart += NumberOfBytesWritten;

                                    _tprintf(_T("Processed %I64d MB of %I64d MB\r"), TotalNumberOfBytesWritten.QuadPart / (1024 * 1024), FileSize.QuadPart);
                                }

                                QueryPerformanceCounter(&EndTime);

                                ElapsedTime.QuadPart = (EndTime.QuadPart - StartTime.QuadPart) / Frequency.QuadPart;

                                _tprintf(_T("\n\nTime elapsed: %I64d:%02I64d minutes:seconds\n\n"), ElapsedTime.QuadPart / 60, ElapsedTime.QuadPart % 60);

                                free(CompressedBuffer);
                            }
                            else {

                                _tprintf(_T("Error: Could not allocate memory.\n"));
                            }

                            free(UncompressedBuffer);
                        }
                        else {

                            _tprintf(_T("Error: Could not allocate memory.\n"));
                        }

                        CloseHandle(hDecompressedDumpFile);

                        if (TotalNumberOfBytesWritten.QuadPart != DmpFileHeader.FileSize.QuadPart) {

                            _tprintf(_T("Error: Could not decompress file.\n"));

                            // DeleteFile(DecompressedDumpFile);
                        }
                    }
                    else {

                        _tprintf(_T("Error: Could not create output file.\n"));
                    }
                }
                else {

                    _tprintf(_T("Error: Not supported file version.\n"));
                }
            }
            else {

                _tprintf(_T("Error: Invalid file signature.\n"));
            }
        }

        CloseHandle(hCompressedDumpFile);
    }
    else {

        _tprintf(_T("Error: Could not open input file.\n"));
    }

    return (TotalNumberOfBytesWritten.QuadPart == DmpFileHeader.FileSize.QuadPart);
}

BOOL
GetRoutine(
    VOID
    )
{
    HMODULE hModule;

    hModule = LoadLibrary(_T("ntdll"));

    if (hModule) {

        RtlDecompressBuffer = (PRTL_DECOMPRESS_BUFFER)GetProcAddress(hModule, "RtlDecompressBuffer");

        if (RtlDecompressBuffer) {

            return TRUE;
        }
    }

    return FALSE;
}

PCTSTR
GetFileExtension(
    _In_ PCTSTR FileName
    )
{
    PCTSTR Extension;

    Extension = _tcsrchr(FileName, L'.');

    if (Extension) {

        Extension += 1;

        return Extension;
    }

    return nullptr;
}

VOID
Help(
    VOID
    )
{
    _tprintf(_T("Usage: Z2Dmp <input file> <output file>\n\n"));

    _tprintf(_T("Description:\n")
             _T("  Decompresses Microsoft crash dump files.\n\n"));

    _tprintf(_T("Options:\n")
             _T("  <input file>      Source path of a compressed Microsoft crash dump file.\n")
             _T("  <output file>     Destination path of the decompressed Microsoft crash dump file.\n"));
}

int _tmain(int argc, TCHAR *argv[])
{
    PCTSTR FileExtension = nullptr;

    _tprintf(_T("\n")
             _T("  Z2Dmp %S\n")
             _T("  Copyright (C) 2017 - 2018, Comae Technologies DMCC <https://www.comae.io>\n\n"),
             COMAE_TOOLKIT_VERSION);

    if (argc < 3) {

        Help();

        return 1;
    }

    FileExtension = GetFileExtension(argv[2]);

    if (!FileExtension) {

        _tprintf(_T("Error: Could not get output file extension.\n"));

        return 1;
    }

    if (0 != _tcsicmp(FileExtension, _T("dmp"))) {

        _tprintf(_T("Error: Output file extension should be '.dmp'.\n"));

        return 1;
    }

    if (GetRoutine()) {

        if (!DecompressDumpFile(argv[1], argv[2])) {

            return 1;
        }
    }

    return 0;
}
