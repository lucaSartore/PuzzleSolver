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

            Comunicator c = new Comunicator("c:\\users\\lucas\\clionprojects\\puzzle_solver\\cpp_lib\\work_path\\");
            return;
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            ApplicationConfiguration.Initialize();
            Application.Run(new MainAppFrom());
        }
    }
}
