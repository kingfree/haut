// Calc.cs
using System;

namespace CalculatorExample
{
    // 这个类包含应用程序的入口点
    class Program
    {
        static void Main()
        {
            Calc c = new Calc();
            int ans = c.Add(10, 84);
            Console.WriteLine("10 + 84 = {0}.", ans);

            // 等待用户输入回车来结束程序
            Console.ReadLine();
        }
    }

    // C# 计算器
    class Calc
    {
        public int Add(int x, int y)
        {
            return x + y;
        }
    }
}
