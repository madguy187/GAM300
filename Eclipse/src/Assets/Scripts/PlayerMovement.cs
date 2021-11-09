using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class PlayerMovement : EclipseBehavior
{
    [Header("Stats")]
    public float moveSpeed = 10f;
    public float moveSpeedClamp = 3f;
    public float jumpForce = 16f;

    [Header("Footprints")]
    public GameObject leftFootprint;
    public GameObject rightFootprint;
    public Transform leftFootLocation;
    public Transform rightFootLocation;

    public float footprintOffset = 0.05f;

    public AudioSource leftFootAudioSource;
    public AudioSource rightFootAudioSource;

    public float footprintSpawnRate;
    private float footprintTimer;

    private bool switchFootprint;

    [Header("Ground Check")]
    public Transform groundCheck;
    public float groundDistance = 0.4f;
    public LayerMask groundMask;

    Vector3 moveForce = Vector3.zero;

    private float horizontalInput;
    private float verticalInput;

    Rigidbody rb;
    bool isGrounded;

    public void Start()
    {
        rb = GetComponent<Rigidbody>();
    }

    public void Update()
    {
        // Keyboard Controls
        horizontalInput = Input.GetAxisRaw("Horizontal");
        verticalInput = Input.GetAxisRaw("Vertical");
    }

    private void FixedUpdate()
    {
        // Move Speed Clamp
        //if (new Vector2(rb.velocity.x, rb.velocity.z).magnitude >= moveSpeedClamp)
        //{
        //    moveForce.x = 0;
        //    moveForce.z = 0;
        //}

        //if (rb.velocity.x != 0 && horizontal == 0 && rb.velocity.z != 0 && vertical == 0)
        //{
        //    float Fx = -rb.velocity.x * rb.mass / Time.fixedDeltaTime;
        //    moveForce.x = Fx;

        //    float Fz = -rb.velocity.z * rb.mass / Time.fixedDeltaTime;
        //    moveForce.z = Fz;
        //}

        //// Move the character
        //rb.AddForce(moveForce);
        //moveForce = Vector3.zero;

        // Movement
        transform.position = transform.position + new Vector3(horizontalInput * moveSpeed * Time.deltaTime, verticalInput * moveSpeed * Time.deltaTime, 0);
    }
}