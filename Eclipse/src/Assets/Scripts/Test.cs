using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public Light light;

    public void Start()
    {
    }

    public void Update()
    {
        Console.WriteLine(light.Entity);
    }
}