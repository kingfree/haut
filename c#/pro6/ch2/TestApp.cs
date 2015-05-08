using System;

// 不再需要这行
// using System.Windows.Forms;

class TestApp
{
    static void Main()
    {
        Console.WriteLine("测试！ 1, 2, 3");

        // 不再需要这行
        // MessageBox.Show("Hello...");

        // 使用 HelloMessage 类
        HelloMessage h = new HelloMessage();
        h.Speak();
    }
}
