using System;
using System.Linq;
using System.Collections.Generic;

public class TestClass
{
    void TestMethod()
    {
        var list = new List<int> { 1, 2, 3, 4, 5 };
        foreach (int num in list)
        {
            Console.WriteLine(num);
        }
    }
}