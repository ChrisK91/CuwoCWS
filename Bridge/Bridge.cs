using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Bridge
{
    public class Bridge
    {
        CWSProtocol.Client c;
        ServerService.Database.KnownPlayers knownPlayers;

        public Bridge()
        {
            string directory = Directory.GetCurrentDirectory();
            int wsize = IntPtr.Size;
            string libdir = (wsize == 4) ? "x86" : "x64";
            SetDllDirectory(Path.Combine(directory,"dll", libdir));

            ConsoleWrite("CWSRestart bridge initializes...");
            c = new CWSProtocol.Client("CuWo");

            bool finished = false;

            for (int i = 0; i < 10 && finished == false; i++)
            {
                if (c.Test())
                {
                    ConsoleWrite("Connection to CWSRestart succesful");

                    string playerDb = c.GetPlayersDatabase();

                    if (playerDb != null && c.SetPlayerIdentification(true))
                    {
                        knownPlayers = new ServerService.Database.KnownPlayers(playerDb);
                        knownPlayers.ClearConnectedPlayers();
                    }

                    finished = true;
                }
                else
                {
                    ConsoleWrite("Connection to CWSRestart not possible");
                }
                System.Threading.Thread.Sleep(100);
            }
            ConsoleWrite("Initialization complete!");
        }

        public void PlayerJoined(string name, string ip)
        {
            if (knownPlayers != null)
            {
                knownPlayers.AddConnectedPlayer(ip, name);
                knownPlayers.AddKnownPlayer(ip, name);
            }
        }

        public void PlayerDisconnected(string ip)
        {
            if (knownPlayers != null)
                knownPlayers.RemoveConnectedPlayer(ip);
        }

        private void ConsoleWrite(string text)
        {
            ConsoleWrite(text, null);
        }

        private void ConsoleWrite(string text, params object[] args)
        {
            ConsoleColor oldB = Console.BackgroundColor;
            ConsoleColor oldF = Console.ForegroundColor;

            Console.ForegroundColor = ConsoleColor.DarkRed;
            Console.BackgroundColor = ConsoleColor.Gray;

            Console.WriteLine(text, args);

            Console.ForegroundColor = oldF;
            Console.BackgroundColor = oldB;
        }

        [System.Runtime.InteropServices.DllImport("kernel32.dll", CharSet = System.Runtime.InteropServices.CharSet.Unicode, SetLastError = true)]
        [return: System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.Bool)]
        static extern bool SetDllDirectory(string lpPathName);
    }
}
