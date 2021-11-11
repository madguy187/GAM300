using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class PlayerMovement : EclipseBehavior
{
    [Header("Stats")]
    public float moveSpeed = 10f;
    //public float moveSpeedClamp = 3f;
    //public float jumpForce = 16f;

    //[Header("Footprints")]
    //public GameObject leftFootprint;
    //public GameObject rightFootprint;
    //public Transform leftFootLocation;
    //public Transform rightFootLocation;

    //public float footprintOffset = 0.05f;

    //public AudioSource leftFootAudioSource;
    //public AudioSource rightFootAudioSource;

    //public float footprintSpawnRate;
    //private float footprintTimer;

    //private bool switchFootprint;

    //[Header("Ground Check")]
    //public Transform groundCheck;
    //public float groundDistance = 0.4f;
    //public LayerMask groundMask;

    //Vector3 moveForce = Vector3.zero;

    private float horizontalInput = 0;
    private float verticalInput = 0;

    //Rigidbody rb;
    //bool isGrounded;

    // Start is called before the first frame update
    void Start()
    {
        //rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        //isGrounded = Physics.CheckSphere(groundCheck.position, groundDistance, groundMask);

        //// Jumping
        //Jump();

        // Controls
        //if (Input.GetKey(KeyCode.A)) //|| Input.GetKey(KeyCode.LeftArrow))
        //{
        //    //transform.position -= new vector3(movespeed * time.deltatime, 0, 0);
        //}

        //if (Input.GetKey(KeyCode.D))// || Input.GetKey(KeyCode.RightArrow))
        //{
        //    //transform.position += new vector3(movespeed * time.deltatime, 0, 0);
        //}

        //if (Input.GetKey(KeyCode.W))// || Input.GetKey(KeyCode.UpArrow))
        //{
        //    //transform.position -= new vector3(movespeed * time.deltatime, 0, 0);
        //}

        //if (Input.GetKey(KeyCode.S))// || Input.GetKey(KeyCode.DownArrow))
        //{
        //    //transform.position += new vector3(movespeed * time.deltatime, 0, 0);
        //}

        horizontalInput = Input.GetAxisRaw("Horizontal");
        verticalInput = Input.GetAxisRaw("Vertical");

        transform.position = transform.position + new Vector3(horizontalInput * moveSpeed * Time.deltaTime, 0, verticalInput * moveSpeed * Time.deltaTime);

        //// Movement        
        //moveForce = (transform.right * horizontal + transform.forward * vertical) * moveSpeed;

        //// Footprints
        //footprintTimer -= Time.deltaTime;

        //if (horizontal + vertical != 0)
        //{
        //    if (footprintTimer <= 0)
        //    {
        //        if (!switchFootprint)
        //        {
        //            switchFootprint = true;

        //            LeftFootStep();
        //        }
        //        else if (switchFootprint)
        //        {
        //            switchFootprint = false;

        //            RightFootStep();
        //        }

        //        footprintTimer = footprintSpawnRate;
        //    }
        //}                              
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
    }
}