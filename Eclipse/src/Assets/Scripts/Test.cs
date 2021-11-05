using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    [Header("HeaderName")]
    public UInt32 VarName;
    private UInt32 wee;

    public void Start()
    {
        Console.WriteLine("C# Start");
    }

    public void Update()
    {
        Console.WriteLine("C# Update");
    }
}