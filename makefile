
# target for the PieceArray DLL that the rust library uses
PieceArrayLink:
	copy .\cpp_lib\cmake-build-debug\PieceArrayLink.dll .\dlls_output\PieceArrayLink.dll
	copy .\cpp_lib\cmake-build-debug\PieceArrayLink.dll.manifest .\dlls_output\PieceArrayLink.dll.manifest


# target for the rust library DLL that will be used by the puzzle solver
RustLib: PieceArrayLink
	cargo build --manifest-path=cpp_lib\rust_solving\Cargo.toml
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.d .\dlls_output\rust_solving_lib.d
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.dll .\dlls_output\rust_solving_lib.dll
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.dll.exp .\dlls_output\rust_solving_lib.dll.exp
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.dll.lib .\dlls_output\rust_solving_lib.dll.lib
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.pdb .\dlls_output\rust_solving_lib.pdb

# target for the main c++ DLL that will be used by the c# "front end" for the app
PuzzleSolverLib: RustLib
	cmake --build C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build-debug --target PuzzleSolverLib -j 9
	copy .\cpp_lib\cmake-build-debug\PuzzleSolverLib.dll .\dlls_output\PuzzleSolverLib.dll
	copy .\cpp_lib\cmake-build-debug\PuzzleSolverLib.dll.manifest .\dlls_output\PuzzleSolverLib.dll.manifest
	copy .\cpp_lib\cmake-build-debug\PuzzleSolverLib.exp .\dlls_output\PuzzleSolverLib.exp
	copy .\cpp_lib\cmake-build-debug\PuzzleSolverLib.lib .\dlls_output\PuzzleSolverLib.lib

uninstall:
	rmdir /s /q .\cpp_lib\cmake-build-debug


install:
	cmake -DCMAKE_BUILD_TYPE=Debug "-DCMAKE_MAKE_PROGRAM=C:/Program Files/JetBrains/CLion 2022.2.4/bin/ninja/win/ninja.exe" -G Ninja -S C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib -B C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build-debug
#cmake -S .\cpp_lib -B .\cpp_lib\cmake-build-debug

