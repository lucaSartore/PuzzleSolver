
# target for the PieceArray DLL that the rust library uses
PieceArrayLink:
	cmake --build C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build --target PieceArrayLink --config Release -j 9
	copy .\cpp_lib\cmake-build\Release\PieceArrayLink.dll .\dlls_output\PieceArrayLink.dll
	copy .\cpp_lib\cmake-build\Release\PieceArrayLink.exp .\dlls_output\PieceArrayLink.exp
	copy .\cpp_lib\cmake-build\Release\PieceArrayLink.lib .\dlls_output\PieceArrayLink.lib

# target for the rust library DLL that will be used by the puzzle solver
RustLib: PieceArrayLink
	cargo build --release --manifest-path=cpp_lib\rust_solving\Cargo.toml
	copy .\cpp_lib\rust_solving\target\release\rust_solving_lib.d .\dlls_output\rust_solving_lib.d
	copy .\cpp_lib\rust_solving\target\release\rust_solving_lib.dll .\dlls_output\rust_solving_lib.dll
	copy .\cpp_lib\rust_solving\target\release\rust_solving_lib.dll.exp .\dlls_output\rust_solving_lib.dll.exp
	copy .\cpp_lib\rust_solving\target\release\rust_solving_lib.dll.lib .\dlls_output\rust_solving_lib.dll.lib

# target for the main c++ DLL that will be used by the c# "front end" for the app
PuzzleSolverLib: RustLib
	cmake --build C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build --target PuzzleSolverLib --config Release -j 9
	copy .\cpp_lib\cmake-build\Release\PuzzleSolverLib.dll .\dlls_output\PuzzleSolverLib.dll
	copy .\cpp_lib\cmake-build\Release\PuzzleSolverLib.exp .\dlls_output\PuzzleSolverLib.exp
	copy .\cpp_lib\cmake-build\Release\PuzzleSolverLib.lib .\dlls_output\PuzzleSolverLib.lib

PuzzleSolverTest: RustLib
	cmake --build C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build --target PuzzleSolverTest --config Release -j 9

RunPuzzleSolverTest:
	.\cpp_lib\cmake-build\Release\PuzzleSolverTest.exe

PuzzleSolverCommand: RustLib
	cmake --build C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build --target PuzzleSolverCommand --config Release -j 9
	copy .\cpp_lib\cmake-build\Release\PuzzleSolverCommand.exe .\dlls_output\PuzzleSolver.exe

uninstall:
	rmdir /s /q .\cpp_lib\cmake-build

install:
	cmake -S C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib -B C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build
