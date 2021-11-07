using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public float mouseSensitivity = 10f;

    public void Start()
    {
        Console.WriteLine("C# Start");
        Invoke("Tests", 2.0f);
    }

    public void Update()
    {
      
    }

    public void Tests()
    {
        Console.WriteLine("YAYY");
    }
}