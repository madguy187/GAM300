using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class SpawnDoctor : EclipseBehavior
{
    private GameObject obj;
    bool isCreated = false;

    public void OnCollision()
    {
      if (!isCreated)
      {
        Console.WriteLine("CREATING PREFAB");
        Vector3 temp = transform.position;
        Vector3 rot = new Vector3(0.0f, 90.0f, 0.0f);
        obj = CreatePrefab("DoctorIslami", temp, rot);
        isCreated = true;
      }
    }

    public void Start()
    {
    }

    public void FixedUpdate()
    {
        if (obj != null)
        {
          Vector3 temp = obj.transform.position;
          temp.x += 0.1f;
          obj.transform.position = temp;
        }
        /*if (Input.GetKey(KeyCode.W))
        {
          Vector3 temp = transform.position;
          transform.position = transform.position + transform.forward * 0.3f;
        }
        if (Input.GetKey(KeyCode.S))
        {
          Vector3 temp = transform.position;
          transform.position = transform.position - transform.forward * 0.3f;
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
        }*/    
    }
}