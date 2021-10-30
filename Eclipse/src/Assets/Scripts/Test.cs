using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public RigidBody rigid;

    public void Start()
    {
        Console.WriteLine("C# Update");
        rigid = GetComponent<RigidBody>();
    }

    public void Update()
    {
        rigid.AddForce(500, 0, 0);
    }
}