using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    [Header("Weeeeeee")]
    public UInt32 test;

    public void Start()
    {
        Console.WriteLine("C# Start");
    }

    public void Update()
    {
        Console.WriteLine("C# Update");
        Console.WriteLine(Vector3.zero.x);
    }
}