<p align="center">
  <h1 align="center">3D editor</h1>
</p>
<p align="center">
	3D models editor with collaboration features
</p>
<details>
<summary>Table of Contents</summary>
	<li><a href="#build-&-run">Build & run</a></li>
  	<li><a href="#demo">Components</a></li>
	<li><a href="#features">Features</a></li>
</details>

## Build & Run
<details>
  <summary>Requirements</summary>
    <li><a>OS Linux</a></li>
    <li><a>C++17 compiler</a></li>
    <li><a>Make</a></li>
    <li><a href="https://docs.conan.io/2/">Conan 2.0</a></li>
</details>

To download all dependencies use command:

```bash
make setup
```

To build 3D editor AND server, use command:

```bash
make build
```

You can find binary in the `build/app` directory, or run it via makefile:

```bash
make run # or make run-intel to use intel GPU
```

Server's binary can be found in `build/network_lib` directory. To run server, run binary with passing port as argument:

```bash
build/network_lib/3d_graphics-server <PORT>
```

---
## Demo
* Mixed model view mode: shows points and faces
![mixed](https://github.com/lolpie244/3d_graphics_editor/assets/86479624/b04e1684-995e-463d-bf00-19ce90bc5a82)
* Points only view mode: shows only points
![points](https://github.com/lolpie244/3d_graphics_editor/assets/86479624/377333c8-f89a-4ae0-a7bf-be01d078b598)
* Save/load model
![load](https://github.com/lolpie244/3d_graphics_editor/assets/86479624/4bef7505-9296-45a2-a194-35bf4c4474ed)
* Duplicate/Join points
![duplicate](https://github.com/lolpie244/3d_graphics_editor/assets/86479624/ee0bb0f6-ec28-4aef-8808-593d9f94700b)
![join](https://github.com/lolpie244/3d_graphics_editor/assets/86479624/f51a0628-9d3c-4a25-b663-171c7c91586d)
* Collaboration
  
https://github.com/lolpie244/3d_graphics_editor/assets/86479624/f77a300c-755b-46f5-a0df-8b98e4dfd5f4



---
## Features

- [x] 3D manipulator (Gizmo)
  - [x] Translate
  - [x] Scale
  - [x] Rotate
- [x] Triangulation
  - [ ] Triangulation via tessalation shader
- [x] Vertex modification
  - [x] Move
  - [x] Duplicate
  - [x] Join
  - [ ] Remove
- [x] Save/Load model
  - [x] Load model from obj file
  - [ ] Export model to obj file
  - [x] Load/Save scene to own binary format
- [x] Light source
- [x] Collaboration

---
