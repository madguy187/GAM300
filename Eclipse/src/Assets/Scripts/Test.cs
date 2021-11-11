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
        if (Input.GetKey(KeyCode.Space))
          transform.position = transform.position + transform.left;
    }
}