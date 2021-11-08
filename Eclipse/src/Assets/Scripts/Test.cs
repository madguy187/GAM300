using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public void Start()
    {
        Console.WriteLine("TEST C# Start");
    }

    public void Update()
    {
        RayCastHit hit;
        if(Physics.Raycast(new Vector3(0, 0, 0), new Vector3(-1, 0, 0), out hit))
        {
            Console.WriteLine("HIT");
            Console.WriteLine(hit.point.x);
            Console.WriteLine(hit.point.y);
            Console.WriteLine(hit.point.z);
        }
    }
}