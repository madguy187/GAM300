using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public GameObject obj;

    public void Start()
    {
    }

    public void Update()
    {
        Console.WriteLine(obj.Entity);
    }
}