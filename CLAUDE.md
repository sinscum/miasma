# MIASMA: Dark Groovebox Firmware for the M-VAVE FM-1

Custom open-source firmware for the M-VAVE FM-1, built for **darksynth, horrorsynth, dark ambient, glitch and noise**, with **bright, hummable chiptune leads** in the style of indie RPG soundtracks.

MIASMA is a standalone project. It has no ties to any other app or project of the owner's; do not add integrations, names or export formats for anything outside this repo.

Deliverables:
1. **Firmware**: an 8-track, 64-step dark groovebox (`.fwsc` package).
2. **Play in browser**: the firmware compiled to WebAssembly on an on-screen FM-1.
3. **Web editor**: live sound and pattern editing over USB MIDI, backup/restore, install, case export.
4. **Cheat sheet**: a printable one-page reference.

All four live in one GitHub repo (owner's account), with GitHub Pages serving the web pages. Submit to the FM-1 Firmware Hub only after testing on real hardware.

Files handed off with this document:
- `CLAUDE.md` (this file)
- `include/miasma_sprite.h`: Dr. Ashgrave sprite (16×26 indexed) and the UI palette in RGB565. Compiles clean with `-Wall -Wextra`.

---

## 1. Key decisions (already made)

| Decision | Choice | Why |
|---|---|---|
| Base | **Fork Skiff** (github.com/illuminus5/skiff, latest release; 1.4 at last check) | Already has 6 tracks, 14 engines, 64 steps, trig conditions, p-locks, chance, ratchets, flams, 8-level undo, 64-pattern pool, 16 scenes, second-core renderer (12 voices), WASM emulator and web editor. 6 → 8 tracks is the smallest jump. |
| Ports | **GR!MM** (github.com/GRiMMBIENT/grimm): SPACE reverb, GRAIN, FREEZE, sequencer tools. **GHOULBOX** (github.com/jasonpersinger/ghoulbox-fm1-dungeon-synth): TAPE, CRSH, host test harness. | Best existing dark-ambient FX on this chip. |
| Reference only | M-VAVE `fm1_open` / FM1 Workbench | Closed binary SDK engine, Windows-only build, hub won't mirror it. Use only as UX proof that 8 tracks / 64 steps run on this hardware. |
| License | **GPL-3.0-only** | Matches Skiff, GR!MM, GHOULBOX, Felucca. Keep every upstream credit and notice. |
| Name | **MIASMA** | Original. Never use "DIRGE" or any earlier working name anywhere in code, UI, docs or file names. |
| Mascot | **Dr. Ashgrave**, an original pixel plague doctor in a top hat with a brass-topped cane | Fronts every screen (see §6). Must stay original: no characters, names, fonts, UI elements or music from any existing game. |

---

## 2. Changes vs Skiff

| Area | Skiff today | MIASMA change | Source |
|---|---|---|---|
| Tracks | 6 | **8**, dark default roles (§3) | New |
| Engines | 14 | Add **CHIP**, PolyBLEP saw stack + wavefolder, chaos/feedback osc, dark wavetable, industrial percussion | New |
| Sequencer | p-locks, conditions, chance, ratchets | Add **decay step**, **scale lock**, **leitmotif slot** | New |
| Sequencer tools | Skiff set | Add GR!MM sequencer tools | GR!MM |
| FX | Skiff bus | Fixed chain DRIVE → CRUSH → TAPE → DELAY → SPACE, plus FREEZE | Mixed |
| Reverb | Skiff | SPACE, compact (fits ~2 s memory) | GR!MM |
| Tape / crush | none | TAPE (wow + saturation), CRSH | GHOULBOX |
| Freeze / grain | none | FREEZE button, GRAIN buffer mangling | GR!MM |
| Screens | Skiff UI | Plague-doctor pixel theme, 6 screens (§6) | New |
| Sync | Skiff MIDI | Confirm MIDI clock in (follows Digitakt II / Digitone II) | Verify |
| Export | none | **Case export**: a project to a file for backup and sharing | New |
| Testing | Skiff tests | Add GHOULBOX host test harness | GHOULBOX |
| Web | emulator + editor | Rethemed play page, editor with case export, new cheat sheet | Mixed |

---

## 3. Tracks and engines

Any engine can go on any track; these are defaults. Ward names are the on-screen track names.

| Track | Role | Default engine | Character | Ward name |
|---|---|---|---|---|
| 1 | Drone | Dark wavetable | Long envelopes, detune, slow random LFO | FEVER |
| 2 | Bass | VA saw + fold | Mono, driven, wavefolded | BILE |
| 3 | Lead / melody | CHIP pulse | Catchy chiptune lead, vibrato, arp macros | WHISTLE |
| 4 | Pad / choir | Dark wavetable | Wide, slow | VAPOR |
| 5 | Noise / texture | Noise | Crackle, LFSR, radio static | COUGH |
| 6 | Kick | Percussion | Industrial | KNELL |
| 7 | Snare | Percussion | Metallic | RATTLE |
| 8 | Hats | FM | Glitch hats | LEECH |

### CHIP engine (new)
- Oscillators: pulse at 12.5 / 25 / 50% duty, triangle (4-bit stepped), stepped LFSR noise (long and short modes).
- Per-note vibrato (depth, rate, delay), pitch slide, fast arpeggio macros (up to 4 notes, ~50 Hz step).
- Optional bit-depth reduction on the voice for older-console grit.
- Knob page OSC: K1 DUTY, K2 VIBRATO, K3 ARP, K4 CRUSH. Pages: OSC / ENV / MOD / ARP.

### Other new engines
- **VA saw + fold**: PolyBLEP saw stack (1–3 voices, detune), wavefolder, drive, sub osc. K1 DETUNE, K2 FOLD, K3 DRIVE, K4 SUB.
- **Chaos osc**: feedback / cross-modulated oscillator with stable-to-unstable sweep.
- **Dark wavetable**: small set of dark tables, slow scan, detune.
- **Industrial perc**: kick (pitch-env sine + click + drive), metallic snare (noise + inharmonic partials), glitch hats.

---

## 4. Sequencer

- 64 steps per track, 4 pages of 16 on the white keys.
- From Skiff: p-locks, chance, ratchets, trig conditions, flams, 64-pattern pool, 16 scenes, 8-level undo.
- **Decay step (new)**: a trig type that does STUTTER, BIT-DROP or REVERSE on its track, optionally gated by a trig condition. Shown with a skull glyph on screen.
- **Scale lock (new)**: per pattern; minor, Dorian, Phrygian, harmonic minor (plus chromatic = off). Notes from keys and p-locks quantize to the scale.
- **Leitmotif slot (new)**: store one short phrase (≤16 steps of notes + lengths). Recall on any track with transpose and ×½ / ×2 speed.
- **Sync**: MIDI clock in on USB and TRS; start/stop/continue.

## 5. FX bus

Fixed order, master bus:
1. DRIVE (new)
2. CRUSH (GHOULBOX CRSH)
3. TAPE: wow + saturation (GHOULBOX)
4. DELAY (Skiff)
5. SPACE: compact reverb (GR!MM)

Plus **FREEZE** (GR!MM): holds the FX tail. Delay memory is about 2 s total; budget SPACE and DELAY to share it.

## 5b. Musical voice

The target sound pairs dark textures with bright, catchy chip melodies: a hummable lead over a grim bed. Borrow the **style only**; never reproduce any existing melody, sample or sound from another game or soundtrack.

- 8 factory cases (projects): each mixes a CHIP lead with drone, bass and industrial drums, in minor or Dorian, 70–110 BPM.
- Factory sounds named in the ward theme (e.g. patch 014 "THIN REED", a 25% pulse lead).

---

## 6. Screen UI

Mock-ups: "MIASMA Screen Mockups" canvas (owner has the link). Drawn at 2× an **assumed 320×240** panel. **Task #1 of Phase 4: read the real panel size and colour depth from Skiff's display driver and re-fit.**

### Look
- Black background, 4 px (2 px at 1×) bone frames around boxes, no rounded corners.
- A narrator box at the bottom of every screen: Dr. Ashgrave's head portrait on the left, 1–2 lines of text starting with `*`. Quoted lines are him speaking.
- Cursor: `▸` in spectral. Selected row: panel-colour fill.
- Accent colour (default ember) is user-selectable in Settings: ember / spectral / violet `#9B6BD8`. The hat band follows the accent.
- Fonts: two **bitmap** fonts, a blocky caps font for labels and a taller pixel font for text. The mock-ups used Silkscreen and VT323 (both OFL); either bake those into bitmaps with their OFL notices or draw an original 5×7 / 6×10 set.

### Palette
In `include/miasma_sprite.h` as RGB565: void #07060B, panel #1C1826, rule #3A3546, coat #2A2436, shadow #4A4258, dim #8A8496, bone #ECE6D8, ember #E0583A, spectral #6FD3C7, brass #C9A24A.

### Sprite
`ash_pixels[26][16]` with `ash_palette[]`. Full body for Title and Casebook; head portrait (rows 0–12, cols 1–14) in every narrator box. Draw with integer scaling only.

### Screens

| Screen | Name on screen | Mode | Shows | Controls |
|---|---|---|---|---|
| Title | MIASMA | Boot | Full sprite, firmware name and version, "8 TRACKS · 64 STEPS", menu: CONTINUE · CASE nn / NEW CASE / SETTINGS. Narrator: "Dr. Ashgrave tips his hat." | Encoder scroll, PLAY to continue |
| Tracks | THE WARD | Track overview | 8 rows: number, ward name, engine, PULSE (level) bar, state (role, SOLO, or QRNTN = muted, name struck through). Header: pattern, BPM, play state. | K1 level, SHIFT+key mute (quarantine), EDIT to sound edit |
| Sequencer | ROUNDS · Tn NAME · p/4 | Step edit | 16 steps in 2×8 grid; page map of 4; glyphs ■ trig, • p-lock, % chance, ×N ratchet, 1:N condition, skull decay step; playhead inverted; selected step outlined in accent. Narrator box offers decay choices STUTTER / BIT-DROP / REVERSE / CLEAR. | Keys toggle steps, hold step + knob = p-lock |
| Sound edit | DIAGNOSIS | Engine params | Left card: track name, number and role, CLASS (engine), waveform, SCALE, PATCH number + name. Right: 4 knob bars with values. Page tabs per engine. | K1–K4, EDIT cycles pages |
| FX | APOTHECARY · MASTER BUS | FX chain | 5 vials in chain order with dose bar, value, origin tag; FREEZE button. | K1 dose of selected, K2–K4 its params |
| Save | CASEBOOK | Projects | Full sprite, AUTOSAVE flag, case slots (name, tracks, patterns, BPM, age). Choices SAVE / EXPORT CASE / CANCEL. | Encoder, SAVE |

Narrator lines should be short and in character, e.g. "Step 21 is infected. It decays every 2nd pass." Keep them in one string table so they can be translated or changed.

---

## 7. Web trio (one GitHub Pages site, same C code compiled to WASM)

| Page | Does | Features |
|---|---|---|
| Play | Plays MIASMA with no FM-1 | On-screen FM-1 panel, computer keys, WebMIDI keyboard, demo cases |
| Editor | Edits a connected FM-1 over USB MIDI | Patches and patterns, backup/restore, Install firmware (same path as the hub), Export case |
| Cheat sheet | Printable one-pager | Button combos, knob maps per engine, default track roles, MIASMA dark theme |

---

## 8. Hardware and constraints

- Chip: JieLi AC791N, two pi32v2 cores. Skiff renders audio on the second core.
- Controls: 4 knobs, 16 white keys (steps), 12 mode buttons, small colour LCD.
- ~2 s of delay memory.
- Install: `.fwsc` through the web updater. Keep M-VAVE stock V15 to roll back; community firmwares drop to safe mode after repeated failed starts.
- Emulator for testing: github.com/joakimeriksson/fm1-emu runs firmwares unchanged in the browser.
- Watch the CPU budget: 8 tracks plus the new engines must hold 12 voices without dropouts. Measure early.

---

## 9. Phases (each ends with a working build)

1. **Fork and boot**: fork Skiff, build it, run it in the emulator. Rename to MIASMA everywhere. CI builds `.fwsc` and the WASM page.
2. **Engines and FX**: CHIP first (it defines the sound), then VA fold, chaos, wavetable, perc; port TAPE/CRSH and SPACE/GRAIN/FREEZE with credits. Host tests for each engine.
3. **Sequencer**: 6 → 8 tracks, decay step, scale lock, leitmotif slot. CPU profiling at full load.
4. **Screens**: read real panel size, re-fit, implement the 6 screens, sprite, fonts, narrator string table, accent setting.
5. **Web trio**: rethemed play page, editor with case export, cheat sheet.
6. **Hardware and release**: test on a real FM-1, factory cases, README, submit to the hub.

## 10. Open questions

- [ ] Real LCD resolution and colour depth
- [ ] Does 8 tracks fit Skiff's CPU budget at 12 voices?
- [ ] Track roles as defaults only (current plan) or locked?
- [ ] Bitmap fonts: bake Silkscreen / VT323 (OFL) or draw original?

## 11. Working rules

- Keep upstream GPL notices and credit Skiff, Felucca, GR!MM and GHOULBOX in README and the about screen.
- Small commits per feature; every phase leaves the emulator build working.
- No trademarked game names, characters or music anywhere. M-VAVE and FM-1 are named only to describe the hardware.
