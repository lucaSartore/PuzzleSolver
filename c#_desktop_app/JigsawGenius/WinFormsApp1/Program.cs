using System.Diagnostics;
using System.IO;

namespace JigsawGenius
{
    internal static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {

            //Comunicator c = new Comunicator("C:\\Users\\lucas\\CLionProjects\\Puzzle_Solver\\cpp_lib\\work_path\\status.txt");
            //return;
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            ApplicationConfiguration.Initialize();
            Application.Run(new MainAppFrom());
        }
    }
}
