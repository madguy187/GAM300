using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public float mouseSensitivity = 10f;

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
            float mouseX = Input.GetAxis("Horizontal") * mouseSensitivity;
            transform.Rotate(new Vector3(0, mouseX, 0));
        }

        if (Input.GetKey(KeyCode.A))
        {
            Console.WriteLine("stop spinnn");
            float mouseX = Input.GetAxis("Horizontal") * mouseSensitivity;
            transform.Rotate(new Vector3(0, -mouseX, 0));
        }
    }
}