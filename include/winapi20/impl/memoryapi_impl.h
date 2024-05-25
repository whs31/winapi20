#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <iosfwd>
#include <winapi20/detail/export.h>
#include <winapi20/detail/definitions.h>
#include <winapi20/detail/template_util.h>

namespace winapi::memory
{
  enum class PageAccess : uint64_t
  {
    Readonly                  = 1 << 1,
    ReadWrite                 = 1 << 2,
    NoAccess                  = 1 << 0,
    WriteCopy                 = 1 << 3,
    Execute                   = 1 << 4,
    ExecuteRead               = 1 << 5,
    ExecuteReadWrite          = 1 << 6,
    ExecuteWriteCopy          = 1 << 7,
    Guard                     = 1 << 8,
    NoCache                   = 1 << 9,
    WriteCombine              = 1 << 10,
    GraphicsNoAccess          = 1 << 11,
    GraphicsReadOnly          = 1 << 12,
    GraphicsReadWrite         = 1 << 13,
    GraphicsExecute           = 1 << 14,
    GraphicsExecuteRead       = 1 << 15,
    GraphicsExecuteReadWrite  = 1 << 16,
    GraphicsCoherent          = 1 << 17,
    GraphicsNocache           = 1 << 18,
    EnclaveThreadControl      = 0x80000000,
    RevertToFileMap           = EnclaveThreadControl,
    TargetsNoUpdate           = 0x40000000,
    TargetsInvalid            = TargetsNoUpdate,
    EnclaveUnvalidated        = 0x20000000,
    EnclaveMask               = 0x10000000,
    EnclaveDecommit           = EnclaveMask,
    EnclaveSsFirst            = EnclaveMask | 1,
    EnclaveSsRest             = EnclaveMask | 2,
  };

  enum class MemoryOperation : uint64_t
  {
    Commit                    = 1 << 12,
    Reserve                   = 1 << 13,
    ReplacePlaceholder        = 1 << 14,
    Decommit                  = ReplacePlaceholder,
    Release                   = 1 << 15,
    Free                      = 1 << 16,
    Private                   = 1 << 17,
    ReservePlaceholder        = 1 << 18,
    Mapped                    = ReservePlaceholder,
    Reset                     = 1 << 19,
    TopDown                   = 1 << 20,
    WriteWatch                = 1 << 21,
    Physical                  = 1 << 22,
    Rotate                    = 1 << 23,
    DifferentImageBaseOk      = Rotate,
    ResetUndo                 = 1 << 24,
    LargePages                = 0x20000000,
    UnmapWithTransientBoost   = 1 << 0,
    CoalescePlaceholders      = UnmapWithTransientBoost,
    PreservePlaceholders      = 1 << 1
  };
}