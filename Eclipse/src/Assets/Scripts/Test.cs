using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    private RigidBodyComponent rigid;

    static public void Type()
    {
        Console.WriteLine("a Dog!");
    }
    public void Bark()
    {
        Console.WriteLine("bark!");
    }
    public void Bark(int times)
    {
        for (var i = 0; i < times; ++i )
            Console.WriteLine("bark!");
    }

    public void Update()
    {
        Console.WriteLine("C# Update");
        //rigid = GetComponent<RigidBodyComponent>();
        GetRigidComponent();
    }
}