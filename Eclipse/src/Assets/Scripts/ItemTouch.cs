/*using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class ItemTouch : EclipseBehavior
{
    public float mouseSensitivity = 500f;

    private float noiseVolume = 0;

    private bool isPickedUp = false;

    Rigidbody rb;
    //AudioSource audioSource;

    private void Start()
    {
        rb = GetComponent<Rigidbody>();
        //audioSource = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        if (isPickedUp == true)
        {
            float mouseX = Input.GetAxis("Mouse X") * mouseSensitivity;
            float mouseY = Input.GetAxis("Mouse Y") * mouseSensitivity;

            transform.Rotate(new Vector3(0f, -mouseX, 0f));
            transform.Rotate(new Vector3(mouseY, 0f, 0f));
        }
    }

    public bool PickUpItem
    {
        get { return isPickedUp; }
        set { isPickedUp = value; }
    }

    //private void OnCollisionEnter(Collision collision)
    //{
    //    if (collision.gameObject.CompareTag("Floor") || collision.gameObject.CompareTag("Wall")) // Collide only with surfaces
    //    {
    //        //CreateNoise(); // Create noise when this object collides with a surface
    //    }
    //}

    //void CreateNoise()
    //{
    //    if (rb.velocity.magnitude >= 0.1f) // Velocity of object has to be high enough to generate noise
    //    {
    //        audioSource.volume = rb.velocity.magnitude; // Create Noise
    //        audioSource.Play();

    //        noiseVolume = audioSource.volume * 30;
    //        //print(noiseVolume);
    //        GameObject enemy = GameObject.Find("Monster");

    //        float distanceToEnemy = Vector3.Distance(transform.position, enemy.transform.position);
    //        //print(distanceToEnemy);

    //        if (distanceToEnemy <= noiseVolume)
    //        {
    //            enemy.GetComponent<Monster>().AlertMonster(transform.position);
    //        }
    //    }
    //}
}*/