using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public UInt32 intTest;

    public void Start()
    {
        Vector2 test = new Vector2(5, 6);
        Console.WriteLine(test.magnitude);
    }

    public void Update()
    {
        if (Input.GetKey(KeyCode.D))
        {
            Console.WriteLine("spinnn");
            transform.Rotate(new Vector3(0, 10, 0));
        }

        if (Input.GetKey(KeyCode.A))
        {
            Console.WriteLine("stop spinnn");
            gameObject.enabled = 0;
        }
    }
}