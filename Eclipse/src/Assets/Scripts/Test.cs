using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public GameObject obj;
    public GameObject camera_obj;

    private GameObject pickedObj;
    bool isPicked = false;

    public void Start()
    {
        Input.LockCamera(1);
    }

    public void Update()
    {
		// Console.WriteLine("Updating movement");
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
        // if (Input.GetKey(KeyCode.D))
        // {
        //   transform.Rotate(new Vector3(0, 2, 0));
        // }
        // if (Input.GetKey(KeyCode.A))
        // {
        //   transform.Rotate(new Vector3(0, -2, 0));
        // }

        if (Input.GetKeyDown(KeyCode.E))
        {
            if (!isPicked)
            {
              RaycastHit hit;

              if (Physics.Raycast(obj.transform.position, transform.forward, out hit, 5.0f))
              {
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
        tempVec.y += 2.0f;
        camera_obj.transform.position = tempVec;

        transform.rotation = Quaternion.identity;
    }

    public void Terminate()
    {
        Input.LockCamera(0);
    }
}