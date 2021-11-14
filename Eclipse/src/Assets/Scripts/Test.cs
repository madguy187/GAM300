using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public GameObject obj;

    private GameObject pickedObj;
    bool isPicked = false;

    public void Start()
    {
    }

    public void Update()
    {
        if (Input.GetKey(KeyCode.W))
        {
          Vector3 temp = transform.position;
          transform.position = transform.position + transform.forward * 0.1f;
        }
        if (Input.GetKey(KeyCode.S))
        {
          Vector3 temp = transform.position;
          transform.position = transform.position - transform.forward * 0.1f;
        }
        if (Input.GetKey(KeyCode.D))
        {
          transform.Rotate(new Vector3(0, 2, 0));
        }
        if (Input.GetKey(KeyCode.A))
        {
          transform.Rotate(new Vector3(0, -2, 0));
        }

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

        
    }
}