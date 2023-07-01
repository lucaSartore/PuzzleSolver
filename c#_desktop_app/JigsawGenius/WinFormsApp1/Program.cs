using System.Diagnostics;
using System.IO;
using System.Text.RegularExpressions;

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

            //Comunicator c = new Comunicator(4, 4, "C:\\Users\\lucas\\Desktop\\raw_test", "C:\\Users\\lucas\\Desktop\\output", 4);
            //return;
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            ApplicationConfiguration.Initialize();
            Application.Run(new MainAppFrom());
        }
    }
}
