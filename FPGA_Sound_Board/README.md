# 🎵 See & Say SoundBoard — FPGA Digital Audio System

A real-time **FPGA-based digital soundboard** implemented in Verilog HDL. The system allows a user to select themed sound libraries using FPGA switches, trigger individual sounds using push buttons, display the selected sound name on the onboard 7-segment display, and play the corresponding audio through an external speaker.

The project was developed for **EGEC 446 — Advanced Digital Design** and demonstrates FPGA design, digital audio playback, memory-based sound storage, input debouncing, display multiplexing, and hardware/software integration.

---

## 🎥 Project Demo

[![See & Say FPGA SoundBoard Demo](https://img.youtube.com/vi/AyihwfW5y8o/maxresdefault.jpg)](https://youtu.be/AyihwfW5y8o)

**▶ Click the image above to watch the FPGA SoundBoard demonstration.**

---

## Project Overview

The goal of the project was to recreate the concept of a **"See & Say" soundboard** using an FPGA.

Instead of having only one group of sounds, the FPGA contains multiple selectable sound libraries.

### Sound Libraries

- 🐄 Animals
- 📯 Horns
- ⛏️ Minecraft Mobs

The user first selects a sound library using the FPGA switches.

Once a library is selected, one of five push buttons can be pressed to select a sound.

The FPGA then performs two actions simultaneously:

1. Plays the selected audio through the connected speaker.
2. Displays the name of the selected sound on the onboard 7-segment display.

---

# System Architecture

```text
                     FPGA
                      │
        ┌─────────────┴─────────────┐
        │                           │
    Switch Inputs              Push Buttons
        │                           │
        │                    Debounce Logic
        │                           │
        └─────────────┬─────────────┘
                      │
                Sound Selection
                      │
          ┌───────────┴───────────┐
          │                       │
          ▼                       ▼
    Display System           Sound Player
          │                       │
     Word Select                Sound ROM
          │                       │
        MUX                  8-bit Samples
          │                       │
 Character Decoder          22.05 kHz Timing
          │                       │
          ▼                       ▼
   7-Segment Display          Audio Output
                                  │
                                  ▼
                               Speaker
```

---

# FPGA Input System

## Library Selection

FPGA switches are used to select the active sound library.

A **3-bit switch implementation** determines which library is currently selected.

The switch state is used by the FPGA control logic to retrieve the appropriate library and sound information.

```text
Switches
   │
   ▼
Library Selection
   │
   ├── Animals
   ├── Horns
   └── Minecraft Mobs
```

---

## Sound Selection

Five FPGA push buttons are used to select individual sounds from the active library.

```text
Selected Library
       │
       ▼
   Push Button
       │
       ▼
 Debounce Logic
       │
       ▼
 Sound Selection
```

The button inputs are **debounced** so that a physical button press produces a reliable synchronous selection.

Once a valid button press is detected, the FPGA:

- Selects the corresponding sound
- Updates the displayed sound name
- Initiates audio playback

---

# Seven-Segment Display System

The FPGA's onboard 7-segment display is used to display the name associated with the selected sound.

A custom decoder was implemented to represent the alphabet using the available seven-segment patterns.

## Character Decoder

The decoder contains representations for letters:

```text
A → Z
```

using a **common-anode seven-segment display configuration**.

This allows the FPGA to construct recognizable words representing sound and library names.

For example:

```text
Selected Sound: Dog

Display:

d o g
```

---

## Display Multiplexing

Because a word contains multiple characters, the display logic separates the word into individual characters.

A multiplexer cycles through the characters and corresponding display positions.

```text
Sound Name
    │
    ▼
Character Selection
    │
    ▼
    MUX
    │
    ▼
Character Decoder
    │
    ▼
7-Segment Display
```

This allows multiple characters to appear together as a readable word on the FPGA display.

---

# 🔊 Digital Audio System

The audio subsystem retrieves stored digital audio samples and outputs them at a controlled sampling rate.

## Audio Conversion

Original sound files were converted from:

```text
.wav
```

into **8-bit digital sample data**.

The converted samples were then stored in:

```text
.mem
```

files that could be loaded into FPGA memory.

Example:

```text
sound_dog.mem
```

---

## Sound ROM

Each sound is represented by digital samples stored in ROM.

```text
.wav Audio
     │
     ▼
8-bit Samples
     │
     ▼
.mem File
     │
     ▼
Sound ROM
     │
     ▼
Sound Player
```

When a sound is selected, the sound player begins reading the corresponding ROM.

The ROM address increments during playback, allowing the FPGA to sequentially retrieve each audio sample.

---

## Audio Sample Timing

Audio samples are output at a steady:

```text
22.05 kHz
```

sampling rate.

The FPGA generates the required timing so that the ROM samples are played sequentially at the correct rate.

```text
FPGA Clock
    │
    ▼
Sample Timing
  22.05 kHz
    │
    ▼
ROM Address
    │
    ▼
8-bit Sample
    │
    ▼
Audio Output
```

The digital sample is then converted into an analog voltage used for speaker output.

---

# Hardware Implementation

The completed system integrates the FPGA development board with an external speaker.

```text
FPGA Board
   │
   ├── Switches
   ├── Push Buttons
   ├── 7-Segment Display
   │
   └── Audio Output
          │
          ▼
       Speaker
```

The FPGA handles the complete real-time processing path:

**User Input → Sound Selection → Display Update → ROM Retrieval → Audio Playback**

---

# RTL Design

The FPGA architecture was divided into multiple functional modules.

```text
Top Module
│
├── Button / Debounce Logic
│
├── Word Selection
│
├── Character MUX
│
├── Seven-Segment Decoder
│
├── Sound Player
│
├── Sound ROM
│
└── Audio Output Logic
```

The modular design separates input handling, display generation, memory access, and audio playback.

---

# Challenges & Debugging

Several implementation challenges were encountered during development.

### Seven-Segment Decoder

The character decoder required additional integration with the display MUX.

Incorrect segment mappings initially caused letters to appear:

- Backwards
- Incomplete
- With missing segments

The character patterns and multiplexing behavior were corrected to produce readable sound names.

### Audio Quality

Audio implementation required tuning of the playback system and sample timing to improve sound quality.

### FPGA Memory

Available **block memory** limited the amount of audio data that could be stored directly on the FPGA.

This required consideration of sound length and sample representation.

### Display / Sound Synchronization

Sound names occasionally did not correspond correctly with the selected audio.

The selection logic was corrected so that the displayed name and audio playback remained synchronized.

---

# Implementation Results

The completed FPGA system successfully demonstrated:

- Reliable switch-based sound library selection
- Debounced push-button sound selection
- Real-time audio playback
- ROM-based digital audio storage
- 8-bit audio sample processing
- 22.05 kHz sample timing
- Alphabetic seven-segment character decoding
- Multiplexed multi-character display output
- Synchronization between displayed sound names and audio playback

The final system successfully retrieves and plays the selected sound based on the user's button input while simultaneously displaying the corresponding name.

---

# FPGA Implementation & Analysis

The completed design was synthesized and implemented on the FPGA.

The project also included analysis of the implemented hardware using FPGA design tools, including:

- Top-level synthesized schematic
- Design timing analysis
- Power analysis
- FPGA implementation verification

These results were used to evaluate the implemented digital architecture after synthesis and implementation.

---

# Skills Demonstrated

### FPGA & RTL Design

- Verilog HDL
- FPGA development
- RTL design
- Modular digital architecture
- Sequential logic
- Combinational logic
- Digital control systems

### Digital Interfaces

- Push-button input handling
- Input debouncing
- Switch-based control
- Seven-segment display interfacing
- Display multiplexing
- Character decoding

### Digital Audio

- Digital audio sampling
- WAV-to-memory conversion
- 8-bit audio representation
- ROM-based audio storage
- Memory address sequencing
- 22.05 kHz sample timing
- Real-time audio playback

### FPGA Verification & Implementation

- RTL synthesis
- FPGA implementation
- Timing analysis
- Power analysis
- Hardware debugging
- Functional hardware validation

---

# Future Improvements

Potential improvements to the design include:

- Additional sound libraries
- Larger external memory for audio storage
- Higher-resolution audio samples
- Improved audio quality
- Increased sound duration
- Additional display characters
- More flexible library selection
- Improved audio filtering
- Expanded user controls

---

## Authors

**Taranaki Faamatuainu**  
Chris Gonzalez  
Varun Yedavilli  
Jesus Arteaga

**EGEC 446 — FPGA Project**  
December 2025
