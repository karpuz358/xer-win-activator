<div align="center">

# 🪟 Xer Activator

**A simple Windows & Office activation tool: clean console interface, no bloat.**

![Platform](https://img.shields.io/badge/platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Windows 10/11](https://img.shields.io/badge/Windows%2010%20%7C%2011-supported-2ea44f?style=for-the-badge&logo=windows&logoColor=white)
![Version](https://img.shields.io/badge/version-1.2-orange?style=for-the-badge)
![Status](https://img.shields.io/badge/status-maintained-yellow?style=for-the-badge)

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-5C2D91?style=flat-square&logo=visualstudio&logoColor=white)
![MSVC](https://img.shields.io/badge/MSVC-v143-red?style=flat-square&logo=visualstudio&logoColor=white)

*Made with ❤️ by Karpuz*

</div>

---

## ✨ Features

- Activate Windows 10/11 (auto-detects the installed edition)
- Pick the edition manually if needed
- Activate Office 2013-2024 with the Ohook method
- Clean, no-nonsense console interface

---

## 🚀 Quick start

> ⚠️ **Run as Administrator is required** — the tool checks this at startup.

1. 📥 **[Download the latest release](https://github.com/karpuz358/xer-win-activator/releases/latest)** and run `Xer Activator.exe` (or build it yourself, see below).
2. Pick **`1 - Activate Windows`** for the detected edition, or **`2 - Select Windows edition`** to pick by hand.
3. Confirm with `y` and watch the three steps run.
4. Windows Settings opens automatically at the end — verify the activation status there.

---

## 🛠️ How it works

The tool drives `slmgr.vbs` (the Windows Software Licensing Management Tool) using the generic/retail install keys published by Microsoft for client editions:

```
slmgr /ipk  <generic key>     — install the product key
slmgr /skms <server>          — point the KMS client to a server
slmgr /ato                    — activate online
```

For **Office** (2013 → 2024, both CTR 32/64-bit), the included **Ohook** method replaces nothing: it adds a `sppcs.dll` symlink plus a patched `sppc.dll` beside the real files, so Office's own components keep working — including on Office 16/Build beyond the usual "stuck" dates.

It runs on Windows 10 **and** Windows 11: the edition name comes from `HKLM\...\CurrentVersion` (`ProductName`) with a `CurrentBuildNumber >= 22000` correction, because Windows 11 still reports `Windows 10 ...` there.

---

## 🧱 Tech stack

| Module | Role |
|---|---|
| `src/main.cpp` | Entry point: admin check → menu |
| `src/console.*` | All drawing: palette, title, menu lines, `[y/N]` prompts |
| `src/wininfo.*` | Admin rights, registry edit detection, generic key table |
| `src/activation.*` | slmgr commands, Windows & Office activation flows |
| `src/menu.*` | Screen navigation only |
| `src/ohook.*` | Ohook method for Office (symlink + patched DLL) |

---

## 🔨 Build

Open the solution in **Visual Studio 2022** (v143 toolset, C++17) and build, **or** from a VS developer prompt at the repo root:

```bash
cl /nologo /EHsc /std:c++17 /Fe:"Xer Activator.exe" src\main.cpp src\console.cpp src\wininfo.cpp src\activation.cpp src\menu.cpp src\ohook.cpp wbemuuid.lib user32.lib shell32.lib advapi32.lib imagehlp.lib
```

---

## 📄 License

Released under the [MIT License](LICENSE).

---

## ⚖️ Disclaimer

Windows and Office are products of **Microsoft Corporation**. This project is **not affiliated with or endorsed by Microsoft**. The product keys used are the standard generic install keys Microsoft publishes publicly for client editions; use this tool responsibly and in accordance with the laws of your country.

<div align="center">

Made with ❤️ by Karpuz

</div>
