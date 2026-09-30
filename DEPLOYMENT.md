# Deployment

This project is deployed as a static WebAssembly site using GitHub Actions and GitHub Pages.

## Deployment target

The deployed site contains:

```text
index.html
root_simulator.js
root_simulator.wasm
```

These files are built from the C++/raylib source using Emscripten.

## GitHub Pages setup

In the GitHub repository:

1. Go to **Settings → Pages**.
2. Under **Build and deployment**, set **Source** to **GitHub Actions**.
3. Make sure GitHub Actions are enabled for the repository.
4. Push to the `main` branch.

The workflow file is located at:

```text
.github/workflows/deploy.yml
```

## Automatic deployment

Deployment runs automatically when changes are pushed to `main`:

```sh
git push origin main
```

The workflow will:

1. Check out the repository.
2. Install Emscripten.
3. Configure the CMake WebAssembly build.
4. Build the project.
5. Collect the generated static files.
6. Upload them as a GitHub Pages artifact.
7. Deploy the site to GitHub Pages.

## Manual deployment

The workflow can also be run manually:

1. Go to the repository on GitHub.
2. Open the **Actions** tab.
3. Select **Deploy WebAssembly site to GitHub Pages**.
4. Click **Run workflow**.

## Local verification before pushing

Before pushing, it is useful to verify that the WebAssembly build works locally:

```sh
emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web --parallel
```

Then serve the build output:

```sh
cd build-web
python3 -m http.server 8000
```

Open:

```text
http://localhost:8000/
```

Expected behavior:

- The root system appears automatically on load.
- The canvas fills the browser window.
- Drag rotates the camera.
- Mouse wheel zooms.
- `P` opens the root parameter panel.
- `N` generates a new root while the parameter panel is open.
- `R` resets parameters while the panel is open, or resets the camera when the panel is closed.

## Generated files

The `build-web/` directory is generated and should not be committed. The repository ignores build outputs through `.gitignore`.

GitHub Actions rebuilds the site from source on every deployment.
