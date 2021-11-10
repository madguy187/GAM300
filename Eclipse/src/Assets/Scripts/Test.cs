using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public void Start()
    {
    }

    public void Update()
    {
        Vector3 a = new Vector3(1, 1, 1);
        Vector3 b = new Vector3(2, 2, 2);

        Console.WriteLine(Vector3.Distance(a, b));
        Console.WriteLine(a * 3.0f);
    }
}