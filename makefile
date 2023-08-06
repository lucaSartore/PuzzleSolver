
PieceArrayLink:
	cmake --build .\cpp_lib\cmake-build-debug --target PieceArrayLink -j 9
	copy .\cpp_lib\cmake-build-debug\PieceArrayLink.dll .\dlls_output\PieceArrayLink.dll
	copy .\cpp_lib\cmake-build-debug\PieceArrayLink.dll.manifest .\dlls_output\PieceArrayLink.dll.manifest
	copy .\cpp_lib\cmake-build-debug\PieceArrayLink.exp .\dlls_output\PieceArrayLink.exp
	copy .\cpp_lib\cmake-build-debug\PieceArrayLink.lib .\dlls_output\PieceArrayLink.lib


RustLib:
	cargo build --manifest-path=cpp_lib\rust_solving\Cargo.toml
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.d .\dlls_output\rust_solving_lib.d
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.dll .\dlls_output\rust_solving_lib.dll
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.dll.exp .\dlls_output\rust_solving_lib.dll.exp
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.dll.lib .\dlls_output\rust_solving_lib.dll.lib
	copy .\cpp_lib\rust_solving\target\debug\rust_solving_lib.pdb .\dlls_output\rust_solving_lib.pdb





