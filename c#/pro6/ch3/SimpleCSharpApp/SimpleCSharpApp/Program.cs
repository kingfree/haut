using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimpleCSharpApp
{
    class Program
    {
        static int Main(string[] args)
        {
            Console.WriteLine("***** C# 控制台应用 *****");
            Console.WriteLine("Hello, 世界");
            Console.WriteLine();

            for (int i = 0; i < args.Length; i++)
                Console.WriteLine("参数 {0}: {1}", i, args[i]);
            
            foreach (string arg in args)
                Console.WriteLine("参数: {0}", arg);

            Console.ReadLine();
            return 0;
        }
    }
}
