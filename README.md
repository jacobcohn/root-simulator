# Root Simulator

A minimal browser-only C++23/raylib/WebAssembly proof of concept for a future procedural plant-root simulator. This currently verifies the web build, responsive canvas, animation, and mouse input foundation only; it does **not** implement procedural root generation or simulation.

## Technology

- C++23
- raylib 5.5, fetched by CMake
- CMake 3.22+
- Emscripten 6.0.10/WebAssembly
- GitHub Actions + GitHub Pages

## Requirements

- macOS, Linux, or WSL for local builds
- CMake 3.22 or newer
- Python 3 for a local static web server
- Emscripten SDK for WebAssembly builds

## Initial Emscripten setup on macOS

One common setup is:

```sh
mkdir -p ~/Library/Developer
git clone https://github.com/emscripten-core/emsdk.git ~/Library/Developer/emsdk
cd ~/Library/Developer/emsdk
./emsdk install 6.0.10
./emsdk activate 6.0.10
source ./emsdk_env.sh
```

This keeps the SDK with other user-level developer tools instead of directly in your home directory. Run `source ~/Library/Developer/emsdk/emsdk_env.sh` in each new terminal before building, or add that source command to your shell startup file.

## Build

From the repository root:

```sh
emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web --parallel
```

The web files are generated in `build-web/`:

- `index.html`
- `root_simulator.js`
- `root_simulator.wasm`

Generated WebAssembly artifacts are intentionally ignored by Git and should not be committed.

## Local browser testing

Serve the build directory over HTTP; do not open `index.html` directly from the filesystem.

```sh
cd build-web
python3 -m http.server 8000
```

Then open:

```text
http://localhost:8000/
```

Expected behavior:

- The canvas fills the browser viewport without page margins or scrollbars.
- The raylib render size follows the canvas/viewport size, not a fixed 800x800 framebuffer.
- Animation runs continuously.
- The mouse marker follows the cursor and remains aligned after resize.
- Clicking changes the placeholder drawing.
- Resizing the browser keeps rendering correct.

## Normal development loop

After the initial `emcmake cmake` configure step, most edits only need:

```sh
cmake --build build-web --parallel
```

Refresh the browser tab served by `python3 -m http.server`.

Re-run the `emcmake cmake ...` configure command if you change `CMakeLists.txt` or update dependencies.

## Responsive canvas implementation

`public/index.html` removes default margins and gives the canvas `100vw` by `100vh`. The C++ frame loop also queries the canvas CSS size through Emscripten browser APIs and updates the actual canvas/window render dimensions. This avoids relying only on CSS stretching and keeps mouse coordinates aligned with drawing coordinates.

The files use relative paths only, so the app can be hosted from a GitHub Pages project URL such as:

```text
https://USERNAME.github.io/root-simulator/
```

## GitHub Pages deployment

The workflow in `.github/workflows/deploy.yml` runs on pushes to `main` and can also be started manually. It:

1. Checks out the repository.
2. Installs Emscripten.
3. Configures CMake with `emcmake`.
4. Builds the raylib/WebAssembly application.
5. Collects `index.html`, `root_simulator.js`, and `root_simulator.wasm`.
6. Uploads the static site as a Pages artifact.
7. Deploys it with the current GitHub Pages Actions workflow.

Required repository settings on GitHub:

1. Go to **Settings → Pages**.
2. Set **Build and deployment → Source** to **GitHub Actions**.
3. Ensure Actions are enabled for the repository.
4. Push to the `main` branch.

