# B+ Tree

## Setup

- Ensure that you have a C++ compiler (we suggest [mingw](https://sourceforge.net/projects/mingw-w64/) for Windows).
- Setup your environment and ensure all C++ files are included in compilation.

  * Use the [code runner](https://marketplace.visualstudio.com/items?itemName=formulahendry.code-runner) extension for VSCode
    Add a field in `settings.json`:
    ```
    "code-runner.executorMap": {
      "cpp": "cd $dir && g++ *.cpp -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
    }
    ```

  * Run C++ with VSCode directly
    Modify `args` field in `tasks.json` to include all C++ files:
    ```
    "tasks": [
      {
        "args": ["-g", "${workspaceFolder}\\*.cpp", "-o", "${fileDirname}\\${fileBasenameNoExtension}.exe"],
      }
    ]
    ```

- Remain on root directory where `main.cpp` locates and compile the executable.
- Run `main.cpp`
- **IMPORTANT**
  If the `main.cpp` gets stuck at reading Records, please exit the VSCode and restart and run until it works.
