// Calc.cs
using System;

namespace CalculatorExample
{
    // ��������Ӧ�ó������ڵ�
    class Program
    {
        static void Main()
        {
            Calc c = new Calc();
            int ans = c.Add(10, 84);
            Console.WriteLine("10 + 84 = {0}.", ans);

            // �ȴ��û�����س�����������
            Console.ReadLine();
        }
    }

    // C# ������
    class Calc
    {
        public int Add(int x, int y)
        {
            return x + y;
        }
    }
}
