
# target for the PieceArray DLL that the rust library uses
PieceArrayLink:
	cmake --build C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build-debug --target PieceArrayLink -j 9
	copy .\cpp_lib\cmake-build-debug\Debug\PieceArrayLink.dll .\dlls_output\PieceArrayLink.dll
	copy .\cpp_lib\cmake-build-debug\Debug\PieceArrayLink.exp .\dlls_output\PieceArrayLink.exp
	copy .\cpp_lib\cmake-build-debug\Debug\PieceArrayLink.lib .\dlls_output\PieceArrayLink.lib
	copy .\cpp_lib\cmake-build-debug\Debug\PieceArrayLink.pdb .\dlls_output\PieceArrayLink.pdb

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
	copy .\cpp_lib\cmake-build-debug\Debug\PuzzleSolverLib.dll .\dlls_output\PuzzleSolverLib.dll
	copy .\cpp_lib\cmake-build-debug\Debug\PuzzleSolverLib.exp .\dlls_output\PuzzleSolverLib.exp
	copy .\cpp_lib\cmake-build-debug\Debug\PuzzleSolverLib.lib .\dlls_output\PuzzleSolverLib.lib
	copy .\cpp_lib\cmake-build-debug\Debug\PuzzleSolverLib.pdb .\dlls_output\PuzzleSolverLib.pdb

uninstall:
	rmdir /s /q .\cpp_lib\cmake-build-debug


install:
	cmake -S C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib -B C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build-debug
#cmake -S .\cpp_lib -B .\cpp_lib\cmake-build-debug

