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
