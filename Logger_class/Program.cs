using System;
using System.IO;

namespace Logger_class
{
    class Logger : IDisposable
    {
        StreamWriter wrt;
        public Logger(string path)
        {
            try
            {
                FileInfo f_info = new FileInfo(path);
                wrt = new StreamWriter(path);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Сatch 0", ex.Message);
                throw ex;
            }
        }

        ~Logger()
        {
            wrt.Close();
            wrt.Dispose();
        }

        public bool Log(string data, severity sev)
        {
            wrt.WriteLine($"[{DateTime.Now}][{SVtostr(sev)}]: {data} ");
            return true;
        }

        public void Dispose()
        {
            wrt.Close();
            wrt.Dispose();
        }

        public string SVtostr(severity sev)
        {
            string str;
            int tmp = Convert.ToInt32(sev);
            switch (tmp)
            {
                case 0: str = "Trace"; break;
                case 1: str = "Debug"; break;
                case 2: str = "Information"; break;
                case 3: str = "Warning"; break;
                case 4: str = "Error"; break;
                case 5: str = "Critical"; break;
                default: str = "Is there something wrong"; break;
            }
            return str;
        }
    }

    public enum severity { Trace, Debug, Information, Warning, Error, Critical };

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Enter the path to the file: ");
            string path = Console.ReadLine();
            Console.WriteLine($"Introduced path: {path}");

            if (!File.Exists(path))
            {
                Console.WriteLine($"There is no file at the specified path: {path}\nA new file will be created!");
                try
                {
                    File.Create(path);
                }
                catch (AggregateException ex)
                {
                    Console.WriteLine("Error: ", ex.Message);
                }
            }

            Logger log = new Logger(path);
            log.Log("Something is written", severity.Trace);
            log.Dispose();
        }
    }
}
