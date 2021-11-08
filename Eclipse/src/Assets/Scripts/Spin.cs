using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    //Test2 wee;

    public void Start()
    {
        //Console.WriteLine("TEST C# Start");
        //wee = GetComponent<Test2>();
        //Console.WriteLine(wee.Entity);
    }

    public void Update()
    {
        if (Input.GetKey(KeyCode.D))
            transform.Rotate(new Vector3(0, 5, 0));

        if (Input.GetKey(KeyCode.A))
            transform.Rotate(new Vector3(0, -5, 0));
    }
}