using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using static System.Windows.Forms.AxHost;
using System.Diagnostics;
using System.Drawing.Configuration;
using JigsawGenius;
using System.Drawing;
using System.Windows.Forms;
using System.Reflection.Metadata.Ecma335;
using static JigsawGenius.DllLib;

namespace JigsawGenius
{

    public class UnknownDllLibrartError: Exception
    {
        public UnknownDllLibrartError() { }
    }


    internal class DllLib
    {
        public DllLib()
        {
        }

        [StructLayout(LayoutKind.Sequential)]
        public unsafe struct PngImagePointer{

            private Byte* _data;
            private int _len;
            // Add other fields as needed

            // tansform the image into a 
            public System.Drawing.Image ToImage(){
                Image image;

                if (_data == null)
                {
                    throw new UnknownDllLibrartError();
                }
                // copy memory to safe array
                byte[] safeData = new byte[_len];
                Marshal.Copy((IntPtr)_data, safeData, 0, _len);

                // converting the image into a readable form
                using (MemoryStream memoryStream = new MemoryStream(safeData)){
                    // Load the image from the memory stream
                    image = Image.FromStream(memoryStream);
                }

                return image;
            }

            public bool IsNull()
            {
                return _data == null;
            }
        }


        // in order for this to work you must add the path to de DLL to your system path
        // in my case is: C:\Users\lucas\CLionProjects\Puzzle_Solver\cpp_lib\cmake-build-debug
        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe void* open_folder([MarshalAs(UnmanagedType.LPStr)] string path);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe void* create_new(uint dim_x, uint dim_y, [MarshalAs(UnmanagedType.LPStr)] string work_path, [MarshalAs(UnmanagedType.LPStr)] string origin_path, uint number_of_cores);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe void save_as(void* puzzle_solver, [MarshalAs(UnmanagedType.LPStr)] string new_path);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe void free_memory(void* puzzle_solver);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe void set_split_threshold(void* puzzle_solver, int new_threshold);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern void enable_preview();

        [DllImport("PuzzleSolverLib.dll")]
        public static extern void disable_preview();

        [DllImport("PuzzleSolverLib.dll")]
        public static extern PngImagePointer next_preview_image(float max_wait_time);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe PngImagePointer get_threshold_preview(void* puzzle_solver);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe int get_state(void* puzzle_solver);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe int split_image(void* puzzle_solver);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe int process_corners(void* puzzle_solver);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe int calculate_connections(void* puzzle_solver);

        [DllImport("PuzzleSolverLib.dll")]
        public static extern unsafe int solve_puzzle(void* puzzle_solver);
    }



    public class Comunicator : IDisposable
    {
        // pointer to the c++ class
        private IntPtr _puzzleSolverClass;

        /// create a communicator by opening a folder with the saved prooject
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

        /// create a communicator by creating an empty new folder
        public Comunicator(uint resX, uint resY, string originPath, string workPath, uint numberOfCores)
        {
            unsafe
            {
                var ptr_void = DllLib.create_new(resX, resY, workPath, originPath, numberOfCores);
                if (ptr_void == null)
                {
                    throw new FileLoadException("Unable to open file");
                }
                _puzzleSolverClass = (IntPtr)ptr_void;
            }
        }

        public void SetSplitThreshold(int treshold)
        {
            unsafe
            {
                void* ptr_void = (void*)_puzzleSolverClass;
                if (ptr_void == null)
                {
                    throw new NullReferenceException("c++ library is not loaded");
                }
                try
                {
                    DllLib.set_split_threshold(ptr_void, treshold);
                }
                catch (Exception)
                {
                    throw new UnknownDllLibrartError();
                }
            }
        }


        public System.Drawing.Image GetThresholdPreview()
        {
            unsafe
            {
                DllLib.PngImagePointer image;

                void* ptr_void = (void*)_puzzleSolverClass;
                if (ptr_void == null)
                {
                    throw new NullReferenceException("c++ library is not loaded");
                }
                try
                {
                    image = DllLib.get_threshold_preview(ptr_void);
                }
                catch (Exception)
                {
                    throw new UnknownDllLibrartError();
                }

                return image.ToImage();
            }
        }

        public State GetState()
        {
            unsafe
            {
                void* ptr_void = (void*)_puzzleSolverClass;
                if (ptr_void == null)
                {
                    throw new NullReferenceException("c++ library is not loaded");
                }
                try
                {
                    var state = DllLib.get_state(ptr_void);
                    return state switch
                    {
                        0 => State.PieceSplitting,
                        1 => State.CornerProcessing,
                        2 => State.ConnectionProcessing,
                        3 => State.CombinationFinding,
                        4 => State.Helping,
                        _ => throw new UnknownDllLibrartError(),
                    };
                }
                catch (Exception)
                {
                    throw new UnknownDllLibrartError();
                }

            }
        }

        public int SplitImage()
        {
            unsafe
            {
                int return_code;

                var ptr_void = (void*)_puzzleSolverClass;
                if (ptr_void == null)
                {
                    throw new NullReferenceException("c++ library is not loaded");
                }
                try
                {
                    return_code = DllLib.split_image(ptr_void);
                }
                catch (Exception)
                {
                    throw new UnknownDllLibrartError();
                }

                return return_code;
            }
        }

        public int ProcessCorners()
        {
            unsafe
            {
                int return_code;

                var ptr_void = (void*)_puzzleSolverClass;
                if (ptr_void == null)
                {
                    throw new NullReferenceException("c++ library is not loaded");
                }
                try
                {
                    return_code = DllLib.process_corners(ptr_void);
                }
                catch (Exception)
                {
                    throw new UnknownDllLibrartError();
                }

                return return_code;
            }
        }

        public int CalculateConnections()
        {
            unsafe
            {
                int return_code;

                var ptr_void = (void*)_puzzleSolverClass;
                if (ptr_void == null)
                {
                    throw new NullReferenceException("c++ library is not loaded");
                }
                try
                {
                    return_code = DllLib.calculate_connections(ptr_void);
                }
                catch (Exception)
                {
                    throw new UnknownDllLibrartError();
                }

                return return_code;
            }
        }


        public int SolvePuzzle()
        {
            unsafe
            {
                int return_code;

                var ptr_void = (void*)_puzzleSolverClass;
                if (ptr_void == null)
                {
                    throw new NullReferenceException("c++ library is not loaded");
                }
                try
                {
                    return_code = DllLib.solve_puzzle(ptr_void);
                }
                catch (Exception)
                {
                    throw new UnknownDllLibrartError();
                }

                return return_code;
            }
        }

        public static void EnablePreview() => DllLib.enable_preview();

        public static void DisablePreview() => DllLib.disable_preview();

        public Image? GetNextPreviewImage()
        {
            var image = DllLib.next_preview_image((float)0.1);

            if (!image.IsNull())
            {
                return image.ToImage();
            }
            return null;
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
            // I dont't whant it to be called automaticly, since the comunication might be destroyed if i pass
            // a controll to a sub struct
            //Dispose(false);
        }
    }

}
