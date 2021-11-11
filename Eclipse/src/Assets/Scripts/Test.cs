using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public GameObject obj;
    bool isLocked = false;

    public void Start()
    {
    }

    public void Update()
    {
        if (Input.GetKey(KeyCode.A))
        {
            if (isLocked == false)
            {
                isLocked = true;
                Cursor.lockState = CursorLockMode.Locked;
            }
            else
            {
                isLocked = false;
                Cursor.lockState = CursorLockMode.None;
            }
        }
    }
}