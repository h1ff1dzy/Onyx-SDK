## Features

- Easy integration with IL2CPP CPP Project
- Included DirectX Drawing ([DirectX Toolkit](https://github.com/microsoft/DirectXTK))
- Clear file structure, it will be pleasant to the eyes

This is only a project for self-study, I do not pretend to be the #1 hacker, but I tried to make a project convenient for myself and decided to put it here.

Please, do not inject this in games with good anticheat, like EAC(satire), BattleEye, or VAC and etc

## Step-by-step installing
Get vcpkg from here [vcpkg.io](https://vcpkg.io/en/index.html "vcpkg.io")

`vcpkg install directxtk:x64-windows`

`vcpkg install freetype:x64-windows-static`

Then include libraries from freetype to project and compile
###### you can just disable freetype in imgui_config.h


## Why you dont should use this

- Obviously detected cause it's using minhooks
- Coded by for study proposes, try to eject the bests and use in your future projects
