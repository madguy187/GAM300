using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public float mouseSensitivity = 10f;

    Test2 wee;

    public void Start()
    {
        Console.WriteLine("TEST C# Start");
        wee = GetComponent<Test2>();
        Console.WriteLine(wee.Entity);
    }

    public void Update()
    {
      
    }
}