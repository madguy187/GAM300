using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
	Public Light light; 
	
    public void Start()
    {
    }

    public void Update()
    {
        if (Input.GetKey(KeyCode.D))
        {
            Quaternion rot = transform.rotation;
            rot.x += (0.01f * Time.deltaTime);
            transform.rotation = rot;
        }

        if (Input.GetKey(KeyCode.A))
        {
            Quaternion rot = transform.rotation;
            rot.x -= (0.01f * Time.deltaTime);
            transform.rotation = rot;
        }

        if (Input.GetKey(KeyCode.Space))
        {
            transform.rotation = Quaternion.identity;
        }

        Console.WriteLine(transform.rotation);
    }
}