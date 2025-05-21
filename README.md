# AvisEasyTrainer

AvisEasyTrainer is a basic trainer for Cyberpunk 2077 written in C++. It uses DirectX 12 for drawing through ImGui and is built as a RED4ext plugin. Mostly a fun side project to learn more about C++, DirectX rendering, and game hooking.

## Features

- Runs through RED4ext
- Compatible with CET
- Uses DirectX 12 background draw list
- ImGui-based overlay

## Setup

To build it yourself, you'll need to clone a few dependencies into the `Vendor/` folder:

- [RED4ext.SDK](https://github.com/wopss/RED4ext) – required
- [ImGui](https://github.com/ocornut/imgui)
- [kiero](https://github.com/Rebzzel/kiero)
- [MinHook](https://github.com/TsudaKageyu/minhook) – should already be included, but clone if missing

## Notes

This is just a learning tool not a serious project. Built for fun and experimentation with game memory, rendering, and plugin structure.

Even though this is a fun project, please provide credit if you end up using it for anything......though I doubt anyone will use this lol.
