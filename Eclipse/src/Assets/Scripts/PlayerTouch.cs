/*using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class PlayerTouch : EclipseBehavior
{
    [Header("Values")]
    public float throwForce = 50f;
    public float pickUpRange = 5f;
    public float touchRange = 1.5f;
    public float touchDuration = 1.5f;
    public float touchOffset = 0.05f;

    [Header("Object Interactions")]
    public GameObject touchedObject;
    public GameObject pickedObject;

    [Header("Lighting")]
    //public Light aoeLight;
    //public Light touchLight;
    public float aoeLightMaxIntensity;

    //[Header("Audio")]
    //public AudioClip touchSFX;

    [Header("Detection Masks")]
    public LayerMask touchMask;
    public LayerMask itemMask;
    public LayerMask interactMask;
    public LayerMask highlightMask;

    [Header("Sprites")]
    public GameObject handCursor;
    public GameObject handPrintDecal;

    Rigidbody pickedObject_rb = null;

    //AudioSource audioSource;

    private void Start()
    {
        //audioSource = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        if (pickedObject != null)
        {
            pickedObject.transform.position = transform.position - new Vector3(0, 0.1f, 0) + transform.TransformDirection(Vector3.forward);

            if (Input.GetKeyDown(KeyCode.Mouse0)) //touch item
            {
               // touchLight.enabled = true;
                //pickedObject_rb.freezeRotation = false;

                gameObject.GetComponent<MouseLook>().enabled = false;
                pickedObject.GetComponent<ItemTouch>().PickUpItem = true;

                //audioSource.PlayOneShot(touchSFX); // Plays touch SFX
            }
            else if (Input.GetKeyUp(KeyCode.Mouse0)) //if not touching
            {
               // touchLight.enabled = false;
                //pickedObject_rb.freezeRotation = true;                

                gameObject.GetComponent<MouseLook>().enabled = true;
                pickedObject.GetComponent<ItemTouch>().PickUpItem = false;
            }

            if (Input.GetKeyDown(KeyCode.E)) //let go of item
            {
               // touchLight.enabled = false;

                //pickedObject_rb.freezeRotation = false;
                pickedObject_rb.velocity = Vector3.zero;
                pickedObject_rb.AddForce(transform.TransformDirection(Vector3.forward) * throwForce * Time.deltaTime);

                gameObject.GetComponent<MouseLook>().enabled = true;
                pickedObject.GetComponent<ItemTouch>().PickUpItem = false;

                pickedObject_rb = null;
                pickedObject = null;
            }
        }

        else if (pickedObject == null)
        {
            //Highlight_Object();

            if (Input.GetKeyDown(KeyCode.Mouse0))
            {
                //if (handCursor.activeInHierarchy)
                //{
                //    handCursor.SetActive(false);
                //}

                if (Touch())
                {
                    //Highlight_Object(); // Highlights interactable objects
                }
                else
                {
                    //Stop_Highlight_Object(); // Turn off last touched object                 
                }
            }
            else
            {
                //if (!handCursor.activeInHierarchy)
                //{
                //    handCursor.SetActive(true);
                //}
            }

            if (Input.GetKeyDown(KeyCode.E))
            {
                PickUp();

                Interact(); // For activating certain interactables (e.g. Buttons, Doors, etc)
            }
        }
    }

    public void PickUp()
    {
        RaycastHit hit;

        if (Physics.Raycast(transform.position, transform.forward, out hit, pickUpRange, itemMask))
        {
            if (hit.point != null)
            {
                //Debug.Log(hit.transform.gameObject.name);
                pickedObject = hit.transform.gameObject;
                pickedObject_rb = hit.transform.gameObject.GetComponent<Rigidbody>();
                //pickedObject_rb.freezeRotation = true;

                //Stop_Highlight_Object(); // Turn off last touched object
            }
        }
    }

    public void Interact() // Only for Interactables that require an action input from the player (buttons, doors, etc)
    {
        RayCastHit hit;

        if (Physics.Raycast(transform.position, transform.forward, out hit, pickUpRange, interactMask))
        {
            if (hit.point != null)
            {
                //Debug.Log(hit.transform.gameObject.name);

                //hit.transform.gameObject.GetComponent<Interactable>().Activate();
            }
        }
    }

    public bool Touch()
    {
        RayCastHit hit;

        if (Physics.Raycast(transform.position, transform.forward, out hit, touchRange, touchMask))
        {
            if (hit.point != null)
            {
                //Debug.Log(hit.transform.gameObject.name);

                //GameObject handPrint = Instantiate(handPrintDecal, hit.point, Quaternion.identity);

                //handPrint.transform.forward = hit.normal;
                //handPrint.transform.position += handPrint.transform.forward.normalized * touchOffset;

                //audioSource.PlayOneShot(touchSFX); // Plays touch SFX

                //Quaternion followRotation = Quaternion.LookRotation(transform.forward);
                //Quaternion followRotation = Quaternion.LookRotation(handPrint.transform.position - transform.position);
                //Quaternion handRotation   = handPrint.transform.rotation;
                ////handRotation.x = handPrint.transform.rotation.x;
                ////handRotation.y = handPrint.transform.rotation.y;
                ////handRotation.z = handPrint.transform.rotation.z;
                ////handRotation.z = followRotation.z;
                //handPrint.transform.rotation = followRotation;                

                return true;
            }

            return false;
        }

        return false;
    }
}*/