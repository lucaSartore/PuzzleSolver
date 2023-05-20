using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using static System.Windows.Forms.AxHost;
using System.Diagnostics;

namespace JigsawGenius
{


    internal class DllLib
    {
        public DllLib()
        {
        }

        [StructLayout(LayoutKind.Sequential)]
        unsafe public struct PngImagePointer
        {
            Byte* data;
            int len;
            // Add other fields as needed
        }

        // in order for this to work you must add the path to de DLL to your system path
        // in my case is: C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build-debug
        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern void* open_folder([MarshalAs(UnmanagedType.LPStr)] string path);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern void* create_new(uint dim_x, uint dim_y, [MarshalAs(UnmanagedType.LPStr)] string work_path, [MarshalAs(UnmanagedType.LPStr)] string origin_path, uint number_of_cores);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern void save_as(void* puzzle_solver, [MarshalAs(UnmanagedType.LPStr)] string new_path);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern void free_memory(void* puzzle_solver);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern void set_number_of_cores(void* puzzle_solver, uint new_number_of_cores);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern void set_split_threshold(void* puzzle_solver, byte new_threshold);

        [DllImport("libPuzzleSolverLib.dll")]
        public static extern void enable_preview();

        [DllImport("libPuzzleSolverLib.dll")]
        public static extern void disable_preview();

        [DllImport("libPuzzleSolverLib.dll")]
        public static extern PngImagePointer next_preview_image(float max_wait_time);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern PngImagePointer get_threshold_preview(void* puzzle_solver);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern State get_state(void* puzzle_solver);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern int split_image(void* puzzle_solver);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern int process_corners(void* puzzle_solver);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern int calculate_connections(void* puzzle_solver);

        [DllImport("libPuzzleSolverLib.dll")]
        unsafe public static extern int solve_puzzle(void* puzzle_solver);
    }



    public class Comunicator : IDisposable
    {
        // pointer to the c++ class
        private IntPtr _puzzleSolverClass;

        /// create a comunicator by opening a folder with the saved prooject
        public Comunicator(string path_to_open)
        {
            unsafe
            {
                void* ptr_void = DllLib.open_folder(path_to_open);
                if (ptr_void == null)
                {
                    throw new FileLoadException("Unable to open file");
                }
                _puzzleSolverClass = (IntPtr)ptr_void;
            }
        }

        /// create a comunicator by creatin an entie new folder
        public Comunicator(uint resX, uint resY, string originPath, string workPath, uint numberOfCores)
        {
            unsafe
            {
                void* ptr_void = DllLib.create_new(resX,resY,workPath,originPath, numberOfCores);
                if (ptr_void == null)
                {
                    throw new FileLoadException("Unable to open file");
                }
                _puzzleSolverClass = (IntPtr)ptr_void;
            }
        }


        // Implement IDisposable
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                // Dispose managed resources here
            }

            // Dispose unmanaged resources (e.g., your C++ allocated memory)
            if (_puzzleSolverClass != IntPtr.Zero)
            {
                Debug.WriteLine("Disposing object");
                unsafe
                {
                    DllLib.free_memory((void*)_puzzleSolverClass);
                }
                _puzzleSolverClass = IntPtr.Zero;
            }
        }

        // Finalizer
        ~Comunicator()
        {
            Dispose(false);
        }
    }

}
