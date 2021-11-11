using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class MouseLook : EclipseBehavior
{
    public GameObject camera_Obj;

    public float mouseSensitivity = 100f;

    public float minimumXRotation = -90f;
    public float maximumXRotation = 50f;

    //public Transform playerBody;

    float xRotation = 0f;

    // Start is called before the first frame update
    void Start()
    {
        Cursor.lockState = CursorLockMode.Locked;
    }

    // Update is called once per frame
    void Update()
    {
        float mouseX = Input.GetAxis("Mouse X") * mouseSensitivity;
        float mouseY = Input.GetAxis("Mouse Y") * mouseSensitivity;
        //Console.WriteLine("Mouse X = " + mouseX);
        //Console.WriteLine("Mouse Y = " + mouseY);
        // Rotate Camera by the X-Axis
        xRotation -= mouseY;

        // Clamps the Camera's X Rotation
        if (xRotation < minimumXRotation)
        {
            xRotation = minimumXRotation;
        }
        else if (xRotation > maximumXRotation)
        {
            xRotation = maximumXRotation;
        }

        //xRotation = Mathf.Clamp(xRotation, minimumXRotation, maximumXRotation);

        //transform.localRotation = Quaternion.Euler(xRotation, 0f, 0f);
        transform.Rotate(new Vector3(0f, xRotation, 0f));
        //playerBody.Rotate(Vector3.up * mouseX);

        // Unlock Mouse cursor
        if (Input.GetKey(KeyCode.H)) 
        {
            Cursor.lockState = CursorLockMode.None;                     
        }

        //  Lock Mouse Cursor
        if (Input.GetKey(KeyCode.L))
        {
            Cursor.lockState = CursorLockMode.Locked;
        }        
    }
}