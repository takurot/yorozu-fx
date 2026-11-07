# Resolve host resource inventory

Recorded on 2026-11-08 JST against base commit `78b2029f81e3da1c9d37311329472a810c7adc28`.
The requirements come from [SPEC §5.1 and §12.3](../SPEC.md) and [TASK_PLAN P02 and P05](../TASK_PLAN.md).

P02 remains OPEN/HOLD. Actual access and startup are UNVERIFIED for all four required host combinations. P05 is not evaluated. This inventory does not authorize P03 or establish G0 acceptance.

## Required hosts

Each row identifies a required OS, GPU backend, and Studio major version. Unknown means no evidence has been supplied or inspected. It does not mean the resource is absent elsewhere.

| ID | Required OS and GPU | Required Studio | Target machine, OS patch, GPU, driver | Exact Studio patch | Validation location and access | Owner | Startup evidence | Status |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| MAC-20 | macOS Apple Silicon, Metal | 20 | Unknown | Unknown | Unknown | Unassigned | None recorded | UNVERIFIED |
| MAC-21 | macOS Apple Silicon, Metal | 21 | Unknown | Unknown | Unknown | Unassigned | None recorded | UNVERIFIED |
| WIN-20 | Windows x64, NVIDIA CUDA | 20 | Unknown | Unknown | Unknown | Unassigned | None recorded | UNVERIFIED |
| WIN-21 | Windows x64, NVIDIA CUDA | 21 | Unknown | Unknown | Unknown | Unassigned | None recorded | UNVERIFIED |

No Windows host was inspected. Native Windows build CI establishes build evidence only. It does not establish NVIDIA hardware, CUDA handoff, a Studio installation, or access to Resolve.

## Observed local prerequisites

The [command receipt](evidence/2026-11-08-local-prerequisites.txt) records read-only checks on the local development machine. Its UTC timestamp falls on 2026-11-07, corresponding to 2026-11-08 JST.

| Observation | Recorded result | Evidence limit |
| --- | --- | --- |
| Architecture and processor | arm64, Apple M1 | No target host assigned |
| Memory | 8589934592 bytes, 8 GiB | No performance equivalence approved |
| OS | macOS 27.2, build 26B123 | No Studio patch identified |
| GPU | Apple M1, 8 cores, Metal 4 reported | No OFX or Metal execution measured |
| Selected developer tools | `/Library/Developer/CommandLineTools` | Full Xcode was absent at `/Applications/Xcode.app` only |
| Metal compiler tools | `xcrun --find metal` and `xcrun --find metallib` both exited 72 | Tools were not found with the selected developer tools |
| Checked Resolve paths | `/Applications/DaVinci Resolve/DaVinci Resolve.app` and `/Applications/DaVinci Resolve.app` both absent | Other locations were not searched. No startup attempted |

These observations do not establish the availability of any required host. Alternate installations, remote machines, and resource owners remain unknown.

## Performance reference candidates

[SPEC §12.3](../SPEC.md) names these initial candidates. Availability and reservation remain unverified.

| Platform | Specified candidate | Availability | Equivalence approval |
| --- | --- | --- | --- |
| macOS | MacBook Pro M2 Pro, 32 GB | Unknown | Named candidate, reservation unknown |
| Windows | Windows 11, RTX 3060 12 GB, RAM 32 GB | Unknown | Named candidate, reservation unknown |

The observed M1 with 8 GiB is a development machine. It is not an approved performance reference. A replacement requires documented equivalence and approval before measurement. A measurement record must include OS, Studio patch, driver, and power mode.

## Restart contract

P02 can resume host verification when each row has an authorized validation location or controlled access path, an owner, and an evidence location. The handoff must identify the machine, exact OS and Studio patch versions, GPU, driver, and access and startup receipts. Host reservations and planned verification dates are unassigned.

The [resource ledger](resource-ledger.md) records the other required owners, dates, and rights terms. The handoff contains locations and evidence paths, never credentials or secrets.

P02 cannot close while mandatory host access remains unverified. P05 additionally requires the P01 through P04 evidence for load, save, reload, Undo, file operations, and GPU handoff on all four hosts. P05 also requires G1 machines and a color author to be available. Startup alone does not satisfy that gate.
