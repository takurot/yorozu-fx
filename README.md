# yorozu-fx

A planned OpenFX color-grading plugin for DaVinci Resolve Studio: blend original looks, preserve selected colors, and reproduce the result with versioned recipes.

## Project status

**Specification stage.** This repository currently contains product planning and documentation. There is no plugin implementation, installer, or downloadable release yet. Features, platform support, and performance targets below are planned and require validation before release.

## Product direction

yorozu-fx focuses on three tasks: **mix, preserve, and reproduce**.

- **Mix original looks:** choose up to three looks from six original bases and adjust their relative weights.
- **Separate color and tone:** control chromatic changes independently from tonal changes.
- **Preserve selected color ranges:** reduce look-induced changes to neutrals, skin-tone ranges, and one custom color range.
- **Explore reproducibly:** generate three deterministic variations while keeping protection settings and locked weights unchanged.
- **Reuse versioned recipes:** save settings with explicit engine and look-pack versions.
- **Export validated LUTs:** generate 33- or 65-point `.cube` files with error measurements and color-space metadata.

Color protection uses pixel color ranges, not face detection, object recognition, or tracking. It can also affect other objects with similar colors. A shared recipe does not automatically match different shots or cameras.

## Planned v1 scope

| Area | Target |
| --- | --- |
| Plugin format | OpenFX, used on the Resolve Color page |
| Host | DaVinci Resolve Studio 20 and 21, subject to compatibility testing |
| macOS | Apple Silicon with Metal |
| Windows | x64 with NVIDIA CUDA |
| Working space | DaVinci Wide Gamut / DaVinci Intermediate, with the same input and output space |
| Finishing evaluation | SDR, Rec.709 Gamma 2.4 |
| Settings | Static look settings in v1 |
| Recipe format | `.yorozu-fxrecipe.json` |

HDR delivery, native ACES workflows, other NLEs, Linux, iPad, and the free edition of Resolve are outside the initial supported scope. Working in a wide-gamut space does not imply validated HDR support.

Resolve remains responsible for media handling, camera input transforms, exposure and white balance, scopes, tracking, output transforms, and video export. Grain, halation, and other spatial or temporal effects are outside the initial plugin scope.

## Intended workflow

1. Configure input color management and balance exposure and white balance in Resolve.
2. Place yorozu-fx in a DWG/DaVinci Intermediate node domain, before the output transform.
3. Select and blend looks, then adjust color, tone, and protection settings.
4. Check representative shots using Resolve's comparison tools.
5. Save the project and, when needed, export a recipe or a validated LUT.

Exported LUTs use DWG/DaVinci Intermediate input and output values. They do not include camera input or display output transforms and must be applied in the specified node domain. See the specification for input-domain limits and validation requirements.

## Documentation

- [Product specification](docs/SPEC.md) — the authoritative specification, written in Japanese. Includes the concept review, competitor research, evaluation of 12 differentiation ideas, selected features, technical contracts, acceptance criteria, and delivery gates.
- [Task plan](docs/TASK_PLAN.md) — 31 tasks with dependencies, deliverables, acceptance criteria, and release gates, written in Japanese.
- [Task plan review](docs/TASK_PLAN_REVIEW.md) — independent sub-agent review results and the resulting clarification.
- [Original concept](docs/Color_Mixing_Studio_Functional_Spec_v1.0.docx.md) — the earlier web-first concept retained for context. The product specification takes precedence for plugin development.

## Development roadmap

1. Validate OFX host integration, persistence, and CPU/Metal/CUDA rendering.
2. Establish a CPU reference, three initial looks, and color-protection behavior.
3. Complete six original looks, the mixer, and deterministic exploration.
4. Implement recipe portability and LUT validation.
5. Run visual, compatibility, performance, and user-workflow evaluations.
6. Prepare signed distribution packages after all required release gates pass.

Build and installation instructions will be added when an implementation is available. The [specification](docs/SPEC.md) defines the required evidence for each stage.
