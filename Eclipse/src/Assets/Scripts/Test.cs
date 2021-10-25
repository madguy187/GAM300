using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public void Update()
    {
        Console.WriteLine("C# Update");
        GetRigidComponent();
        //rigid = GetComponent<RigidBodyComponent>();
    }
}