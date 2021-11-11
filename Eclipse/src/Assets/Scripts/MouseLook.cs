using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class MouseLook : EclipseBehavior
{
    public float mouseSensitivity = 100f;

    public float minimumXRotation = -90f;
    public float maximumXRotation = 50f;

    //public Transform playerBody;

    float xRotation = 0f;

    private bool isMouseLocked = false;

    // Start is called before the first frame update
    void Start()
    {
        isMouseLocked = true;
        Cursor.lockState = CursorLockMode.Locked;
    }

    // Update is called once per frame
    void Update()
    {
        //float mouseX = Input.GetAxis("Mouse X") * mouseSensitivity;
        float mouseY = Input.GetAxis("Mouse Y") * mouseSensitivity;

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
        transform.Rotate(new Vector3(xRotation, 0f, 0f));
        //playerBody.Rotate(Vector3.up * mouseX);

        // Unlock Mouse cursor
        if (Input.GetKey(KeyCode.L)) 
        {
            Cursor.lockState = CursorLockMode.None;                     
        }

        //  Lock Mouse Cursor
        if (Input.GetKey(KeyCode.H))
        {
            Cursor.lockState = CursorLockMode.Locked;
        }        
    }
}