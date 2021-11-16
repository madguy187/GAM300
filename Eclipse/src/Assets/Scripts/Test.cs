using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
	public GameObject hand;
		
    public GameObject obj;
    public GameObject camera_obj;
    public LayerMask mask;

    private GameObject pickedObj;
    bool isPicked = false;

    public void Start()
    {
        Input.LockCamera(1);
    }

    public void Update()
    {
        if (Input.GetKey(KeyCode.W))
        {
		  // Console.WriteLine("Moving Forward");	
          Vector3 temp = transform.position;
          transform.position = transform.position + transform.forward * 0.1f;
        }
        if (Input.GetKey(KeyCode.S))
        {
          Vector3 temp = transform.position;
          transform.position = transform.position - transform.forward * 0.1f;
        }

        if (Input.GetKeyDown(KeyCode.E))
        {
            if (!isPicked)
            {
              RaycastHit hit;

              if (Physics.Raycast(obj.transform.position, transform.forward, out hit, 5.0f, mask.mask))
              {
                Console.WriteLine(hit.Entity);
                pickedObj = new GameObject(hit.Entity, "");

                isPicked = true;
                pickedObj.GetComponent<Rigidbody>().Gravity = false;
              }
            }
            else
            {
              isPicked = false;
              pickedObj.GetComponent<Rigidbody>().Gravity = true;
            }
        }

        if (isPicked)
        {
          if(pickedObj != null)
          {
             Vector3 newVec = transform.forward * 3.0f;
             pickedObj.transform.position = obj.transform.position + newVec;
          }
        }

        Vector3 tempVec = transform.position;
        tempVec.y += 1.0f;
        camera_obj.transform.position = tempVec;
		
		tempVec = transform.position + transform.forward * 4.0f;
		tempVec.y += 0.5f;
		
		hand.transform.position = tempVec;
		//hand.transform.rotation = camera_obj.transform.rotation;

        transform.rotation = Quaternion.identity;
    }

    public void Terminate()
    {
        Input.LockCamera(0);
    }
}