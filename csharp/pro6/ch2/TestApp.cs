using System;

// ������Ҫ����
// using System.Windows.Forms;

class TestApp
{
    static void Main()
    {
        Console.WriteLine("���ԣ� 1, 2, 3");

        // ������Ҫ����
        // MessageBox.Show("Hello...");

        // ʹ�� HelloMessage ��
        HelloMessage h = new HelloMessage();
        h.Speak();
    }
}
