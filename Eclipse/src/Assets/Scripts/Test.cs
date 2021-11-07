using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public void Start()
    {
        Console.WriteLine("C# Update");
        Vector3 test = new Vector3(5, 5, 5);
        Vector3 test2 = new Vector3(0, 1, 0);
        Console.WriteLine(test * test2);
    }

    public void Update()
    {
        Console.WriteLine("C# Update");
    }
}