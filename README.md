# ORYMY Bass — YMY Sounds

A JUCE/CMake C++ bass preamp plugin project targeting VST3 and Standalone on Windows x64.

## Build requirements
- Windows 10/11 x64
- Visual Studio 2022 with the Desktop development with C++ workload
- CMake 3.22 or newer
- Internet access for CMake FetchContent to retrieve JUCE 8.0.0

## Build
```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel
```

Build products are expected at:
- `build/OrymyBass_artefacts/Release/VST3/ORYMY Bass.vst3`
- `build/OrymyBass_artefacts/Release/Standalone/ORYMY Bass.exe`

GitHub Actions builds these targets on pushes to `main`, pushes to `fix/**`, pull requests, or manual dispatch. Download the build artifact from the Actions run.

## Current implementation status
- JUCE plugin project with VST3 and Standalone targets.
- Input/output gain and a basic tanh saturation stage are connected to the audio processor.
- Parameter state is serialized by JUCE's AudioProcessorValueTreeState.
- The editor and preset selector are a starter interface, not yet a finished production UI.
- Preset selection, full knob attachments, tuner, spectrum analyzer, speaker animation, advanced amp/cabinet modeling, installer, and DAW/runtime compatibility testing remain future work.

A successful CI compile proves the binaries were built; it does not replace listening tests or validation inside a DAW on the target machine.
