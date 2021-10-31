using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public RigidBody rigid;

    public void Start()
    {
        Console.WriteLine("C# Start");
        rigid = GetComponent<RigidBody>();
    }

    public void Update()
    {
        Console.WriteLine("C# Update");
        if (Input.GetButtonDown("Right"))
        {
            rigid.AddForce(500, 0, 0);
        }
        else
        {
            rigid.AddForce(-500, 0, 0);
        }
    }
}