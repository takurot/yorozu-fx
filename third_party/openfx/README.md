# OpenFX SDK provenance

The unmodified public `include/` tree and `LICENSE.md` come from Academy Software Foundation OpenFX release 1.5.1.

- Source repository: https://github.com/AcademySoftwareFoundation/openfx
- Source commit: `ab779510b2655b4d11a7e01e5c521f9aa8c88976`
- Annotated release tag object: `43d93ea99255cc61177b0632e421e899e802995e`
- License: BSD-3-Clause, preserved in `LICENSE.md` and each header.

The import verified all 28 files against their Git blob identities from the source commit's recursive tree. `SHA256SUMS` records the imported bytes. Builds use these local headers without network access or the upstream CMake project.

To update the SDK, select a reviewed release commit, copy its complete public include tree and license unchanged, and compare each file with that commit. Regenerate `SHA256SUMS` with SHA256 hashes relative to this directory. Run CTest and native builds on both supported platforms. The bundle includes the unchanged license under `Contents/Resources/openfx-LICENSE.md`.
