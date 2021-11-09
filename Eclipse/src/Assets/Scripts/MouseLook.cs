using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class MouseLook : EclipseBehavior
{
    public float mouseSensitivity = 100f;

    public float minimumXRotation = -90;
    public float maximumXRotation = 50f;

    public Transform playerBody;

    float xRotation = 0f;

    public void Start()
    {
        audioSource = GetComponent<AudioSource>();
    }

    public void Update()
    {
        float mouseX = Input.GetAxis("Mouse X") * mouseSensitivity;
        float mouseY = Input.GetAxis("Mouse Y") * mouseSensitivity;

        xRotation -= mouseY;
        xRotation = Mathf.Clamp(xRotation, minimumXRotation, maximumXRotation);

        transform.localRotation = Quaternion.Euler(xRotation, 0f, 0f);
        playerBody.Rotate(Vector3.up * mouseX);
    }
}